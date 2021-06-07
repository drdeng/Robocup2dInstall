// -*-c++-*-

/*!
  \file main_data.h
  \brief main data supplier class Header File.
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

#ifndef SOCCERWINDOW2_MAIN_DATA_H
#define SOCCERWINDOW2_MAIN_DATA_H

#include "debug_log_holder.h"
#include "trainer_data.h"
#include "view_holder.h"
#include "grid_field_evaluation_data.h"
#include "action_sequence_description.h"
#include "agent_data_holder.h"
#include "draw_data_holder.h"

class MainData {
private:

    TrainerData M_trainer_data;

    //! view data holder
    ViewHolder M_view_holder;

    //! current view data index in M_view_holder
    std::size_t M_view_index;

    DebugLogHolder M_debug_log_holder;

    int M_action_sequence_id;
    rcsc::GameTime M_action_sequence_time;
    boost::shared_ptr< const ActionSequenceHolder > M_action_sequence_holder;
    AgentDataHolder< GridFieldEvaluationData > M_grid_field_evaluation_holder;

    DrawDataHolder M_draw_data_holder;

    // not used
    MainData( const MainData & );
    const MainData & operator=( const MainData & );

public:

    MainData();
    ~MainData();

    void clear();

    bool openRCG( const std::string & file_path );
    bool saveRCG( const std::string & file_path ) const;

    void openDebugView( const std::string & dir_path )
      {
          M_view_holder.openDebugView( dir_path );
      }
    void saveDebugView( const std::string & dir_path ) const
      {
          M_view_holder.saveDebugView( dir_path );
      }

    bool openDrawData( const std::string & filepath );


    bool receiveMonitorPacket( const char * message,
                               const int client_version );
    bool receiveMonitorPacket( const rcsc::rcg::dispinfo_t2 & disp2 );
    bool receiveMonitorPacket( const rcsc::rcg::dispinfo_t & disp1 );

    void receiveDebugClientPacket( const char * message );


    TrainerData & getTrainerData()
      {
          return M_trainer_data;
      }

    const TrainerData & trainerData() const
      {
          return M_trainer_data;
      }

    const ViewHolder & viewHolder() const
      {
          return M_view_holder;
      }

    std::size_t viewIndex() const
      {
          return M_view_index;
      }

    const DebugLogHolder & debugLogHolder() const
      {
          return M_debug_log_holder;
      }

    const AgentDataHolder< GridFieldEvaluationData > & gridFieldEvaluationHolder() const
      {
          return M_grid_field_evaluation_holder;
      }

    MonitorViewData::ConstPtr getViewData( const std::size_t idx ) const
      {
          return viewHolder().getViewData( idx );
      }


    MonitorViewData::ConstPtr getCurrentViewData() const
      {
          return viewHolder().getViewData( viewIndex() );
      }

    const DrawDataHolder & drawDataHolder() const
      {
          return M_draw_data_holder;
      }

    //! update player selection, focus point, field size, and so on.
    void update( const int width,
                 const int height );

private:

    void selectBallNearestPlayer( MonitorViewData::ConstPtr view );

public:

    bool setViewDataIndexFirst();
    bool setViewDataIndexLast();

    bool setViewDataStepBack();
    bool setViewDataStepForward();

    bool setViewDataIndex( const int index );
    bool setViewDataCycle( const int cycle );
    bool setViewDataCycle( const rcsc::GameTime & t );

    //
    //
    //

    void clearDebugLog()
      {
          M_debug_log_holder.clear();
      }

    void setDebugLogLevel( const boost::int32_t level,
                           const bool enable )
      {
          M_debug_log_holder.setLevel( level, enable );
      }

    bool setDebugLogDir( const std::string & team_name,
                         const rcsc::SideID & side,
                         const int unum,
                         const std::string & dir_path )
      {
          return M_debug_log_holder.setDir( team_name, side, unum, dir_path );
      }

    bool seekDebugLogData( const int unum,
                           const rcsc::GameTime & time )
      {
          return M_debug_log_holder.seekData( unum, time );
      }

    bool decrementDebugLogCycle( const int unum )
      {
          return M_debug_log_holder.decrementCycle( unum );
      }
    bool incrementDebugLogCycle( const int unum )
      {
          return M_debug_log_holder.incrementCycle( unum );
      }

    //
    //
    //

    const rcsc::GameTime & actionSequenceTime() const
      {
          return M_action_sequence_time;
      }
    const ActionSequenceHolder::ConstPtr & actionSequenceHolder() const
      {
          return M_action_sequence_holder;
      }
    ActionSequenceDescription::ConstPtr getSelectedActionSequence() const
      {
          return M_action_sequence_holder->getSequence( M_action_sequence_id );
      }
    void setActionSequenceSelection( const int id )
      {
          M_action_sequence_id = id;
      }
    void clearActionSequenceSelection()
      {
          M_action_sequence_id = -1;
      }
    void clearActionSequenceHolder()
      {
          M_action_sequence_id = -1;
          M_action_sequence_holder.reset();
      }
    void setActionSequenceHolder( const rcsc::GameTime & time,
                                  ActionSequenceHolder::ConstPtr seq )
      {
          M_action_sequence_time = time;
          M_action_sequence_holder = seq;
      }

    void clearGridFieldEvaluation()
      {
          M_grid_field_evaluation_holder.clear();
      }
    void setGridFieldEvaluation( const rcsc::GameTime & time,
                                 const AgentID & id,
                                 const boost::shared_ptr< GridFieldEvaluationData > & data )
      {
          M_grid_field_evaluation_holder.set( time, id, data );
      }

};

#endif
