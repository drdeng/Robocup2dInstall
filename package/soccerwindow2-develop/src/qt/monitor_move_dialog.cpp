// -*-c++-*-

/*!
  \file monitor_move_dialog.cpp
  \brief monitor client move control dialog class Source File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa Akiyama

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

#include "monitor_move_dialog.h"

#include "options.h"
#include "main_data.h"
#include "trainer_data.h"

#include <fstream>
#include <iostream>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cassert>

/*-------------------------------------------------------------------*/
/*!

 */
MonitorMoveDialog::MonitorMoveDialog( QWidget * parent,
                                      const MainData & main_data,
                                      TrainerData & trainer_data )


    : QDialog( parent )
    , M_main_data( main_data )
    , M_trainer_data( trainer_data )
{
    this->setWindowTitle( tr( "Player Move Control" ) );

    createWidgets();
}

/*-------------------------------------------------------------------*/
/*!

 */
MonitorMoveDialog::~MonitorMoveDialog()
{
    //std::cerr << "delete MonitorMoveDialog" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MonitorMoveDialog::createWidgets()
{
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setSizeConstraint( QLayout::SetFixedSize );
    top_layout->setContentsMargins( 5, 5, 5, 5 );
    top_layout->setSpacing( 2 );

    // read/save buttons
    {
        QHBoxLayout * layout = new QHBoxLayout();

        QPushButton * read_btn = new QPushButton( tr( "Read Field" ) );
        read_btn->setAutoDefault( false );
        connect( read_btn, SIGNAL( clicked() ),
                 this, SLOT( readFieldStatus() ) );
        layout->addWidget( read_btn, 1, Qt::AlignVCenter );
        //
        QPushButton * open_btn = new QPushButton( tr( "Open" ) );
        open_btn->setAutoDefault( false );
        connect( open_btn, SIGNAL( clicked() ),
                 this, SLOT( open() ) );
        layout->addWidget( open_btn, 0, Qt::AlignVCenter );
        //
        QPushButton * save_btn = new QPushButton( tr( "Save" ) );
        save_btn->setAutoDefault( false );
        connect( save_btn, SIGNAL( clicked() ),
                 this, SLOT( save() ) );
        layout->addWidget( save_btn, 0, Qt::AlignVCenter );
        //
        top_layout->addLayout( layout );
    }

    // ball & playmode
    {
        QHBoxLayout * layout = new QHBoxLayout();

        layout->addWidget( createBallBox() );
        layout->addWidget( createPlayModeBox() );

        top_layout->addLayout( layout );
    }

    // playes
    {
        QHBoxLayout * layout = new QHBoxLayout();

        layout->addWidget( createLeftTeamBox() );
        layout->addWidget( createRightTeamBox() );

        top_layout->addLayout( layout );
    }

    // buttons
    {
        QHBoxLayout * layout = new QHBoxLayout();

        QPushButton * button = new QPushButton( tr( "Send Command!" ) );
        button->setAutoDefault( false );
        connect( button, SIGNAL( clicked() ),
                 this, SLOT( sendCommand() ) );
        layout->addWidget( button, 10, Qt::AlignVCenter );
        //
        QPushButton * cancel = new QPushButton( tr( "Close" ) );
        cancel->setAutoDefault( false );
        connect( cancel, SIGNAL( clicked() ),
                 this, SLOT( reject() ) );
        layout->addWidget( cancel, 2, Qt::AlignVCenter );

        top_layout->addLayout( layout );
    }

    this->setLayout( top_layout );
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
MonitorMoveDialog::createBallBox()
{
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setSizeConstraint( QLayout::SetFixedSize );
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 1 );

    QGridLayout * layout = new QGridLayout();
    layout->setSizeConstraint( QLayout::SetFixedSize );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );

    int row = 0;
    int col = 0;

    M_ball_cb = new QCheckBox();
    M_ball_cb->setChecked( false );
    connect( M_ball_cb, SIGNAL( toggled( bool ) ),
             this, SLOT( toggleBallCheck( bool ) ) );
    layout->addWidget( M_ball_cb, row, col++ );
    //
    layout->addWidget( new QLabel( tr( "X: " ) ), row, col++ );
    //
    M_ball_x = new QLineEdit( tr( "0.0" ) );
    M_ball_x->setEnabled( false );
    M_ball_x->setValidator( new QDoubleValidator( -57.5, 57.5, 3,
                                                  M_ball_x ) );
    M_ball_x->setMaximumSize( 64, 24 );
    layout->addWidget( M_ball_x, row, col++ );
    //
    layout->addWidget( new QLabel( tr( "Y: " ) ), row, col++ );
    //
    M_ball_y = new QLineEdit( tr( "0.0" ) );
    M_ball_y->setEnabled( false );
    M_ball_y->setValidator( new QDoubleValidator( -39.0, 39.0, 3,
                                                  M_ball_y ) );
    M_ball_y->setMaximumSize( 64, 24 );
    layout->addWidget( M_ball_y, row, col++ );

    row = 1;
    col = 0;

    M_ball_vel_cb = new QCheckBox();
    M_ball_vel_cb->setChecked( false );
    connect( M_ball_vel_cb, SIGNAL( toggled( bool ) ),
             this, SLOT( toggleBallVelCheck( bool ) ) );
    layout->addWidget( M_ball_vel_cb, row, col++ );
    //
    layout->addWidget( new QLabel( tr( "VX: " ) ), row, col++ );
    //
    M_ball_vx = new QLineEdit( tr( "0.0" ) );
    M_ball_vx->setEnabled( false );
    M_ball_vx->setValidator( new QDoubleValidator( -3.0, 3.0, 3,
                                                   M_ball_vx ) );
    M_ball_vx->setMaximumSize( 64, 24 );
    layout->addWidget( M_ball_vx, row, col++ );
    //
    layout->addWidget( new QLabel( tr( "VY: " ) ), row, col++ );
    //
    M_ball_vy = new QLineEdit( tr( "0.0" ) );
    M_ball_vy->setEnabled( false );
    M_ball_vy->setValidator( new QDoubleValidator( -3.0, 3.0, 3,
                                                  M_ball_vy ) );
    M_ball_vy->setMaximumSize( 64, 24 );
    layout->addWidget( M_ball_vy, row, col++ );

    top_layout->addLayout( layout );

    QGroupBox * group_box = new QGroupBox( tr( "Ball" ) );
    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
MonitorMoveDialog::createPlayModeBox()
{
    QHBoxLayout * top_layout = new QHBoxLayout();
    top_layout->setSizeConstraint( QLayout::SetFixedSize );
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 1 );

    M_drop_ball_rb = new QRadioButton( tr( "Drop" ) );
    //connect( drop, SIGNAL( clicked() ),
    //this, SLOT( clickDropBall() ) );
    top_layout->addWidget( M_drop_ball_rb );
    //
    M_free_kick_left_rb = new QRadioButton( tr( "Left" ) );
    //connect( left, SIGNAL( clicked() ),
    //this, SLOT( clickFreeKickLeft() ) );
    top_layout->addWidget( M_free_kick_left_rb );
    //
    M_free_kick_right_rb = new QRadioButton( tr( "Right" ) );
    //connect( right, SIGNAL( clicked() ),
    //this, SLOT( clickFreeKickRight() ) );
    top_layout->addWidget( M_free_kick_right_rb );


    M_drop_ball_rb->setChecked( true );

    QGroupBox * group_box = new QGroupBox( tr( "PlayMode" ) );
    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
MonitorMoveDialog::createLeftTeamBox()
{
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setSizeConstraint( QLayout::SetFixedSize );
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 1 );

    QSignalMapper * cb_mapper = new QSignalMapper( this );
    connect( cb_mapper, SIGNAL( mapped( int ) ),
             this, SLOT( toggleLeftCheck( int ) ) );

    QGridLayout * layout = new QGridLayout();
    layout->setSizeConstraint( QLayout::SetFixedSize );
    layout->setContentsMargins( 3, 3, 3, 3 );
    layout->setSpacing( 1 );
    {
        int col = 0;
        layout->addWidget( new QLabel( tr( " " ) ), 0, col++, Qt::AlignHCenter );
        M_left_all_cb = new QCheckBox();
        M_left_all_cb->setChecked( false );
        connect( M_left_all_cb, SIGNAL( toggled( bool ) ),
                 this, SLOT( toggleLeftAll( bool ) ) );
        layout->addWidget( M_left_all_cb, 0, col++, Qt::AlignHCenter );
        layout->addWidget( new QLabel( tr( "X" ) ), 0, col++, Qt::AlignHCenter );
        layout->addWidget( new QLabel( tr( "Y" ) ), 0, col++, Qt::AlignHCenter );
        layout->addWidget( new QLabel( tr( "Body" ) ), 0, col++, Qt::AlignHCenter );
    }

    for ( int i = 0; i < 11; ++i )
    {
        int col = 0;
        char buf[64];

        snprintf( buf, 64, "%2d:", i + 1 );
        layout->addWidget( new QLabel( QString::fromLatin1( buf ) ),
                           i + 1, col++ );
        //
        M_left_cb[i] = new QCheckBox();
        M_left_cb[i]->setChecked( false );
        connect( M_left_cb[i], SIGNAL( clicked() ),
                 cb_mapper, SLOT( map() ) );
        cb_mapper->setMapping( M_left_cb[i], i );
        layout->addWidget( M_left_cb[i], i + 1, col++ );
        //
        snprintf( buf, 64, "%7.3f", -3.0 * (i+1) );
        M_left_x[i] = new QLineEdit( QString::fromLatin1( buf ) );
        M_left_x[i]->setValidator( new QDoubleValidator( -57.5, 57.5, 3,
                                                         M_left_x[i] ) );
        M_left_x[i]->setMaximumSize( 64, 24 );
        layout->addWidget( M_left_x[i], i + 1, col++ );
        //
        M_left_y[i] = new QLineEdit( tr( "-37.000" ) );
        M_left_y[i]->setValidator( new QDoubleValidator( -39.0, 39.0, 3,
                                                         M_left_y[i] ) );
        M_left_y[i]->setMaximumSize( 64, 24 );
        layout->addWidget( M_left_y[i], i + 1, col++ );
        //
        M_left_body[i] = new QLineEdit( tr( "   0.000" ) );
        M_left_body[i]->setValidator( new QDoubleValidator( -180.0, 180.0, 3,
                                                            M_left_body[i] ) );
        M_left_body[i]->setMaximumSize( 64, 24 );
        layout->addWidget( M_left_body[i], i + 1, col++ );
    }

    toggleLeftAll( false );

    top_layout->addLayout( layout );

    QGroupBox * group_box = new QGroupBox( tr( "Left" ) );
    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
MonitorMoveDialog::createRightTeamBox()
{
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setSizeConstraint( QLayout::SetFixedSize );
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 1 );

    QSignalMapper * cb_mapper = new QSignalMapper( this );
    connect( cb_mapper, SIGNAL( mapped( int ) ),
             this, SLOT( toggleRightCheck( int ) ) );

    QGridLayout * layout = new QGridLayout();
    layout->setSizeConstraint( QLayout::SetFixedSize );
    layout->setContentsMargins( 3, 3, 3, 3  );
    layout->setSpacing( 1 );
    {
        int col = 0;
        layout->addWidget( new QLabel( tr( " " ) ), 0, col++, Qt::AlignHCenter );
        M_right_all_cb = new QCheckBox();
        M_right_all_cb->setChecked( false );
        connect( M_right_all_cb, SIGNAL( toggled( bool ) ),
                 this, SLOT( toggleRightAll( bool ) ) );
        layout->addWidget( M_right_all_cb, 0, col++, Qt::AlignHCenter );
        layout->addWidget( new QLabel( tr( "X" ) ), 0, col++, Qt::AlignHCenter );
        layout->addWidget( new QLabel( tr( "Y" ) ), 0, col++, Qt::AlignHCenter );
        layout->addWidget( new QLabel( tr( "Body" ) ), 0, col++, Qt::AlignHCenter );
    }

    for ( int i = 0; i < 11; ++i )
    {
        int col = 0;
        char buf[64];

        snprintf( buf, 64, "%2d:", i + 1 );
        layout->addWidget( new QLabel( QString::fromLatin1( buf ) ),
                           i + 1, col++ );
        //
        M_right_cb[i] = new QCheckBox();
        M_right_cb[i]->setChecked( true );
        connect( M_right_cb[i], SIGNAL( clicked() ),
                 cb_mapper, SLOT( map() ) );
        cb_mapper->setMapping( M_right_cb[i], i );
        layout->addWidget( M_right_cb[i], i + 1, col++ );
        //
        snprintf( buf, 64, "%7.3f", 3.0 * (i+1) );
        M_right_x[i] = new QLineEdit( QString::fromLatin1( buf ) );
        M_right_x[i]->setValidator( new QDoubleValidator( -57.5, 57.5, 3,
                                                          M_right_x[i] ) );
        M_right_x[i]->setMaximumSize( 64, 24 );
        layout->addWidget( M_right_x[i], i + 1, col++ );
        //
        M_right_y[i] = new QLineEdit( tr( "-37.000" ) );
        M_right_y[i]->setValidator( new QDoubleValidator( -39.0, 39.0, 3,
                                                          M_right_y[i] ) );
        M_right_y[i]->setMaximumSize( 64, 24 );
        layout->addWidget( M_right_y[i], i + 1, col++ );
        //
        M_right_body[i] = new QLineEdit( tr( " 180.000" ) );
        M_right_body[i]->setValidator( new QDoubleValidator( -180.0, 180.0, 3,
                                                             M_right_body[i] ) );
        M_right_body[i]->setMaximumSize( 64, 24 );
        layout->addWidget( M_right_body[i], i + 1, col++ );
    }

    toggleRightAll( false );

    top_layout->addLayout( layout );

    QGroupBox * group_box = new QGroupBox( tr( "Right" ) );
    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MonitorMoveDialog::toggleBallCheck( bool on )
{
    M_ball_x->setEnabled( on );
    M_ball_y->setEnabled( on );

    M_ball_vel_cb->setEnabled( on );
    toggleBallVelCheck( on );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MonitorMoveDialog::toggleBallVelCheck( bool on )
{
    M_ball_vx->setEnabled( on );
    M_ball_vy->setEnabled( on );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MonitorMoveDialog::toggleLeftAll( bool on )
{
    for ( int i = 0; i < 11; ++i )
    {
        M_left_cb[i]->setChecked( on );
        M_left_x[i]->setEnabled( on );
        M_left_y[i]->setEnabled( on );
        M_left_body[i]->setEnabled( on );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MonitorMoveDialog::toggleRightAll( bool on )
{
    for ( int i = 0; i < 11; ++i )
    {
        M_right_cb[i]->setChecked( on );
        M_right_x[i]->setEnabled( on );
        M_right_y[i]->setEnabled( on );
        M_right_body[i]->setEnabled( on );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MonitorMoveDialog::toggleLeftCheck( int index )
{
    if ( index < 0 || 11 < index )
    {
        return;
    }

    bool on = M_left_cb[index]->isChecked();

    M_left_x[index]->setEnabled( on );
    M_left_y[index]->setEnabled( on );
    M_left_body[index]->setEnabled( on );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MonitorMoveDialog::toggleRightCheck( int index )
{
    if ( index < 0 || 11 < index )
    {
        return;
    }

    bool on = M_right_cb[index]->isChecked();

    M_right_x[index]->setEnabled( on );
    M_right_y[index]->setEnabled( on );
    M_right_body[index]->setEnabled( on );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MonitorMoveDialog::readFieldStatus()
{
    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    if ( ! view )
    {
        return;
    }

    const bool reverse = Options::instance().reverseSide();
    const double rval =  Options::instance().reverseValue();

    // playmode
    if ( view->isLeftSetPlay() )
    {
        if ( reverse )
        {
            M_free_kick_right_rb->setChecked( true );
        }
        else
        {
            M_free_kick_left_rb->setChecked( true );
        }
    }
    else if ( view->isRightSetPlay() )
    {
        if ( reverse )
        {
            M_free_kick_left_rb->setChecked( true );
        }
        else
        {
            M_free_kick_right_rb->setChecked( true );
        }
    }
    else
    {
        M_drop_ball_rb->setChecked( true );
    }

    char buf[64];

    // ball
    if ( M_ball_cb->isChecked() )
    {
        snprintf( buf, 64, "%.3f", view->ball().x() * rval );
        M_ball_x->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, "%.3f", view->ball().y() * rval );
        M_ball_y->setText( QString::fromLatin1( buf ) );

        if ( M_ball_vel_cb->isChecked()
             && view->ball().hasVelocity() )
        {
            snprintf( buf, 64, "%.3f", view->ball().deltaX() * rval );
            M_ball_vx->setText( QString::fromLatin1( buf ) );

            snprintf( buf, 64, "%.3f", view->ball().deltaY() * rval );
            M_ball_vy->setText( QString::fromLatin1( buf ) );
        }
    }

    // players
    const std::vector< rcsc::rcg::PlayerT > & players = view->players();

    {
        const int start = ( reverse ? 11 : 0 );
        const int end = ( reverse ? 22 : 11 );
        int idx = 0;
        for ( int i = start; i < end; ++i, ++idx )
        {
            if ( ! M_left_cb[idx]->isChecked() ) continue;

            snprintf( buf, 64, "%.3f", players[i].x() * rval );
            M_left_x[idx]->setText( QString::fromLatin1( buf ) );

            snprintf( buf, 64, "%.3f", players[i].y() * rval );
            M_left_y[idx]->setText( QString::fromLatin1( buf ) );

            double body = players[i].body();
            if ( reverse )
            {
                body += 180.0;
                if ( body > 180.0 ) body -= 360.0;
            }
            snprintf( buf, 64, "%.3f", body );
            M_left_body[idx]->setText( QString::fromLatin1( buf ) );
        }
    }

    {
        const int start = ( reverse ? 0 : 11 );
        const int end = ( reverse ? 11 : 22 );
        int idx = 0;
        for ( int i = start; i < end; ++i, ++idx )
        {
            if ( ! M_right_cb[idx]->isChecked() ) continue;

            snprintf( buf, 64, "%.3f", players[i].x() * rval );
            M_right_x[idx]->setText( QString::fromLatin1( buf ) );

            snprintf( buf, 64, "%.3f", players[i].y() * rval );
            M_right_y[idx]->setText( QString::fromLatin1( buf ) );

            double body = players[i].body();
            if ( reverse )
            {
                body += 180.0;
                if ( body > 180.0 ) body -= 360.0;
            }
            snprintf( buf, 64, "%.3f", body );
            M_right_body[idx]->setText( QString::fromLatin1( buf ) );
        }
    }

}

/*-------------------------------------------------------------------*/
/*!

 */
void
MonitorMoveDialog::open()
{
    QString file_path
        = QFileDialog::getOpenFileName( this,
                                        tr( "Open" ) );

    if ( file_path.isEmpty() )
    {
        return;
    }

    std::string path = file_path.toStdString();
    std::ifstream fin( path.c_str() );

    if ( ! fin.is_open() )
    {
        std::cerr << __FILE__ << ": (open) "
                  << "Failed to open the file [" << path
                  << std::endl;
        return;
    }

    M_ball_cb->setChecked( false );
    M_ball_x->setEnabled( false );
    M_ball_y->setEnabled( false );
    M_drop_ball_rb->setChecked( true );

    for ( int i = 0; i < 11; ++i )
    {
        M_left_cb[i]->setChecked( false );
        M_left_x[i]->setEnabled( false );
        M_left_y[i]->setEnabled( false );
        M_left_body[i]->setEnabled( false );

        M_right_cb[i]->setChecked( false );
        M_right_x[i]->setEnabled( false );
        M_right_y[i]->setEnabled( false );
        M_right_body[i]->setEnabled( false );
    }

    int n_line = 0;
    std::string line_buf;
    while ( std::getline( fin, line_buf ) )
    {
        ++n_line;

        char str_id[32];
        int unum;
        char str_x[32];
        char str_y[32];
        char str_vx[32];
        char str_vy[32];
        char str_body[32];

        if ( std::sscanf( line_buf.c_str(),
                          " playmode %s ",
                          str_id ) == 1 )
        {
            if ( ! std::strcmp( str_id, "free_kick_l" ) )
            {
                M_free_kick_left_rb->setChecked( true );
            }
            else if ( ! std::strcmp( str_id, "free_kick_r" ) )
            {
                M_free_kick_right_rb->setChecked( true );
            }
            else
            {
                M_drop_ball_rb->setChecked( true );
            }
        }
        else if ( std::sscanf( line_buf.c_str(),
                               " ball %31s %31s %31s %31s  ",
                               str_x, str_y, str_vx, str_vy ) == 4 )
        {
            if ( str_x[0] == '*'
                 || str_y[0] == '*' )
            {

            }
            else
            {
                double x = std::strtod( str_x, NULL );
                double y = std::strtod( str_y, NULL );
                if ( x == -HUGE_VAL || x == HUGE_VAL
                     || y == -HUGE_VAL || y == HUGE_VAL )
                {
                    std::cerr << __FILE__ << ": (open) "
                              << "Illega data at line " << n_line
                              << " : [" << line_buf << "]"
                              << std::endl;
                    continue;
                }

                M_ball_cb->setChecked( true );
                M_ball_x->setEnabled( true );
                M_ball_y->setEnabled( true );
                M_ball_x->setText( QString::fromLatin1( str_x ) );
                M_ball_y->setText( QString::fromLatin1( str_y ) );

                if ( str_vx[0] != '*'
                     && str_vy[0] != '*' )
                {
                    double vx = std::strtod( str_vx, NULL );
                    double vy = std::strtod( str_vy, NULL );
                    if ( vx == -HUGE_VAL || vx == HUGE_VAL
                         || vy == -HUGE_VAL || vy == HUGE_VAL )
                    {
                        std::cerr << __FILE__ << ": (open) "
                                  << "Illega data at line " << n_line
                                  << " : [" << line_buf << "]"
                                  << std::endl;
                        continue;
                    }

                    M_ball_vel_cb->setChecked( true );
                    M_ball_vx->setEnabled( true );
                    M_ball_vy->setEnabled( true );
                    M_ball_vx->setText( QString::fromLatin1( str_vx ) );
                    M_ball_vy->setText( QString::fromLatin1( str_vy ) );
                }
            }
        }
        else if ( std::sscanf( line_buf.c_str(),
                               " %31s %d %31s %31s %31s ",
                               str_id, &unum, str_x, str_y, str_body )
                  == 5 )
        {
            rcsc::SideID side = rcsc::NEUTRAL;
            if ( ! std::strcmp( str_id, "left" ) )
            {
                side = rcsc::LEFT;
            }
            else if ( ! std::strcmp( str_id, "right" ) )
            {
                side = rcsc::RIGHT;
            }
            else
            {
                std::cerr << __FILE__ << ": (open) "
                          << "Illegal data at line " << n_line
                          << " : [" << line_buf << "]"
                          << std::endl;
                continue;
            }

            if ( unum < 1 || 11 < unum )
            {
                std::cerr << __FILE__ << ": (open) "
                          << "Illegal data at line " << n_line
                          << " : [" << line_buf << "]"
                          << std::endl;
                continue;
            }

            if ( str_x[0] == '*'
                 || str_y[0] == '*'
                 || str_body[0] == '*' )
            {
                continue;
            }

            double x = std::strtod( str_x, NULL );
            double y = std::strtod( str_y, NULL );
            double body = std::strtod( str_body, NULL );

            if ( x == -HUGE_VAL || x == HUGE_VAL
                 || y == -HUGE_VAL || y == HUGE_VAL
                 || body == -HUGE_VAL || body == HUGE_VAL )
            {
                std::cerr << __FILE__ << ": (open) "
                          << "Illega data at line " << n_line
                          << " : [" << line_buf << "]"
                          << std::endl;
                continue;
            }

            if ( side == rcsc::LEFT )
            {
                M_left_cb[unum-1]->setChecked( true );
                M_left_x[unum-1]->setEnabled( true );
                M_left_y[unum-1]->setEnabled( true );
                M_left_body[unum-1]->setEnabled( true );
                M_left_x[unum-1]->setText( QString::fromLatin1( str_x ) );
                M_left_y[unum-1]->setText( QString::fromLatin1( str_y ) );
                M_left_body[unum-1]->setText( QString::fromLatin1( str_body ) );
            }
            if ( side == rcsc::RIGHT )
            {
                M_right_cb[unum-1]->setChecked( true );
                M_right_x[unum-1]->setEnabled( true );
                M_right_y[unum-1]->setEnabled( true );
                M_right_body[unum-1]->setEnabled( true );
                M_right_x[unum-1]->setText( QString::fromLatin1( str_x ) );
                M_right_y[unum-1]->setText( QString::fromLatin1( str_y ) );
                M_right_body[unum-1]->setText( QString::fromLatin1( str_body ) );
            }
        }
        else
        {
            std::cerr << __FILE__ << ": (open) "
                      << "Illega data at line " << n_line
                      << " : [" << line_buf << "]"
                      << std::endl;
        }
    }

    fin.close();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MonitorMoveDialog::save()
{
    QString file_path
        = QFileDialog::getSaveFileName( this,
                                        tr( "Save" ) );

    if ( file_path.isEmpty() )
    {
        return;
    }

    std::string path = file_path.toStdString();
    std::ofstream fout( path.c_str() );

    if ( ! fout.is_open() )
    {
        std::cerr << __FILE__ << ": (save) "
                  << "Failed to open the file [" << path
                  << std::endl;
        return;
    }

    // playmode
    if ( M_free_kick_left_rb->isChecked() )
    {
        fout << "playmode free_kick_l\n";
    }
    else if ( M_free_kick_right_rb->isChecked() )
    {
        fout << "playmode free_kick_r\n";
    }
    else
    {
        fout << "playmode drop_ball\n";
    }

    // ball
    if ( M_ball_cb->isChecked() )
    {
        fout << "ball "
             << M_ball_x->text().toStdString() << ' '
             << M_ball_y->text().toStdString() << ' ';
        if ( M_ball_vel_cb->isChecked() )
        {
            fout << M_ball_vx->text().toStdString() << ' '
                 << M_ball_vy->text().toStdString() << '\n';
        }
        else
        {
            fout << "* *\n";
        }
    }
    else
    {
        fout << "ball * * * * \n";
    }

    // left
    for ( int i = 0; i < 11; ++i )
    {
        if ( M_left_cb[i]->isChecked() )
        {
            fout << "left " << i + 1 << ' '
                 << M_left_x[i]->text().toStdString() << ' '
                 << M_left_y[i]->text().toStdString() << ' '
                 << M_left_body[i]->text().toStdString() << '\n';
        }
        else
        {
            fout << "left " << i + 1 << " * * * \n";
        }
    }

    // right
    for ( int i = 0; i < 11; ++i )
    {
        if ( M_right_cb[i]->isChecked() )
        {
            fout << "right " << i + 1 << ' '
                 << M_right_x[i]->text().toStdString() << ' '
                 << M_right_y[i]->text().toStdString() << ' '
                 << M_right_body[i]->text().toStdString() << '\n';
        }
        else
        {
            fout << "right " << i + 1 << " * * * \n";
        }
    }

    fout << std::flush;
    fout.close();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MonitorMoveDialog::sendCommand()
{
    const bool reverse = Options::instance().reverseSide();

    {
        if ( M_free_kick_left_rb->isChecked() )
        {
            M_trainer_data.setPlayMode( reverse
                                        ? rcsc::PM_FreeKick_Right
                                        : rcsc::PM_FreeKick_Left );
        }
        else if ( M_free_kick_right_rb->isChecked() )
        {
            M_trainer_data.setPlayMode( reverse
                                        ? rcsc::PM_FreeKick_Left
                                        : rcsc::PM_FreeKick_Right );
        }
        else
        {
            M_trainer_data.setPlayMode( rcsc::PM_Drop_Ball );
        }
     }

    // ball
    {
        bool ok = false;
        rcsc::Vector2D pos( 0.0, 0.0 );
        rcsc::Vector2D vel( rcsc::Vector2D::INVALIDATED );

        if ( M_ball_cb->isChecked() )
        {
            pos.x = M_ball_x->text().toDouble( &ok );
            if ( ok ) pos.y = M_ball_y->text().toDouble( &ok );

            if ( ok
                 && M_ball_vel_cb->isChecked() )
            {
                vel.x = M_ball_vx->text().toDouble( &ok );
                if ( ok ) vel.y = M_ball_vy->text().toDouble( &ok );
            }
        }

        if ( ok )
        {
            if ( reverse )
            {
                pos *= -1.0;
                if ( vel.isValid() ) vel *= -1.0;
            }
            M_trainer_data.setBall( pos, vel );
        }
        else
        {
            M_trainer_data.setBall( rcsc::Vector2D::INVALIDATED );
        }
    }

    // left
    rcsc::SideID side = ( reverse
                          ? rcsc::RIGHT
                          : rcsc::LEFT );
    for ( int i = 0; i < 11; ++i )
    {
        bool ok = false;
        rcsc::Vector2D pos;
        double body = 0.0;

        if ( M_left_cb[i]->isChecked() )
        {
            pos.x = M_left_x[i]->text().toDouble( &ok );
            if ( ok ) pos.y = M_left_y[i]->text().toDouble( &ok );
            if ( ok ) body = M_left_body[i]->text().toDouble( &ok );
        }

        if ( ok )
        {
            if ( reverse )
            {
                pos *= -1.0;
                body += 180.0;
            }
            M_trainer_data.setPlayer( side, i + 1,
                                      pos, body );
        }
        else
        {
            M_trainer_data.setPlayer( side, i + 1,
                                      rcsc::Vector2D::INVALIDATED, body );
        }
    }

    // right
    side = ( reverse
             ? rcsc::LEFT
             : rcsc::RIGHT );
    for ( int i = 0; i < 11; ++i )
    {
        bool ok = false;
        rcsc::Vector2D pos;
        double body = 0.0;

        if ( M_right_cb[i]->isChecked() )
        {
            pos.x = M_right_x[i]->text().toDouble( &ok );
            if ( ok ) pos.y = M_right_y[i]->text().toDouble( &ok );
            if ( ok ) body = M_right_body[i]->text().toDouble( &ok );
        }

        if ( ok )
        {
            if ( reverse )
            {
                pos *= -1.0;
                body += 180.0;
            }
            M_trainer_data.setPlayer( side, i + 1,
                                      pos, body );
        }
        else
        {
            M_trainer_data.setPlayer( side, i + 1,
                                      rcsc::Vector2D::INVALIDATED, body );
        }
    }

    emit executed();
}
