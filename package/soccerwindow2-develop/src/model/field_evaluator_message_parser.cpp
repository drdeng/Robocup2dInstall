// -*-c++-*-

/*!
  \file field_evaluator_message_parser.cpp
  \brief parser class to parse field evaluation format Source File.
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

#include "field_evaluator_message_parser.h"

#include "grid_field_evaluation_data.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

/*-------------------------------------------------------------------*/
/*!

*/
boost::shared_ptr< GridFieldEvaluationData >
FieldEvaluatorMessageParser::parse( const std::string & messge,
                                    const double grid_size ) const
{
    boost::shared_ptr< GridFieldEvaluationData > data( new GridFieldEvaluationData( grid_size ) );

    std::istringstream in( messge );

    std::string line;

    while ( std::getline( in, line ) )
    {
        double x;
        double y;
        double evaluation;
        int n;

        if ( std::sscanf( line.c_str(), " %lf %lf %lf %n",
                          &x, &y, &evaluation, &n ) != 3
             || n != static_cast< int >( line.length() ) )
        {
            std::cerr << __FILE__ << ": Invalid field evaluator message ["
                      << line << "]" << std::endl;

            return boost::shared_ptr< GridFieldEvaluationData >();
        }

        data->addEvaluationSample( evaluation, rcsc::Vector2D( x, y ) );
    }

    return data;
}
