// -*-c++-*-

/*!
  \file draw_config.h
  \brief draw object holder class Header File.
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

#ifndef SOCCERWINDOW2_QT_DRAW_CONFIG_H
#define SOCCERWINDOW2_QT_DRAW_CONFIG_H

#include <QBrush>
#include <QColor>
#include <QFont>
#include <QPen>
#include <QString>

//! draw object configuration manager class
class DrawConfig {
public:
    //--------------------------------------------------
    // default color settings
    static const QColor SCORE_BOARD_COLOR;
    static const QColor SCORE_BOARD_FONT_COLOR;
    static const QColor FIELD_COLOR;
    static const QColor FIELD_DARK_COLOR;
    static const QColor LINE_COLOR;
    static const QColor BALL_PEN_COLOR;
    static const QColor BALL_COLOR;
    static const QColor BALL_FONT_COLOR;

    static const QColor PLAYER_PEN_COLOR;
    static const QColor REAL_BODY_PEN_COLOR;
    static const QColor LEFT_TEAM_COLOR;
    static const QColor LEFT_GOALIE_COLOR;
    static const QColor RIGHT_TEAM_COLOR;
    static const QColor RIGHT_GOALIE_COLOR;
    static const QColor PLAYER_SHADOW_COLOR;
    static const QColor PLAYER_NUMBER_FONT_COLOR;
    static const QColor PLAYER_STAMINA_FONT_COLOR;
    static const QColor PLAYER_DETAIL_FONT_COLOR;
    static const QColor SELECTED_PLAYER_FONT_COLOR;
    static const QColor EFFORT_DECAYED_PEN_COLOR;
    static const QColor RECOVERY_DECAYED_PEN_COLOR;
    static const QColor VIEW_CONE_PEN_COLOR;
    static const QColor VIEW_AREA_PEN_COLOR;
    static const QColor KICK_PEN_COLOR;
    static const QColor KICK_FAULT_COLOR;
    static const QColor CATCH_COLOR;
    static const QColor CATCH_FAULT_COLOR;
    static const QColor BALL_COLLISION_COLOR;
    static const QColor PLAYER_COLLISION_COLOR;
    static const QColor TACKLE_COLOR;
    static const QColor TACKLE_FAULT_COLOR;
    static const QColor FOUL_CHARGED_COLOR;
    static const QColor POINTTO_COLOR;
    static const QColor ILLEGAL_DEFENSE_COLOR;

    static const QColor MEASURE_COLOR;
    static const QColor MEASURE_FONT_COLOR;
    static const QColor DEBUG_BALL_COLOR;
    static const QColor DEBUG_SELF_COLOR;
    static const QColor DEBUG_TEAMMATE_COLOR;
    static const QColor DEBUG_OPPONENT_COLOR;
    static const QColor DEBUG_UNKNOWN_TEAMMATE_COLOR;
    static const QColor DEBUG_UNKNOWN_OPPONENT_COLOR;
    static const QColor DEBUG_UNKNOWN_PLAYER_COLOR;
    static const QColor DEBUG_COMMENT_FONT_COLOR;
    static const QColor DEBUG_MESSAGE_FONT_COLOR;
    static const QColor DEBUG_POINTTO_COLOR;
    static const QColor DEBUG_TARGET_COLOR;
    static const QColor DEBUG_LOG_MESSAGE_FONT_COLOR;
    static const QColor DEBUG_SHAPE_COLOR;
    static const QColor DEBUG_ACTION_SEQUENCE_COLOR;
    static const QColor DEBUG_ACTION_PASS_COLOR;

private:

    //--------------------------------------------------
    // general
    QPen M_transparent_pen;
    QBrush M_transparent_brush;

    //--------------------------------------------------
    // score board

    QBrush M_score_board_brush;
    QFont M_score_board_font;
    QPen M_score_board_font_pen;

    //--------------------------------------------------
    // field

    //! field grass brush
    QBrush M_field_brush;
    //! field dark grass brush
    QBrush M_field_dark_brush;
    //! line pen
    QPen M_line_pen;

    //--------------------------------------------------
    // ball

    QPen M_ball_pen; //!< ball edge
    QBrush M_ball_brush; //!< ball body paint info
    QFont M_ball_font; //!< ball info font info
    QPen M_ball_font_pen; //!< ball font foreground color

    //--------------------------------------------------
    // players

    //! player edge pen
    QPen M_player_pen;
    //! real body edge pen
    QPen M_real_body_pen;

    //! shadow brush
    QBrush M_shadow_brush;
    QBrush M_shadow_brushes[256];

    //! left team color pen
    QPen M_left_team_pen;
    //! left team brush (= left player body brush)
    QBrush M_left_team_brush;
    //! left goalie color pen
    QPen M_left_goalie_pen;
    //! left goalie stretch pen
    QPen M_left_goalie_stretch_pen;
    //! left goalie body brush
    QBrush M_left_goalie_brush;

    //! right team pen
    QPen M_right_team_pen;
    //! right team brush(= right player body brush)
    QBrush M_right_team_brush;
    //! right goalie color pen
    QPen M_right_goalie_pen;
    //! right goalie stretch pen
    QPen M_right_goalie_stretch_pen;
    //! left goalie body brush
    QBrush M_right_goalie_brush;

    //! player info font
    QFont M_player_font;
    //! foreground color of the player number font
    QPen M_player_number_font_pen;
    //! foreground color of the player stamina font
    QPen M_player_stamina_font_pen;
    //! foreground color of the player detail
    QPen M_player_detail_font_pen;
    //! foreground color of the selected player font
    QPen M_selected_player_font_pen;

    //! the pen of the effort decayed player indication circle
    QPen M_effort_decayed_pen;
    //! the pen of the recovery decayed player indication circle
    QPen M_recovery_decayed_pen;

    //! the pen of players' view cone
    QPen M_view_cone_pen;
    //! the pen of players' view area
    QPen M_view_area_pen;


    //--------------------------------------------------
    // player status

    QPen M_kick_pen; //!< edge color when kicking
    QPen M_kick_fault_pen; //!< edge color when kick falut
    QBrush M_kick_fault_brush; //!< body paint brush when kick falut
    QBrush M_catch_brush; //!< body paint brush when goalie catch
    QBrush M_catch_fault_brush; //!< body paint brush when goalie catch fault
    QBrush M_ball_collision_brush; //!< body paint brush when collision with ball
    QBrush M_player_collision_brush; //!< body paint brush when collision with player
    QPen M_tackle_pen; //!< edge pen when tackling
    QBrush M_tackle_brush; //!< body paint brush when tackling
    QBrush M_tackle_fault_brush; //!< body paint brush when tackle fault
    QBrush M_foul_charged_brush; //!< body paint brush when foul charged
    QPen M_pointto_pen; //!< pointto painting
    QPen M_illegal_defense_pen; //!< illegal defense state

    //! kick accel info pen
    QPen M_kick_accel_pen;
    //! tackle area rectangle line pen
    QPen M_tackle_area_pen;

    //--------------------------------------------------
    // misc

    // mouse measure
    QPen M_measure_pen;
    QFont M_measure_font;  //!< mouse measure font
    QPen M_measure_font_pen; //!< color of mouse measure font

    //--------------------------------------------------
    // debug view

    // ball
    QBrush M_debug_ball_brush;

    // ball trace
    QPen M_debug_ball_trace_pen;
    QBrush M_debug_ball_trace_brush;

    // player
    QPen M_debug_player_pen; //!< debug info player's body edge pen
    QBrush M_debug_self_brush;
    QBrush M_debug_teammate_brush;
    QBrush M_debug_opponent_brush;
    QBrush M_debug_unknown_teammate_brush;
    QBrush M_debug_unknown_opponent_brush;
    QBrush M_debug_unknown_player_brush;

    // arm
    QPen M_debug_pointto_pen;

    // target
    QPen M_debug_target_pen;

    // message
    QFont M_debug_comment_font;
    QPen M_debug_comment_font_pen;
    QFont M_debug_message_font;
    QPen M_debug_message_font_pen;

    //--------------------------------------------------
    // debug log view

    QFont M_debug_log_message_font;
    QPen M_debug_log_message_font_pen;

    // line, triangle, rectanble, circle
    QPen M_debug_shape_pen;

    // action sequence data
    QPen M_debug_action_sequence_pen;
    QPen M_debug_action_pass_pen;

    //! constructor
    DrawConfig();
    DrawConfig( const DrawConfig & );
    const DrawConfig & operator=( const DrawConfig & );
public:
    //! destructor
    ~DrawConfig();

    //! singleton interface
    static
    DrawConfig & instance()
      {
          static DrawConfig s_instance;
          return s_instance;
      }

    void setDefaultColors();
    void setDefaultFonts();

    //--------------------------------------------------
    //! read configuration from file.
    void readSettings();
    //! save configuration to file.
    void saveSettings();

private:

    //! convert config string to color object
    QColor toColor( const QString & conf_str );
    //! convert clor object to config string
    QString toString( const QColor & color );

public:

    //--------------------------------------------------
    // accessor

    const QPen & transparentPen() const { return M_transparent_pen; }
    const QBrush & transparentBrush() const { return M_transparent_brush; }

    const QBrush & scoreBoardBrush() const { return M_score_board_brush; }
    const QFont & scoreBoardFont() const { return M_score_board_font; }
    const QPen & scoreBoardFontPen() const { return M_score_board_font_pen; }

    const QBrush & fieldBrush() const { return M_field_brush; }
    const QBrush & fieldDarkBrush() const { return M_field_dark_brush; }
    const QPen & linePen() const { return M_line_pen; }

    const QPen & ballPen() const { return M_ball_pen; }
    const QBrush & ballBrush() const { return M_ball_brush; }
    const QFont & ballFont() const { return M_ball_font; }
    const QPen & ballFontPen() const { return M_ball_font_pen; }

    const QPen & playerPen() const { return M_player_pen; }
    const QPen & realBodyPen() const { return M_real_body_pen; }
    const QBrush & shadowBrush() const{ return M_shadow_brush; }
    const QBrush & shadowBrush( const int level ) const
      {
          if ( level < 0 || 255 < level ) return M_shadow_brush;
          return M_shadow_brushes[level];
      }

    const QPen & leftTeamPen() const { return M_left_team_pen; }
    const QBrush & leftTeamBrush() const { return M_left_team_brush; }
    const QPen & leftGoaliePen() const { return M_left_goalie_pen; }
    const QPen & leftGoalieStretchPen() const { return M_left_goalie_stretch_pen; }
    const QBrush & leftGoalieBrush() const { return M_left_goalie_brush; }

    const QPen & rightTeamPen() const { return M_right_team_pen; }
    const QBrush & rightTeamBrush() const { return M_right_team_brush; }
    const QPen & rightGoaliePen() const { return M_right_goalie_pen; }
    const QPen & rightGoalieStretchPen() const { return M_right_goalie_stretch_pen; }
    const QBrush & rightGoalieBrush() const { return M_right_goalie_brush; }

    const QFont & playerFont() const { return M_player_font; }
    const QPen & playerNumberFontPen() const { return M_player_number_font_pen; }
    const QPen & playerStaminaFontPen() const { return M_player_stamina_font_pen; }
    const QPen & playerDetailFontPen() const { return M_player_detail_font_pen; }
    const QPen & selectedPlayerFontPen() const { return M_selected_player_font_pen; }
    const QPen & effortDecayedPen() const { return M_effort_decayed_pen; }
    const QPen & recoveryDecayedPen() const { return M_recovery_decayed_pen; }
    const QPen & viewConePen() const { return M_view_cone_pen; }
    const QPen & viewAreaPen() const { return M_view_area_pen; }

    const QPen & kickPen() const { return M_kick_pen; }
    const QPen & kickFaultPen() const { return M_kick_fault_pen; }
    const QBrush & kickFaultBrush() const { return M_kick_fault_brush; }
    const QBrush & catchBrush() const { return M_catch_brush; }
    const QBrush & catchFaultBrush() const { return M_catch_fault_brush; }
    const QBrush & ballCollisionBrush() const { return M_ball_collision_brush; }
    const QBrush & playerCollisionBrush() const { return M_player_collision_brush; }
    const QPen & tacklePen() const { return M_tackle_pen; }
    const QBrush & tackleBrush() const { return M_tackle_brush; }
    const QBrush & tackleFaultBrush() const { return M_tackle_fault_brush; }
    const QBrush & foulChargedBrush() const { return M_foul_charged_brush; }
    const QPen & pointtoPen() const { return M_pointto_pen; }
    const QPen & illegalDefensePen() const { return M_illegal_defense_pen; }
    const QPen & kickAccelPen() const { return M_kick_accel_pen; }
    const QPen & tackleAreaPen() const { return M_tackle_area_pen; }

    const QPen & measurePen() const { return M_measure_pen; }
    const QFont & measureFont() const { return M_measure_font; }
    const QPen & measureFontPen() const { return M_measure_font_pen; }

    const QBrush & debugBallBrush() const { return M_debug_ball_brush; }
    const QPen & debugBallTracePen() const { return M_debug_ball_trace_pen; }
    const QBrush & debugBallTraceBrush() const { return M_debug_ball_trace_brush; }
    const QPen & debugPlayerPen() const { return M_debug_player_pen; }
    const QBrush & debugSelfBrush() const { return M_debug_self_brush; }
    const QBrush & debugTeammateBrush() const { return M_debug_teammate_brush; }
    const QBrush & debugOpponentBrush() const { return M_debug_opponent_brush; }
    const QBrush & debugUnknownTeammateBrush() const { return M_debug_unknown_teammate_brush; }
    const QBrush & debugUnknownOpponentBrush() const { return M_debug_unknown_opponent_brush; }
    const QBrush & debugUnknownPlayerBrush() const { return M_debug_unknown_player_brush; }
    const QPen & debugPointtoPen() const { return M_debug_pointto_pen; }
    const QPen & debugTargetPen() const { return M_debug_target_pen; }
    const QFont & debugCommentFont() const { return M_debug_comment_font; }
    const QPen & debugCommentFontPen() const { return M_debug_comment_font_pen; }
    const QFont & debugMessageFont() const { return M_debug_message_font; }
    const QPen & debugMessageFontPen() const { return M_debug_message_font_pen; }
    const QFont & debugLogMessageFont() const { return M_debug_log_message_font; }
    const QPen & debugLogMessageFontPen() const { return M_debug_log_message_font_pen; }
    const QPen & debugShapePen() const { return M_debug_shape_pen; }
    const QPen & debugActionSequencePen() const { return M_debug_action_sequence_pen; }
    const QPen & debugActionPassPen() const { return M_debug_action_pass_pen; }

    //
    // color settor
    //

    void setScoreBoardColor( const QColor & col ) { M_score_board_brush.setColor( col ); }
    void setScoreBoardFontColor( const QColor & col ) { M_score_board_font_pen.setColor( col ); }

    void setFieldColor( const QColor & col ) { M_field_brush.setColor( col ); }
    void setFieldDarkColor( const QColor & col ) { M_field_dark_brush.setColor( col ); }
    void setLineColor( const QColor & col ) { M_line_pen.setColor( col ); }
    void setBallColor( const QColor & col ) { M_ball_brush.setColor( col ); }

    void setMeasureColor( const QColor & col ) { M_measure_pen.setColor( col ); }
    void setMeasureFontColor( const QColor & col ) { M_measure_font_pen.setColor( col ); }
    void setLeftTeamColor( const QColor & col )
      {
          M_left_team_pen.setColor( col );
          M_left_team_brush.setColor( col );
      }
    void setLeftGoalieColor( const QColor & col )
      {
          M_left_goalie_pen.setColor( col );
          M_left_goalie_stretch_pen.setColor( col );
          M_left_goalie_brush.setColor( col );
      }
    void setRightTeamColor( const QColor & col )
      {
          M_right_team_pen.setColor( col );
          M_right_team_brush.setColor( col );
      }
    void setRightGoalieColor( const QColor & col )
      {
          M_right_goalie_pen.setColor( col );
          M_right_goalie_stretch_pen.setColor( col );
          M_right_goalie_brush.setColor( col );
      }
    void setPlayerNumberFontColor( const QColor & col ) { M_player_number_font_pen.setColor( col ); }
    void setPlayerStaminaFontColor( const QColor & col ) { M_player_stamina_font_pen.setColor( col ); }
    void setPlayerDetailFontColor( const QColor & col ) { M_player_detail_font_pen.setColor( col ); }
    void setSelectedPlayerFontColor( const QColor & col ) { M_selected_player_font_pen.setColor( col ); }
    void setEffortDecayedColor( const QColor & col ) { M_effort_decayed_pen.setColor( col ); }
    void setRecoveryDecayedColor( const QColor & col ) { M_recovery_decayed_pen.setColor( col ); }
    void setViewConeColor( const QColor & col ) { M_view_cone_pen.setColor( col ); }
    void setViewAreaColor( const QColor & col ) { M_view_area_pen.setColor( col ); }

    void setKickColor( const QColor & col ) { M_kick_pen.setColor( col ); }
    void setKickFaultColor( const QColor & col )
      {
          M_kick_fault_pen.setColor( col );
          M_kick_fault_brush.setColor( col );
      }
    void setCatchColor( const QColor & col ) { M_catch_brush.setColor( col ); }
    void setCatchFaultColor( const QColor & col ) { M_catch_fault_brush.setColor( col ); }
    void setBallCollisionColor( const QColor & col ) { M_ball_collision_brush.setColor( col ); }
    void setPlayerCollisionColor( const QColor & col ) { M_player_collision_brush.setColor( col ); }
    void setTackleColor( const QColor & col )
      {
          M_tackle_pen.setColor( col );
          M_tackle_brush.setColor( col );
      }
    void setTackleFaultColor( const QColor & col ) { M_tackle_fault_brush.setColor( col ); }
    void setFoulChargedColor( const QColor & col ) { M_foul_charged_brush.setColor( col ); }
    void setPointtoColor( const QColor & col ) { M_pointto_pen.setColor( col ); }

    void setKickAccelColor( const QColor & col ) { M_kick_accel_pen.setColor( col ); }
    void setTackleAreaColor( const QColor & col ) { M_tackle_area_pen.setColor( col ); }

    void setDebugBallColor( const QColor & col ) { M_debug_ball_brush.setColor( col ); }
    void setDebugBallTraceColor( const QColor & col )
      {
          M_debug_ball_trace_pen.setColor( col );
          M_debug_ball_trace_brush.setColor( col );
      }
    void setDebugPlayerPenColor( const QColor & col ) { M_debug_player_pen.setColor( col ); }
    void setDebugSelfColor( const QColor & col ) { M_debug_self_brush.setColor( col ); }
    void setDebugTeammateColor( const QColor & col ) { M_debug_teammate_brush.setColor( col ); }
    void setDebugOpponentColor( const QColor & col ) { M_debug_opponent_brush.setColor( col ); }
    void setDebugUnknownTeammateColor( const QColor & col ) { M_debug_unknown_teammate_brush.setColor( col ); }
    void setDebugUnknownOpponentColor( const QColor & col ) { M_debug_unknown_opponent_brush.setColor( col ); }
    void setDebugUnknownPlayerColor( const QColor & col ) { M_debug_unknown_player_brush.setColor( col ); }

    void setDebugPointtoColor( const QColor & col ) { M_debug_pointto_pen.setColor( col ); }
    void setDebugTargetColor( const QColor & col ) { M_debug_target_pen.setColor( col ); }
    void setDebugCommentColor( const QColor & col ) { M_debug_comment_font_pen.setColor( col ); }
    void setDebugMessageColor( const QColor & col ) { M_debug_message_font_pen.setColor( col ); }
    void setDebugLogMessageColor( const QColor & col ) { M_debug_log_message_font_pen.setColor( col ); }
    void setDebugShapeColor( const QColor & col ) { M_debug_shape_pen.setColor( col ); }
    void setDebugActionSequenceColor( const QColor & col ) { M_debug_action_sequence_pen.setColor( col ); }
    void setDebugActionPassColor( const QColor & col ) { M_debug_action_pass_pen.setColor( col ); }

    //

    void setScoreBoardFont( const QFont & font );
    void setBallFont( const QFont & font ) { M_ball_font = font; }
    void setPlayerFont( const QFont & font ) { M_player_font = font; }
    void setMeasureFont( const QFont & font ) { M_measure_font = font; }
    void setDebugCommentFont( const QFont & font ) { M_debug_comment_font = font; }
    void setDebugMessageFont( const QFont & font ) { M_debug_message_font = font; }
    void setDebugLogMessageFont( const QFont & font ) { M_debug_log_message_font = font; }
};

#endif
