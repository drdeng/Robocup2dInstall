// -*-c++-*-

/*!
  \file view_config_dialog.h
  \brief View Config Dialog class Header File.
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

#ifndef SOCCERWINDOW2_QT_VIEW_CONFIG_DIALOG_H
#define SOCCERWINDOW2_QT_VIEW_CONFIG_DIALOG_H

#include <QDialog>

class QCheckBox;
class QCloseEvent;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QSlider;
class QSpinBox;
class QTabWidget;

class MainData;


//! field canvas configuration dialog
class ViewConfigDialog
    : public QDialog {

    Q_OBJECT

private:

    const MainData & M_main_data;

    QTabWidget * M_tab_widget;

    // zoom control
    QSlider * M_scale_slider;
    QLineEdit * M_scale_text;

    // object size
    QCheckBox * M_enlarge_cb;
    QLineEdit * M_ball_size_text;
    QLineEdit * M_player_size_text;

    // canvas size control
    QLineEdit * M_canvas_width_text;
    QLineEdit * M_canvas_height_text;

    // misc options
    QCheckBox * M_anti_aliasing_cb;
    QCheckBox * M_gradient_cb;

    // reverse mode control
    QCheckBox * M_reverse_side_cb;
    QCheckBox * M_player_reverse_draw_cb;

    // player detail control
    QCheckBox * M_player_number_cb;
    QCheckBox * M_player_type_cb;
    QCheckBox * M_stamina_cb;
    QCheckBox * M_stamina_capacity_cb;
    QCheckBox * M_view_area_cb;
    QCheckBox * M_body_shadow_cb;
    QCheckBox * M_catchable_area_cb;
    QCheckBox * M_tackle_area_cb;
    QCheckBox * M_kick_accel_area_cb;
    QCheckBox * M_pointto_cb;
    QCheckBox * M_attentionto_cb;
    QCheckBox * M_card_cb;
    QCheckBox * M_illegal_defense_cb;

    // show/hide control
    QCheckBox * M_anonymous_mode_cb;
    QCheckBox * M_show_score_board_cb;
    QCheckBox * M_show_team_graphic_cb;
    QCheckBox * M_show_ball_cb;
    QCheckBox * M_show_players_cb;
    QCheckBox * M_show_flags_cb;
    QCheckBox * M_show_offside_line_cb;
    QCheckBox * M_show_draw_data_cb;

    // team graphic
    QDoubleSpinBox * M_team_graphic_scale;

    // field style
    QRadioButton * M_grass_normal_rb;
    QRadioButton * M_grass_line_rb;
    QRadioButton * M_grass_checker_rb;

    QCheckBox * M_keepaway_mode_cb;

    QCheckBox * M_show_grid_coord_cb;
    QSlider * M_grid_step_slider;
    QLineEdit * M_grid_step_text;

    QComboBox * M_draw_type_choice;

    // cursor
    QCheckBox * M_cursor_hide_cb;

    // computational geometry control
    QCheckBox * M_show_voronoi_cb;
    QCheckBox * M_show_delaunay_cb;
    QComboBox * M_voronoi_choice;

    // focus control
    QRadioButton * M_focus_ball_rb;
    QRadioButton * M_focus_player_rb;
    QRadioButton * M_focus_fix_rb;

    // player/coach selection
    QRadioButton * M_select_all_rb;
    QRadioButton * M_select_left_rb;
    QRadioButton * M_select_right_rb;
    QRadioButton * M_select_fix_rb;
    QRadioButton * M_unselect_rb;

    QComboBox * M_agent_choice;

    // trace control
    QLineEdit * M_ball_trace_start;
    QLineEdit * M_ball_trace_end;
    QCheckBox * M_ball_trace_cb;
    QLineEdit * M_player_trace_start;
    QLineEdit * M_player_trace_end;
    QCheckBox * M_player_trace_cb;

    QLineEdit * M_auto_trace_start;
    QSpinBox * M_auto_trace_period;

    // inertia movement control
    QSpinBox * M_ball_future;
    QSpinBox * M_player_future;
    QSpinBox * M_player_movable;

    // mouse measure
    QComboBox * M_mouse_measure_choice;
    QDoubleSpinBox * M_mouse_measure_first_length;

public:

    ViewConfigDialog( QWidget * parent,
                      const MainData & main_data );
    ~ViewConfigDialog();
private:

    void createWidgets();

    QWidget * createZoomControls();
    QWidget * createObjectSizeControls();
    QWidget * createCanvasSizeControls();
    QWidget * createTeamGraphicControls();
    QWidget * createReverseControls();
    QWidget * createPlayersDetailControls();
    QWidget * createShowControls();
    QWidget * createFieldStyleControls();
    QWidget * createMiscControls();
    QWidget * createGridStepControls();
    QWidget * createCompGeomControls();
    QWidget * createFocusControls();
    QWidget * createPlayerSelectionControls();
    QWidget * createTraceControls();
    QWidget * createInertiaMoveControls();
    QWidget * createMouseMeasureControls();


protected:

    void showEvent( QShowEvent * event );

private slots:

    // just update this dialog contents
    // no emit
    void updateAll();

    //-----------------------

    // connect to itself
    // emit configured()
    void clickEnlarge( bool checked );
    void slideFieldScale( int value );
    void editFieldScale( const QString & );

    void clickReverseSide( bool checked );
    void clickPlayerReverseDraw( bool checked );

    void clickShowPlayerNumber( bool checked );
    void clickShowPlayerType( bool checked );
    void clickShowStamina( bool checked );
    void clickShowStaminaCapacity( bool checked );
    void clickShowViewArea( bool checked );
    void clickShowBodyShadow( bool checked );
    void clickShowCatchableArea( bool checked );
    void clickShowTackleArea( bool checked );
    void clickShowKickAccelArea( bool checked );
    void clickShowPointto( bool checked );
    void clickShowAttentionto( bool checked );
    void clickShowCard( bool checked );
    void clickShowIllegalDefense( bool checked );

    void clickAnonymousMode( bool checked );
    void clickShowScoreBoard( bool checked );
    void clickShowTeamGraphic( bool checked );
    void clickShowBall( bool checked );
    void clickShowPlayers( bool checked );
    void clickShowFlags( bool checked );
    void clickShowOffsideLine( bool checked );
    void clickShowDrawData( bool checked );

    void clickGrassNormal();
    void clickGrassLine();
    void clickGrassChecker();

    void clickKeepawayMode( bool checked );

    void clickShowGridCoord( bool checked );
    void slideGridStep( int value );
    void editGridStep( const QString & text );
    void selectDrawType( int index );

    void clickCursorHide( bool checked );
    void clickAntiAliasing( bool checked );
    void clickGradient( bool checked );

    void clickShowVoronoiDiagram( bool checked );
    void clickShowDelaunayTriangulation( bool checked );
    void selectCompGeomSide( int index );

    void clickFocusBall();
    void clickFocusPlayer();

    void clickSelectAutoAll();
    void clickSelectAutoLeft();
    void clickSelectAutoRight();
    void clickSelectFix();

    void selectAgent( int number );
    void choiceAgent( int number );

    void editBallTraceStart( const QString & text );
    void editBallTraceEnd( const QString & text );
    void clickBallTraceAll();
    void clickAutoBallTrace( bool checked );

    void editPlayerTraceStart( const QString & text );
    void editPlayerTraceEnd( const QString & text );
    void clickPlayerTraceAll();
    void clickAutoPlayerTrace( bool checked );

    void editAutoTraceStart( const QString & text );
    void changeAutoTracePeriod( int value );

    void clickLinePointButton();

    void changeBallFutureCycle( int value );
    void changePlayerFutureCycle( int value );
    void changePlayerMovableCycle( int value );

    void selectMouseMeasureMode( int index );
    void changeMouseMeasureFirstLength( double value );

public slots:

    // just update this dialog contents
    // no emit
    void updateFieldScale();

    // connect to other objects
    // emit configured()
    void zoomIn();
    void zoomOut();
    void fitToScreen();

    void toggleEnlarge();
    void editBallSize( const QString & text );
    void editPlayerSize( const QString & text );

    void applyCanvasSize();

    void slotTeamGraphicScaleChanged( double value );

    void toggleReverseSide();
    void togglePlayerReverseDraw();

    void toggleShowPlayerNumber();
    void toggleShowPlayerType();
    void toggleShowStamina();
    void toggleShowStaminaCapacity();
    void toggleShowViewArea();
    void toggleShowBodyShadow();
    void toggleShowCatchableArea();
    void toggleShowTackleArea();
    void toggleShowKickAccelArea();
    void toggleShowPointto();
    void toggleShowAttentionto();
    void toggleShowCard();
    void toggleShowIllegalDefense();

    void toggleAnonymousMode();
    void toggleShowScoreBoard();
    void toggleShowTeamGraphic();
    void toggleShowPlayers();
    void toggleShowBall();
    void toggleShowFlags();
    void toggleShowOffsideLine();
    void toggleShowDrawData();

    void toggleFocusBall();
    void toggleFocusPlayer();
    void setFocusFix();
    void setFocusPoint( const QPoint & point );

    void toggleSelectAutoAll();
    void toggleSelectAutoLeft();
    void toggleSelectAutoRight();
    void toggleSelectFix();
    void setUnselect();

    void selectAgentWithKey();
    void selectLeftCoach();
    void selectRightCoach();

    void toggleKeepawayMode();

signals:

    void configured();
    void canvasResized( const QSize & size );

};

#endif
