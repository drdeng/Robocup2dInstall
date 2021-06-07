// -*-c++-*-

/*!
  \file debug_log_holder.cpp
  \brief debug data repository Source File.
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

#include "debug_log_holder.h"

#include "debug_log_data.h"

#include <sstream>
#include <iostream>
#include <cstdio>

const std::string DebugLogHolder::DEFAULT_EXTENSION = ".log";

/*-------------------------------------------------------------------*/
/*!

*/
DebugLogHolder::~DebugLogHolder()
{
    clear();
}


/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogHolder::clear()
{
    for ( int i = 1; i <= 12; ++i )
    {
        M_data_entity[i-1].clear();
    }
}

/*-------------------------------------------------------------------*/
/*!
  \return if available file is found more then one, returun true.
*/
bool
DebugLogHolder::setDir( const std::string & team_name,
                        const rcsc::SideID side,
                        const int unum,
                        const std::string & dirpath )
{
    if ( team_name.empty() )
    {
        std::cerr << __FILE__ << ": ***ERROR*** (setDir)"
                  << " No team name!" << std::endl;
        return false;
    }

    if ( unum < 1 || 12 < unum )
    {
        std::cerr << __FILE__ << ": ***ERROR*** (setDir)"
                  << " Invalid unum! " << unum
                  << std::endl;
        return false;
    }


    std::string base_path = dirpath;
    if ( ! base_path.empty()
         && base_path[base_path.size() - 1] != '/' )
    {
        base_path += '/';
    }

    //for ( int i = 1; i <= 11; ++i )
    {
        DataEntity & de = M_data_entity[unum - 1];
        de.clear();

        de.side_ = side;

        // open file stream
        std::string filepath = base_path;
        {
            std::ostringstream ostr;
            if ( unum == 12 )
            {
                ostr << team_name << "-coach.log";
            }
            else
            {
                ostr << team_name << '-' << unum << ".log";
            }

            filepath += ostr.str();
        }

        std::cerr << __FILE__ << ": open debug file [" << filepath << "]" << std::endl;
        de.fin_.open( filepath.c_str() );

        if ( ! de.fin_.is_open()
             || ! de.fin_.good() )
        {
            std::cerr << __FILE__ << ": ***ERROR*** (setDir)"
                      << " Failed to open [" << filepath << "]"
                      << std::endl;
            de.clear();
            return false;
        }

        // record stream positions where cycle number is changed.

        std::streampos pos = 0;
        std::string buf;
        int prev_cycle = -1;
        int prev_stopped = 0;
        int cycle = 0;
        int stopped = 0;

        int n_line = 0;
        std::ifstream & fin = de.fin_;
        while ( std::getline( fin, buf ) )
        {
            ++n_line;
            // first element have to be cycle value.
            if ( std::sscanf( buf.c_str(), " %d , %d ", &cycle, &stopped ) == 2
                 || std::sscanf( buf.c_str(), " %d ", &cycle ) == 1 )
            {
                if ( cycle > prev_cycle
                     || ( cycle == prev_cycle
                          && stopped > prev_stopped ) )
                {
                    rcsc::GameTime t( cycle, stopped );
                    de.time_map_.insert( TimeMap::value_type( t, pos ) );
                    prev_cycle = cycle;
                    prev_stopped = stopped;
                }

                if ( cycle < prev_cycle )
                {
                    std::cerr << __FILE__ << ": ***ERROR*** (setDir)"
                              << " Invalid cycle! path= "<< filepath
                              << "  line= " << n_line
                              << std::endl;
                }
            }
            pos = fin.tellg();
        }

        // reset get pointer
        de.fin_.clear();
        de.fin_.seekg( 0 );

        std::cerr << __FILE__ << ": (setDir) "
                  << " unum=" << unum
                  << " lines = " << n_line
                  << " cycle size = " << de.time_map_.size()
                  << std::endl;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogHolder::readData( const int unum,
                          const rcsc::GameTime & time,
                          boost::shared_ptr< DebugLogData > data_ptr )
{
    if ( unum < 1 || 12 < unum )
    {
        return false;
    }

    DataEntity & de = M_data_entity[unum - 1];

    // get the start stream position of 'cycle'
    TimeMap::iterator it = de.time_map_.find( time );

    if ( it == de.time_map_.end() )
    {
        // no data!
        return false;
    }

    ///////////////////////////////////////////////////
    // seek & analyze data

    // seek stream position
    std::ifstream & fin = de.fin_;
    fin.clear();
    fin.seekg( it->second );

    // read loop
    char buf[8192];
    int n_line = 0;
    while ( fin.getline( buf, 8192 ) )
    {
        if ( ++n_line > 8192 * 8 )
        {
            std::cerr << __FILE__ << ": ***ERROR*** (readData)"
                      << " Too many lines! cycle =" << time << std::endl;
            break;
        }

        int read_cycle = 0;
        int read_stopped = 0;
        int n_read = 0;
        if ( std::sscanf( buf, " %d , %d %n",
                          &read_cycle, &read_stopped,
                          &n_read ) == 2
             && n_read > 0 )
        {

        }
        else if ( std::sscanf( buf, " %d %n",
                               &read_cycle,
                               &n_read ) == 1
                  && n_read > 0 )
        {

        }
        else
        {
            // illegal cycle info.
            std::cerr << __FILE__ << ": ***WARNING*** (readData)"
                      << " illegal time data. line=" << n_line << std::endl;
            continue;
        }

        // reach new cycle
        if ( read_cycle > time.cycle()
             || ( read_cycle == time.cycle()
                  && read_stopped > time.stopped() ) )
        {
            break;
        }

        if ( ! data_ptr->parse( buf + n_read ) )
        {
            std::cerr << __FILE__ << ": ***ERROR*** (readData)"
                      << " Parse Error! unum = " << unum
                      << "  cycle = " << time
                      << "  line = " << n_line
                      << std::endl;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogHolder::seekData( const int unum,
                          const rcsc::GameTime & time )
{
    if ( unum < 1 || 12 < unum )
    {
        std::cerr << __FILE__ << ": (seekData) Invalid unum " << unum
                  << std::endl;
        return false;
    }

    DataEntity & de = M_data_entity[unum - 1];

    if ( ! de.fin_.is_open() )
    {
        return false;
    }

    ///////////////////////////////////////////////////
    // get the stream position

    TimeMap::iterator it = de.time_map_.find( time );
    if ( it == de.time_map_.end() )
    {
        std::cerr << __FILE__ << ": ***ERROR*** (seekData)"
                  << " could not find stream position."
                  << " unum = " << unum
                  << " time = " << time
                  << std::endl;
        return false;
    }

    boost::shared_ptr< DebugLogData > data( new DebugLogData( time ) );
    if ( readData( unum, time, data ) )
    {
        de.data_ = data;
    }

    M_time = time;
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogHolder::incrementCycle( const int unum )
{
    if ( unum < 1 || 12 < unum )
    {
        std::cerr << __FILE__ << ": ***ERROR*** (incrementCycle)"
                  << " Invalid unum " << unum << std::endl;
        return false;
    }

    DataEntity & de = M_data_entity[unum - 1];

    if ( ! de.fin_.is_open() )
    {
        //std::cerr << __FILE__ << ": ***ERROR*** (incrementCycle)"
        //          << " File is not opened for unum = " << unum
        //          << std::endl;
        return false;
    }

    if ( ! de.data_ )
    {
        std::cerr << __FILE__ << ": ***ERROR*** (incrementCycle)"
                  << " No current data! unum = " << unum
                  << std::endl;
        return false;
    }

    // check stoppage time
    {
        rcsc::GameTime next( M_time.cycle(), M_time.stopped() + 1 );
        TimeMap::iterator it = de.time_map_.find( next );
        if ( it != de.time_map_.end() )
        {
            boost::shared_ptr< DebugLogData > data( new DebugLogData( next ) );
            if ( readData( unum, next, data ) )
            {
                de.data_ = data;
            }

            M_time = next;
            return true;
        }
    }

    // check normal time
    {
        rcsc::GameTime next( M_time.cycle() + 1, 0 );
        TimeMap::iterator it = de.time_map_.find( next );
        if ( it != de.time_map_.end() )
        {
            boost::shared_ptr< DebugLogData > data( new DebugLogData( next ) );
            if ( readData( unum, next, data ) )
            {
                de.data_ = data;
            }

            M_time = next;
            return true;
        }
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogHolder::decrementCycle( const int unum )
{
    if ( unum < 1 || 12 < unum )
    {
        std::cerr << __FILE__ << ": ***ERROR*** (decrementCycle)"
                  << " Invalid unum " << unum << std::endl;
        return false;
    }

    DataEntity & de = M_data_entity[unum - 1];

    if ( ! de.fin_.is_open() )
    {
        //std::cerr << __FILE__ << ": ***ERROR*** (decrementCycle)"
        //          << " File is not opened for unum = " << unum
        //          << std::endl;
        return false;
    }

    if ( ! de.data_ )
    {
        std::cerr << __FILE__ << ": ***ERROR*** (decrementCycle)"
                  << " No current data! unum = " << unum << std::endl;
        return false;
    }

    if ( M_time.stopped() > 0 )
    {
        // check stoppage time
        rcsc::GameTime prev( M_time.cycle(), M_time.stopped() - 1 );
        TimeMap::iterator it = de.time_map_.find( prev );
        if ( it != de.time_map_.end() )
        {
            boost::shared_ptr< DebugLogData > data( new DebugLogData( prev ) );
            if ( readData( unum, prev, data ) )
            {
                de.data_ = data;
            }

            M_time = prev;
            return true;
        }
    }
    else
    {
        // check normal time
        rcsc::GameTime prev( M_time.cycle() - 1, 0 );
        TimeMap::iterator it = de.time_map_.find( prev );
        if ( it != de.time_map_.end() )
        {
            boost::shared_ptr< DebugLogData > data( new DebugLogData( prev ) );
            if ( readData( unum, prev, data ) )
            {
                de.data_ = data;
            }

            M_time = prev;
            return true;
        }
    }

    return false;
}
