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

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QHostInfo>
#include <QUdpSocket>
#include <QTextCursor>
#include <QBoxLayout>
#include <QLabel>

/* local library headers */

/* local headers */
#include "Version.hpp"
#include "MyTextEdit.hpp"


MainWidget::MainWidget( QWidget *parent )
: QWidget(parent)
, mpSocket( new QUdpSocket( this ) )
, mpMyPort( new QSpinBox( this ) )
, mpDestHost( new QLineEdit( this ) )
, mpDestPort( new QSpinBox( this ) )
, mpEdit( new MyTextEdit( this ) )
{
   QSettings settings;
   mpEdit->setFont( QFont("Courier") );
   mpMyPort->setAlignment( Qt::AlignRight );
   mpDestPort->setAlignment( Qt::AlignRight );
   mpMyPort->setRange( 1025, 65535 );
   mpDestPort->setRange( 1, 65535 );
   mpMyPort->setValue( settings.value( "MyPort", 6666 ).toInt() );
   mpDestHost->setText( settings.value( "DestHost" ).toString() );
   mpDestPort->setValue( settings.value( "DestPort", 6666 ).toInt() );
   mpEdit->append( tr("*** Version %1 ***\n\n").arg(ASUC_VERSION), true );
   mpEdit->setFocus();

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QBoxLayout *topLayout  = new QHBoxLayout();
   topLayout->addWidget( new QLabel(tr("Local Port:"),this) );
   topLayout->addWidget( mpMyPort );
   topLayout->addWidget( new QLabel(tr("Remote Host:"),this) );
   topLayout->addWidget( mpDestHost, 1 );
   topLayout->addWidget( new QLabel(tr("Remote Port:"),this) );
   topLayout->addWidget( mpDestPort );
   mainLayout->addLayout( topLayout );
   mainLayout->addWidget( mpEdit );

   /* restart listener if parameter changed */
   connect( mpMyPort, SIGNAL(valueChanged(int)),
            this, SLOT(udpStartListen()) );
   udpStartListen();

   /* communication socket <-> widget */
   connect( mpSocket, SIGNAL(readyRead()),
            this, SLOT(udpRead()) );
   connect( mpEdit, SIGNAL(text(QString)),
            this, SLOT(udpSend(QString)) );
}

MainWidget::~MainWidget()
{
   QSettings settings;
   settings.setValue( "MyPort",   mpMyPort->value() );
   settings.setValue( "DestHost", mpDestHost->text() );
   settings.setValue( "DestPort", mpDestPort->value() );
   settings.sync();
}


void MainWidget::udpStartListen()
{
   if( mpSocket->state() != QAbstractSocket::UnconnectedState )
   {
      mpSocket->close();
   }
   if( mpSocket->bind( QHostAddress::Any, mpMyPort->value() ) )
   {
      mpEdit->append( tr("*** listening on port %1 ***\n").arg(mpMyPort->value()), true );
   }
   else
   {
      mpEdit->append( tr("*** bind failed ***\n"), true );
   }
}


void MainWidget::udpRead()
{
   while( mpSocket->hasPendingDatagrams() )
   {
      QByteArray data;
      data.resize( mpSocket->pendingDatagramSize() );
      mpSocket->readDatagram( data.data(), data.size() );
      mpEdit->append( QString::fromUtf8(data) );
   }
}


void MainWidget::udpSend( const QString &text )
{
   QByteArray data( text.toUtf8() );
   QHostAddress host( mpDestHost->text() );
   if( host.isNull() )
   {
      mpEdit->append( tr("*** send error: %1 ***\n").arg(mpDestHost->text() ), true );
   }
   else
   {
      quint16 port( mpDestPort->value() );
      mpSocket->writeDatagram( data, host, port );
   }
}
