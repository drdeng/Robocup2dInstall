// -*-c++-*-

/*!
  \file ball_trace_painter.cpp
  \brief ball trace painter class Source File.
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

#include "ball_trace_painter.h"

#include "draw_config.h"

#include "options.h"
#include "main_data.h"
#include "monitor_view_data.h"


/*-------------------------------------------------------------------*/
/*!

*/
BallTracePainter::BallTracePainter( const MainData & main_data )
    : M_main_data( main_data )
    , M_line_trace( false )
    , M_center_x( 0 )
    , M_center_y( 0 )
    , M_field_scale( 0.0 )
{
    M_point_pixmap
        = boost::shared_ptr< QPixmap >( new QPixmap( 5, 5 ) );

    M_point_pixmap->fill( Qt::black );

    QPainter painter( M_point_pixmap.get() );
    painter.setPen( DrawConfig::instance().ballPen() );
    painter.setBrush( Qt::gray );
    painter.drawEllipse( QRectF( 0, 0, 5, 5 ) );
    painter.end();

    M_point_pixmap->setMask( M_point_pixmap->createMaskFromColor( Qt::black ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
BallTracePainter::draw( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( ! opt.showBallTrace() )
    {
        return;
    }

#if 1
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

    drawTrace( painter );

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
#else
    if ( opt.isBallAutoTrace()
         || opt.focusType() != ViewConfig::FOCUS_POINT )
    {
        drawTrace( painter );
        return;
    }
    bool redraw = opt.isBallAutoTrace();
    if ( ! M_all_pixmap
         || M_all_pixmap->size() != painter.window().size() )
    {
        M_all_pixmap
            = boost::shared_ptr< QPixmap >
            ( new QPixmap( painter.window().size() ) );
        redraw = true;
    }

    if ( M_center_x != opt.fieldCenter().x
         || M_center_y != opt.fieldCenter().y
         || M_field_scale != opt.fieldScale()
         || M_line_trace != opt.isLineTrace() )
    {
        M_center_x = opt.fieldCenter().x;
        M_center_y = opt.fieldCenter().y;
        M_field_scale = opt.fieldScale();
        M_line_trace = opt.isLineTrace();
        redraw = true;
    }

    if ( redraw )
    {
        M_all_pixmap->fill( Qt::yellow );

        QPainter pm_painter( M_all_pixmap.get() );
        if ( ! drawTrace( pm_painter ) )
        {
            return;
        }
        pm_painter.end();

        M_all_pixmap->setMask( M_all_pixmap->createMaskFromColor( Qt::yellow ) );
    }

    painter.drawPixmap( 0, 0, *M_all_pixmap );
#endif
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
BallTracePainter::drawTrace( QPainter & painter )
{
    const ViewHolder & holder = M_main_data.viewHolder();
    const MonitorViewData::Cont & vc = holder.monitorViewCont();

    if ( vc.empty() )
    {
        return false;
    }

    const Options & opt = Options::instance();
    int start_cycle = opt.ballTraceStart();
    int end_cycle = opt.ballTraceEnd();

    if ( opt.ballAutoTrace() )
    {
        MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();
        if ( ! view )
        {
            return false;
        }
        start_cycle = std::max( 1L, view->time().cycle() + opt.autoTraceStart() );
    }
    else if ( start_cycle >= end_cycle )
    {
        return false;
    }

    const std::size_t first = holder.getIndexOf( start_cycle );
    const std::size_t last = ( opt.ballAutoTrace()
                               ? std::min( first + opt.autoTracePeriod(),
                                           vc.size() - 1 )
                               : holder.getIndexOf( end_cycle ) );
    if ( first >= last )
    {
        return false;
    }

    const bool line_trace = opt.lineTrace();

    painter.setBrush( DrawConfig::instance().transparentBrush() );

    const QPen & bpen = DrawConfig::instance().ballPen();
    QPen black_dot_pen( Qt::black );
    black_dot_pen.setStyle( Qt::DotLine );

    std::size_t i = first;
    QPointF prev( opt.screenX( vc[i]->ball().x() ),
                  opt.screenY( vc[i]->ball().y() ) );
    ++i;
    for ( ; i <= last; ++i )
    {
        switch ( vc[i]->playmode() ) {
        case rcsc::PM_BeforeKickOff:
        case rcsc::PM_TimeOver:
        case rcsc::PM_KickOff_Left:
        case rcsc::PM_KickOff_Right:
            prev.setX( opt.screenX( 0.0 ) );
            prev.setY( opt.screenY( 0.0 ) );
            continue;
        case rcsc::PM_PlayOn:
        case rcsc::PM_AfterGoal_Left:
        case rcsc::PM_AfterGoal_Right:
            painter.setPen( bpen );
            break;
        default:
            painter.setPen( black_dot_pen );
            break;
        }

        const rcsc::rcg::BallT & b = vc[i]->ball();
        QPointF next( opt.screenX( b.x() ),
                      opt.screenY( b.y() ) );

        painter.drawLine( QLineF( prev, next ) );
        if ( ! line_trace )
        {
            painter.drawPixmap( QPointF( next.x() - 2, next.y() - 2 ),
                                *M_point_pixmap );
        }
        prev = next;
    }

    return true;
}
