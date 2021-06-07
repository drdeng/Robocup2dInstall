// -*-c++-*-

/*!
  \file edit_dialog.cpp
  \brief Formation editor position edit Dialog class Source File.
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

#include "edit_dialog.h"

#include "edit_data.h"
#include "options.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

*/
EditDialog::EditDialog( QWidget * parent )
    : QDialog( parent )
{
    this->setWindowTitle( tr( "Edit Dialog" ) );

    createWidgets();
    updateData();
}

/*-------------------------------------------------------------------*/
/*!

*/
EditDialog::~EditDialog()
{
    //std::cerr << "delete EditDialog" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EditDialog::createWidgets()
{
    QVBoxLayout * top_vbox = new QVBoxLayout();
    top_vbox->setMargin( 2 );
    top_vbox->setSpacing( 2 );
    top_vbox->setSizeConstraint( QLayout::SetFixedSize );

    // method name
    {
        QHBoxLayout * layout = new QHBoxLayout();
        top_vbox->addLayout( layout );

        //layout->addStretch( 1 );
        {
            QLabel * label = new QLabel( tr( "Type Name " ) );
            label->setAlignment( Qt::AlignCenter );
            layout->addWidget( label,
                               1,
                               Qt::AlignLeft | Qt::AlignVCenter );
        }
        //layout->addSpacing( 4 );
        {
            M_type_name = new QLineEdit( tr( "---" ) );
            M_type_name->setMinimumWidth( this->fontMetrics().width( "DelaunayTriangulationXXXX" ) + 4 );
            M_type_name->setReadOnly( true ); // no editable
            M_type_name->setEnabled( false ); // no editable

            layout->addWidget( M_type_name,
                               1,
                               Qt::AlignCenter );
        }
        layout->addStretch( 1 );
    }

    // ball info
    {
        QHBoxLayout * layout = new QHBoxLayout();
        top_vbox->addLayout( layout );

        //layout->addStretch( 1 );
        {
            QLabel * label = new QLabel( tr( "Ball" ) );
            label->setMaximumSize( 40, this->fontMetrics().height() + 12 );
            layout->addWidget( label, 0, Qt::AlignCenter );
        }
        //layout->addStretch( 1 );
        {
            QLabel * label = new QLabel( tr( " X:" ) );
            label->setMaximumSize( 24, this->fontMetrics().height() + 12 );
            layout->addWidget( label, 0, Qt::AlignLeft | Qt::AlignVCenter );
        }
        {
            M_ball_pos_x = new QLineEdit( tr( "0" ) );
            M_ball_pos_x->setMinimumSize( 48, 24 );
            M_ball_pos_x->setMaximumSize( 64, 24 );
            M_ball_pos_x->setValidator( new QDoubleValidator( -57.5, 57.5, 2, M_ball_pos_x ) );
            connect( M_ball_pos_x, SIGNAL( editingFinished() ),
                     this, SLOT( validateBallCoordinate() ) );
            layout->addWidget( M_ball_pos_x, 0, Qt::AlignLeft | Qt::AlignVCenter );
        }
        //layout->addStretch( 1 );
        {
            QLabel * label = new QLabel( tr( " Y:" ) );
            label->setMaximumSize( 24, this->fontMetrics().height() + 12 );
            layout->addWidget( label, 0, Qt::AlignLeft | Qt::AlignVCenter );
        }
        {
            M_ball_pos_y = new QLineEdit( tr( "0" ) );
            M_ball_pos_y->setMinimumSize( 48, 24 );
            M_ball_pos_y->setMaximumSize( 64, 24 );
            M_ball_pos_y->setValidator( new QDoubleValidator( -39.0, 39.0, 2, M_ball_pos_y ) );
            connect( M_ball_pos_y, SIGNAL( editingFinished() ),
                     this, SLOT( validateBallCoordinate() ) );
            layout->addWidget( M_ball_pos_y, 0, Qt::AlignLeft | Qt::AlignVCenter );
        }
        layout->addStretch( 1 );
    }

    {
        const int unum_width = this->fontMetrics().width( tr( "Unum" ) ) + 4;
        const int symmetry_width = this->fontMetrics().width( tr( "0000" ) ) + 4;
        const int role_width = this->fontMetrics().width( tr( "CenterForwardXXXX" ) ) + 4;
        const int coord_width = this->fontMetrics().width( tr( "-00.0000" ) ) + 4;
        const int marker_width = this->fontMetrics().width( tr( "SPM" ) ) + 4;
        const int smarker_width = this->fontMetrics().width( tr( "SPM" ) ) + 4;

        QGridLayout * layout = new QGridLayout();
        top_vbox->addLayout( layout );

        layout->setMargin( 1 );
        layout->setSpacing( 0 );
        layout->setColumnMinimumWidth( 0, unum_width );
        layout->setColumnMinimumWidth( 1, symmetry_width );
        layout->setColumnMinimumWidth( 5, marker_width );
        layout->setColumnMinimumWidth( 6, smarker_width );

        // header
        int row = 0;
        int col = 0;
        layout->addWidget( new QLabel( tr( "Unum" ) ), 0, col, Qt::AlignCenter ); ++col;
        {
            QLabel * l = new QLabel( tr( "R" ) );
            l->setToolTip( tr( "Symmetry Reference Number" ) );
            layout->addWidget( l, 0, col, Qt::AlignCenter ); ++col;
        }
        layout->addWidget( new QLabel( tr( "Type" ) ), 0, col, Qt::AlignCenter ); ++col;
        layout->addWidget( new QLabel( tr( "Role Name" ) ), 0, col, Qt::AlignCenter ); ++col;
        layout->addWidget( new QLabel( tr( "X" ) ), 0, col, Qt::AlignCenter ); ++col;
        layout->addWidget( new QLabel( tr( "Y" ) ), 0, col, Qt::AlignCenter ); ++col;
        {
            QLabel * l = new QLabel( tr( "M" ) );
            l->setToolTip( tr( "Marker Type" ) );
            layout->addWidget( l, 0, col, Qt::AlignCenter ); ++col;
        }
        {
            QLabel * l = new QLabel( tr( "SPM" ) );
            l->setToolTip( tr( "SetPlay Marker Type" ) );
            layout->addWidget( l, 0, col, Qt::AlignCenter ); ++col;
        }

        row = 1;
        for ( int i = 0; i < 11; ++i, ++row )
        {
            col = 0;
            QLabel * label = new QLabel( tr( "%1" ).arg( i + 1 ) );
            label->setAlignment( Qt::AlignCenter );
            label->setMinimumSize( unum_width, 24 );
            label->setMaximumSize( unum_width + 8, 24 );
            layout->addWidget( label, row, col, Qt::AlignCenter );
            ++col;

            M_symmetry_unum[i] = new QLineEdit( tr( "0" ) );
            M_symmetry_unum[i]->setMinimumSize( symmetry_width, 24 );
            M_symmetry_unum[i]->setMaximumSize( symmetry_width, 24 );
            M_symmetry_unum[i]->setValidator( new QIntValidator( -1, 11, M_symmetry_unum[i] ) );
            layout->addWidget( M_symmetry_unum[i], row, col, Qt::AlignCenter );
            ++col;

            M_role_type[i] = new QComboBox();
            M_role_type[i]->addItem( tr( "G" ) );
            M_role_type[i]->addItem( tr( "DF" ) );
            M_role_type[i]->addItem( tr( "MF" ) );
            M_role_type[i]->addItem( tr( "FW" ) );
            layout->addWidget( M_role_type[i], row, col, Qt::AlignCenter );
            ++col;

            M_role_name[i] = new QLineEdit( tr( "Role" ) );
            M_role_name[i]->setMaximumSize( role_width, 24 );
            layout->addWidget( M_role_name[i], row, col, Qt::AlignCenter );
            ++col;

            M_pos_x[i] = new QLineEdit( tr( "0.0" ) );
            M_pos_x[i]->setMaximumSize( coord_width, 24 );
            M_pos_x[i]->setValidator( new QDoubleValidator( -57.5, 57.5, 2, M_pos_x[i] ) );
            layout->addWidget( M_pos_x[i], row, col, Qt::AlignCenter );
            ++col;

            M_pos_y[i] = new QLineEdit( tr( "0.0" ) );
            M_pos_y[i]->setMaximumSize( coord_width, 24 );
            M_pos_y[i]->setValidator( new QDoubleValidator( -39.0, 39.0, 2, M_pos_y[i] ) );
            layout->addWidget( M_pos_y[i], row, col, Qt::AlignCenter );
            ++col;

            M_marker[i] = new QCheckBox();
            layout->addWidget( M_marker[i], row, col, Qt::AlignCenter );
            ++col;

            M_setplay_marker[i] = new QCheckBox();
            layout->addWidget( M_setplay_marker[i], row, col, Qt::AlignCenter );
            ++col;
        }
    }

    {
        QHBoxLayout * layout = new QHBoxLayout();
        top_vbox->addLayout( layout );

        {
            QPushButton * btn = new QPushButton( tr( "Apply" ) );
            //btn->setAutoDefault( false );
            connect( btn, SIGNAL( clicked() ),
                     this, SLOT( applyToField() ) );
            layout->addWidget( btn, 0, Qt::AlignLeft );
        }
        layout->addStretch( 1 );
        {
            QPushButton * btn = new QPushButton( tr( "Reset" ) );
            btn->setAutoDefault( false );
            btn->setDefault( false );
            connect( btn, SIGNAL( clicked() ),
                     this, SLOT( resetChanges() ) );
            layout->addWidget( btn, 0, Qt::AlignRight );
        }
        {
            QPushButton * btn = new QPushButton( tr( "Close" ) );
            btn->setAutoDefault( false );
            btn->setDefault( false );
            connect( btn, SIGNAL( clicked() ),
                     this, SLOT( close() ) );
            layout->addWidget( btn, 0, Qt::AlignRight );
        }
    }

    this->setLayout( top_vbox );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EditDialog::showEvent( QShowEvent * event )
{
    QDialog::showEvent( event );

    updateData();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EditDialog::closeEvent( QCloseEvent * event )
{
    event->accept();
    emit shown( false );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EditDialog::updateData()
{
    if ( ! this->isVisible() )
    {
        return;
    }

    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr )
    {
        return;
    }

    rcsc::Formation::ConstPtr f = ptr->formation();
    if ( ! f )
    {
        std::cerr << "EditDialog. no formation data" << std::endl;
        return;
    }
    M_type_name->setText( QString::fromStdString( f->methodName() ) );


    M_type_name->setText( QString::fromStdString( f->methodName() ) );

    // ball info
    const rcsc::formation::SampleData & s = ptr->state();

    M_ball_pos_x->setText( QString::number( s.ball_.x, 'f', 2 ) );
    M_ball_pos_y->setText( QString::number( s.ball_.y, 'f', 2 ) );

    // player info

    Q_ASSERT( s.players_.size() == 11 );

    for ( size_t i = 0; i < 11; ++i )
    {
        int unum = i + 1;
        //const bool symmetry = ( p->symmetryUnum() > 0 );
        M_symmetry_unum[i]->setText( QString::number( f->getSymmetryNumber( unum ) ) );

        M_role_type[i]->setCurrentIndex( static_cast< int >( f->roleType( unum ).type() ) );
        M_role_name[i]->setText( QString::fromStdString( f->getRoleName( unum ) ) );

        M_pos_x[i]->setText( QString::number( s.players_[i].x, 'f', 2 ) );
        M_pos_y[i]->setText( QString::number( s.players_[i].y, 'f', 2 ) );

        //M_role_name[i]->setReadOnly( symmetry );
        //M_role_name[i]->setEnabled( ! symmetry );

        M_marker[i]->setCheckState( f->isMarker( unum )
                                    ? Qt::Checked
                                    : Qt::Unchecked );
        M_setplay_marker[i]->setCheckState( f->isSetPlayMarker( unum )
                                            ? Qt::Checked
                                            : Qt::Unchecked );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EditDialog::validateBallCoordinate()
{
    QLineEdit * editor = qobject_cast< QLineEdit * >( sender() );
    bool ok = false;
    double value = editor->text().toDouble( &ok );
    if ( ok )
    {
        if ( value != 0.0
             && std::fabs( value ) < 0.5 )
        {
            value = rint( value * 2.0 ) * 0.5;
            editor->setText( QString::number( value, 'f', 2 ) );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EditDialog::resetChanges()
{
    updateData();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EditDialog::applyToField()
{
    boost::shared_ptr< EditData > ptr = M_edit_data.lock();
    if ( ! ptr )
    {
        std::cerr << "EditDialog::applyToField  no data" << std::endl;
        return;
    }

    bool data_auto_select = Options::instance().dataAutoSelect();
    bool player_auto_move = Options::instance().playerAutoMove();
    bool symmetry_mode = Options::instance().symmetryMode();
    Options::instance().setDataAutoSelect( false );
    Options::instance().setPlayerAutoMove( false );
    Options::instance().setSymmetryMode( false );

    // ball
    {
        bool ok_x = false;
        bool ok_y = false;
        double x = M_ball_pos_x->text().toDouble( &ok_x );
        double y = M_ball_pos_y->text().toDouble( &ok_y );
        if ( ok_x && ok_y )
        {
            ptr->moveBallTo( x, y );
        }
    }

    // players
    for ( int unum = 1; unum <= 11; ++unum )
    {
        bool ok = false;
        int symmetry_unum = M_symmetry_unum[unum-1]->text().toInt( &ok );
        if ( ! ok )
        {
            std::cerr << __FILE__ << ':' << __LINE__
                      << " *** ERROR *** Invalid symmetry number."
                      << std::endl;
            continue;
        }

        std::string role_name = M_role_name[unum-1]->text().toStdString();
        if ( role_name.empty() )
        {
            std::cerr << __FILE__ << ':' << __LINE__
                      << " *** ERROR *** Empty role name."
                      << std::endl;
        }
        else
        {
            ptr->updateRoleData( unum, symmetry_unum, role_name );
        }

        bool ok_x = false;
        bool ok_y = false;
        double x = M_pos_x[unum-1]->text().toDouble( &ok_x );
        double y = M_pos_y[unum-1]->text().toDouble( &ok_y );
        if ( ok_x && ok_y )
        {
            ptr->movePlayerTo( unum, x, y );
        }

        ptr->updateRoleType( unum, M_role_type[unum-1]->currentIndex() );

        ptr->updateMarkerData( unum,
                               ( M_marker[unum-1]->checkState() == Qt::Checked ),
                               ( M_setplay_marker[unum-1]->checkState() == Qt::Checked ) );
    }

    updateData();

    Options::instance().setDataAutoSelect( data_auto_select );
    Options::instance().setPlayerAutoMove( player_auto_move );
    Options::instance().setSymmetryMode( symmetry_mode );

    emit viewUpdated();

}
