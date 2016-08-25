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

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_ 1

#include <QMainWindow>

class MainWidget;

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow();

protected:
   virtual void closeEvent(QCloseEvent *event);
};

#endif // MAIN_WINDOW_H_
