// -*-c++-*-

/*!
  \file action_sequence_description.h
  \brief definition of action sequence data Header File.
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

#ifndef SOCCERWINDOW2_ACTION_SEQUENCE_DESCRIPTION_H
#define SOCCERWINDOW2_ACTION_SEQUENCE_DESCRIPTION_H

#include "agent_id.h"

#include <rcsc/geom/vector_2d.h>
#include <rcsc/game_time.h>

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <iosfwd>


class ActionDescription {
public:
    enum Category { HOLD, PASS, DRIBBLE, SHOOT, MOVE };

private:
    int M_id;

    Category M_category;

    std::string M_name;
    int M_number;

    int M_kick_count;
    int M_duration_time;

    int M_from_unum;
    rcsc::Vector2D M_from_pos;

    int M_to_unum;
    rcsc::Vector2D M_to_pos;

    int M_safe_level;

    double M_value;

    std::string M_description;

public:
    ActionDescription()
        : M_id( -1 ),
          M_category( HOLD ),
          M_name(),
          M_number( -1 ),
          M_kick_count( -1 ),
          M_duration_time( -1 ),
          M_from_unum( -1 ),
          M_from_pos(),
          M_to_unum( -1 ),
          M_to_pos(),
          M_safe_level( 0 ),
          M_value( 0.0 ),
          M_description()
      { }

    void setPass( const char * name,
                  const int number,
                  const int kick_count,
                  const int duration_time,
                  const int from_unum,
                  const double from_x,
                  const double from_y,
                  const int to_unum,
                  const double to_x,
                  const double to_y,
                  const int safe_level,
                  const double value )
      {
          M_category = PASS;
          M_name = name;
          M_number = number;
          M_kick_count = kick_count;
          M_duration_time = duration_time;
          M_from_unum = from_unum;
          M_from_pos.assign( from_x, from_y );
          M_to_unum = to_unum;
          M_to_pos.assign( to_x, to_y );
          M_safe_level = safe_level;
          M_value = value;
      }

    void setDribble( const char * name,
                     const int number,
                     const int duration_time,
                     const int unum,
                     const double from_x,
                     const double from_y,
                     const double to_x,
                     const double to_y,
                     const int safe_level,
                     const double value )
      {
          M_category = DRIBBLE;
          M_name = name;
          M_number = number;
          M_duration_time = duration_time;
          M_from_unum = unum;
          M_from_pos.assign( from_x, from_y );
          M_to_unum = unum;
          M_to_pos.assign( to_x, to_y );
          M_safe_level = safe_level;
          M_value = value;
      }

    void setShoot( const char * name,
                   const int duration_time,
                   const int unum,
                   const double from_x,
                   const double from_y,
                   const double to_x,
                   const double to_y,
                   const int safe_level,
                   const double value )
      {
          M_category = SHOOT;
          M_name = name;
          M_duration_time = duration_time;
          M_from_unum = unum;
          M_from_pos.assign( from_x, from_y );
          M_to_unum = unum;
          M_to_pos.assign( to_x, to_y );
          M_safe_level = safe_level;
          M_value = value;
      }

    void setHold( const char * name,
                  const int duration_time,
                  const int unum,
                  const double x,
                  const double y,
                  const int safe_level,
                  const double value )
      {
          M_category = HOLD;
          M_name = name;
          M_duration_time = duration_time;
          M_from_unum = unum;
          M_from_pos.assign( x, y );
          M_to_unum = unum;
          M_to_pos.assign( x, y );
          M_safe_level = safe_level;
          M_value = value;
      }

    void setMove( const char * name,
                  const int duration_time,
                  const int unum,
                  const double from_x,
                  const double from_y,
                  const double to_x,
                  const double to_y,
                  const int safe_level,
                  const double value )
      {
          M_category = MOVE;
          M_name = name;
          M_duration_time = duration_time;
          M_from_unum = unum;
          M_from_pos.assign( from_x, from_y );
          M_to_unum = unum;
          M_to_pos.assign( to_x, to_y );
          M_safe_level = safe_level;
          M_value = value;
      }

    void setId( const int id )
      {
          M_id = id;
      }

    void setDescription( const std::string & description )
      {
          M_description = description;
      }

    int id() const
      {
          return M_id;
      }

    Category category() const
      {
          return M_category;
      }

    const std::string & name() const
      {
          return M_name;
      }

    int number() const
      {
          return M_number;
      }

    int kickCount() const
      {
          return M_kick_count;
      }

    int durationTime() const
      {
          return M_duration_time;
      }

    int fromUnum() const
      {
          return M_from_unum;
      }

    const rcsc::Vector2D & fromPos() const
      {
          return M_from_pos;
      }

    int toUnum() const
      {
          return M_to_unum;
      }

    const rcsc::Vector2D & toPos() const
      {
          return M_to_pos;
      }

    int safeLevel() const
      {
          return M_safe_level;
      }

    double value() const
      {
          return M_value;
      }

    const std::string & description() const
      {
          return M_description;
      }

    std::ostream & print( std::ostream & os,
                          const int count ) const;
};

class ActionSequenceDescription {
public:
    typedef boost::shared_ptr< const ActionSequenceDescription > ConstPtr;

private:
    int M_id;
    double M_value;
    std::vector< ActionDescription > M_actions;
    std::vector< std::string > M_evaluation_description;
    std::string M_ranking_data;

public:
    explicit
    ActionSequenceDescription( int id )
        : M_id( id ),
          M_value( 0.0 )
      { }

    int id() const
      {
          return M_id;
      }

    double value() const
      {
          return M_value;
      }

    const std::vector< ActionDescription > & actions() const
      {
          return M_actions;
      }

    const std::vector< std::string > & evaluationDescription() const
      {
          return M_evaluation_description;
      }

    const std::string & rankingData() const
      {
          return M_ranking_data;
      }

    void setValue( const double value )
      {
          M_value = value;
      }

    void add( const ActionDescription & act )
      {
          M_actions.push_back( act );
      }

    void setEvaluationDescription( const std::vector< std::string > & evaluation_description )
      {
          M_evaluation_description = evaluation_description;
      }

    void setRankingData( const std::string & str )
      {
          M_ranking_data = str;
      }

    std::ostream & print( std::ostream & os ) const;

    //

    // struct Less {
    //     bool operator()( const ActionSequenceDescription & lhs,
    //                      const ActionSequenceDescription & rhs )
    //       {
    //           const std::vector< ActionDescription > & l = lhs.actions();
    //           const std::vector< ActionDescription > & r = rhs.actions();
    //           if ( l.size() < r.size() ) return true;
    //           if ( l.size() > r.size() ) return false;
    //           for ( std::vector< ActionDescription >::const_iterator li = l.begin(), ri = r.begin(), lend = l.end();
    //                 li != lend;
    //                 ++li )
    //           {
    //               if ( li->id() < ri->id() ) return true;
    //               if ( li->id() > ri->id() ) return false;
    //           }
    //           return false;
    //       }
    // };
};


class ActionSequenceHolder {
public:
    typedef boost::shared_ptr< ActionSequenceHolder > Ptr;
    typedef boost::shared_ptr< const ActionSequenceHolder > ConstPtr;
    typedef std::map< int, ActionSequenceDescription::ConstPtr > Cont;

private:
    AgentID M_first_player_id;
    Cont M_data;

public:

    ActionSequenceHolder()
        : M_first_player_id()
      { }

    const AgentID & firstPlayerID() const
      {
          return M_first_player_id;
      }

    const Cont & data() const
      {
          return M_data;
      }

    ActionSequenceDescription::ConstPtr getSequence( const int id ) const
      {
          Cont::const_iterator it = M_data.find( id );
          if ( it != M_data.end() )
          {
              return it->second;
          }
          return ActionSequenceDescription::ConstPtr();
      }

    void setFirstPlayer( const AgentID & id )
      {
          M_first_player_id = id;
      }

    void add( const ActionSequenceDescription::ConstPtr & seq )
      {
          //M_sequences.insert( std::make_pair( seq->value(), seq ) );
          M_data.insert( Cont::value_type( seq->id(), seq ) );
      }
};

#endif
