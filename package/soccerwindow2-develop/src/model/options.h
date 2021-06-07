// -*-c++-*-

/*!
  \file options.h
  \brief global option holder class Header File.
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

#ifndef SOCCERWINDOW2_OPTIONS_H
#define SOCCERWINDOW2_OPTIONS_H

#include "point.h"
#include "agent_id.h"

#include <rcsc/types.h>
#include <rcsc/geom/vector_2d.h>

#include <boost/shared_ptr.hpp>

#include <string>
#include <utility>

/*!
  \class Options
  \brief global option holder.
*/
class Options {
public:

    enum GrassType {
        GRASS_LINES,
        GRASS_CHECKER,
        GRASS_NORMAL,
    };

    enum PaintStyle {
        PAINT_DEFAULT,
        PAINT_RCSSMONITOR,
    };

    enum AgentSelectType {
        SELECT_FIX = 0 ,
        SELECT_AUTO_LEFT = 1,
        SELECT_AUTO_RIGHT = -1,
        SELECT_AUTO_ALL = 2,
        SELECT_COACH_LEFT = 3,
        SELECT_COACH_RIGHT = -3,
        SELECT_UNSELECT = 4,
    };

    enum FocusType {
        FOCUS_BALL,
        FOCUS_PLAYER,
        FOCUS_POINT, // include center
    };

    enum MouseMeasureMode {
        MEASURE_NO_MODE,
        MEASURE_BALL_MOVE,
        MEASURE_PLAYER_MOVE,
    };


    static const double MIN_FIELD_SCALE;
    static const double MAX_FIELD_SCALE;

    static const double ZOOM_RATIO;

    static const int MIN_SCORE_BOARD_HEIGHT;
    static const int MAX_SCORE_BOARD_HEIGHT;

    static const int DEFAULT_TIMER_INTERVAL; //<! default logplayer timer step (ms).

    static const int CURSOR_TIMEOUT;

private:

    //
    // monitor client options
    //
    bool M_connect;
    std::string M_host;
    int M_port;
    int M_client_version;
    int M_wait_seconds;
    bool M_auto_quit_mode;
    bool M_kill_server;
    int M_server_pid;
    std::string M_server_path; //!< rcssserver command line path
    bool M_time_shift_replay;

    //
    // logplayer options
    //
    std::string M_game_log_file_path;
    std::string M_game_log_dir; //!< default dir to search game log
    bool M_auto_loop_mode;
    int M_timer_interval; //!< logplayer's timer interval. default 100[ms]

    //
    // window options
    //
    int M_pos_x;
    int M_pos_y;
    int M_width;
    int M_height;

    bool M_maximize;
    bool M_full_screen;

    std::string M_tool_bar_area;
    bool M_hide_menu_bar;
    bool M_hide_tool_bar;
    bool M_hide_status_bar;

    //
    // screen state
    //

    int M_canvas_width;
    int M_canvas_height;
    PointF M_field_center; //!< the screen point of field center
    double M_field_scale; //!< field scale rate
    int M_score_board_font_height;
    int M_score_board_height; //!< screen height of score board
    bool M_zoomed; //!< true if canvas scale is changed.

    FocusType M_focus_type;
    rcsc::Vector2D M_focus_point; //!< real coordinates

    int M_selected_number; //!< selected player's uniform number(if right side team, negative value is set)
    AgentSelectType M_agent_select_type; //!< player/coach selection type

    double M_team_graphic_scale; //!< scale for team graphic

    //
    // view options
    //

    bool M_anonymous_mode;
    GrassType M_field_grass_type;
    PaintStyle M_paint_style;
    bool M_keepaway_mode;

    bool M_anti_aliasing;
    bool M_gradient;
    bool M_cursor_hide;

    bool M_reverse_side;
    bool M_player_reverse_draw;

    bool M_show_score_board;
    bool M_show_team_graphic;
    bool M_show_flags; // no cmd line option
    bool M_show_draw_data; // no cmd line option
    bool M_show_grid_coord; // no cmd line option
    int M_grid_step; // no cmd line option

    bool M_show_ball; // no cmd line option
    bool M_show_players; // no cmd line option
    bool M_show_player_number;
    bool M_show_player_type;
    bool M_show_view_area;
    bool M_show_illegal_defense_state;
    bool M_show_pointto;
    bool M_show_attentionto;
    bool M_show_stamina;
    bool M_show_stamina_capacity;
    bool M_show_body_shadow; // no cmd line option
    bool M_show_catchable_area; // no cmd line option
    bool M_show_tackle_area; // no cmd line option
    bool M_show_kick_accel_area; // no cmd line option
    bool M_show_offside_line; // no cmd line option
    bool M_show_card;

    // object size

    bool M_enlarge_mode;
    double M_ball_size;
    double M_player_size;

    // computational geometry

    bool M_show_voronoi_diagram; // no cmd line option
    bool M_show_delaunay_triangulation; // no cmd line option
    rcsc::SideID M_voronoi_target; // no cmd line option

    // objects

    bool M_ball_auto_trace; // no cmd line option
    int M_ball_trace_start; // no cmd line option
    int M_ball_trace_end; // no cmd line option

    bool M_player_auto_trace; // no cmd line option
    int M_player_trace_start; // no cmd line option
    int M_player_trace_end; // no cmd line option

    int M_auto_trace_start; // no cmd line option
    int M_auto_trace_period; // no cmd line option

    bool M_line_trace; // no cmd line option

    int M_ball_future_cycle; // no cmd line option
    int M_player_future_cycle; // no cmd line option

    int M_player_movable_cycle; // no cmd line option

    //
    // debug server options
    //
    bool M_debug_server_mode;
    int M_debug_server_port;
    std::string M_debug_log_dir; //!< default dir to search debug log

    //
    // debug view options
    //
    bool M_show_debug_view;
    bool M_show_debug_view_ball;
    bool M_show_debug_view_self;
    bool M_show_debug_view_teammates;
    bool M_show_debug_view_opponents;
    bool M_show_debug_view_comment;
    bool M_show_debug_view_shape;
    bool M_show_debug_view_target;
    bool M_show_debug_view_message;
    bool M_show_debug_log_objects;


    //
    // offline team command
    //
    std::string M_offline_team_command_left;
    std::string M_offline_team_command_right;


    //
    // field evaluator
    //
    std::string M_evaluator_command_left;
    std::string M_evaluator_command_right;
    std::string M_evaluator_name;
    std::string M_evaluator_param_file_left;
    std::string M_evaluator_param_file_right;

    double M_evaluator_grid_size;

    double M_evaluation_view_min_rate;
    double M_evaluation_view_max_rate;

    //
    // image save options
    //
    bool M_auto_image_save;
    std::string M_image_save_dir;
    std::string M_image_name_prefix;
    std::string M_image_save_format;

    //
    // files
    //
    std::string M_intercept_decision_file;
    std::string M_intercept_evaluate_file;

    std::string M_draw_data_file;


    // no command line options

    // flag variable. if true, soccerwindow2 is a monitor client mode
    bool M_monitor_client_mode;


    //
    // mouse measure
    //
    MouseMeasureMode M_mouse_measure_mode;
    double M_mouse_measure_first_length;


    //! private access for singleton
    Options();

    //! noncopyable
    Options( const Options & );
    //! noncopyable
    Options & operator=( const Options & );

public:
    ~Options();

    static
    Options & instance();

    /*!
      \brief analyze command line options
    */
    bool parseCmdLine( int argc,
                       char ** argv );

    void setGameLogDir( const std::string & dir ) { M_game_log_dir = dir; }
    void setKillServer( bool on ) { M_kill_server = on; }
    void setServerPID( const int pid ) { M_server_pid = pid; }
    void setMonitorClientMode( bool on ) { M_monitor_client_mode = on; }
    void setDebugLogDir( const std::string & dir ) { M_debug_log_dir = dir; }

    //
    // monitor client options
    //

    bool connect() const { return M_connect; }
    const std::string & host() const { return M_host; }
    int port() const { return M_port; }
    int clientVersion() const { return M_client_version; }
    int waitSeconds() const { return M_wait_seconds; }
    bool autoQuitMode() const { return M_auto_quit_mode; }
    bool killServer() const { return M_kill_server; }
    int serverPID() const { return M_server_pid; }
    const std::string & serverPath() const { return M_server_path; }

    bool monitorClientMode() const { return M_monitor_client_mode; }

    //
    // debug server options
    //

    bool debugServerMode() const { return M_debug_server_mode; }
    int debugServerPort() const { return M_debug_server_port; }
    const std::string & debugLogDir() const { return M_debug_log_dir; }

    //
    // logplayer options
    //

    const std::string & gameLogFilePath() const { return M_game_log_file_path; }
    const std::string & gameLogDir() const { return M_game_log_dir; }
    bool timeShiftReplay() const { return M_time_shift_replay; }
    bool autoLoopMode() const { return M_auto_loop_mode; }
    int timerInterval() const { return M_timer_interval; }

    //
    // window options
    //

    void setFramePos( const int pos_x,
                      const int pos_y )
      {
          M_pos_x = pos_x;
          M_pos_y = pos_y;
      }

    void setFrameSize( const int width,
                       const int height )
      {
          M_width = width;
          M_height = height;
      }

    int framePosX() const { return M_pos_x; }
    int framePosY() const { return M_pos_y; }
    int frameWidth() const { return M_width; }
    int frameHeight() const { return M_height; }

    bool maximize() const { return M_maximize; }
    bool fullScreen() const { return M_full_screen; }

    const std::string & toolBarArea() const { return M_tool_bar_area; }

    bool hideMenuBar() const { return M_hide_menu_bar;}
    bool hideToolBar() const { return M_hide_tool_bar; }
    bool hideStatusBar() const { return M_hide_status_bar; }

    //
    // screen state
    //

    int canvasWidth() const { return M_canvas_width; }
    int canvasHeight() const { return M_canvas_height; }

    void setScoreBoardFontHeight( const int pixel ) { M_score_board_font_height = pixel; }

    int scoreBoardHeight() const { return M_score_board_height; }

    void zoomIn();
    void zoomOut();
    void fitToScreen();
    bool zoomed() const { return M_zoomed; }

    void setFieldScale( const double & scale );
    double fieldScale() const { return M_field_scale; }

    const PointF & fieldCenter() const { return M_field_center; }

    void setFocusPoint( const int screen_x,
                        const int screen_y );
    void updateFocusPoint( const double x,
                           const double y );
    const rcsc::Vector2D & focusPoint() const { return M_focus_point; }

    void unselectAgent();
    void setAgentSelectType( const AgentSelectType type );
    AgentSelectType agentSelectType() const { return M_agent_select_type; }
    bool isPlayerAutoSelect() const
      {
          return ( M_agent_select_type == SELECT_AUTO_LEFT
                   || M_agent_select_type == SELECT_AUTO_RIGHT );
      }
    void setSelectedNumber( rcsc::SideID side,
                            int unum )
      {
          M_selected_number = ( side == rcsc::LEFT ? unum : -unum );
      }
    int selectedNumber() const { return M_selected_number; }
    AgentID selectedAgent() const;
    bool isSelectedAgent( rcsc::SideID side,
                          int unum ) const
      {
          return ( M_selected_number == ( side == rcsc::LEFT ? unum : -unum ) );
      }

    double teamGraphicScale() const { return M_team_graphic_scale; }
    void setTeamGraphicScale( const double value )
      {
          M_team_graphic_scale = value;
          if ( value < 0.1 ) M_team_graphic_scale = 0.1;
          if ( value > 16.0 ) M_team_graphic_scale = 16.0;
      }

    //
    // view options
    //

    void toggleAnonymousMode() { M_anonymous_mode = ! M_anonymous_mode; }
    bool anonymousMode() const { return M_anonymous_mode; }

    void setFieldGrassType( GrassType value ) { M_field_grass_type = value; }
    GrassType fieldGrassType() const { return M_field_grass_type; }

    void setPaintStyle( PaintStyle value ) { M_paint_style = value; }
    PaintStyle paintStyle() const { return M_paint_style; }

    void toggleKeepawayMode() { M_keepaway_mode = ! M_keepaway_mode; }
    bool keepawayMode() const { return M_keepaway_mode; }

    void toggleAntiAliasing() { M_anti_aliasing = ! M_anti_aliasing; }
    bool antiAliasing() const { return M_anti_aliasing; }

    void toggleGradient() { M_gradient = ! M_gradient; }
    bool gradient() const { return M_gradient; }

    void toggleCursorHide() { M_cursor_hide = ! M_cursor_hide; }
    bool cursorHide() const { return M_cursor_hide; }

    void toggleReverseSide() { M_reverse_side = ! M_reverse_side; }
    bool reverseSide() const { return M_reverse_side; }
    double reverseValue() const { return ( M_reverse_side ? -1.0 : 1.0 ); }

    void togglePlayerReverseDraw() { M_player_reverse_draw = ! M_player_reverse_draw; }
    bool playerReverseDraw() const { return M_player_reverse_draw; }


    void toggleShowScoreBoard() { M_show_score_board = ! M_show_score_board; }
    bool showScoreBoard() const { return M_show_score_board; }

    void toggleShowTeamGraphic() { M_show_team_graphic = ! M_show_team_graphic; }
    bool showTeamGraphic() const { return M_show_team_graphic; }

    void toggleShowFlags() { M_show_flags = ! M_show_flags; }
    bool showFlags() const { return M_show_flags; }

    void toggleShowGridCoord() { M_show_grid_coord = ! M_show_grid_coord; }
    bool showGridCoord() const { return M_show_grid_coord; }

    void setGridStep( const int step ) { M_grid_step = step; }
    int gridStep() const { return M_grid_step; }

    void toggleShowBall() { M_show_ball = ! M_show_ball; }
    bool showBall() const { return M_show_ball; }

    void toggleShowPlayers() { M_show_players = ! M_show_players; }
    bool showPlayers() const { return M_show_players; }

    void toggleShowPlayerNumber() { M_show_player_number = ! M_show_player_number; }
    bool showPlayerNumber() const { return M_show_player_number; }

    void toggleShowPlayerType() { M_show_player_type = ! M_show_player_type; }
    bool showPlayerType() const { return M_show_player_type; }

    void toggleShowViewArea() { M_show_view_area = ! M_show_view_area; }
    bool showViewArea() const { return M_show_view_area; }

    void toggleShowIllegalDefenseState() { M_show_illegal_defense_state = ! M_show_illegal_defense_state; }
    bool showIllegalDefenseState() const { return M_show_illegal_defense_state; }

    void toggleShowPointto() { M_show_pointto = ! M_show_pointto; }
    bool showPointto() const { return M_show_pointto; }

    void toggleShowAttentionto() { M_show_attentionto = ! M_show_attentionto; }
    bool showAttentionto() const { return M_show_attentionto; }

    void toggleShowStamina() { M_show_stamina = ! M_show_stamina; }
    bool showStamina() const { return M_show_stamina; }

    void toggleShowStaminaCapacity() { M_show_stamina_capacity = ! M_show_stamina_capacity; }
    bool showStaminaCapacity() const { return M_show_stamina_capacity; }

    void toggleShowCard() { M_show_card = ! M_show_card; }
    bool showCard() const { return M_show_card; }

    void toggleShowBodyShadow() { M_show_body_shadow = ! M_show_body_shadow; }
    bool showBodyShadow() const { return M_show_body_shadow; }

    void toggleShowCatchableArea() { M_show_catchable_area = ! M_show_catchable_area; }
    bool showCatchableArea() const { return M_show_catchable_area; }

    void toggleShowTackleArea() { M_show_tackle_area = ! M_show_tackle_area; }
    bool showTackleArea() const { return M_show_tackle_area; }

    void toggleShowKickAccelArea() { M_show_kick_accel_area = ! M_show_kick_accel_area; }
    bool showKickAccelArea() const { return M_show_kick_accel_area; }

    void toggleShowOffsideLine() { M_show_offside_line = ! M_show_offside_line; }
    bool showOffsideLine() const { return M_show_offside_line; }

    void toggleShowDrawData() { M_show_draw_data = ! M_show_draw_data; }
    bool showDrawData() const { return M_show_draw_data; }

    //

    void toggleEnlargeMode() { M_enlarge_mode = ! M_enlarge_mode; }
    bool enlargeMode() const { return M_enlarge_mode; }

    void setBallSize( const double & size );
    const double & ballSize() const { return M_ball_size; }

    void setPlayerSize( const double & size );
    const double & playerSize() const { return M_player_size; }

    void setFocusType( const FocusType type ) { M_focus_type = type; }
    FocusType focusType() const { return M_focus_type; }

    void toggleShowVoronoiDiagram() { M_show_voronoi_diagram = ! M_show_voronoi_diagram; }
    bool showVoronoiDiagram() const { return M_show_voronoi_diagram; }

    void toggleShowDelaunayTriangulation() { M_show_delaunay_triangulation = ! M_show_delaunay_triangulation; }
    bool showDelaunayTriangulation() const { return M_show_delaunay_triangulation; }

    void setVoronoiTarget( const rcsc::SideID side ) { M_voronoi_target = side; }
    rcsc::SideID voronoiTarget() const { return M_voronoi_target; }

    void toggleBallAutoTrace() { M_ball_auto_trace = ! M_ball_auto_trace; }
    bool ballAutoTrace() const { return M_ball_auto_trace; }

    void setBallTraceStart( const int cycle );
    int ballTraceStart() const { return M_ball_trace_start; }

    void setBallTraceEnd( const int cycle );
    int ballTraceEnd() const { return M_ball_trace_end; }

    bool showBallTrace() const
      {
          return ( ballAutoTrace()
                   || ( M_ball_trace_start < M_ball_trace_end ) );
      }

    void togglePlayerAutoTrace() { M_player_auto_trace = ! M_player_auto_trace; }
    bool playerAutoTrace() const { return M_player_auto_trace; }

    void setPlayerTraceStart( const int cycle );
    int playerTraceStart() const { return M_player_trace_start; }

    void setPlayerTraceEnd( const int cycle );
    int playerTraceEnd() const { return M_player_trace_end; }

    bool showPlayerTrace() const
      {
          return ( playerAutoTrace()
                   || ( M_player_trace_start < M_player_trace_end ) );
      }

    void setAutoTraceStart( const int start ) { M_auto_trace_start = start; }
    int autoTraceStart() const { return M_auto_trace_start; }

    void setAutoTracePeriod( const int period );
    int autoTracePeriod() const { return M_auto_trace_period; }

    void toggleLineTrace() { M_line_trace = ! M_line_trace; }
    bool lineTrace() const { return M_line_trace; }

    void setBallFutureCycle( const int cycle );
    int ballFutureCycle() const { return M_ball_future_cycle; }

    void setPlayerFutureCycle( const int cycle );
    int playerFutureCycle() const { return M_player_future_cycle; }

    void setPlayerMovableCycle( const int cycle );
    int playerMovableCycle() const { return M_player_movable_cycle; }

    //
    // debug view options
    //

    void toggleShowDebugView() { M_show_debug_view = ! M_show_debug_view; }
    bool showDebugView() const { return M_show_debug_view; }

    void toggleShowDebugViewBall() { M_show_debug_view_ball = ! M_show_debug_view_ball; }
    bool showDebugViewBall() const { return M_show_debug_view_ball; }

    void toggleShowDebugViewSelf() { M_show_debug_view_self = ! M_show_debug_view_self; }
    bool showDebugViewSelf() const { return M_show_debug_view_self; }

    void toggleShowDebugViewTeammates() { M_show_debug_view_teammates = ! M_show_debug_view_teammates; }
    bool showDebugViewTeammates() const { return M_show_debug_view_teammates; }

    void toggleShowDebugViewOpponents() { M_show_debug_view_opponents = ! M_show_debug_view_opponents; }
    bool showDebugViewOpponents() const { return M_show_debug_view_opponents; }

    void toggleShowDebugViewComment() { M_show_debug_view_comment = ! M_show_debug_view_comment; }
    bool showDebugViewComment() const { return M_show_debug_view_comment; }

    void toggleShowDebugViewShape() { M_show_debug_view_shape = ! M_show_debug_view_shape; }
    bool showDebugViewShape() const { return M_show_debug_view_shape; }

    void toggleShowDebugViewTarget() { M_show_debug_view_target = ! M_show_debug_view_target; }
    bool showDebugViewTarget() const { return M_show_debug_view_target; }

    void toggleShowDebugViewMessage() { M_show_debug_view_message = ! M_show_debug_view_message; }
    bool showDebugViewMessage() const { return M_show_debug_view_message; }

    void toggleShowDebugLogObjects() { M_show_debug_log_objects = ! M_show_debug_log_objects; }
    bool showDebugLogObjects() const { return M_show_debug_log_objects; }

    //
    // offline team command
    //
    const std::string & offlineTeamCommandLeft() const { return M_offline_team_command_left; }
    const std::string & offlineTeamCommandRight() const { return M_offline_team_command_right; }
    void setOfflineTeamCommandLeft( const std::string & str ) { M_offline_team_command_left = str; }
    void setOfflineTeamCommandRight( const std::string & str ) { M_offline_team_command_right = str; }

    //
    // evaluator
    //
    const std::string & evaluatorCommandLeft() const { return M_evaluator_command_left; }
    const std::string & evaluatorCommandRight() const { return M_evaluator_command_right; }
    void setEvaluatorCommandLeft( const std::string & str ) { M_evaluator_command_left = str; }
    void setEvaluatorCommandRight( const std::string & str ) { M_evaluator_command_right = str; }

    const std::string & evaluatorParamFileLeft() const { return M_evaluator_param_file_left; }
    const std::string & evaluatorParamFileRight() const { return M_evaluator_param_file_right; }
    void setEvaluatorParamFileLeft( const std::string & str ) { M_evaluator_param_file_left = str; }
    void setEvaluatorParamFileRight( const std::string & str ) { M_evaluator_param_file_right = str; }

    const std::string & evaluatorName() const { return M_evaluator_name; }
    void setEvaluatorName( const std::string & name ) { M_evaluator_name = name; }

    double evaluatorGridSize() const { return M_evaluator_grid_size; }
    void setEvaluatorGridSize( const double val ) { M_evaluator_grid_size = val; }

    double evaluationViewMinRate() const { return M_evaluation_view_min_rate; }
    double evaluationViewMaxRate() const { return M_evaluation_view_max_rate; }
    void setEvaluationViewMinRate( double rate )
      {
          M_evaluation_view_min_rate = rate;
          M_evaluation_view_max_rate = std::max( M_evaluation_view_max_rate, rate );
      }
    void setEvaluationViewMaxRate( double rate )
      {
          M_evaluation_view_max_rate = rate;
          M_evaluation_view_min_rate = std::min( M_evaluation_view_min_rate, rate );
      }

    //
    // image save options
    //

    const std::string & imageSaveDir() const { return M_image_save_dir; }
    const std::string & imageNamePrefix() const { return M_image_name_prefix; }
    const std::string & imageSaveFormat() const { return M_image_save_format; }
    bool autoImageSave() const { return M_auto_image_save; }

    //
    // files
    //
    const std::string & interceptDecisionFile() const { return M_intercept_decision_file; }
    const std::string & interceptEvaluateFile() const { return M_intercept_evaluate_file; }
    const std::string & drawDataFile() const { return M_draw_data_file; }

    //
    // mouse measure
    //

    MouseMeasureMode mouseMeasureMode() const { return M_mouse_measure_mode; }
    void setMouseMeasureMode( const MouseMeasureMode mode ) { M_mouse_measure_mode = mode; }

    const double & mouseMeasureFirstLength() const { return M_mouse_measure_first_length; }
    void setMouseMeasureFirstLength( const double & len ) { M_mouse_measure_first_length = len; }

    /*!
      \brief update parameters with canvas size
      \param canvas_width pixel of canvas width
      \param canvas_height pixel of canvas height
    */
    void updateFieldSize( const int canvas_width,
                          const int canvas_height );

private:

    void updateFieldCenter( const int canvas_width,
                            const int canvas_height );

    /*
      \brief update scoreboard size
      \param canvas_width canvas window width
      \param canvas_height canvas window height
    */
    void updateScoreBoardSize( const int canvas_width,
                               const int canvas_height );

public:


    /*!
      \brief scale input length to screen length.
      \param len real length
      \return screen pixel length
    */
    double scale( const double len ) const
      {
          return len * fieldScale();
      }

    /*!
      \brief convert 'x' to the screen coordinate X.
      \param abs_x real point
      \return screen point
    */
    double absScreenX( const double abs_x ) const
      {
          return fieldCenter().x + scale( abs_x );
      }

    /*!
      \brief convert 'x' to the screen coordinate X with reverse mode.
      \param x real point (may be reversed)
      \return screen point
    */
    double screenX( const double x ) const
      {
          return ( M_reverse_side
                   ? fieldCenter().x + scale( -x )
                   : fieldCenter().x + scale( x ) );
      }

    /*!
      \brief convert 'y' to the screen coordinate X.
      \param abs_y real point
      \return screen point
    */
    double absScreenY( const double abs_y ) const
      {
          return fieldCenter().y + scale( abs_y );
      }

    /*!
      \brief convert 'y' to the screen coordinate X.
      \param y real point (may be reversed)
      \return screen point
    */
    double screenY( const double y ) const
      {
          return ( M_reverse_side
                   ? fieldCenter().y + scale( -y )
                   : fieldCenter().y + scale( y ) );
      }

    /*!
      \brief convert 'x' of screen to field coordinate x
      \param x screen point value
      \return field real point value
     */
    double fieldX( const double x ) const
      {
          return ( x - fieldCenter().x ) / fieldScale();
      }

    /*!
      \brief convert 'y' of screen to field coordinate y
      \param y screen point value
      \return field real point value
     */
    double fieldY( const double y ) const
      {
          return ( y - fieldCenter().y ) / fieldScale();
      }

    /*!
      \brief convert screen point to the field real coordinate
      \param point screen point value
      \return field real point value
     */
    rcsc::Vector2D fieldPos( const Point & point ) const
      {
          return rcsc::Vector2D( fieldX( point.x ),
                                 fieldY( point.y ) );
      }

};

#endif
