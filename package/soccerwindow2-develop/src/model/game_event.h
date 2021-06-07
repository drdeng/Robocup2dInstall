// -*-c++-*-

/*!
  \file game_event.h
  \brief game event class Header File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa Akiyama

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

#ifndef SOCCERWINDOW2_MODEL_GAME_EVENT_H
#define SOCCERWINDOW2_MODEL_GAME_EVENT_H

#include <rcsc/geom/vector_2d.h>
#include <rcsc/game_time.h>
#include <rcsc/types.h>

#include <boost/shared_ptr.hpp>

#include <vector>

class GameEvent {
public:

    typedef boost::shared_ptr< GameEvent > Ptr;
    typedef boost::shared_ptr< const GameEvent > ConstPtr;
    typedef std::vector< ConstPtr > Cont;

    enum Type {
        HOLD,
        PASS,
        DRIBBLE,
        SHOOT,
        INTERCEPT,
        PLAY_STOP,
    };

    static const char HOLD_ID = 'h';
    static const char PASS_ID = 'p';
    static const char DRIBBLE_ID = 'd';
    static const char SHOOT_ID = 's';
    static const char INTERCEPT_ID = 'i';
    static const char PLAY_STOP_ID = 'P';

private:

    Type M_type;

    rcsc::GameTime M_start_time;
    rcsc::Vector2D M_start_pos;
    rcsc::SideID M_start_player_side;
    int M_start_player_unum;

    rcsc::GameTime M_end_time;
    rcsc::Vector2D M_end_pos;
    rcsc::SideID M_end_player_side;
    int M_end_player_unum;


    GameEvent( const Type type,
               const rcsc::GameTime & start_time,
               const rcsc::Vector2D & start_pos,
               const rcsc::SideID start_player_side,
               const int start_player_unum,
               const rcsc::GameTime & end_time,
               const rcsc::Vector2D & end_pos,
               const rcsc::SideID end_player_side,
               const int end_player_unum )
        : M_type( type ),
          M_start_time( start_time ),
          M_start_pos( start_pos ),
          M_start_player_side( start_player_side ),
          M_start_player_unum( start_player_unum ),
          M_end_time( end_time ),
          M_end_pos( end_pos ),
          M_end_player_side( end_player_side ),
          M_end_player_unum( end_player_unum )
      { }

public:

    static
    ConstPtr create_hold( const rcsc::GameTime & start_time,
                          const rcsc::GameTime & end_time,
                          const rcsc::Vector2D & start_pos,
                          const rcsc::Vector2D & end_pos,
                          const rcsc::SideID player_side,
                          const int player_unum )
      {
          return ConstPtr( new GameEvent( HOLD,
                                          start_time, start_pos, player_side, player_unum,
                                          end_time, end_pos, player_side, player_unum ) );
      }

    static
    ConstPtr create_pass( const rcsc::GameTime & start_time,
                          const rcsc::GameTime & end_time,
                          const rcsc::Vector2D & start_pos,
                          const rcsc::Vector2D & end_pos,
                          const rcsc::SideID player_side,
                          const int start_player_unum,
                          const int end_player_unum )
      {
          return ConstPtr( new GameEvent( PASS,
                                          start_time, start_pos, player_side, start_player_unum,
                                          end_time, end_pos, player_side, end_player_unum ) );
      }

    static
    ConstPtr create_dribble( const rcsc::GameTime & start_time,
                             const rcsc::GameTime & end_time,
                             const rcsc::Vector2D & start_pos,
                             const rcsc::Vector2D & end_pos,
                             const rcsc::SideID player_side,
                             const int player_unum )
      {
          return ConstPtr( new GameEvent( DRIBBLE,
                                          start_time, start_pos, player_side, player_unum,
                                          end_time, end_pos, player_side, player_unum ) );
      }

    static
    ConstPtr create_shoot( const rcsc::GameTime & start_time,
                           const rcsc::GameTime & end_time,
                           const rcsc::Vector2D & start_pos,
                           const rcsc::Vector2D & end_pos,
                           const rcsc::SideID player_side,
                           const int player_unum )
      {
          return ConstPtr( new GameEvent( SHOOT,
                                          start_time, start_pos, player_side, player_unum,
                                          end_time, end_pos, rcsc::NEUTRAL, rcsc::Unum_Unknown ) );
      }

    static
    ConstPtr create_intercept( const rcsc::GameTime & start_time,
                               const rcsc::GameTime & end_time,
                               const rcsc::Vector2D & start_pos,
                               const rcsc::Vector2D & end_pos,
                               const rcsc::SideID start_player_side,
                               const int start_player_unum,
                               const rcsc::SideID end_player_side,
                               const int end_player_unum )
      {
          return ConstPtr( new GameEvent( INTERCEPT,
                                          start_time, start_pos, start_player_side, start_player_unum,
                                          end_time, end_pos, end_player_side, end_player_unum ) );
      }


    static
    ConstPtr create_play_stop( const rcsc::GameTime & start_time,
                               const rcsc::GameTime & end_time,
                               const rcsc::Vector2D & start_pos,
                               const rcsc::Vector2D & end_pos,
                               const rcsc::SideID player_side,
                               const int player_unum )
      {
          return ConstPtr( new GameEvent( PLAY_STOP,
                                          start_time, start_pos, player_side, player_unum,
                                          end_time, end_pos, player_side, player_unum ) );
      }



    Type type() const
      {
          return M_type;
      }

    const rcsc::GameTime & startTime() const
      {
          return M_start_time;
      }
    const rcsc::Vector2D & startPos() const
      {
          return M_start_pos;
      }
    rcsc::SideID startPlayerSide() const
      {
          return M_start_player_side;
      }
    int startPlayerUnum() const
      {
          return M_start_player_unum;
      }

    const rcsc::GameTime & endTime() const
      {
          return M_end_time;
      }
    const rcsc::Vector2D & endPos() const
      {
          return M_end_pos;
      }
    rcsc::SideID endPlayerSide() const
      {
          return M_end_player_side;
      }
    int endPlayerUnum() const
      {
          return M_end_player_unum;
      }
};

#endif
