// -*-c++-*-

/*!
  \file view_holder.cpp
  \brief view data repository Source File.
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

#include "view_holder.h"

#include "options.h"
#include "debug_client_parser.h"
#include "agent_id.h"

#include <rcsc/common/player_param.h>
#include <rcsc/common/server_param.h>
#include <rcsc/rcg/serializer.h>
#include <rcsc/rcg/util.h>

#include <iterator>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstring>

const std::size_t RCG_SIZE_LIMIT = 6000 * 8;

const std::string DEFAULT_DEBUG_VIEW_EXTENSION = ".dcl";

/*-------------------------------------------------------------------*/
/*!
  default constructor.
*/
ViewHolder::ViewHolder()
    : M_last_playmode( rcsc::PM_Null ),
      M_default_type()
{
    M_player_types.insert( std::pair< int, rcsc::PlayerType >( 0, M_default_type ) );

    M_monitor_view_cont.reserve( 1024 * 8 );
}

/*-------------------------------------------------------------------*/
/*!

*/
ViewHolder::~ViewHolder()
{
    //std::cerr << "ViewHolder Left Debug View Size " << M_left_debug_view.size() << std::endl;
    //std::cerr << "ViewHolder Right Debug View Size " << M_right_debug_view.size() << std::endl;
}

/*-------------------------------------------------------------------*/
/*!
  clear all stored data
*/
void
ViewHolder::clear()
{
    M_last_playmode = rcsc::PM_Null;

    M_last_team_left.clear();
    M_last_team_right.clear();

    M_score_change_indices.clear();
    M_penalty_scores_left.clear();
    M_penalty_scores_right.clear();

    M_last_monitor_view.reset();
    M_monitor_view_cont.clear();

    M_left_debug_view.clear();
    M_right_debug_view.clear();

    M_default_type = rcsc::PlayerType();
    M_player_types.clear();
    M_player_types.insert( std::pair< int, rcsc::PlayerType >( 0, M_default_type ) );

    M_team_graphic_left.clear();
    M_team_graphic_right.clear();
}

/*-------------------------------------------------------------------*/
/*!

*/
std::ostream &
ViewHolder::saveRCG( std::ostream & os ) const
{
    rcsc::rcg::Serializer::Ptr serializer = rcsc::rcg::Serializer::create( logVersion() );

    if ( ! serializer )
    {
        std::cerr << "Failed to create the RCG serializer version " << logVersion()
                  << std::endl;
        return os;
    }

    // header
    serializer->serializeHeader( os );

    // params

    serializer->serializeParam( os, rcsc::ServerParam::i().toServerString() );
    serializer->serializeParam( os, rcsc::PlayerParam::i().toServerString() );
    for ( std::map< int, rcsc::PlayerType >::const_iterator it
              = playerTypeCont().begin();
          it != playerTypeCont().end();
          ++it )
    {
        serializer->serializeParam( os, it->second.toServerString() );
    }

    // playmode, team, show

    rcsc::rcg::DispInfoT disp;

    for ( MonitorViewData::Cont::const_iterator it = monitorViewCont().begin(),
              end = monitorViewCont().end();
          it != end;
          ++it )
    {
        (*it)->convertTo( disp );

        serializer->serialize( os, disp );
    }

    return os;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ViewHolder::saveDebugView( const std::string & dir_path ) const
{
    if ( dir_path.empty() )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " Dir path string is empty!"
                  << std::endl;
        return;
    }

    if ( ! M_last_monitor_view )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " No view data!!"
                  << std::endl;
        return;
    }

    std::string left_team = M_last_monitor_view->leftTeam().name();

    if ( ! left_team.empty()
         && ! M_left_debug_view.empty() )
    {
        for ( int unum = 1; unum <= 11; ++unum )
        {
            if ( saveDebugView( dir_path, left_team, unum,
                                M_left_debug_view ) )
            {
                std::cerr << "saved debug view data : "
                          << left_team << "-" << unum
                          << std::endl;
            }
        }
    }

    std::string right_team = M_last_monitor_view->rightTeam().name();

    if ( ! right_team.empty()
         && ! M_right_debug_view.empty() )
    {
        for ( int unum = 1; unum <= 11; ++unum )
        {
            if ( saveDebugView( dir_path, right_team, unum,
                                M_right_debug_view ) )
            {
                std::cerr << "saved debug view data : "
                          << right_team << "-" << unum
                          << std::endl;
            }
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::saveDebugView( const std::string & dir_path,
                           const std::string & team_name,
                           const int unum,
                           const DebugViewData::Map & data ) const
{
    std::string filepath = dir_path;
    {
        if ( filepath.empty()
             || *filepath.rbegin() != '/' )
        {
            filepath += '/';
        }

        std::ostringstream ostr;
        ostr << team_name << '-' << unum
             << DEFAULT_DEBUG_VIEW_EXTENSION;
        ostr << std::flush;
        filepath += ostr.str();
    }

    std::ofstream fout( filepath.c_str() );

    if ( ! fout )
    {
        return false;
    }

    const std::map< rcsc::GameTime, DebugViewData::Cont >::const_iterator begin
        = data.lower_bound( rcsc::GameTime( -1, 0 ) );
    const std::map< rcsc::GameTime, DebugViewData::Cont >::const_iterator end
        = data.upper_bound( M_last_monitor_view->time() );

    for ( std::map< rcsc::GameTime, DebugViewData::Cont >::const_iterator it = begin;
          it != end;
          ++it )
    {
        if ( it->second[unum - 1] )
        {
            it->second[unum - 1]->print( fout, it->first );
        }
    }

    fout.close();

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ViewHolder::openDebugView( const std::string & dir_path )
{
    if ( dir_path.empty() )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " Dir path string is empty!"
                  << std::endl;
        return;
    }

    if ( ! M_last_monitor_view )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " No view data!!"
                  << std::endl;
        return;
    }

    M_left_debug_view.clear();
    M_right_debug_view.clear();

    std::string left_team = M_last_monitor_view->leftTeam().name();

    if ( ! left_team.empty() )
    {
        for ( int unum = 1; unum <= 12; ++unum )
        {
            openDebugView( dir_path, left_team, unum );
        }
    }

    std::string right_team = M_last_monitor_view->rightTeam().name();

    if ( ! right_team.empty() )
    {
        for ( int unum = 1; unum <= 12; ++unum )
        {
            openDebugView( dir_path, right_team, unum );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::openDebugView( const std::string & dir_path,
                           const std::string & team_name,
                           const int unum )
{
    std::string filepath = dir_path;
    std::string filename;
    {
        if ( filepath.empty()
             || *filepath.rbegin() != '/' )
        {
            filepath += '/';
        }

        std::ostringstream ostr;
        ostr << team_name << '-';
        if ( unum != 12 )
        {
            ostr << unum;
        }
        else
        {
            ostr << "coach";
        }
        ostr << DEFAULT_DEBUG_VIEW_EXTENSION;
        ostr << std::flush;

        filename = ostr.str();
        filepath += filename;
    }

    std::ifstream fin( filepath.c_str() );

    if ( ! fin )
    {
        std::cerr << __FILE__ << ": could not open the file [" << filepath
                  << "]" << std::endl;
        return false;
    }

    DebugClientParser parser;

    std::string line_buf;
    while ( std::getline( fin, line_buf ) )
    {
        if ( line_buf.compare( 0, 10, "%% debug [" ) )
        {
            // other debug log data
            continue;
        }

        parser.parse( line_buf.c_str() + 10, *this );
    }

    fin.close();

    std::cerr << __FILE__ << ": read debug view data : " << filename << std::endl;
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::handleEOF()
{
    if ( M_last_monitor_view )
    {
        M_monitor_view_cont.push_back( M_last_monitor_view );
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::handleShow( const rcsc::rcg::ShowInfoT & show )
{
    if ( M_monitor_view_cont.size() > RCG_SIZE_LIMIT )
    {
        return false;
    }

    MonitorViewData::Ptr ptr( new MonitorViewData( show,
                                                   M_last_playmode,
                                                   M_last_team_left,
                                                   M_last_team_right ) );
    if ( M_last_monitor_view )
    {
        const rcsc::PlayMode pm = ptr->playmode();

        if ( ! M_monitor_view_cont.empty() )
        {
            // before adding the last view data,
            // we should remove old BeforeKickOffMode or TimeOver data
            // from data container if exist.

            if ( pm == rcsc::PM_BeforeKickOff
                 || pm == rcsc::PM_TimeOver )
            {
                rcsc::PlayMode back_pmode = M_monitor_view_cont.back()->playmode();
                if ( back_pmode == rcsc::PM_BeforeKickOff
                     || back_pmode == rcsc::PM_TimeOver )
                {
                    // last data must be swapped.
                    M_monitor_view_cont.pop_back();
                }
            }
            else if ( Options::instance().monitorClientMode()
                      && ! Options::instance().timeShiftReplay() )
            {
                M_monitor_view_cont.pop_back();
            }
        }

        M_monitor_view_cont.push_back( M_last_monitor_view );

        if ( M_last_monitor_view->time().cycle() == ptr->time().cycle()
             && ( pm == rcsc::PM_BeforeKickOff
                  || pm == rcsc::PM_TimeOver
                  || pm == rcsc::PM_AfterGoal_Left
                  || pm == rcsc::PM_AfterGoal_Right
                  || pm == rcsc::PM_OffSide_Left
                  || pm == rcsc::PM_OffSide_Right
                  || pm == rcsc::PM_Foul_Charge_Left
                  || pm == rcsc::PM_Foul_Charge_Right
                  || pm == rcsc::PM_Foul_Push_Left
                  || pm == rcsc::PM_Foul_Push_Right
                  || pm == rcsc::PM_Back_Pass_Left
                  || pm == rcsc::PM_Back_Pass_Right
                  || pm == rcsc::PM_Free_Kick_Fault_Left
                  || pm == rcsc::PM_Free_Kick_Fault_Right
                  || pm == rcsc::PM_CatchFault_Left
                  || pm == rcsc::PM_CatchFault_Right ) )
        {
            ptr->setStoppageTime( M_last_monitor_view->time().stopped() + 1 );
        }
    }

    M_last_monitor_view = ptr;

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::handleMsg( const int,
                       const int,
                       const std::string & msg )
{
    if ( ! msg.compare( 0, std::strlen( "(team_graphic_" ),
                        "(team_graphic_" ) )
    {
        return analyzeTeamGraphic( msg );
    }
    else if ( ! msg.compare( 0, std::strlen( "(change_player_type" ),
                             "(change_player_type" ) )

    {
        return true;
    }

    std::cerr << PACKAGE_NAME << ": recv msginfo [" << msg << ']'
              << std::endl;

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::handleDraw( const int,
                        const rcsc::rcg::drawinfo_t & msg )
{
    std::cerr << PACKAGE_NAME << ": recv drawinfo_t: mode = " << msg.mode
              << std::endl;
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::handlePlayMode( const int time,
                            const rcsc::PlayMode playmode )
{
    if ( M_last_playmode != playmode )
    {
        if ( playmode == rcsc::PM_PenaltyScore_Left
             || playmode == rcsc::PM_PenaltyMiss_Left )
        {
            M_penalty_scores_left.push_back( std::make_pair( time, playmode ) );
        }
        else if ( playmode == rcsc::PM_PenaltyScore_Right
                  || playmode == rcsc::PM_PenaltyMiss_Right )
        {
            M_penalty_scores_right.push_back( std::make_pair( time, playmode ) );
        }
    }

    M_last_playmode = playmode;
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::handleTeam( const int,
                        const rcsc::rcg::TeamT & team_l,
                        const rcsc::rcg::TeamT & team_r )
{
    if ( M_last_team_left.score_ != team_l.score_
         || M_last_team_right.score_ != team_r.score_ )
    {
        M_score_change_indices.push_back( M_monitor_view_cont.size() );
    }

    M_last_team_left = team_l;
    M_last_team_right = team_r;
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::handlePlayerType( const std::string & msg )
{
    rcsc::PlayerType player_type( msg.c_str(), 999.0 );

    if ( player_type.id() < 0 )
    {
        std::cerr << __FILE__ << ": (handlePlayerType) illegal player type id = "
                  << player_type.id() << std::endl;
        return false;
    }

    if ( M_player_types.find( player_type.id() ) != M_player_types.end() )
    {
        M_player_types[player_type.id()] = player_type;
    }
    else
    {
        M_player_types.insert( std::pair< int, rcsc::PlayerType >( player_type.id(), player_type ) );
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::handleServerParam( const std::string & msg )
{
    rcsc::ServerParam::instance().parse( msg.c_str(), 999.0 );
    M_default_type = rcsc::PlayerType();
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::handlePlayerParam( const std::string & msg )
{
    rcsc::PlayerParam::instance().parse( msg.c_str(), 999.0 );
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ViewHolder::addDebugView( const rcsc::GameTime & time,
                          char side,
                          const int unum,
                          DebugViewData::Ptr debug_view )
{
    if ( ! debug_view )
    {
        return;
    }

    if ( unum < 1 || 12 < unum )
    {
        std::cerr << __FILE__ << ": (addDebugView) Illegal unum "
                  << unum << std::endl;
        return;
    }

    DebugViewData::Map * view_map = ( side == 'l'
                                      ? &M_left_debug_view
                                      : &M_right_debug_view );

    DebugViewData::Map::iterator it = view_map->find( time );

    // already exist this cycle data
    if ( it != view_map->end() )
    {
        if ( ! it->second[unum - 1] )
        {
            it->second[unum - 1] = debug_view;
        }
        else
        {
            const rcsc::ServerParam & SP = rcsc::ServerParam::i();
            if ( time.cycle() == 0 )
            {
                it->second[unum - 1] = debug_view;
            }
            else if ( SP.actualHalfTime() > 0
                      && SP.nrNormalHalfs() >= 0 )
            {
                if ( ( time.cycle() <= SP.nrNormalHalfs() * SP.actualHalfTime()
                       && time.cycle() % SP.actualHalfTime() == 0 )
                     || ( SP.actualExtraHalfTime() > 0
                          && SP.nrExtraHalfs() >= 0
                          && time.cycle() > SP.nrNormalHalfs() * SP.actualHalfTime()
                          && time.cycle() % SP.actualExtraHalfTime() == 0 ) )
                {
                    it->second[unum - 1] = debug_view;
                }
            }
            else
            {
                it->second[unum - 1] = debug_view;
            }
        }
    }
    else
    {
        // insert new cycle (include coach)
        std::pair< DebugViewData::Map::iterator, bool > p
            = view_map->insert( std::make_pair( time, DebugViewData::Cont( 12 ) ) );

        if ( ! p.second )
        {
            std::cerr << __FILE__ << ": (addDebugView) Failed to add DebugView"
                      << std::endl;
            return;
        }

        p.first->second[unum - 1] = debug_view;
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
DebugViewData::ConstPtr
ViewHolder::getDebugView( const rcsc::GameTime & time,
                          const AgentID & player ) const
{
    if ( player.side() == rcsc::NEUTRAL )
    {
        return DebugViewData::ConstPtr();
    }

    const DebugViewData::Map & view_map
        = ( player.side() == rcsc::RIGHT
            ? rightDebugView()
            : leftDebugView() );

    DebugViewData::Map::const_iterator it = view_map.find( time );

    if ( it == view_map.end()
         || static_cast< int >( it->second.size() ) < player.unum() )
    {
        return DebugViewData::ConstPtr();
    }

    return (*it).second[ player.unum() - 1 ];
}

/*-------------------------------------------------------------------*/
/*!

*/
MonitorViewData::ConstPtr
ViewHolder::getViewData( const std::size_t idx ) const
{
    if ( M_monitor_view_cont.size() <= idx )
    {
        return MonitorViewData::ConstPtr(); // null pointer
    }

    return M_monitor_view_cont[idx];
}

/*-------------------------------------------------------------------*/
/*!

*/
namespace {

struct CycleCmp {
    bool operator()( const MonitorViewData::ConstPtr & lhs,
                     const int rhs )
      {
          return lhs->time().cycle() < rhs;
      }
};

struct TimeCmp {
    bool operator()( const MonitorViewData::ConstPtr & lhs,
                     const rcsc::GameTime & rhs )
      {
          return lhs->time() < rhs;
      }
};

}

/*-------------------------------------------------------------------*/
/*!

*/
std::size_t
ViewHolder::getIndexOf( const int cycle ) const
{
    MonitorViewData::Cont::const_iterator it
        = std::lower_bound( M_monitor_view_cont.begin(),
                            M_monitor_view_cont.end(),
                            cycle,
                            CycleCmp() );
    if ( it == M_monitor_view_cont.end() )
    {
        return 0;
    }

    return std::distance( M_monitor_view_cont.begin(), it );

//     MonitorViewCont::size_type len = M_monitor_view_cont.size();
//     MonitorViewCont::size_type half;

//     MonitorViewCont::const_iterator first = M_monitor_view_cont.begin();
//     MonitorViewCont::const_iterator middle;

//     // lower bound algorithm
//     while ( len > 0 )
//     {
//         half = len >> 1;
//         middle = first;
//         middle += half;
//         if ( (*middle)->cycle() < cycle )
//         {
//             first = middle;
//             ++first;
//             len = len - half - 1;
//         }
//         else
//         {
//             len = half;
//         }
//     }

//     if ( first == M_monitor_view_cont.end() )
//     {
//         if ( cycle > 0
//              && ! M_monitor_view_cont.empty() )
//         {
//             return M_monitor_view_cont.size() - 1;
//         }
//         return 0;
//     }

//     return std::distance( M_monitor_view_cont.begin(), first );
}

/*-------------------------------------------------------------------*/
/*!

*/
std::size_t
ViewHolder::getIndexOf( const rcsc::GameTime & t ) const
{
    MonitorViewData::Cont::const_iterator it
        = std::lower_bound( M_monitor_view_cont.begin(),
                            M_monitor_view_cont.end(),
                            t,
                            TimeCmp() );
    if ( it == M_monitor_view_cont.end() )
    {
        return 0;
    }

    return std::distance( M_monitor_view_cont.begin(), it );
}

/*-------------------------------------------------------------------*/
/*!

*/
const
rcsc::PlayerType &
ViewHolder::playerType( const int id ) const
{
    std::map< int, rcsc::PlayerType >::const_iterator it
        = M_player_types.find( id );

    if ( it == M_player_types.end() )
    {
        return M_default_type;
    }

    return it->second;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::analyzeTeamGraphic( const std::string & msg )
{
    char side = '?';
    int x = -1, y = -1;

    if ( std::sscanf( msg.c_str(),
                      "(team_graphic_%c ( %d %d ",
                      &side, &x, &y ) != 3
         || ( side != 'l' && side != 'r' )
         || x < 0
         || y < 0 )
    {
        std::cerr << __FILE__ << ": (analyzeTeamGraphic) Illegal team_graphic message ["
                  << msg << "]" << std::endl;
        return false;
    }

    if ( side == 'l' )
    {
        //std::cerr << "recv team_graphic_l (" << x << ',' << y << ')'
        //          << std::endl;
        return M_team_graphic_left.parse( msg.c_str() );
    }

    if ( side == 'r' )
    {
        //std::cerr << "recv team_graphic_r (" << x << ',' << y << ')'
        //          << std::endl;
        return M_team_graphic_right.parse( msg.c_str() );
    }

    return false;
}
