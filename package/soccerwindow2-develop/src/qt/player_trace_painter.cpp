// -*-c++-*-

/*!
  \file player_trace_painter.cpp
  \brief player trace painter class Source File.
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

#include "player_trace_painter.h"

#include "draw_config.h"
// model
#include "options.h"
#include "main_data.h"
#include "monitor_view_data.h"

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerTracePainter::draw( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( ! opt.showPlayerTrace() )
    {
        return;
    }

    if ( opt.selectedNumber() == 0 )
    {
        return;
    }

    const ViewHolder & holder = M_main_data.viewHolder();

    int start_cycle = opt.playerTraceStart();
    int end_cycle = opt.playerTraceEnd();

    const MonitorViewData::Cont & vc = M_main_data.viewHolder().monitorViewCont();

    if ( vc.empty() )
    {
        return;
    }

    if ( opt.playerAutoTrace() )
    {
        MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();
        if ( ! view )
        {
            return;
        }
        start_cycle = std::max( 1L, view->time().cycle() + opt.autoTraceStart() );
    }
    else if ( start_cycle >= end_cycle )
    {
        return;
    }

    const std::size_t first = holder.getIndexOf( start_cycle );
    const std::size_t last = ( opt.playerAutoTrace()
                               ? std::min( first + opt.autoTracePeriod(),
                                           vc.size() - 1 )
                               : holder.getIndexOf( end_cycle ) );
    if ( first >= last )
    {
        return;
    }

    if ( opt.antiAliasing() )
    {
#ifdef USE_HIGH_QUALITY_ANTIALIASING
        painter.setRenderHints( QPainter::HighQualityAntialiasing |
                                QPainter::Antialiasing,
                                false );
#else
        painter.setRenderHint( QPainter::Antialiasing, false );
#endif
    }

    const std::size_t idx = static_cast< std::size_t >
        ( opt.selectedNumber() > 0
          ? opt.selectedNumber() - 1
          : 11 - opt.selectedNumber() - 1 );
    const QPen & my_pen = ( opt.reverseSide()
                            ? ( opt.selectedNumber() > 0
                                ? DrawConfig::instance().rightTeamPen()
                                : DrawConfig::instance().leftTeamPen() )
                            : ( opt.selectedNumber() > 0
                                ? DrawConfig::instance().leftTeamPen()
                                : DrawConfig::instance().rightTeamPen() )
                            );
    QPen black_dot_pen( Qt::black );
    black_dot_pen.setStyle( Qt::DotLine );

    const bool line_trace = opt.lineTrace();

    if ( line_trace )
    {
        painter.setBrush( DrawConfig::instance().transparentBrush() );
    }
    else
    {
        painter.setBrush( Qt::gray );
    }

    //QPainterPath playon_path;
    //QPainterPath setplay_path;

    std::size_t i = first;
    QPointF prev( opt.screenX( vc[i]->players()[idx].x() ),
                  opt.screenY( vc[i]->players()[idx].y() ) );
    ++i;
    for ( ; i <= last; ++i )
    {
        //QPainterPath * path;
        switch ( vc[i]->playmode() ) {
        case rcsc::PM_BeforeKickOff:
        case rcsc::PM_TimeOver:
        case rcsc::PM_AfterGoal_Left:
        case rcsc::PM_AfterGoal_Right:
            continue;
        case rcsc::PM_PlayOn:
            painter.setPen( my_pen );
            //path = &playon_path;
            break;
        default:
            painter.setPen( black_dot_pen );
            //path = &setplay_path;
            break;
        }

        const rcsc::rcg::PlayerT & p = vc[i]->players()[idx];
        QPointF next( opt.screenX( p.x() ),
                      opt.screenY( p.y() ) );
        if ( line_trace )
        {
            painter.drawLine( QLineF( prev, next ) );
            //path->moveTo( prev_x, prev_y );
            //path->lineTo( ix, iy );
        }
        else
        {
            painter.drawLine( QLineF(  prev, next ) );
            painter.drawEllipse( QRectF( next.x() - 2.0,
                                         next.y() - 2.0,
                                         4.0,
                                         4.0 ) );
            //path->moveTo( prev_x, prev_y );
            //path->lineTo( ix, iy );
            //path->addEllipse( ix, iy, 4, 4 );
        }
        prev = next;
    }

//     painter.setPen( my_pen );
//     painter.drawPath( playon_path );

//     painter.setPen( black_dot_pen );
//     painter.drawPath( setplay_path );

    if ( opt.antiAliasing() )
    {
#ifdef USE_HIGH_QUALITY_ANTIALIASING
        painter.setRenderHints( QPainter::HighQualityAntialiasing |
                                QPainter::Antialiasing,
                                true );
#else
        painter.setRenderHint( QPainter::Antialiasing, true );
#endif
    }
}
