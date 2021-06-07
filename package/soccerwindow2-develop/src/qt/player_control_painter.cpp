// -*-c++-*-

/*!
  \file player_control_painter.cpp
  \brief player control painter class Source File.
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

#include "player_control_painter.h"

#include "draw_config.h"
// model
#include "options.h"
#include "main_data.h"
#include "monitor_view_data.h"

#include <rcsc/common/player_type.h>
#include <rcsc/common/server_param.h>
#include <rcsc/geom/circle_2d.h>

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerControlPainter::draw( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( ! opt.showKickAccelArea() )
    {
        return;
    }

    int number = opt.selectedNumber();
    if ( number == 0 )
    {
        return;
    }

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    if ( ! view )
    {
        return;
    }

    const rcsc::rcg::BallT & ball = view->ball();

    if ( ! ball.hasVelocity() )
    {
        return;
    }

    const rcsc::ServerParam & SP = rcsc::ServerParam::i();
    const DrawConfig & dconf = DrawConfig::instance();

    const rcsc::rcg::PlayerT & player = view->players()[ ( number > 0
                                                           ? number - 1
                                                           : -number - 1 + 11 ) ];
    const rcsc::PlayerType & ptype = M_main_data.viewHolder().playerType( player.type() );

    rcsc::Vector2D ppos( player.x(), player.y() );
    rcsc::Vector2D bpos( ball.x(), ball.y() );
    rcsc::Vector2D player_to_ball = bpos - ppos;
    player_to_ball.rotate( - player.body() );

    rcsc::Vector2D bnext( bpos.x + ball.deltaX(),
                          bpos.y + ball.deltaY() );
    double ball_dist = player_to_ball.r();

    if ( ball_dist < ptype.kickableArea() )
    {
        double max_kick_accel
            = SP.maxPower()
            //* SP.kickPowerRate()
            * ptype.kickPowerRate()
            * ( 1.0 - 0.25 * player_to_ball.th().abs() / 180.0
                - 0.25
                * ( ball_dist - ptype.playerSize() - SP.ballSize() )
                / ptype.kickableMargin() );

        if ( max_kick_accel > SP.ballAccelMax() )
        {
            max_kick_accel = SP.ballAccelMax();
        }

        QPointF bpos_screen( opt.screenX( bpos.x ),
                             opt.screenY( bpos.y ) );
        QPointF bnext_screen( opt.screenX( bnext.x ),
                              opt.screenY( bnext.y ) );
        double max_speed_screen = opt.scale( SP.ballSpeedMax() );
        double max_kick_accel_screen = opt.scale( max_kick_accel );

        painter.setPen( dconf.kickAccelPen() );
        painter.setBrush( dconf.transparentBrush() );

        // draw no noise next ball move only by inertia
        painter.drawLine( QLineF( bpos_screen, bnext_screen ) );

        rcsc::Circle2D max_speed_circle( bpos, SP.ballSpeedMax() );
        rcsc::Circle2D max_accel_circle( bnext, max_kick_accel );
        rcsc::Vector2D intersection_1, intersection_2;

        if ( max_speed_circle.intersection( max_accel_circle,
                                            &intersection_1,
                                            &intersection_2 ) != 2 )

        {
            // no intersection points

            // just draw a next ball reachable area by max accel
            painter.drawEllipse( QRectF( bnext_screen.x() - max_kick_accel_screen,
                                         bnext_screen.y() - max_kick_accel_screen,
                                         max_kick_accel_screen * 2,
                                         max_kick_accel_screen * 2 ) );
        }
        else
        {
            // exists 2 intersection points

            rcsc::AngleDeg bpos_to_intersection_1 = ( intersection_1 - bpos ).th();
            rcsc::AngleDeg bpos_to_intersection_2 = ( intersection_2 - bpos ).th();

            rcsc::AngleDeg bpos_to_bnext_angle = ( bnext - bpos ).th();

            rcsc::AngleDeg * bpos_start_angle = 0;
            double bpos_angle_span = 0.0;
            if ( bpos_to_intersection_1.isLeftOf( bpos_to_bnext_angle ) )
            {
                bpos_start_angle = &bpos_to_intersection_1;
                bpos_angle_span = ( bpos_to_intersection_2 - bpos_to_intersection_1 ).degree();
                if ( bpos_angle_span < 0.0 )
                {
                    bpos_angle_span += 360.0;
                }
                bpos_angle_span *= -1.0;
            }
            else
            {
                bpos_start_angle = &bpos_to_intersection_2;
                bpos_angle_span = ( bpos_to_intersection_1 - bpos_to_intersection_2 ).degree();
                if ( bpos_angle_span < 0.0 )
                {
                    bpos_angle_span += 360.0;
                }
                bpos_angle_span *= -1.0;
            }

            if ( opt.reverseSide() )
            {
                *bpos_start_angle += 180.0;
            }

            int bpos_start_angle_int
                = static_cast< int >( rint( - bpos_start_angle->degree() * 16 ) );
            int bpos_angle_span_int
                = static_cast< int >( rint( bpos_angle_span * 16 ) );
            painter.drawArc( QRectF( bpos_screen.x() - max_speed_screen,
                                     bpos_screen.y() - max_speed_screen,
                                     max_speed_screen * 2,
                                     max_speed_screen * 2 ),
                             bpos_start_angle_int,
                             bpos_angle_span_int  );

            rcsc::AngleDeg bnext_to_intersection_1 = ( intersection_1 - bnext ).th();
            rcsc::AngleDeg bnext_to_intersection_2 = ( intersection_2 - bnext ).th();

            rcsc::AngleDeg bnext_to_bpos_angle = bpos_to_bnext_angle + 180.0;

            rcsc::AngleDeg * bnext_start_angle = 0;
            double bnext_angle_span = 0.0;
            if ( bnext_to_intersection_1.isLeftOf( bnext_to_bpos_angle ) )
            {
                bnext_start_angle = &bnext_to_intersection_1;
                bnext_angle_span = ( bnext_to_intersection_2 - bnext_to_intersection_1 ).degree();
                if ( bnext_angle_span < 0.0 )
                {
                    bnext_angle_span += 360.0;
                }
                bnext_angle_span *= -1.0;
            }
            else
            {
                bnext_start_angle = &bnext_to_intersection_2;
                bnext_angle_span = ( bnext_to_intersection_1 - bnext_to_intersection_2 ).degree();
                if ( bnext_angle_span < 0.0 )
                {
                    bnext_angle_span += 360.0;
                }
                bnext_angle_span *= -1.0;
            }

            if ( opt.reverseSide() )
            {
                *bnext_start_angle += 180.0;
            }

            int bnext_start_angle_int
                = static_cast< int >( rint( - bnext_start_angle->degree() * 16 ) );
            int bnext_angle_span_int
                = static_cast< int >( rint( bnext_angle_span * 16 ) );
            painter.drawArc( QRectF( bnext_screen.x() - max_kick_accel_screen,
                                     bnext_screen.y() - max_kick_accel_screen,
                                     max_kick_accel_screen * 2,
                                     max_kick_accel_screen * 2 ),
                             bnext_start_angle_int,
                             bnext_angle_span_int );
        }

        // draw kick info text
        painter.setFont( dconf.playerFont() );
        painter.setPen( dconf.kickAccelPen() );

        //snprintf( buf, 32, "MaxAccel=%.3f", max_kick_accel );
        painter.drawText( QPointF( bnext_screen.x() + 10,
                                   bnext_screen.y() + painter.fontMetrics().ascent() ),
                          QString( "MaxAccel=%1" ).arg( max_kick_accel, 0, 'g', 4 ) );
    }
}
