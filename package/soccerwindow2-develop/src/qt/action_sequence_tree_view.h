// -*-c++-*-

/*!
  \file action_sequence_selector.h
  \brief action sequence data selector widget Header File.
*/

/*
 *Copyright:

 Copyright (C) Hiroki SHIMORA

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

#ifndef SOCCERWINDOW2_ACTION_SEQUENCE_TREEVIEW_H
#define SOCCERWINDOW2_ACTION_SEQUENCE_TREEVIEW_H

#include <QTreeWidget>

class ActionSequenceTreeView
    : public QTreeWidget {

    Q_OBJECT

public:

  enum {
    CLICK_COLUMN = 0,
    NO_CLICK_COLUMN = 1,
    ID_COLUMN = 2,
    VALUE_COLUMN = 3,
    LENGTH_COLUMN = 4,
    SEQ_COLUMN = 5,
    DESC_COLUMN = 6,
  };

private:

    QTreeWidgetItem * M_pressed_item;

public:

    ActionSequenceTreeView( QWidget * parent = 0 );

    ~ActionSequenceTreeView();

protected:

    virtual
    void dropEvent( QDropEvent * event );

private slots:

    void slotItemPressed( QTreeWidgetItem * item,
                          int column );

};

#endif
