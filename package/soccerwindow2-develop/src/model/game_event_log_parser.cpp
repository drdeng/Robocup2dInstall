// -*-c++-*-

/*!
  \file game_event_log_parser.cpp
  \brief game event data parser class Source File.
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

#include "game_event_log_parser.h"

#include <rcsc/types.h>

#include <iostream>
#include <cstdio>

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

*/
GameEvent::ConstPtr
GameEventLogParser::parse( const char * msg )
{
    /*
      <type> <start-time> <start-pos-x> <start-pos-y> <start-player-side> <start-player-unum>
             <end-time> <end-pos-x> <start-pos-y> <start-player-side> <start-player-unum>

     */


    const char * buf = msg;

    char type;

    int start_time, start_time_stopped;
    double start_x, start_y;
    char start_side_char;
    int start_unum;

    int end_time, end_time_stopped;
    double end_x, end_y;
    char end_side_char;
    int end_unum;

    if ( std::sscanf( buf,
                      " %c "
                      "( ( %d , %d ) ( %lf %lf ) ( %c %d ) ) "
                      "( ( %d , %d ) ( %lf %lf ) ( %c %d ) ) ",
                      &type,
                      &start_time, &start_time_stopped, &start_x, &start_y, &start_side_char, &start_unum,
                      &end_time, &end_time_stopped, &end_x, &end_y, &end_side_char, &end_unum )
         != 13 )
    {
        return GameEvent::ConstPtr();
    }

    const SideID start_side = ( start_side_char == 'l' ? LEFT : start_side_char == 'r' ? RIGHT : NEUTRAL );
    const SideID end_side = ( start_side_char == 'l' ? LEFT : start_side_char == 'r' ? RIGHT : NEUTRAL );

    GameEvent::ConstPtr ptr;

    switch ( type ) {
    case GameEvent::HOLD_ID:
        ptr = GameEvent::create_hold( GameTime( start_time, start_time_stopped ),
                                      GameTime( end_time, end_time_stopped ),
                                      Vector2D( start_x, start_y ),
                                      Vector2D( end_x, end_y ),
                                      start_side, start_unum );
        break;
    case GameEvent::PASS_ID:
        ptr = GameEvent::create_pass( GameTime( start_time, start_time_stopped ),
                                      GameTime( end_time, end_time_stopped ),
                                      Vector2D( start_x, start_y ),
                                      Vector2D( end_x, end_y ),
                                      start_side, start_unum, end_unum );
        break;
    case GameEvent::DRIBBLE_ID:
        ptr = GameEvent::create_dribble( GameTime( start_time, start_time_stopped ),
                                         GameTime( end_time, end_time_stopped ),
                                         Vector2D( start_x, start_y ),
                                         Vector2D( end_x, end_y ),
                                         start_side, start_unum );
        break;
    case GameEvent::SHOOT_ID:
        ptr = GameEvent::create_shoot( GameTime( start_time, start_time_stopped ),
                                       GameTime( end_time, end_time_stopped ),
                                       Vector2D( start_x, start_y ),
                                       Vector2D( end_x, end_y ),
                                       start_side, start_unum );
        break;
    case GameEvent::INTERCEPT_ID:
        ptr = GameEvent::create_intercept( GameTime( start_time, start_time_stopped ),
                                           GameTime( end_time, end_time_stopped ),
                                           Vector2D( start_x, start_y ),
                                           Vector2D( end_x, end_y ),
                                           start_side, start_unum,
                                           end_side, end_unum );
        break;
    case GameEvent::PLAY_STOP_ID:
        ptr = GameEvent::create_play_stop( GameTime( start_time, start_time_stopped ),
                                           GameTime( end_time, end_time_stopped ),
                                           Vector2D( start_x, start_y ),
                                           Vector2D( end_x, end_y ),
                                           start_side, start_unum );
        break;
    default:
        break;
    }

    return ptr;
}
