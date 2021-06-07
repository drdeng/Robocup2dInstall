// -*-c++-*-

/*!
  \file ball_painter.h
  \brief ball painter class Header File.
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

#ifndef SOCCERWINDOW2_QT_BALL_PAINTER_H
#define SOCCERWINDOW2_QT_BALL_PAINTER_H

#include "painter_interface.h"

class QPainter;

class MainData;

class BallPainter
    : public PainterInterface {
private:
    const MainData & M_main_data;

    // not used
    BallPainter();
    BallPainter( const BallPainter & );
    const BallPainter & operator=( const BallPainter & );
public:
    //! constructor with data referance
    explicit
    BallPainter( const MainData & main_data )
        : M_main_data( main_data )
      { }

    //! paint ball object to the canvas
    void draw( QPainter & painter );

private:

    void drawFutureState( QPainter & painter ) const;

};

#endif
