// -*-c++-*-

/*!
  \file debug_message_window.cpp
  \brief debug management window class Source File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA, Hiroki SHIMORA

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

#include "debug_message_window.h"

#include "main_data.h"
#include "action_sequence_selector.h"
#include "debug_log_holder.h"
#include "debug_log_data.h"
#include "debug_log_dir_dialog.h"
#include "evaluator_control_panel.h"

#include "options.h"
#include "agent_id.h"

#include <rcsc/common/logger.h>
#include <rcsc/common/server_param.h>

#include <boost/shared_ptr.hpp>

// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

#include "xpm/debug_view_all.xpm"
#include "xpm/debug_view_self.xpm"
#include "xpm/debug_view_ball.xpm"
#include "xpm/debug_view_teammate.xpm"
#include "xpm/debug_view_opponent.xpm"
#include "xpm/debug_view_comment.xpm"
#include "xpm/debug_view_line.xpm"
#include "xpm/debug_view_target.xpm"
#include "xpm/debug_view_message.xpm"

#include "xpm/debug_log_view.xpm"

#include "xpm/open_dir.xpm"
#include "xpm/clear.xpm"
#include "xpm/refresh.xpm"

#include "xpm/sync.xpm"
#include "xpm/logplayer_one_step_back.xpm"
#include "xpm/logplayer_one_step_forward.xpm"

#include "xpm/num_1.xpm"
#include "xpm/num_2.xpm"
#include "xpm/num_3.xpm"
#include "xpm/num_4.xpm"
#include "xpm/num_5.xpm"
#include "xpm/num_6.xpm"
#include "xpm/num_7.xpm"
#include "xpm/num_8.xpm"
#include "xpm/num_9.xpm"
#include "xpm/num_10.xpm"
#include "xpm/num_11.xpm"
#include "xpm/num_12.xpm"
#include "xpm/num_13.xpm"
#include "xpm/num_14.xpm"
#include "xpm/num_15.xpm"
#include "xpm/num_16.xpm"
#include "xpm/num_17.xpm"
#include "xpm/num_18.xpm"
#include "xpm/num_19.xpm"
#include "xpm/num_20.xpm"
#include "xpm/num_21.xpm"
#include "xpm/num_22.xpm"
#include "xpm/num_23.xpm"
#include "xpm/num_24.xpm"
#include "xpm/num_25.xpm"
#include "xpm/num_26.xpm"
#include "xpm/num_27.xpm"
#include "xpm/num_28.xpm"
#include "xpm/num_29.xpm"
#include "xpm/num_30.xpm"
#include "xpm/num_31.xpm"
#include "xpm/num_32.xpm"

class TabWidget
    : public QTabWidget {

public:

    void selectIndex( int index )
      {
          static int last_index = -1;

          QTabBar * tab_bar = tabBar();
          if ( tab_bar )
          {
              if ( last_index != -1 )
              {
                  tab_bar->setTabTextColor( last_index,
                                            tab_bar->palette().color( QPalette::WindowText ) );
              }

              tab_bar->setTabTextColor( index, Qt::red );
              this->QTabWidget::setCurrentIndex( index );
              last_index = index;
          }
      }

};

/*-------------------------------------------------------------------*/
/*!

*/
DebugMessageWindow::DebugMessageWindow( QWidget * parent,
                                        MainData & main_data )
    : QMainWindow( parent ),
      M_main_data( main_data )
{
    this->setWindowTitle( tr( "Debug Message" ) );

    int i = -1;
    M_debug_text[++i] = tr( "System" );
    M_debug_text[++i] = tr( "Sensor" );
    M_debug_text[++i] = tr( "WM" );
    M_debug_text[++i] = tr( "Action" );
    M_debug_text[++i] = tr( "Intercept" );
    M_debug_text[++i] = tr( "Kick" );
    M_debug_text[++i] = tr( "Hold" );
    M_debug_text[++i] = tr( "Dribble" );
    M_debug_text[++i] = tr( "Pass" );
    M_debug_text[++i] = tr( "Cross" );
    M_debug_text[++i] = tr( "Shoot" );
    M_debug_text[++i] = tr( "Clear" );
    M_debug_text[++i] = tr( "Block" );
    M_debug_text[++i] = tr( "Mark" );
    M_debug_text[++i] = tr( "Position" );
    M_debug_text[++i] = tr( "Role" );
    M_debug_text[++i] = tr( "Team" );
    M_debug_text[++i] = tr( "Comm" );
    M_debug_text[++i] = tr( "Analyzer" );
    M_debug_text[++i] = tr( "ActSeq" );
    M_debug_text[++i] = tr( "Plan" );

    createActions();
    createMenus();
    createWindows();

    readSettings();

    this->resize( 800, 600 );

    // this->setWindowFlags( this->windowFlags() | Qt::WindowStaysOnTopHint );
}

/*-------------------------------------------------------------------*/
/*!

*/
DebugMessageWindow::~DebugMessageWindow()
{
    //std::cerr << "delete DebugMessageWindow" << std::endl;
    saveSettings();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::showEvent( QShowEvent * event )
{
    QMainWindow::showEvent( event );

    QTimer::singleShot( 100, this, SLOT( openDebugLogDir() ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::closeEvent( QCloseEvent * event )
{
    QMainWindow::closeEvent( event );

    M_action_sequence_selector->close();
}


/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::hideEvent( QHideEvent * event )
{
    QMainWindow::hideEvent( event );

    M_action_sequence_selector->close();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::readSettings()
{
#ifndef Q_WS_WIN
    QSettings settings( QDir::homePath() + "/.soccerwindow2",
                        QSettings::IniFormat );
#else
    QSettings settings( QDir::currentPath() + "/soccerwindow2.ini",
                        QSettings::IniFormat );
#endif
    //QSettings settings( QSettings::IniFormat,
    //                    QSettings::UserScope,
    //                    PACKAGE_NAME );

    settings.beginGroup( "DebugMessageWindow" );

    // read debug view switch status
    {
        QVariant val;

        val = settings.value( tr( "debug_view_all" ) );
        if ( val.isValid() )
        {
            M_show_debug_view_all_act->setChecked( val.toBool() );
            toggleShowDebugViewAll( val.toBool() );
        }

        val = settings.value( tr( "debug_view_self" ) );
        if ( val.isValid() )
        {
            M_show_debug_view_self_act->setChecked( val.toBool() );
            toggleShowDebugViewSelf( val.toBool() );
        }

        val = settings.value( tr( "debug_view_ball" ) );
        if ( val.isValid() )
        {
            M_show_debug_view_ball_act->setChecked( val.toBool() );
            toggleShowDebugViewBall( val.toBool() );
        }

        val = settings.value( tr( "debug_view_teammates" ) );
        if ( val.isValid() )
        {
            M_show_debug_view_teammates_act->setChecked( val.toBool() );
            toggleShowDebugViewTeammates( val.toBool() );
        }

        val = settings.value( tr( "debug_view_opponents" ) );
        if ( val.isValid() )
        {
            M_show_debug_view_opponents_act->setChecked( val.toBool() );
            toggleShowDebugViewOpponents( val.toBool() );
        }

        val = settings.value( tr( "debug_view_comment" ) );
        if ( val.isValid() )
        {
            M_show_debug_view_comment_act->setChecked( val.toBool() );
            toggleShowDebugViewComment( val.toBool() );
        }

        val = settings.value( tr( "debug_view_shape" ) );
        if ( val.isValid() )
        {
            M_show_debug_view_shape_act->setChecked( val.toBool() );
            toggleShowDebugViewShape( val.toBool() );
        }

        val = settings.value( tr( "debug_view_target" ) );
        if ( val.isValid() )
        {
            M_show_debug_view_target_act->setChecked( val.toBool() );
            toggleShowDebugViewTarget( val.toBool() );
        }

        val = settings.value( tr( "debug_view_message" ) );
        if ( val.isValid() )
        {
            M_show_debug_view_message_act->setChecked( val.toBool() );
            toggleShowDebugViewMessage( val.toBool() );
        }

        val = settings.value( tr( "debug_log_objects" ) );
        if ( val.isValid() )
        {
            M_show_debug_log_objects_act->setChecked( val.toBool() );
            toggleShowDebugLogObjects( val.toBool() );
        }

    }

    // read debug switch status
    for ( int i = 0; i < 32; ++i )
    {
        //snprintf( key, 16, "debug%02d", i + 1 );
        QVariant val = settings.value( QString( "debug%1" ).arg( i + 1, 2, 10, QChar( '0' ) ) );
        if ( val.isValid() )
        {
            bool enable = val.toBool();

            boost::int32_t level = rcsc::Logger::LEVEL_01 << i;
            M_main_data.setDebugLogLevel( level, enable );

            M_debug_level_act[i]->setChecked( enable );
        }
        else
        {
            M_debug_level_act[i]->setChecked( false );
        }
    }

    // read debug text string
    for ( int i = 0; i < 32; ++i )
    {
        //snprintf( key, 32, "debug_text_%02d", i + 1 );
        QVariant val = settings.value( QString( "debug_text_%1" ).arg( i + 1, 2, 10, QChar( '0' ) ) );
        if ( val.isValid() )
        {
            M_debug_text[i] = val.toString();
            //QString text = tr( "Level %1 " ).arg( i + 1 );
            //text += M_debug_text[i];
            //M_debug_level_act[i]->setText( text );
            //M_debug_level_act[i]->setStatusTip( tr( "Debug " ) + text );
            M_debug_level_act[i]->setText( M_debug_text[i] );
            M_debug_level_act[i]->setStatusTip( tr( "Debug " ) + M_debug_text[i] );
        }
    }

    settings.endGroup();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::saveSettings()
{
#ifndef Q_WS_WIN
    QSettings settings( QDir::homePath() + "/.soccerwindow2",
                        QSettings::IniFormat );
#else
    QSettings settings( QDir::currentPath() + "/soccerwindow2.ini",
                        QSettings::IniFormat );
#endif
    //QSettings settings( QSettings::IniFormat,
    //                    QSettings::UserScope,
    //                    PACKAGE_NAME );

    settings.beginGroup( "DebugMessageWindow" );

    // save debug view switches
    settings.setValue( tr( "debug_view_all" ),  M_show_debug_view_all_act->isChecked() );
    settings.setValue( tr( "debug_view_self" ), M_show_debug_view_self_act->isChecked() );
    settings.setValue( tr( "debug_view_ball" ), M_show_debug_view_ball_act->isChecked() );
    settings.setValue( tr( "debug_view_teammates" ), M_show_debug_view_teammates_act->isChecked() );
    settings.setValue( tr( "debug_view_opponents" ), M_show_debug_view_opponents_act->isChecked() );
    settings.setValue( tr( "debug_view_comment" ), M_show_debug_view_comment_act->isChecked() );
    settings.setValue( tr( "debug_view_shape" ), M_show_debug_view_shape_act->isChecked() );
    settings.setValue( tr( "debug_view_target" ), M_show_debug_view_target_act->isChecked() );
    settings.setValue( tr( "debug_view_message" ), M_show_debug_view_message_act->isChecked() );
    settings.setValue( tr( "debug_log_objects" ), M_show_debug_log_objects_act->isChecked() );

    // save debug level switches
    for ( int i = 0; i < 32; ++i )
    {
        //snprintf( key, 16, "debug%02d", i + 1 );
        settings.setValue( QString( "debug%1" ).arg( i + 1, 2, 10, QChar( '0' ) ),
                           M_debug_level_act[i]->isChecked() );
    }

    // save debug level text
    for ( int i = 0; i < 32; ++i )
    {
        if ( ! M_debug_text[i].isEmpty() )
        {
            //snprintf( key, 32, "debug_text_%02d", i + 1 );
            settings.setValue( QString( "debug_text_%1" ).arg( i + 1, 2, 10, QChar( '0' ) ),
                               M_debug_text[i] );
        }
    }

    settings.endGroup();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::createActions()
{
    // view switches

    M_show_debug_view_all_act = new QAction( QIcon( QPixmap( debug_view_all_xpm ) ),
                                             tr( "Toggle debug view" ), this );
    M_show_debug_view_all_act->setStatusTip( tr( "Show/Hide all debug view" ) );
    M_show_debug_view_all_act->setCheckable( true );
    M_show_debug_view_all_act->setChecked( true );
    connect( M_show_debug_view_all_act, SIGNAL( triggered( bool ) ),
             this, SLOT( toggleShowDebugViewAll( bool ) ) );

    M_show_debug_view_self_act = new QAction( QIcon( QPixmap( debug_view_self_xpm ) ),
                                              tr( "Toggle debug view self" ), this );
    M_show_debug_view_self_act->setStatusTip( tr( "Show/Hide debug view self" ) );
    M_show_debug_view_self_act->setCheckable( true );
    M_show_debug_view_self_act->setChecked( true );
    connect( M_show_debug_view_self_act, SIGNAL( triggered( bool ) ),
             this, SLOT( toggleShowDebugViewSelf( bool ) ) );

    M_show_debug_view_ball_act = new QAction( QIcon( QPixmap( debug_view_ball_xpm ) ),
                                              tr( "Toggle debug view ball" ), this );
    M_show_debug_view_ball_act->setStatusTip( tr( "Show/Hide debug view ball" ) );
    M_show_debug_view_ball_act->setCheckable( true );
    M_show_debug_view_ball_act->setChecked( true );
    connect( M_show_debug_view_ball_act, SIGNAL( triggered( bool ) ),
             this, SLOT( toggleShowDebugViewBall( bool ) ) );

    M_show_debug_view_teammates_act = new QAction( QIcon( QPixmap( debug_view_teammate_xpm ) ),
                                                   tr( "Toggle debug view teammates" ), this );
    M_show_debug_view_teammates_act->setStatusTip( tr( "Show/Hide debug view teammates" ) );
    M_show_debug_view_teammates_act->setCheckable( true );
    M_show_debug_view_teammates_act->setChecked( true );
    connect( M_show_debug_view_teammates_act, SIGNAL( triggered( bool ) ),
             this, SLOT( toggleShowDebugViewTeammates( bool ) ) );

    M_show_debug_view_opponents_act = new QAction( QIcon( QPixmap( debug_view_opponent_xpm ) ),
                                                   tr( "Toggle debug view opponents" ), this );
    M_show_debug_view_opponents_act->setStatusTip( tr( "Show/Hide debug view opponents" ) );
    M_show_debug_view_opponents_act->setCheckable( true );
    M_show_debug_view_opponents_act->setChecked( true );
    connect( M_show_debug_view_opponents_act, SIGNAL( triggered( bool ) ),
             this, SLOT( toggleShowDebugViewOpponents( bool ) ) );

    M_show_debug_view_comment_act = new QAction( QIcon( QPixmap( debug_view_comment_xpm ) ),
                                              tr( "Toggle debug view comment" ), this );
    M_show_debug_view_comment_act->setStatusTip( tr( "Show/Hide debug view comment" ) );
    M_show_debug_view_comment_act->setCheckable( true );
    M_show_debug_view_comment_act->setChecked( true );
    connect( M_show_debug_view_comment_act, SIGNAL( triggered( bool ) ),
             this, SLOT( toggleShowDebugViewComment( bool ) ) );

    M_show_debug_view_shape_act = new QAction( QIcon( QPixmap( debug_view_line_xpm ) ),
                                              tr( "Toggle debug view shape" ), this );
    M_show_debug_view_shape_act->setStatusTip( tr( "Show/Hide debug view shape" ) );
    M_show_debug_view_shape_act->setCheckable( true );
    M_show_debug_view_shape_act->setChecked( true );
    connect( M_show_debug_view_shape_act, SIGNAL( triggered( bool ) ),
             this, SLOT( toggleShowDebugViewShape( bool ) ) );

    M_show_debug_view_target_act = new QAction( QIcon( QPixmap( debug_view_target_xpm ) ),
                                              tr( "Toggle debug view target" ), this );
    M_show_debug_view_target_act->setStatusTip( tr( "Show/Hide debug view target" ) );
    M_show_debug_view_target_act->setCheckable( true );
    M_show_debug_view_target_act->setChecked( true );
    connect( M_show_debug_view_target_act, SIGNAL( triggered( bool ) ),
             this, SLOT( toggleShowDebugViewTarget( bool ) ) );

    M_show_debug_view_message_act = new QAction( QIcon( QPixmap( debug_view_message_xpm ) ),
                                              tr( "Toggle debug view message" ), this );
    M_show_debug_view_message_act->setStatusTip( tr( "Show/Hide debug view message" ) );
    M_show_debug_view_message_act->setCheckable( true );
    M_show_debug_view_message_act->setChecked( true );
    connect( M_show_debug_view_message_act, SIGNAL( triggered( bool ) ),
             this, SLOT( toggleShowDebugViewMessage( bool ) ) );
    //////////////////////////////////////////////
    // debug log view
    M_show_debug_log_objects_act = new QAction( QIcon( QPixmap( debug_log_view_xpm ) ),
                                                tr( "Toggle debug log objects" ),
                                                this );
    M_show_debug_log_objects_act->setStatusTip( tr( "Show/Hide debug log objects" ) );
    M_show_debug_log_objects_act->setCheckable( true );
    M_show_debug_log_objects_act->setChecked( true );
    connect( M_show_debug_log_objects_act, SIGNAL( triggered( bool ) ),
             this, SLOT( toggleShowDebugLogObjects( bool ) ) );

    //////////////////////////////////////////////
    // directry selection
    M_open_debug_log_dir_act = new QAction( QIcon( QPixmap( open_dir_xpm ) ),
                                            tr( "Open log dir..." ), this );
#ifdef Q_WS_MAC
    M_open_debug_log_dir_act->setShortcut( Qt::META + Qt::Key_O );
#else
    M_open_debug_log_dir_act->setShortcut( Qt::CTRL + Qt::Key_O );
#endif
    M_open_debug_log_dir_act->setStatusTip( tr( "Open the debug log directory" ) );
    connect( M_open_debug_log_dir_act, SIGNAL( triggered() ),
             this, SLOT( showDebugLogDirDialog() ) );
    //
    M_clear_data_act = new QAction( QIcon( QPixmap( clear_xpm ) ),
                                    tr( "Clear data" ), this );
    M_clear_data_act->setStatusTip( tr( "Clear all data" ) );
    connect( M_clear_data_act, SIGNAL( triggered() ),
             this, SLOT( clearAll() ) );

    //////////////////////////////////////////////
    // cycle control
    M_sync_act = new QAction( QIcon( QPixmap( sync_xpm ) ),
                              tr( "Sync" ), this );
#ifdef Q_WS_MAC
    M_sync_act->setShortcut( Qt::META + Qt::Key_S );
#else
    M_sync_act->setShortcut( Qt::CTRL + Qt::Key_S );
#endif
    M_sync_act->setStatusTip( tr( "Synchronize with field canvas" ) );
    connect( M_sync_act, SIGNAL( triggered() ),
             this, SLOT( syncAll() ) );

    M_decrement_act = new QAction( QIcon( QPixmap( logplayer_one_step_back_xpm ) ),
                              tr( "Decrement" ), this );
#ifdef Q_WS_MAC
    M_decrement_act->setShortcut( Qt::META + Qt::Key_Left );
#else
    M_decrement_act->setShortcut( Qt::CTRL + Qt::Key_Left );
#endif
    M_decrement_act->setStatusTip( tr( "Decrement message cycle" ) );
    connect( M_decrement_act, SIGNAL( triggered() ),
             this, SLOT( decrementCycle() ) );

    M_increment_act = new QAction( QIcon( QPixmap( logplayer_one_step_forward_xpm ) ),
                                   tr( "Increment" ), this );
#ifdef Q_WS_MAC
    M_increment_act->setShortcut( Qt::META + Qt::Key_Right );
#else
    M_increment_act->setShortcut( Qt::CTRL + Qt::Key_Right );
#endif
    M_increment_act->setStatusTip( tr( "Increment message cycle" ) );
    connect( M_increment_act, SIGNAL( triggered() ),
             this, SLOT( incrementCycle() ) );

    //
    // debug level switches
    //
    const char ** num_xpm[] = { num_1_xpm, num_2_xpm, num_3_xpm, num_4_xpm,
                                num_5_xpm, num_6_xpm, num_7_xpm, num_8_xpm,
                                num_9_xpm, num_10_xpm, num_11_xpm, num_12_xpm,
                                num_13_xpm, num_14_xpm, num_15_xpm, num_16_xpm,
                                num_17_xpm, num_18_xpm, num_19_xpm, num_20_xpm,
                                num_21_xpm, num_22_xpm, num_23_xpm, num_24_xpm,
                                num_25_xpm, num_26_xpm, num_27_xpm, num_28_xpm,
                                num_29_xpm, num_30_xpm, num_31_xpm, num_32_xpm };

    QSignalMapper * debug_level_mapper = new QSignalMapper( this );
    connect( debug_level_mapper, SIGNAL( mapped( int ) ),
             this, SLOT( toggleDebugLevel( int ) ) );

    for ( int i = 0; i < 32; ++i )
    {
        QString menu_text;
        if ( M_debug_text[i].isEmpty() )
        {
            menu_text = tr( "Level %1 " ).arg( i + 1 );
            M_debug_level_act[i] = new QAction( QIcon( QPixmap( num_xpm[i] ) ),
                                                menu_text, this );
        }
        else
        {
            menu_text = M_debug_text[i];
            M_debug_level_act[i] = new QAction( menu_text, this );
        }

        M_debug_level_act[i]->setStatusTip( tr( "Debug " ) + menu_text );
        M_debug_level_act[i]->setCheckable( true );

        connect( M_debug_level_act[i], SIGNAL( triggered() ),
                 debug_level_mapper, SLOT( map() ) );
        debug_level_mapper->setMapping( M_debug_level_act[i], i + 1 );
    }

    //
    // actions for training data
    //

    // M_intercept_ok_act = new QAction( tr( "InterceptOK" ), this );
    // connect( M_intercept_ok_act, SIGNAL( triggered() ),
    //          this, SLOT( onInterceptOK() ) );
    // M_intercept_ng_act = new QAction( tr( "InterceptNG" ), this );
    // connect( M_intercept_ng_act, SIGNAL( triggered() ),
    //          this, SLOT( onInterceptNG() ) );

    M_pass_request_move_ok_act = new QAction( tr( "PassReqMoveOK" ), this );
    connect( M_pass_request_move_ok_act, SIGNAL( triggered() ),
             this, SLOT( onPassRequestMoveOK() ) );
    M_pass_request_move_ng_act = new QAction( tr( "PassReqMoveNG" ), this );
    connect( M_pass_request_move_ng_act, SIGNAL( triggered() ),
             this, SLOT( onPassRequestMoveNG() ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::createMenus()
{
    {
        QMenu * menu = menuBar()->addMenu( "Data" );

        menu->addAction( M_open_debug_log_dir_act );
        menu->addAction( M_clear_data_act );

        menu->addAction( M_sync_act );
        menu->addAction( M_decrement_act );
        menu->addAction( M_increment_act );
    }

    {
        QMenu * menu = menuBar()->addMenu( "View" );

        menu->addAction( M_show_debug_view_all_act );
        menu->addAction( M_show_debug_view_self_act );
        menu->addAction( M_show_debug_view_ball_act );
        menu->addAction( M_show_debug_view_teammates_act );
        menu->addAction( M_show_debug_view_opponents_act );
        menu->addAction( M_show_debug_view_comment_act );
        menu->addAction( M_show_debug_view_shape_act );
        menu->addAction( M_show_debug_view_target_act );
        menu->addAction( M_show_debug_view_message_act );
        menu->addAction( M_show_debug_log_objects_act );
    }

    {
        QMenu * menu = menuBar()->addMenu( "Level" );
        for ( int i = 0; i < 32; ++i )
        {
            menu->addAction( M_debug_level_act[i] );
        }
    }

    menuBar()->hide();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::createWindows()
{
    //
    // central widget
    //

    M_tab_widget = new TabWidget();

    this->setCentralWidget( M_tab_widget );

    for ( int i = 0; i < 11; ++i )
    {
        M_message[i] = new QTextEdit();
        M_message[i]->setReadOnly( true );
        M_message[i]->setWordWrapMode( QTextOption::NoWrap );
        M_tab_widget->addTab( M_message[i],
                              QString( "   %1 " ).arg( i + 1, 2 ) );
    }

    {
        M_message[11] = new QTextEdit();
        M_message[11]->setReadOnly( true );
        M_message[11]->setWordWrapMode( QTextOption::NoWrap );
        M_tab_widget->addTab( M_message[11], tr( "Coach" ) );
    }

    M_tab_widget->selectIndex( 0 );

    // connect signals here in order to avoid redundunt sync
    connect( M_tab_widget, SIGNAL( currentChanged( int ) ),
             this, SLOT( changeCurrentTab( int ) ) );

    //
    // dialog
    //

    M_action_sequence_selector = new ActionSequenceSelector( this, M_main_data );

    // connect( M_evaluator_control_panel, SIGNAL( showSelector() ),
    //          M_action_sequence_selector, SLOT( show() ) );
    connect( M_action_sequence_selector, SIGNAL( selected( int ) ),
             this, SLOT( selectActionSequence( int ) ) );

    M_action_sequence_selector->hide();


    //
    // tool bar
    //
    createToolBar();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::createToolBar()
{
    createControlToolBar();
    createDebugViewToolBar();
    this->addToolBarBreak();
    createTrainingToolBar();

    createDebugLevelToolBar();

    createDebugEvaluatorToolBar();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::createControlToolBar()
{
    QToolBar * tbar = new QToolBar( tr( "Control" ), this );
    tbar->setIconSize( QSize( 16, 16 ) );

    M_find_box = new QLineEdit();
    M_find_box->setMaximumSize( 100, 48 );
    connect( M_find_box,  SIGNAL( returnPressed() ),
             this, SLOT( findExistString() ) );
    connect( M_find_box, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( findString( const QString & ) ) );

    // invisible action
    {
        QAction * act = new QAction( tr( "Focus Find Box" ), this );
#ifdef Q_WS_MAC
        act->setShortcut( Qt::META + Qt::Key_F );
#else
        act->setShortcut( Qt::CTRL + Qt::Key_F );
#endif
        connect( act, SIGNAL( triggered() ),
                 M_find_box, SLOT( setFocus() ) );
    }

    M_find_forward_rb = new QRadioButton( tr( "Down" ) );
    connect( M_find_forward_rb, SIGNAL( clicked() ),
             this, SLOT( findExistString() ) );
    M_find_backward_rb = new QRadioButton( tr( "Up" ) );
    connect( M_find_backward_rb, SIGNAL( clicked() ),
             this, SLOT( findExistString() ) );

    M_find_forward_rb->setChecked( true );


    QAction * run_offline_client_act = new QAction( QIcon( QPixmap( refresh_xpm ) ),
                                                    tr( "run offline client" ),
                                                    this );
    run_offline_client_act->setStatusTip( tr( "run offline client and reload log data" ) );
    run_offline_client_act->setToolTip( tr( "run offline client and reload log data" ) );
    connect( run_offline_client_act, SIGNAL( triggered() ),
             this, SLOT( runOfflineClient() ) );

    //
    //
    //

    tbar->addAction( M_open_debug_log_dir_act );
    tbar->addAction( M_clear_data_act );

    tbar->addAction( run_offline_client_act );

    tbar->addSeparator();

    tbar->addAction( M_sync_act );
    tbar->addAction( M_decrement_act );
    tbar->addAction( M_increment_act );

    tbar->addSeparator();

    tbar->addWidget( new QLabel( tr( "Find: " ) ) );
    tbar->addWidget( M_find_box );
    tbar->addWidget( M_find_forward_rb );
    tbar->addWidget( M_find_backward_rb );

    this->addToolBar( Qt::TopToolBarArea, tbar );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::createDebugViewToolBar()
{
    QToolBar * tbar = new QToolBar( tr( "Debug view" ), this );
    tbar->setIconSize( QSize( 16, 16 ) );

    tbar->addAction( M_show_debug_view_all_act );
    tbar->addAction( M_show_debug_view_self_act );
    tbar->addAction( M_show_debug_view_ball_act );
    tbar->addAction( M_show_debug_view_teammates_act );
    tbar->addAction( M_show_debug_view_opponents_act );
    tbar->addAction( M_show_debug_view_comment_act );
    tbar->addAction( M_show_debug_view_shape_act );
    tbar->addAction( M_show_debug_view_target_act );
    tbar->addAction( M_show_debug_view_message_act );

    tbar->addSeparator();

    tbar->addAction( M_show_debug_log_objects_act );

    this->addToolBar( Qt::TopToolBarArea, tbar );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::createDebugLevelToolBar()
{
    QToolBar * tbar = new QToolBar( tr( "Debug Level" ), this );
    tbar->setIconSize( QSize( 16, 16 ) );

    QToolBar * hbar = new QToolBar( tr( "Debug Level Hidden" ), this );
    hbar->setIconSize( QSize( 16, 16 ) );

    for ( int i = 0; i < 32; ++i )
    {
        if ( ! M_debug_text[i].isEmpty() )
        {
            tbar->addAction( M_debug_level_act[i] );
        }
        else
        {
            hbar->addAction( M_debug_level_act[i] );
        }
    }

    this->addToolBar( Qt::LeftToolBarArea, tbar );
    this->addToolBar( Qt::LeftToolBarArea, hbar );
    hbar->hide();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::createTrainingToolBar()
{
#if 0
    {
        QToolBar * tbar = new QToolBar( tr( "Training Intercept" ), this );
        tbar->setIconSize( QSize( 16, 16 ) );

        tbar->addAction( M_intercept_ok_act );
        tbar->addAction( M_intercept_ng_act );

        this->addToolBar( Qt::TopToolBarArea, tbar );
        tbar->hide();
    }
#endif
    {
        QToolBar * tbar = new QToolBar( tr( "Training PassRequestMove" ), this );
        tbar->setIconSize( QSize( 16, 16 ) );

        tbar->addAction( M_pass_request_move_ok_act );
        tbar->addAction( M_pass_request_move_ng_act );

        this->addToolBar( Qt::TopToolBarArea, tbar );
        //tbar->hide();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::createDebugEvaluatorToolBar()
{
    QToolBar * tbar = new QToolBar( tr( "FieldEvaluator Debug" ), this );

    QFrame * panel = new QFrame( this );
    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins( 2, 2, 2, 2 );
    panel->setLayout( layout );

    QPushButton * btn = new QPushButton( tr( "show action sequence selector" ) );
    connect( btn, SIGNAL( clicked() ),
             M_action_sequence_selector, SLOT( show() ) );
    layout->addWidget( btn, 1 );

    tbar->addWidget( panel );

    this->addToolBar( Qt::BottomToolBarArea, tbar );
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugMessageWindow::openDebugLogDir( const rcsc::SideID side,
                                     const std::string & dir_path )
{
    MonitorViewData::ConstPtr ptr = M_main_data.getCurrentViewData();
    if ( ! ptr )
    {
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "No view data!" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        std::cerr << __FILE__ << ": (openDebugLogDir) No current view data!"
                  << std::endl;
        return false;
    }


    if ( side == rcsc::LEFT )
    {
        if ( ptr->leftTeam().name().empty() )
        {
            QMessageBox::critical( this,
                                   tr( "Error" ),
                                   tr( "Empty team name!" ),
                                   QMessageBox::Ok, QMessageBox::NoButton );
            std::cerr << __FILE__ << ": (openDebugLogDir) Empty left team name!"
                      << std::endl;
            return false;
        }

        if ( M_main_data.setDebugLogDir( ptr->leftTeam().name(),
                                         side,
                                         M_tab_widget->currentIndex() + 1,
                                         dir_path ) )
        {
            syncCycle();
            return true;
        }
    }
    else if ( side == rcsc::RIGHT )
    {
        if ( ptr->rightTeam().name().empty() )
        {
            QMessageBox::critical( this,
                                   tr( "Error" ),
                                   tr( "Empty team name!" ),
                                   QMessageBox::Ok, QMessageBox::NoButton );
            std::cerr << __FILE__ << ": (openDebugLogDir) Empty right team name!"
                      << std::endl;
            return false;
        }

        if ( M_main_data.setDebugLogDir( ptr->rightTeam().name(),
                                         side,
                                         M_tab_widget->currentIndex() + 1,
                                         dir_path ) )
        {
            syncCycle();
            return true;
        }
    }

    // QMessageBox::critical( this,
    //                        tr( "Error" ),
    //                        tr( "No valid log data!\nMaybe need to run offline client." ),
    //                        QMessageBox::Ok, QMessageBox::NoButton );
    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::openDebugLogDir()
{
    const AgentID s = Options::instance().selectedAgent();
    if ( ! s.isNull() )
    {
        if ( ! Options::instance().debugLogDir().empty() )
        {
            changeCurrentTab( s.unum() - 1 );
            if ( ! openDebugLogDir( s.side(), Options::instance().debugLogDir() ) )
            {
                runOfflineClient();
            }
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::changeCurrentTab( int index )
{
    M_tab_widget->selectIndex( index );
    syncCycle();

    const AgentID current_agent = Options::instance().selectedAgent();
    if ( ! current_agent.isNull()
         && current_agent.unum() != index + 1 )
    {
        Options::instance().setSelectedNumber( current_agent.side(), index + 1 );
        Options::instance().setAgentSelectType( Options::SELECT_FIX );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::toggleShowDebugViewAll( bool checked )
{
    if ( Options::instance().showDebugView() != checked )
    {
        Options::instance().toggleShowDebugView();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::toggleShowDebugViewSelf( bool checked )
{
    if ( Options::instance().showDebugViewSelf() != checked )
    {
        Options::instance().toggleShowDebugViewSelf();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::toggleShowDebugViewBall( bool checked )
{
    if ( Options::instance().showDebugViewBall() != checked )
    {
        Options::instance().toggleShowDebugViewBall();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::toggleShowDebugViewTeammates( bool checked )
{
    if ( Options::instance().showDebugViewTeammates() != checked )
    {
        Options::instance().toggleShowDebugViewTeammates();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::toggleShowDebugViewOpponents( bool checked )
{
    if ( Options::instance().showDebugViewOpponents() != checked )
    {
        Options::instance().toggleShowDebugViewOpponents();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::toggleShowDebugViewComment( bool checked )
{
    if ( Options::instance().showDebugViewComment() != checked )
    {
        Options::instance().toggleShowDebugViewComment();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::toggleShowDebugViewShape( bool checked )
{
    if ( Options::instance().showDebugViewShape() != checked )
    {
        Options::instance().toggleShowDebugViewShape();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::toggleShowDebugViewTarget( bool checked )
{
    if ( Options::instance().showDebugViewTarget() != checked )
    {
        Options::instance().toggleShowDebugViewTarget();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::toggleShowDebugViewMessage( bool checked )
{
    if ( Options::instance().showDebugViewMessage() != checked )
    {
        Options::instance().toggleShowDebugViewMessage();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::toggleShowDebugLogObjects( bool checked )
{
    if ( Options::instance().showDebugLogObjects() != checked )
    {
        Options::instance().toggleShowDebugLogObjects();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::showDebugLogDirDialog()
{
    DebugLogDirDialog dlg( this );

    if ( ! dlg.exec() )
    {
        return;
    }

    rcsc::SideID side = dlg.side();
    QString dir_path = dlg.dirPath();

    std::string dir_path_string = dlg.dirPath().toStdString();
    std::cerr << __FILE__ << ": (showDebugLofDirDialog) select side = "
              << ( side == rcsc::LEFT ? "left" : "right" )
              << "  debug log dir = [" << dir_path_string << "]"
              << std::endl;

    Options::instance().setDebugLogDir( dir_path_string );

    openDebugLogDir( side, dir_path_string );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::syncCycle()
{
    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();
    if ( ! view )
    {
        //std::cerr << "sync debug cycle. no view data." << std::endl;
        return;
    }

    const int unum = M_tab_widget->currentIndex() + 1;

    std::cerr << __FILE__ << ": sync debug log."
              << " unum=" << unum
              << " time=" << view->time()
              << std::endl;

    if ( ! M_main_data.seekDebugLogData( unum, view->time() ) )
    {
        std::cerr << __FILE__ << ": (syncCycle) No data! unum = " << unum
                  << std::endl;
        return;
    }

    updateMessage();
    if ( M_action_sequence_selector->isVisible() )
    {
        M_action_sequence_selector->updateListView();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::decrementCycle()
{
    int unum = M_tab_widget->currentIndex() + 1;
    if ( ! M_main_data.decrementDebugLogCycle( unum ) )
    {
        return;
    }

    updateMessage();
    if ( M_action_sequence_selector->isVisible() )
    {
        M_action_sequence_selector->updateListView();
    }

    emit timeSelected( M_main_data.debugLogHolder().currentTime() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::incrementCycle()
{
    int unum = M_tab_widget->currentIndex() + 1;
    if ( ! M_main_data.incrementDebugLogCycle( unum ) )
    {
        return;
    }

    updateMessage();
    if ( M_action_sequence_selector->isVisible() )
    {
        M_action_sequence_selector->updateListView();
    }

    emit timeSelected( M_main_data.debugLogHolder().currentTime() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::findExistString()
{
    QString expr = M_find_box->text();
    if ( ! expr.isEmpty() )
    {
        findString( expr );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::findString( const QString & expr )
{
    // case insentive && no whole word
    QTextDocument::FindFlags flags( M_find_forward_rb->isChecked()
                                    ? 0
                                    : QTextDocument::FindBackward );
    QTextEdit * edit = M_message[ M_tab_widget->currentIndex() ];
    if ( ! edit->find( expr, flags ) )
    {
        if ( M_find_forward_rb->isChecked() )
        {
            edit->moveCursor( QTextCursor::Start );
        }
        else
        {
            edit->moveCursor( QTextCursor::End );
        }

        edit->find( expr, flags );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::toggleDebugLevel( int level )
{
    //M_level ^= DebugLogData::LEVEL_1; // reverse only 1 bit

    if ( level < 1 || 32 < level )
    {
        std::cerr << __FILE__ << ": (toggleDebugLevel) Invalid level "
                  << level << std::endl;
        return;
    }

    int level_flag = 1 << ( level - 1 );

    M_main_data.setDebugLogLevel( level_flag,
                                  M_debug_level_act[level-1]->isChecked() );

    updateMessage();
    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::onInterceptOK()
{
    saveInterceptDecision( true );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::onInterceptNG()
{
    saveInterceptDecision( false );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::onPassRequestMoveOK()
{
    saveTrainingDataPassRequestMove( true );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::onPassRequestMoveNG()
{
    saveTrainingDataPassRequestMove( false );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::clearAll()
{
    M_main_data.clearDebugLog();

    for ( int i = 0; i < 12; ++i )
    {
        M_message[i]->clear();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::updateMessage()
{
    const int unum = M_tab_widget->currentIndex() + 1;
    const boost::shared_ptr< const DebugLogData > data = M_main_data.debugLogHolder().getData( unum );
    if ( ! data )
    {
        //std::cerr << __FILE__ << ": (updateMessage) no data" << std::endl;
        return;
    }

    QTextEdit * text_edit = M_message[ M_tab_widget->currentIndex() ];

    // clear current data
    text_edit->clear();

    // QString cycle_line( "CYCLE " );
    // cycle_line += QString::number( data->time().cycle() );
    // cycle_line += tr( ", " );
    // cycle_line += QString::number( data->time().stopped() );
    // cycle_line += tr( " ------------------------------" );
    // text_edit->append( cycle_line );

    const boost::int32_t level = M_main_data.debugLogHolder().level();
    if ( level == 0 )
    {
        // std::cerr << __FILE__ << ": (updateMessage) no debug level" << std::endl;
        return;
    }

    int n_line = 0;
    QString main_buffer;
    main_buffer.reserve( 1000 * 256 );
    for ( DebugLogData::TextCont::const_iterator text_it = data->textCont().begin(),
              text_end = data->textCont().end();
          text_it != text_end;
          ++text_it )
    {
        // level check
        if ( level & text_it->level_ )
        {
            main_buffer += QString::fromStdString( text_it->msg_ );
            if ( ++n_line > 1000 )
            {
                // append message to text control
                text_edit->append( main_buffer );
                main_buffer.clear();
                n_line = 0;
            }
        }
    }

    if ( ! main_buffer.isEmpty() )
    {
        text_edit->append( main_buffer );
        main_buffer.clear();
    }

    QTextCursor cursor = text_edit->textCursor();
    cursor.setPosition( 0 );
    text_edit->setTextCursor( cursor );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::syncAll()
{
    syncCycle();

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
std::ostream *
DebugMessageWindow::openFile( const QString & filepath )
{
    QFileInfo finfo( filepath );

    if ( ! finfo.exists()
         || ! finfo.isWritable() )
    {
        return static_cast< std::ostream * >( 0 );
    }

    std::ofstream * fout = new std::ofstream( filepath.toStdString().c_str(), std::ios::app );

    return fout;
}

/*-------------------------------------------------------------------*/
/*!

*/
std::ostream *
DebugMessageWindow::createFile( const QString & filepath )
{
    std::ofstream * fout = new std::ofstream( filepath.toStdString().c_str() );
    if ( ! fout )
    {
        return static_cast< std::ostream * >( 0 );
    }

    if ( ! fout->is_open() )
    {
        delete fout;
        return static_cast< std::ostream * >( 0 );
    }

    return fout;
}

/*-------------------------------------------------------------------*/
/*!

*/
std::ostream *
DebugMessageWindow::openOrCreateInterceptDecisionFile()
{
    QString filepath = QString::fromStdString( Options::instance().interceptDecisionFile() );
    std::ostream * out = openFile( filepath );
    if ( out )
    {
        return out;
    }

    out = createFile( filepath );
#if 0
    if ( out )
    {
        //
        // decision for intercept or not
        //
        *fout << "@relation intercept_decision\n"
              << '\n'
            //<< "@attribute role {GK,DF,SW,CB,SB,MF,DH,OH,SH,FW,SF,CF}\n"
              << "@attribute my_step NUMERIC\n"
              << "@attribute teammate_step NUMERIC\n"
              << "@attribute teammate_accuracy NUMERIC\n"
              << "@attribute opponent_step NUMERIC\n"
              << "@attribute opponent_accuracy NUMERIC\n"
              << "@attribute my_pos_x NUMERIC\n"
              << "@attribute my_pos_y NUMERIC\n"
              << "@attribute ball_pos_x NUMERIC\n"
              << "@attribute ball_pos_y NUMERIC\n"
              << "@attribute my_trap_x NUMERIC\n"
              << "@attribute my_trap_y NUMERIC\n"
            // << "@attribute ball_speed NUMERIC\n"
            // << "@attribute ball_angle NUMERIC\n"
              << "@attribute class {Yes,No}\n"
              << '\n'
              << "@data\n";
    }
#endif
    return out;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::saveInterceptDecision( bool positive )
{
    const int unum = M_tab_widget->currentIndex() + 1;
    const boost::shared_ptr< const DebugLogData > data = M_main_data.debugLogHolder().getData( unum );
    if ( ! data )
    {
        std::cerr << __FILE__ << ": (saveInterceptDecision) no data" << std::endl;
        return;
    }

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();
    if ( ! view )
    {
        std::cerr << __FILE__ << ": (saveInterceptDecision) no view data" << std::endl;
        return;
    }

    if ( data->time() != view->time() )
    {
        std::cerr << __FILE__ << ": (saveInterceptDecision) time mismatched." << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "The time is not synchronized." ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    std::ostream * out = openOrCreateInterceptDecisionFile();
    if ( ! out
         || ! *out )
    {
        if ( out )
        {
            delete out;
            out = static_cast< std::ostream * >( 0 );
        }

        QString error_msg = tr( "Could not open the file. ");
        error_msg += QString::fromStdString( Options::instance().interceptDecisionFile() );
        QMessageBox::critical( this,
                               tr( "Error" ),
                               error_msg,
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    const boost::int32_t level = rcsc::Logger::INTERCEPT;

    for ( DebugLogData::TextCont::const_iterator text_it = data->textCont().begin(),
              text_end = data->textCont().end();
          text_it != text_end;
          ++text_it )
    {
        // level check
        // if ( text_it->level_ != rcsc::Logger::LEVEL_ANY
        //      && text_it->level_ & rcsc::Logger::INTERCEPT
        //      && text_it->level_ & rcsc::Logger::TABLE_LOG )
        if ( text_it->level_ == level )
        {
            for ( std::string::const_iterator c = text_it->msg_.begin(), c_end = text_it->msg_.end();
                  c != c_end;
                  ++c )
            {
                if ( *c != '\n' )
                {
                    *out << *c;
                }
            }
            *out << ( positive ? "Yes" : "No" ) << '\n';
        }
    }

    out->flush();
    delete out;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::saveTrainingDataPassRequestMove( const bool ok )
{
    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();
    if ( ! view )
    {
        return;
    }

    if ( view->time() != M_main_data.debugLogHolder().currentTime() )
    {
        syncCycle();
    }

    //
    // find the data
    //
    const int unum = M_tab_widget->currentIndex() + 1;
    const boost::shared_ptr< const DebugLogData > data = M_main_data.debugLogHolder().getData( unum );
    if ( ! data )
    {
        return;
    }

    std::string line;
    for ( DebugLogData::TextCont::const_iterator text_it = data->textCont().begin(),
              text_end = data->textCont().end();
          text_it != text_end;
          ++text_it )
    {
        if ( rcsc::Logger::TRAINING & text_it->level_ )
        {
            if ( text_it->msg_.compare( 0, 15, "PassRequestMove" ) == 0 )
            {
                line = text_it->msg_.substr( 15 );
                break;
            }
        }
    }

    //
    // open the file
    //
    QString filename = "PRM-";
    filename += QString::fromStdString( view->leftTeam().name() );
    filename += "-vs-";
    filename += QString::fromStdString( view->rightTeam().name() );
    filename += ".csv";

    QString filepath = QString::fromStdString( Options::instance().debugLogDir() );
    filepath += QDir::separator();
    filepath += filename;

    std::ofstream out( filepath.toStdString().c_str(), std::ios::app );

    if ( ! out
         || ! out.is_open() )
    {
        QString error_msg = tr( "Could not open the file. ");
        error_msg += filepath;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               error_msg,
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    //
    // append the new line
    //
    if ( ! line.empty() )
    {
        std::cerr << "save: " << filepath.toStdString() << std::endl;
        out << ( ok ? "1" : "0" ) << line << std::flush;
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::runOfflineClient()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );
    this->setEnabled( false );

    runOfflineClientImpl();

    this->setEnabled( true );
    QApplication::restoreOverrideCursor();

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::runOfflineClientImpl()
{
    const AgentID s = Options::instance().selectedAgent();

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    if ( s.isNull() )
    {
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "No agent selected" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    if ( ! view )
    {
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "No view found" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    const rcsc::rcg::TeamT & team = ( s.side() == rcsc::RIGHT
                                      ? view->rightTeam()
                                      : view->leftTeam() );

    if ( ! removeOldLogFile( team.name(), s.unum() ) )
    {
        return;
    }

    //
    // run offline client command
    //

    QString command = QString::fromStdString( s.side() == rcsc::RIGHT
                                              ? Options::instance().offlineTeamCommandRight()
                                              : Options::instance().offlineTeamCommandLeft() );

    if ( command.isEmpty() )
    {
        std::cerr << __FILE__ << ": (runOfflineClient) Empty team command.\n"
                  << "set the team script by '--offline-team-command-{left,right}'" << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "Empty team command!" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        command = QFileDialog::getOpenFileName( this, tr( "Select the team start script"),
                                                QDir::homePath(),
                                                tr( "Shell Script (*.sh);;all(*)") );
        if ( command.isEmpty() )
        {
            return;
        }
    }

    while ( 1 )
    {
        QStringList args;
        args << "--offline-client-mode"
             << "--foreground"
             << "--log-dir" << QString::fromStdString( Options::instance().debugLogDir() )
             << "--debug" << "--debug-server-logging"
             << "--unum" << QString::number( s.unum() )
             << "--teamname" << QString::fromStdString( team.name() );

        std::cerr << __FILE__ << ": (runOfflineClient)\n"
                  << " invoking command [" << command.toStdString() << " "
                  << args.join( " " ).toStdString() << "]" << std::endl;

        if ( QProcess::execute( command, args ) == 0 )
        {
            break;
        }

        // std::cerr << __FILE__ << ": (runOfflineClient) "
        //           << "command execution failed [" << command.toStdString() << " "
        //           << args.join( " " ).toStdString() << "]" << std::endl;

        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "Command execution failed[" )
                               + command + tr( " " ) + args.join( " " )
                               + tr( "]" ),
                               QMessageBox::Ok, QMessageBox::NoButton );

        command = QFileDialog::getOpenFileName( this, tr( "Select the team start script"),
                                                QDir::homePath(),
                                                tr( "Shell Script (*.sh);;all(*)") );
        if ( command.isEmpty() )
        {
            return;
        }
    }


    //
    // reload log file
    //
    if ( ! Options::instance().debugLogDir().empty() )
    {
        openDebugLogDir( s.side(), Options::instance().debugLogDir() );
        M_main_data.openDebugView( Options::instance().debugLogDir() );
    }

    if ( s.side() == rcsc::LEFT )
    {
        std::cerr << "setOfflineTeamCommandLeft "
                  << command.toStdString() << std::endl;
        Options::instance().setOfflineTeamCommandLeft( command.toStdString() );
    }
    else if ( s.side() == rcsc::RIGHT )
    {
        Options::instance().setOfflineTeamCommandRight( command.toStdString() );
    }

}

/*-------------------------------------------------------------------*/
/*!

*/
QString
DebugMessageWindow::getLogFilePath( const std::string & teamname,
                                    const int unum )
{
    QString filepath = QString::fromStdString( Options::instance().debugLogDir() );
    filepath += '/';
    filepath += QString::fromStdString( teamname );
    filepath += '-';
    filepath += QString::number( unum );
    filepath += ".log";

    return filepath;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugMessageWindow::removeOldLogFile( const std::string & teamname,
                                      const int unum )
{
    QString filepath = getLogFilePath( teamname, unum );

    if ( QFile::exists( filepath )
         && ! QFile::remove( filepath ) )
    {
        // std::cerr << __FILE__ << ": (runOfflineClient) "
        //           << "Cannot remove the file [" << filepath.toStdString()
        //           << "]" << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "Cannot remove the file [" )
                               + filepath
                               + tr( "]" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugMessageWindow::existsLogFile( const std::string & teamname,
                                   const int unum )
{
    QString filepath = getLogFilePath( teamname, unum );

    return QFile::exists( filepath );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugMessageWindow::selectActionSequence( int id )
{
    // std::cerr << "(DebugMessageWindow::selectActionSequence) id = " << id << std::endl;
    M_main_data.setActionSequenceSelection( id );

    emit configured();
}
