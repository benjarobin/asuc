/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
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
