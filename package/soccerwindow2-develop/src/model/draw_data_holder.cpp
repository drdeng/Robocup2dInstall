// -*-c++-*-

/*!
  \file draw_data_holder.cpp
  \brief draw data holder class Source File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa Akiyama

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

#include "draw_data_holder.h"

#include "draw_data_parser.h"

#include <iostream>
#include <fstream>

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

*/
DrawDataHolder::DrawDataHolder()
{

}

/*-------------------------------------------------------------------*/
/*!

*/
DrawDataHolder::~DrawDataHolder()
{

}

/*-------------------------------------------------------------------*/
/*!

*/
void
DrawDataHolder::clear()
{
    M_data_map.clear();
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DrawDataHolder::open( const std::string & filepath )
{
    std::ifstream fin( filepath.c_str() );

    if ( ! fin )
    {
        std::cerr << "(DrawDataHolder::open) could not open the file [" << filepath << "]" << std::endl;
        return false;
    }

    clear();

    DrawDataParser parser( *this );

    std::string line_buf;
    int n_line = 0;
    while ( std::getline( fin, line_buf ) )
    {
        ++n_line;

        if ( line_buf.empty()
             || line_buf[0] == '#' )
        {
            continue;
        }

        if ( ! parser.parse( line_buf.c_str() ) )
        {
            std::cerr << "(DrawDataHolder::open) Error at line " << n_line << std::endl;
            continue;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DrawDataHolder::addText( const rcsc::GameTime & time,
                         const TextT & text )
{
    M_data_map[time].texts_.push_back( text );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DrawDataHolder::addPoint( const rcsc::GameTime & time,
                          const PointT & point )
{
    M_data_map[time].points_.push_back( point );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DrawDataHolder::addLine( const rcsc::GameTime & time,
                         const LineT & line )
{
    M_data_map[time].lines_.push_back( line );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DrawDataHolder::addRect( const rcsc::GameTime & time,
                         const RectT & rect )
{
    M_data_map[time].rects_.push_back( rect );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DrawDataHolder::addCircle( const rcsc::GameTime & time,
                           const CircleT & circle )
{
    M_data_map[time].circles_.push_back( circle );
}
