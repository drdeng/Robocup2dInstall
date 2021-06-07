// -*-c++-*-

/*!
  \file field_evaluator_message_parser.h
  \brief parser class to parse field evaluation format Header File.
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

#ifndef SOCCERWINDOW2_FIELD_EVALUATOR_MESSAGE_PARSER_H
#define SOCCERWINDOW2_FIELD_EVALUATOR_MESSAGE_PARSER_H

#include <boost/shared_ptr.hpp>

#include <string>

class GridFieldEvaluationData;


/*!
  \class FieldEvaluatorMessageParser
  \brief parser class to parse field evaluation format
*/
class FieldEvaluatorMessageParser {

public:
    /*
      \brief construct grid field evaluation data from string
      \param messge string in order to parse
      \param grid_size size of grid
      \return parse result data
     */
    boost::shared_ptr< GridFieldEvaluationData > parse( const std::string & messge,
                                                        const double grid_size ) const;
};

#endif
