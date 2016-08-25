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
#include <QHostInfo>
#include <QUdpSocket>
#include <QTextCursor>
#include <QBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QHostInfo>

#include "MainWidget.h"
#include "Version.h"
#include "ConsoleTextEdit.h"
#include "Settings.h"

MainWidget::MainWidget(QWidget *_parent) : QWidget(_parent),
    m_socket(new QUdpSocket(this)),
    m_localPort(new QSpinBox(this)),
    m_hostEdit(new QLineEdit(this)),
    m_destPort(new QSpinBox(this)),
    m_txtEdit(new ConsoleTextEdit(this)),
    m_lookUpId(-1)
{
    QSettings settings;

    m_localPort->setAlignment(Qt::AlignRight);
    m_localPort->setRange(1025, 65535);
    m_localPort->setValue(settings.value(SRC_PORT, 6666).toInt());

    m_destPort->setAlignment(Qt::AlignRight);
    m_destPort->setRange(1, 65535);
    m_destPort->setValue(settings.value(DEST_PORT, 6666).toInt());

    m_hostEdit->setText(settings.value(DEST_HOST).toString());

    m_txtEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    m_txtEdit->append(tr("*** Version %1\n").arg(ASUC_VERSION), ConsoleTextEdit::TYPE_INFO);
    m_txtEdit->setFocus();

    QBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(new QLabel(tr("Local Port:"), this));
    topLayout->addWidget(m_localPort );
    topLayout->addWidget(new QLabel(tr("Remote Host:"), this));
    topLayout->addWidget(m_hostEdit, 1);
    topLayout->addWidget(new QLabel(tr("Remote Port:"), this));
    topLayout->addWidget(m_destPort);

    QBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_txtEdit);

    // restart listener if parameter changed
    connect(m_localPort, SIGNAL(valueChanged(int)), this, SLOT(udpStartListen()));
    connect(m_hostEdit, SIGNAL(editingFinished()), this, SLOT(hostAddrEdited()));
    connect(m_destPort, SIGNAL(valueChanged(int)), this, SLOT(showSendingAddr()));

    // communication socket <-> widget
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(udpRead()));
    connect(m_txtEdit, SIGNAL(text(QString)), this, SLOT(udpSend(QString)));

    udpStartListen();
    hostAddrEdited();
}

MainWidget::~MainWidget()
{
    QSettings settings;
    settings.setValue(SRC_PORT,  m_localPort->value());
    settings.setValue(DEST_PORT, m_destPort->value());
    settings.setValue(DEST_HOST, m_hostEdit->text());
    settings.sync();
}

void MainWidget::udpStartListen()
{
    int port = m_localPort->value();

    if (m_socket->state() != QAbstractSocket::UnconnectedState)
    {
        m_socket->close();
    }

    if (m_socket->bind(QHostAddress::Any, port))
    {
        m_txtEdit->append(tr("*** Listening on port %1\n").arg(port), ConsoleTextEdit::TYPE_INFO);
    }
    else
    {
        m_txtEdit->append(tr("*** Bind failed on port %1\n").arg(port), ConsoleTextEdit::TYPE_ERR);
    }
}

void MainWidget::hostAddrEdited()
{
    QString host(m_hostEdit->text().trimmed());

    if (host != m_prevHost)
    {
        // Abort any running host look up
        if (m_lookUpId >= 0)
        {
            QHostInfo::abortHostLookup(m_lookUpId);
            m_lookUpId = -1;
        }

        // Try to parse the host as an IP
        if (host.length() > 0)
        {
            m_destAddr.setAddress(host);
            if (m_destAddr.isNull())
            {
                // Parse failed, try to resolve it using DNS
                m_lookUpId = QHostInfo::lookupHost(host, this, SLOT(hostLookedUp(QHostInfo)));
            }
            else
            {
                showSendingAddr();
            }
        }
        else
        {
            m_destAddr.clear();
        }

        m_prevHost = host;
    }
}

void MainWidget::hostLookedUp(const QHostInfo &host)
{
    m_lookUpId = -1;

    if (host.error() != QHostInfo::NoError)
    {
        m_txtEdit->append(tr("*** Host look up failed: %1\n").arg(host.errorString()),
                          ConsoleTextEdit::TYPE_ERR);
    }
    else
    {
        QHostAddress addrFound;
        QList<QHostAddress> lstAddr = host.addresses();

        // Try to find an IPv4 first
        for (const QHostAddress &addr : lstAddr)
        {
            if (addr.protocol() == QAbstractSocket::IPv4Protocol)
            {
                addrFound = addr;
                break;
            }
        }

        // Then an IPv6
        if (addrFound.isNull())
        {
            for (const QHostAddress &addr : lstAddr)
            {
                if (addr.protocol() == QAbstractSocket::IPv6Protocol)
                {
                    addrFound = addr;
                    break;
                }
            }
        }

        // If none is found show a warning, else set the found address
        if (addrFound.isNull())
        {
            m_txtEdit->append(tr("*** Host look up without IP: %1\n").arg(host.hostName()),
                              ConsoleTextEdit::TYPE_ERR);
        }
        else
        {
            m_destAddr = addrFound;
            showSendingAddr();
        }
    }
}

void MainWidget::showSendingAddr()
{
    QString ip;

    if (m_destAddr.protocol() == QAbstractSocket::IPv4Protocol)
    {
        ip.append(m_destAddr.toString());
    }
    else
    {
        ip.append(QLatin1Char('[')).append(m_destAddr.toString()).append(QLatin1Char(']'));
    }

    m_txtEdit->append(tr("*** Sending to %1:%2\n").arg(ip).arg(m_destPort->value()),
                      ConsoleTextEdit::TYPE_INFO);
}

void MainWidget::udpRead()
{
    while (m_socket->hasPendingDatagrams())
    {
        QByteArray dataRcv;
        dataRcv.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(dataRcv.data(), dataRcv.size());
        m_txtEdit->append(QString::fromUtf8(dataRcv));
    }
}

void MainWidget::udpSend(const QString &text)
{
    QByteArray dataSnd(text.toUtf8());
    if (m_destAddr.isNull())
    {
        m_txtEdit->append(tr("*** Send error: %1\n").arg(m_hostEdit->text()),
                          ConsoleTextEdit::TYPE_ERR);
    }
    else
    {
        quint16 port(m_destPort->value());
        m_socket->writeDatagram(dataSnd, m_destAddr, port);
    }
}


