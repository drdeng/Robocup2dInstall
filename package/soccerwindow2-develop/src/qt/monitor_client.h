// -*-c++-*-

/*!
  \file monitor_client.h
  \brief Monitor Client class Header File.
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

#ifndef SOCCERWINDOW2_QT_MONITOR_CLIENT_H
#define SOCCERWINDOW2_QT_MONITOR_CLIENT_H

#include <QObject>

#include <rcsc/types.h>

#include <boost/scoped_ptr.hpp>

class QTimer;
class QUdpSocket;

namespace rcsc {
class MonitorCommand;
class TrainerCommand;
}

class MainData;
class MonitorClientImpl;

//! monitor client that connect to the rcssserver
class MonitorClient
    : public QObject {

    Q_OBJECT

private:

    boost::scoped_ptr< MonitorClientImpl > M_impl;

    MainData & M_main_data;

    QUdpSocket * M_socket;
    QTimer * M_timer;

    int M_version; //!< protocol version

    int M_waited_msec;


    //! not used
    MonitorClient();
    MonitorClient( const MonitorClient & );
    MonitorClient & operator=( const MonitorClient & );
public:

    MonitorClient( QObject * parent,
                   MainData & main_data,
                   const char * hostname,
                   const int port,
                   const int version );

    ~MonitorClient();

    void disconnect();

    bool isConnected() const;

private:

    void sendCommand( const rcsc::MonitorCommand & com );
    void sendCommand( const rcsc::TrainerCommand & com );

public:

    // monitor command

    void sendDispInit();
    void sendDispBye();
    void sendKickOff();
    void sendFreeKick( const double & x,
                       const double & y,
                       const rcsc::SideID side );
    void sendDropBall( const double & x,
                       const double & y )
      {
          sendFreeKick( x, y, rcsc::NEUTRAL );
      }
    void sendFreeKickLeft( const double & x,
                           const double & y )
      {
          sendFreeKick( x, y, rcsc::LEFT );
      }
    void sendFreeKickRight( const double & x,
                            const double & y )
      {
          sendFreeKick( x, y, rcsc::RIGHT );
      }

    void sendMove( const rcsc::SideID side,
                   const int unum,
                   const double & x,
                   const double & y,
                   const double & angle );

    void sendDiscard( const rcsc::SideID side,
                      const int unum );

    void sendCard( const rcsc::SideID side,
                   const int unum,
                   const rcsc::Card card );


    // trainer command

    void sendChangeMode( const rcsc::PlayMode pmode );

    void sendTrainerMoveBall( const double & x,
                              const double & y );

    void sendTrainerMoveBall( const double & x,
                              const double & y,
                              const double & vx,
                              const double & vy );

    void sendTrainerMovePlayer( const std::string & team_name,
                                const int unum,
                                const double & x,
                                const double & y );
    void sendTrainerMovePlayer( const std::string & team_name,
                                const int unum,
                                const double & x,
                                const double & y,
                                const double & angle );
    void sendTrainerMovePlayer( const std::string & team_name,
                                const int unum,
                                const double & x,
                                const double & y,
                                const double & angle,
                                const double & vx,
                                const double & vy );


    void sendRecover();

    void sencChangePlayerType( const std::string & team_name,
                               const int unum,
                               const int type );

    void sendCheckBal();

private slots:

    void handleReceive();
    void handleTimer();


signals:

    void received();
    void timeout();

};

#endif
