// -*-c++-*-

/*!
  \file view_holder.h
  \brief view data repository Header File.
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

#ifndef SOCCERWINDOW2_MODEL_VIEW_HOLDER_H
#define SOCCERWINDOW2_MODEL_VIEW_HOLDER_H

#include "monitor_view_data.h"
#include "debug_view_data.h"

#include <rcsc/common/player_type.h>
#include <rcsc/common/team_graphic.h>
#include <rcsc/rcg/handler.h>
#include <rcsc/game_time.h>

#include <map>
#include <ostream>

class AgentID;

class ViewHolder
    : public rcsc::rcg::Handler {
private:

    //! last changed playmode
    rcsc::PlayMode M_last_playmode;
    //! last changed left team info
    rcsc::rcg::TeamT M_last_team_left;
    //! last changed right team info
    rcsc::rcg::TeamT M_last_team_right;

    //! the set of score changed index
    std::vector< std::size_t > M_score_change_indices;

    //! the set of penalty score/miss event. first: time, second: playmode
    std::vector< std::pair< int, rcsc::PlayMode > > M_penalty_scores_left;
    std::vector< std::pair< int, rcsc::PlayMode > > M_penalty_scores_right;

    // default type
    rcsc::PlayerType M_default_type;
    //! player type set
    std::map< int, rcsc::PlayerType > M_player_types;

    //! last read view data
    MonitorViewData::ConstPtr M_last_monitor_view;

    //! view data container.
    MonitorViewData::Cont M_monitor_view_cont;

    //! debug view data set for left players (key:cycle)
    DebugViewData::Map M_left_debug_view;

    //! debug view data set for right players (key:cycle)
    DebugViewData::Map M_right_debug_view;

    rcsc::TeamGraphic M_team_graphic_left; //! left team graphic
    rcsc::TeamGraphic M_team_graphic_right; //! right team graphic

    //! nocopyable
    ViewHolder( const ViewHolder & );
    //! nocopyable
    ViewHolder & operator=( const ViewHolder & );

public:
    //! constructor.
    ViewHolder();

    ~ViewHolder();
    //--------------------------------------------------
    //! clear all stored data
    void clear();

    //! save a stored rcg data
    std::ostream & saveRCG( std::ostream & os ) const;

    //! save debug view data to directory
    void saveDebugView( const std::string & dir_path ) const;

    //! load debug view data from directory
    void openDebugView( const std::string & dir_path );

    //--------------------------------------------------
    // override methods.

    virtual
    bool handleEOF();

    virtual
    bool handleShow( const rcsc::rcg::ShowInfoT & show );
    virtual
    bool handleMsg( const int time,
                    const int board,
                    const std::string & msg );
    virtual
    bool handleDraw( const int time,
                     const rcsc::rcg::drawinfo_t & draw );
    virtual
    bool handlePlayMode( const int time,
                         const rcsc::PlayMode pm );
    virtual
    bool handleTeam( const int time,
                     const rcsc::rcg::TeamT & team_l,
                     const rcsc::rcg::TeamT & team_r );

    virtual
    bool handlePlayerType( const std::string & msg );
    virtual
    bool handleServerParam( const std::string & msg );
    virtual
    bool handlePlayerParam( const std::string & msg );


    //--------------------------------------------------

    //! set debug view data
    void addDebugView( const rcsc::GameTime & time,
                       const char side,
                       const int unum,
                       DebugViewData::Ptr debug_view );

    //--------------------------------------------------

    rcsc::PlayMode lastPlayMode() const
      {
          return M_last_playmode;
      }

    //! get const reference of data container.
    const MonitorViewData::Cont & monitorViewCont() const
      {
          return M_monitor_view_cont;
      }

    //! get debug view data container
    const DebugViewData::Map & leftDebugView() const
      {
          return M_left_debug_view;
      }

    //! get debug view data container
    const DebugViewData::Map & rightDebugView() const
      {
          return M_right_debug_view;
      }

    // get debug view data
    DebugViewData::ConstPtr getDebugView( const rcsc::GameTime & time,
                                          const AgentID & player ) const;

    const std::map< int, rcsc::PlayerType > & playerTypeCont() const
      {
          return M_player_types;
      }

    const
    rcsc::PlayerType & playerType( const int id ) const;

    const
    std::vector< std::size_t > & scoreChangeIndices() const
      {
          return M_score_change_indices;
      }

    const
    std::vector< std::pair< int, rcsc::PlayMode > > & penaltyScoresLeft() const
      {
          return M_penalty_scores_left;
      }

    const
    std::vector< std::pair< int, rcsc::PlayMode > > & penaltyScoresRight() const
      {
          return M_penalty_scores_right;
      }

    MonitorViewData::ConstPtr getViewData( const std::size_t idx ) const;

    MonitorViewData::ConstPtr lastMonitorView() const
      {
          return M_last_monitor_view;
      }

    std::size_t getIndexOf( const int cycle ) const;
    std::size_t getIndexOf( const rcsc::GameTime & t ) const;

    const
    rcsc::TeamGraphic & teamGraphicLeft() const
      {
          return M_team_graphic_left;
      }

    const
    rcsc::TeamGraphic & teamGraphicRight() const
      {
          return M_team_graphic_right;
      }

private:
    bool saveDebugView( const std::string & dir_path,
                        const std::string & team_name,
                        const int unum,
                        const DebugViewData::Map & data ) const;

    bool openDebugView( const std::string & dir_path,
                        const std::string & team_name,
                        const int unum );

    void addMonitorViewData( MonitorViewData::ConstPtr ptr );

    bool analyzeTeamGraphic( const std::string & msg );
};

#endif
