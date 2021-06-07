// -*-c++-*-

/*!
  \file field_evaluation_painter.cpp
  \brief field evaluation painter class Source File.
*/

/*
 *Copyright:

 Copyright (C) Hiroki SHIMORA

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "field_evaluation_painter.h"

#include "draw_config.h"
#include "main_data.h"
#include "monitor_view_data.h"
#include "grid_field_evaluation_data.h"
#include "agent_id.h"
#include "options.h"

#include <rcsc/color/thermo_color_provider.h>
#include <rcsc/math_util.h>
#include <rcsc/game_time.h>

/*-------------------------------------------------------------------*/
/*!

*/
FieldEvaluationPainter::FieldEvaluationPainter( const MainData & main_data )
    : M_main_data( main_data )
{

}

/*-------------------------------------------------------------------*/
/*!

*/
void
FieldEvaluationPainter::draw( QPainter & painter )
{
    //
    // draw grid
    //
    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    if ( ! view )
    {
        return;
    }

    AgentID s = Options::instance().selectedAgent();

    if ( s.isNull() )
    {
        return;
    }

    paintGrid( painter, view->time(), s );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
FieldEvaluationPainter::paintGrid( QPainter & painter,
                                   const rcsc::GameTime & time,
                                   const AgentID & player )
{
#if 0
    const double TRANSPARENCY = 192; // range: [0, 255]
#else
    const double TRANSPARENCY = 224; // range: [0, 255]
#endif

    GridFieldEvaluationData::S_xy_value_map.clear();

    const boost::shared_ptr< const GridFieldEvaluationData > data_ptr
        = M_main_data.gridFieldEvaluationHolder().get( time, player );

    if ( ! data_ptr )
    {
        return;
    }

    const GridFieldEvaluationData::MapType & data = data_ptr->data();

    if ( data.empty() )
    {
        return;
    }

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

    const double min_rate = opt.evaluationViewMinRate();
    const double max_rate = opt.evaluationViewMaxRate();

    painter.setBrush( dconf.transparentBrush() );
    painter.setPen( dconf.transparentPen() );

    const double grid_size = data_ptr->gridSize();
    const double w = grid_size / 2.0;
    const double max_x = painter.window().width();
    const double max_y = painter.window().height();

    const rcsc::ThermoColorProvider color_provider;

    const size_t size = data.size();

    int i = 0;
    int nth = 0;
    double last_value = 0.0;

    // std::cerr << "start grid painting. data size=" << data.size() << std::endl;

    for ( GridFieldEvaluationData::MapType::const_iterator
              it = data.begin(), end = data.end();
          it != end;
          ++it, ++i )
    {
        const rcsc::Vector2D & pos = (*it).second;
        const QRectF rect( opt.screenX( pos.x - w ),
                           opt.screenY( pos.y - w ),
                           opt.scale( grid_size ),
                           opt.scale( grid_size ) );
        if ( rect.left() > max_x
             || rect.right() < 0.
             || rect.top() > max_y
             || rect.bottom() < 0.0 )
        {
            continue;
        }

        const double value = (*it).first;
        if ( last_value != value || i == 0 )
        {
            nth = i;
            last_value = value;
        }

        const double rate = static_cast< double >( size - nth ) / static_cast< double >( size );
        const double paint_range = max_rate - min_rate;

        rcsc::RGBColor c = color_provider.convertToColor( ( rate - min_rate ) / paint_range );
        // if ( rate < min_rate || paint_range < rcsc::EPS )
        // {
        //     c = rcsc::RGBColor( 0.0, 0.0, 0.0 );
        // }
        // else if ( max_rate < rate )
        // {
        //     c = rcsc::RGBColor( 1.0, 1.0, 1.0 );
        // }
        // else
        // {
        //     c = color_prover.convertToColor( ( rate - min_rate ) / paint_range );
        // }


        const QBrush brush( QColor( c.red8bit(), c.green8bit(), c.blue8bit(),
                                    TRANSPARENCY ) );
        painter.fillRect( rect, brush );

        std::pair< int, int > xy = std::make_pair( static_cast< int >( rint( pos.x / grid_size ) ),
                                                   static_cast< int >( rint( pos.y / grid_size ) ) );
        GridFieldEvaluationData::S_xy_value_map[xy] = it->first;
    }

    // std::cerr << "end grid painting" << std::endl;

    GridFieldEvaluationData::S_grid_size = grid_size;


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
