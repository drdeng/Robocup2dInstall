// -*-c++-*-

/*!
  \file field_painter.h
  \brief field painter class Header File.
*/

/*
 *Copyright:

 Copyright (C) 2004 Hidehisa AKIYAMA

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

#ifndef SOCCERWINDOW2_QT_FIELD_PAINTER_H
#define SOCCERWINDOW2_QT_FIELD_PAINTER_H

#include "painter_interface.h"

#include <rcsc/geom/vector_2d.h>

#include <boost/shared_ptr.hpp>

class QPixmap;

class FieldPainter
    : public PainterInterface {
private:

    int M_center_x;
    int M_center_y;

    boost::shared_ptr< QPixmap > M_pixmap;

    // not used
    FieldPainter( const FieldPainter & );
    const FieldPainter & operator=( const FieldPainter & );
public:

    FieldPainter();

    /*!
      \brief draw soccer field and stationary objects
      \param painter reference to the painter object
    */
    void draw( QPainter & painter );

private:
    /*!
      \brief draw back ground
      \param painter reference to the painter object
    */
    void drawBackGround( QPainter & painter ) const;

    /*!
      \brief draw line type grass
      \param painter reference to the painter object
    */
    void drawLinesGrass( QPainter & painter ) const;

    /*!
      \brief draw checker type grass
      \param painter reference to the painter object
    */
    void drawCheckerGrass( QPainter & painter ) const;

    /*!
      \brief draw field lines
      \param painter reference to the painter object
    */
    void drawLines( QPainter & painter ) const;

    /*!
      \brief draw penalty area
      \param painter reference to the painter object
    */
    void drawPenaltyAreaLines( QPainter & painter ) const;

    /*!
      \brief draw goal area
      \param painter reference to the painter object
    */
    void drawGoalAreaLines( QPainter & painter ) const;
    /*!
      \brief draw goals
      \param painter reference to the painter object
    */
    void drawGoals( QPainter & painter ) const;

    /*!
      \brief draw flags
      \param painter reference to the painter object
    */
    void drawFlags( QPainter & painter ) const;

    /*!
      \brief draw grid lines
      \param painter reference to the painter object
     */
    void drawGrid( QPainter & painter ) const;
};

#endif
