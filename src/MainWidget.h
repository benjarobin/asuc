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

#ifndef MAIN_WIDGET_H_
#define MAIN_WIDGET_H_ 1

#include <QWidget>
#include <QHostAddress>

class QUdpSocket;
class QLineEdit;
class QSpinBox;
class QHostInfo;
class ConsoleTextEdit;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *_parent = 0);
    virtual ~MainWidget();

public slots:
    /* (re)start the listener */
    void udpStartListen();
    /* Destination address changed */
    void hostAddrEdited();
    void hostLookedUp(const QHostInfo &host);
    void showSendingAddr();
    /* read data from UDP socket */
    void udpRead();
    /* send out data via UDP */
    void udpSend(const QString &text);

private:
    QUdpSocket      *m_socket;
    QSpinBox        *m_localPort;
    QLineEdit       *m_hostEdit;
    QSpinBox        *m_destPort;
    ConsoleTextEdit *m_txtEdit;

    QString         m_prevHost;
    QHostAddress    m_destAddr;
    int             m_lookUpId;
};

#endif // MAIN_WIDGET_H_
