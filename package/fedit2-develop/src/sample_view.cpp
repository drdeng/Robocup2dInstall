// -*-c++-*-

/*!
  \file sample_view.cpp
  \brief sample data view class Source File.
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

#include "sample_view.h"

#include "coordinate_delegate.h"
#include "edit_data.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

 */
SampleView::SampleView( QWidget * parent )
    : QTreeWidget( parent )
{
    this->setSelectionBehavior( QAbstractItemView::SelectRows );
    this->setSelectionMode( QAbstractItemView::SingleSelection );
    //this->setEditTriggers( QAbstractItemView::NoEditTriggers );
    this->setEditTriggers( QAbstractItemView::DoubleClicked );
    this->setAlternatingRowColors( true );
    this->setAutoScroll( true );
    this->setExpandsOnDoubleClick( true );

    //this->setDragDropMode( QAbstractItemView::DragOnly );
    this->setDragDropMode( QAbstractItemView::DragDrop );
    //this->setDragDropMode( QAbstractItemView::InternalMove );
    this->setDragDropOverwriteMode( false );
    this->setDropIndicatorShown( true );

    QStringList header_labels;
    //header_labels << tr( "Index" ) << tr( "Value" );
    header_labels << tr( "Index" ) << tr( "X" ) << tr( "Y" );
    this->setHeaderLabels( header_labels );
    //this->setHeaderHidden( true );

    this->setColumnWidth( 0, this->fontMetrics().width( tr( "1234567890" ) + 64 ) );
    this->setColumnWidth( 1, this->fontMetrics().width( tr( "-12.3456" ) + 32 ) );
    this->setColumnWidth( 2, this->fontMetrics().width( tr( "-12.3456" ) + 32 ) );

    //     M_constraints = new QTreeWidgetItem( this );
    //     M_constraints->setText( 0, tr( "Constraints" ) );
    //     M_constraints->setExpanded( true );
    //     M_constraints->setFlags( Qt::ItemIsEnabled
    //                              | Qt::ItemIsDropEnabled );

//     M_samples = new QTreeWidgetItem( this );
//     M_samples->setText( 0, tr( "Samples" ) );
//     M_samples->setExpanded( true );
//     M_samples->setFlags( Qt::ItemIsEnabled
//                          | Qt::ItemIsDropEnabled );

    connect( this, SIGNAL( currentItemChanged( QTreeWidgetItem *,  QTreeWidgetItem * ) ),
             this, SLOT( setCurrentData( QTreeWidgetItem * ) ) );
    //     connect( this, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ),
    //              this, SLOT( doubleClickItem( QTreeWidgetItem *, int ) ) );

    CoordinateDelegate * x_delegate = new CoordinateDelegate( -52.5 - 2.0, 52.5 + 2.0, this );
    CoordinateDelegate * y_delegate = new CoordinateDelegate( -34.0 - 2.0, 34.0 + 2.0, this );

    this->setItemDelegateForColumn( 1, x_delegate );
    this->setItemDelegateForColumn( 2, y_delegate );

    connect( x_delegate, SIGNAL( valueChanged( const QModelIndex &, double ) ),
             this, SLOT( changeCoordinates( const QModelIndex &, double ) ) );
    connect( y_delegate, SIGNAL( valueChanged( const QModelIndex &, double ) ),
             this, SLOT( changeCoordinates( const QModelIndex &, double ) ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
SampleView::~SampleView()
{
    //std::cerr << "delete SampleView" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
SampleView::updateData()
{
    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr
         || ! ptr->samples() )
    {
        return;
    }

    //
    // update samples
    //

    const int data_count = ptr->samples()->dataCont().size();

    while ( this->topLevelItemCount() > data_count )
    {
        this->takeTopLevelItem( this->topLevelItemCount() - 1 );
    }

    int idx = 0;
    const rcsc::formation::SampleDataSet::DataCont::const_iterator end = ptr->samples()->dataCont().end();
    for ( rcsc::formation::SampleDataSet::DataCont::const_iterator it = ptr->samples()->dataCont().begin();
          it != end;
          ++it, ++idx )
    {
        QTreeWidgetItem * item = this->topLevelItem( idx );
        if ( ! item )
        {
            item = new QTreeWidgetItem();
            item->setFlags( Qt::ItemIsSelectable
                            | Qt::ItemIsEditable
                            | Qt::ItemIsDragEnabled
                            //| Qt::ItemIsDropEnabled
                            | Qt::ItemIsUserCheckable
                            | Qt::ItemIsEnabled
                            );
            this->insertTopLevelItem( idx, item );

            for ( size_t i = 0; i < it->players_.size(); ++i )
            {
                QTreeWidgetItem * p = new QTreeWidgetItem();
                p->setText( 0, tr( "p%1" ).arg( i + 1 ) );
                //p->setData( 0, Qt::DisplayRole, tr( "p%1" ).arg( i + 1 ) );
                p->setFlags( //Qt::ItemIsSelectable
                            Qt::ItemIsEditable
                            //| Qt::ItemIsDragEnabled
                            //| Qt::ItemIsUserCheckable
                            | Qt::ItemIsEnabled
                             );

                item->addChild( p );
            }
        }

        item->setText( 0, QString::number( idx ) ); // visual index
        item->setText( 1, QString::number( it->ball_.x, 'f', 2 ) );
        item->setText( 2, QString::number( it->ball_.y, 'f', 2 ) );
        //item->setData( 0, Qt::DisplayRole, idx + 1 ); // visual index
        //item->setData( 1, Qt::EditRole, it->ball_.x );
        //item->setData( 2, Qt::EditRole, it->ball_.y );

        for ( size_t i = 0; i < it->players_.size(); ++i )
        {
            QTreeWidgetItem * p = item->child( i );
            if ( ! p )
            {
                std::cerr << "SampleView::updateData " << idx
                          << " player " << i + 1 << " not found"
                          << std::endl;
                //p = new QTreeWidgetItem();
                //p->setText( 0, QString::number( i + 1 ) );
                //item->insertChild( i, p );
                continue;
            }

            p->setText( 1, QString::number( it->players_[i].x, 'f', 2 ) );
            p->setText( 2, QString::number( it->players_[i].y, 'f', 2 ) );
            //p->setData( 1, Qt::EditRole, it->players_[i].x );
            //p->setData( 2, Qt::EditRole, it->players_[i].y );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
SampleView::selectSample( int idx )
{
    //std::cerr << "SampleView::selectDataIndex " << idx << std::endl;

    QTreeWidgetItem * item = this->topLevelItem( idx );
    if ( item )
    {
        //         this->clearSelection();
        //         item->setSelected( true );
        //         this->scrollToItem( item );
        this->setCurrentItem( item );
    }
    else
    {
        this->setCurrentItem( static_cast< QTreeWidgetItem * >( 0 ) );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
SampleView::unselectData()
{
    //std::cerr << "SampleView::unselectDataIndex" << std::endl;

    //this->clearSelection();
    this->setCurrentItem( static_cast< QTreeWidgetItem * >( 0 ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
// Qt::DropActions
// SampleView::supportedDropActions() const
// {
//     return Qt::MoveAction;
// }

/*-------------------------------------------------------------------*/
/*!

 */
void
SampleView::contextMenuEvent( QContextMenuEvent * event )
{
    QTreeWidgetItem * item = this->itemAt( event->pos() );

    if ( ! item )
    {
        return;
    }

    if ( item->parent() )
    {
        std::cerr << "contextMenuEvent clicked item has parent." << std::endl;
        return;
    }

    if ( ! item->isSelected() )
    {
        //         this->clearSelection();
        //         item->setSelected( true );
        this->setCurrentItem( item );
    }

    std::cerr << "SampleView::contextMenuEvent "
              << event->pos().x() << ',' << event->pos().y()
              << "  item: idx=" << item->text( 0 ).toStdString()
              << "  value=" << item->text( 1 ).toStdString()
              << std::endl;

    QMenu menu( this );
    menu.addAction( tr( "Change Index" ), this, SLOT( menuChangeSampleIndex() ) );
    menu.addAction( tr( "Delete" ), this, SLOT( menuDeleteSample() ) );

    menu.exec( event->globalPos() );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
SampleView::dropEvent( QDropEvent * event )
{
    //std::cerr << "dropEvent action=" << std::hex << event->dropAction()
    //          << " possibleActions=" << std::hex << event->possibleActions()
    //          << " proposedAction=" << std::hex << event->proposedAction()
    //          << "\n  pos=" << event->pos().x() << ',' << event->pos().y()
    //          << std::endl;

    QTreeWidgetItem * moving = this->currentItem();
    if ( ! moving )
    {
        event->ignore();
        return;
    }

    QTreeWidgetItem * dest = this->itemAt( event->pos() );
    if ( ! dest )
    {
        event->ignore();
        return;
    }

    if ( this->dropIndicatorPosition() == QAbstractItemView::AboveItem )
    {
        int dest_index = this->indexOfTopLevelItem( dest );
        if ( dest_index > 0 )
        {
            dest = this->topLevelItem( dest_index - 1 );
            if ( ! dest )
            {
                event->ignore();
                return;
            }
        }
    }

    if ( moving == dest )
    {
        event->ignore();
        return;
    }

    if ( moving->parent() != dest->parent() )
    {
        event->ignore();
        return;
    }

    event->ignore();

    //std::cerr << "  moving item = " << moving->text( 0 ).toStdString()
    //          << " , " << moving->text( 1 ).toStdString()
    //          << "\n  dest=" << dest->text( 0 ).toStdString()
    //          << " , " << dest->text( 1 ).toStdString()
    //          << std::endl;

    if ( ! moving->parent() )
    {
        int visual_moving_index = this->indexOfTopLevelItem( moving ) + 1;
        int visual_dest_index = this->indexOfTopLevelItem( dest ) + 1;

        emit sampleIndexChangeRequested( visual_moving_index, visual_dest_index );
    }

    //QTreeWidget::dropEvent( event );
}


/*-------------------------------------------------------------------*/
/*!

 */
// bool
// SampleView::dropMimeData( QTreeWidgetItem * parent,
//                         int index,
//                         const QMimeData * data,
//                         Qt::DropAction action )
// {
//     std::cerr << "drpMimeData parent=" << parent->text( 0 ).toStdString()
//               << " index=" << index
//               << " mimeData=" << data->text().toStdString()
//               << " action=" << action
//               << std::endl;

//     return QTreeWidget::dropMimeData( parent, index, data, action );
// }

/*-------------------------------------------------------------------*/
/*!

 */
void
SampleView::setCurrentData( QTreeWidgetItem * current )
{
    int idx = this->indexOfTopLevelItem( current );
    if ( 0 <= idx )
    {
        emit sampleSelected( idx );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
SampleView::menuChangeSampleIndex()
{
    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr
         || ! ptr->samples() )
    {
        return;
    }

    QTreeWidgetItem * item = this->currentItem();
    if ( ! item )
    {
        return;
    }

    int idx = this->indexOfTopLevelItem( item );
    if ( 0 <= idx )
    {
        const int data_size = ptr->samples()->dataCont().size();
        if ( data_size == 0 )
        {
            return;
        }

        int visual_idx = idx + 1;
        bool ok = false;
        int new_visual_idx = QInputDialog::getInt( this,
                                                   tr( "Select New Index" ),
                                                   tr( "New Index" ),
                                                   visual_idx, // default
                                                   1, // min
                                                   data_size, // max
                                                   1, // step
                                                   &ok );
        if ( ok )
        {
            if ( visual_idx == new_visual_idx )
            {
                return;
            }

            emit sampleIndexChangeRequested( visual_idx, new_visual_idx );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
SampleView::menuDeleteSample()
{
    QTreeWidgetItem * item = this->currentItem();
    if ( item )
    {
        int idx = this->indexOfTopLevelItem( item );
        if ( 0 <= idx )
        {
            emit sampleDeleteRequested( idx );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
// void
// SampleView::doubleClickItem( QTreeWidgetItem * item,
//                            int column )
// {
//     if ( column != 1 )
//     {
//         return;
//     }

//     if ( ! item->parent() )
//     {
//         //std::cerr << "SampleView::doubleClickItem topLevelItem" << std::endl;
//         return;
//     }

//     std::cerr << "SampleView::doubleClickItem " << item->text( 0 ).toStdString()
//               << " , " << item->text( 1 ).toStdString()
//               << std::endl;

//     editItem( item, 1 );
//     //openPersistentEditor( item, 1 );
// }


/*-------------------------------------------------------------------*/
/*!

 */
void
SampleView::changeCoordinates( const QModelIndex & index,
                             double value )
{
    std::cerr << "SampleView::changeCoordinates "
              << " index=" << index.row() << ',' << index.column()
              << " value=" << value
              << std::endl;
    QTreeWidgetItem * item = this->itemFromIndex( index );

    if ( item )
    {
        if ( ! item->parent() ) // no parent == root item
        {
            // ball
            int idx = this->indexOfTopLevelItem( item );
            if ( 0 <= idx )
            {
                double x = 0.0, y = 0.0;
                if ( index.column() == 1 )
                {
                    x = value;
                    y = item->text( 2 ).toDouble();
                }
                else if ( index.column() == 2 )
                {
                    x = item->text( 1 ).toDouble();
                    y = value;
                }
                else
                {
                    std::cerr << __FILE__ << ':' << __LINE__ << ':'
                              << " Illegal column count"
                              << std::endl;
                    return;
                }

                emit ballReplaced( idx, x, y );
            }
        }
        else if ( item->parent()
                  && ! item->parent()->parent() )
        {
            // players
            int idx = this->indexOfTopLevelItem( item->parent() );
            if ( 0 <= idx )
            {
                int unum = item->text( 0 ).mid( 1 ).toInt();

                double x = 0.0, y = 0.0;
                if ( index.column() == 1 )
                {
                    x = value;
                    y = item->text( 2 ).toDouble();
                }
                else if ( index.column() == 2 )
                {
                    x = item->text( 1 ).toDouble();
                    y = value;
                }
                else
                {
                    std::cerr << __FILE__ << ':' << __LINE__ << ':'
                              << " Illegal column count"
                              << std::endl;
                    return;
                }

                emit playerReplaced( idx, unum, x, y );
            }
        }
    }

}

/*-------------------------------------------------------------------*/
/*!

 */
// void
// SampleView::closeEditor( QWidget * editor,
//                        QAbstractItemDelegate::EndEditHint hint )
// {
//     std::cerr << "SampleView::closeEditor hint=" //<< hint
//               << std::endl;
//     QTreeWidget::closeEditor( editor, hint );
// }
