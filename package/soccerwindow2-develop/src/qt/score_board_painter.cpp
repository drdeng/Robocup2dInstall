// -*-c++-*-

/*!
  \file score_board_painter.cpp
  \brief score board painter class Source File.
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

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "score_board_painter.h"

#include "draw_config.h"
// model
#include "options.h"
#include "main_data.h"
#include "monitor_view_data.h"

#include <iostream>


/*-------------------------------------------------------------------*/
/*!

*/
ScoreBoardPainter::ScoreBoardPainter( const MainData & main_data )
    : M_main_data( main_data )
#if 0
    , M_last_width( -100 )
    , M_last_height( -100 )
    , M_last_left_name()
    , M_last_left_score( -100 )
    , M_last_right_name()
    , M_last_right_score( -100 )
    , M_last_playmode()
    , M_last_cycle( -100 )
#endif
{

}

/*-------------------------------------------------------------------*/
/*!

*/
void
ScoreBoardPainter::draw( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( ! opt.showScoreBoard() )
    {
        return;
    }

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    if ( ! view )
    {
        return;
    }

    const DrawConfig & dconf = DrawConfig::instance();

    // if ( dconf.scoreBoardFont().pointSize() != opt.scoreBoardFontSize() )
    // {
    //     DrawConfig::instance().resizeScoreBoardFont( opt.scoreBoardFontSize() );
    // }

    int board_left_x = 0;
    int board_top_y = 0;
    int board_width = opt.canvasWidth();

    painter.setFont( dconf.scoreBoardFont() );
    painter.setPen( dconf.scoreBoardFontPen() );

    //std::cerr << "font = " << painter.font().toString().toStdString()
    //          << std::endl;

    {
        QRect left_rect( board_left_x,
                         board_top_y,
                         board_width/3,
                         opt.scoreBoardHeight() );

        painter.fillRect( left_rect, dconf.leftTeamBrush() );

        const rcsc::rcg::TeamT & team = ( opt.reverseSide()
                                          ? view->rightTeam()
                                          : view->leftTeam() );
        char buf[32];
        snprintf( buf, 32,
                  "%s  %d",
                  ( opt.anonymousMode() || team.name().empty() ) ? "left team" : team.name().c_str(),
                  team.score() );

        painter.drawText( left_rect,
                          Qt::AlignCenter,
                          QString::fromLatin1( buf ) );
    }
    {
        QRect right_rect( board_left_x + board_width*2/3 + 1,
                          board_top_y,
                          board_width/3,
                          opt.scoreBoardHeight() );

        painter.fillRect( right_rect, dconf.rightTeamBrush() );

        const rcsc::rcg::TeamT & team = ( opt.reverseSide()
                                          ? view->leftTeam()
                                          : view->rightTeam() );

        char buf[32];
        snprintf( buf, 32,
                  "%s  %d",
                  ( opt.anonymousMode() || team.name().empty() ) ? "right team" : team.name().c_str(),
                  team.score() );

        painter.drawText( right_rect,
                          Qt::AlignCenter,
                          QString::fromLatin1( buf ) );
    }
    {
        QRect time_rect( board_width/3 - 1,
                         board_top_y,
                         board_width/3 + 2,
                         opt.scoreBoardHeight() );

        painter.fillRect( time_rect, dconf.scoreBoardBrush() );

        char buf[32];
        if ( view->time().stopped() > 0 )
        {
            snprintf( buf, 32,
                      "%s  %4ld,%ld",
                      view->playmodeString().c_str(),
                      view->time().cycle(),
                      view->time().stopped() );
        }
        else
        {
            snprintf( buf, 32,
                      "%s  %4ld",
                      view->playmodeString().c_str(),
                      view->time().cycle() );
        }

        if ( opt.reverseSide() )
        {
            std::size_t len = view->playmodeString().length() - 1;
            if ( buf[len] == 'l' )
            {
                buf[len] = 'r';
            }
            else if ( buf[len] == 'r' )
            {
                buf[len] = 'l';
            }
        }

        painter.drawText( time_rect,
                          Qt::AlignCenter,
                          QString::fromLatin1( buf ) );
    }

    drawPenaltyScores( painter );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ScoreBoardPainter::drawPenaltyScores( QPainter & painter )
{
    const Options & opt = Options::instance();

    const std::vector< std::pair< int, rcsc::PlayMode > > & scores_left
        = ( opt.reverseSide()
            ? M_main_data.viewHolder().penaltyScoresRight()
            : M_main_data.viewHolder().penaltyScoresLeft() );
    const std::vector< std::pair< int, rcsc::PlayMode > > & scores_right
        = ( opt.reverseSide()
            ? M_main_data.viewHolder().penaltyScoresLeft()
            : M_main_data.viewHolder().penaltyScoresRight() );

    if ( scores_left.empty()
         && scores_right.empty() )
    {
        return;
    }

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    if ( ! view )
    {
        return;
    }

    if ( ( scores_left.empty()
           || view->time().cycle() < scores_left.front().first )
         && ( scores_right.empty()
              || view->time().cycle() < scores_right.front().first )
         && view->playmode() != rcsc::PM_PenaltySetup_Left
         && view->playmode() != rcsc::PM_PenaltySetup_Right
         && view->playmode() != rcsc::PM_PenaltyReady_Left
         && view->playmode() != rcsc::PM_PenaltyReady_Right
         && view->playmode() != rcsc::PM_PenaltyTaken_Left
         && view->playmode() != rcsc::PM_PenaltyTaken_Right )
    {
        return;
    }

    const DrawConfig & dconf = DrawConfig::instance();

    painter.setFont( dconf.scoreBoardFont() );
    painter.setPen( dconf.scoreBoardFontPen() );
    painter.setBrush( dconf.transparentBrush() );

    bool left_goal = ( view->ball().x() < 0.0 );
    if ( opt.reverseSide() ) left_goal = ! left_goal;

    const int cell_size = painter.fontMetrics().height() + 4;
    const int name_width = painter.fontMetrics().width( QObject::tr( "right" ) ) + 4;

    int score_size = std::max( scores_left.size(), scores_right.size() );
    if ( score_size > 5 )
    {
        const std::vector< std::pair< int, rcsc::PlayMode > > & scores
            = ( scores_left.empty()
                ? scores_right
                : scores_right.empty()
                ? scores_left
                : scores_left.front().first < scores_right.front().first
                ? scores_left
                : scores_right );
        score_size = 0;
        for ( std::vector< std::pair< int, rcsc::PlayMode > >::const_iterator it = scores.begin();
              it != scores.end();
              ++it )
        {
            if ( it->first > view->time().cycle() ) break;
            ++score_size;
        }
    }
    if ( score_size < 5 ) score_size = 5;

    double board_left_x
        = ( left_goal
            ? opt.absScreenX( 1.0 )
            : opt.absScreenX( -1.0 ) - cell_size*score_size - name_width );
    double board_top_y = opt.absScreenY( 17.0 );

    double board_width = name_width + cell_size*score_size;
    double board_height = cell_size*3;

    //int board_right_x = board_left_x + board_width;
    double board_bottom_y = board_top_y + board_height;

    painter.fillRect( QRectF( board_left_x, board_top_y,
                              board_width, board_height ),
                      Qt::white );

    painter.fillRect( QRectF( board_left_x, board_top_y + cell_size,
                              board_width, cell_size ),
                      dconf.leftTeamBrush() );
    painter.fillRect( QRectF( board_left_x, board_top_y + cell_size*2,
                              board_width, cell_size ),
                      dconf.rightTeamBrush() );

    painter.drawText( QRectF( board_left_x, board_top_y + cell_size,
                              name_width, cell_size ),
                      Qt::AlignCenter,
                      QObject::tr( "left" ) );
    painter.drawText( QRectF( board_left_x, board_top_y + cell_size*2,
                              name_width, cell_size ),
                      Qt::AlignCenter,
                      QObject::tr( "right" ) );

    painter.drawLine( QLineF( board_left_x + name_width, board_top_y,
                              board_left_x + name_width, board_bottom_y ) );

    for ( int i = 1; i <= score_size; ++i )
    {
        double line_x = board_left_x + name_width + cell_size*i;
        painter.drawText( QRectF( line_x - cell_size, board_top_y,
                                  cell_size, cell_size ),
                          Qt::AlignCenter,
                          QString::number( i ) );
        painter.drawLine( QLineF( line_x, board_top_y,
                                 line_x, board_bottom_y ) );
    }

    int idx = 0;
    for ( std::vector< std::pair< int, rcsc::PlayMode > >::const_iterator it = scores_left.begin();
          it != scores_left.end();
          ++it )
    {
        if ( it->first > view->time().cycle() ) break;

        double row_x = board_left_x + name_width + cell_size*idx;
        if ( it->second == rcsc::PM_PenaltyScore_Left
             || it->second == rcsc::PM_PenaltyScore_Right )
        {
            painter.drawEllipse( QRectF( row_x + 4,
                                         board_top_y + cell_size + 4,
                                         cell_size-6,
                                         cell_size-6 ) );
        }
        else if ( it->second == rcsc::PM_PenaltyMiss_Left
                  || it->second == rcsc::PM_PenaltyMiss_Right )
        {
            painter.drawLine( QLineF( row_x + 4, board_top_y + cell_size + 4,
                                      row_x + cell_size - 4, board_top_y + cell_size*2 - 4 ) );
            painter.drawLine( QLineF( row_x + cell_size - 4, board_top_y + cell_size + 4,
                                      row_x + 4, board_top_y + cell_size*2 - 4 ) );
        }

        ++idx;
    }

    idx = 0;
    for ( std::vector< std::pair< int, rcsc::PlayMode > >::const_iterator it = scores_right.begin();
          it != scores_right.end();
          ++it )
    {
        if ( it->first > view->time().cycle() ) break;

        double row_x = board_left_x + name_width + cell_size*idx;
        if ( it->second == rcsc::PM_PenaltyScore_Left
             || it->second == rcsc::PM_PenaltyScore_Right )
        {
            painter.drawEllipse( QRectF( row_x + 4,
                                         board_top_y + cell_size*2 + 4,
                                         cell_size-6,
                                         cell_size-6 ) );
        }
        else if ( it->second == rcsc::PM_PenaltyMiss_Left
                  || it->second == rcsc::PM_PenaltyMiss_Right )
        {
            painter.drawLine( QLineF( row_x + 4, board_top_y + cell_size*2 + 4,
                                      row_x + cell_size - 4, board_top_y + cell_size*3 - 4 ) );
            painter.drawLine( QLineF( row_x + cell_size - 4, board_top_y + cell_size*2 + 4,
                                      row_x + 4, board_top_y + cell_size*3 - 4 ) );
        }

        ++idx;
    }
}
