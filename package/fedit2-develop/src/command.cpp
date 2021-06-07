// -*-c++-*-

/*!
  \file command.cpp
  \brief edit command class Source File.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "command.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

 */
MoveCommand::MoveCommand( const QPointF & old_pos,
                          const QPointF & new_pos,
                          QUndoCommand * parent )
    : QUndoCommand( parent )
    , M_old_pos( old_pos )
    , M_new_pos( new_pos )
{

}


/*-------------------------------------------------------------------*/
/*!

 */
MoveCommand::~MoveCommand()
{
    std::cerr << "delete MoveCommand"
              <<"\n  old_pos=(" << M_old_pos.x() << ", " << M_old_pos.y() << ")"
              <<"\n  new_pos=(" << M_new_pos.x() << ", " << M_new_pos.y() << ")"
              << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MoveCommand::undo()
{
//     M_item->setPos( M_old_pos );
//     M_scene->updateItemPositions();
    std::cerr << "undo "
              << " new=" << M_new_pos.x() << ',' << M_new_pos.y()
              << " ->  old=" << M_old_pos.x() << ',' << M_old_pos.y()
              << std::endl;

    updateText();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MoveCommand::redo()
{
//     M_item->setPos( M_new_pos );
//     M_scene->updateItemPositions();
    std::cerr << "redo "
              << " new=" << M_new_pos.x() << ',' << M_new_pos.y()
              << " ->  old=" << M_old_pos.x() << ',' << M_old_pos.y()
              << std::endl;

    updateText();
 }

/*-------------------------------------------------------------------*/
/*!

 */
// bool
// MoveCommand::mergeWith( const QUndoCommand * command )
// {
//     const MoveCommand * merged_command = reinterpret_cast< const MoveCommand * >( command );

//     if ( M_item != merged_command->M_item )
//     {
//         return false;
//     }

//     M_new_pos = M_item->pos();
//     updateText();
//     return true;
// }

/*-------------------------------------------------------------------*/
/*!

 */
void
MoveCommand::updateText()
{
//     if ( qgraphicsitem_cast< BallItem * >( M_item ) )
//     {
//         this->setText( QObject::tr( "Move Ball (%1, %2)" )
//                        .arg( M_new_pos.x() )
//                        .arg( M_new_pos.y() ) );
//     }
//     else if ( PlayerItem * p = qgraphicsitem_cast< PlayerItem * >( M_item ) )
//     {
//         this->setText( QObject::tr( "Move Player %1 (%2, %3)" )
//                        .arg( p->unum() )
//                        .arg( M_new_pos.x() )
//                        .arg( M_new_pos.y() ) );
//     }
//     else
    {
        this->setText( QObject::tr( "Move ??? (%1, %2)" )
                       .arg( M_new_pos.x() )
                       .arg( M_new_pos.y() ) );
    }
}
