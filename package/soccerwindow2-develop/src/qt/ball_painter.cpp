// -*-c++-*-

/*!
  \file ball_painter.cpp
  \brief ball painter class Source File.
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

#include "ball_painter.h"

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
BallPainter::draw( QPainter & painter )
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
                      ? opt.scale( opt.ballSize() )
                      : opt.scale( rcsc::ServerParam::i().ballSize() ) )
                    );
    const double ball_diameter = ball_radius * 2.0;
    const double ix = opt.screenX( view->ball().x() );
    const double iy = opt.screenY( view->ball().y() );


    // set GDI objects
    painter.setPen( dconf.transparentPen() );

    if ( opt.gradient() )
    {
        QRadialGradient gradient( ix, iy,
                                  ball_radius,
                                  ix - ball_radius*0.5, iy - ball_radius*0.5 );
        gradient.setColorAt( 0.0, dconf.ballBrush().color() );
        //gradient.setColorAt( 1.0, dconf.ballBrush().color().darker( 127 ) );
        gradient.setColorAt( 1.0, dconf.ballBrush().color().darker( 115 ) );
        //gradient.setColorAt( 1.0, Qt::lightGray );
        painter.setBrush( gradient );
        painter.setPen( Qt::black );
    }
    else
    {
        painter.setBrush( dconf.ballBrush() );
    }

    // draw ball body
    painter.drawEllipse( QRectF( ix - ball_radius,
                                 iy - ball_radius,
                                 ball_diameter,
                                 ball_diameter ) );

    // draw setplay ball owner team color circle
    if ( view->isLeftSetPlay() )
    {
        double color_radius = ball_radius;

        if ( opt.gradient() )
        {
            //color_radius = ball_radius;
            QRadialGradient gradient( ix,
                                      iy,
                                      color_radius,
                                      ix - color_radius/2,
                                      iy - color_radius/2 );
            gradient.setColorAt( 0.0, Qt::white );
            gradient.setColorAt( 1.0, dconf.leftTeamBrush().color() );
            painter.setBrush( gradient );
        }
        else
        {
            color_radius = std::max( 1.0, ball_radius - 2.0 );
            painter.setBrush( dconf.leftTeamBrush() );
        }

        painter.drawEllipse( QRectF( ix - color_radius,
                                     iy - color_radius,
                                     color_radius * 2,
                                     color_radius * 2 ) );
    }
    else if ( view->isRightSetPlay() )
    {
        double color_radius = ball_radius;

        if ( opt.gradient() )
        {
            //color_radius = ball_radius;
            QRadialGradient gradient( ix,
                                      iy,
                                      color_radius,
                                      ix - color_radius/2,
                                      iy - color_radius/2 );
            gradient.setColorAt( 0.0, Qt::white );
            gradient.setColorAt( 1.0, dconf.rightTeamBrush().color() );
            painter.setBrush( gradient );
        }
        else
        {
            color_radius = std::max( 1.0, ball_radius - 2.0 );
            painter.setBrush( dconf.rightTeamBrush() );
        }

        painter.drawEllipse( QRectF( ix - color_radius,
                                     iy - color_radius,
                                     color_radius * 2,
                                     color_radius * 2 ) );
    }

    // draw additional circle
    if ( ! opt.enlargeMode()
         || ball_radius <= 1  )
    {
        painter.setPen( dconf.ballPen() );
        painter.setBrush( dconf.transparentBrush() );

        double kickable_radius
            = opt.scale( rcsc::ServerParam::i().defaultKickableMargin() );
        painter.drawEllipse( QRectF( ix - kickable_radius,
                                     iy - kickable_radius,
                                     kickable_radius * 2,
                                     kickable_radius * 2 ) );
    }

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
BallPainter::drawFutureState( QPainter & painter ) const
{
    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    const rcsc::ServerParam & SP = rcsc::ServerParam::i();
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

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

    bpos *= opt.reverseValue();
    bvel *= opt.reverseValue();

    QPointF first_point( opt.absScreenX( bpos.x ),
                         opt.absScreenY( bpos.y ) );
    QPointF last_point = first_point;
    const int last = opt.ballFutureCycle();

#ifdef USE_GLWIDGET
    painter.setPen( dconf.ballPen() );
    painter.setBrush( dconf.transparentBrush() );

    for ( int i = 0; i < last; ++i )
    {
        bpos += bvel;
        bvel *= SP.ballDecay();

        QPointF pt( opt.absScreenX( bpos.x ),
                    opt.absScreenY( bpos.y ) );
        if ( std::fabs( last_point.x() - pt.x() ) < 1
             && std::fabs( last_point.y() - pt.y() ) < 1 )
        {
            break;
        }
        last_point = pt;
        painter.drawEllipse( QRectF( pt.x() - 1, pt.y() - 1, 2, 2 ) );
    }

    painter.drawLine( QLineF( first_point, last_point ) );
#else
    QPainterPath path;
    for ( int i = 0; i < last; ++i )
    {
        bpos += bvel;
        bvel *= SP.ballDecay();

        QPointF pt( opt.absScreenX( bpos.x ),
                    opt.absScreenY( bpos.y ) );
        if ( std::fabs( last_point.x() - pt.x() ) < 1
             && std::fabs( last_point.y() - pt.y() ) < 1 )
        {
            break;
        }
        last_point = pt;
        path.addEllipse( pt.x() - 1,
                         pt.y() - 1,
                         2,
                         2 );
    }

    path.moveTo( first_point );
    path.lineTo( last_point );

    painter.setPen( dconf.ballPen() );
    painter.setBrush( dconf.transparentBrush() );

    painter.drawPath( path );

#endif

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
