// -*-c++-*-

/*!
  \file draw_data_parser.cpp
  \brief draw data parser class Source File.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "draw_data_parser.h"
#include "draw_data_holder.h"

#include <iostream>
#include <cstring>
// #include <cctype>
// #include <climits>
// #include <cmath>
// #include <cstdlib>
#include <cstdio>

/*-------------------------------------------------------------------*/
/*!

*/
DrawDataParser::DrawDataParser( DrawDataHolder & holder )
    : M_holder( holder )
{

}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DrawDataParser::parse( const char * buf )
{
    /*
      MSG := TIME DATA
      TIME := long,long
      DATA := TYPE INFO
      TYPE := t|p|l|r|c
      INFO := string
      | x y color
      | x1 y1 x2 y2 color [fill]
      | left top width height color [fill]
      | x y r color [fill]
     */

    long cycle;
    long stopped;
    char type;
    int n_read = 0;

    if ( std::sscanf( buf, " %ld %ld %c %n", &cycle, &stopped, &type, &n_read ) != 3
         || n_read == 0 )
    {
        return false;
    }
    buf += n_read;

    const rcsc::GameTime time( cycle, stopped );

    switch ( type ) {
    case 't':
        return parseText( time, buf );
        break;
    case 'p':
        return parsePoint( time, buf );
        break;
    case 'l':
        return parseLine( time, buf );
        break;
    case 'r':
        return parseRect( time, buf );
        break;
    case 'c':
        return parseCircle( time, buf );
        break;
    default:
        break;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DrawDataParser::parseText( const rcsc::GameTime & time,
                           const char * buf )
{
    double x, y;
    char color[32];
    int n_read = 0;
    if ( std::sscanf( buf, " %lf %lf %s %n ", &x, &y, color, &n_read ) != 3 )
    {
        std::cerr << "(DrawDataParser::parseText) illegal line [" << buf << "]" << std::endl;
        return false;
    }

    M_holder.addText( time, DrawDataHolder::TextT( x, y, color, std::string( buf + n_read ) ) );
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DrawDataParser::parsePoint( const rcsc::GameTime & time,
                            const char * buf )
{
    double x, y;
    char color[32];
    if ( std::sscanf( buf, " %lf %lf %s ", &x, &y, color ) != 3 )
    {
        std::cerr << "(DrawDataParser::parsePoint) illegal data [" << buf << "]" << std::endl;
        return false;
    }

    M_holder.addPoint( time, DrawDataHolder::PointT( x, y, color ) );

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DrawDataParser::parseLine( const rcsc::GameTime & time,
                           const char * buf )
{
    double x1, y1, x2, y2;
    char color[32];
    if ( std::sscanf( buf, " %lf %lf %lf %lf %s ", &x1, &y1, &x2, &y2, color ) != 5 )
    {
        std::cerr << "(DrawDataParser::parseLine) illegal data [" << buf << "]" << std::endl;
        return false;
    }

    M_holder.addLine( time, DrawDataHolder::LineT( x1, y1, x2, y2, color ) );

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DrawDataParser::parseRect( const rcsc::GameTime & time,
                           const char * buf )
{
    double top, left, width, height;
    char line_color[32];
    char fill_color[32];

    int n = std::sscanf( buf, " %lf %lf %lf %lf %s %s ", &top, &left, &width, &height, line_color, fill_color );

    if ( n == 6 )
    {
        // filled rectangle
        M_holder.addRect( time, DrawDataHolder::RectT( top, left, width, height, line_color, fill_color ) );
    }
    else if ( n == 5 )
    {
        // only lines
        M_holder.addRect( time, DrawDataHolder::RectT( top, left, width, height, line_color, "" ) );
    }
    else
    {
        std::cerr << "(DrawDataParser::parseRect) illegal data [" << buf << "]" << std::endl;
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DrawDataParser::parseCircle( const rcsc::GameTime & time,
                             const char * buf )
{
    double x, y, r;
    char line_color[32];
    char fill_color[32];

    int n = std::sscanf( buf, " %lf %lf %lf %s %s ", &x, &y, &r, line_color, fill_color );

    if ( n == 5 )
    {
        // filled circle
        M_holder.addCircle( time, DrawDataHolder::CircleT( x, y, r, line_color, fill_color ) );
    }
    else if ( n == 4 )
    {
        // only lines
        M_holder.addCircle( time, DrawDataHolder::CircleT( x, y, r, line_color, "" ) );
    }
    else
    {
        std::cerr << "(DrawDataParser::parseRect) illegal data [" << buf << "]" << std::endl;
        return false;
    }

    return true;
}
