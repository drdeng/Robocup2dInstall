// -*-c++-*-

/*!
  \file action_sequence_log_parser.h
  \brief parser class to parse action sequence log format Header File.
*/

/*
 *Copyright:

 Copyright (C) Hiroki SHIMORA, Hidehisa AKIYAMA

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

#ifndef SOCCERWINDOW2_ACTION_SEQUENCE_LOG_PARSER_H
#define SOCCERWINDOW2_ACTION_SEQUENCE_LOG_PARSER_H

#include "action_sequence_description.h"

#include <boost/shared_ptr.hpp>

#include <iosfwd>

/*!
  \class ActionSequenceLogParser
  \brief parser class to parse action sequence log format
*/
class ActionSequenceLogParser {

public:
    /*
      \brief construct chain action log data from stream
      \param in input strean in order to parse
      \return parse result data
     */
    static
    ActionSequenceHolder::Ptr parse( std::istream & in );
};

#endif
