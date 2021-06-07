// -*-c++-*-

/*!
  \file debug_log_data.cpp
  \brief debug log data Source File.
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

#include "debug_log_data.h"

#include <iostream>
#include <cstdio>
#include <cstring>

/*-------------------------------------------------------------------*/
/*!
  \param buf debug message string. but original input string contains
  <Time> info on top of the string.
*/
bool
DebugLogData::parse( const char * buf )
{
    // <Level> <Type> <Content>

    boost::int32_t level;
    char type;
    int n_read = 0;

    if ( std::sscanf( buf, " %d %c %n",
                      &level, &type,
                      &n_read ) != 2
         || n_read == 0 )
    {
        std::cerr << __FILE__ << ": (parse) "
                  << "Illegal format [" << buf
                  << "]  read level =" << level
                  << "  read type = " << type
                  << "  read length = " << n_read
                  << std::endl;
        return false;
    }

    buf += n_read;

    switch ( type ) {
    case 'M':
        M_text_cont.push_back( TextT( level ) );
        M_text_cont.back().msg_ = buf;
        M_text_cont.back().msg_ += '\n';
        return true;
    case 'l':
        return addLine( level, buf );
    case 'a':
        return addArc( level, buf );
    case 'c':
        return addCircle( level, buf, false );
    case 'C':
        return addCircle( level, buf, true );
    case 'r':
        return addRect( level, buf, false );
    case 'R':
        return addRect( level, buf, true );
    case 's':
        return addSector( level, buf, false );
    case 'S':
        return addSector( level, buf, true );
    case 't':
        return addTriangle( level, buf, false );
    case 'T':
        return addTriangle( level, buf, true );
    case 'p':
        return addPoint( level, buf );
    case 'm':
        return addMessage( level, buf );
    default:
        break;
    }

    std::cerr << __FILE__ << ": (parse) Unsupported type message ["
              << buf << ']' << std::endl;
    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogData::addPoint( const boost::int32_t level,
                        const char * buf )
{
    // <x> <y>
    PointT point;
    int n_read = 0;
    if ( std::sscanf( buf, " %lf %lf %n",
                      &point.x_, &point.y_,
                      &n_read ) != 2 )
    {
        std::cerr << __FILE__ << ": (addPoint) Illegal message ["
                  << buf << ']' << std::endl;
        return false;
    }

    buf += n_read;

    if ( std::strlen( buf ) > 0 )
    {
        point.color_ = buf;
    }

    point.level_ = level;

    M_point_cont.push_back( point );
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogData::addLine( const boost::int32_t level,
                       const char * buf )
{
    // <x1> <y1> <x2> <y2>
    LineT line;
    int n_read = 0;
    if ( std::sscanf( buf, " %lf %lf %lf %lf %n",
                      &line.x1_, &line.y1_,
                      &line.x2_, &line.y2_,
                      &n_read ) != 4 )
    {
        std::cerr << __FILE__ << ": (addLine) Illegal message ["
                  << buf << ']' << std::endl;
        return false;
    }

    buf += n_read;

    if ( std::strlen( buf ) > 0 )
    {
        line.color_ = buf;
    }

    line.level_ = level;

    M_line_cont.push_back( line );
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogData::addArc( const boost::int32_t level,
                       const char * buf )
{
    // <x> <y> <r> <star_angle> <span_angle>
    ArcT arc;
    int n_read = 0;
    if ( std::sscanf( buf, " %lf %lf %lf %lf %lf %n",
                      &arc.x_, &arc.y_,
                      &arc.r_,
                      &arc.start_angle_, &arc.span_angle_,
                      &n_read ) != 5 )
    {
        std::cerr << __FILE__ << ": (addArc) Illegal message ["
                  << buf << ']' << std::endl;
        return false;
    }

    buf += n_read;

    if ( std::strlen( buf ) > 0 )
    {
        arc.color_ = buf;
    }

    arc.level_ = level;

    M_arc_cont.push_back( arc );
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogData::addCircle( const boost::int32_t level,
                         const char * buf,
                         const bool fill )
{
    // <x> <y> <r>
    CircleT c;
    int n_read = 0;
    if ( std::sscanf( buf, " %lf %lf %lf %n",
                      &c.x_, &c.y_, &c.r_,
                      &n_read ) != 3 )
    {
        std::cerr << __FILE__ << ": (addCircle) Illegal message ["
                  << buf << ']' << std::endl;
        return false;
    }

    buf += n_read;

    if ( std::strlen( buf ) > 0 )
    {
        c.color_ = buf;
    }

    c.level_ = level;

    if ( fill )
    {
        M_filled_circle_cont.push_back( c );
    }
    else
    {
        M_circle_cont.push_back( c );
    }
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogData::addTriangle( const boost::int32_t level,
                           const char * buf,
                           const bool fill )
{
    // <x1> <y1> <x2> <y2> <x3> <y3>
    TriangleT t;
    int n_read = 0;
    if ( std::sscanf( buf, " %lf %lf %lf %lf %lf %lf %n",
                      &t.x1_, &t.y1_,
                      &t.x2_, &t.y2_,
                      &t.x3_, &t.y3_,
                      &n_read ) != 6 )
    {
        std::cerr << __FILE__ << ": (addTriangle) Illegal message ["
                  << buf << ']' << std::endl;
        return false;
    }

    buf += n_read;

    if ( std::strlen( buf ) > 0 )
    {
        t.color_ = buf;
    }

    t.level_ = level;

    if ( fill )
    {
        M_filled_triangle_cont.push_back( t );
    }
    else
    {
        M_triangle_cont.push_back( t );
    }
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogData::addRect( const boost::int32_t level,
                       const char * buf,
                       const bool fill )
{
    // <left> <top> <width> <height> <color name>
    RectT r;
    int n_read = 0;
    if ( std::sscanf( buf, " %lf %lf %lf %lf %n",
                      &r.left_, &r.top_,
                      &r.width_, &r.height_,
                      &n_read ) != 4 )
    {
        std::cerr << __FILE__ << ": (addRect) Illegal message ["
                  << buf << ']' << std::endl;
        return false;
    }

    buf += n_read;

    if ( std::strlen( buf ) > 0 )
    {
        r.color_ = buf;
    }

    r.level_ = level;

    if ( fill )
    {
        M_filled_rect_cont.push_back( r );
    }
    else
    {
        M_rect_cont.push_back( r );
    }
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogData::addSector( const boost::int32_t level,
                         const char * buf,
                         const bool fill )
{
    // <x> <y> <min_r> <max_r> <star_angle> <span_angle>
    SectorT sector;
    int n_read = 0;
    if ( std::sscanf( buf, " %lf %lf %lf %lf %lf %lf %n",
                      &sector.x_, &sector.y_,
                      &sector.min_r_, &sector.max_r_,
                      &sector.start_angle_, &sector.span_angle_,
                      &n_read ) != 6 )
    {
        std::cerr << __FILE__ << ": (addSector) Illegal message ["
                  << buf << ']' << std::endl;
        return false;
    }
    buf += n_read;

    if ( std::strlen( buf ) > 0 )
    {
        sector.color_ = buf;
    }

    sector.level_ = level;

    if ( fill )
    {
        M_filled_sector_cont.push_back( sector );
    }
    else
    {
        M_sector_cont.push_back( sector );
    }
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogData::addMessage( const boost::int32_t level,
                          const char * buf )
{
    // <x> <y>[ (c <color>)] <str>
    MessageT mes;
    int n_read = 0;
    if ( std::sscanf( buf, " %lf %lf %n",
                      &mes.x_, &mes.y_,
                      &n_read ) != 2
         || n_read == 0 )
    {
        std::cerr << __FILE__ << ": (addMessage) Illegal position ["
                  << buf << ']' << std::endl;
        return false;
    }

    buf += n_read;

    if ( ! std::strncmp( buf, "(c ", 3 ) )
    {
        char col[32];
        if ( std::sscanf( buf + 3, " %31[^)]) %n",
                          col, &n_read ) != 1 )
        {
            std::cerr << __FILE__ << ": (addMessage) Illegal color ["
                      << buf << ']' << std::endl;
            return false;
        }

        mes.color_ = col;

        buf += 3 + n_read;
    }

    mes.message_ = buf;
    mes.level_ = level;

    M_message_cont.push_back( mes );
    return true;
}
