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

#include <QtGui>
#include <QHostInfo>
#include <QUdpSocket>
#include <QTextCursor>
#include <QBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>

#include "MainWidget.h"
#include "Version.h"
#include "ConsoleTextEdit.h"
#include "Settings.h"

MainWidget::MainWidget(QWidget *_parent) : QWidget(_parent),
    m_socket(new QUdpSocket(this)),
    m_srcPort(new QSpinBox(this)),
    m_destHost(new QLineEdit(this)),
    m_destPort(new QSpinBox(this)),
    m_txtEdit(new ConsoleTextEdit(this))
{
    QSettings settings;

    m_srcPort->setAlignment(Qt::AlignRight);
    m_srcPort->setRange(1025, 65535);
    m_srcPort->setValue(settings.value(SRC_PORT, 6666).toInt());

    m_destPort->setAlignment(Qt::AlignRight);
    m_destPort->setRange(1, 65535);
    m_destPort->setValue(settings.value(DEST_PORT, 6666).toInt());

    m_destHost->setText(settings.value(DEST_HOST).toString());

    m_txtEdit->setFont(QFont(QLatin1String("Courier")));
    m_txtEdit->append(tr("*** Version %1 ***\n").arg(ASUC_VERSION), ConsoleTextEdit::TYPE_INFO);
    m_txtEdit->setFocus();

    QBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(new QLabel(tr("Local Port:"), this));
    topLayout->addWidget(m_srcPort );
    topLayout->addWidget(new QLabel(tr("Remote Host:"), this));
    topLayout->addWidget(m_destHost, 1);
    topLayout->addWidget(new QLabel(tr("Remote Port:"), this));
    topLayout->addWidget(m_destPort);

    QBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_txtEdit);

    /* restart listener if parameter changed */
    connect(m_srcPort, SIGNAL(valueChanged(int)), this, SLOT(udpStartListen()));
    connect(m_destHost, SIGNAL(textChanged(QString)), this, SLOT(hostAddrChanged(QString)));

    /* communication socket <-> widget */
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(udpRead()));
    connect(m_txtEdit, SIGNAL(text(QString)), this, SLOT(udpSend(QString)));

    udpStartListen();
}

MainWidget::~MainWidget()
{
    QSettings settings;
    settings.setValue(SRC_PORT,  m_srcPort->value());
    settings.setValue(DEST_PORT, m_destPort->value());
    settings.setValue(DEST_HOST, m_destHost->text());
    settings.sync();
}

void MainWidget::udpStartListen()
{
    int port = m_srcPort->value();

    if (m_socket->state() != QAbstractSocket::UnconnectedState)
    {
        m_socket->close();
    }

    if (m_socket->bind(QHostAddress::Any, port))
    {
        m_txtEdit->append(tr("*** listening on port %1 ***\n").arg(port), ConsoleTextEdit::TYPE_INFO);
    }
    else
    {
        m_txtEdit->append(tr("*** bind failed on port %1 ***\n").arg(port), ConsoleTextEdit::TYPE_ERR);
    }
}

void MainWidget::hostAddrChanged(const QString &text)
{
    m_destAddr.setAddress(text);
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
        m_txtEdit->append(tr("*** send error: %1 ***\n").arg(m_destHost->text()),
                          ConsoleTextEdit::TYPE_ERR);
    }
    else
    {
        quint16 port(m_destPort->value());
        m_socket->writeDatagram(dataSnd, m_destAddr, port);
    }
}
