/**
 * MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MyTextEdit.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QClipboard>
#include <QMimeData>
#include <QScrollBar>
#include <QApplication>
#include <QMenu>

/* local library headers */

/* local headers */


MyTextEdit::MyTextEdit(QWidget *parent)
: QTextEdit(parent)
, mTextColor( 0, 0, 0 )
, mInfoColor( 255, 0, 0 )
{
   setReadOnly( true );
   setAcceptRichText( false );
   setCursorWidth( 2 );
}


MyTextEdit::~MyTextEdit()
{
}


void MyTextEdit::append( const QString &data, bool info )
{
   QStringList list( data.split(QChar(8)) );

   QTextCharFormat format;
   if( info )
   {
      format.setForeground( QBrush( mInfoColor ) );
   }
   else
   {
      format.setForeground( QBrush( mTextColor ) );
   }
   QTextCursor cursor( textCursor() );
   cursor.movePosition( QTextCursor::End );

   bool skipFirst = true;
   foreach( const QString &s, list )
   {
      if( skipFirst )
      {
         skipFirst = false;
      }
      else
      {
         cursor.deletePreviousChar();
      }
      cursor.insertText( s, format );
   }
   setTextCursor( cursor );

   QScrollBar *sb = verticalScrollBar();
   sb->setValue( sb->maximum() );
}


void MyTextEdit::insertClipboard( bool useSelection )
{
   const QMimeData *mimeData = QApplication::clipboard()->mimeData( useSelection ? QClipboard::Selection : QClipboard::Clipboard );

   if( mimeData->hasText() )
   {
      emit text( mimeData->text() );
   }
}


void MyTextEdit::keyPressEvent( QKeyEvent *e )
{
   if( ((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_V)) ||
       ((e->modifiers() == Qt::ShiftModifier)   && (e->key() == Qt::Key_Insert)) )
   {
      insertClipboard();
      e->accept();
      return;
   }

   if( (e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_A) )
   {
      selectAll();
      e->accept();
      return;
   }

   if( ((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_C)) ||
       ((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_Insert)) )
   {
      copy();
      e->accept();
      return;
   }

   if( e->text().isEmpty() )
   {
      e->ignore();
      return;
   }

   emit text( e->text() );
   e->accept();
}


void MyTextEdit::contextMenuEvent( QContextMenuEvent *e )
{
   QMenu *menu = new QMenu( this );
   QAction *a;

   menu->addAction( tr("C&lear"),
                    this, SLOT(clear()) );
   menu->addSeparator();
   a = menu->addAction( tr("&Copy\tCtrl+C"),
                        this, SLOT(copy()) );
   a->setEnabled(textCursor().hasSelection());
   menu->addAction( tr("&Paste\tCtrl+V"),
                    this, SLOT(insertClipboard()) );
   menu->addSeparator();
   a = menu->addAction( tr("Select All\tCtrl+A"),
                        this, SLOT(selectAll()) );
   a->setEnabled( !toPlainText().isEmpty() );

   menu->setAttribute( Qt::WA_DeleteOnClose );
   menu->popup( e->globalPos() );
}

#ifdef Q_WS_X11
void MyTextEdit::mousePressEvent( QMouseEvent *e )
{
   if( e->button() == Qt::MidButton )
   {
      insertClipboard( true );
   }
   QTextEdit::mousePressEvent( e );
}
#endif
