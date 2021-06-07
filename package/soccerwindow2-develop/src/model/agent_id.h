// -*-c++-*-

/*!
  \file agent_id.h
  \brief unique key class of agent in a game Header File.
*/

/*
 *Copyright:

 Copyright (C) Hiroki SHIMORA

 This code is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
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

#ifndef SOCCERWINDOW2_AGENT_ID_H
#define SOCCERWINDOW2_AGENT_ID_H

#include <rcsc/types.h>

/*!
  \class AgentID
  \brief unique key class of agent in a game
*/
class AgentID {
private:
    rcsc::SideID M_side;
    int M_unum;

public:

    AgentID()
        : M_side( rcsc::NEUTRAL ),
          M_unum( rcsc::Unum_Unknown )
      { }

    AgentID( const rcsc::SideID side,
             const int unum )
        : M_side( side ),
          M_unum( unum )
      { }

    rcsc::SideID side() const
      {
          return M_side;
      }

    int unum() const
      {
          return M_unum;
      }


    bool isNull() const
      {
          return M_side == rcsc::NEUTRAL
              || M_unum == rcsc::Unum_Unknown;
      }

    bool operator<( const AgentID & a ) const
      {
          if ( M_side == a.M_side )
          {
              return M_unum < a.M_unum;
          }
          else
          {
              return ( static_cast< int >( M_side ) < static_cast< int >( a.M_side ) );
          }
      }
};

#endif
