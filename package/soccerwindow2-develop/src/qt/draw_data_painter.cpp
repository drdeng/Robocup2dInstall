// -*-c++-*-

/*!
  \file draw_data_painter.cpp
  \brief draw data painter class Source File.
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

#include "draw_data_painter.h"

#include "options.h"
#include "main_data.h"
#include "draw_data_holder.h"

#include <iostream>

namespace {

/*-------------------------------------------------------------------*/
void
draw_texts( QPainter & painter,
            const DrawDataHolder::TextCont & cont )
{
    const Options & opt = Options::instance();

    for ( const auto & t : cont )
    {
        QColor col( t.color_.c_str() );
        if ( col.isValid() )
        {
            painter.setPen( col );
        }
        else
        {
            painter.setPen( Qt::white );
        }

        painter.drawText( QPointF( opt.screenX( t.x_ ),
                                   opt.screenY( t.y_ ) ),
                          QString::fromStdString( t.msg_ ) );
    }
}

/*-------------------------------------------------------------------*/
void
draw_points( QPainter & painter,
             const DrawDataHolder::PointCont & cont )
{
    const Options & opt = Options::instance();

    for ( const auto & p : cont )
    {
        QColor col( p.color_.c_str() );
        if ( col.isValid() )
        {
            painter.setPen( col );
        }
        else
        {
            painter.setPen( Qt::white );
        }

        painter.drawPoint( QPointF( opt.screenX( p.x_ ),
                                    opt.screenY( p.y_ ) ) );
    }
}

/*-------------------------------------------------------------------*/
void
draw_lines( QPainter & painter,
            const DrawDataHolder::LineCont & cont )
{
    const Options & opt = Options::instance();

    for ( const auto & l : cont )
    {
        QColor col( l.color_.c_str() );
        if ( col.isValid() )
        {
            painter.setPen( col );
        }
        else
        {
            painter.setPen( Qt::white );
        }

        painter.drawLine( QLineF( opt.screenX( l.x1_ ),
                                  opt.screenY( l.y1_ ),
                                  opt.screenX( l.x2_ ),
                                  opt.screenY( l.y2_ ) ) );
    }
}

/*-------------------------------------------------------------------*/
void
draw_rects( QPainter & painter,
            const DrawDataHolder::RectCont & cont )
{
    const Options & opt = Options::instance();

    for ( const auto & r : cont )
    {
        QColor line_col( r.line_color_.c_str() );
        QColor fill_col( r.fill_color_.c_str() );
        if ( line_col.isValid() )
        {
            painter.setPen( line_col );
        }
        else
        {
            painter.setPen( Qt::NoPen );
        }

        if ( fill_col.isValid() )
        {
            painter.setBrush( fill_col );
        }
        else
        {
            painter.setBrush( Qt::NoBrush );
        }

        painter.drawRect( QRectF( opt.screenX( r.left_ ),
                                  opt.screenY( r.top_ ),
                                  opt.scale( r.width_ ),
                                  opt.scale( r.height_ ) ) );
    }
}

/*-------------------------------------------------------------------*/
void
draw_circles( QPainter & painter,
              const DrawDataHolder::CircleCont & cont )
{
    const Options & opt = Options::instance();

    for ( const auto & c : cont )
    {
        QColor line_col( c.line_color_.c_str() );
        QColor fill_col( c.fill_color_.c_str() );

        if ( line_col.isValid() )
        {
            painter.setPen( line_col );
        }
        else
        {
            painter.setPen( Qt::NoPen );
        }

        if ( fill_col.isValid() )
        {
            painter.setBrush( fill_col );
        }
        else
        {
            painter.setBrush( Qt::NoBrush );
        }

        double r = opt.scale( c.r_ );
        painter.drawEllipse( QPointF( opt.screenX( c.x_ ),
                                      opt.screenY( c.y_ ) ),
                             r, r );
    }
}

}


/*-------------------------------------------------------------------*/
/*!

*/
DrawDataPainter::DrawDataPainter( const MainData & main_data )
    : M_main_data( main_data )
{

}

/*-------------------------------------------------------------------*/
/*!

*/
void
DrawDataPainter::draw( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( ! opt.showDrawData() )
    {
        return;
    }

    MonitorViewData::ConstPtr monitor_view = M_main_data.getCurrentViewData();
    if ( ! monitor_view )
    {
        return;
    }

    const auto it = M_main_data.drawDataHolder().dataMap().find( monitor_view->time() );
    if ( it == M_main_data.drawDataHolder().dataMap().end() )
    {
        return;
    }

    draw_rects( painter, it->second.rects_ );
    draw_circles( painter, it->second.circles_ );
    draw_lines( painter, it->second.lines_ );
    draw_texts( painter, it->second.texts_ );
    draw_points( painter, it->second.points_ );
}
