/**
 * This file is part of Asuc.
 *
 * Copyright 2010 Sven Oliver Moll
 * Copyright 2016 Benjamin Robin
 *
 * Asuc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Asuc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Asuc.  If not, see <http://www.gnu.org/licenses/>
 */

#include <QtGui>
#include <QClipboard>
#include <QMimeData>
#include <QScrollBar>
#include <QApplication>
#include <QMenu>

#include "ConsoleTextEdit.h"

ConsoleTextEdit::ConsoleTextEdit(QWidget *_parent) : QTextEdit(_parent)
{
    m_txtFormat[TYPE_RCV].setForeground(QBrush(QColor(0, 0, 0)));
    m_txtFormat[TYPE_INFO].setForeground(QBrush(QColor(0, 0, 180)));
    m_txtFormat[TYPE_ERR].setForeground(QBrush(QColor(255, 0, 0)));

    setReadOnly(true);
    setAcceptRichText(false);
    setOverwriteMode(true);

    m_editCursor = textCursor();
}

ConsoleTextEdit::~ConsoleTextEdit()
{
}

void ConsoleTextEdit::append(const QString &txtRcv, ConsoleTextEdit::type info)
{
    QStringList list(txtRcv.split(QChar(8)));
    QTextCharFormat format;

    switch (info) {
        case TYPE_RCV:
            format = m_txtFormat[TYPE_RCV];
            break;
        case TYPE_INFO:
            format = m_txtFormat[TYPE_INFO];
            break;
        case TYPE_ERR:
        default:
            format = m_txtFormat[TYPE_ERR];
            break;
    }

    bool skipFirst = true;
    foreach (const QString &s, list)
    {
        if (skipFirst)
        {
            skipFirst = false;
        }
        else
        {
            m_editCursor.deletePreviousChar();
        }
        m_editCursor.insertText(s, format);
    }
    setTextCursor(m_editCursor);

    QScrollBar *sb = verticalScrollBar();
    sb->setValue(sb->maximum());
}

void ConsoleTextEdit::insertClipboard(bool useSelection)
{
    const QMimeData *mimeData = QApplication::clipboard()->mimeData(
        useSelection ? QClipboard::Selection : QClipboard::Clipboard);

    if (mimeData->hasText())
    {
        emit text(mimeData->text());
    }
}

bool ConsoleTextEdit::focusNextPrevChild(bool next)
{
    Q_UNUSED(next);
    return false;
}

void ConsoleTextEdit::keyPressEvent(QKeyEvent *e)
{
    const Qt::KeyboardModifiers mod = e->modifiers();
    const int key = e->key();

    e->accept();

    if ( ((mod == Qt::ControlModifier) && (key == Qt::Key_V)) ||
         ((mod == Qt::ShiftModifier)   && (key == Qt::Key_Insert)) )
    {
        insertClipboard();
        return;
    }

    if ( (mod == Qt::ControlModifier) && (key == Qt::Key_A) )
    {
        selectAll();
        return;
    }

    if ( ((mod == Qt::ControlModifier) && (key == Qt::Key_C)) ||
         ((mod == Qt::ControlModifier) && (key == Qt::Key_Insert)) )
    {
        copy();
        return;
    }

    if (mod == Qt::NoModifier)
    {
        switch (key) {
        case Qt::Key_Up:
            emit text(QLatin1String("\033[A"));
            return;
        case Qt::Key_Down:
            emit text(QLatin1String("\033[B"));
            return;
        case Qt::Key_Right:
            emit text(QLatin1String("a")); //("\033[C"));
            return;
        case Qt::Key_Left:
            //emit text(QLatin1String("\033[D"));
            emit text(QLatin1String("\008"));
            return;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            emit text(QLatin1String("\n"));
            return;
        default:
            break;
        }
    }

    if (!e->text().isEmpty())
    {
        emit text(e->text());
        return;
    }
}

void ConsoleTextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *menu = new QMenu(this);
    QAction *a;

    menu->addAction(tr("C&lear"), this, SLOT(clear()));
    menu->addSeparator();

    a = menu->addAction(tr("&Copy\tCtrl+C"), this, SLOT(copy()));
    a->setEnabled(textCursor().hasSelection());

    menu->addAction(tr("&Paste\tCtrl+V"), this, SLOT(insertClipboard()));
    menu->addSeparator();

    a = menu->addAction(tr("Select All\tCtrl+A"), this, SLOT(selectAll()));
    a->setEnabled(!toPlainText().isEmpty());

    menu->setAttribute(Qt::WA_DeleteOnClose);
    menu->popup(e->globalPos());
}

#ifdef Q_WS_X11
void ConsoleTextEdit::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::MidButton)
    {
        insertClipboard(true);
    }
    QTextEdit::mousePressEvent(e);
}
#endif
