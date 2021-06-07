// -*-c++-*-

/*!
  \file game_event_holder.h
  \brief game event data holder Header File.
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

#ifndef SOCCERWINDOW2_GAME_EVENT_HOLDER_H
#define SOCCERWINDOW2_GAME_EVENT_HOLDER_H

#include "game_event.h"

//! streaming debug data repository
class GameEventHolder {
private:

    GameEvent::Cont M_events;

    // noncopyable
    GameEventHolder( const GameEventHolder & );
    GameEventHolder & operator=( const GameEventHolder & );

public:

    GameEventHolder()
      { }

    ~GameEventHolder();

    void clear();

    bool open( const std::string & filepath );
    bool save( const std::string & filepath );

    const GameEvent::Cont & events() const
      {
          return M_events;
      }
};

#endif
