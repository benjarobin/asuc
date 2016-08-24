/**
 * MainWindow.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP MAINWINDOW_HPP

/* base class */
#include <QMainWindow>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class MainWidget;


class MainWindow : public QMainWindow
{
   Q_OBJECT
   
public:
   MainWindow( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

protected:
   /* intercept for writing the settings */
   virtual void closeEvent( QCloseEvent *event );
   /* very ugly workaround for wrong position restoration on Ubuntu */
   virtual bool event( QEvent *event );
   
private:
   int mForbidMove;

   MainWindow( const MainWindow &other );
   MainWindow &operator=( const MainWindow &other );
};

#endif
