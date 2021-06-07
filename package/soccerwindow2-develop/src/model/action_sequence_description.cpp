// -*-c++-*-

/*!
  \file action_sequence_description.cpp
  \brief definition of action sequence data Source File.
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

#include "action_sequence_description.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

*/
std::ostream &
ActionSequenceDescription::print( std::ostream & os ) const
{
    os << id() << ": evaluation=" << value() << '\n';

    int i = 0;
    for ( std::vector< ActionDescription >::const_iterator it = actions().begin(), end = actions().end();
          it != end;
          ++it, ++i )
    {
        it->print( os, i );
    }

    return os;
}

/*-------------------------------------------------------------------*/
/*!

*/
std::ostream &
ActionDescription::print( std::ostream & os,
                          int count ) const
{
    os << "__ " << count << ": ";

    switch ( category() ) {
    case PASS:
        {
            os << "pass (" << name() << "[" << number() << "])"
               << " t=" << durationTime()
               << " from[" << fromUnum() << "]"
               << "(" << fromPos().x << " " << fromPos().y << ")"
               << "-to[" << toUnum() << "]"
               << "(" << toPos().x << " " << toPos().y << "), "
               << "safe=" << safeLevel() << '\n';
        }
        break;

    case DRIBBLE:
        {
            os << "dribble (" << name() << "[" << number() << "])"
               << " t=" << durationTime()
               << " from[" << fromUnum() << "]"
               << "(" << fromPos().x << " " << fromPos().y << ")"
               << "-to"
               << "(" << toPos().x << " " << toPos().y << "), "
               << "safe=" << safeLevel() << '\n';
        }
        break;

    case SHOOT:
        {
            os << "shoot (" << name() << ")"
               << " t=" << durationTime()
               << " from[" << fromUnum() << "]"
               << "(" << fromPos().x << " " << fromPos().y << ")"
               << "-to"
               << "(" << toPos().x << " " << toPos().y << "), "
               << "safe=" << safeLevel() << '\n';
        }
        break;

    case HOLD:
        {
            os << "hold (" << name() << "[" << number() << "])"
               << " t=" << durationTime()
               << " from[" << fromUnum() << "]"
               << "(" << fromPos().x << " " << fromPos().y << "), "
               << "safe=" << safeLevel() << '\n';
        }
        break;

    case MOVE:
        {
            os << "move (" << name() << ")"
               << " t=" << durationTime()
               << " from[" << fromUnum() << "]"
               << "(" << fromPos().x << " " << fromPos().y << ")"
               << "-to[" << toUnum() << "]"
               << "(" << toPos().x << " " << toPos().y << "),"
               << "safe=" << safeLevel() << '\n';
        }
        break;

    default:
        {
            os << "???? (" << name() << "[" << number() << "])"
               << " t=" << durationTime()
               << " from[" << fromUnum() << "]"
               << "(" << fromPos().x << " " << fromPos().y << ")"
               << "-to[" << toUnum() << "]"
               << "(" << toPos().x << " " << toPos().y << "),"
               << "safe=" << safeLevel() << '\n';
        }
        break;
    }

    return os;
}
