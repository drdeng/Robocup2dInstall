// -*-c++-*-

/*!
  \file debug_log_painter.cpp
  \brief debug log_info painter class Source File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa Akicyama

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

#include "debug_log_painter.h"

#include "draw_config.h"

#include "options.h"
#include "main_data.h"
#include "debug_log_data.h"
#include "debug_log_holder.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogPainter::draw( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( ! opt.showDebugLogObjects() )
    {
        return;
    }

    const int level = M_main_data.debugLogHolder().level();
    if ( level == 0 )
    {
        return;
    }

    const int number = opt.selectedNumber();
    if ( number == 0 )
    {
        return;
    }

    const int unum = std::abs( number );

    boost::shared_ptr< const DebugLogData > data
        = M_main_data.debugLogHolder().getData( unum );

    if ( ! data )
    {
        return;
    }

    MonitorViewData::ConstPtr monitor_view = M_main_data.getCurrentViewData();
    if ( ! monitor_view )
    {
        return;
    }

    if ( monitor_view->time() != data->time() )
    {
        return;
    }

    const rcsc::SideID player_side = ( number < 0 ? rcsc::RIGHT : rcsc::LEFT );

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


    drawPoints( painter, player_side, *data );
    drawLines( painter, player_side, *data );
    drawArcs( painter, player_side, *data );
    drawCircles( painter, player_side, *data );
    drawTriangles( painter, player_side, *data );
    drawRectangles( painter, player_side, *data );
    drawSectors( painter, player_side, *data );
    drawMessages( painter, player_side, *data );

    drawActionSequence( painter, player_side );

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

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogPainter::drawPoints( QPainter & painter,
                             const rcsc::SideID player_side,
                             const DebugLogData & log_data ) const
{
    const int level = M_main_data.debugLogHolder().level();

    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    const double reverse = ( player_side == rcsc::LEFT
                             ? 1.0
                             : -1.0 );

    painter.setBrush( dconf.transparentBrush() );

    const DebugLogData::PointCont::const_iterator end = log_data.pointCont().end();
    for ( DebugLogData::PointCont::const_iterator it = log_data.pointCont().begin();
          it != end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
            }
            else
            {
                painter.setPen( dconf.debugShapePen() );
            }

            painter.drawPoint( QPointF( opt.screenX( it->x_ * reverse ),
                                        opt.screenY( it->y_ * reverse ) ) );
        }
    }

}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogPainter::drawLines( QPainter & painter,
                            const rcsc::SideID player_side,
                            const DebugLogData & log_data ) const
{
    const int level = M_main_data.debugLogHolder().level();

    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    const double reverse = ( player_side == rcsc::LEFT
                             ? 1.0
                             : -1.0 );

    painter.setBrush( dconf.transparentBrush() );

    const DebugLogData::LineCont::const_iterator end = log_data.lineCont().end();
    for ( DebugLogData::LineCont::const_iterator it = log_data.lineCont().begin();
          it != end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
            }
            else
            {
                painter.setPen( dconf.debugShapePen() );
            }

            painter.drawLine( QLineF( opt.screenX( it->x1_ * reverse ),
                                      opt.screenY( it->y1_ * reverse ),
                                      opt.screenX( it->x2_ * reverse ),
                                      opt.screenY( it->y2_ * reverse ) ) );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogPainter::drawArcs( QPainter & painter,
                           const rcsc::SideID player_side,
                           const DebugLogData & log_data ) const
{
    const int level = M_main_data.debugLogHolder().level();

    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    const double reverse = ( player_side == rcsc::LEFT
                             ? 1.0
                             : -1.0 );

    painter.setBrush( dconf.transparentBrush() );

    const DebugLogData::ArcCont::const_iterator end = log_data.arcCont().end();
    for ( DebugLogData::ArcCont::const_iterator it = log_data.arcCont().begin();
          it != end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
            }
            else
            {
                painter.setPen( dconf.debugShapePen() );
            }

            const double circumference_factor = ( 2.0 * M_PI ) * std::fabs( it->span_angle_ / 360.0 );
            const double len = ( it->r_ * opt.fieldScale() ) * circumference_factor;

            const int min_min_loop = ( it->span_angle_ < 45.0
                                       ? 1
                                       : it->span_angle_ < 90.0
                                       ? 2
                                       : it->span_angle_ < 180.0
                                       ? 3
                                       : 4 );

            const int loop = std::min( std::max( min_min_loop,
                                                 static_cast< int >( rint( len / 32.0 ) ) ),
                                       18 );

            const double angle_step = ( loop == 1
                                        ? 0.0
                                        : it->span_angle_ / ( loop - 1 ) );

            rcsc::AngleDeg angle = it->start_angle_;
            rcsc::Vector2D rpos = rcsc::Vector2D::polar2vector( it->r_, angle );

            if ( loop == 1 )
            {
                painter.drawPoint( QPointF( opt.screenX( ( it->x_ + rpos.x ) * reverse ),
                                            opt.screenY( ( it->y_ + rpos.y ) * reverse ) ) );
                continue;
            }


            QPainterPath path;

            path.moveTo( opt.screenX( ( it->x_ + rpos.x ) * reverse ),
                         opt.screenY( ( it->y_ + rpos.y ) * reverse ) );

            angle += angle_step;
            for ( int i = 1; i < loop; ++i, angle += angle_step )
            {
                rpos = rcsc::Vector2D::polar2vector( it->r_, angle );
                path.lineTo( opt.screenX( ( it->x_ + rpos.x ) * reverse ),
                             opt.screenY( ( it->y_ + rpos.y ) * reverse ) );
            }

            painter.drawPath( path );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogPainter::drawCircles( QPainter & painter,
                              const rcsc::SideID player_side,
                              const DebugLogData & log_data ) const
{
    const int level = M_main_data.debugLogHolder().level();

    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    const double reverse = ( player_side == rcsc::LEFT
                             ? 1.0
                             : -1.0 );

    const DebugLogData::CircleCont::const_iterator fc_end = log_data.filledCircleCont().end();
    for ( DebugLogData::CircleCont::const_iterator it = log_data.filledCircleCont().begin();
          it != fc_end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
                painter.setBrush( col );
            }
            else
            {
                painter.setPen( dconf.debugShapePen() );
                painter.setBrush( dconf.debugShapePen().color() );
            }

            double r = opt.scale( it->r_ );
            painter.drawEllipse( QRectF( opt.screenX( ( it->x_ - it->r_ ) * reverse ),
                                         opt.screenY( ( it->y_ - it->r_ ) * reverse ),
                                         r * 2, r * 2 ) );
        }
    }

    painter.setBrush( dconf.transparentBrush() );

    const DebugLogData::CircleCont::const_iterator c_end = log_data.circleCont().end();
    for ( DebugLogData::CircleCont::const_iterator it = log_data.circleCont().begin();
          it != c_end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
            }
            else
            {
                painter.setPen( dconf.debugShapePen() );
            }

            double r = opt.scale( it->r_ );
            painter.drawEllipse( QRectF( opt.screenX( ( it->x_ - it->r_ ) * reverse ),
                                         opt.screenY( ( it->y_ - it->r_ ) * reverse ),
                                         r * 2, r * 2 ) );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogPainter::drawTriangles( QPainter & painter,
                                const rcsc::SideID player_side,
                                const DebugLogData & log_data ) const
{
    const int level = M_main_data.debugLogHolder().level();

    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    const double reverse = ( player_side == rcsc::LEFT
                             ? 1.0
                             : -1.0 );

    const DebugLogData::TriangleCont::const_iterator ft_end = log_data.filledTriangleCont().end();
    for ( DebugLogData::TriangleCont::const_iterator it = log_data.filledTriangleCont().begin();
          it != ft_end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
                painter.setBrush( col );
            }
            else
            {
                painter.setPen( dconf.debugShapePen() );
                painter.setBrush( dconf.debugShapePen().color() );
            }

            QPointF points[4];
            points[0].setX( opt.screenX( it->x1_ * reverse ) );
            points[0].setY( opt.screenY( it->y1_ * reverse ) );
            points[1].setX( opt.screenX( it->x2_ * reverse ) );
            points[1].setY( opt.screenY( it->y2_ * reverse ) );
            points[2].setX( opt.screenX( it->x3_ * reverse ) );
            points[2].setY( opt.screenY( it->y3_ * reverse ) );
            points[3] = points[0];

            painter.drawPolyline( points, 4 );
        }
    }

    painter.setBrush( dconf.transparentBrush() );

    const DebugLogData::TriangleCont::const_iterator t_end = log_data.triangleCont().end();
    for ( DebugLogData::TriangleCont::const_iterator it = log_data.triangleCont().begin();
          it != t_end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
            }
            else
            {
                painter.setPen( dconf.debugShapePen() );
            }

            QPointF points[4];
            points[0].setX( opt.screenX( it->x1_ * reverse ) );
            points[0].setY( opt.screenY( it->y1_ * reverse ) );
            points[1].setX( opt.screenX( it->x2_ * reverse ) );
            points[1].setY( opt.screenY( it->y2_ * reverse ) );
            points[2].setX( opt.screenX( it->x3_ * reverse ) );
            points[2].setY( opt.screenY( it->y3_ * reverse ) );
            points[3] = points[0];

            painter.drawPolyline( points, 4 );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogPainter::drawRectangles( QPainter & painter,
                                 const rcsc::SideID player_side,
                                 const DebugLogData & log_data ) const
{
    const int level = M_main_data.debugLogHolder().level();

    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    const double reverse = ( player_side == rcsc::LEFT
                             ? 1.0
                             : -1.0 );

    const DebugLogData::RectCont::const_iterator fr_end = log_data.filledRectCont().end();
    for ( DebugLogData::RectCont::const_iterator it = log_data.filledRectCont().begin();
          it != fr_end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
                painter.setBrush( col );
            }
            else
            {
                painter.setPen( dconf.debugShapePen() );
                painter.setBrush( dconf.debugShapePen().color() );
            }

            painter.drawRect( QRectF( opt.screenX( it->left_ * reverse ),
                                      opt.screenY( it->top_ * reverse ),
                                      opt.scale( it->width_ ),
                                      opt.scale( it->height_ ) ) );
        }
    }

    painter.setBrush( dconf.transparentBrush() );

    const DebugLogData::RectCont::const_iterator r_end = log_data.rectCont().end();
    for ( DebugLogData::RectCont::const_iterator it = log_data.rectCont().begin();
          it != r_end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
            }
            else
            {
                painter.setPen( dconf.debugShapePen() );
            }

            painter.drawRect( QRectF( opt.screenX( it->left_ * reverse ),
                                      opt.screenY( it->top_ * reverse ),
                                      opt.scale( it->width_ ),
                                      opt.scale( it->height_ ) ) );
        }
    }
}

namespace {
/*-------------------------------------------------------------------*/
/*!

*/
void
draw_sector( QPainter & painter,
             const rcsc::SideID player_side,
             const DebugLogData::SectorT & sector )
{
    const Options & opt = Options::instance();
    const double reverse = ( player_side == rcsc::LEFT
                             ? 1.0
                             : -1.0 );

    const double circumference_factor = ( 2.0 * M_PI ) * std::fabs( sector.span_angle_ / 360.0 );
    const double min_len = ( sector.min_r_ * opt.fieldScale() ) * circumference_factor;
    const double max_len = ( sector.max_r_ * opt.fieldScale() ) * circumference_factor;

    const int min_min_loop = ( sector.span_angle_ < 1.0
                               ? 1
                               : sector.span_angle_ < 45.0
                               ? 4
                               : sector.span_angle_ < 90.0
                               ? 6
                               : sector.span_angle_ < 180.0
                               ? 8
                               : sector.span_angle_ < 270.0
                               ? 12
                               : 16 );

    const int min_loop = std::min( std::max( min_min_loop,
                                             static_cast< int >( rint( min_len / 32.0 ) ) ),
                                   18 );
    const int max_loop = std::min( std::max( min_min_loop,
                                             static_cast< int >( rint( max_len / 32.0 ) ) ),
                                   18 );

    const double min_angle_step = ( min_loop == 1
                                    ? 0.0
                                    : sector.span_angle_ / ( min_loop - 1 ) );
    const double max_angle_step = ( min_loop == 1
                                    ? 0.0
                                    : sector.span_angle_ / ( max_loop - 1 ) );

    QPainterPath path;

    rcsc::AngleDeg angle = sector.start_angle_;

    rcsc::Vector2D rpos = rcsc::Vector2D::polar2vector( sector.max_r_, angle );
    path.moveTo( opt.screenX( ( sector.x_ + rpos.x ) * reverse ),
                 opt.screenY( ( sector.y_ + rpos.y ) * reverse ) );

    angle += max_angle_step;
    for ( int i = 1; i < max_loop; ++i, angle += max_angle_step )
    {
        rpos = rcsc::Vector2D::polar2vector( sector.max_r_, angle );
        path.lineTo( opt.screenX( ( sector.x_ + rpos.x  ) * reverse ),
                     opt.screenY( ( sector.y_ + rpos.y ) * reverse ) );
    }

    if ( sector.max_r_ <= 1.0e-5 )
    {
        rpos = rcsc::Vector2D::polar2vector( sector.min_r_, angle );
    }
    else
    {
        rpos *= sector.min_r_ / sector.max_r_;
    }

    path.lineTo( opt.screenX( ( sector.x_ + rpos.x  ) * reverse ),
                 opt.screenY( ( sector.y_ + rpos.y ) * reverse ) );

    angle -= min_angle_step;
    for ( int i = 0; i < min_loop; ++i, angle -= min_angle_step )
    {
        rpos = rcsc::Vector2D::polar2vector( sector.min_r_, angle );
        path.lineTo( opt.screenX( ( sector.x_ + rpos.x  ) * reverse ),
                     opt.screenY( ( sector.y_ + rpos.y ) * reverse ) );
    }

    path.closeSubpath();

    painter.drawPath( path );
}

}


/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogPainter::drawSectors( QPainter & painter,
                              const rcsc::SideID player_side,
                              const DebugLogData & log_data ) const
{
    const int level = M_main_data.debugLogHolder().level();

    const DrawConfig & dconf = DrawConfig::instance();

    const DebugLogData::SectorCont::const_iterator f_end = log_data.filledSectorCont().end();
    for ( DebugLogData::SectorCont::const_iterator it = log_data.filledSectorCont().begin();
          it != f_end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
                painter.setBrush( col );
            }
            else
            {
                painter.setPen( dconf.debugShapePen() );
                painter.setBrush( dconf.debugShapePen().color() );
            }

            draw_sector( painter, player_side, *it );
        }
    }

    painter.setBrush( dconf.transparentBrush() );

    const DebugLogData::SectorCont::const_iterator end = log_data.sectorCont().end();
    for ( DebugLogData::SectorCont::const_iterator it = log_data.sectorCont().begin();
          it != end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
            }
            else
            {
                painter.setPen( dconf.debugShapePen() );
            }

            draw_sector( painter, player_side, *it );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogPainter::drawMessages( QPainter & painter,
                               const rcsc::SideID player_side,
                               const DebugLogData & log_data ) const
{
    const int level = M_main_data.debugLogHolder().level();

    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    const double reverse = ( player_side == rcsc::LEFT
                             ? 1.0
                             : -1.0 );

    painter.setFont( dconf.debugLogMessageFont() );
    painter.setBrush( dconf.transparentBrush() );

    const DebugLogData::MessageCont::const_iterator end = log_data.messageCont().end();
    for ( DebugLogData::MessageCont::const_iterator it = log_data.messageCont().begin();
          it != end;
          ++it )
    {
        if ( level & it->level_ )
        {
            QColor col( it->color_.c_str() );
            if ( col.isValid() )
            {
                painter.setPen( col );
            }
            else
            {
                painter.setPen( dconf.debugLogMessageFontPen() );
            }

            painter.drawText( QPointF( opt.screenX( it->x_ * reverse ),
                                       opt.screenY( it->y_ * reverse ) ),
                              QString::fromStdString( it->message_ ) );
        }
    }

}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogPainter::drawActionSequence( QPainter & painter,
                                     const rcsc::SideID player_side ) const
{
    if ( M_main_data.actionSequenceTime() != M_main_data.debugLogHolder().currentTime() )
    {
        return;
    }

    ActionSequenceDescription::ConstPtr ptr = M_main_data.getSelectedActionSequence();
    if ( ! ptr
         || ptr->actions().empty() )
    {
        return;
    }

    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    const double reverse = ( player_side == rcsc::LEFT
                             ? 1.0
                             : -1.0 );

    painter.setBrush( dconf.transparentBrush() );

    double r = opt.scale( 0.5 );
    for ( std::vector< ActionDescription >::const_iterator it = ptr->actions().begin(),
              end = ptr->actions().end();
          it != end;
          ++it )
    {
        QPointF to( opt.screenX( it->toPos().x * reverse ),
                    opt.screenY( it->toPos().y * reverse ) );

        if ( it->category() == ActionDescription::PASS )
        {
            painter.setPen( dconf.debugActionPassPen() );
        }
        else
        {
            painter.setPen( dconf.debugActionSequencePen() );
        }

        painter.drawLine( QPointF( opt.screenX( it->fromPos().x * reverse ),
                                   opt.screenY( it->fromPos().y * reverse ) ),
                          to );
        painter.drawEllipse( to, r, r );
    }

    painter.setPen( dconf.debugLogMessageFontPen() );
    for ( std::vector< ActionDescription >::const_iterator it = ptr->actions().begin(),
              end = ptr->actions().end();
          it != end;
          ++it )
    {
        QPointF to( opt.screenX( it->toPos().x * reverse ) + r,
                    opt.screenY( it->toPos().y * reverse ) + r );
        painter.drawText( to, QString( "%1,%2" ).arg( it->safeLevel() ).arg( it->value() ) );
    }

    QPointF text_pos( opt.screenX( ptr->actions().back().toPos().x * reverse ) + r,
                      opt.screenY( ptr->actions().back().toPos().y * reverse ) - r );

    painter.setPen( dconf.debugActionSequencePen() );
    painter.drawText( text_pos, QString::number( ptr->value() ) );
}
