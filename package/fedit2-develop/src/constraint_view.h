// -*-c++-*-

/*!
  \file constraint_view.h
  \brief constraint view class Header File.
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

#ifndef FEDIT2_CONSTRAINT_VIEW_H
#define FEDIT2_CONSTRAINT_VIEW_H

#include <QTableWidget>

#include <boost/weak_ptr.hpp>

class ConstraintDelegate;
class EditData;

class ConstraintView
    : public QTableWidget {

    Q_OBJECT

private:

    boost::weak_ptr< EditData > M_edit_data;

    ConstraintDelegate * M_origin_delegate;
    ConstraintDelegate * M_terminal_delegate;

public:

    ConstraintView( QWidget * parent = 0 );
    ~ConstraintView();

    void setData( boost::shared_ptr< EditData > ptr )
      {
          M_edit_data = ptr;
      }

    void updateData();

protected:

    void contextMenuEvent( QContextMenuEvent * event );

private slots:

    void setCurrentData( QTableWidgetItem * current );
    void menuDeleteConstraint();
    void changeConstraintIndex( const QModelIndex & index,
                                int value );

signals:

    void constraintSelected( int idx );
    void constraintDeleteRequested( int origin_idx,
                                    int terminal_idx );
    void constraintReplaced( int idx,
                             int origin_idx,
                             int terminal_idx );

};

#endif
