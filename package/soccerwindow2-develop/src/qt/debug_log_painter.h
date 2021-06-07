// -*-c++-*-

/*!
  \file debug_log_painter.h
  \brief debug log info painter class Header File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa Akiyama

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

#ifndef SOCCERWINDOW2_QT4_DEBUG_LOG_PAINTER_H
#define SOCCERWINDOW2_QT4_DEBUG_LOG_PAINTER_H

#include "painter_interface.h"

#include <rcsc/types.h>

class MainData;
class DebugLogData;

class DebugLogPainter
    : public PainterInterface {
private:
    const MainData & M_main_data;

public:

    explicit
    DebugLogPainter( const MainData & main_data )
        : M_main_data( main_data )
      { }

    void draw( QPainter & painter );

private:


    void drawPoints( QPainter & painter,
                     const rcsc::SideID player_side,
                     const DebugLogData & log_data ) const;

    void drawLines( QPainter & painter,
                    const rcsc::SideID player_side,
                    const DebugLogData & log_data ) const;

    void drawArcs( QPainter & painter,
                   const rcsc::SideID player_side,
                   const DebugLogData & log_data ) const;

    void drawCircles( QPainter & painter,
                      const rcsc::SideID player_side,
                      const DebugLogData & log_data ) const;

    void drawTriangles( QPainter & painter,
                       const rcsc::SideID player_side,
                       const DebugLogData & log_data ) const;

    void drawRectangles( QPainter & painter,
                         const rcsc::SideID player_side,
                         const DebugLogData & log_data ) const;

    void drawSectors( QPainter & painter,
                      const rcsc::SideID player_side,
                      const DebugLogData & log_data ) const;


    void drawMessages( QPainter & painter,
                       const rcsc::SideID player_side,
                       const DebugLogData & log_data ) const;

    void drawActionSequence( QPainter & painter,
                             const rcsc::SideID player_side ) const;

};

#endif
