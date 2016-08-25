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

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QUdpSocket;

/* forward declaration of local classes */
class MyTextEdit;


class MainWidget : public QWidget
{
   Q_OBJECT

public:
   MainWidget( QWidget *parent = 0 );
   virtual ~MainWidget();

public slots:
   /* (re)start the listener */
   void udpStartListen();
   /* read data from UDP socket */
   void udpRead();
   /* send out data via UDP */
   void udpSend( const QString &text );

private:
   QUdpSocket     *mpSocket;
   QSpinBox       *mpMyPort;
   QLineEdit      *mpDestHost;
   QSpinBox       *mpDestPort;
   MyTextEdit     *mpEdit;
};

#endif // MAINWIDGET_HPP
