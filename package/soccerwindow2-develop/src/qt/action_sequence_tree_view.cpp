// -*-c++-*-

/*!
  \file action_sequence_selector.h
  \brief action sequence data selector widget Source File.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "action_sequence_tree_view.h"

#include <iostream>
#include <limits>


/*-------------------------------------------------------------------*/
/*!

*/
ActionSequenceTreeView::ActionSequenceTreeView( QWidget * parent )
    : QTreeWidget( parent ),
      M_pressed_item( static_cast< QTreeWidgetItem * >( 0 ) )
{
    //this->setContextMenuPolicy( Qt::CustomContextMenu );
    this->setSelectionBehavior( QAbstractItemView::SelectRows );
    this->setSelectionMode( QAbstractItemView::SingleSelection );
    this->setSortingEnabled( true );
    this->setAlternatingRowColors( true );
    this->setAutoScroll( true );
    this->setRootIsDecorated( false );
#if 1
    // settings for drag and drop reordering
    this->setDragEnabled( true );
    this->setDropIndicatorShown( true );
    this->setDragDropMode( QAbstractItemView::InternalMove );
    this->viewport()->setAcceptDrops( true );
#endif
    this->setEditTriggers( QAbstractItemView::NoEditTriggers ); // handled only by double click

    {
        QTreeWidgetItem * h = this->headerItem();
        h->setText( CLICK_COLUMN, tr( "CLICK" ) );
        h->setText( NO_CLICK_COLUMN, tr( "No CLICK") );
        h->setText( ID_COLUMN, tr( "ID" ) );
        h->setText( VALUE_COLUMN, tr( "Value" ) );
        h->setText( LENGTH_COLUMN, tr( "Len" ) );
        h->setText( SEQ_COLUMN, tr( "Seq" ) );
        h->setText( DESC_COLUMN, tr( "Description" ) );
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    this->header()->setSectionsMovable( false );
#else
    this->header()->setMovable( false );
    //this->header()->setResizeMode( QHeaderView::ResizeToContents );
    //this->header()->setSortIndicatorShown( false );
#endif

    const QFontMetrics metrics = this->fontMetrics();
    this->setColumnWidth( CLICK_COLUMN, metrics.width( tr( "00000" ) ) );
    this->setColumnWidth( NO_CLICK_COLUMN, metrics.width( tr( "00000" ) ) );
    this->setColumnWidth( ID_COLUMN, metrics.width( tr( "00000" ) ) );
    this->setColumnWidth( VALUE_COLUMN, metrics.width( tr( "000000.000" ) ) );
    this->setColumnWidth( LENGTH_COLUMN, metrics.width( tr( "00" ) ) );
    this->setColumnWidth( SEQ_COLUMN, metrics.width( tr( "0000" ) ) );

    connect( this, SIGNAL( itemPressed( QTreeWidgetItem *, int ) ),
             this, SLOT( slotItemPressed( QTreeWidgetItem *, int ) ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
ActionSequenceTreeView::~ActionSequenceTreeView()
{

}

/*-------------------------------------------------------------------*/
/*!

*/
void
ActionSequenceTreeView::dropEvent( QDropEvent * event )
{
    if ( this->sortColumn() != VALUE_COLUMN )
    {
        std::cerr << "(dropEvent) sortColumn != VALUE" << std::endl;
        event->setDropAction( Qt::IgnoreAction );
        event->accept();
        return;
    }

    QModelIndex drop_index = this->indexAt( event->pos() );
    if ( ! drop_index.isValid() )
    {
        std::cerr << "(dropEvent) invalid drop_index" << std::endl;
        event->setDropAction( Qt::IgnoreAction );
        event->accept();
        return;
    }

    QTreeWidget::dropEvent( event );

    // std::cerr << "(dropEvent) drop_index=(" << drop_index.row() << ","
    //           << drop_index.column() << ")" << std::endl;

    if ( ! M_pressed_item )
    {
        std::cerr << "(dropEvent) no pressed item" << std::endl;
        return;
    }

    DropIndicatorPosition dip = this->dropIndicatorPosition();

    QTreeWidgetItem * prev = static_cast< QTreeWidgetItem * >( 0 );
    QTreeWidgetItem * next = static_cast< QTreeWidgetItem * >( 0 );

    if ( dip == QAbstractItemView::AboveItem )
    {
        if ( drop_index.row() > 0 )
        {
            prev = this->topLevelItem( drop_index.row() - 1 );
        }
        next = this->topLevelItem( drop_index.row() );
    }
    else if ( dip == QAbstractItemView::BelowItem )
    {
        prev = this->topLevelItem( drop_index.row() );
        if ( drop_index.row() < this->topLevelItemCount() )
        {
            next = this->topLevelItem( drop_index.row() + 1 );
        }
    }

    if ( ! prev
         && ! next )
    {
        std::cerr << "(dropEvent) no neighbor items" << std::endl;
        return;
    }

    double old_value = M_pressed_item->data( VALUE_COLUMN, Qt::DisplayRole ).toDouble();
    double new_value = old_value;

    if ( prev
         && ! next )
    {
        double prev_value = prev->data( VALUE_COLUMN, Qt::DisplayRole ).toDouble();
        new_value = prev_value - 1.0;
    }

    if ( ! prev
         && next )
    {
        double next_value = next->data( VALUE_COLUMN, Qt::DisplayRole ).toDouble();
        new_value = next_value + 1.0;
    }

    if ( prev
         && next )
    {
        double prev_value = prev->data( VALUE_COLUMN, Qt::DisplayRole ).toDouble();
        double next_value = next->data( VALUE_COLUMN, Qt::DisplayRole ).toDouble();
        new_value = ( prev_value + next_value ) * 0.5;
    }

    std::cerr << "update value " << old_value << " -> " << new_value
              << std::endl;
    M_pressed_item->setData( VALUE_COLUMN, Qt::DisplayRole, new_value );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ActionSequenceTreeView::slotItemPressed( QTreeWidgetItem * item,
                                         int /*column*/ )
{
    M_pressed_item = item;
}
