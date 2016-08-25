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
#include <QApplication>

#include "MainWindow.h"
#include "MainWidget.h"
#include "Settings.h"

MainWindow::MainWindow() : QMainWindow(NULL, 0)
{
    QSettings settings;

    setCentralWidget(new MainWidget(this));
    setWindowTitle(tr("asuc : A Simple Udp Console"));

    restoreGeometry(settings.value(MAIN_WIN_GEOMETRY).toByteArray());
    restoreState(settings.value(MAIN_WIN_STATE).toByteArray());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;

    settings.setValue(MAIN_WIN_GEOMETRY, saveGeometry());
    settings.setValue(MAIN_WIN_STATE, saveState());
    QMainWindow::closeEvent(event);
}
