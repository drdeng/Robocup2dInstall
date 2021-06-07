// -*-c++-*-

/*!
  \file debug_log_holder.h
  \brief debug data repository Header File.
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

#ifndef SOCCERWINDOW2_DEBUG_LOG_HOLDER_H
#define SOCCERWINDOW2_DEBUG_LOG_HOLDER_H

#include <rcsc/game_time.h>
#include <rcsc/types.h>

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include <cassert>
#include <string>
#include <fstream>
#include <list>
#include <map>

class DebugLogData;

//! streaming debug data repository
class DebugLogHolder {

    // key: game time, value: streampos at cycle is changed
    typedef std::map< rcsc::GameTime, std::streampos, rcsc::GameTime::Less > TimeMap;

private:
    static const std::string DEFAULT_EXTENSION;

    struct DataEntity {
        std::ifstream fin_; // input file stream
        rcsc::SideID side_;
        boost::shared_ptr< const DebugLogData > data_;
        TimeMap time_map_;

        void clear()
          {
              if ( fin_.is_open() )
              {
                  fin_.close();
              }
              side_ = rcsc::NEUTRAL;
              time_map_.clear();
          }
    };

    boost::int32_t M_level; //!< current log level;

    rcsc::GameTime M_time; //!< current focus cycle

    DataEntity M_data_entity[12];

    // noncopyable
    DebugLogHolder( const DebugLogHolder & );
    DebugLogHolder & operator=( const DebugLogHolder & );

public:

    DebugLogHolder()
        : M_level( 0xffffffff )
        , M_time( 0, 0 )
      { }

    ~DebugLogHolder();

    void clear();

    bool setDir( const std::string & team_name,
                 const rcsc::SideID side,
                 const int unum,
                 const std::string & dir_path );

    bool incrementCycle( const int unum );
    bool decrementCycle( const int unum );

    bool seekData( const int unum,
                   const rcsc::GameTime & time );

    boost::int32_t level() const
      {
          return M_level;
      }

    void setLevel( const boost::int32_t level,
                   const bool enable )
      {
          if ( enable )
          {
              M_level |= level;
          }
          else
          {
              M_level &= ~level;
          }
      }

    const rcsc::GameTime currentTime() const
      {
          return M_time;
      }

    const boost::shared_ptr< const DebugLogData > getData( const int unum ) const
      {
          return ( ( unum < 1 || 12 < unum )
                   ? boost::shared_ptr< const DebugLogData >()
                   : M_data_entity[unum - 1].data_ );
      }

private:

    bool readData( const int unum,
                   const rcsc::GameTime & time,
                   boost::shared_ptr< DebugLogData > data_ptr );

};

#endif
