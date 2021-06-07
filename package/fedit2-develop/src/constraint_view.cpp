// -*-c++-*-

/*!
  \file constrant_view.cpp
  \brief constraint data view class Source File.
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

#include "constraint_view.h"

#include "constraint_delegate.h"
#include "edit_data.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

 */
ConstraintView::ConstraintView( QWidget * parent )
    : QTableWidget( parent )
{
    this->setColumnCount( 3 );

    this->setSelectionBehavior( QAbstractItemView::SelectRows );
    this->setSelectionMode( QAbstractItemView::SingleSelection );
    //this->setEditTriggers( QAbstractItemView::NoEditTriggers );
    this->setEditTriggers( QAbstractItemView::DoubleClicked );
    this->setAlternatingRowColors( true );
    this->setAutoScroll( true );

    //     this->setDragDropMode( QAbstractItemView::DragDrop );
    //     this->setDragDropOverwriteMode( false );
    //     this->setDropIndicatorShown( true );

    QStringList header_labels;
    header_labels << tr( "Index" ) << tr( "Origin" ) << tr( "Terminal" );
    //header_labels << tr( "Origin" ) << tr( "Terminal" );
    this->setHorizontalHeaderLabels( header_labels );

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    this->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
#else
    //this->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    this->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    //this->horizontalHeader()->setStretchLastSection( true );
#endif

    this->verticalHeader()->hide();

    connect( this, SIGNAL( currentItemChanged( QTableWidgetItem *,  QTableWidgetItem * ) ),
             this, SLOT( setCurrentData( QTableWidgetItem * ) ) );

    M_origin_delegate = new ConstraintDelegate( this );
    M_terminal_delegate = new ConstraintDelegate( this );

    this->setItemDelegateForColumn( 1, M_origin_delegate );
    this->setItemDelegateForColumn( 2, M_terminal_delegate );

    connect( M_origin_delegate, SIGNAL( valueChanged( const QModelIndex &, int ) ),
             this, SLOT( changeConstraintIndex( const QModelIndex &, int ) ) );
    connect( M_terminal_delegate, SIGNAL( valueChanged( const QModelIndex &, int ) ),
             this, SLOT( changeConstraintIndex( const QModelIndex &, int ) ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
ConstraintView::~ConstraintView()
{
    //std::cerr << "delete ConstraintView" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConstraintView::updateData()
{
    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr
         || ! ptr->samples() )
    {
        M_origin_delegate->setRange( 0, 0 );
        M_terminal_delegate->setRange( 0, 0 );
        return;
    }

    //
    // update range
    //

    if ( ! ptr->samples()->constraints().empty() )
    {
        int maximum = ptr->samples()->dataCont().size();
        M_origin_delegate->setRange( 1,  maximum );
        M_terminal_delegate->setRange( 1, maximum );
    }

    //
    // update constraints
    //

    const int data_count = ptr->samples()->constraints().size();

    while ( this->rowCount() > data_count )
    {
        this->removeRow( this->rowCount() - 1 );
    }

    int idx = 0;
    const rcsc::formation::SampleDataSet::Constraints::const_iterator end = ptr->samples()->constraints().end();
    for ( rcsc::formation::SampleDataSet::Constraints::const_iterator it = ptr->samples()->constraints().begin();
          it != end;
          ++it, ++idx )
    {
        QTableWidgetItem * item = this->item( idx, 0 );
        if ( ! item )
        {
            this->insertRow( idx );
            int col = 0;

            item = new QTableWidgetItem( QString::number( idx + 1 ) ); // index
            item->setFlags( Qt::ItemIsSelectable
                            | Qt::ItemIsEnabled
                            );
            this->setItem( idx, col, item );
            ++col;

            item = new QTableWidgetItem( QString::number( it->first->index_ + 1 ) );
            item->setFlags( Qt::ItemIsSelectable
                            | Qt::ItemIsEditable
                            | Qt::ItemIsDragEnabled
                            //| Qt::ItemIsDropEnabled
                            | Qt::ItemIsUserCheckable
                            | Qt::ItemIsEnabled
                            );
            this->setItem( idx, col, item );
            ++col;

            item = new QTableWidgetItem( QString::number( it->second->index_ + 1 ) );
            item->setFlags( Qt::ItemIsSelectable
                            | Qt::ItemIsEditable
                            | Qt::ItemIsDragEnabled
                            //| Qt::ItemIsDropEnabled
                            | Qt::ItemIsUserCheckable
                            | Qt::ItemIsEnabled
                            );
            this->setItem( idx, col, item );
            ++col;
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConstraintView::contextMenuEvent( QContextMenuEvent * event )
{
    QTableWidgetItem * item = this->itemAt( event->pos() );

    if ( ! item )
    {
        return;
    }

    if ( ! item->isSelected() )
    {
        //this->setCurrentItem( item );
        this->setCurrentCell( this->row( item ), 0 );
    }

    std::cerr << "ConstraintView::contextMenuEvent "
              << event->pos().x() << ',' << event->pos().y()
              << "  row=" << this->currentRow()
              << std::endl;

    QMenu menu( this );
    //menu.addAction( tr( "Change Index" ), this, SLOT( menuChangeSampleIndex() ) );
    menu.addAction( tr( "Delete" ), this, SLOT( menuDeleteConstraint() ) );

    menu.exec( event->globalPos() );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConstraintView::setCurrentData( QTableWidgetItem * current )
{
    int idx = this->row( current );
    std::cerr << "ConstraintView::setCurrentData() index=" << idx
              << " currentRow=" << this->currentRow()
              << std::endl;
    if ( 0 <= idx )
    {
        emit constraintSelected( idx );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConstraintView::menuDeleteConstraint()
{
    int idx = this->currentRow();
    if ( 0 <= idx )
    {
        int origin = this->item( idx, 1 )->text().toInt();
        int terminal = this->item( idx, 2 )->text().toInt();
        std::cerr << "ConstraintView::menuDeleteConstraint() index=" << idx
                  << " origin=" << origin - 1
                  << " terminal=" << terminal - 1
                  << std::endl;
        emit constraintDeleteRequested( origin - 1, terminal - 1 );
    }
}


/*-------------------------------------------------------------------*/
/*!

 */
void
ConstraintView::changeConstraintIndex( const QModelIndex & index,
                                       int value )
{
    QTableWidgetItem * origin_item = this->item( index.row(), 1 );
    QTableWidgetItem * terminal_item = this->item( index.row(), 2 );

    if ( ! origin_item
         || ! terminal_item )
    {
        return;
    }

    int origin = 0;
    int terminal = 0;

    if ( index.column() == 1 )
    {
        origin = value - 1;
        terminal = terminal_item->text().toInt() - 1;
    }
    else if ( index.column() == 2 )
    {
        origin = origin_item->text().toInt() - 1;
        terminal = value - 1;
    }
    else
    {
        std::cerr << __FILE__ << ':' << __LINE__ << ':'
                  << " Illegal column count"
                  << std::endl;
        return;
    }

    std::cerr << "ConstraintView::changeConstraintIndex "
              << " index=" << index.row() << ',' << index.column()
              << " (" << origin_item->text().toStdString()
              << ',' << terminal_item->text().toStdString() << ")"
              << "->(" << origin << ',' << terminal << ')'
              << std::endl;

    emit constraintReplaced( index.row(), origin, terminal );
}
