// -*-c++-*-

/*!
  \file debug_server.cpp
  \brief Debug Server class Source File.
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

#include "debug_server.h"

#include "main_data.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

*/
DebugServer::DebugServer( QObject * parent,
                          MainData & main_data,
                          const int port )

    : QObject( parent ),
      M_socket( new QUdpSocket( this ) ),
      M_main_data( main_data )
{
    if ( ! M_socket->bind( port ) )
    {

    }

    if ( ! isConnected() )
    {
        std::cerr << __FILE__ << ": failed to initialize the udp socket"
                  << std::endl;
        return;
    }

    connect( M_socket, SIGNAL( readyRead() ),
             this, SLOT( handleReceive() ) );
}

/*-------------------------------------------------------------------*/
/*!
  destructor.
*/
DebugServer::~DebugServer()
{
    if ( isConnected() )
    {
        M_socket->close();
    }
    //std::cerr << "delete DebugServer" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugServer::isConnected() const
{
    return ( M_socket->socketDescriptor() != -1 );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugServer::handleReceive()
{
    char buf[8192];

    while ( M_socket->hasPendingDatagrams() )
    {
        int n = M_socket->readDatagram( buf, sizeof( buf ) );
        if ( n > 0 )
        {
            buf[n] = '\0';
            M_main_data.receiveDebugClientPacket( buf );
        }
    }
}
