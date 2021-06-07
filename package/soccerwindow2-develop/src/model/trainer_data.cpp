// -*-c++-*-

/*!
  \file trainer_data.cpp
  \brief trainer related data holder class Source File.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "trainer_data.h"

/*-------------------------------------------------------------------*/
/*!

*/
TrainerData::TrainerData()
    : M_drag_mode( false )
    , M_dragged_player_side( rcsc::NEUTRAL )
    , M_dragged_player_number( 0 )
    , M_play_mode( rcsc::PM_Drop_Ball )
    , M_ball_move_pos( 0.0, 0.0 )
    , M_ball_move_vel( 0.0, 0.0 )
{
    for ( int i = 0; i < 11; ++i )
    {
        M_left_move_pos[i].assign( 0.0, 0.0 );
        M_right_move_pos[i].assign( 0.0, 0.0 );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
TrainerData::setPlayer( const rcsc::SideID side,
                        const int number,
                        const rcsc::Vector2D & pos,
                        const rcsc::AngleDeg & body )
{
    if ( number < 1 || 11 < number )
    {
        return;
    }

    if ( side == rcsc::LEFT )
    {
        M_left_move_pos[number - 1] = pos;
        M_left_body[number - 1] = body;
    }
    else if ( side == rcsc::RIGHT )
    {
        M_right_move_pos[number - 1] = pos;
        M_right_body[number - 1] = body;
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
TrainerData::disablePlayer( const rcsc::SideID side,
                            const int number )
{
    if ( number < 1 || 11 < number )
    {
        return;
    }

    if ( side == rcsc::LEFT )
    {
        M_left_move_pos[number - 1].invalidate();
    }
    else if ( side == rcsc::RIGHT )
    {
        M_right_move_pos[number - 1].invalidate();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
const
rcsc::Vector2D &
TrainerData::playerMovePos( const rcsc::SideID side,
                            const int number ) const
{
    if ( number < 1 || 11 < number )
    {
        return M_left_move_pos[0];
    }

    if ( side == rcsc::LEFT )
    {
        return M_left_move_pos[number - 1];
    }
    else if ( side == rcsc::RIGHT )
    {
        return M_right_move_pos[number - 1];
    }

    return M_left_move_pos[0];
}

/*-------------------------------------------------------------------*/
/*!

*/
const
rcsc::AngleDeg &
TrainerData::playerBody( const rcsc::SideID side,
                         const int number ) const
{
    if ( number < 1 || 11 < number )
    {
        return M_left_body[0];
    }

    if ( side == rcsc::LEFT )
    {
        return M_left_body[number - 1];
    }
    else if ( side == rcsc::RIGHT )
    {
        return M_right_body[number - 1];
    }

    return M_left_body[0];
}
