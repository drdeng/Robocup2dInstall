// -*-c++-*-

/*!
  \file field_evaluation_painter.h
  \brief field evaluation painter class Header File.
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

#ifndef SOCCERWINDOW2_QT_FIELD_EVALUATION_PAINTER_H
#define SOCCERWINDOW2_QT_FIELD_EVALUATION_PAINTER_H

#include "painter_interface.h"

namespace rcsc {
class GameTime;
}

class AgentID;
class MainData;

class QPainter;
class QBrush;


/*!
  \class FieldEvaluationPainter
  \brief field evaluation painter class to paint point cloud evaluation data
*/
class FieldEvaluationPainter
    : public PainterInterface {

private:
    //! model
    const MainData & M_main_data;

private:
    /*
      \brief private constructor without implementation to inhibit copying
     */
    FieldEvaluationPainter( const FieldEvaluationPainter & );

    /*
      \brief private operator= without implementation to inhibit copying
     */
    FieldEvaluationPainter & operator= ( const FieldEvaluationPainter & );

public:
    /*
      \brief constructor
     */
    FieldEvaluationPainter( const MainData & main_data );

    /*
      \brief paint point cloud evaluation data
     */
    void draw( QPainter & painter );

private:
    void paintGrid( QPainter & painter,
                    const rcsc::GameTime & time,
                    const AgentID & player );

};

#endif
