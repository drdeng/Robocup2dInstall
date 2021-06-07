// -*-c++-*-

/*!
  \file monitor_view_data.h
  \brief monitor view data type for drawing on soccerwindow Header File.
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

#ifndef SOCCERWINDOW2_MONITOR_VIEW_DATA_H
#define SOCCERWINDOW2_MONITOR_VIEW_DATA_H

#include <rcsc/rcg/types.h>
#include <rcsc/game_time.h>

#include <boost/shared_ptr.hpp>

#include <vector>

//! one cycle monitor data snapshot
class MonitorViewData {
public:

    typedef boost::shared_ptr< MonitorViewData > Ptr;
    typedef boost::shared_ptr< const MonitorViewData > ConstPtr;
    typedef std::vector< ConstPtr > Cont;


private:
    rcsc::PlayMode M_playmode;
    rcsc::rcg::TeamT M_teams[2];
    rcsc::rcg::BallT M_ball;
    std::vector< rcsc::rcg::PlayerT > M_players;
    rcsc::GameTime M_time;

    //! not used
    MonitorViewData();
    //! not used
    MonitorViewData( const MonitorViewData & );
    //! not used
    MonitorViewData & operator=( const MonitorViewData & );
public:

    //! construct from rcg v4 data (only for rcg v4)
    MonitorViewData( const rcsc::rcg::ShowInfoT & show,
                     const rcsc::PlayMode pmode,
                     const rcsc::rcg::TeamT & team_l,
                     const rcsc::rcg::TeamT & team_r );

    //! construct from rcg v3 data (only for rcg v3)
    MonitorViewData( const rcsc::rcg::short_showinfo_t2 & show,
                     const rcsc::PlayMode pmode,
                     const rcsc::rcg::TeamT & team_l,
                     const rcsc::rcg::TeamT & team_r );

    //! construct from rcg v3 data (only for monitor v2)
    MonitorViewData( const rcsc::rcg::showinfo_t2 & show );

    //! construct from rcg v1 data
    MonitorViewData( const rcsc::rcg::showinfo_t & show );


    void convertTo( rcsc::rcg::showinfo_t2 & show2 ) const;
    void convertTo( rcsc::rcg::DispInfoT & disp ) const;

    void setStoppageTime( const int stoppage );


    rcsc::PlayMode playmode() const
      {
          return M_playmode;
      }
    const std::string & playmodeString() const;

    bool isLeftSetPlay() const;
    bool isRightSetPlay() const;

    const rcsc::rcg::TeamT & leftTeam() const
      {
          return M_teams[0];
      }

    const rcsc::rcg::TeamT & rightTeam() const
      {
          return M_teams[1];
      }

    const rcsc::rcg::BallT & ball() const
      {
          return M_ball;
      }

    const std::vector< rcsc::rcg::PlayerT > & players() const
      {
          return M_players;
      }

    const rcsc::GameTime & time() const
      {
          return M_time;
      }

};

#endif
