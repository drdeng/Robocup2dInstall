// -*-c++-*-

/*!
  \file game_event_holder.cpp
  \brief game event holder Source File.
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

#include "game_event_holder.h"

#include "game_event_log_parser.h"
#include "game_event.h"

#include <iostream>
#include <fstream>
#include <cstdio>

/*-------------------------------------------------------------------*/
/*!

*/
GameEventHolder::~GameEventHolder()
{
    clear();
}


/*-------------------------------------------------------------------*/
/*!

*/
void
GameEventHolder::clear()
{
    M_events.clear();
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
GameEventHolder::open( const std::string & filepath )
{
    std::ifstream fin( filepath.c_str() );

    if ( ! fin )
    {
        std::cerr << __FILE__ << ": (open) could not open the file ["
                  << filepath << ']' << std::endl;
        return false;
    }

    GameEventLogParser parser;

    int n_line = 0;
    std::string line_buf;
    while ( std::getline( fin, line_buf ) )
    {
        ++n_line;
        GameEvent::ConstPtr ptr = parser.parse( line_buf.c_str() );
        if ( ptr )
        {
            M_events.push_back( ptr );
        }
    }

    std::cout << __FILE__ << ": (open) read " << n_line << " lines." << std::endl;
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
GameEventHolder::save( const std::string & filepath )
{
    std::ofstream fout( filepath.c_str() );

    if ( ! fout )
    {
        std::cerr << __FILE__ << ": (save) could not open the file ["
                  << filepath << ']' << std::endl;
        return false;
    }

    return true;
}
