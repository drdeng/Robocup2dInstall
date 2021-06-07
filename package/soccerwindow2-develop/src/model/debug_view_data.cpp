// -*-c++-*-

/*!
  \file debug_view_data.cpp
  \brief debug view data to be drawn Source File.
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

#include "debug_view_data.h"

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugViewData::clear()
{
    M_ball.reset();
    M_self.reset();
    M_teammates.clear();
    M_opponents.clear();
    M_unknown_teammates.clear();
    M_unknown_opponents.clear();
    M_unknown_players.clear();
    M_lines.clear();
    M_target_teammate = 0;
    M_target_point.reset();
    M_message.erase();
    M_say_message.erase();
}

/*-------------------------------------------------------------------*/
/*!

*/
std::ostream &
DebugViewData::print( std::ostream & os,
                      const rcsc::GameTime & time,
                      bool print_debug_line ) const
{
    if ( print_debug_line )
    {
        os << "%% step " << time.cycle() << ',' << time.stopped() << '\n';
        os << "%% debug [";
    }

    os << "((debug (format-version 5)) (time "
       << time.cycle() << ',' << time.stopped() << ')';

    if ( M_self )
    {
        os << " (s " << M_self->side_ << ' '
           << M_self->unum_ << ' '
           << M_self->ptype_ << ' '
           << static_cast< double >( M_self->x_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << static_cast< double >( M_self->y_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << static_cast< double >( M_self->vx_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << static_cast< double >( M_self->vy_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << M_self->body_ << ' '
           << M_self->neck_ << " (c \""
           << M_self->comment_ << "\"))";
    }

    if ( M_ball )
    {
        os << " (b "
           << static_cast< double >( M_ball->x_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << static_cast< double >( M_ball->y_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << static_cast< double >( M_ball->vx_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << static_cast< double >( M_ball->vy_ ) / rcsc::rcg::SHOWINFO_SCALE2
           << " (\"" << M_ball->comment_ << "\"))";
    }

    // players

    for ( PlayerCont::const_iterator it = M_teammates.begin();
          it != M_teammates.end();
          ++it )
    {
        os << " (t "
           << (*it)->unum_ << ' '
           << (*it)->ptype_ << ' '
           << static_cast< double >( (*it)->x_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << static_cast< double >( (*it)->y_ ) / rcsc::rcg::SHOWINFO_SCALE2;
        if ( (*it)->body_ != -360 )
        {
            os << " (bd " << (*it)->body_ << ')';
        }
        if ( (*it)->pointto_ != -360 )
        {
            os << " (pt " << (*it)->pointto_ << ')';
        }
        os << " (c \"" << (*it)->comment_ << "\"))";
    }

    for ( PlayerCont::const_iterator it = M_unknown_teammates.begin();
          it != M_unknown_teammates.end();
          ++it )
    {
        os << " (ut "
           << static_cast< double >( (*it)->x_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << static_cast< double >( (*it)->y_ ) / rcsc::rcg::SHOWINFO_SCALE2;
        os << " (c \"" << (*it)->comment_ << "\"))";
    }

    for ( PlayerCont::const_iterator it = M_opponents.begin();
          it != M_opponents.end();
          ++it )
    {
        os << " (o "
           << (*it)->unum_ << ' '
           << (*it)->ptype_ << ' '
           << static_cast< double >( (*it)->x_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << static_cast< double >( (*it)->y_ ) / rcsc::rcg::SHOWINFO_SCALE2;
        if ( (*it)->body_ != -360 )
        {
            os << " (bd " << (*it)->body_ << ')';
        }
        if ( (*it)->pointto_ != -360 )
        {
            os << " (pt " << (*it)->pointto_ << ')';
        }
        os << " (c \"" << (*it)->comment_ << "\"))";
    }

    for ( PlayerCont::const_iterator it = M_unknown_opponents.begin();
          it != M_unknown_opponents.end();
          ++it )
    {
        os << " (uo "
           << static_cast< double >( (*it)->x_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << static_cast< double >( (*it)->y_ ) / rcsc::rcg::SHOWINFO_SCALE2;
        os << " (c \"" << (*it)->comment_ << "\"))";
    }

    for ( PlayerCont::const_iterator it = M_unknown_players.begin();
          it != M_unknown_players.end();
          ++it )
    {
        os << " (u "
           << static_cast< double >( (*it)->x_ ) / rcsc::rcg::SHOWINFO_SCALE2 << ' '
           << static_cast< double >( (*it)->y_ ) / rcsc::rcg::SHOWINFO_SCALE2;
        os << " (c \"" << (*it)->comment_ << "\"))";
    }

    // target

    if ( M_target_teammate != 0 )
    {
        os << " (target-teammate " << M_target_teammate << ')';
    }

    if ( M_target_point )
    {
        os << " (target-point "
           << M_target_point->x << ' '
           << M_target_point->y << ')';
    }

    // say message
    if ( ! M_say_message.empty() )
    {
        os << " (say \"" << M_say_message << "\")";
    }

    if ( ! M_hear_messages.empty() )
    {
        os << " (hear";
        for ( std::map< int, std::string >::const_iterator it = M_hear_messages.begin();
              it != M_hear_messages.end();
              ++it )
        {
            os << " (" << it->first << " \"" << it->second << "\")";
        }
        os << ')';
    }

    // message

    if ( ! M_message.empty() )
    {
        os << " (message \"" << M_message << "\")";
    }

    // shapes

    for ( std::list< LineT >::const_iterator it = M_lines.begin();
          it != M_lines.end();
          ++it )
    {
        os << " (line "
           << it->x1_ << ' ' << it->y1_ << ' '
           << it->x2_ << ' ' << it->y2_ << ')';
    }

    for ( std::list< TriangleT >::const_iterator it = M_triangles.begin();
          it != M_triangles.end();
          ++it )
    {
        os << " (tri "
           << it->x1_ << ' ' << it->y1_ << ' '
           << it->x2_ << ' ' << it->y2_ << ' '
           << it->x3_ << ' ' << it->y3_ << ')';
    }

    for ( std::list< RectT >::const_iterator it = M_rectangles.begin();
          it != M_rectangles.end();
          ++it )
    {
        os << " (rect "
           << it->left_x_ << ' ' << it->top_y_ << ' '
           << it->right_x_ << ' ' << it->bottom_y_ << ')';
    }

    for ( std::list< CircleT >::const_iterator it = M_circles.begin();
          it != M_circles.end();
          ++it )
    {
        os << " (circle "
           << it->center_x_ << ' ' << it->center_y_ << ' '
           << it->radius_ << ')';
    }


    os << ")";
    if ( print_debug_line )
    {
        os << "]\n";
    }
    return os;
}
