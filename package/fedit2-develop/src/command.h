// -*-c++-*-

/*!
  \file command.h
  \brief edit command class Header File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

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

#ifndef FEDIT2_COMMAND_H
#define FEDIT2_COMMAND_H

#include <QUndoCommand>
#include <QPointF>

class EditScene;

class QGraphicsItem;

class MoveCommand
    : public QUndoCommand {
private:

    QPointF M_old_pos;
    QPointF M_new_pos;

public:

    MoveCommand( const QPointF & old_pos,
                 const QPointF & new_pos,
                 QUndoCommand * parent = 0 );
    ~MoveCommand();

    int id() const
      {
          return 1;
      }

    void undo();
    void redo();
//     bool mergeWith( const QUndoCommand * command );

private:

    void updateText();

};

#endif
