/**
 * This file is part of Asuc.
 *
 * Copyright 2010 Sven Oliver Moll
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

#ifndef CONSOLE_TEXT_EDIT_H_
#define CONSOLE_TEXT_EDIT_H_ 1

#include <QTextEdit>
#include <QTextCharFormat>

class ConsoleTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    ConsoleTextEdit(QWidget *_parent = 0);
    virtual ~ConsoleTextEdit();

    enum type {
        TYPE_RCV = 0,
        TYPE_INFO = 1,
        TYPE_ERR = 2
    };

    /* append text to text edit */
    void append(const QString &txtRcv, type info = TYPE_RCV);

public slots:
    /* send the clipboard */
    void insertClipboard(bool useSelection = false);

signals:
    /* send entered text */
    void text(const QString &text);

protected:
    /* reimplemented to send them via text signal */
    void keyPressEvent(QKeyEvent *event);
    /* reimplemented to add paste & clear */
    void contextMenuEvent(QContextMenuEvent *event);
#ifdef Q_WS_X11
    /* reimplemented to add support for middle mouse button */
    void mousePressEvent(QMouseEvent *e);
#endif

private:
    ConsoleTextEdit(const ConsoleTextEdit &that);
    ConsoleTextEdit &operator=(const ConsoleTextEdit &that);

    QTextCharFormat m_txtFormat[3];
};

#endif // CONSOLE_TEXT_EDIT_H_
