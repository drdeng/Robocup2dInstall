// -*-c++-*-

/*!
  \file debug_painter.h
  \brief debug info painter class Header File.
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

#ifndef SOCCERWINDOW2_QT_DEBUG_PAINTER_H
#define SOCCERWINDOW2_QT_DEBUG_PAINTER_H

#include "painter_interface.h"
#include "debug_view_data.h"

#include <rcsc/types.h>

class MainData;
class MonitorViewData;

class DebugPainter
    : public PainterInterface {
private:
    const MainData & M_main_data;

public:

    explicit
    DebugPainter( const MainData & main_data )
        : M_main_data( main_data )
      { }

    void draw( QPainter & painter );

private:

    void drawSelf( QPainter & painter,
                   const rcsc::SideID self_side,
                   const DebugViewData & view ) const;

    void drawBall( QPainter & painter,
                   const rcsc::SideID self_side,
                   const DebugViewData & view ) const;

    void drawPlayers( QPainter &painter,
                      const rcsc::SideID self_side,
                      const DebugViewData::PlayerCont & players,
                      const QBrush & body_brush ) const;

    void drawTarget( QPainter & painter,
                     const rcsc::SideID self_side,
                     const DebugViewData & view ) const;

    void drawLines( QPainter & painter,
                    const rcsc::SideID self_side,
                    const DebugViewData & view ) const;

    void drawTriangles( QPainter & painter,
                        const rcsc::SideID self_side,
                        const DebugViewData & view ) const;

    void drawRectangles( QPainter & painter,
                         const rcsc::SideID self_side,
                         const DebugViewData & view ) const;

    void drawCircles( QPainter & painter,
                      const rcsc::SideID self_side,
                      const DebugViewData & view ) const;

    void drawMessage( QPainter & painter,
                      const DebugViewData & view ) const;

};

#endif
