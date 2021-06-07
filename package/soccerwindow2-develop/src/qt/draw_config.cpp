// -*-c++-*-

/*!
  \file draw_config.cpp
  \brief draw object holder class Source File.
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

#include "draw_config.h"

#include "options.h"

#include <iostream>

/*-------------------------------------------------------------------*/
const QColor DrawConfig::SCORE_BOARD_COLOR = QColor( 255, 255, 255 );
const QColor DrawConfig::SCORE_BOARD_FONT_COLOR = QColor( 0, 0, 0 );
const QColor DrawConfig::FIELD_COLOR = QColor( 31, 160, 31 );
const QColor DrawConfig::FIELD_DARK_COLOR = QColor( 15, 143, 15 ); //( 20, 150, 20 );
const QColor DrawConfig::LINE_COLOR = QColor( 255, 255, 255 );
const QColor DrawConfig::BALL_PEN_COLOR = QColor( 255, 255, 255 );
const QColor DrawConfig::BALL_COLOR = QColor( 255, 255, 255 );
const QColor DrawConfig::BALL_FONT_COLOR = QColor( 255, 255, 255 );
const QColor DrawConfig::PLAYER_PEN_COLOR = QColor( 0, 0, 0 );
const QColor DrawConfig::REAL_BODY_PEN_COLOR = QColor( 55, 255, 255 );
const QColor DrawConfig::LEFT_TEAM_COLOR = QColor( 255, 215, 0 );
const QColor DrawConfig::LEFT_GOALIE_COLOR = QColor( 40, 230, 30 );
const QColor DrawConfig::RIGHT_TEAM_COLOR = QColor( 240, 20, 20 );
const QColor DrawConfig::RIGHT_GOALIE_COLOR = QColor( 195, 55, 240 );
const QColor DrawConfig::PLAYER_NUMBER_FONT_COLOR = QColor( 255, 255, 255 );
const QColor DrawConfig::PLAYER_STAMINA_FONT_COLOR = QColor( 15, 255, 141 );//QColor( 252, 232, 31 );
const QColor DrawConfig::PLAYER_DETAIL_FONT_COLOR = QColor( 255, 255, 255 );
const QColor DrawConfig::SELECTED_PLAYER_FONT_COLOR = QColor( 255, 255, 255 );
const QColor DrawConfig::EFFORT_DECAYED_PEN_COLOR = QColor( 255, 0, 0 );
const QColor DrawConfig::RECOVERY_DECAYED_PEN_COLOR = QColor( 252, 231, 31 );
const QColor DrawConfig::VIEW_CONE_PEN_COLOR = QColor( 55, 255, 255 );
const QColor DrawConfig::VIEW_AREA_PEN_COLOR = QColor( 255, 255, 255 );
const QColor DrawConfig::KICK_PEN_COLOR = QColor( 255, 255, 255 );
const QColor DrawConfig::KICK_FAULT_COLOR = QColor( 0, 255, 0 );
const QColor DrawConfig::CATCH_COLOR = QColor( 10, 80, 10 );
const QColor DrawConfig::CATCH_FAULT_COLOR = QColor( 10, 80, 150 );
const QColor DrawConfig::BALL_COLLISION_COLOR = QColor( 5, 255, 255 );
const QColor DrawConfig::PLAYER_COLLISION_COLOR = QColor( 105, 185, 255 );
const QColor DrawConfig::TACKLE_COLOR = QColor( 255, 136, 127 );
const QColor DrawConfig::TACKLE_FAULT_COLOR = QColor( 79, 159, 159 );
const QColor DrawConfig::FOUL_CHARGED_COLOR = QColor( 0, 127, 0 );
const QColor DrawConfig::POINTTO_COLOR = QColor( 255, 136, 127 );
const QColor DrawConfig::ILLEGAL_DEFENSE_COLOR = QColor( 255, 0, 0 );
const QColor DrawConfig::MEASURE_COLOR = QColor( 0, 255, 255 );
const QColor DrawConfig::MEASURE_FONT_COLOR = QColor( 255, 191, 191 );
const QColor DrawConfig::DEBUG_BALL_COLOR = QColor( 255, 0, 127 );
const QColor DrawConfig::DEBUG_SELF_COLOR = QColor( 255, 127, 0 );
const QColor DrawConfig::DEBUG_TEAMMATE_COLOR = QColor( 0, 255, 95 );
const QColor DrawConfig::DEBUG_OPPONENT_COLOR = QColor( 0, 95, 255 );
const QColor DrawConfig::DEBUG_UNKNOWN_TEAMMATE_COLOR = QColor( 215, 255, 0 );
const QColor DrawConfig::DEBUG_UNKNOWN_OPPONENT_COLOR = QColor( 127, 95, 255 );
const QColor DrawConfig::DEBUG_UNKNOWN_PLAYER_COLOR = QColor( 191, 191, 191 );
const QColor DrawConfig::DEBUG_COMMENT_FONT_COLOR = QColor( 191, 191, 191 );
const QColor DrawConfig::DEBUG_MESSAGE_FONT_COLOR = QColor( 255, 255, 255 );
const QColor DrawConfig::DEBUG_POINTTO_COLOR = QColor( 127, 127, 127 );
const QColor DrawConfig::DEBUG_TARGET_COLOR = QColor( 255, 127, 0 );
const QColor DrawConfig::DEBUG_LOG_MESSAGE_FONT_COLOR = QColor( 0, 0, 0 );
const QColor DrawConfig::DEBUG_SHAPE_COLOR = QColor( 255, 127, 0 );
const QColor DrawConfig::DEBUG_ACTION_SEQUENCE_COLOR = QColor( 255, 255, 0 );
const QColor DrawConfig::DEBUG_ACTION_PASS_COLOR = QColor( 255, 160, 255 );

/*-------------------------------------------------------------------*/
/*!

*/
DrawConfig::DrawConfig()
    : M_transparent_pen( Qt::NoPen ),
      M_transparent_brush( Qt::NoBrush ),
      M_score_board_brush( SCORE_BOARD_COLOR, Qt::SolidPattern ),
      M_score_board_font( "Sans Serif", 16 ),
      M_score_board_font_pen( SCORE_BOARD_FONT_COLOR, 0, Qt::SolidLine ),
      M_field_brush( FIELD_COLOR, Qt::SolidPattern ),
      M_field_dark_brush( FIELD_DARK_COLOR, Qt::SolidPattern ),
      M_line_pen( LINE_COLOR, 1, Qt::SolidLine ),
      M_ball_pen( BALL_PEN_COLOR, 0, Qt::SolidLine ),
      M_ball_brush( BALL_COLOR, Qt::SolidPattern ),
      M_ball_font( "Sans Serif", 10 ),
      M_ball_font_pen( BALL_FONT_COLOR, 0, Qt::SolidLine ),
      M_player_pen( PLAYER_PEN_COLOR, 0, Qt::SolidLine ),
      M_real_body_pen( REAL_BODY_PEN_COLOR, 0, Qt::SolidLine ),
      M_shadow_brush( QColor( 0, 0, 0 ), Qt::SolidPattern ),
      M_left_team_pen( LEFT_TEAM_COLOR, 0, Qt::SolidLine ),
      M_left_team_brush( LEFT_TEAM_COLOR, Qt::SolidPattern ),
      M_left_goalie_pen( LEFT_GOALIE_COLOR, 0, Qt::SolidLine ),
      M_left_goalie_stretch_pen( LEFT_GOALIE_COLOR, 0, Qt::DotLine ),
      M_left_goalie_brush( LEFT_GOALIE_COLOR, Qt::SolidPattern ),
      M_right_team_pen( RIGHT_TEAM_COLOR, 0, Qt::SolidLine ),
      M_right_team_brush( RIGHT_TEAM_COLOR, Qt::SolidPattern ),
      M_right_goalie_pen( RIGHT_GOALIE_COLOR, 0, Qt::SolidLine ),
      M_right_goalie_stretch_pen( RIGHT_GOALIE_COLOR, 0, Qt::DotLine ),
      M_right_goalie_brush( RIGHT_GOALIE_COLOR, Qt::SolidPattern ),
      M_player_font( "Sans Serif", 10 ),
      M_player_number_font_pen( PLAYER_NUMBER_FONT_COLOR, 0, Qt::SolidLine ),
      M_player_stamina_font_pen( PLAYER_STAMINA_FONT_COLOR, 0, Qt::SolidLine ),
      M_player_detail_font_pen( PLAYER_DETAIL_FONT_COLOR, 0, Qt::SolidLine ),
      M_selected_player_font_pen( SELECTED_PLAYER_FONT_COLOR, 0, Qt::SolidLine ),
      M_effort_decayed_pen( EFFORT_DECAYED_PEN_COLOR, 0, Qt::SolidLine ),
      M_recovery_decayed_pen( RECOVERY_DECAYED_PEN_COLOR, 0, Qt::SolidLine ),
      M_view_cone_pen( VIEW_CONE_PEN_COLOR, 0, Qt::SolidLine ),
      M_view_area_pen( VIEW_AREA_PEN_COLOR, 0, Qt::SolidLine ),
      M_kick_pen( KICK_PEN_COLOR, 2, Qt::SolidLine ),
      M_kick_fault_pen( KICK_FAULT_COLOR, 2, Qt::SolidLine ),
      M_kick_fault_brush( KICK_FAULT_COLOR, Qt::SolidPattern ),
      M_catch_brush( CATCH_COLOR, Qt::SolidPattern ),
      M_catch_fault_brush( CATCH_FAULT_COLOR, Qt::SolidPattern ),
      M_ball_collision_brush( BALL_COLLISION_COLOR, Qt::SolidPattern ),
      M_player_collision_brush( PLAYER_COLLISION_COLOR, Qt::SolidPattern ),
      M_tackle_pen( TACKLE_COLOR, 2, Qt::SolidLine ),
      M_tackle_brush( TACKLE_COLOR, Qt::SolidPattern ),
      M_tackle_fault_brush( TACKLE_FAULT_COLOR, Qt::SolidPattern ),
      M_foul_charged_brush( FOUL_CHARGED_COLOR, Qt::SolidPattern ),
      M_pointto_pen( POINTTO_COLOR, 0, Qt::SolidLine ),
      M_illegal_defense_pen( ILLEGAL_DEFENSE_COLOR, 0, Qt::SolidLine ),
      M_kick_accel_pen( QColor( 0, 255, 0 ), 0, Qt::SolidLine ),
      M_tackle_area_pen( TACKLE_COLOR, 0, Qt::SolidLine ),
      M_measure_pen( MEASURE_COLOR, 1, Qt::SolidLine ),
      M_measure_font( "Sans Serif", 10 ),
      M_measure_font_pen( MEASURE_FONT_COLOR, 0, Qt::SolidLine ),
      M_debug_ball_brush( DEBUG_BALL_COLOR, Qt::SolidPattern ),
      M_debug_ball_trace_pen( DEBUG_BALL_COLOR, 0, Qt::SolidLine ),
      M_debug_ball_trace_brush( DEBUG_BALL_COLOR, Qt::SolidPattern ),
      M_debug_player_pen( QColor( 127, 127, 127 ), 0, Qt::SolidLine ),
      M_debug_self_brush( DEBUG_SELF_COLOR, Qt::SolidPattern ),
      M_debug_teammate_brush( DEBUG_TEAMMATE_COLOR, Qt::SolidPattern ),
      M_debug_opponent_brush( DEBUG_OPPONENT_COLOR, Qt::SolidPattern ),
      M_debug_unknown_teammate_brush( DEBUG_UNKNOWN_TEAMMATE_COLOR, Qt::SolidPattern ),
      M_debug_unknown_opponent_brush( DEBUG_UNKNOWN_OPPONENT_COLOR, Qt::SolidPattern ),
      M_debug_unknown_player_brush( DEBUG_UNKNOWN_PLAYER_COLOR, Qt::SolidPattern ),
      M_debug_pointto_pen( DEBUG_POINTTO_COLOR, 2, Qt::SolidLine ),
      M_debug_target_pen( DEBUG_TARGET_COLOR, 2, Qt::SolidLine ),
      M_debug_comment_font( "Sans Serif", 10 ),
      M_debug_comment_font_pen( DEBUG_COMMENT_FONT_COLOR, 0, Qt::SolidLine ),
      M_debug_message_font( "Sans Serif", 14 ),
      M_debug_message_font_pen( DEBUG_MESSAGE_FONT_COLOR, 0, Qt::SolidLine ),
      M_debug_log_message_font( "Sans Serif", 8 ),
      M_debug_log_message_font_pen( DEBUG_LOG_MESSAGE_FONT_COLOR, 0, Qt::SolidLine ),
      M_debug_shape_pen( DEBUG_SHAPE_COLOR, 1, Qt::SolidLine ),
      M_debug_action_sequence_pen( DEBUG_ACTION_SEQUENCE_COLOR, 3, Qt::SolidLine ),
      M_debug_action_pass_pen( DEBUG_ACTION_PASS_COLOR, 3, Qt::SolidLine )
{
    //std::cerr << "create DrawConfig" << std::endl;
    readSettings();

    for ( int i = 0; i < 256; ++i )
    {
        M_shadow_brushes[i].setStyle( Qt::SolidPattern );
        M_shadow_brushes[i].setColor( QColor( i, i, i ) );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
DrawConfig::~DrawConfig()
{
    //std::cerr << "delete DrawConfig" << std::endl;
    saveSettings();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DrawConfig::setDefaultColors()
{
    M_score_board_brush.setColor( SCORE_BOARD_COLOR );
    M_score_board_font_pen.setColor( SCORE_BOARD_FONT_COLOR );
    M_field_brush.setColor( FIELD_COLOR );
    M_field_dark_brush.setColor( FIELD_DARK_COLOR );
    M_line_pen.setColor( LINE_COLOR );
    M_ball_pen.setColor( BALL_PEN_COLOR );
    M_ball_brush.setColor( BALL_COLOR );
    M_ball_font_pen.setColor( BALL_FONT_COLOR );
    M_player_pen.setColor( PLAYER_PEN_COLOR );
    M_real_body_pen.setColor( REAL_BODY_PEN_COLOR );
    M_shadow_brush.setColor( QColor( 0, 0, 0 ) );
    M_left_team_pen.setColor( LEFT_TEAM_COLOR );
    M_left_team_brush.setColor( LEFT_TEAM_COLOR );
    M_left_goalie_pen.setColor( LEFT_GOALIE_COLOR );
    M_left_goalie_stretch_pen.setColor( LEFT_GOALIE_COLOR );
    M_left_goalie_brush.setColor( LEFT_GOALIE_COLOR );
    M_right_team_pen.setColor( RIGHT_TEAM_COLOR );
    M_right_team_brush.setColor( RIGHT_TEAM_COLOR );
    M_right_goalie_pen.setColor( RIGHT_GOALIE_COLOR );
    M_right_goalie_stretch_pen.setColor( RIGHT_GOALIE_COLOR );
    M_right_goalie_brush.setColor( RIGHT_GOALIE_COLOR );
    M_player_number_font_pen.setColor( PLAYER_NUMBER_FONT_COLOR );
    M_player_stamina_font_pen.setColor( PLAYER_STAMINA_FONT_COLOR );
    M_player_detail_font_pen.setColor( PLAYER_DETAIL_FONT_COLOR );
    M_selected_player_font_pen.setColor( SELECTED_PLAYER_FONT_COLOR );
    M_effort_decayed_pen.setColor( EFFORT_DECAYED_PEN_COLOR );
    M_recovery_decayed_pen.setColor( RECOVERY_DECAYED_PEN_COLOR );
    M_view_cone_pen.setColor( VIEW_CONE_PEN_COLOR );
    M_view_area_pen.setColor( VIEW_AREA_PEN_COLOR );
    M_kick_pen.setColor( KICK_PEN_COLOR );
    M_kick_fault_pen.setColor( KICK_FAULT_COLOR );
    M_kick_fault_brush.setColor( KICK_FAULT_COLOR );
    M_catch_brush.setColor( CATCH_COLOR );
    M_catch_fault_brush.setColor( CATCH_FAULT_COLOR );
    M_ball_collision_brush.setColor( BALL_COLLISION_COLOR );
    M_player_collision_brush.setColor( PLAYER_COLLISION_COLOR );
    M_tackle_pen.setColor( TACKLE_COLOR );
    M_tackle_brush.setColor( TACKLE_COLOR );
    M_tackle_fault_brush.setColor( TACKLE_FAULT_COLOR );
    M_foul_charged_brush.setColor( FOUL_CHARGED_COLOR );
    M_pointto_pen.setColor( POINTTO_COLOR );
    M_illegal_defense_pen.setColor( ILLEGAL_DEFENSE_COLOR );
    M_kick_accel_pen.setColor( QColor( 0, 255, 0 ) );
    M_tackle_area_pen.setColor( TACKLE_COLOR );
    M_measure_pen.setColor( MEASURE_COLOR );
    M_measure_font_pen.setColor( MEASURE_FONT_COLOR );
    M_debug_ball_brush.setColor( DEBUG_BALL_COLOR );
    M_debug_ball_trace_pen.setColor( DEBUG_BALL_COLOR );
    M_debug_ball_trace_brush.setColor( DEBUG_BALL_COLOR );
    M_debug_player_pen.setColor( QColor( 127, 127, 127 ) );
    M_debug_self_brush.setColor( DEBUG_SELF_COLOR );
    M_debug_teammate_brush.setColor( DEBUG_TEAMMATE_COLOR );
    M_debug_opponent_brush.setColor( DEBUG_OPPONENT_COLOR );
    M_debug_unknown_teammate_brush.setColor( DEBUG_UNKNOWN_TEAMMATE_COLOR );
    M_debug_unknown_opponent_brush.setColor( DEBUG_UNKNOWN_OPPONENT_COLOR );
    M_debug_unknown_player_brush.setColor( DEBUG_UNKNOWN_PLAYER_COLOR );
    M_debug_pointto_pen.setColor( DEBUG_POINTTO_COLOR );
    M_debug_target_pen.setColor( DEBUG_TARGET_COLOR );
    M_debug_comment_font_pen.setColor( DEBUG_COMMENT_FONT_COLOR );
    M_debug_message_font_pen.setColor( DEBUG_MESSAGE_FONT_COLOR );
    M_debug_log_message_font_pen.setColor( DEBUG_LOG_MESSAGE_FONT_COLOR );
    M_debug_shape_pen.setColor( DEBUG_SHAPE_COLOR );
    M_debug_action_sequence_pen.setColor( DEBUG_ACTION_SEQUENCE_COLOR );
    M_debug_action_pass_pen.setColor( DEBUG_ACTION_PASS_COLOR );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DrawConfig::setDefaultFonts()
{
    M_score_board_font.setFamily( "Sans Serif" );
    M_score_board_font.setPointSize( 16 );

    M_ball_font.setFamily( "Sans Serif" );
    M_ball_font.setPointSize( 10 );

    M_player_font.setFamily( "Sans Serif" );
    M_player_font.setPointSize( 10 );

    M_measure_font.setFamily( "Sans Serif" );
    M_measure_font.setPointSize( 10 );

    M_debug_comment_font.setFamily( "Sans Serif" );
    M_debug_comment_font.setPointSize( 10 );

    M_debug_message_font.setFamily( "Sans Serif" );
    M_debug_message_font.setPointSize( 14 );

    M_debug_log_message_font.setFamily( "Sans Serif" );
    M_debug_log_message_font.setPointSize( 8 );


    QFontMetrics fm( M_score_board_font );
    Options::instance().setScoreBoardFontHeight( fm.height() );
}


/*-------------------------------------------------------------------*/
/*!

*/
void
DrawConfig::readSettings()
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


    settings.beginGroup( "DrawConfig" );

    QVariant val;

    // font

    val = settings.value( "score_board_font" );
    if ( val.isValid() )
    {
        M_score_board_font.fromString( val.toString() );
    }
    val = settings.value( "ball_font" );
    if ( val.isValid() )
    {
        M_ball_font.fromString( val.toString() );
    }
    val = settings.value( "player_font" );
    if ( val.isValid() )
    {
        M_player_font.fromString( val.toString() );
    }
    val = settings.value( "measure_font" );
    if ( val.isValid() )
    {
        M_measure_font.fromString( val.toString() );
    }
    val = settings.value( "debug_comment_font" );
    if ( val.isValid() )
    {
        M_debug_comment_font.fromString( val.toString() );
    }
    val = settings.value( "debug_message_font" );
    if ( val.isValid() )
    {
        M_debug_message_font.fromString( val.toString() );
    }

    // color

    // score boar
    val = settings.value( "score_board_color" );
    if ( val.isValid() )
    {
        M_score_board_brush = toColor( val.toString() );
    }

    val = settings.value( "score_board_font_color" );
    if ( val.isValid() )
    {
        M_score_board_font_pen.setColor( val.toString() );
    }

    // field
    val = settings.value( "field_color" );
    if ( val.isValid() )
    {
        M_field_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "field_dark_color" );
    if ( val.isValid() )
    {
        M_field_dark_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "line_color" );
    if ( val.isValid() )
    {
        M_line_pen.setColor( toColor( val.toString() ) );
    }

    // ball
    val = settings.value( "ball_pen_color" );
    if ( val.isValid() )
    {
        M_ball_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "ball_color" );
    if ( val.isValid() )
    {
        M_ball_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "ball_font_color" );
    if ( val.isValid() )
    {
        M_ball_font_pen.setColor( toColor( val.toString() ) );
    }

    // player body
    val = settings.value( "player_pen_color" );
    if ( val.isValid() )
    {
        M_player_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "real_body_pen_color" );
    if ( val.isValid() )
    {
        M_real_body_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "left_team_color" );
    if ( val.isValid() )
    {
        M_left_team_pen.setColor( toColor( val.toString() ) );
        M_left_team_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "left_goalie_color" );
    if ( val.isValid() )
    {
        M_left_goalie_pen.setColor( toColor( val.toString() ) );
        M_left_goalie_stretch_pen.setColor( toColor( val.toString() ) );
        M_left_goalie_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "right_team_color" );
    if ( val.isValid() )
    {
        M_right_team_pen.setColor( toColor( val.toString() ) );
        M_right_team_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "right_goalie_color" );
    if ( val.isValid() )
    {
        M_right_goalie_pen.setColor( toColor( val.toString() ) );
        M_right_goalie_stretch_pen.setColor( toColor( val.toString() ) );
        M_right_goalie_brush.setColor( toColor( val.toString() ) );
    }

    // player font
    val = settings.value( "player_number_font_color" );
    if ( val.isValid() )
    {
        M_player_number_font_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "player_stamina_font_color" );
    if ( val.isValid() )
    {
        M_player_stamina_font_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "player_detail_font_color" );
    if ( val.isValid() )
    {
        M_player_detail_font_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "selected_player_font_color" );
    if ( val.isValid() )
    {
        M_selected_player_font_pen.setColor( toColor( val.toString() ) );
    }

    // player pen
    val = settings.value( "effort_decayed_pen_color" );
    if ( val.isValid() )
    {
        M_effort_decayed_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "recovery_decayed_pen_color" );
    if ( val.isValid() )
    {
        M_recovery_decayed_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "view_cone_pen_color" );
    if ( val.isValid() )
    {
        M_view_cone_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "view_area_pen_color" );
    if ( val.isValid() )
    {
        M_view_area_pen.setColor( toColor( val.toString() ) );
    }

    // player status
    val = settings.value( "kick_pen_color" );
    if ( val.isValid() )
    {
        M_kick_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "kick_fault_color" );
    if ( val.isValid() )
    {
        M_kick_fault_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "catch_color" );
    if ( val.isValid() )
    {
        M_catch_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "catch_fault_color" );
    if ( val.isValid() )
    {
        M_catch_fault_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "ball_collision_color" );
    if ( val.isValid() )
    {
        M_ball_collision_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "player_collision_color" );
    if ( val.isValid() )
    {
        M_player_collision_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "tackle_color" );
    if ( val.isValid() )
    {
        M_tackle_pen.setColor( toColor( val.toString() ) );
        M_tackle_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "tackle_area_color" );
    if ( val.isValid() )
    {
        M_tackle_area_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "tackle_fault_color" );
    if ( val.isValid() )
    {
        M_tackle_fault_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "foul_charged_color" );
    if ( val.isValid() )
    {
        M_foul_charged_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "pointto_color" );
    if ( val.isValid() )
    {
        M_pointto_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "illegal_defense_color" );
    if ( val.isValid() )
    {
        M_illegal_defense_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "kick_accel_color" );
    if ( val.isValid() )
    {
        M_kick_accel_pen.setColor( toColor( val.toString() ) );
    }

    // measure
    val = settings.value( "measure_color" );
    if ( val.isValid() )
    {
        M_measure_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "measure_font_color" );
    if ( val.isValid() )
    {
        M_measure_font_pen.setColor( toColor( val.toString() ) );
    }

    // debug view
    val = settings.value( "debug_teammate_color" );
    if ( val.isValid() )
    {
        M_debug_teammate_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "debug_opponent_color" );
    if ( val.isValid() )
    {
        M_debug_opponent_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "debug_unknown_teammate_color" );
    if ( val.isValid() )
    {
        M_debug_unknown_teammate_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "debug_unknown_opponent_color" );
    if ( val.isValid() )
    {
        M_debug_unknown_opponent_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "debug_unknown_player_color" );
    if ( val.isValid() )
    {
        M_debug_unknown_player_brush.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "debug_pointto_pen_color" );
    if ( val.isValid() )
    {
        M_debug_pointto_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "debug_target_pen_color" );
    if ( val.isValid() )
    {
        M_debug_target_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "debug_comment_font_color" );
    if ( val.isValid() )
    {
        M_debug_comment_font_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "debug_message_font_color" );
    if ( val.isValid() )
    {
        M_debug_message_font_pen.setColor( toColor( val.toString() ) );
    }

    // debug log
    val = settings.value( "debug_log_message_font_color" );
    if ( val.isValid() )
    {
        M_debug_log_message_font_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "debug_shape_pen_color" );
    if ( val.isValid() )
    {
        M_debug_shape_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "debug_action_sequence_color" );
    if ( val.isValid() )
    {
        M_debug_action_sequence_pen.setColor( toColor( val.toString() ) );
    }

    val = settings.value( "debug_action_pass_color" );
    if ( val.isValid() )
    {
        M_debug_action_pass_pen.setColor( toColor( val.toString() ) );
    }

    settings.endGroup();


    QFontMetrics fm( M_score_board_font );
    Options::instance().setScoreBoardFontHeight( fm.height() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DrawConfig::saveSettings()
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
    //                   "rctools",
    //                    PACKAGE_NAME );

    settings.beginGroup( "DrawConfig" );

    // font

    settings.setValue( "score_board_font",
                       scoreBoardFont().toString() );
    settings.setValue( "ball_font",
                       ballFont().toString() );
    settings.setValue( "player_font",
                       playerFont().toString() );
    settings.setValue( "measure_font",
                       measureFont().toString() );
    settings.setValue( "debug_comment_font",
                       debugCommentFont().toString() );
    settings.setValue( "debug_message_font",
                       debugMessageFont().toString() );

    // color

    // score board
    settings.setValue( "score_board_color",
                       toString( scoreBoardBrush().color() ) );
    settings.setValue( "score_board_font_color",
                       toString( scoreBoardFontPen().color() ) );
    // field
    settings.setValue( "field_color",
                       toString( fieldBrush().color() ) );
    settings.setValue( "field_dark_color",
                       toString( fieldDarkBrush().color() ) );
    settings.setValue( "line_color",
                       toString( linePen().color() ) );
    // ball
    settings.setValue( "ball_pen_color",
                       toString( ballPen().color() ) );
    settings.setValue( "ball_color",
                       toString( ballBrush().color() ) );
    settings.setValue( "ball_font_color",
                       toString( ballFontPen().color() ) );
    // player body
    settings.setValue( "player_pen_color",
                       toString( playerPen().color() ) );
    settings.setValue( "real_body_pen_color",
                       toString( realBodyPen().color() ) );
    settings.setValue( "left_team_color",
                       toString( leftTeamPen().color() ) );
    settings.setValue( "left_goalie_color",
                       toString( leftGoaliePen().color() ) );
    settings.setValue( "right_team_color",
                       toString( rightTeamPen().color() ) );
    settings.setValue( "right_goalie_color",
                       toString( rightGoaliePen().color() ) );
    settings.setValue( "player_number_font_color",
                       toString( playerNumberFontPen().color() ) );
    settings.setValue( "player_stamina_font_color",
                       toString( playerStaminaFontPen().color() ) );
    settings.setValue( "player_detail_font_color",
                       toString( playerDetailFontPen().color() ) );
    settings.setValue( "selected_player_font_color",
                       toString( selectedPlayerFontPen().color() ) );
    // player pen
    settings.setValue( "effort_decayed_pen_color",
                       toString( effortDecayedPen().color() ) );
    settings.setValue( "recovery_decayed_pen_color",
                       toString( recoveryDecayedPen().color() ) );
    settings.setValue( "view_cone_pen_color",
                       toString( viewConePen().color() ) );
    settings.setValue( "view_area_pen_color",
                       toString( viewAreaPen().color() ) );
    // player status
    settings.setValue( "kick_pen_color",
                       toString( kickPen().color() ) );
    settings.setValue( "kick_fault_color",
                       toString( kickFaultBrush().color() ) );
    settings.setValue( "catch_color",
                       toString( catchBrush().color() ) );
    settings.setValue( "catch_fault_color",
                       toString( catchFaultBrush().color() ) );
    settings.setValue( "ball_collision_color",
                       toString( ballCollisionBrush().color() ) );
    settings.setValue( "player_collision_color",
                       toString( playerCollisionBrush().color() ) );
    settings.setValue( "tackle_color",
                       toString( tackleBrush().color() ) );
    settings.setValue( "tackle_area_color",
                       toString( tackleAreaPen().color() ) );
    settings.setValue( "tackle_fault_color",
                       toString( tackleFaultBrush().color() ) );
    settings.setValue( "foul_charged_color",
                       toString( foulChargedBrush().color() ) );
    settings.setValue( "pointto_color",
                       toString( pointtoPen().color() ) );
    settings.setValue( "illegal_defense_color",
                       toString( illegalDefensePen().color() ) );
    settings.setValue( "kick_accel_color",
                       toString( kickAccelPen().color() ) );

    settings.setValue( "measure_color",
                       toString( measurePen().color() ) );
    settings.setValue( "measure_font_color",
                       toString( measureFontPen().color() ) );

    settings.setValue( "debug_teammate_color",
                       toString( debugTeammateBrush().color() ) );
    settings.setValue( "debug_opponent_color",
                       toString( debugOpponentBrush().color() ) );
    settings.setValue( "debug_unknown_teammate_color",
                       toString( debugUnknownTeammateBrush().color() ) );
    settings.setValue( "debug_unknown_opponent_color",
                       toString( debugUnknownOpponentBrush().color() ) );
    settings.setValue( "debug_unknown_player_color",
                       toString( debugUnknownPlayerBrush().color() ) );
    settings.setValue( "debug_pointto_pen_color",
                       toString( debugPointtoPen().color() ) );
    settings.setValue( "debug_target_pen_color",
                       toString( debugTargetPen().color() ) );
    settings.setValue( "debug_comment_font_color",
                       toString( debugCommentFontPen().color() ) );
    settings.setValue( "debug_message_font_color",
                       toString( debugMessageFontPen().color() ) );

    settings.setValue( "debug_log_message_font_color",
                       toString( debugLogMessageFontPen().color() ) );
    settings.setValue( "debug_shape_pen_color",
                       toString( debugShapePen().color() ) );

    settings.setValue( "debug_action_sequence_color",
                       toString( debugActionSequencePen().color() ) );
    settings.setValue( "debug_action_pass_color",
                       toString( debugActionPassPen().color() ) );

    settings.endGroup();
}

/*-------------------------------------------------------------------*/
/*!

*/
QColor
DrawConfig::toColor( const QString & conf_str )
{
    QColor color;
    color.setNamedColor( conf_str );
    return color;
}

/*-------------------------------------------------------------------*/
/*!

*/
QString
DrawConfig::toString( const QColor & color )
{
    return color.name();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DrawConfig::setScoreBoardFont( const QFont & font )
{
    M_score_board_font = font;

    QFontMetrics fm( font );
    Options::instance().setScoreBoardFontHeight( fm.height() );
}
