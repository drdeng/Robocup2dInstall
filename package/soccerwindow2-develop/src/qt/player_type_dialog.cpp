// -*-c++-*-

/*!
  \file player_type_dialog.cpp
  \brief player type list ialog class Source File.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "player_type_dialog.h"

#include "main_data.h"

#include <rcsc/common/server_param.h>
#include <rcsc/common/player_type.h>

#include <iostream>
#include <cstdio>

namespace {
const int COL_SIZE = 15; // the number of param string
//const int FONT_SIZE = 10;
//const int CELL_HEIGHT = 18;
}

class PlayerTypeSortProxy
    : public QSortFilterProxyModel {
public:
    PlayerTypeSortProxy( QObject * parent )
        : QSortFilterProxyModel( parent )
      { }

protected:

    virtual
    bool lessThan( const QModelIndex & left,
                   const QModelIndex & right ) const
      {
          const QAbstractItemModel * src = sourceModel();

          double lhs = src->data( left ).toString().toDouble();
          double rhs = src->data( right ).toString().toDouble();

          return lhs < rhs;
      }
};

/*-------------------------------------------------------------------*/
/*!

*/
PlayerTypeDialog::PlayerTypeDialog( QWidget * parent,
                            const MainData & main_data )
    : QDialog( parent, Qt::MSWindowsFixedSizeDialogHint )
    , M_main_data( main_data )
{
    this->setWindowTitle( tr( "Player Type List" ) );
    this->setMinimumWidth( 640 );

    //QFont font = this->font();
    //font.setPointSize( 8 );
    //this->setFont( font );

    createTable();
}

/*-------------------------------------------------------------------*/
/*!

*/
PlayerTypeDialog::~PlayerTypeDialog()
{
    //std::cerr << "delete PlayerTypeDialog" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerTypeDialog::createTable()
{
    QVBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins( 4, 4, 4, 4 );

    M_item_view = new QTableView();
    //M_item_view->setRootIsDecorated( false );
    //M_item_view->setGridStyle( Qt::NoPen );
    M_item_view->verticalHeader()->hide();
    M_item_view->setShowGrid( false );
    M_item_view->setAlternatingRowColors( true );
    M_item_view->setSortingEnabled( true );
    M_item_view->setEditTriggers( QAbstractItemView::NoEditTriggers );

    M_item_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    M_item_view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    M_item_view->horizontalHeader()->setSortIndicatorShown( false );
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    M_item_view->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
#else
    M_item_view->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
#endif

    QFont font = M_item_view->font();
    font.setPointSize( 10 );
    M_item_view->setFont( font );


    createModel();

    QSortFilterProxyModel * proxy = new PlayerTypeSortProxy( this );
    proxy->setSourceModel( M_model );
    M_item_view->setModel( proxy );

    //std::cerr << "table row count = " << M_model->rowCount() << std::endl;

    connect( M_item_view, SIGNAL( doubleClicked( const QModelIndex & ) ),
             this, SLOT( handleDoubleClick( const QModelIndex & ) ) );


    layout->addWidget( M_item_view );
    this->setLayout( layout );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerTypeDialog::createModel()
{
    const int ROW_SIZE = M_main_data.viewHolder().playerTypeCont().size();

    //M_model = new QStandardItemModel( ROW_SIZE, 16, this );
    M_model = new QStandardItemModel( ROW_SIZE, 13, this );

    int i = 0;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "id" ) ); ++i;
    //M_model->setHeaderData( i, Qt::Horizontal, tr( "Size" ) ); ++i;
    //M_model->setHeaderData( i, Qt::Horizontal, tr( "Speed Real/Max" ) ); ++i;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "MaxSpeed" ) ); ++i;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "AccStep" ) ); ++i;
    //M_model->setHeaderData( i, Qt::Horizontal, tr( "AccMax" ) ); ++i;

    M_model->setHeaderData( i, Qt::Horizontal, tr( "5m" ) ); ++i;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "10m" ) ); ++i;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "20m" ) ); ++i;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "30m" ) ); ++i;

    // M_model->setHeaderData( i, Qt::Horizontal, tr( "DashRate" ) ); ++i;
    // M_model->setHeaderData( i, Qt::Horizontal, tr( "Decay" ) ); ++i;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "IMoment" ) ); ++i;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "KickArea" ) ); ++i;
    //M_model->setHeaderData( i, Qt::Horizontal, tr( "KickMargin" ) ); ++i;
    // M_model->setHeaderData( i, Qt::Horizontal, tr( "KickRate" ) ); ++i;
    // M_model->setHeaderData( i, Qt::Horizontal, tr( "KickRand" ) ); ++i;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "UCatchRange" ) ); ++i;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "StamInc" ) ); ++i;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "Consume" ) ); ++i;
    M_model->setHeaderData( i, Qt::Horizontal, tr( "ExtStam" ) ); ++i;
    // M_model->setHeaderData( i, Qt::Horizontal, tr( "Eff Max-Min" ) ); ++i;
    // M_model->setHeaderData( i, Qt::Horizontal, tr( "FoulProb" ) ); ++i;

    updateData();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerTypeDialog::updateData()
{
    if ( ! this->isVisible() )
    {
        return;
    }

    const int ROW_SIZE = M_main_data.viewHolder().playerTypeCont().size();

    if ( M_model->rowCount() != ROW_SIZE )
    {
        std::cerr << __FILE__ << ": (updateData) "
                  << "invalid row count" << M_model->rowCount() << std::endl;
        M_model->setRowCount( ROW_SIZE );
    }

    const rcsc::ServerParam & SP = rcsc::ServerParam::i();

    for ( int row = 0; row < ROW_SIZE; ++row )
    {
        const std::map< int , rcsc::PlayerType >::const_iterator it
            = M_main_data.viewHolder().playerTypeCont().find( row );
        if ( it == M_main_data.viewHolder().playerTypeCont().end() )
        {
            continue;
        }

        const rcsc::PlayerType & param = it->second;

        int col = 0;
        char buf[32];

        // id
        M_model->setData( M_model->index( row, col++ ),
                          param.id(),
                          Qt::DisplayRole );

        // size
//         //snprintf( buf, 32, "%.2f", param.playerSize() );
//         M_model->setData( M_model->index( row, col++ ),
//                           QString::number( param.playerSize() ), //QString::fromAscii( buf ),
//                           Qt::DisplayRole );

        // // speed real/max
        // snprintf( buf, 32, "%5.3f / %5.3f",
        //           param.realSpeedMax(), param.playerSpeedMax() );
        // M_model->setData( M_model->index( row, col++ ),
        //                   QString::fromLatin1( buf ),
        //                   Qt::DisplayRole );
        M_model->setData( M_model->index( row, col++ ),
                          param.realSpeedMax(),
                          Qt::DisplayRole );

        // accel step
        M_model->setData( M_model->index( row, col++ ),
                          param.cyclesToReachMaxSpeed(),
                          Qt::DisplayRole );
#if 0
        // accel max
        //snprintf( buf, 32, "%.4f",
        //          SP.maxPower() * param.dashPowerRate() * param.effortMax() );
        M_model->setData( M_model->index( row, col++ ),
                          //QString::fromLatin1( buf ),
                          QString::number( SP.maxPower() * param.dashPowerRate() * param.effortMax(), 'g', 5 ),
                          Qt::DisplayRole );
#endif
        // steps to reach each distance
        M_model->setData( M_model->index( row, col++  ),
                          param.cyclesToReachDistance( 5.0 ),
                          Qt::DisplayRole );
        M_model->setData( M_model->index( row, col++  ),
                          param.cyclesToReachDistance( 10.0 ),
                          Qt::DisplayRole );
        M_model->setData( M_model->index( row, col++  ),
                          param.cyclesToReachDistance( 20.0 ),
                          Qt::DisplayRole );
        M_model->setData( M_model->index( row, col++  ),
                          param.cyclesToReachDistance( 30.0 ),
                          Qt::DisplayRole );

#if 0
        // dash power rate
        //snprintf( buf, 32, "%.5f", param.dashPowerRate() );
        M_model->setData( M_model->index( row, col++ ),
                          //QString::fromLatin1( buf ),
                          QString::number( param.dashPowerRate(), 'g', 4 ),
                          Qt::DisplayRole );
#endif
#if 0
        // decay
        //snprintf( buf, 32, "%.3f", param.playerDecay() );
        M_model->setData( M_model->index( row, col++ ),
                          //QString::fromLatin1( buf ),
                          QString::number( param.playerDecay(), 'g', 5 ),
                          Qt::DisplayRole );
#endif
        // inertia moment
        //snprintf( buf, 32, "%.2f", param.inertiaMoment() );
        M_model->setData( M_model->index( row, col++ ),
                          //QString::fromLatin1( buf ),
                          QString::number( param.inertiaMoment(), 'g', 5 ),
                          Qt::DisplayRole );

        // kickable area
        //snprintf( buf, 32, "%.3f", param.playerSize() + param.kickableMargin() + SP.ballSize() );
        M_model->setData( M_model->index( row, col++ ),
                          //QString::fromLatin1( buf ),
                          QString::number( param.playerSize() + param.kickableMargin() + SP.ballSize() ),
                          Qt::DisplayRole );

        // kickable margin
//         //snprintf( buf, 32, "%.3f", param.kickableMargin() );
//         M_model->setData( M_model->index( row, col++ ),
//                           //QString::fromLatin1( buf ),
//                           QString::number( param.kickableMargin() ),
//                           Qt::DisplayRole );
#if 0
        // kick power rate
        M_model->setData( M_model->index( row, col++ ),
                          QString::number( param.kickPowerRate() ),
                          Qt::DisplayRole );
#endif
#if 0
        // kick rand
        //snprintf( buf, 32, "%.2f", param.kickRand() );
        M_model->setData( M_model->index( row, col++ ),
                          //QString::fromLatin1( buf ),
                          QString::number( param.kickRand(), 'g', 4 ),
                          Qt::DisplayRole );
#endif
        // catch area length stretch
        double max_r = std::sqrt( std::pow( SP.catchAreaWidth() * 0.5, 2 )
                                  + std::pow( SP.catchAreaLength() * param.catchAreaLengthStretch(), 2 ) );
        double min_r = std::sqrt( std::pow( SP.catchAreaWidth() * 0.5, 2 )
                                  + std::pow( SP.catchAreaLength() * ( 1.0 - ( param.catchAreaLengthStretch() - 1.0 ) ), 2 ) );
        snprintf( buf, 32, "%.3f - %.3f",
                  min_r, max_r );
        M_model->setData( M_model->index( row, col++ ),
                          QString::fromLatin1( buf ),
                          Qt::DisplayRole );

        // stamina inc max
        //snprintf( buf, 32, "%.2f", param.staminaIncMax() );
        M_model->setData( M_model->index( row, col++ ),
                          //QString::fromLatin1( buf ),
                          QString::number( param.staminaIncMax() ),
                          Qt::DisplayRole );

        // consume
        //snprintf( buf, 32, "%.2f", param.getOneStepStaminaComsumption() );
        M_model->setData( M_model->index( row, col++ ),
                          //QString::fromLatin1( buf ),
                          QString::number( param.getOneStepStaminaComsumption(), 'g', 4 ),
                          Qt::DisplayRole );

        // extra stamina
        //snprintf( buf, 32, "%.2f", param.extraStamina() );
        M_model->setData( M_model->index( row, col++ ),
                          //QString::fromLatin1( buf ),
                          QString::number( param.extraStamina(), 'g', 4 ),
                          Qt::DisplayRole );
#if 0
        // effort max - min
        snprintf( buf, 32, "%.3f - %.3f",
                  param.effortMax(), param.effortMin() );
        M_model->setData( M_model->index( row, col++ ),
                          QString::fromLatin1( buf ),
                          Qt::DisplayRole );
#endif
#if 0
        // foul detect probability
        M_model->setData( M_model->index( row, col++ ),
                          QString::number( param.foulDetectProbability() ),
                          Qt::DisplayRole );
#endif
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerTypeDialog::handleDoubleClick( const QModelIndex & index )
{
    // QVariant value = M_model->data( index );
    // std::cerr << "handleDoubleClick row=" << index.row()
    //           << " column=" << index.column()
    //           << " value=" << value.toString().toStdString()
    //           << std::endl;
    // if ( ! value.isValid() )
    // {
    //     std::cerr << "Invalid value" << std::endl;
    // }

    // const int id = M_model->data( M_model->index( index.row(), 0 ) ).toInt();
    const int id = index.row();

    const std::map< int, rcsc::PlayerType >::const_iterator it
        = M_main_data.viewHolder().playerTypeCont().find( id );
    if ( it == M_main_data.viewHolder().playerTypeCont().end() )
    {
        return;
    }

    const rcsc::PlayerType & ptype = it->second;

    const double dash_power = rcsc::ServerParam::i().maxDashPower();
    const double max_accel = dash_power * ptype.dashPowerRate() * ptype.effortMax();
    double speed = 0.0;
    double dist = 0.0;

    std::fprintf( stdout, "===== player type %d dash distance table=====\n",
                  id );
    for ( int i = 1; i <= 50; ++i )
    {
        speed += max_accel;
        if ( speed > ptype.playerSpeedMax() )
        {
            speed = ptype.playerSpeedMax();
        }
        dist += speed;
        std::fprintf( stdout, "%2d: speed=%.3f(%.3f) dist=%7.3f\n",
                      i,
                      speed,
                      speed * ptype.playerDecay(),
                      dist );

        speed *= ptype.playerDecay();
    }

    std::fflush( stdout );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerTypeDialog::showEvent( QShowEvent * event )
{
    updateData();

    QFontMetrics metrics = M_item_view->fontMetrics();

    //M_item_view->resizeRowsToContents();
    const int row_height = metrics.height() + 2;
    const int ROW_SIZE = M_main_data.viewHolder().playerTypeCont().size();
    for ( int i = 0; i < ROW_SIZE; ++i )
    {
        M_item_view->setRowHeight( i, row_height );
    }

    int i = 0;
    // id
    M_item_view->setColumnWidth( i, metrics.width( " 00" ) + 4 ); ++i;
//     // size
//     M_item_view->setColumnWidth( i, metrics.width( "  0.00" ) + 4 ); ++i;
    // speed max
    M_item_view->setColumnWidth( i, metrics.width( "00.000 / 00.000" ) + 4 ); ++i;
    // accel step
    M_item_view->setColumnWidth( i, metrics.width( "   0" ) + 4 ); ++i;
    // accel max
    //M_item_view->setColumnWidth( i, metrics.width( "  0.000000" ) + 4 ); ++i;
    // reachable steps
    M_item_view->setColumnWidth( i, metrics.width( " 00" ) + 4 ); ++i; // 5m
    M_item_view->setColumnWidth( i, metrics.width( " 00" ) + 4 ); ++i; // 10m
    M_item_view->setColumnWidth( i, metrics.width( " 00" ) + 4 ); ++i; // 20m
    M_item_view->setColumnWidth( i, metrics.width( " 00" ) + 4 ); ++i; // 30m
    // dash power rate
    //M_item_view->setColumnWidth( i, metrics.width( "  0.000000" ) + 4 ); ++i;
    // decay
    //M_item_view->setColumnWidth( i, metrics.width( "  0.0000" ) + 4 ); ++i;
    // inertia moment
    M_item_view->setColumnWidth( i, metrics.width( "  0.0000" ) + 4 ); ++i;
    // kickable area
    M_item_view->setColumnWidth( i, metrics.width( "  0.0000" ) + 4 ); ++i;
//     // kickable margin
//     M_item_view->setColumnWidth( i, metrics.width( "  0.0000" ) + 4 ); ++i;
    // kick power rate
    //M_item_view->setColumnWidth( i, metrics.width( "  0.000000" ) + 4 ); ++i;
    // kick rand
    //M_item_view->setColumnWidth( i, metrics.width( "  0.0000" ) + 4 ); ++i;
    // catch area
    M_item_view->setColumnWidth( i, metrics.width( "  0.000 - 0.000" ) + 4 ); ++i;
    // stamina inc max
    M_item_view->setColumnWidth( i, metrics.width( "  00.00" ) + 4 ); ++i;
    // consume
    M_item_view->setColumnWidth( i, metrics.width( "  00.00" ) + 4 ); ++i;
    // extra stamina
    M_item_view->setColumnWidth( i, metrics.width( "  00.00" ) + 4 ); ++i;
    // effort max - min
    //M_item_view->setColumnWidth( i, metrics.width( "  0.000 - 0.000" ) + 4 ); ++i;
    // foul detect probability
    //M_item_view->setColumnWidth( i, metrics.width( "  0.0000" ) + 4 ); ++i;

    QRect rect = this->geometry();
    QRect child_rect = this->childrenRect();
    int width_diff = rect.width() - child_rect.width();
    int height_diff = rect.height() - child_rect.height();

    rect.setWidth( M_item_view->horizontalHeader()->length()
                   + width_diff );
    rect.setHeight( M_item_view->horizontalHeader()->height()
                    + M_item_view->rowHeight( 0 ) * ROW_SIZE
                    + height_diff );

    this->setGeometry( rect );

    event->accept();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerTypeDialog::wheelEvent( QWheelEvent * event )
{
    if ( event->delta() < 0 )
    {
        this->setWindowOpacity( std::max( 0.1, this->windowOpacity() - 0.05 ) );
    }
    else
    {
        this->setWindowOpacity( std::min( 1.0, this->windowOpacity() + 0.05 ) );
    }

    event->accept();
}
