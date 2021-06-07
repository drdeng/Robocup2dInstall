// -*-c++-*-

/*!
  \file monitor_view_data.cpp
  \brief monitor view data type for drawing on soccerwindow Source File.
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

#include "monitor_view_data.h"

#include <rcsc/rcg/util.h>

/*-------------------------------------------------------------------*/
/*!

*/
MonitorViewData::MonitorViewData( const rcsc::rcg::ShowInfoT & show,
                                  const rcsc::PlayMode pmode,
                                  const rcsc::rcg::TeamT & team_l,
                                  const rcsc::rcg::TeamT & team_r )
    : M_playmode( pmode ),
      M_ball( show.ball_ ),
      M_time( show.time_, 0 )
{
    M_teams[0] = team_l;
    M_teams[1] = team_r;

    M_players.reserve( rcsc::MAX_PLAYER * 2 );
    for ( int i = 0; i < rcsc::MAX_PLAYER*2 ; ++i )
    {
        M_players.push_back( show.player_[i] );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
MonitorViewData::MonitorViewData( const rcsc::rcg::short_showinfo_t2 & show,
                                  const rcsc::PlayMode pmode,
                                  const rcsc::rcg::TeamT & team_l,
                                  const rcsc::rcg::TeamT & team_r )
    : M_playmode( pmode ),
      M_time( rcsc::rcg::nstohi( show.time ), 0 )
{
    if ( M_time.cycle() == 65535 ) M_time.setCycle( 0 );

    M_teams[0] = team_l;
    M_teams[1] = team_r;

    rcsc::rcg::convert( show.ball, M_ball );

    M_players.reserve( rcsc::MAX_PLAYER * 2 );
    for ( int i = 0; i < rcsc::MAX_PLAYER ; ++i )
    {
        rcsc::rcg::PlayerT p;
        rcsc::rcg::convert( rcsc::LEFT, i + 1, show.pos[i], p );
        M_players.push_back( p );
    }
    for ( int i = rcsc::MAX_PLAYER; i < rcsc::MAX_PLAYER * 2 ; ++i )
    {
        rcsc::rcg::PlayerT p;
        rcsc::rcg::convert( rcsc::RIGHT, i + 1 - rcsc::MAX_PLAYER, show.pos[i], p );
        M_players.push_back( p );
    }
}

/*-------------------------------------------------------------------*/
/*!
  \param show monitor protocol v2 view data
*/
MonitorViewData::MonitorViewData( const rcsc::rcg::showinfo_t2 & show )
    : M_playmode( static_cast< rcsc::PlayMode >( show.pmode ) ),
      M_time( rcsc::rcg::nstohi( show.time ), 0 )
{
    if ( M_time.cycle() == 65535 ) M_time.setCycle( 0 );

    rcsc::rcg::convert( show.team[0], M_teams[0] );
    rcsc::rcg::convert( show.team[1], M_teams[1] );

    rcsc::rcg::convert( show.ball, M_ball );

    M_players.reserve( rcsc::MAX_PLAYER * 2 );
    for ( int i = 0; i < rcsc::MAX_PLAYER; ++i )
    {
        rcsc::rcg::PlayerT p;
        rcsc::rcg::convert( rcsc::LEFT,
                            i + 1,
                            show.pos[i],
                            p );
        M_players.push_back( p );
    }
    for ( int i = rcsc::MAX_PLAYER;
          i < rcsc::MAX_PLAYER * 2;
          ++i )
    {
        rcsc::rcg::PlayerT p;
        rcsc::rcg::convert( rcsc::RIGHT,
                            i - rcsc::MAX_PLAYER + 1,
                            show.pos[i],
                            p );
        M_players.push_back( p );
    }
}

/*-------------------------------------------------------------------*/
/*!
  \param show monitor v1 or RCG v1,v2 view data
*/
MonitorViewData::MonitorViewData( const rcsc::rcg::showinfo_t & show )
    : M_playmode( static_cast< rcsc::PlayMode >( show.pmode ) ),
      M_time( rcsc::rcg::nstohi( show.time ), 0 )
{
    if ( M_time.cycle() == 65535 ) M_time.setCycle( 0 );

    rcsc::rcg::convert( show.team[0], M_teams[0] );
    rcsc::rcg::convert( show.team[1], M_teams[1] );

    rcsc::rcg::convert( show.pos[0], M_ball );

    // In old format, players' index is started from '1'.
    M_players.reserve( rcsc::MAX_PLAYER * 2 );
    for ( int i = 0; i < rcsc::MAX_PLAYER*2; ++i )
    {
        rcsc::rcg::PlayerT p;
        rcsc::rcg::convert( show.pos[i], p );
        M_players.push_back( p );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorViewData::convertTo( rcsc::rcg::showinfo_t2 & show2 ) const
{
    show2.pmode = static_cast< char >( playmode() );

    rcsc::rcg::convert( M_teams[0], show2.team[0] );
    rcsc::rcg::convert( M_teams[1], show2.team[1] );

    rcsc::rcg::convert( M_ball, show2.ball );

    int i = 0;
    for ( std::vector< rcsc::rcg::PlayerT >::const_iterator p = players().begin(),
              end = players().end();
          p != end;
          ++p, ++i )
    {
        rcsc::rcg::convert( *p, show2.pos[i] );
    }

    show2.time = rcsc::rcg::hitons( M_time.cycle() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorViewData::convertTo( rcsc::rcg::DispInfoT & disp ) const
{
    disp.pmode_ = playmode();

    disp.team_[0] = M_teams[0];
    disp.team_[1] = M_teams[1];

    disp.show_.ball_ = M_ball;

    int i = 0;
    for ( std::vector< rcsc::rcg::PlayerT >::const_iterator p = players().begin(),
              end = players().end();
          p != end;
          ++p, ++i )
    {
        disp.show_.player_[i] = *p;
    }

    disp.show_.time_ = M_time.cycle();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorViewData::setStoppageTime( const int stoppage )
{
    M_time.setStopped( stoppage );
}

/*-------------------------------------------------------------------*/
/*!

*/
const std::string &
MonitorViewData::playmodeString() const
{
    static std::string s_unknown( "unknown_playmode" );
    static const std::string s_playmode_str[] = PLAYMODE_STRINGS;

    if ( playmode() <= rcsc::PM_Null
         || rcsc::PM_MAX <= playmode() )
    {
        return s_unknown;
    }

    return s_playmode_str[M_playmode];
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
MonitorViewData::isLeftSetPlay() const
{
    switch ( playmode() ) {
    case rcsc::PM_KickOff_Left:
    case rcsc::PM_KickIn_Left:
    case rcsc::PM_FreeKick_Left:
    case rcsc::PM_CornerKick_Left:
    case rcsc::PM_GoalKick_Left:
    case rcsc::PM_OffSide_Right:
    case rcsc::PM_Back_Pass_Right:
    case rcsc::PM_Free_Kick_Fault_Right:
    case rcsc::PM_CatchFault_Right:
    case rcsc::PM_IndFreeKick_Left:
    case rcsc::PM_PenaltySetup_Left:
    case rcsc::PM_PenaltyReady_Left:
        return true;
    default:
        break;
    }
    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
MonitorViewData::isRightSetPlay() const
{
    switch ( playmode() ) {
    case rcsc::PM_KickOff_Right:
    case rcsc::PM_KickIn_Right:
    case rcsc::PM_FreeKick_Right:
    case rcsc::PM_CornerKick_Right:
    case rcsc::PM_GoalKick_Right:
    case rcsc::PM_OffSide_Left:
    case rcsc::PM_Back_Pass_Left:
    case rcsc::PM_Free_Kick_Fault_Left:
    case rcsc::PM_CatchFault_Left:
    case rcsc::PM_IndFreeKick_Right:
    case rcsc::PM_PenaltySetup_Right:
    case rcsc::PM_PenaltyReady_Right:
        return true;
    default:
        break;
    }
    return false;
}
