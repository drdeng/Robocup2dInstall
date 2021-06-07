// -*-c++-*-

/*!
  \file main_window.cpp
  \brief main application window class Source File.
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

#include "main_window.h"

#include "color_setting_dialog.h"
#include "image_save_dialog.h"
#include "detail_dialog.h"
#include "font_setting_dialog.h"
#include "monitor_move_dialog.h"
#include "player_type_dialog.h"
#include "view_config_dialog.h"
#include "debug_message_window.h"
#include "field_canvas.h"
#include "debug_server.h"
#include "monitor_client.h"
#include "launcher_dialog.h"
#include "log_player.h"
#include "log_player_tool_bar.h"
#include "dir_selector.h"

#include "options.h"
#include "grid_field_evaluation_data.h"

#include <rcsc/common/server_param.h>

#include <boost/shared_ptr.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <set>
#include <limits>

#include <csignal> // ::kill()
#include <cstdlib> // system()
#include <sys/types.h> // SIGINT


#include "xpm/soccerwindow2.xpm"
#include "xpm/soccerwindow2-nostr.xpm"

#include "xpm/open_rcg.xpm"
#include "xpm/save.xpm"

#include "xpm/logplayer_one_step_back.xpm"
#include "xpm/logplayer_one_step_forward.xpm"
#include "xpm/logplayer_play_or_stop.xpm"
#include "xpm/logplayer_stop.xpm"
#include "xpm/logplayer_play_back.xpm"
#include "xpm/logplayer_play_forward.xpm"
#include "xpm/logplayer_go_prev_score.xpm"
#include "xpm/logplayer_go_next_score.xpm"
#include "xpm/logplayer_go_first.xpm"
#include "xpm/logplayer_go_last.xpm"
#include "xpm/logplayer_shift_down.xpm"
#include "xpm/logplayer_shift_up.xpm"

#include "xpm/logplayer_live_mode.xpm"
#include "xpm/monitor_move_player.xpm"
#include "xpm/debug_server_switch.xpm"

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "soccerwindow2"
#endif

/*-------------------------------------------------------------------*/
/*!

*/
MainWindow::MainWindow()
    : M_log_player( new LogPlayer( M_main_data, this ) ),
      M_detail_dialog( static_cast< DetailDialog * >( 0 ) ),
      M_player_type_dialog( static_cast< PlayerTypeDialog * >( 0 ) ),
      M_monitor_move_dialog( static_cast< MonitorMoveDialog * >( 0 ) ),
      M_view_config_dialog( static_cast< ViewConfigDialog * >( 0 ) ),
      M_launcher_dialog( static_cast< LauncherDialog * >( 0 ) ),
      M_debug_message_window( static_cast< DebugMessageWindow * >( 0 ) ),
      M_monitor_client( static_cast< MonitorClient * >( 0 ) ),
      M_debug_server( static_cast< DebugServer * >( 0 ) ),
      M_last_connected_host( "127.0.0.1" )
{
    readSettings();

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    // central widget
    createFieldCanvas();
    // control dialogs
    createViewConfigDialog();

    connect( M_log_player, SIGNAL( updated() ),
             this, SIGNAL( viewUpdated() ) );

    this->setWindowIcon( QIcon( QPixmap( soccerwindow2_xpm ) ) );
    this->setWindowTitle( tr( PACKAGE_NAME ) );

    this->setMinimumSize( 280, 220 );
    this->resize( Options::instance().frameWidth() > 0
                  ? Options::instance().frameWidth()
                  : 640,
                  Options::instance().frameHeight() > 0
                  ? Options::instance().frameHeight()
                  : 480 );

    this->move( Options::instance().framePosX() >= 0
                ? Options::instance().framePosX()
                : this->x(),
                Options::instance().framePosY() >= 0
                ? Options::instance().framePosY()
                : this->y() );

    // this->setWindowOpacity( 0.5 ); // window transparency

    this->setAcceptDrops( true );

    if ( Options::instance().hideToolBar() )
    {
        M_log_player_tool_bar->hide();
        //M_monitor_tool_bar->hide();
    }

    if ( Options::instance().hideStatusBar() )
    {
        this->statusBar()->hide();
    }

    if ( Options::instance().hideMenuBar() )
    {
        this->menuBar()->hide();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
MainWindow::~MainWindow()
{
    //std::cerr << "delete MainWindow" << std::endl;

    if ( Options::instance().killServer() )
    {
        killServer();
    }
    saveSettings();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::init()
{
    if ( ! Options::instance().drawDataFile().empty() )
    {
        openDrawData( QString::fromStdString( Options::instance().drawDataFile() ) );
    }

    if ( ! Options::instance().gameLogFilePath().empty() )
    {
        openRCG( QString::fromStdString( Options::instance().gameLogFilePath() ) );
    }
    else if ( Options::instance().connect() )
    {
        std::string host = Options::instance().host();
        if ( host.empty() )
        {
            host = "127.0.0.1";
        }

        connectMonitorTo( host.c_str() );

        if ( Options::instance().debugServerMode() )
        {
            startDebugServer();
        }
    }

    if ( Options::instance().fullScreen() )
    {
        this->showFullScreen();
    }
    else if ( Options::instance().maximize() )
    {
        this->showMaximized();
    }
    else if ( Options::instance().canvasWidth() > 0
              && Options::instance().canvasHeight() > 0 )
    {
        resizeCanvas( QSize( Options::instance().canvasWidth(),
                             Options::instance().canvasHeight() ) );
    }


    if ( QApplication::setStyle( "plastique" ) )
    {
        Q_FOREACH( QAction * action, M_style_act_group->actions() )
        {
            if ( action->data().toString().toLower()
                 == QApplication::style()->objectName().toLower() )
            {
                action->setChecked( true );
                break;
            }
        }
    }

    if ( Options::instance().autoImageSave() )
    {
        QTimer::singleShot( 500,
                            this, SLOT( saveImageAndQuit() ) );
        this->setEnabled( false );
    }

    M_debug_message_window = new DebugMessageWindow( this,
                                                     M_main_data );
    connect( M_debug_message_window, SIGNAL( configured() ),
             this, SIGNAL( viewUpdated() ) );
    connect( M_debug_message_window, SIGNAL( timeSelected( const rcsc::GameTime & ) ),
             M_log_player, SLOT( goToCycle( const rcsc::GameTime & ) ) );

    M_debug_message_window->hide();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::readSettings()
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
    //QSettings settings( QSettings::NativeFormat,
    //                    QSettings::UserScope,
    //                    "rctools",
    //                    PACKAGE_NAME );

    settings.beginGroup( "Options" );

    if ( Options::instance().framePosX() < 0
         || Options::instance().framePosY() < 0 )
    {
        Options::instance().setFramePos( settings.value( "framePosX", -1 ).toInt(),
                                         settings.value( "framePosY", -1 ).toInt() );
    }

    if ( Options::instance().frameWidth() < 0
         || Options::instance().frameHeight() < 0 )
    {
        Options::instance().setFrameSize( settings.value( "frameWidth", -1 ).toInt(),
                                          settings.value( "frameHeight", -1 ).toInt() );
    }

    Options::instance().setTeamGraphicScale( settings.value( "team_graphic_scale", 1.0 ).toDouble() );

    if ( Options::instance().gameLogDir().empty() )
    {
        Options::instance().setGameLogDir( settings.value( "gameLogDir", "" )
                                           .toString()
                                           .toStdString() );
    }

    if ( Options::instance().debugLogDir().empty() )
    {
        Options::instance().setDebugLogDir( settings.value( "debugLogDir", "" )
                                            .toString()
                                            .toStdString() );
    }

    if ( Options::instance().offlineTeamCommandLeft().empty() )
    {
        Options::instance().setOfflineTeamCommandLeft( settings.value( "offline_team_command_left", "" )
                                                       .toString()
                                                       .toStdString() );
    }

    if ( Options::instance().offlineTeamCommandRight().empty() )
    {
        Options::instance().setOfflineTeamCommandRight( settings.value( "offline_team_command_right", "" )
                                                        .toString()
                                                        .toStdString() );
    }

    if ( Options::instance().evaluatorCommandLeft().empty() )
    {
        Options::instance().setEvaluatorCommandLeft( settings.value( "evaluator_command_left", "" )
                                                     .toString()
                                                     .toStdString() );
    }

    if ( Options::instance().evaluatorCommandRight().empty() )
    {
        Options::instance().setEvaluatorCommandRight( settings.value( "evaluator_command_right", "" )
                                                      .toString()
                                                      .toStdString() );
    }

    if ( Options::instance().evaluatorName().empty() )
    {
        Options::instance().setEvaluatorName( settings.value( "evaluator_name", "" )
                                              .toString()
                                              .toStdString() );
    }

    if ( Options::instance().evaluatorParamFileLeft().empty() )
    {
        Options::instance().setEvaluatorParamFileLeft( settings.value( "evaluator_param_file_left", "" )
                                                       .toString()
                                                       .toStdString() );
    }

    if ( Options::instance().evaluatorParamFileRight().empty() )
    {
        Options::instance().setEvaluatorParamFileRight( settings.value( "evaluator_param_file_right", "" )
                                                        .toString()
                                                        .toStdString() );
    }


    settings.endGroup();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::saveSettings()
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
    //QSettings settings( QSettings::NativeFormat,
    //                    QSettings::UserScope,
    //                    "rctools",
    //                    PACKAGE_NAME );

    settings.beginGroup( "Options" );

    const Options & o = Options::instance();

    settings.setValue( "framePosX", this->x() );
    settings.setValue( "framePosY", this->y() );
    settings.setValue( "frameWidth", this->width() );
    settings.setValue( "frameHeight", this->height() );

    settings.setValue( "team_graphic_scale", o.teamGraphicScale() );

    settings.setValue( "gameLogDir", QString::fromStdString( o.gameLogDir() ) );
    settings.setValue( "debugLogDir", QString::fromStdString( o.debugLogDir() ) );

    settings.setValue( "offline_team_command_left", QString::fromStdString( o.offlineTeamCommandLeft() ) );
    settings.setValue( "offline_team_command_right", QString::fromStdString( o.offlineTeamCommandRight() ) );
    settings.setValue( "evaluator_command_left", QString::fromStdString( o.evaluatorCommandLeft() ) );
    settings.setValue( "evaluator_command_right", QString::fromStdString( o.evaluatorCommandRight() ) );
    settings.setValue( "evaluator_name", QString::fromStdString( o.evaluatorName() ) );
    settings.setValue( "evaluator_param_file_left", QString::fromStdString( o.evaluatorParamFileLeft() ) );
    settings.setValue( "evaluator_param_file_right", QString::fromStdString( o.evaluatorParamFileRight() ) );

    settings.endGroup();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createActions()
{
    createActionsFile();
    createActionsMonitor();
    createActionsView();
    createActionsViewConfig();
    createActionsLogPlayer();
    createActionsDebug();
    createActionsHelp();

}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createActionsFile()
{
    M_open_rcg_act = new QAction( QIcon( QPixmap( open_rcg_xpm ) ),
                                  tr( "&Open rcg file..." ), this );
#ifdef Q_WS_MAC
    M_open_rcg_act->setShortcut( Qt::META + Qt::Key_O );
#else
    M_open_rcg_act->setShortcut( Qt::CTRL + Qt::Key_O );
#endif
    M_open_rcg_act->setStatusTip( tr( "Open RoboCup Game Log file" ) );
    connect( M_open_rcg_act, SIGNAL( triggered() ), this, SLOT( openRCG() ) );
    this->addAction( M_open_rcg_act );
    //
    M_save_rcg_act = new QAction( QIcon( QPixmap( save_xpm ) ),
                                  tr( "Save rcg file as..." ), this );
    M_save_rcg_act->setStatusTip( tr( "Save RoboCup Game Log file" ) );
    connect( M_save_rcg_act, SIGNAL( triggered() ), this, SLOT( saveRCG() ) );
    this->addAction( M_save_rcg_act );
    //
    M_open_debug_view_act = new QAction( QIcon( QPixmap( open_rcg_xpm ) ),
                                         tr( "Open debug view" ), this );
#ifdef Q_WS_MAC
    M_open_debug_view_act->setShortcut( Qt::META + Qt::SHIFT + Qt::Key_O );
#else
    M_open_debug_view_act->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_O );
#endif
    M_open_debug_view_act->setStatusTip( tr( "Open the directory where debug view logs exist" ) );
    connect( M_open_debug_view_act, SIGNAL( triggered() ),
             this, SLOT( openDebugView() ) );
    this->addAction( M_open_debug_view_act );
    //
    M_save_debug_view_act = new QAction( QIcon( QPixmap( save_xpm ) ),
                                         tr( "Save debug view" ), this );
    M_save_debug_view_act->setStatusTip( tr( "Save debug view logs to the directory..." ) );
    connect( M_save_debug_view_act, SIGNAL( triggered() ),
             this, SLOT( saveDebugView() ) );
    this->addAction( M_save_debug_view_act );
    //
    M_open_draw_data_act = new QAction( QIcon( QPixmap( open_rcg_xpm ) ),
                                         tr( "Open draw data" ), this );
    M_open_draw_data_act->setStatusTip( tr( "Open draw data file" ) );
    connect( M_open_draw_data_act, SIGNAL( triggered() ), this, SLOT( openDrawData() ) );
    this->addAction( M_open_draw_data_act );
    //
    M_show_image_save_dialog_act = new QAction( tr( "Save &Image" ), this );
    M_show_image_save_dialog_act->setStatusTip( tr( "Save game log data as image files" ) );
    connect( M_show_image_save_dialog_act, SIGNAL( triggered() ),
             this, SLOT( showImageSaveDialog() ) );
    this->addAction( M_show_image_save_dialog_act );
    //
    M_exit_act = new QAction( tr( "&Quit" ), this );
#ifdef Q_WS_MAC
    M_exit_act->setShortcut( Qt::META + Qt::Key_Q );
#else
    M_exit_act->setShortcut( Qt::CTRL + Qt::Key_Q );
#endif
    M_exit_act->setStatusTip( tr( "Exit the application." ) );
    connect( M_exit_act, SIGNAL( triggered() ), this, SLOT( close() ) );
    this->addAction( M_exit_act );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createActionsMonitor()
{
    M_kick_off_act = new QAction( tr( "&KickOff" ), this );
#ifdef Q_WS_MAC
    M_kick_off_act->setShortcut( Qt::META + Qt::Key_K );
#else
    M_kick_off_act->setShortcut( Qt::CTRL + Qt::Key_K );
#endif
    M_kick_off_act->setStatusTip( tr( "Start the game" ) );
    M_kick_off_act->setEnabled( false );
    connect( M_kick_off_act, SIGNAL( triggered() ), this, SLOT( kickOff() ) );
    this->addAction( M_kick_off_act );
    //
    M_set_live_mode_act = new QAction( QIcon( QPixmap( logplayer_live_mode_xpm ) ),
                                       tr( "&Live Mode" ), this );
#ifdef Q_WS_MAC
    M_set_live_mode_act->setShortcut( Qt::META + Qt::Key_L );
#else
    M_set_live_mode_act->setShortcut( Qt::CTRL + Qt::Key_L );
#endif
    M_set_live_mode_act->setStatusTip( tr( "set monitor to live mode" ) );
    M_set_live_mode_act->setEnabled( false );
    connect( M_set_live_mode_act, SIGNAL( triggered() ),
             this, SLOT( setLiveMode() ) );
    this->addAction( M_set_live_mode_act );
    //
    M_connect_monitor_act = new QAction( tr( "&Connect" ), this );
#ifdef Q_WS_MAC
    M_connect_monitor_act->setShortcut( Qt::META + Qt::Key_C );
#else
    M_connect_monitor_act->setShortcut( Qt::CTRL + Qt::Key_C );
#endif
    M_connect_monitor_act->setStatusTip( "Connect to the rcssserver on localhost" );
    M_connect_monitor_act->setEnabled( true );
    connect( M_connect_monitor_act, SIGNAL( triggered() ),
             this, SLOT( connectMonitor() ) );
    this->addAction( M_connect_monitor_act );
    //
    M_connect_monitor_to_act = new QAction( tr( "Connect &to ..." ), this );
    M_connect_monitor_to_act->setStatusTip( tr( "Connect to the rcssserver on other host" ) );
    M_connect_monitor_to_act->setEnabled( true );
    connect( M_connect_monitor_to_act, SIGNAL( triggered() ),
             this, SLOT( connectMonitorTo() ) );
    this->addAction( M_connect_monitor_to_act );
    //
    M_disconnect_monitor_act = new QAction( tr( "&Disconnect" ), this );
    M_disconnect_monitor_act->setStatusTip( tr( "Disonnect from rcssserver" ) );
    M_disconnect_monitor_act->setEnabled( false );
    connect( M_disconnect_monitor_act, SIGNAL( triggered() ),
             this, SLOT( disconnectMonitor() ) );
    this->addAction( M_disconnect_monitor_act );
    //
#ifndef Q_WS_WIN
    M_kill_server_act = new QAction( tr( "&Kill server" ), this );
    M_kill_server_act->setStatusTip( tr( "Kill the rcssserver process" ) );
    M_kill_server_act->setEnabled( false );
    connect( M_kill_server_act, SIGNAL( triggered() ),
             this, SLOT( killServer() ) );
    this->addAction( M_kill_server_act );
    //
    M_restart_server_act = new QAction( tr( "(Re)&start server" ), this );
    M_restart_server_act->setStatusTip( tr( "(Re)start rcssserver" ) );
    connect( M_restart_server_act, SIGNAL( triggered() ),
             this, SLOT( restartServer() ) );
    this->addAction( M_restart_server_act );
#endif
    //
    M_toggle_drag_move_mode_act = new QAction( QIcon( QPixmap( monitor_move_player_xpm ) ),
                                               tr( "Drag Move Mode." ),
                                               this );
    M_toggle_drag_move_mode_act->setStatusTip( tr( "Toggle drag move mode." ) );
    M_toggle_drag_move_mode_act->setEnabled( false );
    M_toggle_drag_move_mode_act->setCheckable( true );
    M_toggle_drag_move_mode_act->setChecked( false );
    connect( M_toggle_drag_move_mode_act, SIGNAL( toggled( bool ) ),
             this, SLOT( toggleDragMoveMode( bool ) ) );
    this->addAction( M_toggle_drag_move_mode_act );
    //
    M_show_monitor_move_dialog_act = new QAction( tr( "Move Dialog" ), this );
    M_show_monitor_move_dialog_act->setStatusTip( tr( "Show player move dialog" ) );
    //M_show_monitor_move_dialog_act->setEnabled( false );
    connect( M_show_monitor_move_dialog_act, SIGNAL( triggered() ),
             this, SLOT( showMonitorMoveDialog() ) );
    this->addAction( M_show_monitor_move_dialog_act );
    //
#ifndef Q_WS_WIN
    M_show_launcher_dialog_act = new QAction( tr( "Launcher Dialog" ), this );
#ifdef Q_WS_MAC
    M_show_launcher_dialog_act->setShortcut( Qt::META + Qt::Key_X );
#else
    M_show_launcher_dialog_act->setShortcut( Qt::CTRL + Qt::Key_X );
#endif
    M_show_launcher_dialog_act->setStatusTip( tr( "Show launcher dialog" ) );
    connect( M_show_launcher_dialog_act, SIGNAL( triggered() ),
             this, SLOT( showLauncherDialog() ) );
    this->addAction( M_show_launcher_dialog_act );
#endif

    //
    M_playmode_change_act_group = new QActionGroup( this );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createActionsView()
{
    M_toggle_menu_bar_act = new QAction( tr( "&Menu Bar" ), this );
#ifdef Q_WS_MAC
    M_toggle_menu_bar_act->setShortcut( Qt::META + Qt::Key_M );
#else
    M_toggle_menu_bar_act->setShortcut( Qt::CTRL + Qt::Key_M );
#endif
    M_toggle_menu_bar_act->setStatusTip( tr( "Show/Hide Menu Bar" ) );
    connect( M_toggle_menu_bar_act, SIGNAL( triggered() ),
             this, SLOT( toggleMenuBar() ) );
    this->addAction( M_toggle_menu_bar_act );
    //
    M_toggle_tool_bar_act = new QAction( tr( "&Tool Bar" ), this );
    M_toggle_tool_bar_act->setStatusTip( tr( "Show/Hide Tool Bar" ) );
    connect( M_toggle_tool_bar_act, SIGNAL( triggered() ),
             this, SLOT( toggleToolBar() ) );
    this->addAction( M_toggle_tool_bar_act );
    //
    M_toggle_status_bar_act = new QAction( tr( "&Status Bar" ), this );
    M_toggle_status_bar_act->setStatusTip( tr( "Show/Hide Status Bar" ) );
    connect( M_toggle_status_bar_act, SIGNAL( triggered() ),
             this, SLOT( toggleStatusBar() ) );
    this->addAction( M_toggle_status_bar_act );
    //
    M_full_screen_act = new QAction( tr( "&Full Screen" ), this );
    M_full_screen_act->setShortcut( Qt::Key_F11 );
    M_full_screen_act->setStatusTip( tr( "Toggle Full Screen" ) );
    connect( M_full_screen_act, SIGNAL( triggered() ),
             this, SLOT( toggleFullScreen() ) );
    this->addAction( M_full_screen_act );
//     (void) new QShortcut( Qt::ALT + Qt::Key_Return,
//                           this, SLOT( toggleFullScreen() ) );
//     (void) new QShortcut( Qt::ALT + Qt::Key_Enter,
//                           this, SLOT( toggleFullScreen() ) );
    {
        QAction * act = new QAction( tr( "Toggle Full Screen" ), this );
        act->setShortcut( Qt::ALT + Qt::Key_Return );
        act->setStatusTip( tr( "Toggle Full Screen" ) );
        this->addAction( act );
        connect( act, SIGNAL( triggered() ),
                 this, SLOT( toggleFullScreen() ) );
    }
    {
        QAction * act = new QAction( tr( "Toggle Full Screen" ), this );
        act->setShortcut( Qt::ALT + Qt::Key_Enter );
        act->setStatusTip( tr( "Toggle Full Screen" ) );
        this->addAction( act );
        connect( act, SIGNAL( triggered() ),
                 this, SLOT( toggleFullScreen() ) );
    }
    //
    M_show_player_type_dialog_act = new QAction( tr( "&Player Type List" ), this );
#ifdef Q_WS_MAC
    M_show_player_type_dialog_act->setShortcut( Qt::META + Qt::Key_H );
#else
    M_show_player_type_dialog_act->setShortcut( Qt::CTRL + Qt::Key_H );
#endif
    M_show_player_type_dialog_act->setStatusTip( tr( "Show player type parameters dialog" ) );
    connect( M_show_player_type_dialog_act, SIGNAL( triggered() ),
             this, SLOT( showPlayerTypeDialog() ) );
    this->addAction( M_show_player_type_dialog_act );
    //
    M_show_detail_dialog_act = new QAction( tr( "&Object Detail" ), this );
#ifdef Q_WS_MAC
    M_show_detail_dialog_act->setShortcut( Qt::META + Qt::Key_I );
#else
    M_show_detail_dialog_act->setShortcut( Qt::CTRL + Qt::Key_I );
#endif
    M_show_detail_dialog_act->setStatusTip( tr( "Show detail information dialog" ) );
    connect( M_show_detail_dialog_act, SIGNAL( triggered() ),
             this, SLOT( showDetailDialog() ) );
    this->addAction( M_show_detail_dialog_act );

    // qt style menu group
    M_style_act_group = new QActionGroup( this );
    Q_FOREACH ( QString style_name, QStyleFactory::keys() )
    {
        QAction * subaction = new QAction( M_style_act_group );
        subaction->setText( style_name );
        subaction->setData( style_name );
        subaction->setCheckable( true );
        if ( style_name.toLower() == QApplication::style()->objectName().toLower() )
        {
            subaction->setChecked( true );
        }
        connect( subaction, SIGNAL( triggered( bool ) ),
                 this, SLOT( changeStyle( bool ) ) );
    }
    //
    M_show_color_setting_dialog_act = new QAction( tr( "&Color Settings" ),
                                                   this );
    M_show_color_setting_dialog_act->setStatusTip( tr( "Show color setting dialog" ) );
    connect( M_show_color_setting_dialog_act, SIGNAL( triggered() ),
             this, SLOT( showColorSettingDialog() ) );
    this->addAction( M_show_color_setting_dialog_act );
    //
    M_show_font_setting_dialog_act = new QAction( tr( "&Font Settings" ), this );
    M_show_font_setting_dialog_act->setStatusTip( tr( "Show font setting dialog" ) );
    connect( M_show_font_setting_dialog_act, SIGNAL( triggered() ),
             this, SLOT( showFontSettingDialog() ) );
    this->addAction( M_show_font_setting_dialog_act );
    //
    M_show_view_config_dialog_act = new QAction( tr( "&View Preference" ), this );
#ifdef Q_WS_MAC
    M_show_view_config_dialog_act->setShortcut( Qt::META + Qt::Key_V );
#else
    M_show_view_config_dialog_act->setShortcut( Qt::CTRL + Qt::Key_V );
#endif
    M_show_view_config_dialog_act->setStatusTip( tr( "Show view preference dialog" ) );
    connect( M_show_view_config_dialog_act, SIGNAL( triggered() ),
             this, SLOT( showViewConfigDialog() ) );
    this->addAction( M_show_view_config_dialog_act );
}


/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createActionsViewConfig()
{
    // z
    M_zoom_in_act = new QAction( tr( "ZoomIn" ), this );
    M_zoom_in_act->setShortcut( Qt::Key_Z );
    M_zoom_in_act->setStatusTip( tr( "Zoom In" ) );
    this->addAction( M_zoom_in_act );

    // x
    M_zoom_out_act = new QAction( tr( "ZoomOut" ), this );
    M_zoom_out_act->setShortcut( Qt::Key_X );
    M_zoom_out_act->setStatusTip( tr( "Zoom Out" ) );
    this->addAction( M_zoom_out_act );

    M_fit_to_screen_act = new QAction( tr( "Fit" ), this );
    M_fit_to_screen_act->setShortcut( Qt::Key_I );
    M_fit_to_screen_act->setStatusTip( tr( "Fit field to the screen" ) );
    this->addAction( M_fit_to_screen_act );

    // e
    M_toggle_enlarge_act = new QAction( tr( "Toggle Enlarge" ), this );
    M_toggle_enlarge_act->setShortcut( Qt::Key_E );
    M_toggle_enlarge_act->setStatusTip( tr( "Toggle enlarge mode" ) );
    this->addAction( M_toggle_enlarge_act );

    // Ctrl + r
    M_toggle_reverse_side_act = new QAction( tr( "Reverse Side" ), this );
#ifdef Q_WS_MAC
    M_toggle_reverse_side_act->setShortcut( Qt::META + Qt::Key_R );
#else
    M_toggle_reverse_side_act->setShortcut( Qt::CTRL + Qt::Key_R );
#endif
    M_toggle_reverse_side_act->setStatusTip( tr( "Toggle team side" ) );
    this->addAction( M_toggle_reverse_side_act );

    // Ctrl + Shift + r
    M_toggle_player_reverse_draw_act = new QAction( tr( "Player Reverse Draw" ), this );
#ifdef Q_WS_MAC
    M_toggle_player_reverse_draw_act->setShortcut( Qt::META + Qt::SHIFT + Qt::Key_R );
#else
    M_toggle_player_reverse_draw_act->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_R );
#endif
    M_toggle_player_reverse_draw_act->setStatusTip( tr( "Toggle player painting order" ) );
    this->addAction( M_toggle_player_reverse_draw_act );

    // n
    M_toggle_show_player_number_act = new QAction( tr( "Show Player Number" ), this );
    M_toggle_show_player_number_act->setShortcut( Qt::Key_N );
    M_toggle_show_player_number_act->setStatusTip( tr( "Show/Hide players' unuform number" ) );
    this->addAction( M_toggle_show_player_number_act );

    // h
    M_toggle_show_player_type_act = new QAction( tr( "Show Player Type" ), this );
    M_toggle_show_player_type_act->setShortcut( Qt::Key_H );
    M_toggle_show_player_type_act->setStatusTip( tr( "Show/Hide player type id" ) );
    this->addAction( M_toggle_show_player_type_act );

    // s
    M_toggle_show_stamina_act = new QAction( tr( "Show Stamina" ), this );
    M_toggle_show_stamina_act->setShortcut( Qt::Key_S );
    M_toggle_show_stamina_act->setStatusTip( tr( "Show/Hide players' stamina" ) );
    this->addAction( M_toggle_show_stamina_act );

    // v
    M_toggle_show_view_area_act = new QAction( tr( "Show View Area" ), this );
    M_toggle_show_view_area_act->setShortcut( Qt::Key_V );
    M_toggle_show_view_area_act->setStatusTip( tr( "Show/Hide players' view area" ) );
    this->addAction( M_toggle_show_view_area_act );

    // d
    M_toggle_show_body_shadow_act = new QAction( tr( "Show Body Shadow" ), this );
    M_toggle_show_body_shadow_act->setShortcut( Qt::Key_D );
    M_toggle_show_body_shadow_act->setStatusTip( tr( "Show/Hide players' body shadow" ) );
    this->addAction( M_toggle_show_body_shadow_act );

    // c
    M_toggle_show_catchable_area_act = new QAction( tr( "Show Catchable Area" ), this );
    M_toggle_show_catchable_area_act->setShortcut( Qt::Key_C );
    M_toggle_show_catchable_area_act->setStatusTip( tr( "Show/Hide goalies' catchable area" ) );
    this->addAction( M_toggle_show_catchable_area_act );

    // t
    M_toggle_show_tackle_area_act = new QAction( tr( "Show Tackle Area" ), this );
    M_toggle_show_tackle_area_act->setShortcut( Qt::Key_T );
    M_toggle_show_tackle_area_act->setStatusTip( tr( "Show/Hide players' tackle area" ) );
    this->addAction( M_toggle_show_tackle_area_act );

    // k
    M_toggle_show_kick_accel_area_act = new QAction( tr( "Show Kick Accel Area" ), this );
    M_toggle_show_kick_accel_area_act->setShortcut( Qt::Key_K );
    M_toggle_show_kick_accel_area_act->setStatusTip( tr( "Show/Hide players' kick accel area" ) );
    this->addAction( M_toggle_show_kick_accel_area_act );

    // f
    M_toggle_show_pointto_act = new QAction( tr( "Show Pointto" ), this );
    M_toggle_show_pointto_act->setShortcut( Qt::Key_F );
    M_toggle_show_pointto_act->setStatusTip( tr( "Show/Hide players' pointto state" ) );
    this->addAction( M_toggle_show_pointto_act );

    // Ctrl + a
    M_toggle_show_attentionto_act = new QAction( tr( "Show Attentionto" ), this );
#ifdef Q_WS_MAC
    M_toggle_show_attentionto_act->setShortcut( Qt::META + Qt::Key_A );
#else
    M_toggle_show_attentionto_act->setShortcut( Qt::CTRL + Qt::Key_A );
#endif
    M_toggle_show_attentionto_act->setStatusTip( tr( "Show/Hide players' attentionto state" ) );
    this->addAction( M_toggle_show_attentionto_act );

    // Ctrl + t
    M_toggle_show_score_board_act = new QAction( tr( "Show Score Board" ), this );
#ifdef Q_WS_MAC
    M_toggle_show_score_board_act->setShortcut( Qt::META + Qt::Key_T );
#else
    M_toggle_show_score_board_act->setShortcut( Qt::CTRL + Qt::Key_T );
#endif
    M_toggle_show_score_board_act->setStatusTip( tr( "Show/Hide score board" ) );
    this->addAction( M_toggle_show_score_board_act );

    // Shift + t
    M_toggle_show_team_graphic_act = new QAction( tr( "Show Team Graphic" ), this );
    M_toggle_show_team_graphic_act->setShortcut( Qt::SHIFT + Qt::Key_T );
    M_toggle_show_team_graphic_act->setStatusTip( tr( "Show/Hide team graphic image" ) );
    this->addAction( M_toggle_show_team_graphic_act );

    // Ctrl + b
    M_toggle_show_ball_act = new QAction( tr( "Show Ball" ), this );
#ifdef Q_WS_MAC
    M_toggle_show_ball_act->setShortcut( Qt::META + Qt::Key_B );
#else
    M_toggle_show_ball_act->setShortcut( Qt::CTRL + Qt::Key_B );
#endif
    M_toggle_show_ball_act->setStatusTip( tr( "Show/Hide ball" ) );
    this->addAction( M_toggle_show_ball_act );

    // Ctrl + p
    M_toggle_show_players_act = new QAction( tr( "Show Players" ), this );
#ifdef Q_WS_MAC
    M_toggle_show_players_act->setShortcut( Qt::META + Qt::Key_P );
#else
    M_toggle_show_players_act->setShortcut( Qt::CTRL + Qt::Key_P );
#endif
    M_toggle_show_players_act->setStatusTip( tr( "Show/Hide players" ) );
    this->addAction( M_toggle_show_players_act );

    // Ctrl + f
    M_toggle_show_flags_act = new QAction( tr( "Show Flags" ), this );
#ifdef Q_WS_MAC
    M_toggle_show_flags_act->setShortcut( Qt::META + Qt::Key_F );
#else
    M_toggle_show_flags_act->setShortcut( Qt::CTRL + Qt::Key_F );
#endif
    M_toggle_show_flags_act->setStatusTip( tr( "Show/Hide flags" ) );
    this->addAction( M_toggle_show_flags_act );

    // o
    M_toggle_show_offside_line_act = new QAction( tr( "Show Offside Lines" ), this );
    M_toggle_show_offside_line_act->setShortcut( Qt::Key_O );
    M_toggle_show_offside_line_act->setStatusTip( tr( "Show/Hide offside lines" ) );
    this->addAction( M_toggle_show_offside_line_act );

    // number 1-9
    for ( int i = 1; i <= 9; ++i )
    {
        {
            M_select_left_agent_act[i-1] = new QAction( QString( "Select Left %1" ).arg( i ), this );
            M_select_left_agent_act[i-1]->setShortcut( Qt::Key_0 + i );
            M_select_left_agent_act[i-1]->setStatusTip( tr( "Select left player %1" ).arg( i ) );
            this->addAction(  M_select_left_agent_act[i-1] );
        }
        {
            M_select_right_agent_act[i-1] = new QAction( QString( "Selct Right %1" ).arg( i ), this );
#ifdef Q_WS_MAC
            M_select_right_agent_act[i-1]->setShortcut( Qt::META + Qt::Key_0 + i );
#else
            M_select_right_agent_act[i-1]->setShortcut( Qt::CTRL + Qt::Key_0 + i );
#endif
            M_select_right_agent_act[i-1]->setStatusTip( tr( "Select right player %1" ).arg( i ) );
            this->addAction( M_select_right_agent_act[i-1] );
        }
    }
    // number 10
    M_select_left_agent_act[9] = new QAction( tr( "Selct Left 10" ), this );
    M_select_left_agent_act[9]->setShortcut( Qt::Key_0 );
    M_select_left_agent_act[9]->setStatusTip( tr( "Select left player 10" ) );
    this->addAction( M_select_left_agent_act[9] );

    M_select_right_agent_act[9] = new QAction( tr( "Selct Right 10" ), this );
#ifdef Q_WS_MAC
    M_select_right_agent_act[9]->setShortcut( Qt::META + Qt::Key_0 );
#else
    M_select_right_agent_act[9]->setShortcut( Qt::CTRL + Qt::Key_0 );
#endif
    M_select_right_agent_act[9]->setStatusTip( tr( "Select right player 10" ) );
    this->addAction( M_select_right_agent_act[9] );

    // number 11
    M_select_left_agent_act[10] = new QAction( tr( "Selct Left 11" ), this );
    M_select_left_agent_act[10]->setShortcut( Qt::Key_Minus );
    M_select_left_agent_act[10]->setStatusTip( tr( "Select left player 11" ) );
    this->addAction( M_select_left_agent_act[10] );

    M_select_right_agent_act[10] = new QAction( tr( "Selct Right 11" ), this );
#ifdef Q_WS_MAC
    M_select_right_agent_act[10]->setShortcut( Qt::META + Qt::Key_Minus );
#else
    M_select_right_agent_act[10]->setShortcut( Qt::CTRL + Qt::Key_Minus );
#endif
    M_select_right_agent_act[10]->setStatusTip( tr( "Select right player 11" ) );
    this->addAction( M_select_right_agent_act[10] );

    // coach
    M_select_left_agent_act[11] = new QAction( tr( "Select Left Coach" ), this );
    M_select_left_agent_act[11]->setShortcut( Qt::Key_Equal );
    M_select_left_agent_act[11]->setStatusTip( tr( "Select left coach" ) );
    this->addAction( M_select_left_agent_act[11] );

    M_select_right_agent_act[11] = new QAction( tr( "Select Right Coach" ), this );
#ifdef Q_WS_MAC
    M_select_right_agent_act[11]->setShortcut( Qt::META + Qt::Key_Equal );
#else
    M_select_right_agent_act[11]->setShortcut( Qt::CTRL + Qt::Key_Equal );
#endif
    M_select_right_agent_act[11]->setStatusTip( tr( "Select right coach" ) );
    this->addAction( M_select_right_agent_act[11] );

    // b
    M_toggle_focus_ball_act = new QAction( tr( "Focus Ball" ), this );
    M_toggle_focus_ball_act->setShortcut( Qt::Key_B );
    M_toggle_focus_ball_act->setStatusTip( tr( "Focus to the ball" ) );
    this->addAction( M_toggle_focus_ball_act );

    // p
    M_toggle_focus_player_act = new QAction( tr( "Focus Player" ), this );
    M_toggle_focus_player_act->setShortcut( Qt::Key_P );
    M_toggle_focus_player_act->setStatusTip( tr( "Focus to the selected player" ) );
    this->addAction( M_toggle_focus_player_act );

    // a
    M_toggle_select_auto_all_act = new QAction( tr( "Select auto all" ), this );
    M_toggle_select_auto_all_act->setShortcut( Qt::Key_A );
    M_toggle_select_auto_all_act->setStatusTip( tr( "Select the player narest to the ball in all players" ) );
    this->addAction( M_toggle_select_auto_all_act );

    // l
    M_toggle_select_auto_left_act = new QAction( tr( "Toggle auto left" ), this );
    M_toggle_select_auto_left_act->setShortcut( Qt::Key_L );
    M_toggle_select_auto_left_act->setStatusTip( tr( "Toggle the player selection mode in left team. (auto/unselect)" ) );
    this->addAction( M_toggle_select_auto_left_act );

    // r
    M_toggle_select_auto_right_act = new QAction( tr( "Toggle auto right" ), this );
    M_toggle_select_auto_right_act->setShortcut( Qt::Key_R );
    M_toggle_select_auto_right_act->setStatusTip( tr( "Toggle the agent selection mode in right team. (auto/unselect)" ) );
    this->addAction( M_toggle_select_auto_right_act );

    // u
    M_unselect_act = new QAction( tr( "Unselect" ), this );
    M_unselect_act->setShortcut( Qt::Key_U );
    M_unselect_act->setStatusTip( tr( "Unselect agent" ) );
    this->addAction( M_unselect_act );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createActionsLogPlayer()
{
    //
    // visible actions
    //

    //
    M_log_player_go_first_act = new QAction( QIcon( QPixmap( logplayer_go_first_xpm ) ),
                                             tr( "Go first" ), this );
    M_log_player_go_first_act->setShortcut( Qt::Key_Home );
    M_log_player_go_first_act->setStatusTip( tr( "Go to the first cycle.(" )
                                             + M_log_player_go_first_act->shortcut().toString()
                                             + tr( ")" ) );
    connect( M_log_player_go_first_act, SIGNAL( triggered() ),
             M_log_player, SLOT( goToFirst() ) );
    this->addAction( M_log_player_go_first_act );

    //
    M_log_player_go_prev_score_act = new QAction( QIcon( QPixmap( logplayer_go_prev_score_xpm ) ),
                                                  tr( "Go prev goal" ), this );
#ifdef Q_WS_MAC
    M_log_player_go_prev_score_act->setShortcut( Qt::META + Qt::Key_G );
#else
    M_log_player_go_prev_score_act->setShortcut( Qt::CTRL + Qt::Key_G );
#endif
    M_log_player_go_prev_score_act->setStatusTip( tr( "Go to the previous goal scene.(" )
                                                  + M_log_player_go_prev_score_act->shortcut().toString()
                                                  + (")" ) );
    connect( M_log_player_go_prev_score_act, SIGNAL( triggered() ),
             M_log_player, SLOT( goToPrevScore() ) );
    this->addAction( M_log_player_go_prev_score_act );

    //
    M_log_player_one_step_back_act = new QAction( QIcon( QPixmap( logplayer_one_step_back_xpm ) ),
                                                  tr( "Step Back" ), this );
    M_log_player_one_step_back_act->setShortcut( Qt::Key_Left );
    M_log_player_one_step_back_act->setStatusTip( tr( "One step back the log player. (" )
                                                  + M_log_player_one_step_back_act->shortcut().toString()
                                                  + tr ( ")" ) );
    connect( M_log_player_one_step_back_act, SIGNAL( triggered() ),
             M_log_player, SLOT( stepBack() ) );
    this->addAction( M_log_player_one_step_back_act );

    //
    M_log_player_play_or_stop_act = new QAction( QIcon( QPixmap( logplayer_play_or_stop_xpm ) ),
                                                 tr( "Play/Stop." ), this );
    M_log_player_play_or_stop_act->setShortcut( Qt::Key_Space );
    M_log_player_play_or_stop_act->setStatusTip( tr( "Play or Stop the log player.(" )
                                                 + M_log_player_play_or_stop_act->shortcut().toString()
                                                 + tr( ")" ) );
    connect( M_log_player_play_or_stop_act, SIGNAL( triggered() ),
             M_log_player, SLOT( playOrStop() ) );
    this->addAction( M_log_player_play_or_stop_act );

    //
    M_log_player_one_step_forward_act = new QAction( QIcon( QPixmap( logplayer_one_step_forward_xpm ) ),
                                                     tr( "Step Forward" ), this );
    M_log_player_one_step_forward_act->setShortcut( Qt::Key_Right );
    M_log_player_one_step_forward_act->setStatusTip( tr( "One step forward the log player.(" )
                                                     + M_log_player_one_step_forward_act->shortcut().toString()
                                                     + tr( ")" ) );
    connect( M_log_player_one_step_forward_act, SIGNAL( triggered() ),
             M_log_player, SLOT( stepForward() ) );
    this->addAction( M_log_player_one_step_forward_act );

    //
    M_log_player_go_next_score_act = new QAction( QIcon( QPixmap( logplayer_go_next_score_xpm ) ),
                                                  tr( "Go next score" ), this );
    M_log_player_go_next_score_act->setShortcut( Qt::Key_G );
    M_log_player_go_next_score_act->setStatusTip( tr( "Go to the next goal scene.(" )
                                                  + M_log_player_go_next_score_act->shortcut().toString()
                                                  + tr( ")" ) );
    connect( M_log_player_go_next_score_act, SIGNAL( triggered() ),
             M_log_player, SLOT( goToNextScore() ) );
    this->addAction( M_log_player_go_next_score_act );

    //
    M_log_player_go_last_act = new QAction( QIcon( QPixmap( logplayer_go_last_xpm ) ),
                                            tr( "Go last" ), this );
    M_log_player_go_last_act->setShortcut( Qt::Key_End );
    M_log_player_go_last_act->setStatusTip( tr( "Go to the last.(" )
                                            + M_log_player_go_last_act->shortcut().toString()
                                            + tr( ")" ) );
    connect( M_log_player_go_last_act, SIGNAL( triggered() ),
             M_log_player, SLOT( goToLast() ) );
    this->addAction( M_log_player_go_last_act );

    //
    M_log_player_shift_down_act = new QAction( QIcon( QPixmap( logplayer_shift_down_xpm ) ),
                                               tr( "Decelerate" ), this );
#ifdef Q_WS_MAC
    M_log_player_shift_down_act->setShortcut( Qt::META + Qt::Key_Left );
#else
    M_log_player_shift_down_act->setShortcut( Qt::CTRL + Qt::Key_Left );
#endif
    M_log_player_shift_down_act->setStatusTip( tr( "Decelerate the log player.(" )
                                               + M_log_player_shift_down_act->shortcut().toString()
                                               + tr( ")" ) );
    connect( M_log_player_shift_down_act, SIGNAL( triggered() ),
             M_log_player, SLOT( decelerate() ) );
    this->addAction( M_log_player_shift_down_act );

    //
    M_log_player_shift_up_act = new QAction( QIcon( QPixmap( logplayer_shift_up_xpm ) ),
                                             tr( "Accelerate" ), this );
#ifdef Q_WS_MAC
    M_log_player_shift_up_act->setShortcut( Qt::META + Qt::Key_Right );
#else
    M_log_player_shift_up_act->setShortcut( Qt::CTRL + Qt::Key_Right );
#endif
    M_log_player_shift_up_act->setStatusTip( tr ( "Accelerate the log player.(" )
                                             + M_log_player_shift_up_act->shortcut().toString()
                                             + tr( ")" ) );
    connect( M_log_player_shift_up_act, SIGNAL( triggered() ),
             M_log_player, SLOT( accelerate() ) );
    this->addAction( M_log_player_shift_up_act );

    //
    // invisible actions
    //

    //(void)new QShortcut( Qt::Key_Down, this, SLOT( stop() ) );
    M_log_player_stop_act = new QAction( QIcon( QPixmap( logplayer_stop_xpm ) ),
                                         tr( "Stop" ), this );
    M_log_player_stop_act->setShortcut( Qt::Key_Down );
    M_log_player_stop_act->setStatusTip( tr( "Stop the log player. (" )
                                         + M_log_player_stop_act->shortcut().toString()
                                         + tr ( ")" ) );
    connect( M_log_player_stop_act, SIGNAL( triggered() ),
             M_log_player, SLOT( stop() ) );
    this->addAction( M_log_player_stop_act );

    //new QShortcut( Qt::CTRL + Qt::Key_Down, this, SLOT( playBack() ) );
    M_log_player_play_back_act = new QAction( QIcon( QPixmap( logplayer_play_back_xpm ) ),
                                              tr( "Play Backward" ), this );
    M_log_player_play_back_act->setStatusTip( tr( "Play backward the log player.(" )
                                              + M_log_player_play_back_act->shortcut().toString()
                                              + tr( ")" ) );
#ifdef Q_WS_MAC
    M_log_player_play_back_act->setShortcut( Qt::META + Qt::Key_Down );
#else
    M_log_player_play_back_act->setShortcut( Qt::CTRL + Qt::Key_Down );
#endif
    connect( M_log_player_play_back_act, SIGNAL( triggered() ),
             M_log_player, SLOT( playBack() ) );
    this->addAction( M_log_player_play_back_act );

    //(void)new QShortcut( Qt::Key_Up, this, SLOT( playForward() ) );
    M_log_player_play_forward_act = new QAction( QIcon( QPixmap( logplayer_play_forward_xpm ) ),
                                                 tr( "Play Forward" ), this );
    M_log_player_play_forward_act->setShortcut( Qt::Key_Up );
    M_log_player_play_forward_act->setStatusTip( tr( "Play forward the log player.(" )
                                                 + M_log_player_play_forward_act->shortcut().toString()
                                                 + tr( ")" ) );
    connect( M_log_player_play_forward_act, SIGNAL( triggered() ),
             M_log_player, SLOT( playForward() ) );
    this->addAction( M_log_player_play_forward_act );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createActionsDebug()
{
    M_show_debug_message_window_act = new QAction( tr( "Debug &Message" ), this );
#ifdef Q_WS_MAC
    M_show_debug_message_window_act->setShortcut( Qt::META + Qt::Key_D );
#else
    M_show_debug_message_window_act->setShortcut( Qt::CTRL + Qt::Key_D );
#endif
    M_show_debug_message_window_act->setStatusTip( tr( "Show debug message window" ) );
    connect( M_show_debug_message_window_act, SIGNAL( triggered() ),
             this, SLOT( showDebugMessageWindow() ) );
    this->addAction( M_show_debug_message_window_act );
    //
    M_toggle_debug_server_act = new QAction( QIcon( QPixmap( debug_server_switch_xpm ) ),
                                             tr( "Start/Stop the debug server." ),
                                             this );
#ifdef Q_WS_MAC
    M_toggle_debug_server_act->setShortcut( Qt::META + Qt::Key_S );
#else
    M_toggle_debug_server_act->setShortcut( Qt::CTRL + Qt::Key_S );
#endif
    M_toggle_debug_server_act->setStatusTip( tr( "Start/Stop the debug server." ) );
    M_toggle_debug_server_act->setEnabled( false );
    M_toggle_debug_server_act->setCheckable( true );
    M_toggle_debug_server_act->setChecked( false );
    connect( M_toggle_debug_server_act, SIGNAL( toggled( bool ) ),
             this, SLOT( toggleDebugServer( bool ) ) );
    this->addAction( M_toggle_debug_server_act );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createActionsHelp()
{
    M_about_act = new QAction( QIcon( QPixmap( soccerwindow2_nostr_xpm ) ),
                               tr( "&About" ), this );
    M_about_act->setStatusTip( tr( "Show the about dialog." ) );
    connect( M_about_act, SIGNAL( triggered() ), this, SLOT( about() ) );
    this->addAction( M_about_act );
    //

    //
    M_shortcut_keys_act = new QAction( tr( "Shortcut Keys" ), this );
    M_shortcut_keys_act->setStatusTip( tr( "Print available shortcut keys." ) );
    connect( M_shortcut_keys_act, SIGNAL( triggered() ),
             this, SLOT( printShortcutKeys() ) );
    this->addAction( M_shortcut_keys_act );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createMenus()
{
    createMenuFile();
    createMenuMonitor();
    createMenuLogPlayer();
    createMenuView();
    createMenuDebug();
    createMenuHelp();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createMenuFile()
{
    QMenu * menu = menuBar()->addMenu( tr( "&File" ) );

    menu->addAction( M_open_rcg_act );
    menu->addAction( M_save_rcg_act );

    menu->addSeparator();
    menu->addAction( M_open_debug_view_act );
    menu->addAction( M_save_debug_view_act );

    menu->addSeparator();
    menu->addAction( M_open_draw_data_act );

    menu->addSeparator();
    menu->addAction( M_show_image_save_dialog_act );

    menu->addSeparator();
    menu->addAction( M_exit_act );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createMenuMonitor()
{
    QMenu * menu = menuBar()->addMenu( tr( "&Monitor" ) );

    menu->addAction( M_kick_off_act );
    menu->addAction( M_set_live_mode_act );

    menu->addSeparator();
    menu->addAction( M_connect_monitor_act );
    menu->addAction( M_connect_monitor_to_act );
    menu->addAction( M_disconnect_monitor_act );

#ifndef Q_WS_WIN
    menu->addSeparator();
    menu->addAction( M_kill_server_act );
    menu->addAction( M_restart_server_act );
#endif
    menu->addSeparator();
    menu->addAction( M_toggle_drag_move_mode_act );
    menu->addAction( M_show_monitor_move_dialog_act );

#ifndef Q_WS_WIN
    menu->addSeparator();
    menu->addAction( M_show_launcher_dialog_act );
#endif
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createMenuLogPlayer()
{
#ifdef Q_WS_MAC
    //QMenu * menu = new QMenu( this );
    QMenu * menu = menuBar()->addMenu( tr( "&LogPlayer" ) );

    menu->addAction( M_log_player_go_first_act );
    menu->addAction( M_log_player_go_prev_score_act );
    menu->addAction( M_log_player_one_step_back_act );
    menu->addAction( M_log_player_play_or_stop_act );
    menu->addAction( M_log_player_one_step_forward_act );
    menu->addAction( M_log_player_go_next_score_act );
    menu->addAction( M_log_player_go_last_act );
    menu->addAction( M_log_player_shift_down_act );
    menu->addAction( M_log_player_shift_up_act );
    menu->addAction( M_log_player_stop_act );
    menu->addAction( M_log_player_play_back_act );
    menu->addAction( M_log_player_play_forward_act );
#endif
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createMenuView()
{
    QMenu * menu = menuBar()->addMenu( tr( "&View" ) );
    menu->addAction( M_toggle_menu_bar_act );
    menu->addAction( M_toggle_tool_bar_act );
    menu->addAction( M_toggle_status_bar_act );

    menu->addSeparator();
    menu->addAction( M_full_screen_act );

    menu->addSeparator();
    menu->addAction( M_show_player_type_dialog_act );
    menu->addAction( M_show_detail_dialog_act );

    menu->addSeparator();
    {
        QMenu * submenu = menu->addMenu( tr( "Qt &Style" ) );
        Q_FOREACH ( QAction * action, M_style_act_group->actions() )
            {
                submenu->addAction( action );
            }
    }
    menu->addAction( M_show_color_setting_dialog_act );
    menu->addAction( M_show_font_setting_dialog_act );
    menu->addAction( M_show_view_config_dialog_act );

#ifdef Q_WS_MAC
    menu->addSeparator();
    {
        QMenu * submenu = menu->addMenu( tr( "Field Settings" ) );
        //QMenu * submenu = new QMenu ( this );

        submenu->addAction( M_zoom_in_act );
        submenu->addAction( M_zoom_out_act );
        submenu->addAction( M_fit_to_screen_act );
        submenu->addAction( M_toggle_enlarge_act );
        submenu->addAction( M_toggle_reverse_side_act );
        submenu->addAction( M_toggle_player_reverse_draw_act );
        submenu->addAction( M_toggle_show_player_number_act );
        submenu->addAction( M_toggle_show_player_type_act );
        submenu->addAction( M_toggle_show_stamina_act );
        submenu->addAction( M_toggle_show_view_area_act );
        submenu->addAction( M_toggle_show_body_shadow_act );
        submenu->addAction( M_toggle_show_catchable_area_act );
        submenu->addAction( M_toggle_show_tackle_area_act );
        submenu->addAction( M_toggle_show_kick_accel_area_act );
        submenu->addAction( M_toggle_show_pointto_act );
        submenu->addAction( M_toggle_show_attentionto_act );
        submenu->addAction( M_toggle_show_score_board_act );
        submenu->addAction( M_toggle_show_team_graphic_act );
        submenu->addAction( M_toggle_show_ball_act );
        submenu->addAction( M_toggle_show_players_act );
        submenu->addAction( M_toggle_show_flags_act );
        submenu->addAction( M_toggle_show_offside_line_act );
        {
            QMenu * subsubmenu = submenu->addMenu( tr( "Select Agent" ) );
            for ( int i = 0; i < 12; ++i )
            {
                subsubmenu->addAction( M_select_left_agent_act[i] );
            }
            for ( int i = 0; i < 12; ++i )
            {
                subsubmenu->addAction( M_select_right_agent_act[i] );
            }
        }
        submenu->addAction( M_toggle_focus_ball_act );
        submenu->addAction( M_toggle_focus_player_act );
        submenu->addAction( M_toggle_select_auto_all_act );
        submenu->addAction( M_toggle_select_auto_left_act );
        submenu->addAction( M_toggle_select_auto_right_act );
        submenu->addAction(  M_unselect_act );
    }
#endif
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createMenuDebug()
{
    QMenu * menu = menuBar()->addMenu( tr( "&Debug" ) );
    menu->addAction( M_show_debug_message_window_act );
    menu->addAction( M_toggle_debug_server_act );
}
/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createMenuHelp()
{
    QMenu * menu = menuBar()->addMenu( tr( "&Help" ) );
    menu->addAction( M_about_act );
    menu->addAction( M_shortcut_keys_act );

//     QAction * act = new QAction( tr( "About Qt" ), this );
//     act->setStatusTip( tr( "Show about Qt." ) );
//     connect( act, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
//     menu->addAction( act );
    menu->addAction( tr( "About Qt" ), qApp, SLOT( aboutQt() ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createToolBars()
{
//     M_monitor_tool_bar = new QToolBar( tr( "Monitor" ), this );
//     M_monitor_tool_bar->setIconSize( QSize( 16, 16 ) );
//     M_monitor_tool_bar->addAction( M_set_live_mode_act );
//     M_monitor_tool_bar->addAction( M_toggle_drag_move_mode_act );
//     M_monitor_tool_bar->addAction( M_toggle_debug_server_act );

//     this->addToolBar( M_monitor_tool_bar );


    //
    // create log player tool bar
    //

    //
    M_log_player_tool_bar = new LogPlayerToolBar( M_main_data, this );

    M_log_player_tool_bar->addAction( M_log_player_go_first_act );
    M_log_player_tool_bar->addAction( M_log_player_go_prev_score_act );
    M_log_player_tool_bar->addAction( M_log_player_one_step_back_act );
    M_log_player_tool_bar->addAction( M_log_player_play_or_stop_act );
    M_log_player_tool_bar->addAction( M_log_player_one_step_forward_act );
    M_log_player_tool_bar->addAction( M_log_player_go_next_score_act );
    M_log_player_tool_bar->addAction( M_log_player_go_last_act );
    M_log_player_tool_bar->addAction( M_log_player_shift_down_act );
    M_log_player_tool_bar->addAction( M_log_player_shift_up_act );

    M_log_player_tool_bar->createCycleSlider();
    M_log_player_tool_bar->createCycleEdit();

    M_log_player_tool_bar->addSeparator();

    M_log_player_tool_bar->addAction( M_set_live_mode_act );
    M_log_player_tool_bar->addAction( M_toggle_drag_move_mode_act );
    M_log_player_tool_bar->addAction( M_toggle_debug_server_act );
    {
        QFrame * dummy_frame = new QFrame;
        QHBoxLayout * layout = new QHBoxLayout;
        //layout->addSpacing( 10 );
        layout->addStretch( 1 );
        dummy_frame->setLayout( layout );
        M_log_player_tool_bar->addWidget( dummy_frame );
    }
    {
        QFrame * dummy_frame = new QFrame;
        QVBoxLayout * layout = new QVBoxLayout;
        //layout->addSpacing( 10 );
        layout->addStretch( 1 );
        dummy_frame->setLayout( layout );
        M_log_player_tool_bar->addWidget( dummy_frame );
    }

    connect( M_log_player_tool_bar,  SIGNAL( indexChanged( int ) ),
             M_log_player, SLOT( goToIndex( int ) ) );
    connect( M_log_player_tool_bar,  SIGNAL( cycleChanged( int ) ),
             M_log_player, SLOT( goToCycle( int ) ) );

    connect( this, SIGNAL( viewUpdated() ),
             M_log_player_tool_bar, SLOT( updateSlider() ) );


    if ( Options::instance().toolBarArea() == "top" )
    {
        this->addToolBar( Qt::TopToolBarArea, M_log_player_tool_bar );
    }
    else if ( Options::instance().toolBarArea() == "bottom" )
    {
        this->addToolBar( Qt::BottomToolBarArea, M_log_player_tool_bar );
    }
    else if ( Options::instance().toolBarArea() == "left" )
    {
        M_log_player_tool_bar->changeOrientation( Qt::Vertical );
        this->addToolBar( Qt::LeftToolBarArea, M_log_player_tool_bar );
    }
    else if ( Options::instance().toolBarArea() == "right" )
    {
        M_log_player_tool_bar->changeOrientation( Qt::Vertical );
        this->addToolBar( Qt::RightToolBarArea, M_log_player_tool_bar );
    }
    else
    {
        std::cerr << __FILE__ << ": (createToolBars) "
                  << "***WARNING*** Unsupported tool bar position ["
                  << Options::instance().toolBarArea() << ']'
                  << std::endl;
    }

    //this->addToolBar( M_log_player_tool_bar ); // qt4

}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createStatusBar()
{
    this->statusBar()->showMessage( tr( "Ready" ) );

    M_position_label = new QLabel( tr( "(0.0, 0.0)" ) );

    int min_width
        = M_position_label->fontMetrics().width(  tr( "(-60.0, -30.0)" ) )
        + 16;
    M_position_label->setMinimumWidth( min_width );
    M_position_label->setAlignment( Qt::AlignRight );

    this->statusBar()->addPermanentWidget( M_position_label );

    //QSlider * slider = new QSlider( Qt::Horizontal );
    //this->statusBar()->addWidget( slider );

    //QLineEdit * edit = new QLineEdit( tr( "0" ) );
    //edit->setMaximumSize( 36, 20 );
    //this->statusBar()->addWidget( edit );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createFieldCanvas()
{
    M_field_canvas = new FieldCanvas( M_main_data );
    //M_field_canvas->setParent( this );
    this->setCentralWidget( M_field_canvas );

    M_field_canvas->setFocus();

    connect( this, SIGNAL( viewUpdated() ),
             M_field_canvas, SLOT( update() ) );
             //M_field_canvas, SLOT( repaint() ) );

    connect( M_field_canvas, SIGNAL( mouseMoved( const QPoint & ) ),
             this, SLOT( updatePositionLabel( const QPoint & ) ) );

    connect( M_field_canvas, SIGNAL( dropBall( const QPoint & ) ),
             this, SLOT( dropBall( const QPoint & ) ) );
    connect( M_field_canvas, SIGNAL( freeKickLeft( const QPoint & ) ),
             this, SLOT( freeKickLeft( const QPoint & ) ) );
    connect( M_field_canvas, SIGNAL( freeKickRight( const QPoint & ) ),
             this, SLOT( freeKickRight( const QPoint & ) ) );

    connect( M_field_canvas, SIGNAL( playModeChanged( int, const QPoint & ) ),
             this, SLOT( changePlayMode( int, const QPoint & ) ) );

    connect( M_field_canvas, SIGNAL( playerMoved( const QPoint & ) ),
             this, SLOT( movePlayer( const QPoint & ) ) );

    M_field_canvas->setNormalMenu( createNormalPopupMenu() );
    M_field_canvas->setSystemMenu( createSystemPopupMenu() );
    M_field_canvas->setMonitorMenu( createMonitorPopupMenu() );
}

/*-------------------------------------------------------------------*/
/*!

*/
QMenu *
MainWindow::createNormalPopupMenu()
{
    QMenu * menu = new QMenu( M_field_canvas );
    menu->addAction( M_open_rcg_act );
    menu->addAction( M_connect_monitor_act );
// #ifndef Q_WS_WIN
//         menu->addAction( M_restart_server_act );
// #endif

    return menu;
}

/*-------------------------------------------------------------------*/
/*!

*/
QMenu *
MainWindow::createSystemPopupMenu()
{
    QMenu * menu = new QMenu( M_field_canvas );
    menu->addAction( M_open_rcg_act );
    menu->addAction( M_connect_monitor_act );
#ifndef Q_WS_WIN
    menu->addSeparator();
    menu->addAction( M_kill_server_act );
    // menu->addAction( M_restart_server_act );
#endif

    return menu;
}

/*-------------------------------------------------------------------*/
/*!

*/
QMenu *
MainWindow::createMonitorPopupMenu()
{
    QMenu * menu = new QMenu( M_field_canvas );

    menu->addAction( M_kick_off_act );
    menu->addSeparator();
    menu->addAction( tr( "Drop Ball" ),
                     M_field_canvas, SLOT( dropBall() ) );
    {
        QAction * act = new QAction( tr( "Drop Ball There" ), this );
        act->setStatusTip( tr( "Drop ball at the current ball position." ) );
        connect( act, SIGNAL( triggered() ),
                 this, SLOT( dropBallThere() ) );
        menu->addAction( act );
    }
    menu->addAction( tr( "Free Kick Left" ),
                     M_field_canvas, SLOT( freeKickLeft() ) );
    menu->addAction( tr( "Free Kick Right" ),
                     M_field_canvas, SLOT( freeKickRight() ) );
    //
    menu->addSeparator();

    //
    QMenu * playmode_menu = menu->addMenu( tr( "Change Playmode" ) );
    {
        const char * playmode_strings[] = PLAYMODE_STRINGS;

        QSignalMapper * mapper = new QSignalMapper( this );
        for ( int mode = 0; mode < rcsc::PM_MAX; ++mode )
        {
            if ( mode == rcsc::PM_BeforeKickOff
                 || mode == rcsc::PM_PlayOn
                 || mode == rcsc::PM_KickIn_Left
                 || mode == rcsc::PM_KickIn_Right
                 || mode == rcsc::PM_CornerKick_Left
                 || mode == rcsc::PM_CornerKick_Right
                 || mode == rcsc::PM_GoalKick_Left
                 || mode == rcsc::PM_GoalKick_Right
                 || mode == rcsc::PM_Foul_Charge_Left
                 || mode == rcsc::PM_Foul_Charge_Right
                 || mode == rcsc::PM_Foul_Push_Left
                 || mode == rcsc::PM_Foul_Push_Right
                 || mode == rcsc::PM_Back_Pass_Left
                 || mode == rcsc::PM_Back_Pass_Right
                 || mode == rcsc::PM_GoalKick_Right
                 || mode == rcsc::PM_IndFreeKick_Left
                 || mode == rcsc::PM_IndFreeKick_Right )
            {
                QAction * act = new QAction( M_playmode_change_act_group );
                act->setText( QString::fromLatin1( playmode_strings[mode] ) );
                connect( act, SIGNAL( triggered() ), mapper, SLOT( map() ) );
                mapper->setMapping( act, mode );
            }
        }
        connect( mapper, SIGNAL( mapped( int ) ),
                 M_field_canvas, SLOT( changePlayMode( int ) ) );
    }
    Q_FOREACH ( QAction * act, M_playmode_change_act_group->actions() )
    {
        playmode_menu->addAction( act );
    }

    //
    menu->addSeparator();
    //
    {
        QAction * act = new QAction( tr( "Yellow Card" ), this );
        act->setStatusTip( tr( "Call yellow card to the selected player." ) );
        connect( act, SIGNAL( triggered() ),
                 this, SLOT( yellowCard() ) );
        menu->addAction( act );
    }
    {
        QAction * act = new QAction( tr( "Red Card" ), this );
        act->setStatusTip( tr( "Call red card to the selected player." ) );
        connect( act, SIGNAL( triggered() ),
                 this, SLOT( redCard() ) );
        menu->addAction( act );
    }

    return menu;
}

/*-------------------------------------------------------------------*/
/*!

*/
QMenu *
MainWindow::createOfflinePopupMenu()
{
    QMenu * menu = new QMenu( M_field_canvas );

    // menu->addAction( tr( "Evaluate Point" ),
    //                  M_field_canvas, SLOT( evaluatePoint() ) );

    return menu;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::createViewConfigDialog()
{
    if ( M_view_config_dialog )
    {
        return;
    }

    M_view_config_dialog = new ViewConfigDialog( this, M_main_data );

    //M_view_config_dialog->setParent( this, Qt::Dialog );
    M_view_config_dialog->hide();

    connect( M_view_config_dialog, SIGNAL( configured() ),
             this, SIGNAL( viewUpdated() ) );

    connect( M_view_config_dialog, SIGNAL( canvasResized( const QSize & ) ),
             this, SLOT( resizeCanvas( const QSize & ) ) );

    connect( M_field_canvas, SIGNAL( focusChanged( const QPoint & ) ),
             M_view_config_dialog, SLOT( setFocusPoint( const QPoint & ) ) );

    connect( M_view_config_dialog, SIGNAL( configured() ),
             M_field_canvas, SLOT( setRedrawAllFlag()  ) );

    //
    // set action
    //

    connect( M_zoom_in_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( zoomIn() ) );
    connect( M_zoom_out_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( zoomOut() ) );
    {
        // Ctrl + z
        QAction * act = new QAction( tr( "ZoomOut" ), this );
#ifdef Q_WS_MAC
        act->setShortcut( Qt::META + Qt::Key_Z );
#else
        act->setShortcut( Qt::CTRL + Qt::Key_Z );
#endif
        act->setStatusTip( tr( "Zoom Out" ) );
        this->addAction( act );
        connect( act, SIGNAL( triggered() ),
                 M_view_config_dialog, SLOT( zoomOut() ) );
    }
    connect( M_fit_to_screen_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( fitToScreen() ) );
    connect( M_toggle_enlarge_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleEnlarge() ) );
    connect( M_toggle_reverse_side_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleReverseSide() ) );
    connect( M_toggle_player_reverse_draw_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( togglePlayerReverseDraw() ) );
    connect( M_toggle_show_player_number_act, SIGNAL( triggered() ),
                 M_view_config_dialog, SLOT( toggleShowPlayerNumber() ) );
    connect( M_toggle_show_player_type_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowPlayerType() ) );
    connect( M_toggle_show_stamina_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowStamina() ) );
    connect( M_toggle_show_view_area_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowViewArea() ) );
    connect( M_toggle_show_body_shadow_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowBodyShadow() ) );
    connect( M_toggle_show_catchable_area_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowCatchableArea() ) );
    connect( M_toggle_show_tackle_area_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowTackleArea() ) );
    connect( M_toggle_show_kick_accel_area_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowKickAccelArea() ) );
    connect( M_toggle_show_pointto_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowPointto() ) );
    connect( M_toggle_show_attentionto_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowAttentionto() ) );
    connect( M_toggle_show_score_board_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowScoreBoard() ) );
    connect( M_toggle_show_team_graphic_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowTeamGraphic() ) );
    connect( M_toggle_show_ball_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowBall() ) );
    connect( M_toggle_show_players_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowPlayers() ) );
    connect( M_toggle_show_flags_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowFlags() ) );
    connect( M_toggle_show_offside_line_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleShowOffsideLine() ) );
    for ( int i = 0; i < 12; ++i )
    {
        connect( M_select_left_agent_act[i], SIGNAL( triggered() ),
                 M_view_config_dialog, SLOT( selectAgentWithKey() ) );
        connect( M_select_right_agent_act[i], SIGNAL( triggered() ),
                 M_view_config_dialog, SLOT( selectAgentWithKey() ) );
    }
    {
        QShortcut * s = new QShortcut( Qt::Key_AsciiCircum,
                                       M_view_config_dialog, SLOT( selectLeftCoach() ) );
        s->setParent( this );
    }
    {
        QShortcut * s = new QShortcut(
#ifdef Q_WS_MAC
                                      Qt::META + Qt::Key_AsciiCircum,
#else
                                      Qt::CTRL + Qt::Key_AsciiCircum,
#endif
                                       M_view_config_dialog, SLOT( selectRightCoach() ) );
        s->setParent( this );
    }


    connect( M_toggle_focus_ball_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleFocusBall() ) );
    connect( M_toggle_focus_player_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleFocusPlayer() ) );
    connect( M_toggle_select_auto_all_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleSelectAutoAll() ) );
    connect( M_toggle_select_auto_left_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleSelectAutoLeft() ) );
    connect( M_toggle_select_auto_right_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( toggleSelectAutoRight() ) );
    connect( M_unselect_act, SIGNAL( triggered() ),
             M_view_config_dialog, SLOT( setUnselect() ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::closeEvent( QCloseEvent * event )
{
    event->ignore();

    //QCoreApplication::instance()->quit();
    qApp->quit();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::resizeEvent( QResizeEvent * event )
{
    event->accept();

    if ( M_view_config_dialog
         && M_view_config_dialog->isVisible() )
    {
        M_view_config_dialog->updateFieldScale();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::wheelEvent( QWheelEvent * event )
{
    if ( event->delta() < 0 )
    {
        M_log_player->stepForward();
    }
    else
    {
        M_log_player->stepBack();
    }

    event->accept();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::dragEnterEvent( QDragEnterEvent * event )
{
    if ( event->mimeData()->hasFormat( "text/uri-list" ) )
    {
        event->acceptProposedAction();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::dropEvent( QDropEvent * event )
{
    const QMimeData * mimedata = event->mimeData();

    QList< QUrl > urls = mimedata->urls();

//     std::cerr << "urls size = " << urls.size() << std::endl;

//     for ( int i = 0; i < urls.size() && i < 32; ++i )
//     {
//         std::cerr << "url " << i << ": "
//                   << urls.at(i).path().toStdString()
//                   << std::endl;
//     }

    while ( ! urls.empty()
            && urls.back().isEmpty() )
    {
        urls.pop_back();
    }

    if ( urls.empty() )
    {
        QMessageBox::information( this,
                                  tr( "Information" ),
                                  tr( "Dropped data has no file path." ),
                                  QMessageBox::Ok, QMessageBox::NoButton );
    }
    else if ( urls.size() == 1 )
    {
        openRCG( urls.front().toLocalFile() );
    }
    else if ( urls.size() > 1 )
    {
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "Too many files are dropped." ),
                               QMessageBox::Ok, QMessageBox::NoButton );
    }

    event->acceptProposedAction();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::openRCG()
{
#ifdef HAVE_LIBZ
    QString filter( tr( "Game Log files (*.rcg *.rcg.gz);;"
                        "All files (*)" ) );
#else
    QString filter( tr( "Game Log files (*.rcg);;"
                        "All files (*)" ) );
#endif

    QString default_dir
        = QString::fromStdString( Options::instance().gameLogDir() );

    QString file_path = QFileDialog::getOpenFileName( this,
                                                      tr( "Choose a game log file to open" ),
                                                      default_dir,
                                                      filter );

    if ( file_path.isEmpty() )
    {
        //std::cerr << "MainWindow::opneRCG() empty file path" << std::endl;
        return;
    }

    //std::cerr << "file = [" << file_path.toStdString() << ']' << std::endl;

    openRCG( file_path );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::openRCG( const QString & file_path )
{
    if ( ! QFile::exists( file_path ) )
    {
        std::cerr << "File [" << file_path.toStdString()
                  << "] does not exist."
                  << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               file_path + tr( " does not exist." ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    disconnectMonitor();
    M_log_player->stop();

    if ( ! M_main_data.openRCG( file_path.toStdString() ) )
    {
        QString err_msg = tr( "Failed to read [" );
        err_msg += file_path;
        err_msg += tr( "]" );
        QMessageBox::critical( this,
                               tr( "Error" ),
                               err_msg,
                               QMessageBox::Ok, QMessageBox::NoButton );
        this->setWindowTitle( tr( PACKAGE_NAME ) );
        return;
    }

    if ( M_main_data.viewHolder().monitorViewCont().empty() )
    {
        QString err_msg = tr( "Empty log file [" );
        err_msg += file_path;
        err_msg += tr( "]" );
        QMessageBox::critical( this,
                               tr( "Error" ),
                               err_msg,
                               QMessageBox::Ok, QMessageBox::NoButton );
        this->setWindowTitle( tr( PACKAGE_NAME ) );
        return;
    }

    QFileInfo file_info( file_path );

    Options::instance().setGameLogDir( file_info.absoluteFilePath().toStdString() );

    if ( M_player_type_dialog )
    {
        M_player_type_dialog->updateData();
    }

    if ( M_debug_message_window )
    {
        M_debug_message_window->clearAll();
    }

    if ( M_view_config_dialog )
    {
        M_view_config_dialog->fitToScreen();
    }

    if ( ! Options::instance().anonymousMode() )
    {
        //this->setWindowTitle( file_info.baseName() + tr( " - "PACKAGE_NAME ) );
        QString name = file_info.fileName();
        if ( name.length() > 128 )
        {
            name.replace( 125, name.length() - 125, tr( "..." ) );
        }
        this->setWindowTitle( name + tr( " - " PACKAGE_NAME ) );
    }

    // M_toggle_debug_server_act->setChecked( false );

    emit viewUpdated();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::saveRCG()
{
    if ( M_main_data.viewHolder().monitorViewCont().empty() )
    {
        QMessageBox::warning( this,
                              tr( "Error" ),
                              tr( "No Monitor View Data!" ) );
        return;
    }

    QString default_file_name;
    {
        const MonitorViewData::ConstPtr view = M_main_data.viewHolder().lastMonitorView();

        if ( view )
        {
            default_file_name
                = QDateTime::currentDateTime().toString( "yyyyMMddhhmm-" );

            QString left_team = QString::fromStdString( view->leftTeam().name() );
            QString left_score = QString::number( view->leftTeam().score() );

            QString right_team = QString::fromStdString( view->rightTeam().name() );
            QString right_score = QString::number( view->rightTeam().score() );

            default_file_name += left_team;
            default_file_name += tr( "_" );
            default_file_name += left_score;
            default_file_name += tr( "-" );
            default_file_name += right_team;
            default_file_name += tr( "_" );
            default_file_name += right_score;

            default_file_name += tr( ".rcg" );
        }
    }

#ifdef HAVE_LIBZ
    QString filter( tr( "Game Log files (*.rcg *.rcg.gz);;"
                        "All files (*)" ) );
#else
    QString filter( tr( "Game Log files (*.rcg);;"
                        "All files (*)" ) );
#endif

    QString default_dir
        = QString::fromStdString( Options::instance().gameLogDir() );
    if ( ! default_file_name.isEmpty() )
    {
        default_dir += tr( "/" );
        default_dir += default_file_name;
    }

    QString file_path = QFileDialog::getSaveFileName( this,
                                                      tr( "Save a game log file as" ),
                                                      default_dir,
                                                      filter );

    if ( file_path.isEmpty() )
    {
        std::cerr << __FILE__ << ": (saveRCG) empty file path" << std::endl;
        return;
    }

    std::string file_path_string = file_path.toStdString();

    std::cerr << __FILE__ << ": (saveRCG) "
              << "save game log data to the file = [" << file_path_string
              << ']' << std::endl;

    // update game log dir
    QFileInfo file_info( file_path );
    Options::instance().setGameLogDir( file_info.absolutePath().toStdString() );

    // check gzip usability
    bool is_gzip = false;
    if ( file_path_string.length() > 3
         && file_path_string.compare( file_path_string.length() - 3, 3, ".gz" ) == 0 )
    {
#ifdef HAVE_LIBZ
        if ( file_path_string.length() <= 7
             || file_path_string.compare( file_path_string.length() - 4, 4, ".rcg.gz" ) != 0 )
        {
            file_path_string == ".rcg.gz";
        }
        is_gzip = true;
#else
        // erase '.gz'
        file_path_string.erase( file_path_string.length() - 3 );
#endif
    }

    // check the extention string
    if ( ! is_gzip )
    {
        if ( file_path_string.length() <= 4
             || file_path_string.compare( file_path_string.length() - 4, 4, ".rcg" ) != 0 )
        {
            file_path_string += ".rcg";
        }
    }

    M_main_data.saveRCG( file_path_string );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::openDebugView()
{
    //std::cerr << __FILE__ << ": (openDebugView)" << std::endl;

    QString default_dir
        = QString::fromStdString( Options::instance().debugLogDir() );
#if 0
    QString dir_path
        = QFileDialog::getExistingDirectory( this,
                                             tr( "Choose a debug view log directory" ),
                                             default_dir );
#endif
    DirSelector selector( this,
                          tr( "Choose a debug view log directory" ),
                          default_dir );

    if ( ! selector.exec() )
    {
        std::cerr << __FILE__ << ": (openDebugView) canceled" << std::endl;
        return;
    }

    QString dir_path = selector.dirPath();

    if ( dir_path.isEmpty() )
    {
        std::cerr << __FILE__ << ": (openDebugView) empty dir path" << std::endl;
        return;
    }

    std::cerr << "open the debug view. dir = [" << dir_path.toStdString() << std::endl;

    M_main_data.openDebugView( dir_path.toStdString() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::saveDebugView()
{
    //std::cerr << "MainWindow::saveDebugView()" << std::endl;
#if 0
    QString dir_path
        = QFileDialog::getExistingDirectory( this,
                                             tr( "Choose a directory to save a debug view logs" ) );

#endif
    DirSelector selector( this,
                          tr( "Choose a debug view log directory" ),
                          QDir::currentPath() );

    if ( ! selector.exec() )
    {
        std::cerr << __FILE__ << "(openDebugView) canceled" << std::endl;
        return;
    }

    QString dir_path = selector.dirPath();

    if ( dir_path.isEmpty() )
    {
        std::cerr << __FILE__ << ": (openDebugView() empty dir path" << std::endl;
        return;
    }

    std::cerr << "save debug view. dir = [" << dir_path.toStdString() << std::endl;


    M_main_data.saveDebugView( dir_path.toStdString() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::openDrawData()
{
    QString filter( tr( "Draw data file (*.txt *.csv *.tsv);;"
                        "All files (*)" ) );
    QString default_dir = QString::fromStdString( Options::instance().drawDataFile() );

    QString file_path = QFileDialog::getOpenFileName( this,
                                                      tr( "Choose a draw data file" ),
                                                      default_dir,
                                                      filter );
    if ( file_path.isEmpty() )
    {
        return;
    }

    openDrawData( file_path );
}


/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::openDrawData( const QString & file_path )
{
    if ( ! QFile::exists( file_path ) )
    {
        std::cerr << "File [" << file_path.toStdString() << "] does not exist." << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               file_path + tr( " does not exist." ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    if ( ! M_main_data.openDrawData( file_path.toStdString() ) )
    {
        QString err_msg = tr( "Failed to read [" );
        err_msg += file_path;
        err_msg += tr( "]" );
        QMessageBox::critical( this,
                               tr( "Error" ),
                               err_msg,
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    if ( ! Options::instance().showDrawData() )
    {
        Options::instance().toggleShowDrawData();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::kickOff()
{
    if ( M_monitor_client
         && M_monitor_client->isConnected() )
    {
        M_monitor_client->sendKickOff();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::setLiveMode()
{
    if ( M_monitor_client
         && M_monitor_client->isConnected() )
    {
        M_log_player->setLiveMode();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::connectMonitor()
{
    connectMonitorTo( "127.0.0.1" );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::connectMonitorTo()
{
    if ( M_last_connected_host.isEmpty() )
    {
        M_last_connected_host = "127.0.0.1";
    }

    bool ok = true;
    QString text = QInputDialog::getText( this,
                                          tr( "Input sserver host name" ),
                                          tr( "Host name: "),
                                          QLineEdit::Normal,
                                          M_last_connected_host,
                                          & ok );
    if ( ok
         && ! text.isEmpty() )
    {
        connectMonitorTo( text.toStdString().c_str() );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::connectMonitorTo( const char * hostname )
{
    if ( std::strlen( hostname ) == 0 )
    {
        std::cerr << __FILE__ << ": (connectMonitorTo) "
                  << "Empty host name! Connection failed!" << std::endl;
        return;
    }

    std::cerr << "Connect to rcssserver on [" << hostname << "]" << std::endl;

    M_monitor_client = new MonitorClient( this,
                                          M_main_data,
                                          hostname,
                                          Options::instance().port(),
                                          Options::instance().clientVersion() );

    if ( ! M_monitor_client->isConnected() )
    {
        std::cerr << __FILE__ << ": (connectMonitorTo) "
                  << "Conenction failed." << std::endl;
        delete M_monitor_client;
        M_monitor_client = static_cast< MonitorClient * >( 0 );
        return;
    }

    M_last_connected_host = hostname;

    // reset all data
    M_main_data.clear();

    if ( M_player_type_dialog )
    {
        M_player_type_dialog->hide();
    }

    if ( M_debug_message_window )
    {
        M_debug_message_window->clearAll();
    }

    if ( M_view_config_dialog )
    {
        M_view_config_dialog->fitToScreen();
    }

    if ( M_toggle_debug_server_act->isChecked() )
    {
        std::cerr << "Start the debug server." << std::endl;
        stopDebugServer();
        startDebugServer();
    }

    Options::instance().setMonitorClientMode( true );

    M_save_rcg_act->setEnabled( false );

    M_kick_off_act->setEnabled( true );
    M_set_live_mode_act->setEnabled( true );
    M_connect_monitor_act->setEnabled( false );
    M_connect_monitor_to_act->setEnabled( false );
    M_disconnect_monitor_act->setEnabled( true );
#ifndef Q_WS_WIN
    M_kill_server_act->setEnabled( true );
#endif
    M_toggle_drag_move_mode_act->setEnabled( true );
    //M_show_monitor_move_dialog_act->setEnabled( true );

    M_toggle_debug_server_act->setEnabled( true );
    M_show_image_save_dialog_act->setEnabled( false );

    connect( M_monitor_client, SIGNAL( received() ),
             this, SLOT( receiveMonitorPacket() ) );
    connect( M_monitor_client, SIGNAL( timeout() ),
             this, SLOT( disconnectMonitor() ) );

    M_log_player->setLiveMode();

    M_monitor_client->sendDispInit();

    if ( QApplication::overrideCursor() )
    {
        QApplication::restoreOverrideCursor();
    }

    this->setWindowTitle( tr( PACKAGE_NAME ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::disconnectMonitor()
{
    if ( M_monitor_client )
    {
        M_monitor_client->disconnect();

        disconnect( M_monitor_client, SIGNAL( received() ),
                    this, SLOT( receiveMonitorPacket() ) );

        disconnect( M_monitor_client, SIGNAL( timeout() ),
                    this, SLOT( disconnectMonitor() ) );

        delete M_monitor_client;
        M_monitor_client = static_cast< MonitorClient * >( 0 );
    }

    if ( M_debug_server )
    {
        delete M_debug_server;
        M_debug_server = static_cast< DebugServer * >( 0 );
    }

    Options::instance().setMonitorClientMode( false );

    M_save_rcg_act->setEnabled( true );

    M_kick_off_act->setEnabled( false );
    M_set_live_mode_act->setEnabled( false );
    M_connect_monitor_act->setEnabled( true );
    M_connect_monitor_to_act->setEnabled( true );
    M_disconnect_monitor_act->setEnabled( false );
#ifndef Q_WS_WIN
    M_kill_server_act->setEnabled( false );
#endif
    M_toggle_drag_move_mode_act->setEnabled( false );
    //M_show_monitor_move_dialog_act->setEnabled( false );

    M_toggle_debug_server_act->setEnabled( false );
    M_show_image_save_dialog_act->setEnabled( true );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::killServer()
{
    disconnectMonitor();

    Options::instance().setKillServer( false );

#ifndef Q_WS_WIN
    if ( Options::instance().serverPID() != 0 )
    {
        Options::instance().setServerPID( 0 );
        ::kill( pid_t( Options::instance().serverPID() ), SIGINT );
    }
    else
    {
        //std::system( "killall -INT rcssserver" );
        QString command( "killall -SIGINT rcssserver" );
        QProcess::execute( command );
    }
#endif
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::startServer()
{
    Options::instance().setServerPID( 0 );
    Options::instance().setKillServer( true );

    QString server_command;
    if ( M_server_command.isEmpty() )
    {
        server_command
            = QString::fromStdString( Options::instance().serverPath() );
    }
    else
    {
        server_command = M_server_command;
        M_server_command.resize( 0 );
    }

    if ( server_command.isEmpty() )
    {
        return;
    }

    QProcess::startDetached( server_command );

    if ( ! QApplication::overrideCursor() )
    {
        QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
    }

    QTimer::singleShot( 1000,
                        this, SLOT( connectMonitor() ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::restartServer()
{
    restartServer( Options::instance().serverPath().c_str() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::restartServer( const QString & command )
{
    static bool s_last_auto_start = false;

    M_server_command = command;

    bool auto_start = command.contains( "server::team_l_start" );

    if ( M_monitor_client )
    {
        killServer();

        if ( ! QApplication::overrideCursor() )
        {
            QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
        }

        int timer_msec = 1000;
        if ( s_last_auto_start )
        {
            timer_msec = 3000;
        }

        QTimer::singleShot( timer_msec,
                            this, SLOT( startServer() ) );
    }
    else
    {
        startServer();
    }

    s_last_auto_start = auto_start;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::toggleDragMoveMode( bool on )
{
    if ( M_main_data.trainerData().dragMode() != on )
    {
        M_main_data.getTrainerData().toggleDragMode();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::showLauncherDialog()
{
    if ( M_launcher_dialog )
    {
        M_launcher_dialog->setVisible( ! M_launcher_dialog->isVisible() );
    }
    else
    {
        M_launcher_dialog = new LauncherDialog( this );
        M_launcher_dialog->show();

        QSize size = M_launcher_dialog->size();
        M_launcher_dialog->setMinimumSize( size );
        M_launcher_dialog->setMaximumSize( 1024, size.height() );

        connect( M_launcher_dialog, SIGNAL( launchServer( const QString & ) ),
                 this, SLOT( restartServer( const QString & ) ) );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::toggleMenuBar()
{
    this->menuBar()->setVisible( ! this->menuBar()->isVisible() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::toggleToolBar()
{
    M_log_player_tool_bar->setVisible( ! M_log_player_tool_bar->isVisible() );
    //M_monitor_tool_bar->setVisible( ! M_monitor_tool_bar->isVisible() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::toggleStatusBar()
{
    this->statusBar()->setVisible( ! this->statusBar()->isVisible() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::toggleFullScreen()
{
    if ( this->isFullScreen() )
    {
        this->showNormal();
    }
    else
    {
        this->showFullScreen();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::showPlayerTypeDialog()
{
    if ( M_player_type_dialog )
    {
        M_player_type_dialog->setVisible( ! M_player_type_dialog->isVisible() );
    }
    else
    {
        M_player_type_dialog = new PlayerTypeDialog( this, M_main_data );
        M_player_type_dialog->show();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::showMonitorMoveDialog()
{
    if ( M_monitor_move_dialog )
    {
        M_monitor_move_dialog->setVisible( ! M_monitor_move_dialog->isVisible() );
    }
    else
    {
        M_monitor_move_dialog
            = new MonitorMoveDialog( this,
                                     M_main_data,
                                     M_main_data.getTrainerData() );
        connect( M_monitor_move_dialog, SIGNAL( executed() ),
                 this, SLOT( moveObjects() ) );

        M_monitor_move_dialog->show();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::showDetailDialog()
{
    if ( M_detail_dialog )
    {
        M_detail_dialog->setVisible( ! M_detail_dialog->isVisible() );
    }
    else
    {
        M_detail_dialog = new DetailDialog( this, M_main_data );
        connect( this,  SIGNAL( viewUpdated() ),
                 M_detail_dialog, SLOT( updateLabels() ) );

        M_detail_dialog->show();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::changeStyle( bool checked )
{
    if ( ! checked )
    {
        return;
    }

    QAction * action = qobject_cast< QAction * >( sender() );
    QStyle * style = QStyleFactory::create( action->data().toString() );
    Q_ASSERT( style );

    QApplication::setStyle( style );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::showColorSettingDialog()
{
    ColorSettingDialog dlg( this );

    connect( &dlg, SIGNAL( colorChanged() ),
             M_field_canvas, SLOT( redrawAll() ) );

    dlg.exec();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::showFontSettingDialog()
{
    FontSettingDialog dlg( this );

    connect( &dlg, SIGNAL( fontChanged() ),
             M_field_canvas, SLOT( redrawAll() ) );

    dlg.exec();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::showViewConfigDialog()
{
    M_view_config_dialog->setVisible( ! M_view_config_dialog->isVisible() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::showDebugMessageWindow()
{
    if ( M_debug_message_window )
    {
        M_debug_message_window
            ->setVisible( ! M_debug_message_window->isVisible() );
    }
//     else
//     {
//         M_debug_message_window = new DebugMessageWindow( this,
//                                                          M_main_data );
//         connect( M_debug_message_window, SIGNAL( configured() ),
//                  this, SIGNAL( viewUpdated() ) );

//         M_debug_message_window->show();
//     }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::toggleDebugServer( bool on )
{
    if ( on )
    {
        startDebugServer();
    }
    else
    {
        stopDebugServer();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::startDebugServer()
{
    if ( M_debug_server )
    {
        // the server instance has already existed.
        return;
    }

    int port = static_cast< int >( Options::instance().debugServerPort() );

    M_debug_server = new DebugServer( this, M_main_data, port );

    if ( ! M_debug_server->isConnected() )
    {
        std::cerr << __FILE__ << ": (startDebugServer) "
                  << "failed to create Debug Server" << std::endl;
        M_toggle_debug_server_act->setChecked( false );

        delete M_debug_server;
        M_debug_server = static_cast< DebugServer * >( 0 );
        return;
    }

    M_toggle_debug_server_act->setChecked( true );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::stopDebugServer()
{
    if ( M_debug_server )
    {
        //std::cerr << "Stop Debug Server" << std::endl;
        delete M_debug_server;
        M_debug_server = static_cast< DebugServer * >( 0 );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::showImageSaveDialog()
{
    M_log_player->stop();

    ImageSaveDialog dlg( this,
                         M_field_canvas,
                         M_main_data );

    dlg.exec();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::about()
{
    QString msg( tr( PACKAGE_NAME ) );
    msg += tr( "-" );
    msg += tr( VERSION );
    msg += tr( "\n\n" );
    msg += tr( "soccerwindow2 is a viewer applicaton for\n"
               "the RoboCup Soccer Simulator.\n"
               "  http://sserver.sourceforge.net/\n"
               "\n"
               "soccerwindow2 Development Site:\n"
               "  http://rctools.sourceforge.jp/\n"
               "Author:\n"
               "  Hidehisa Akiyama <akky@users.sourceforge.jp>" );

    QMessageBox::about( this,
                        tr( "About soccerwindow2" ),
                        msg );

    // from Qt 4.1 documents
    /*
      about() looks for a suitable icon in four locations:

      1. It prefers parent->icon() if that exists.
      2. If not, it tries the top-level widget containing parent.
      3. If that fails, it tries the active window.
      4. As a last resort it uses the Information icon.

      The about box has a single button labelled "OK".
    */
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::printShortcutKeys()
{
    QDialog dialog( this );
    QVBoxLayout * layout = new QVBoxLayout();

    QTableWidget * table_widget = new QTableWidget( &dialog );
    table_widget->insertColumn( 0 );
    table_widget->insertColumn( 1 );

    QStringList header;
    header.push_back( tr( "key" ) );
    header.push_back( tr( "action" ) );
    table_widget->setHorizontalHeaderLabels( header );

    table_widget->horizontalHeader()->setStretchLastSection( true );
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    table_widget->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
#else
    table_widget->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
#endif
    table_widget->verticalHeader()->hide();

    int row = 0;

    Q_FOREACH( QAction * act, this->actions() )
    {
        if ( ! act->shortcut().isEmpty() )
        {
            //std::cout << '[' << act->shortcut().toString().toStdString() << "] "
            //          << QString( act->text() ).remove( QChar( '&' ) ).toStdString()
            //    //<< ", " << act->statusTip().toStdString()
            //          << '\n';
            table_widget->insertRow( row );
            table_widget->setItem ( row, 0, new QTableWidgetItem( act->shortcut().toString() ) );
            table_widget->setItem ( row, 1, new QTableWidgetItem( QString( act->statusTip() ).remove( QChar( '&' ) ) ) );
            ++row;
        }
    }

    table_widget->setSortingEnabled( true );
//     std::cerr <<  "table row_count = " << table_widget->rowCount()
//               <<  "table col_count = " << table_widget->columnCount()
//               << std::endl;

    layout->addWidget( table_widget );
    dialog.setLayout( layout );

    if ( row > 0 )
    {
        dialog.exec();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::resizeCanvas( const QSize & size )
{
    if ( size.width() < this->minimumWidth()
         || size.height() < this->minimumHeight() )
    {
        std::cerr << "Too small canvas size ("
                  << size.width() << " "
                  << size.height() << ")"
                  << std::endl;
        return;
    }

    if ( centralWidget() )
    {
        if ( this->isMaximized()
             || this->isFullScreen() )
        {
            this->showNormal();
        }

        QRect rect = this->geometry();

        int width_diff = rect.width() - centralWidget()->width();
        int height_diff = rect.height() - centralWidget()->height();

        rect.setWidth( size.width() + width_diff );
        rect.setHeight( size.height() + height_diff );

        this->setGeometry( rect );

        //std::cerr << "centralWidget width = " << centralWidget()->width()
        //          << " height = " << centralWidget()->height()
        //          << std::endl;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::saveImageAndQuit()
{
    if ( ! this->isVisible() )
    {
        QTimer::singleShot( 500,
                            this, SLOT( saveImageAndQuit() ) );
        return;
    }

    ImageSaveDialog dlg( this,
                         M_field_canvas,
                         M_main_data );
    dlg.show();
    dlg.selectAllCycle();
    dlg.executeSave();

    qApp->quit(); // QApplication::exit( 0 );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::receiveMonitorPacket()
{
    if ( M_log_player->isLiveMode() )
    {
        M_log_player->showLive();
    }
    else
    {
        //std::cerr << "receive monitor packet  no live" << std::endl;
        M_log_player_tool_bar->updateSlider();
    }


    if ( Options::instance().autoQuitMode() )
    {
        if ( M_main_data.viewHolder().lastPlayMode() == rcsc::PM_TimeOver )
        {
            static QDateTime s_game_end_time;

            if ( ! s_game_end_time.isValid() )
            {
                s_game_end_time = QDateTime::currentDateTime();
            }
            else
            {
                if ( s_game_end_time.secsTo( QDateTime::currentDateTime() )
                     >= Options::instance().waitSeconds() )
                {
                    std::cerr << "Elapsed " << Options::instance().waitSeconds()
                              << " seconds after the end of game.\n"
                              << "Exit..."
                              << std::endl;
                    qApp->quit();
                }
            }
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::updatePositionLabel( const QPoint & point )
{
    if ( M_position_label
         && statusBar()
         && statusBar()->isVisible() )
    {
        double x = Options::instance().fieldX( point.x() );
        double y = Options::instance().fieldY( point.y() );

        double value = -std::numeric_limits< double >::max();

        const double grid_size = GridFieldEvaluationData::S_grid_size;
        std::pair< int, int > xy = std::make_pair( static_cast< int >( rint( x / grid_size ) ),
                                                   static_cast< int >( rint( y / grid_size ) ) );
        std::map< std::pair< int, int >, double >::const_iterator
            it = GridFieldEvaluationData::S_xy_value_map.find( xy );
        if ( it != GridFieldEvaluationData::S_xy_value_map.end() )
        {
            value = it->second;
        }

        char buf[128];
        if ( value != -std::numeric_limits< double >::max() )
        {
            snprintf( buf, 128, "(%.2f, %.2f) %f", x, y, value );
        }
        else
        {
            snprintf( buf, 128, "(%.2f, %.2f)", x, y );
        }

        M_position_label->setText( QString::fromLatin1( buf ) );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::dropBallThere()
{
    if ( M_monitor_client
         && M_monitor_client->isConnected() )
    {
        MonitorViewData::ConstPtr view = M_main_data.viewHolder().lastMonitorView();
        if ( view )
        {
            std::cerr << "drop ball at current position "
                      << std::endl;
            M_monitor_client->sendDropBall( view->ball().x(),
                                            view->ball().y() );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::dropBall( const QPoint & point )
{
    if ( M_monitor_client
         && M_monitor_client->isConnected() )
    {
        double x = Options::instance().fieldX( point.x() );
        double y = Options::instance().fieldY( point.y() );

        if ( Options::instance().reverseSide() )
        {
            x = -x;
            y = -y;
        }
        std::cerr << "drop ball to ("
                  << x << ", " << y << ")"
                  << std::endl;
        M_monitor_client->sendDropBall( x, y );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::freeKickLeft( const QPoint & point )
{
    if ( M_monitor_client
         && M_monitor_client->isConnected() )
    {
        double x = Options::instance().fieldX( point.x() );
        double y = Options::instance().fieldY( point.y() );

        if ( Options::instance().reverseSide() )
        {
            x = -x;
            y = -y;
            M_monitor_client->sendFreeKickRight( x, y );
        }
        else
        {
            //std::cerr << "free kick left at ("
            //          << x << ", " << y << ")"
            //          << std::endl;
            M_monitor_client->sendFreeKickLeft( x, y );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::freeKickRight( const QPoint & point )
{
    if ( M_monitor_client
         && M_monitor_client->isConnected() )
    {
        double x = Options::instance().fieldX( point.x() );
        double y = Options::instance().fieldY( point.y() );

        if ( Options::instance().reverseSide() )
        {
            x = -x;
            y = -y;
            M_monitor_client->sendFreeKickLeft( x, y );
        }
        else
        {
            //std::cerr << "free kick right at ("
            //          << x << ", " << y << ")"
            //          << std::endl;
            M_monitor_client->sendFreeKickRight( x, y );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::movePlayer( const QPoint & point )
{
    if ( M_monitor_client
         && M_monitor_client->isConnected() )
    {
        rcsc::SideID side = M_main_data.trainerData().draggedPlayerSide();
        int unum = M_main_data.trainerData().draggedPlayerNumber();

        if ( side != rcsc::NEUTRAL
             && unum != 0 )
        {
            double x = Options::instance().fieldX( point.x() );
            double y = Options::instance().fieldY( point.y() );
            if ( Options::instance().reverseSide() )
            {
                x = -x;
                y = -y;
            }

            //std::cerr << "move player to " << x << ' ' << y << std::endl;
            M_main_data.getTrainerData().unsetDrag();
            M_monitor_client->sendMove( side, unum, x, y, 0.0 );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::moveObjects()
{
    if ( ! M_monitor_client
         || ! M_monitor_client->isConnected() )
    {
        return;
    }

    TrainerData & data = M_main_data.getTrainerData();

    // ball
    if ( data.ballPos().isValid() )
    {
        if ( rcsc::ServerParam::i().coachMode()
             || rcsc::ServerParam::i().coachWithRefereeMode() )
        {
            if ( data.ballVel().isValid() )
            {
                M_monitor_client->sendTrainerMoveBall( data.ballPos().x,
                                                       data.ballPos().y,
                                                       data.ballVel().x,
                                                       data.ballVel().y );
            }
            else
            {
                M_monitor_client->sendTrainerMoveBall( data.ballPos().x,
                                                       data.ballPos().y );
            }
        }
        else
        {
            if ( data.playMode() == rcsc::PM_FreeKick_Left )
            {
                M_monitor_client->sendFreeKickLeft( data.ballPos().x,
                                                    data.ballPos().y );
            }
            else if ( data.playMode() == rcsc::PM_FreeKick_Right )
            {
                M_monitor_client->sendFreeKickRight( data.ballPos().x,
                                                     data.ballPos().y );
            }
            else
            {
                M_monitor_client->sendDropBall( data.ballPos().x,
                                                data.ballPos().y );
            }
        }
    }

    // left
    for ( int unum = 1; unum <= 11; ++unum )
    {
        rcsc::Vector2D pos = data.playerMovePos( rcsc::LEFT, unum );
        rcsc::AngleDeg body = data.playerBody( rcsc::LEFT, unum );
        if ( pos.isValid() )
        {
            M_monitor_client->sendMove( rcsc::LEFT, unum,
                                        pos.x, pos.y,
                                        body.degree() );
        }
    }

    // right
    for ( int unum = 1; unum <= 11; ++unum )
    {
        rcsc::Vector2D pos = data.playerMovePos( rcsc::RIGHT, unum );
        rcsc::AngleDeg body = data.playerBody( rcsc::RIGHT, unum );
        if ( pos.isValid() )
        {
            M_monitor_client->sendMove( rcsc::RIGHT, unum,
                                        pos.x, pos.y,
                                        body.degree() );
        }
    }

}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::yellowCard()
{
    if ( ! M_monitor_client
         || ! M_monitor_client->isConnected() )
    {
        return;
    }

    int unum = Options::instance().selectedNumber();
    if ( unum == 0 )
    {
        return;
    }

    rcsc::SideID side = rcsc::LEFT;
    if ( unum < 0 )
    {
        unum = -unum;
        side = rcsc::RIGHT;
    }

    M_monitor_client->sendCard( side, unum, rcsc::YELLOW );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MainWindow::redCard()
{
    if ( ! M_monitor_client
         || ! M_monitor_client->isConnected() )
    {
        return;
    }

    int unum = Options::instance().selectedNumber();
    if ( unum == 0 )
    {
        return;
    }

    rcsc::SideID side = rcsc::LEFT;
    if ( unum < 0 )
    {
        unum = -unum;
        side = rcsc::RIGHT;
    }

    M_monitor_client->sendCard( side, unum, rcsc::RED );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::changePlayMode( int mode,
                            const QPoint & point )
{
    if ( ! M_monitor_client
         || ! M_monitor_client->isConnected() )
    {
        return;
    }

    if ( ! rcsc::ServerParam::i().coachMode()
         && ! rcsc::ServerParam::i().coachWithRefereeMode() )
    {
        QMessageBox::warning( this,
                              tr( "PlayMode Change" ),
                              tr( "Server is not running with coach mode." ) );
        return;
    }

    rcsc::PlayMode pmode = static_cast< rcsc::PlayMode >( mode );

    double x = Options::instance().fieldX( point.x() );
    double y = Options::instance().fieldY( point.y() );

    x = std::min( x, + rcsc::ServerParam::i().pitchHalfLength() );
    x = std::max( x, - rcsc::ServerParam::i().pitchHalfLength() );
    y = std::min( y, + rcsc::ServerParam::i().pitchHalfWidth() );
    y = std::max( y, - rcsc::ServerParam::i().pitchHalfWidth() );

    switch ( pmode ) {
    case rcsc::PM_BeforeKickOff:
        M_monitor_client->sendTrainerMoveBall( 0.0, 0.0, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_PlayOn:
        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_KickIn_Left:
    case rcsc::PM_KickIn_Right:
        y = ( y > 0.0
              ? + rcsc::ServerParam::i().pitchHalfWidth()
              : - rcsc::ServerParam::i().pitchHalfWidth() );

        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_CornerKick_Left:
        x = + rcsc::ServerParam::i().pitchHalfLength()
            - rcsc::ServerParam::i().cornerKickMargin();
        y = ( y > 0.0
              ? ( + rcsc::ServerParam::i().pitchHalfWidth()
                  - rcsc::ServerParam::i().cornerKickMargin() )
              : ( - rcsc::ServerParam::i().pitchHalfWidth()
                  + rcsc::ServerParam::i().cornerKickMargin() ) );

        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_CornerKick_Right:
        x = - rcsc::ServerParam::i().pitchHalfLength()
            + rcsc::ServerParam::i().cornerKickMargin();
        y = ( y > 0.0
              ? ( + rcsc::ServerParam::i().pitchHalfWidth()
                  - rcsc::ServerParam::i().cornerKickMargin() )
              : ( - rcsc::ServerParam::i().pitchHalfWidth()
                  + rcsc::ServerParam::i().cornerKickMargin() ) );

        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_GoalKick_Left:
        x = - rcsc::ServerParam::i().pitchHalfLength()
            + rcsc::ServerParam::i().goalAreaLength();
        y = ( y > 0.0
              ? + rcsc::ServerParam::i().goalAreaHalfWidth()
              : - rcsc::ServerParam::i().goalAreaHalfWidth() );

        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_GoalKick_Right:
        x = + rcsc::ServerParam::i().pitchHalfLength()
            - rcsc::ServerParam::i().goalAreaLength();
        y = ( y > 0.0
              ? + rcsc::ServerParam::i().goalAreaHalfWidth()
              : - rcsc::ServerParam::i().goalAreaHalfWidth() );

        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_Foul_Charge_Left:
        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_Foul_Charge_Right:
        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_Foul_Push_Left:
        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_Foul_Push_Right:
        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_Back_Pass_Right:
        if ( x < ( + rcsc::ServerParam::i().pitchHalfLength()
                   - rcsc::ServerParam::i().penaltyAreaLength() ) )
        {
            x = + rcsc::ServerParam::i().pitchHalfLength()
                - rcsc::ServerParam::i().penaltyAreaLength();
        }

        if ( std::fabs( y ) > rcsc::ServerParam::i().penaltyAreaHalfWidth() )
        {
            y = ( y > 0.0
                  ? + rcsc::ServerParam::i().penaltyAreaHalfWidth()
                  : - rcsc::ServerParam::i().penaltyAreaHalfWidth() );
        }

        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_IndFreeKick_Left:
        if ( x >= ( + rcsc::ServerParam::i().pitchHalfLength()
                    - rcsc::ServerParam::i().goalAreaLength() )
             && std::fabs( y ) <= rcsc::ServerParam::i().goalAreaHalfWidth() )
        {
            x = + rcsc::ServerParam::i().pitchHalfLength()
                - rcsc::ServerParam::i().goalAreaLength();
        }

        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_Back_Pass_Left:
        if ( x > ( - rcsc::ServerParam::i().pitchHalfLength()
                   + rcsc::ServerParam::i().penaltyAreaLength() ) )
        {
            x = - rcsc::ServerParam::i().pitchHalfLength()
                + rcsc::ServerParam::i().penaltyAreaLength();
        }

        if ( std::fabs( y ) > rcsc::ServerParam::i().penaltyAreaHalfWidth() )
        {
            y = ( y > 0.0
                  ? + rcsc::ServerParam::i().penaltyAreaHalfWidth()
                  : - rcsc::ServerParam::i().penaltyAreaHalfWidth() );
        }

        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    case rcsc::PM_IndFreeKick_Right:
        if ( x <= ( - rcsc::ServerParam::i().pitchHalfLength()
                    + rcsc::ServerParam::i().goalAreaLength() )
             && std::fabs( y ) <= rcsc::ServerParam::i().goalAreaHalfWidth() )
        {
            x = - rcsc::ServerParam::i().pitchHalfLength()
                + rcsc::ServerParam::i().goalAreaLength();
        }

        M_monitor_client->sendTrainerMoveBall( x, y, 0.0, 0.0 );
        M_monitor_client->sendChangeMode( pmode );
        break;
    default:
        QMessageBox::warning( this,
                              tr( "PlayMode Change" ),
                              tr( "Unsupported playmode " ) );
        break;
    }
}
