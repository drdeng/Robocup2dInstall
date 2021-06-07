// -*-c++-*-

/*!
  \file sample_view.h
  \brief sample data view class Header File.
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

#ifndef FEDIT2_SAMPLE_VIEW_H
#define FEDIT2_SAMPLE_VIEW_H

#include <QTreeWidget>

#include <boost/weak_ptr.hpp>

class EditData;

class SampleView
    : public QTreeWidget {

    Q_OBJECT

private:

    boost::weak_ptr< EditData > M_edit_data;

public:

    SampleView( QWidget * parent = 0 );
    ~SampleView();

    void setData( boost::shared_ptr< EditData > ptr )
      {
          M_edit_data = ptr;
      }


    void updateData();

    void selectSample( int idx );
    void unselectData();

protected:

    void contextMenuEvent( QContextMenuEvent * event );
    void dropEvent( QDropEvent * event );

private slots:

    void setCurrentData( QTreeWidgetItem * current );
    void menuChangeSampleIndex();
    void menuDeleteSample();
    void changeCoordinates( const QModelIndex & index,
                            double value );

signals:

    void sampleSelected( int idx );
    void sampleIndexChangeRequested( int old_visual_index,
                                     int new_visual_index );
    void sampleDeleteRequested( int idx );

    void ballReplaced( int idx, double x, double y );
    void playerReplaced( int idx, int unum, double x, double y );

};

#endif
