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
