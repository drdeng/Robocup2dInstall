// -*-c++-*-

/*!
  \file monitor_client.cpp
  \brief Monitor Client class Source File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3, or (at your option)
 any later version.

 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtNetwork>

#include "monitor_client.h"

#include "options.h"
#include "main_data.h"

#include <rcsc/trainer/trainer_command.h>
#include <rcsc/monitor/monitor_command.h>
#include <rcsc/rcg/parser_v4.h>
#include <rcsc/rcg/parser_v5.h>
#include <rcsc/rcg/types.h>

#include <sstream>
#include <iostream>
#include <cassert>


namespace {
const int POLL_INTERVAL_MS = 1000;
}

class MonitorClientImpl {
public:

    QHostAddress server_addr_;
    quint16 server_port_;

};


/*-------------------------------------------------------------------*/
/*!

*/
MonitorClient::MonitorClient( QObject * parent,
                              MainData & main_data,
                              const char * hostname,
                              const int port,
                              const int version )

    : QObject( parent ),
      M_impl( new MonitorClientImpl() ),
      M_main_data( main_data ),
      M_socket( new QUdpSocket( this ) ),
      M_timer( new QTimer( this ) ),
      M_version( version ),
      M_waited_msec( 0 )
{

    // check protocl versin range
    if ( version < 1 )
    {
        M_version = 1;
    }

    if ( 4 < version )
    {
        M_version = 4;
    }

    QHostInfo host = QHostInfo::fromName( QString::fromLatin1( hostname ) );

    if ( host.error() != QHostInfo::NoError )
    {
        qDebug() << "Error " << host.errorString();
        return;
    }

    M_impl->server_addr_ = host.addresses().front();
    M_impl->server_port_ = port;

    // setReadBufferSize() makes no effect for UdpSocet...
    //M_socket->setReadBufferSize( 8192 * 256 );

    //M_socket->bind( M_impl->server_addr_,
    //                0 ); // created a local port
    // INADDR_ANY, bind random created port to local
    if ( ! M_socket->bind( 0 ) )
    {
        std::cerr << __FILE__ << ": failed to bind the socket."
                  << std::endl;
        return;
    }

    if ( ! isConnected() )
    {
        std::cerr << __FILE__ << " failed to initialize the socket."
                  << std::endl;
        return;
    }

    connect( M_socket, SIGNAL( readyRead() ), this, SLOT( handleReceive() ) );
    connect( M_timer, SIGNAL( timeout() ), this, SLOT( handleTimer() ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
MonitorClient::~MonitorClient()
{
    disconnect();

    //std::cerr << "delete MonitorClient" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::disconnect()
{
    if ( M_timer )
    {
        M_timer->stop();
    }

    if ( isConnected() )
    {
        sendDispBye();
        M_socket->close();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
MonitorClient::isConnected() const
{
    return ( M_socket->socketDescriptor() != -1 );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::handleReceive()
{
    int receive_count = 0;

    if ( M_version >= 3 )
    {
        char buf[8192];
        while ( M_socket->hasPendingDatagrams() )
        {
            quint16 from_port;
            int n = M_socket->readDatagram( buf,
                                            8192,
                                            0, // QHosAddress
                                            &from_port );
            if ( n > 0 )
            {
                if ( ! M_main_data.receiveMonitorPacket( buf, M_version ) )
                {
                    std::cerr << PACKAGE_NAME << " recv: " << buf << std::endl;
                }

                if ( from_port != M_impl->server_port_ )
                {
                    std::cerr << PACKAGE_NAME << " updated server port number = "
                              << from_port
                        //<< "  localPort = "
                        //<< M_socket->localPort()
                              << std::endl;

                    M_impl->server_port_ = from_port;
                }
            }
            ++receive_count;
        }
    }
    else if ( M_version == 2 )
    {
        rcsc::rcg::dispinfo_t2 disp2;
        while ( M_socket->hasPendingDatagrams() )
        {
            quint16 from_port;
            int n = M_socket->readDatagram( reinterpret_cast< char * >( &disp2 ),
                                            sizeof( disp2 ),
                                            0, // QHosAddress
                                            &from_port );
            if ( n > 0 )
            {
                if ( ! M_main_data.receiveMonitorPacket( disp2 ) )
                {
                    std::cerr << PACKAGE_NAME << " recv: "
                              << reinterpret_cast< char * >( &disp2 )
                              << std::endl;
                }

                if ( from_port != M_impl->server_port_ )
                {
                    std::cerr << PACKAGE_NAME << " updated server port number = "
                              << from_port
                        //<< "  localPort = "
                        //<< M_socket->localPort()
                              << std::endl;

                    M_impl->server_port_ = from_port;
                }
            }
            ++receive_count;
        }
    }
    else if ( M_version == 1 )
    {
        rcsc::rcg::dispinfo_t disp1;
        while ( M_socket->hasPendingDatagrams() )
        {
            quint16 from_port;
            int n =  M_socket->readDatagram( reinterpret_cast< char * >( &disp1 ),
                                             sizeof( disp1 ),
                                             0, // QHosAddress
                                             &from_port );
            if ( n > 0 )
            {
                if ( ! M_main_data.receiveMonitorPacket( disp1 ) )
                {
                    std::cerr << PACKAGE_NAME << " recv: "
                              << reinterpret_cast< char * >( &disp1 )
                              << std::endl;
                }

                if ( from_port != M_impl->server_port_ )
                {
                    std::cerr << PACKAGE_NAME << " updated port number = "
                              << from_port << std::endl;

                    M_impl->server_port_ = from_port;
                }
            }
            ++receive_count;
        }
    }

    if ( receive_count > 0 )
    {
        M_waited_msec = 0;
        M_timer->start( POLL_INTERVAL_MS );

        emit received();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::handleTimer()
{
    M_waited_msec += POLL_INTERVAL_MS;

    //std::cerr << "handleTimer waited = " << M_waited_msec << std::endl;

    if ( Options::instance().waitSeconds() > 0
         && M_waited_msec >= Options::instance().waitSeconds() * 1000 )
    {
        if ( Options::instance().autoQuitMode() )
        {
            std::cerr << PACKAGE_NAME << " waited " << M_waited_msec * 1000 << " seconds.\n"
                      << "exit..." << std::endl;
            QCoreApplication::exit();
        }
        emit timeout();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendCommand( const rcsc::MonitorCommand & com )
{
    if ( ! isConnected() )
    {
        return;
    }

    std::ostringstream ostr;
    com.toCommandString( ostr );

    std::string com_str = ostr.str();
    M_socket->writeDatagram( com_str.c_str(), com_str.length() + 1,
                             M_impl->server_addr_,
                             M_impl->server_port_ );
    std::cerr << PACKAGE_NAME <<  " send: " << com_str << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendCommand( const rcsc::TrainerCommand & com )
{
    if ( ! isConnected() )
    {
        return;
    }

    std::ostringstream ostr;
    com.toCommandString( ostr );

    std::string com_str = ostr.str();
    M_socket->writeDatagram( com_str.c_str(), com_str.length() + 1,
                             M_impl->server_addr_,
                             M_impl->server_port_ );
    std::cerr << PACKAGE_NAME << " send: " << com_str << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendDispInit()
{
    rcsc::MonitorInitCommand com( M_version );

    sendCommand( com );

    if ( M_timer )
    {
        M_timer->start( 1000 );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendDispBye()
{
    rcsc::MonitorByeCommand com;

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendKickOff()
{
    rcsc::MonitorKickOffCommand com;

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendFreeKick( const double & x,
                             const double & y,
                             const rcsc::SideID side )
{
    rcsc::MonitorFreeKickCommand com( x, y, side );

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendMove( const rcsc::SideID side,
                         const int unum,
                         const double & x, const double & y,
                         const double & angle )
{
    if ( side == rcsc::NEUTRAL )
    {
        std::cerr << __FILE__ << ": (sendMove)"
                  << " dispplayer invalie side" << std::endl;
        return;
    }

    if ( unum < 1 || 11 < unum )
    {
        std::cerr << __FILE__ << ": (sendMove)"
                  << " dispplayer invalie unum " << unum << std::endl;
        return;
    }

    rcsc::MonitorMovePlayerCommand com( side, unum, x, y, angle );

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendDiscard( const rcsc::SideID side,
                            const int unum )
{
    if ( side == rcsc::NEUTRAL )
    {
        std::cerr << __FILE__ << ": (sendDiscard)"
                  << " dispdiscard Invalie side" << std::endl;
        return;
    }

    if ( unum < 1 || 11 < unum )
    {
        std::cerr << __FILE__ << ": (sendDiscard)"
                  << " dispdiscard invalie unum " << unum << std::endl;
        return;
    }

    rcsc::MonitorDiscardPlayerCommand com( side, unum );

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendCard( const rcsc::SideID side,
                         const int unum,
                         const rcsc::Card card )
{
    if ( side == rcsc::NEUTRAL )
    {
        std::cerr << __FILE__ << ": (sendCard)"
                  << " dispcard invalid side" << std::endl;
        return;
    }

    if ( unum < 1 || 11 < unum )
    {
        std::cerr << __FILE__ << ": (sendCard)"
                  << " dispcard invalid unum " << unum << std::endl;
        return;
    }

    if ( card != rcsc::YELLOW
         && card != rcsc::RED )
    {
        std::cerr << __FILE__ << ": (sendCard)"
                  << " dispcard invalid card " << card << std::endl;
        return;
    }

    rcsc::MonitorCardCommand com( side, unum, card );

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendChangeMode( const rcsc::PlayMode pmode )
{
    rcsc::TrainerChangeModeCommand com( pmode );

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendTrainerMoveBall( const double & x,
                                    const double & y )
{
    rcsc::TrainerMoveBallCommand com( x, y );

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendTrainerMoveBall( const double & x,
                                    const double & y,
                                    const double & vx,
                                    const double & vy )
{
    rcsc::TrainerMoveBallCommand com( x, y, vx, vy );

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendTrainerMovePlayer( const std::string & team_name,
                                      const int unum,
                                      const double & x,
                                      const double & y )
{
    rcsc::TrainerMovePlayerCommand com( team_name, unum,
                                        x, y );

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendTrainerMovePlayer( const std::string & team_name,
                                      const int unum,
                                      const double & x,
                                      const double & y,
                                      const double & angle )
{
    rcsc::TrainerMovePlayerCommand com( team_name, unum,
                                        x, y,
                                        angle );

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendTrainerMovePlayer( const std::string & team_name,
                                      const int unum,
                                      const double & x,
                                      const double & y,
                                      const double & angle,
                                      const double & vx,
                                      const double & vy )
{
    rcsc::TrainerMovePlayerCommand com( team_name, unum,
                                        x, y,
                                        angle,
                                        vx, vy );

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendRecover()
{
    rcsc::TrainerRecoverCommand com;

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sencChangePlayerType( const std::string & team_name,
                                     const int unum,
                                     const int type )
{
    rcsc::TrainerChangePlayerTypeCommand com( team_name, unum, type );

    sendCommand( com );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorClient::sendCheckBal()
{
    rcsc::TrainerCheckBallCommand com;

    sendCommand( com );
}
