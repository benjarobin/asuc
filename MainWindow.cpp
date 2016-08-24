/**
 * MainWindow.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MainWindow.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QApplication>

/* local library headers */

/* local headers */
#include "MainWidget.hpp"


MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mForbidMove( 50 )
{
   QSettings settings;
   setCentralWidget( new MainWidget( this ) );
   setWindowTitle( QApplication::applicationName()+
                   ": A Simple Udp Console" );
   
   QPoint defaultPos( -1, -1 );
   QSize  defaultSize( -1, -1 );
   QPoint pos( settings.value( "MainWindowPosition", defaultPos ).toPoint() );
   QSize  size( settings.value( "MainWindowSize",    defaultSize ).toSize() );
   if( size != defaultSize )
   {
      resize( size );
   }
   if( pos != defaultPos )
   {
      move( pos );
   }
   
   //setWindowIcon( QIcon( ":/clock.png" ) );
}


/* Evil workaround: in the first fifty events revert all move requests */
bool MainWindow::event( QEvent *event )
{
   if( mForbidMove > 0 )
   {
      if( event->type() == QEvent::Move )
      {
         QPoint defaultPos( -1, -1 );
         QPoint pos( QSettings().value( "MainWindowPosition", defaultPos ).toPoint() );
         if( pos != defaultPos )
         {
            move( pos );
         }
      }
      mForbidMove--;
   }
   return QWidget::event( event );
}


void MainWindow::closeEvent( QCloseEvent *event )
{
   QSettings settings;
   settings.setValue( "MainWindowPosition", pos() );
   settings.setValue( "MainWindowSize",    size() );

   event->accept();
}
