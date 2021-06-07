// -*-c++-*-

/*!
  \file action_sequence_log_parser.cpp
  \brief parser class to parse action sequence log format Source File.
*/

/*
 *Copyright:

 Copyright (C) Hiroki SHIMORA, Hidehisa AKIYAMA

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "action_sequence_log_parser.h"

#include <rcsc/geom/vector_2d.h>

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>

/*-------------------------------------------------------------------*/
/*!

*/
ActionSequenceHolder::Ptr
ActionSequenceLogParser::parse( std::istream & in )
{
    std::string line;

    ActionSequenceHolder::Ptr data( new ActionSequenceHolder() );

    boost::shared_ptr< ActionSequenceDescription > seq;
    std::vector< std::string > evaluation_details;
    std::string ranking_data;

    while ( std::getline( in, line ) )
    {
        if ( line.empty() ) continue;
        if ( line[0] == '>' || line[0] == '<' ) continue;
        if ( line[0] == 'C' ) continue; // "CYCLE" data line

        if ( line[0] == '(' )
        {
            if ( line.compare( 0, 7, "(eval) " ) == 0 )
            {
                evaluation_details.push_back( line.substr( 7 ) );
                continue;
            }
            else if ( line.compare( 0, 7, "(rank) " ) == 0 )
            {
                ranking_data = line.substr( 7 );
                continue;
            }
        }
        else if ( line[0] == '_' )
        {
            //
            // read each action
            //
            if ( ! seq )
            {
                std::cerr << __FILE__ << ": !! skip[" << line << "]" << std::endl;
                continue;
            }

            int n_read = 0;
            int depth = 0;
            int node_index = 0;
            char type[16];

            char action_name[256];
            int action_number = 0;
            int kick_count = 0;
            int duration_time = 0;
            int from = -1;
            double from_x = 0.0;
            double from_y = 0.0;
            int to = -1;
            double to_x = 0.0;
            double to_y = 0.0;
            int safe_level = 0.0;
            double value = 0.0;

            ActionDescription act;

            if ( std::sscanf( line.c_str(),
                              "__ %d: %d %15s %n",
                              &depth, &node_index, type, &n_read ) != 3
                 || n_read == 0 )
            {
                std::cerr << __FILE__ << ": !! skip[" << line << "]" << std::endl;
                continue;
            }

            act.setId( node_index );
            act.setDescription( line );

            if ( ! std::strcmp( type, "pass" ) )
            {
                if ( std::sscanf( line.c_str() + n_read,
                                  " (%255[0-9a-zA-Z_-][%d]) k=%d t=%d"
                                  " from[%d](%lf %lf)-to[%d](%lf %lf), safe=%d value=%lf",
                                  action_name, &action_number, &kick_count, &duration_time,
                                  &from, &from_x, &from_y,
                                  &to, &to_x, &to_y,
                                  &safe_level, &value ) == 12 )
                {
                    act.setPass( action_name, action_number, kick_count, duration_time,
                                 from, from_x, from_y,
                                 to, to_x, to_y,
                                 safe_level, value );
                    seq->add( act );
                    continue;
                }
            }
            else if ( ! std::strcmp( type, "dribble" ) )
            {
                if ( std::sscanf( line.c_str() + n_read,
                                  " (%255[0-9a-zA-Z_-][%d]) t=%d"
                                  " from[%d](%lf %lf)-to(%lf %lf), safe=%d value=%lf",
                                  action_name, &action_number, &duration_time,
                                  &from, &from_x, &from_y,
                                  &to_x, &to_y, &safe_level, &value ) == 10 )
                {
                    act.setDribble( action_name, action_number, duration_time,
                                    from, from_x, from_y,
                                    to_x, to_y,
                                    safe_level, value );
                    seq->add( act );
                    continue;
                }
            }
            else if ( ! std::strcmp( type, "shoot" ) )
            {
                if ( std::sscanf( line.c_str() + n_read,
                                  " (%255[0-9a-zA-Z_-]) t=%d"
                                  " from[%d](%lf %lf)-to(%lf %lf), safe=%d value=%lf",
                                  action_name, &duration_time,
                                  &from, &from_x, &from_y,
                                  &to_x, &to_y, &safe_level, &value ) == 9 )
                {
                    act.setShoot( action_name, duration_time,
                                  from, from_x, from_y,
                                  to_x, to_y,
                                  safe_level, value );
                    seq->add( act );
                    continue;
                }
            }
            else if ( ! std::strcmp( type, "hold" ) )
            {
                if ( std::sscanf( line.c_str() + n_read,
                                  " (%255[0-9a-zA-Z_-]) t=%d"
                                  " from[%d](%lf %lf), safe=%d value=%lf",
                                  action_name, &duration_time,
                                  &from, &from_x, &from_y,
                                  &safe_level, &value ) == 7 )
                {
                    act.setHold( action_name, duration_time,
                                 from, from_x, from_y,
                                 safe_level, value );
                    seq->add( act );
                    continue;
                }
            }
            else if ( ! std::strcmp( type, "move" ) )
            {
                if ( std::sscanf( line.c_str() + n_read,
                                  " (%255[0-9a-zA-Z_-]) t=%d"
                                  " from[%d](%lf %lf)-to(%lf %lf), safe=%d value=%lf",
                                  action_name, &duration_time,
                                  &from, &from_x, &from_y,
                                  &to_x, &to_y, &safe_level, &value ) == 9 )
                {
                    act.setMove( action_name, duration_time,
                                 from, from_x, from_y,
                                 to_x, to_y,
                                 safe_level, value );
                    seq->add( act );
                    continue;
                }
            }
        }
        else
        {
            //
            // read header [id, evaluation]
            //
            int id = -1;
            double evaluation = 0.0;
            int n_read = 0;
            if ( std::sscanf( line.c_str(),
                              "%d: evaluation=%lf%n",
                              &id, &evaluation, &n_read ) == 2
                 && n_read != 0 )
            {
                if ( seq )
                {
                    data->add( seq );
                }
                seq = boost::shared_ptr< ActionSequenceDescription >( new ActionSequenceDescription( id ) );
                seq->setValue( evaluation );
                seq->setEvaluationDescription( evaluation_details );
                seq->setRankingData( ranking_data );
                evaluation_details.clear();
                continue;
            }
        }

        std::cerr << __FILE__ << ": !! skip[" << line << "]" << std::endl;
    }

    if ( seq
         && ! seq->actions().empty() )
    {
        data->add( seq );
    }

    return data;
}
