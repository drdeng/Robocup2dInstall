// -*-c++-*-

/*!
  \file ball_painter_rcss.cpp
  \brief rcssmonitor style: ball painter class Source File.
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

#include "ball_painter_rcss.h"

#include "draw_config.h"

#include "options.h"
#include "main_data.h"
#include "monitor_view_data.h"

#include <rcsc/common/server_param.h>
#include <rcsc/geom/vector_2d.h>

#include <vector>

/*-------------------------------------------------------------------*/
/*!

*/
void
BallPainterRCSS::draw( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( ! opt.showBall() )
    {
        return;
    }

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();
    if ( ! view )
    {
        return;
    }

    const DrawConfig & dconf = DrawConfig::instance();

    // decide radius
    const double ball_radius
        = std::max( 1.0,
                    ( opt.enlargeMode()
                      ? opt.scale( 0.3 )
                      : opt.scale( rcsc::ServerParam::i().ballSize() ) )
                    );
    const double kickable_radius
        = std::max( 1.0,
                    opt.scale( rcsc::ServerParam::i().defaultKickableArea() ) );

    const double ix = opt.screenX( view->ball().x() );
    const double iy = opt.screenY( view->ball().y() );

    // draw ball body
    painter.setPen( dconf.transparentPen() );
    painter.setBrush( dconf.ballBrush() );
    painter.drawEllipse( QRectF( ix - ball_radius,
                                 iy - ball_radius,
                                 ball_radius * 2,
                                 ball_radius * 2 ) );

    // draw kickable margin
    painter.setPen( dconf.ballPen() );
    painter.setBrush( dconf.transparentBrush() );
    painter.drawEllipse( QRectF( ix - kickable_radius,
                                 iy - kickable_radius,
                                 kickable_radius * 2,
                                 kickable_radius * 2 ) );

    // draw future status
    if ( opt.ballFutureCycle() > 0
         && view->ball().hasVelocity() )
    {
        drawFutureState( painter );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
BallPainterRCSS::drawFutureState( QPainter & painter ) const
{
    static QPen pen( QColor( 255, 0, 0 ), Qt::SolidLine );

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    const rcsc::ServerParam & SP = rcsc::ServerParam::i();
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    //QPainter::RenderHints hints = painter.renderHints();
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

    rcsc::Vector2D bpos( view->ball().x(),
                         view->ball().y() );
    rcsc::Vector2D bvel( view->ball().deltaX(),
                         view->ball().deltaY() );
    const double ball_speed = bvel.r();

    bpos *= opt.reverseValue();
    bvel *= opt.reverseValue();

    QPointF first_point( opt.absScreenX( bpos.x ),
                         opt.absScreenY( bpos.y ) );

    painter.save();
    painter.translate( first_point.x(), first_point.y() );
    painter.rotate( bvel.th().degree() );

    painter.setPen( pen );
    painter.setBrush( dconf.transparentBrush() );

    double travel = 0.0;
    double speed = ball_speed;
    double last_x = 0;
    const int max_loop = opt.ballFutureCycle();
    for ( int i = 0; i < max_loop; ++i )
    {
        travel += speed;
        speed *= SP.ballDecay();

        if ( speed < 0.5 )
        {
            break;
        }

        double x = opt.scale( travel );
        if ( std::abs( last_x - x ) < 1 )
        {
            break;
        }
        last_x = x;

        double ispeed_y = opt.scale( speed * 0.5 );
        painter.drawLine( QLineF( x, -ispeed_y,
                                  x, ispeed_y ) );
    }

    last_x = opt.scale( ball_speed / ( 1.0 - SP.ballDecay() ) );
    painter.drawLine( QLineF( 0.0, 0.0, last_x, 0.0 ) );

    painter.restore();

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
