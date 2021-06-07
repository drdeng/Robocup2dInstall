// -*-c++-*-

/*!
  \file agent_data_holder.h
  \brief template holder class for each agent/time Header File.
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

#ifndef SOCCERWINDOW2_AGENT_DATA_HOLDER_H
#define SOCCERWINDOW2_AGENT_DATA_HOLDER_H

#include "agent_id.h"

#include <rcsc/types.h>
#include <rcsc/game_time.h>

#include <boost/shared_ptr.hpp>

#include <map>
#include <utility>

template< typename T >
class AgentDataHolder {
private:

    class Less {
    private:
        bool lessTime( const rcsc::GameTime & lhs,
                       const rcsc::GameTime & rhs ) const
          {
              if ( lhs.cycle() == rhs.cycle() )
              {
                  return ( lhs.stopped() < rhs.stopped() );
              }
              else
              {
                  return ( lhs.cycle() < rhs.cycle() );
              }
          }

    public:
        bool operator()( const std::pair< rcsc::GameTime, AgentID > & lhs,
                         const std::pair< rcsc::GameTime, AgentID > & rhs ) const
          {
              if ( lessTime( lhs.first, rhs.first ) )
              {
                  return true;
              }
              else if ( lessTime( rhs.first, lhs.first ) )
              {
                  return false;
              }
              else
              {
                  return ( lhs.second < rhs.second );
              }
          }
    };

public:
    typedef boost::shared_ptr< T > Ptr;
    typedef boost::shared_ptr< const T > ConstPtr;
    typedef std::map< std::pair< rcsc::GameTime, AgentID >, Ptr, Less > MapType;

private:
    MapType M_map;

public:

    void set( const rcsc::GameTime & time,
              const AgentID & agent,
              const Ptr & data )
      {
          M_map[ std::make_pair( time, agent ) ] = data;
      }

    boost::shared_ptr< T > get( const rcsc::GameTime & time,
                                const AgentID & agent )
      {
          typename MapType::const_iterator it = M_map.find( std::make_pair( time, agent ) );
          if ( it == M_map.end() )
          {
              return boost::shared_ptr<T>();
          }
          else
          {
              return (*it).second;
          }
      }

    boost::shared_ptr< const T > get( const rcsc::GameTime & time,
                                      const AgentID & agent ) const
      {
          typename MapType::const_iterator it = M_map.find( std::make_pair( time, agent ) );
          if ( it == M_map.end() )
          {
              return boost::shared_ptr<const T>();
          }
          else
          {
              return (*it).second;
          }
      }

    void clear()
    {
        M_map.clear();
    }
};

#endif
