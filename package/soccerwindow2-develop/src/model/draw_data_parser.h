// -*-c++-*-

/*!
  \file draw_data_parser.h
  \brief draw data parser class Header File.
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

#ifndef SOCCERWINDOW2_DRAW_DATA_PARSER_H
#define SOCCERWINDOW2_DRAW_DATA_PARSER_H

#include <rcsc/game_time.h>

class DrawDataHolder;

class DrawDataParser {
private:

    DrawDataHolder & M_holder;

public:

    explicit
    DrawDataParser( DrawDataHolder & holder );

    bool parse( const char * buf );

private:

    bool parseText( const rcsc::GameTime & time,
                    const char * buf );
    bool parsePoint( const rcsc::GameTime & time,
                     const char * buf );
    bool parseLine( const rcsc::GameTime & time,
                    const char * buf );
    bool parseRect( const rcsc::GameTime & time,
                    const char * buf );
    bool parseCircle( const rcsc::GameTime & time,
                      const char * buf );
};

#endif
