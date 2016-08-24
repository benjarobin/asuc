/**
 * MyTextEdit.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
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
