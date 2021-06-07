// -*-c++-*-

/*!
  \file score_board_painter_rcss.cpp
  \brief rcssmonitor style: score board painter class Source File.
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

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "score_board_painter_rcss.h"

#include "draw_config.h"
// model
#include "options.h"
#include "main_data.h"
#include "monitor_view_data.h"

#include <iostream>


/*-------------------------------------------------------------------*/
/*!

*/
ScoreBoardPainterRCSS::ScoreBoardPainterRCSS( const MainData & main_data )
    : M_main_data( main_data )
{

}

/*-------------------------------------------------------------------*/
/*!

*/
void
ScoreBoardPainterRCSS::draw( QPainter & painter )
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

    static bool first = true;
    static QBrush brush( QColor( 0, 0, 0 ), Qt::SolidPattern );
    static QPen font_pen( QColor( 255, 255, 255 ), 0, Qt::SolidLine );
    static QFont font( "6x13bold", 9, QFont::Bold );
    if ( first )
    {
        font.setPointSize( 9 );
        font.setBold( true );
        //font.setStyleHint( QFont::System, QFont::PreferBitmap );
        font.setBold( true );
        font.setFixedPitch( true );
        first = false;
    }

    const rcsc::rcg::TeamT & left_team = ( opt.reverseSide()
                                           ? view->rightTeam()
                                           : view->leftTeam() );

    const rcsc::rcg::TeamT & right_team = ( opt.reverseSide()
                                            ? view->leftTeam()
                                            : view->rightTeam() );

    const std::vector< std::pair< int, rcsc::PlayMode > > & pen_scores_left
        = ( opt.reverseSide()
            ? M_main_data.viewHolder().penaltyScoresRight()
            : M_main_data.viewHolder().penaltyScoresLeft() );
    const std::vector< std::pair< int, rcsc::PlayMode > > & pen_scores_right
        = ( opt.reverseSide()
            ? M_main_data.viewHolder().penaltyScoresLeft()
            : M_main_data.viewHolder().penaltyScoresRight() );

    bool show_pen_score = true;

    if ( pen_scores_left.empty()
         && pen_scores_right.empty() )
    {
        show_pen_score = false;
    }
    else if ( ( ! pen_scores_left.empty()
                && view->time().cycle() < pen_scores_left.front().first )
              && ( ! pen_scores_right.empty()
                   && view->time().cycle() < pen_scores_right.front().first )
              && view->playmode() != rcsc::PM_PenaltySetup_Left
              && view->playmode() != rcsc::PM_PenaltySetup_Right
              && view->playmode() != rcsc::PM_PenaltyReady_Left
              && view->playmode() != rcsc::PM_PenaltyReady_Right
              && view->playmode() != rcsc::PM_PenaltyTaken_Left
              && view->playmode() != rcsc::PM_PenaltyTaken_Right )
    {
        show_pen_score = false;
    }


    QString text;

    std::string mode_str = view->playmodeString();
    if ( opt.reverseSide() )
    {
        if ( *mode_str.rbegin() == 'l' )
        {
            *mode_str.rbegin() = 'r';
        }
        else if ( *mode_str.rbegin() == 'r' )
        {
            *mode_str.rbegin() = 'l';
        }
    }

    if ( ! show_pen_score )
    {
        char buf[128];
        snprintf( buf, 128, " %10s %d:%d %-10s %16s %6ld    ",
                  ( left_team.name().empty() || opt.anonymousMode() ) ? "" : left_team.name().c_str(),
                  left_team.score(),
                  right_team.score(),
                  ( right_team.name().empty() || opt.anonymousMode() ) ? "" : right_team.name().c_str(),
                  mode_str.c_str(),
                  view->time().cycle() );
        text = QString::fromLatin1( buf );
    }
    else
    {
        int left_pen_score = 0;
        int left_pen_miss = 0;
        int right_pen_score = 0;
        int right_pen_miss = 0;

        for ( std::vector< std::pair< int, rcsc::PlayMode > >::const_iterator it = pen_scores_left.begin();
              it != pen_scores_left.end();
              ++it )
        {
            if ( it->first > view->time().cycle() ) break;

            if ( it->second == rcsc::PM_PenaltyScore_Left
                 || it->second == rcsc::PM_PenaltyScore_Right )
            {
                ++left_pen_score;
            }
            else if ( it->second == rcsc::PM_PenaltyMiss_Left
                      || it->second == rcsc::PM_PenaltyMiss_Right )
            {
                ++left_pen_miss;
            }
        }

        for ( std::vector< std::pair< int, rcsc::PlayMode > >::const_iterator it = pen_scores_right.begin();
              it != pen_scores_right.end();
              ++it )
        {
            if ( it->first > view->time().cycle() ) break;

            if ( it->second == rcsc::PM_PenaltyScore_Left
                 || it->second == rcsc::PM_PenaltyScore_Right )
            {
                ++right_pen_score;
            }
            else if ( it->second == rcsc::PM_PenaltyMiss_Left
                      || it->second == rcsc::PM_PenaltyMiss_Right )
            {
                ++right_pen_miss;
            }
        }

        char buf[128];
        snprintf( buf, 128, " %10s %d(%d/%d):%d(%d/%d) %-10s %16s %6ld",
                  ( left_team.name().empty() || opt.anonymousMode() ) ? "" : left_team.name().c_str(),
                  left_team.score(), left_pen_score, left_pen_score + left_pen_miss,
                  right_team.score(), right_pen_score, right_pen_score + right_pen_miss,
                  ( right_team.name().empty() || opt.anonymousMode() ) ? "" : right_team.name().c_str(),
                  mode_str.c_str(),
                  view->time().cycle() );
        text = QString::fromLatin1( buf );
    }

    painter.setFont( font );

    QRect bounding_rect = painter.fontMetrics().boundingRect( text );
    QRect rect( 0,
                painter.window().bottom() - bounding_rect.height() + 1,
                bounding_rect.width(),
                bounding_rect.height() );

    painter.fillRect( rect, brush );

    painter.setPen( font_pen );
    painter.setBrush( dconf.transparentBrush() );
    painter.drawText( rect,
                      Qt::AlignVCenter,
                      text );
}
