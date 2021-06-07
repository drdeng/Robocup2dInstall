// -*-c++-*-

/*!
  \file grid_field_evaluation_data.cpp
  \brief grid field evaluation data holder class Source File.
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

#include "grid_field_evaluation_data.h"

#include <utility>

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

*/
double GridFieldEvaluationData::S_grid_size = 1.0;
std::map< std::pair< int, int >, double > GridFieldEvaluationData::S_xy_value_map;

GridFieldEvaluationData::GridFieldEvaluationData( double grid_size )
    : M_grid_size( grid_size ),
      M_evaluated_map()
{

}

/*-------------------------------------------------------------------*/
/*!

*/
GridFieldEvaluationData::~GridFieldEvaluationData()
{

}

/*-------------------------------------------------------------------*/
/*!

*/
void
GridFieldEvaluationData::addEvaluationSample( double evaluation,
                                              const rcsc::Vector2D & pos )
{
    M_evaluated_map.insert( std::make_pair( evaluation, pos ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
const GridFieldEvaluationData::MapType &
GridFieldEvaluationData::data() const
{
    return M_evaluated_map;
}

/*-------------------------------------------------------------------*/
/*!

*/
double
GridFieldEvaluationData::gridSize() const
{
    return M_grid_size;
}
