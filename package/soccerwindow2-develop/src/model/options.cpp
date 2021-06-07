// -*-c++-*-

/*!
  \file options.cpp
  \brief global option holder class Source File.
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

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "soccerwindow2"
#endif
#ifndef VERSION
#define VERSION "unknown-version"
#endif

#include "options.h"

#include <rcsc/common/server_param.h>
#include <rcsc/param/param_map.h>
#include <rcsc/param/cmd_line_parser.h>
#include <rcsc/math_util.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>

const double Options::MIN_FIELD_SCALE = 1.0;
const double Options::MAX_FIELD_SCALE = 5000.0;

const double Options::ZOOM_RATIO = 1.5;

const int Options::MIN_SCORE_BOARD_HEIGHT = 20;
const int Options::MAX_SCORE_BOARD_HEIGHT = 60;

const int Options::DEFAULT_TIMER_INTERVAL = 100;

const int Options::CURSOR_TIMEOUT = 5000;

/*-------------------------------------------------------------------*/
/*!
  singleton interface
*/
Options &
Options::instance()
{
    static Options s_instance;
    return s_instance;
}

/*-------------------------------------------------------------------*/
/*!

 */
Options::Options()
    // monitor client options
    : M_connect( false ),
      M_host( "127.0.0.1" ),
      M_port( 6000 ),
      M_client_version( 4 ),
      M_wait_seconds( 5 ),
      M_auto_quit_mode( false ),
      M_kill_server( false ),
      M_server_pid( 0 ),
      M_server_path( "rcssserver" ),
      M_time_shift_replay( true ),
      // logplayer options
      M_game_log_file_path( "" ),
      M_game_log_dir( "" ),
      M_auto_loop_mode( false ),
      M_timer_interval( DEFAULT_TIMER_INTERVAL ),
      // window options
      M_pos_x( -1 ),
      M_pos_y( -1 ),
      M_width( -1 ),
      M_height( -1 ),
      M_maximize( false ),
      M_full_screen( false ),
      M_tool_bar_area( "top" ),
      M_hide_menu_bar( false ),
      M_hide_tool_bar( false ),
      M_hide_status_bar( false ),
      // screen state
      M_canvas_width( -1 ),
      M_canvas_height( -1 ),
      M_field_center( 0.0, 0.0 ),
      M_field_scale( 8.0 ),
      M_score_board_font_height( 14 ),
      M_score_board_height( 0 ),
      M_zoomed( false ),
      M_focus_type( FOCUS_POINT ),
      M_focus_point( 0.0, 0.0 ),
      M_selected_number( 0 ),
      M_agent_select_type( SELECT_UNSELECT ),
      M_team_graphic_scale( 1.0 ),
      // view options
      M_anonymous_mode( false ),
      M_field_grass_type( GRASS_NORMAL ),
      M_paint_style( PAINT_DEFAULT ),
      M_keepaway_mode( false ),
      M_anti_aliasing( true ),
      M_gradient( false ),
      M_cursor_hide( false ),
      M_reverse_side( false ),
      M_player_reverse_draw( false ),
      M_show_score_board( true ),
      M_show_team_graphic( true ),
      M_show_flags( false ),
      M_show_draw_data( false ),
      M_show_grid_coord( true ),
      M_grid_step( 0 ),
      M_show_ball( true ),
      M_show_players( true ),
      M_show_player_number( true ),
      M_show_player_type( false ),
      M_show_view_area( true ),
      M_show_illegal_defense_state( true ),
      M_show_pointto( true ),
      M_show_attentionto( false ),
      M_show_stamina( true ),
      M_show_stamina_capacity( false ),
      M_show_body_shadow( true ),
      M_show_catchable_area( false ),
      M_show_tackle_area( false ),
      M_show_kick_accel_area( false ),
      M_show_offside_line( false ),
      M_show_card( true ),
      M_enlarge_mode( true ),
      M_ball_size( 0.35 ),
      M_player_size( 0.0 ),
      M_show_voronoi_diagram( false ),
      M_show_delaunay_triangulation( false ),
      M_voronoi_target( rcsc::NEUTRAL ),
      M_ball_auto_trace( false ),
      M_ball_trace_start( 0 ),
      M_ball_trace_end( 0 ),
      M_player_auto_trace( false ),
      M_player_trace_start( 0 ),
      M_player_trace_end( 0 ),
      M_auto_trace_start( -10 ),
      M_auto_trace_period( 20 ),
      M_line_trace( true ),
      M_ball_future_cycle( 0 ),
      M_player_future_cycle( 0 ),
      M_player_movable_cycle( 0 ),
      // debug server options
      M_debug_server_mode( false ),
      M_debug_server_port( 6000 + 32 ),
      M_debug_log_dir( "/tmp" ),
      // debug view options
      M_show_debug_view( true ),
      M_show_debug_view_ball( true ),
      M_show_debug_view_self( true ),
      M_show_debug_view_teammates( true ),
      M_show_debug_view_opponents( true ),
      M_show_debug_view_comment( true ),
      M_show_debug_view_shape( true ),
      M_show_debug_view_target( true ),
      M_show_debug_view_message( true ),
      M_show_debug_log_objects( true ),
      // evaluator
      M_offline_team_command_left( "" ),
      M_offline_team_command_right( "" ),
      M_evaluator_command_left( "" ),
      M_evaluator_command_right( "" ),
      M_evaluator_name( "" ), // "SIRMsModelFieldEvaluator" ),
      M_evaluator_param_file_left( "" ),
      M_evaluator_param_file_right( "" ),
      M_evaluator_grid_size( 0.5 ),
      M_evaluation_view_min_rate( 0.0 ),
      M_evaluation_view_max_rate( 1.0 ),
      // image save options
      M_auto_image_save( false ),
      M_image_save_dir( "" ),
      M_image_name_prefix( "image-" ),
      M_image_save_format( "PNG" ),
      // files
      M_intercept_decision_file( "intercept_decision.csv" ),
      M_intercept_evaluate_file( "intercept_evaluate.csv" ),
      M_draw_data_file( "" ),
      // other options
      M_monitor_client_mode( false ),
      M_mouse_measure_mode( MEASURE_BALL_MOVE ),
      M_mouse_measure_first_length( -1.0 )
{

}

/*-------------------------------------------------------------------*/
/*!

 */
Options::~Options()
{

}

/*-------------------------------------------------------------------*/
/*!
  analyze command line options
*/
bool
Options::parseCmdLine( int argc,
                       char ** argv )
{
    rcsc::ParamMap system_options( "System Options" );
    rcsc::ParamMap client_options( "Monitor Client Options" );
    rcsc::ParamMap logplayer_options( "Log Player Options" );
    rcsc::ParamMap window_options( "Window Options" );
    rcsc::ParamMap view_options( "View Options" );
    rcsc::ParamMap debug_server_options( "Debug Server Options" );
    rcsc::ParamMap debug_view_options( "Debug View Options" );
    rcsc::ParamMap evaluator_options( "Evaluator Options" );
    rcsc::ParamMap image_options( "Image Save Options" );
    rcsc::ParamMap file_options( "File Options" );

    bool help = false;
    bool version = false;
    std::string geometry;
    std::string canvas_size;
    std::string field_grass_type;
    std::string paint_style;

    system_options.add()
        ( "help", "h",
          rcsc::BoolSwitch( &help ),
          "print this message." )
        ( "version", "v",
          rcsc::BoolSwitch( &version ),
          "print version." )
        ;

    client_options.add()
        ( "connect", "c",
          rcsc::BoolSwitch( &M_connect ),
          "start as a soccer monitor." )
        ( "host", "",
          &M_host,
          "set host name to be connected." )
        ( "port", "",
          &M_port,
          "set port number for the monitor client." )
        ( "client-version", "",
          &M_client_version,
          "set a monitor client protocol version." )
        ( "wait-seconds", "",
          &M_wait_seconds,
          "set maximal seconds to wait a message from rcssserver." )
        ( "auto-quit-mode", "",
          rcsc::BoolSwitch( &M_auto_quit_mode ),
          "enable automatic quit after game end." )
        ( "kill-server", "",
          rcsc::BoolSwitch( &M_kill_server ),
          "kill rcssserver when soccerwindow2 is quit." )
        ( "server-pid", "",
          &M_server_pid,
          "set rcssserver process ID to kill it." )
        ( "server-path", "",
          &M_server_path,
          "set a rcssserver command line." )
        ( "time-shift-replay", "",
          &M_time_shift_replay,
          "enable time shift replay mode." )
        ;

    logplayer_options.add()
        ( "game-log", "l",
          &M_game_log_file_path,
          "set the path to Game Log file(.rcg) to be loaded.")
        ( "game-log-dir", "",
          &M_game_log_dir,
          "set a default path where game log files exist." )
        ( "auto-loop-mode", "",
          rcsc::BoolSwitch( &M_auto_loop_mode ),
          "enable automatic replay loop." )
        ( "timer-interval", "",
          &M_timer_interval,
          "set the logplayer timer interval [msec]." )
        ;

    window_options.add()
        ( "geometry", "",
          &geometry,
          "specifies the window geometry ([WxH][+X+Y]). e.g. --geometry=1024x768+1+1" )
        //         ( "pos-x", "",
        //           &M_pos_x,
        //           "set left x position of the main window." )
        //         ( "pos-y", "",
        //           &M_pos_y,
        //           "set top y position of the main window." )
        //         ( "width", "",
        //           &M_width,
        //           "set width of the main window." )
        //         ( "height", "",
        //           &M_height,
        //           "set height of the main window." )
        ( "maximize", "",
          rcsc::BoolSwitch( &M_maximize ),
          "start with a maximized frame." )
        ( "full-screen", "",
          rcsc::BoolSwitch( &M_full_screen ),
          "start with a full screen frame." )
        ( "canvas-size", "",
          &canvas_size,
          "specifies the canvas size(WxH). e.g. --canvas-size=1024x768" )
        ( "tool-bar-area", "",
          &M_tool_bar_area,
          "specify the initial tool bar position {top,bottom,left,right}." )
        ( "hide-menu-bar", "",
          rcsc::BoolSwitch( &M_hide_menu_bar ),
          "start without a menu bar (available only by Qt3 version)." )
        ( "hide-tool-bar", "",
          rcsc::BoolSwitch( &M_hide_tool_bar ),
          "start without a tool bar." )
        ( "hide-status-bar", "",
          rcsc::BoolSwitch( &M_hide_status_bar ),
          "start without a status bar." )
        ;

    view_options.add()
        ( "hide-cursor", "",
          rcsc::BoolSwitch( &M_cursor_hide ),
          "hide cursor icon if no operations" )
        ( "anonymous-mode", "",
          rcsc::BoolSwitch( &M_anonymous_mode ),
          "hide team names." )
        ( "paint-style", "",
          &paint_style,
          "set a paint style {default,rcssmonitor}." )
        ( "field-grass-type", "",
          &field_grass_type,
          "set a field grass type {mono,lines,checker}." )
        ( "keepaway-mode", "",
          rcsc::BoolSwitch( &M_keepaway_mode ),
          "start as a keepaway mode monitor." )
        ( "show-score-board", "",
          rcsc::BoolSwitch( &M_show_score_board ),
          "show score board." )
        ( "hide-score-board", "",
          rcsc::NegateSwitch( &M_show_score_board ),
          "hide score board." )
        ( "show-team-graphic", "",
          rcsc::BoolSwitch( &M_show_team_graphic ),
          "show team graphic." )
        ( "hide-team-graphic", "",
          rcsc::NegateSwitch( &M_show_team_graphic ),
          "hide team graphic." )
        ( "anti-aliasing", "",
          &M_anti_aliasing,
          "set an anti-aliasing draw mode (only for Qt4 version)." )
        ( "gradient", "",
          &M_gradient,
          "set a gradient draw mode (only for Qt4 version)." )
        ( "reverse-side", "",
          rcsc::BoolSwitch( &M_reverse_side ),
          "reverse left<->right." )
        ( "player-reverse-draw", "",
          rcsc::BoolSwitch( &M_player_reverse_draw ),
          "reverse the player draw order." )
        ( "show-player-number", "",
          rcsc::BoolSwitch( &M_show_player_number ),
          "show player\'s uniform number." )
        ( "hide-player-number", "",
          rcsc::NegateSwitch( &M_show_player_number ),
          "hide player\'s uniform number." )
        ( "show-player-type", "",
          rcsc::BoolSwitch( &M_show_player_type ),
          "show player\'s heterogeneous type ID." )
        ( "hide-player-type", "",
          rcsc::NegateSwitch( &M_show_player_type ),
          "hide player\'s heterogeneous type ID." )
        ( "show-view-area", "",
          rcsc::BoolSwitch( &M_show_view_area ),
          "show player\'s view area." )
        ( "show-illegal-defense-state", "",
          rcsc::BoolSwitch( &M_show_illegal_defense_state ),
          "show player\'s illegal defense state." )
        ( "hide-view-area", "",
          rcsc::NegateSwitch( &M_show_view_area ),
          "show player\'s view area." )
        ( "show-pointto", "",
          rcsc::BoolSwitch( &M_show_pointto ),
          "show player\'s pointto information." )
        ( "hide-pointto", "",
          rcsc::NegateSwitch( &M_show_pointto ),
          "hide player\'s pointto information." )
        ( "show-attentionto", "",
          rcsc::BoolSwitch( &M_show_attentionto ),
          "show player\'s attentionto information." )
        ( "hide-attentionto", "",
          rcsc::NegateSwitch( &M_show_attentionto ),
          "hide player\'s attentionto information." )
        ( "show-stamina", "",
          rcsc::BoolSwitch( &M_show_stamina ),
          "show player\'s stamina." )
        ( "hide-stamina", "",
          rcsc::NegateSwitch( &M_show_stamina ),
          "hide player\'s stamina." )
        ( "show-stamina-capacity", "",
          rcsc::BoolSwitch( &M_show_stamina_capacity ),
          "show player\'s stamina capacity." )
        ( "hide-stamina-capacity", "",
          rcsc::NegateSwitch( &M_show_stamina_capacity ),
          "hide player\'s stamina capacity." )
        ( "show-card", "",
          rcsc::BoolSwitch( &M_show_card ),
          "show player\'s card status." )
        ( "hide-card", "",
          rcsc::NegateSwitch( &M_show_card ),
          "hide player\'s card status." )
        ( "enlarge-mode", "",
          rcsc::BoolSwitch( &M_enlarge_mode ),
          "show enlarged objects." )
        ( "ball-size", "",
          &M_ball_size,
          "set a ball radius in enlarge mode." )
        ( "player-size", "",
          &M_player_size,
          "set a fixed player radius in enlarge mode." )
        ;

    debug_server_options.add()
        ( "debug-server-mode", "d",
          rcsc::BoolSwitch( &M_debug_server_mode ),
          "start as a debug server." )
        ( "debug-server-port", "",
          &M_debug_server_port,
          "set port number for the debug server." )
        ( "debug-log-dir", "",
          &M_debug_log_dir,
          "set the default log file location." )
        ;

    debug_view_options.add()
        ( "show-debug-view", "",
          rcsc::BoolSwitch( &M_show_debug_view ),
          "show debug view information.")
        ( "hide-debug-view", "",
          rcsc::NegateSwitch( &M_show_debug_view ),
          "hide debug view information.")
        ( "show-debug-view-ball", "",
          rcsc::BoolSwitch( &M_show_debug_view_ball ),
          "show debug view ball information.")
        ( "hide-debug-view-ball", "",
          rcsc::NegateSwitch( &M_show_debug_view_ball ),
          "hide debug view ball information.")
        ( "show-debug-view-self", "",
          rcsc::BoolSwitch( &M_show_debug_view_self ),
          "show debug view self information.")
        ( "hide-debug-view-self", "",
          rcsc::NegateSwitch( &M_show_debug_view_self ),
          "hide debug view self information.")
        ( "show-debug-view-teammates", "",
          rcsc::BoolSwitch( &M_show_debug_view_teammates ),
          "show debug view teammate information.")
        ( "hide-debug-view-teammates", "",
          rcsc::NegateSwitch( &M_show_debug_view_teammates ),
          "hide debug view teammate information.")
        ( "show-debug-view-opponents", "",
          rcsc::BoolSwitch( &M_show_debug_view_opponents ),
          "show debug view opponent information.")
        ( "hide-debug-view-opponents", "",
          rcsc::NegateSwitch( &M_show_debug_view_opponents ),
          "hide debug view opponent information.")
        ( "show-debug-view-comment", "",
          rcsc::BoolSwitch( &M_show_debug_view_comment ),
          "show debug view comment information.")
        ( "hide-debug-view-comment", "",
          rcsc::NegateSwitch( &M_show_debug_view_comment ),
          "hide debug view comment information.")
        ( "show-debug-view-shape", "",
          rcsc::BoolSwitch( &M_show_debug_view_shape ),
          "show debug view shape information.")
        ( "hide-debug-view-shape", "",
          rcsc::NegateSwitch( &M_show_debug_view_shape ),
          "hide debug view shape information.")
        ( "show-debug-view-target", "",
          rcsc::BoolSwitch( &M_show_debug_view_target ),
          "show debug view target information.")
        ( "hide-debug-view-target", "",
          rcsc::NegateSwitch( &M_show_debug_view_target ),
          "hide debug view target information.")
        ( "show-debug-view-message", "",
          rcsc::BoolSwitch( &M_show_debug_view_message ),
          "show debug view message information.")
        ( "hide-debug-view-message", "",
          rcsc::NegateSwitch( &M_show_debug_view_message ),
          "hide debug view message information.")
        ( "show-debug-log-objects", "",
          rcsc::BoolSwitch( &M_show_debug_log_objects ),
          "show misc data in debug log files.")
        ( "hide-debug-log-objects", "",
          rcsc::NegateSwitch( &M_show_debug_log_objects ),
          "hide misc data in debug log files.")
        ;

    evaluator_options.add()
        ( "offline-team-command-left", "",
          &M_offline_team_command_left,
          "set an offline team command for the left team." )
        ( "offline-team-command-right", "",
          &M_offline_team_command_right,
          "set an offline team command for the right team." )
        ( "evaluator-command-left", "",
          &M_evaluator_command_left,
          "set an evaluator command for the left team." )
        ( "evaluator-command-right", "",
          &M_evaluator_command_left,
          "set an evaluator command for the right team." )
        ( "evaluator-name", "",
          &M_evaluator_name,
          "set an evaluator type name {ParamFieldEvaluator,SIRMsModelFieldEvaluator}")
        ( "evaluator-param-file-left", "",
          &M_evaluator_param_file_left,
          "set an evaluator parameter file or directory path for the left team." )
        ( "evaluator-param-file-right", "",
          &M_evaluator_param_file_right,
          "set an evaluator parameter file or directory path for the right team." )
        ( "evaluator-grid-size", "",
          &M_evaluator_grid_size,
          "set the evaluator grid size." )
        ;

    image_options.add()
        ( "auto-image-save", "",
          rcsc::BoolSwitch( &M_auto_image_save ),
          "automatically saved image and quit application." )
        ( "image-save-dir", "",
          &M_image_save_dir,
          "set a default path where image files are saved." )
        ( "image-name-prefix", "",
          &M_image_name_prefix,
          "set a default prefix of the image file name." )
        ( "image-save-format", "",
          &M_image_save_format,
          "set a default image format type." )
        ;

    file_options.add()
        //     ( "intercept-decision-file", "",
        //       &M_intercept_decision_file_path,
        //       "training data file for intercept decision." )
        //     ( "intercept-evaluate-file", "",
        //       &M_intercept_evaluate_file_path,
        //       "training data file for intercept evaluation." )
        ( "draw-data-file", "",
          &M_draw_data_file,
          "the path to an additiona draw data file")
        ;

    rcsc::CmdLineParser parser( argc, argv );

    parser.parse( system_options );
    parser.parse( client_options );
    parser.parse( logplayer_options );
    parser.parse( window_options );
    parser.parse( view_options );
    parser.parse( debug_server_options );
    parser.parse( debug_view_options );
    parser.parse( evaluator_options );
    parser.parse( image_options );
    parser.parse( file_options );


    if ( help
         || parser.failed()
         || parser.positionalOptions().size() > 1 )
    {
        if ( parser.failed() )
        {
            std::cout << "Detect unsupported options:\n";
            parser.printOptionNameArgs( std::cout, '\n' );
            std::cout << std::endl;
        }

        std::cout << "Usage: " << PACKAGE_NAME
                  << " [options ... ] [<GameLogFile>]\n";
        system_options.printHelp( std::cout, false );
        client_options.printHelp( std::cout );
        logplayer_options.printHelp( std::cout );
        window_options.printHelp( std::cout );
        view_options.printHelp( std::cout );
        debug_server_options.printHelp( std::cout );
        debug_view_options.printHelp( std::cout );
        evaluator_options.printHelp( std::cout );
        image_options.printHelp( std::cout );
        file_options.printHelp( std::cout );
        return false;
    }

    if ( version )
    {
        std::cout << PACKAGE_NAME << " Version " << VERSION
                  << std::endl;
        return false;
    }


    //
    // initialize other variables using parsed values.
    //

    if ( M_game_log_file_path.empty()
         && ! parser.positionalOptions().empty() )
    {
        M_game_log_file_path = parser.positionalOptions().front();
    }

    if ( M_timer_interval < 0 )
    {
        std::cerr << "Illegal timer interval " << M_timer_interval
                  << ". set default value." << std::endl;
        M_timer_interval = DEFAULT_TIMER_INTERVAL;
    }

    if ( M_timer_interval < 5 )
    {
        std::cerr << "Too small timer interval " << M_timer_interval
                  << ". The minimum value is 5 [ms]." << std::endl;
        M_timer_interval = 5;
    }

    if ( M_timer_interval > 5000 )
    {
        std::cerr << "Too huge timer interval " << M_timer_interval
                  << ". The maximum value is 5000 [ms]." << std::endl;
        M_timer_interval = 5000;
    }

    if ( ! geometry.empty() )
    {
        int w = -1, h = -1;
        int x = -1, y = -1;

        int num = std::sscanf( geometry.c_str(),
                               " %d x %d %d %d " ,
                               &w, &h, &x, &y );
        //std::cerr << "geometry = " << geometry
        //          << "  param_num=" << num
        //          << " width=" << w << " height=" << h
        //          << " x=" << x << " y=" << y
        //          << std::endl;
        if ( num == 4 || num == 2 )
        {
            if ( w <= 0 || h <= 0 )
            {
                std::cerr << "Illegal window size [" << geometry
                          << "]" << std::endl;
            }
            else
            {
                M_width = w;
                M_height = h;
                M_pos_x = x;
                M_pos_y = y;
            }
        }
        else if ( std::sscanf( geometry.c_str(),
                               " %d %d " ,
                               &x, &y ) == 2 )
        {
            //std::cerr << "only pos = "
            //          << " x=" << x
            //          << " y=" << y
            //          << std::endl;
            M_pos_x = x;
            M_pos_y = y;
        }
        else
        {
            std::cerr << "Illegal window geometry [" << geometry
                      << "]" << std::endl;
        }
    }

    if ( ! canvas_size.empty() )
    {
        int w = -1, h = -1;
        if ( std::sscanf( canvas_size.c_str(),
                          " %d x %d ",
                          &w, &h ) == 2
             && w > 1
             && h > 1 )
        {
            M_canvas_width = w;
            M_canvas_height = h;
        }
        else
        {
            std::cerr << "Illegal canvas size [" << canvas_size
                      << "]" << std::endl;
        }
    }

    if ( ! field_grass_type.empty() )
    {
        if ( field_grass_type == "mono" )
        {
            M_field_grass_type = GRASS_NORMAL;
        }
        else if ( field_grass_type == "lines" )
        {
            M_field_grass_type = GRASS_LINES;
        }
        else if ( field_grass_type == "checker" )
        {
            M_field_grass_type = GRASS_CHECKER;
        }
        else
        {
            std::cerr << "Unsupported field grass type name ["
                      << field_grass_type << "]"
                      << std::endl;
        }
    }

    if ( ! paint_style.empty() )
    {
        if ( paint_style == "default"
             || paint_style == "soccerwindow2" )
        {
            M_paint_style = PAINT_DEFAULT;
        }
        else if ( paint_style == "rcssmonitor" )
        {
            M_paint_style = PAINT_RCSSMONITOR;
        }
        else
        {
            std::cerr << "Unsupported draw type name ["
                      << paint_style << "]"
                      << std::endl;
        }
    }

    if ( M_ball_size <= 0.001 ) M_ball_size = 0.001;

    if ( M_player_size < 0.0 ) M_player_size = 0.0;

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::updateFieldSize( const int canvas_width,
                          const int canvas_height )
{
    static bool s_score_board = true;
    static Options::PaintStyle s_paint_style = Options::PAINT_DEFAULT;

    if ( M_canvas_width != canvas_width
         || M_canvas_height != canvas_height
         || Options::instance().showScoreBoard() != s_score_board
         || Options::instance().paintStyle() != s_paint_style )
    {
        M_canvas_width = canvas_width;
        M_canvas_height = canvas_height;

        s_score_board = Options::instance().showScoreBoard();
        s_paint_style = Options::instance().paintStyle();
        // adjust score board size
        updateScoreBoardSize( canvas_width, canvas_height );

        // draw area height
        int field_height = canvas_height - scoreBoardHeight();

        // adjust field scale to window size.
        if ( ! zoomed() )
        {
            double total_pitch_l = ( rcsc::ServerParam::i().pitchLength()
                                     + rcsc::ServerParam::i().pitchMargin() * 2.0
                                     + 1.0 );
            double total_pitch_w = ( rcsc::ServerParam::i().pitchWidth()
                                     + rcsc::ServerParam::i().pitchMargin() * 2.0
                                     + 1.0 );

            M_field_scale = static_cast< double >( canvas_width ) / total_pitch_l;
            // automatically adjust a field scale
            if ( total_pitch_w * fieldScale() > field_height )
            {
                M_field_scale = static_cast< double >( field_height ) / total_pitch_w;
            }
            // check the scale threshold
            if ( fieldScale() < MIN_FIELD_SCALE )
            {
                M_field_scale = MIN_FIELD_SCALE;
            }

            M_field_scale = rint( M_field_scale * 100.0 ) / 100.0;
        }
    }

    updateFieldCenter( canvas_width, canvas_height );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::updateFieldCenter( const int canvas_width,
                            const int canvas_height )
{
    int field_height = canvas_height - scoreBoardHeight();

    M_field_center.x = canvas_width*0.5 - scale( focusPoint().x );
    M_field_center.y
        = scoreBoardHeight()
        + field_height*0.5
        - scale( focusPoint().y );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::updateScoreBoardSize( const int /*canvas_width*/,
                               const int canvas_height )
{
    if ( paintStyle() == Options::PAINT_RCSSMONITOR )
    {
        M_score_board_height = 0;
    }
    else if ( showScoreBoard() )
    {
        M_score_board_height = rcsc::bound( Options::MIN_SCORE_BOARD_HEIGHT,
                                            canvas_height / 40,
                                            Options::MAX_SCORE_BOARD_HEIGHT );
        if ( M_score_board_height < M_score_board_font_height + 4 )
        {
            M_score_board_height = M_score_board_font_height + 4;
        }
        if ( M_score_board_height > M_score_board_font_height + 10 )
        {
            M_score_board_height = M_score_board_font_height + 10;
        }
    }
    else
    {
        M_score_board_height = 0;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setBallSize( const double & size )
{
    if ( size < 0.001 ) return;
    M_ball_size = size;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setPlayerSize( const double & size )
{
    if ( size < 0.0 ) return;
    M_player_size = size;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setBallTraceStart( const int cycle )
{
    if ( 0 <= cycle )
    {
        M_ball_trace_start = cycle;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setBallTraceEnd( const int cycle )
{
    if ( 0 <= cycle )
    {
        M_ball_trace_end = std::max( cycle, ballTraceStart() );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setPlayerTraceStart( const int cycle )
{
    if ( 0 <= cycle )
    {
        M_player_trace_start = cycle;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setPlayerTraceEnd( const int cycle )
{
    if ( 0 <= cycle )
    {
        M_player_trace_end = std::max( cycle, playerTraceStart() );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setAutoTracePeriod( const int period )
{
    M_auto_trace_period = rcsc::bound( 1, period, 100 );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setBallFutureCycle( const int cycle )
{
    M_ball_future_cycle = rcsc::bound( 0, cycle, 100 );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setPlayerFutureCycle( const int cycle )
{
    M_player_future_cycle = rcsc::bound( 0, cycle, 100 );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setPlayerMovableCycle( const int cycle )
{
    M_player_movable_cycle = rcsc::bound( 0, cycle, 30 );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::zoomIn()
{
    M_zoomed = true;
    M_field_scale *= ZOOM_RATIO;
    if ( M_field_scale > MAX_FIELD_SCALE )
    {
        M_field_scale = MAX_FIELD_SCALE;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::zoomOut()
{
    M_zoomed = true;
    M_field_scale /= ZOOM_RATIO;
    if ( M_field_scale < MIN_FIELD_SCALE )
    {
        M_field_scale = MIN_FIELD_SCALE;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::fitToScreen()
{
    if ( M_zoomed
         || focusType() != Options::FOCUS_POINT
         || focusPoint().x != 0.0
         || focusPoint().y != 0.0 )
    {
        M_focus_type = Options::FOCUS_POINT;
        M_focus_point.assign( 0.0, 0.0 );
        M_zoomed = false;

        int temp_width = M_canvas_width;
        M_canvas_width += 1;
        updateFieldSize( temp_width, M_canvas_height );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setFieldScale( const double & scale )
{
    if ( std::fabs( M_field_scale - scale ) > 0.01 )
    {
        M_field_scale = scale;
        if ( M_field_scale > MAX_FIELD_SCALE )
        {
            M_field_scale = MAX_FIELD_SCALE;
        }
        if ( M_field_scale < MIN_FIELD_SCALE )
        {
            M_field_scale = MIN_FIELD_SCALE;
        }
        M_zoomed = true;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setFocusPoint( const int screen_x,
                        const int screen_y )
{
    M_focus_type = Options::FOCUS_POINT;

    M_focus_point.x = fieldX( screen_x );
    M_focus_point.y = fieldY( screen_y );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::updateFocusPoint( const double x,
                           const double y )
{
    M_focus_point.assign( x, y );

    if ( M_reverse_side )
    {
        M_focus_point *= -1.0;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::unselectAgent()
{
    M_agent_select_type = Options::SELECT_UNSELECT;
    setSelectedNumber( rcsc::NEUTRAL, 0 );
    M_focus_type = Options::FOCUS_POINT;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Options::setAgentSelectType( const Options::AgentSelectType type )
{
    if ( type != Options::SELECT_FIX
         && type == M_agent_select_type )
    {
        unselectAgent();
    }
    else
    {
        M_agent_select_type = type;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
AgentID
Options::selectedAgent() const
{
    if ( M_selected_number > 0 )
    {
        return AgentID( rcsc::LEFT, M_selected_number );
    }
    else if ( M_selected_number < 0 )
    {
        return AgentID( rcsc::RIGHT, - M_selected_number );
    }

    return AgentID();
}
