// -*-c++-*-

/*!
  \file trainer_data.h
  \brief trainer relate data holder class Header File.
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

#ifndef SOCCERWINDOW2_MODEL_TRAINER_DATA_H
#define SOCCERWINDOW2_MODEL_TRAINER_DATA_H

#include <rcsc/geom/vector_2d.h>
#include <rcsc/types.h>

class TrainerData {
private:

    bool M_drag_mode;

    rcsc::SideID M_dragged_player_side;
    int M_dragged_player_number;

    rcsc::PlayMode M_play_mode;

    rcsc::Vector2D M_ball_move_pos;
    rcsc::Vector2D M_ball_move_vel;

    rcsc::Vector2D M_left_move_pos[11];
    rcsc::AngleDeg M_left_body[11];

    rcsc::Vector2D M_right_move_pos[11];
    rcsc::AngleDeg M_right_body[11];


public:

    TrainerData();

    bool dragMode() const
      {
          return M_drag_mode;
      }
    void toggleDragMode()
      {
          M_drag_mode = ! M_drag_mode;
      }

    rcsc::PlayMode playMode() const
      {
          return M_play_mode;
      }
    void setPlayMode( const rcsc::PlayMode mode )
      {
          M_play_mode = mode;
      }

    const
    rcsc::Vector2D & ballPos() const
      {
          return M_ball_move_pos;
      }
    const
    rcsc::Vector2D & ballVel() const
      {
          return M_ball_move_vel;
      }
    void setBall( const rcsc::Vector2D & pos )
      {
          M_ball_move_pos = pos;
      }
    void setBall( const rcsc::Vector2D & pos,
                  const rcsc::Vector2D & vel )
      {
          M_ball_move_pos = pos;
          M_ball_move_vel = vel;
      }

    const
    rcsc::Vector2D & playerMovePos( const rcsc::SideID side,
                                    const int number ) const;

    const
    rcsc::AngleDeg & playerBody( const rcsc::SideID side,
                                 const int number ) const;

    void disablePlayer( const rcsc::SideID side,
                        const int unum );

    void setPlayer( const rcsc::SideID side,
                    const int unum,
                    const rcsc::Vector2D & pos,
                    const rcsc::AngleDeg & body );

    void setDrag( const rcsc::SideID side,
                  const int unum )
      {
          if ( M_drag_mode )
          {
              M_dragged_player_side = side;
              M_dragged_player_number = unum;
          }
      }

    void setDrag( const rcsc::SideID side,
                  const int unum,
                  const rcsc::Vector2D & pos,
                  const rcsc::AngleDeg & body )
      {
          if ( M_drag_mode )
          {
              M_dragged_player_side = side;
              M_dragged_player_number = unum;
              setPlayer( side, unum, pos, body );
          }
      }

    void unsetDrag()
      {
          M_dragged_player_side = rcsc::NEUTRAL;
          M_dragged_player_number = 0;
      }

    rcsc::SideID draggedPlayerSide() const
      {
          return M_dragged_player_side;
      }

    int draggedPlayerNumber() const
      {
          return M_dragged_player_number;
      }

    bool isPlayerDragged() const
      {
          return ( M_dragged_player_side != rcsc::NEUTRAL
                   && 1 <= M_dragged_player_number
                   && M_dragged_player_number <= 11 );
      }

    bool isDragged( const rcsc::SideID side,
                    const int unum ) const
      {
          return ( side == M_dragged_player_side
                   && unum == M_dragged_player_number );
      }

};

#endif
