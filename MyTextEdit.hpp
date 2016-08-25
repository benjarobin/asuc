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

#ifndef MYTEXTEDIT_HPP
#define MYTEXTEDIT_HPP MYTEXTEDIT_HPP

/* base class */
#include <QTextEdit>

/* system headers */

/* Qt headers */
#include <QColor>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class MyTextEdit : public QTextEdit
{
   Q_OBJECT

public:
   MyTextEdit(QWidget *parent = 0);
   virtual ~MyTextEdit();

   /* append text to text edit */
   void append( const QString &data, bool info = false );

public slots:
   /* send the clipboard */
   void insertClipboard( bool useSelection = false );

signals:
   /* send entered text */
   void text( const QString &text );

protected:
   /* reimplemented to send them via text signal */
   void keyPressEvent( QKeyEvent *event );
   /* reimplemented to add paste & clear */
   void contextMenuEvent( QContextMenuEvent *event );
#ifdef Q_WS_X11
   /* reimplemented to add support for middle mouse button */
   void mousePressEvent( QMouseEvent *e );
#endif

private:
   MyTextEdit( const MyTextEdit &that );
   MyTextEdit &operator=( const MyTextEdit &that );

   QColor         mTextColor;
   QColor         mInfoColor;
};

#endif // MYTEXTEDIT_HPP
