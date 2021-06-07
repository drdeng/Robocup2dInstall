// -*-c++-*-

/*!
  \file debug_server.h
  \brief Debug Server class Header File.
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

#ifndef SOCCERWINDOW2_QT_DEBUG_SERVER_H
#define SOCCERWINDOW2_QT_DEBUG_SERVER_H

#include <QObject>

class QUdpSocket;

class MainData;

struct DebugServerImpl;

class DebugServer
    : public QObject {

    Q_OBJECT

private:
    QUdpSocket * M_socket;

    MainData & M_main_data;

    //! not used
    DebugServer();
    DebugServer( const DebugServer & );
    DebugServer & operator=( const DebugServer & );
public:

    DebugServer( QObject * parent,
                 MainData & main_data,
                 const int port );

    ~DebugServer();

    //! check if socket is opened.
    bool isConnected() const;

private slots:

    void handleReceive();

};

#endif
