// -*-c++-*-

/*!
  \file grid_field_evaluation_data.h
  \brief grid field evaluation data holder class Header File.
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

#ifndef SOCCERWINDOW2_GRID_FIELD_EVALUATION_DATA_H
#define SOCCERWINDOW2_GRID_FIELD_EVALUATION_DATA_H

#include "agent_id.h"

#include <rcsc/geom/vector_2d.h>

#include <map>


/*!
  \class GridFieldEvaluationData
  \brief field evaluation data holder
*/
class GridFieldEvaluationData {

public:
    typedef std::multimap< double, const rcsc::Vector2D, std::greater< double > > MapType;

private:
    double M_grid_size;
    MapType M_evaluated_map;

public:

    static double S_grid_size;
    static std::map< std::pair< int, int >, double > S_xy_value_map;

    /*
      \brief constructor
     */
    GridFieldEvaluationData( double grid_size );

    /*
      \brief destructor
     */
    ~GridFieldEvaluationData();

    /*
      \brief register evaluation
      \param evaluation evaluation of point
      \param pos point to evaluate
     */
    void addEvaluationSample( const double evaluation,
                              const rcsc::Vector2D & pos );

    /*
      \brief retrieve field evaluation data
      \return field evaluation data
     */
    const MapType & data() const;

    /*
      \brief retrieve grid size
      \return grid size
     */
    double gridSize() const;
};

#endif
