// -*-c++-*-

/*!
  \file team_graphic_painter.h
  \brief team logo image painter class Header File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

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

#ifndef SOCCERWINDOW2_QT4_TEAM_GRAPHIC_PAINTER_H
#define SOCCERWINDOW2_QT4_TEAM_GRAPHIC_PAINTER_H

#include "painter_interface.h"

#include <rcsc/common/team_graphic.h>

#include <QPixmap>

#include <set>

class MainData;

class TeamGraphicPainter
    : public PainterInterface {
private:
    const MainData & M_main_data;

    std::set< rcsc::TeamGraphic::Index > M_team_graphic_left_set;
    QPixmap M_team_graphic_pixmap_left;

    std::set< rcsc::TeamGraphic::Index > M_team_graphic_right_set;
    QPixmap M_team_graphic_pixmap_right;

    // not used
    TeamGraphicPainter();
public:

    //! constructor with data referance
    explicit
    TeamGraphicPainter( const MainData & main_data );

    //! draw soccer field and stationary objects
    void draw( QPainter & painter );

private:

    void copyTeamGraphic( QPixmap & dst_pixmap,
                          std::set< rcsc::TeamGraphic::Index > & index_set,
                          const rcsc::TeamGraphic & team_graphic );
    void copyTeamGraphicXpmTile( QPixmap & dst_pixmap,
                                 const rcsc::TeamGraphic::Index & index,
                                 const rcsc::TeamGraphic::XpmTile & tile );
    void copyTeamGraphicXpmTile( QPixmap & dst_pixmap,
                                 const int x,
                                 const int y,
                                 const char * const * xpm );

};

#endif
