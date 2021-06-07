// -*-c++-*-

/*!
  \file view_config_dialog.cpp
  \brief View Config Dialog class Source File.
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

#include "view_config_dialog.h"

#include "options.h"
#include "main_data.h"

#include <iostream>
#include <cassert>

/*-------------------------------------------------------------------*/
/*!

 */
ViewConfigDialog::ViewConfigDialog( QWidget * parent,
                                    const MainData & main_data )
    : QDialog( parent )
    , M_main_data( main_data )
{
    this->setWindowTitle( tr( "View Preference" ) );

    //QFont font = this->font();
    //font.setPointSize( 8 );
    //this->setFont( font );

    createWidgets();
}

/*-------------------------------------------------------------------*/
/*!

 */
ViewConfigDialog::~ViewConfigDialog()
{
    //std::cerr << "delete ViewConfigDialog" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::createWidgets()
{
    M_tab_widget = new QTabWidget();
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setSizeConstraint( QLayout::SetFixedSize );

    // show
    {
        QFrame * frame = new QFrame();
        QVBoxLayout * layout = new QVBoxLayout();
        layout->setSizeConstraint( QLayout::SetFixedSize );
        layout->setContentsMargins( 4, 4, 4, 4 );
        layout->setSpacing( 4 );

        layout->addWidget( createShowControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createPlayersDetailControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createReverseControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createGridStepControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createCompGeomControls(),
                           0, Qt::AlignLeft );

        frame->setLayout( layout );
        M_tab_widget->addTab( frame, tr( "Show" ) );
    }
    // object features
    {
        QFrame * frame = new QFrame();
        QVBoxLayout * layout = new QVBoxLayout();
        layout->setSizeConstraint( QLayout::SetFixedSize );
        layout->setContentsMargins( 4, 4, 4, 4 );
        layout->setSpacing( 4 );

        layout->addWidget( createObjectSizeControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createFocusControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createPlayerSelectionControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createTraceControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createInertiaMoveControls(),
                           0, Qt::AlignLeft );

        frame->setLayout( layout );
        M_tab_widget->addTab( frame, tr( "Object" ) );
    }
    // canvas
    {
        QFrame * frame = new QFrame();
        QVBoxLayout * layout = new QVBoxLayout();
        layout->setSizeConstraint( QLayout::SetFixedSize );
        layout->setContentsMargins( 4, 4, 4, 4 );
        layout->setSpacing( 4 );

        layout->addWidget( createZoomControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createCanvasSizeControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createTeamGraphicControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createFieldStyleControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createMiscControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createMouseMeasureControls(),
                           0, Qt::AlignLeft );

        frame->setLayout( layout );
        M_tab_widget->addTab( frame, tr( "Canvas" ) );
    }

    top_layout->addWidget( M_tab_widget );
    this->setLayout( top_layout );
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createZoomControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Zoom" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 0 );

    const QFontMetrics metrics = this->fontMetrics();
    const int button_height = metrics.height() + 12;

    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setSpacing( 0 );

        QPushButton * zoomin_button = new QPushButton( tr( "+" ) );
        zoomin_button->setMaximumSize( 48, button_height );
        connect( zoomin_button, SIGNAL( clicked() ),
                 this, SLOT( zoomIn() ) );
        layout->addWidget( zoomin_button );
        //
        QPushButton * zoomout_button = new QPushButton( tr( "-" ) );
        zoomout_button->setMaximumSize( 48, button_height );
        connect( zoomout_button, SIGNAL( clicked() ),
                 this, SLOT( zoomOut() ) );
        layout->addWidget( zoomout_button );
        //
        layout->addSpacing( 1 );
        layout->addStretch( 1 );
        //
        QPushButton * fit_button = new QPushButton( tr( "Fit" ) );
        fit_button->setMaximumSize( 60, button_height );
        connect( fit_button, SIGNAL( clicked() ),
                 this, SLOT( fitToScreen() ) );
        layout->addWidget( fit_button );

        top_layout->addLayout( layout );
    }

    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setSpacing( 0 );

        layout->addWidget( new QLabel( tr( "Scale:" ) ) );

        layout->addSpacing( 2 );

        M_scale_slider = new QSlider( Qt::Horizontal );
        M_scale_slider->setRange( static_cast< int >( Options::MIN_FIELD_SCALE
                                                      * 10.0 ),
                                  static_cast< int >( Options::MAX_FIELD_SCALE
                                                      * 10.0 ) );
        M_scale_slider->setValue( 80 );
        M_scale_slider->setSingleStep( 1 );
        M_scale_slider->setPageStep( 1 ); // set the minimal increment/decrement step
        M_scale_slider->setMinimumSize( 260, 24 );
        connect( M_scale_slider, SIGNAL( sliderMoved( int ) ),
                 this, SLOT( slideFieldScale( int ) ) );
        layout->addWidget( M_scale_slider );

        layout->addSpacing( 2 );

        M_scale_text = new QLineEdit( tr( "0.0" ) );
        M_scale_text->setValidator( new QDoubleValidator( 0.0, 400.0, 3, M_scale_text ) );
        M_scale_text->setMaximumSize( 48, 24 );
        connect( M_scale_text, SIGNAL( textChanged( const QString & ) ),
                 this, SLOT( editFieldScale( const QString & ) ) );
        layout->addWidget( M_scale_text );

        top_layout->addLayout( layout );
    }


    group_box->setLayout( top_layout );
    return group_box;;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createObjectSizeControls()
{
    const Options & opt = Options::instance();

    QGroupBox * group_box = new QGroupBox( tr( "Object Size" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 0 );

    M_enlarge_cb = new QCheckBox( tr( "Enlarge Objects" ) );
    M_enlarge_cb->setChecked( opt.enlargeMode() );
    connect( M_enlarge_cb, SIGNAL( clicked( bool ) ),
             this, SLOT( clickEnlarge( bool ) ) );
    top_layout->addWidget( M_enlarge_cb );


    QHBoxLayout * box = new QHBoxLayout();
    //
    box->addWidget( new QLabel( tr( "Ball Size:" ) ) );
    //
    M_ball_size_text = new QLineEdit( tr( "0.35" ) );
    M_ball_size_text->setValidator( new QDoubleValidator( 0.01, 100.0, 3, M_ball_size_text ) );
    M_ball_size_text->setMaximumSize( 48, 24 );
    connect( M_ball_size_text, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( editBallSize( const QString & ) ) );
    box->addWidget( M_ball_size_text );
    //
    box->addWidget( new QLabel( tr( " Player Size:" ) ) );
    //
    M_player_size_text = new QLineEdit( tr( "0.0" ) );
    M_player_size_text->setValidator( new QDoubleValidator( 0.0, 100.0, 3, M_ball_size_text ) );
    M_player_size_text->setMaximumSize( 48, 24 );
    connect( M_player_size_text, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( editPlayerSize( const QString & ) ) );
    box->addWidget( M_player_size_text );
    //
    top_layout->addLayout( box );

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createCanvasSizeControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Canvas Size" ) );

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins( 1, 1, 1, 1 );
    layout->setSpacing( 0 );

    layout->addWidget( new QLabel( tr( " Width:" ) ) );

    M_canvas_width_text = new QLineEdit( tr( "640" ) );
    M_canvas_width_text->setValidator( new QIntValidator( 100, 3000, M_canvas_width_text ) );
    M_canvas_width_text->setMaximumSize( 48, 24 );
    layout->addWidget( M_canvas_width_text );

    layout->addWidget( new QLabel( tr( " Height:" ) ) );

    M_canvas_height_text = new QLineEdit( tr( "480" ) );
    M_canvas_height_text->setValidator( new QIntValidator( 100, 3000, M_canvas_height_text ) );
    M_canvas_height_text->setMaximumSize( 48, 24 );
    layout->addWidget( M_canvas_height_text );

    layout->addSpacing( 12 );

    QPushButton * apply_canvas_size_btn = new QPushButton( tr( "apply" ) );
    apply_canvas_size_btn->setMaximumSize( 60, this->fontMetrics().height() + 12 );
    connect( apply_canvas_size_btn, SIGNAL( clicked() ),
             this, SLOT( applyCanvasSize() ) );
    layout->addWidget( apply_canvas_size_btn );

    group_box->setLayout( layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createTeamGraphicControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Team Graphic" ) );

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins( 1, 1, 1, 1 );
    layout->setSpacing( 0 );

    layout->addWidget( new QLabel( tr( "Scale:" ) ) );
    //
    M_team_graphic_scale = new QDoubleSpinBox();
    M_team_graphic_scale->setRange( 0.01, 16.0 );
    M_team_graphic_scale->setSingleStep( 0.01 );
    M_team_graphic_scale->setMaximumSize( 128, 24 );
    connect( M_team_graphic_scale, SIGNAL( valueChanged( double ) ),
             this, SLOT( slotTeamGraphicScaleChanged( double ) ) );
    layout->addWidget( M_team_graphic_scale );

    group_box->setLayout( layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createReverseControls()
{
    const Options & opt = Options::instance();

    QGroupBox * group_box = new QGroupBox( tr( "Reverse" ) );

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins( 1, 1, 1, 1 );
    layout->setSpacing( 0 );

    M_reverse_side_cb = new QCheckBox( tr( "Reverse Side" ));
    M_reverse_side_cb->setChecked( opt.reverseSide() );
    connect( M_reverse_side_cb, SIGNAL( toggled( bool ) ),
             this, SLOT( clickReverseSide( bool ) ) );
    layout->addWidget( M_reverse_side_cb );

    M_player_reverse_draw_cb = new QCheckBox( tr( "Reverse Player Draw Order" ) );
    M_player_reverse_draw_cb->setChecked( opt.playerReverseDraw() );
    connect( M_player_reverse_draw_cb, SIGNAL( toggled( bool ) ),
             this, SLOT( clickPlayerReverseDraw( bool ) ) );
    layout->addWidget( M_player_reverse_draw_cb );

    group_box->setLayout( layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createPlayersDetailControls()
{
    const Options & opt = Options::instance();

    QGroupBox * group_box = new QGroupBox( tr( "Players\' Detail" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 0 );

    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        M_player_number_cb = new QCheckBox( tr( "Unum" ) );
        M_player_number_cb->setChecked( opt.showPlayerNumber() );
        connect( M_player_number_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowPlayerNumber( bool ) ) );
        layout->addWidget( M_player_number_cb );
        //
        M_player_type_cb = new QCheckBox( tr( "Type" ) );
        M_player_type_cb->setChecked( opt.showPlayerType() );
        connect( M_player_type_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowPlayerType( bool ) ) );
        layout->addWidget( M_player_type_cb );
        //
        M_stamina_cb = new QCheckBox( tr( "Stamina" ) );
        M_stamina_cb->setChecked( opt.showStamina() );
        connect( M_stamina_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowStamina( bool ) ) );
        layout->addWidget( M_stamina_cb );
        //
        M_stamina_capacity_cb = new QCheckBox( tr( "Stamina Capacity" ) );
        M_stamina_capacity_cb->setChecked( opt.showStaminaCapacity() );
        connect( M_stamina_capacity_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowStaminaCapacity( bool ) ) );
        layout->addWidget( M_stamina_capacity_cb );

        top_layout->addLayout( layout );
    }
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        M_view_area_cb = new QCheckBox( tr( "View Area" ) );
        M_view_area_cb->setChecked( opt.showViewArea() );
        connect( M_view_area_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowViewArea( bool ) ) );
        layout->addWidget( M_view_area_cb );
        //
        M_body_shadow_cb = new QCheckBox( tr( "Body Shadow" ) );
        M_body_shadow_cb->setChecked( opt.showBodyShadow() );
        connect( M_body_shadow_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowBodyShadow( bool ) ) );
        layout->addWidget( M_body_shadow_cb );
        //
        M_catchable_area_cb = new QCheckBox( tr( "Catch Area" ) );
        M_catchable_area_cb->setChecked( opt.showCatchableArea() );
        connect( M_catchable_area_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowCatchableArea( bool ) ) );
        layout->addWidget( M_catchable_area_cb );

        top_layout->addLayout( layout );
    }
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        //
        M_tackle_area_cb = new QCheckBox( tr( "Tackle Area" ) );
        M_tackle_area_cb->setChecked( opt.showTackleArea() );
        connect( M_tackle_area_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowTackleArea( bool ) ) );
        layout->addWidget( M_tackle_area_cb );
        //
        M_kick_accel_area_cb = new QCheckBox( tr( "Kick Accel Area" ) );
        M_kick_accel_area_cb->setChecked( opt.showKickAccelArea() );
        connect( M_kick_accel_area_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowKickAccelArea( bool ) ) );
        layout->addWidget( M_kick_accel_area_cb );

        top_layout->addLayout( layout );
    }
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        //
        M_pointto_cb = new QCheckBox( tr( "Pointto" ) );
        M_pointto_cb->setChecked( opt.showPointto() );
        connect( M_pointto_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowPointto( bool ) ) );
        layout->addWidget( M_pointto_cb );
        //
        M_attentionto_cb = new QCheckBox( tr( "Attentionto" ) );
        M_attentionto_cb->setChecked( opt.showAttentionto() );
        connect( M_attentionto_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowAttentionto( bool ) ) );
        layout->addWidget( M_attentionto_cb );
        //
        M_card_cb = new QCheckBox( tr( "Card" ) );
        M_card_cb->setChecked( opt.showCard() );
        connect( M_card_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowCard( bool ) ) );
        layout->addWidget( M_card_cb );

        top_layout->addLayout( layout );
    }
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        //
        M_illegal_defense_cb = new QCheckBox( tr( "Illegal Defense" ) );
        M_illegal_defense_cb->setChecked( opt.showIllegalDefenseState() );
        connect( M_illegal_defense_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowIllegalDefense( bool ) ) );
        layout->addWidget( M_illegal_defense_cb );

        top_layout->addLayout( layout );
    }

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createShowControls()
{
    const Options & opt = Options::instance();

    QGroupBox * group_box = new QGroupBox( tr( "Show" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 0 );

    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        M_anonymous_mode_cb = new QCheckBox( tr( "Anonymous Mode" ) );
        M_anonymous_mode_cb->setChecked( opt.anonymousMode() );
        connect( M_anonymous_mode_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickAnonymousMode( bool ) ) );
        layout->addWidget( M_anonymous_mode_cb );

        top_layout->addLayout( layout );
    }
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        M_show_score_board_cb = new QCheckBox( tr( "Score Board" ) );
        M_show_score_board_cb->setChecked( opt.showScoreBoard() );
        connect( M_show_score_board_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowScoreBoard( bool ) ) );
        layout->addWidget( M_show_score_board_cb );
        //
        M_show_team_graphic_cb = new QCheckBox( tr( "Team Graphic" ) );
        M_show_team_graphic_cb->setChecked( opt.showTeamGraphic() );
        connect( M_show_team_graphic_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowTeamGraphic( bool ) ) );
        layout->addWidget( M_show_team_graphic_cb );
        //
        M_show_flags_cb = new QCheckBox( tr( "Flag" ) );
        M_show_flags_cb->setChecked( opt.showFlags() );
        connect( M_show_flags_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowFlags( bool ) ) );
        layout->addWidget( M_show_flags_cb );

        top_layout->addLayout( layout );
    }
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        M_show_ball_cb = new QCheckBox( tr( "Ball" ) );
        M_show_ball_cb->setChecked( opt.showBall() );
        connect( M_show_ball_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowBall( bool ) ) );
        layout->addWidget( M_show_ball_cb );
        //
        M_show_players_cb = new QCheckBox( tr( "Player" ) );
        M_show_players_cb->setChecked( opt.showPlayers() );
        connect( M_show_players_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowPlayers( bool ) ) );
        layout->addWidget( M_show_players_cb );
        //
        M_show_offside_line_cb = new QCheckBox( tr( "Offside Line" ) );
        M_show_offside_line_cb->setChecked( opt.showOffsideLine() );
        connect( M_show_offside_line_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowOffsideLine( bool ) ) );
        layout->addWidget( M_show_offside_line_cb );

        top_layout->addLayout( layout );
    }

    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        M_show_draw_data_cb = new QCheckBox( tr( "Draw Data" ) );
        M_show_draw_data_cb->setChecked( opt.showDrawData() );
        connect( M_show_draw_data_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowDrawData( bool ) ) );
        layout->addWidget( M_show_draw_data_cb );

        top_layout->addLayout( layout );
    }

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createCompGeomControls()
{
    const Options & opt = Options::instance();

    QGroupBox * group_box = new QGroupBox( tr( "Compututational Geometry" ) );
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 0 );

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins( 1, 1, 1, 1 );
    layout->setSpacing( 0 );

    {
        M_show_voronoi_cb = new QCheckBox( tr( "VoronoiDiagram" ) );
        M_show_voronoi_cb->setChecked( opt.showVoronoiDiagram() );
        connect( M_show_voronoi_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowVoronoiDiagram( bool ) ) );
        layout->addWidget( M_show_voronoi_cb );
    }
    {
        M_show_delaunay_cb = new QCheckBox( tr( "DelaunayTriangulation" ) );
        M_show_delaunay_cb->setChecked( opt.showDelaunayTriangulation() );
        connect( M_show_delaunay_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowDelaunayTriangulation( bool ) ) );
        layout->addWidget( M_show_delaunay_cb );
    }
    top_layout->addLayout( layout );

    {
        M_voronoi_choice = new QComboBox();
        M_voronoi_choice->addItem( tr( "All" ) );
        M_voronoi_choice->addItem( tr( "Left Team" ) );
        M_voronoi_choice->addItem( tr( "Right Team" ) );
        M_voronoi_choice->setMaximumSize( this->fontMetrics().width( tr( "Right Team" ) ) + 32,
                                          this->fontMetrics().height() + 12 );
        M_voronoi_choice->setCurrentIndex( 0 );
        connect( M_voronoi_choice, SIGNAL( currentIndexChanged( int ) ),
                 this, SLOT( selectCompGeomSide( int ) ) );
        top_layout->addWidget( M_voronoi_choice );
    }

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createFocusControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Focus" ) );

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins( 1, 1, 1, 1 );
    layout->setSpacing( 0 );

    {
        M_focus_ball_rb = new QRadioButton( tr( "Ball" ) );
        connect( M_focus_ball_rb, SIGNAL( clicked() ),
                 this, SLOT( clickFocusBall() ) );
        layout->addWidget( M_focus_ball_rb );
    }
    {
        M_focus_player_rb = new QRadioButton( tr( "Selected Player" ) );
        connect( M_focus_player_rb, SIGNAL( clicked() ),
                 this, SLOT( clickFocusPlayer() ) );
        layout->addWidget( M_focus_player_rb );
    }
    {
        M_focus_fix_rb = new QRadioButton( tr( "Fix" ) );
        connect( M_focus_fix_rb, SIGNAL( clicked() ),
                 this, SLOT( setFocusFix() ) );
        layout->addWidget( M_focus_fix_rb );
    }
    M_focus_fix_rb->setChecked( true );

    group_box->setLayout( layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createPlayerSelectionControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Player Selection" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 0 );

    // selection type
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );
        {
            M_select_all_rb = new QRadioButton( tr( "Auto" ) );
            connect( M_select_all_rb, SIGNAL( clicked() ),
                     this, SLOT( clickSelectAutoAll() ) );
            layout->addWidget( M_select_all_rb );
        }
        {
            M_select_left_rb = new QRadioButton( tr( "Auto left" ) );
            connect( M_select_left_rb, SIGNAL( clicked() ),
                     this, SLOT( clickSelectAutoLeft() ) );
            layout->addWidget( M_select_left_rb );
        }
        {
            M_select_right_rb = new QRadioButton( tr( "Auto right" ) );
            connect( M_select_right_rb, SIGNAL( clicked() ),
                     this, SLOT( clickSelectAutoRight() ) );
            layout->addWidget( M_select_right_rb );
        }
        {
            M_select_fix_rb = new QRadioButton( tr( "Fix" ) );
            connect( M_select_fix_rb, SIGNAL( clicked() ),
                     this, SLOT( clickSelectFix() ) );
            layout->addWidget( M_select_fix_rb );
        }
        {
            M_unselect_rb = new QRadioButton( tr( "Unselect" ) );
            connect( M_unselect_rb, SIGNAL( clicked() ),
                     this, SLOT( setUnselect() ) );
            layout->addWidget( M_unselect_rb );
        }
        M_unselect_rb->setChecked( true );

        top_layout->addLayout( layout );
    }

    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );
        //layout->addSpacing( 16 );

        M_agent_choice = new QComboBox();
        M_agent_choice->addItem( tr ( "No Selection" ) );

        for ( int i = 1; i <= 11; ++i )
        {
            M_agent_choice->addItem( QString( "Left number: %1" ).arg( i ) );
        }
        M_agent_choice->addItem( tr( "Left Coach" ) );

        for ( int i = 1; i <= 11; ++i )
        {
            M_agent_choice->addItem( QString( "Right number: %1" ).arg( i ) );
        }
        M_agent_choice->addItem( tr( "Right Coach" ) );

        M_agent_choice->setCurrentIndex( 0 );

        int id_width = this->fontMetrics().width( tr( "Right Number: 11" ) );
        M_agent_choice->setMaximumWidth( id_width + 40 );
        connect( M_agent_choice, SIGNAL( currentIndexChanged( int ) ),
                 this, SLOT( choiceAgent( int ) ) );
        layout->addWidget( M_agent_choice, 0, Qt::AlignLeft );

        layout->addSpacing( 16 );

        top_layout->addLayout( layout );
    }

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createFieldStyleControls()
{
    const Options & opt = Options::instance();

    QGroupBox * group_box = new QGroupBox( tr( "Field Style" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 0 );

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins( 1, 1, 1, 1  );
    layout->setSpacing( 0 );

    {
        M_keepaway_mode_cb = new QCheckBox( tr( "Keepaway" ) );
        M_keepaway_mode_cb->setChecked( opt.keepawayMode() );
        connect( M_keepaway_mode_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickKeepawayMode( bool ) ) );
        layout->addWidget( M_keepaway_mode_cb );
    }
    {
        M_grass_normal_rb = new QRadioButton( tr( "mono" ) );
        connect( M_grass_normal_rb, SIGNAL( clicked() ),
                 this, SLOT( clickGrassNormal() ) );
        layout->addWidget( M_grass_normal_rb );
    }
    {
        M_grass_line_rb = new QRadioButton( tr( "lines" ) );
        connect( M_grass_line_rb, SIGNAL( clicked() ),
                 this, SLOT( clickGrassLine() ) );
        layout->addWidget( M_grass_line_rb );
    }
    {
        M_grass_checker_rb = new QRadioButton( tr( "checker" ) );
        connect( M_grass_checker_rb, SIGNAL( clicked() ),
                 this, SLOT( clickGrassChecker() ) );
        layout->addWidget( M_grass_checker_rb );
    }
    top_layout->addLayout( layout );

    {
        M_draw_type_choice = new QComboBox();
        M_draw_type_choice->addItem( tr( "default" ) );
        M_draw_type_choice->addItem( tr( "rcssmonitor" ) );
        connect( M_draw_type_choice, SIGNAL( currentIndexChanged( int ) ),
                 this, SLOT( selectDrawType( int ) ) );
        top_layout->addWidget( M_draw_type_choice );
    }

    switch ( Options::instance().fieldGrassType() ) {
    case Options::GRASS_NORMAL:
        M_grass_normal_rb->setChecked( true );
        break;
    case Options::GRASS_LINES:
        M_grass_line_rb->setChecked( true );
        break;
    case Options::GRASS_CHECKER:
        M_grass_checker_rb->setChecked( true );
        break;
    default:
        M_grass_normal_rb->setChecked( true );
        break;
    }

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createMiscControls()
{
    const Options & opt = Options::instance();

    QGroupBox * group_box = new QGroupBox( tr( "Misc" ) );
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 0 );

    M_cursor_hide_cb = new QCheckBox( tr( "Automatic Cursor Hide" ) );
    M_cursor_hide_cb->setChecked( opt.cursorHide() );
    connect( M_cursor_hide_cb, SIGNAL( toggled( bool ) ),
             this, SLOT( clickCursorHide( bool ) ) );
    top_layout->addWidget( M_cursor_hide_cb );
    //
    M_anti_aliasing_cb = new QCheckBox( tr( "Anti Aliasing" ) );
    M_anti_aliasing_cb->setChecked( opt.antiAliasing() );
    connect( M_anti_aliasing_cb, SIGNAL( toggled( bool ) ),
             this, SLOT( clickAntiAliasing( bool ) ) );
    top_layout->addWidget( M_anti_aliasing_cb );
    //
    M_gradient_cb = new QCheckBox( tr( "Gradient" ) );
    M_gradient_cb->setChecked( opt.gradient() );
    connect( M_gradient_cb, SIGNAL( toggled( bool ) ),
             this, SLOT( clickGradient( bool ) ) );
    top_layout->addWidget( M_gradient_cb );

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createGridStepControls()
{
    const Options & opt = Options::instance();

    QGroupBox * group_box = new QGroupBox( tr( "Grid Lines" ) );
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 1, 1, 1, 1 );
    top_layout->setSpacing( 0 );

    M_show_grid_coord_cb = new QCheckBox( tr( "Grid Coordinate" ) );
    M_show_grid_coord_cb->setChecked( opt.showGridCoord() );
    connect( M_show_grid_coord_cb, SIGNAL( toggled( bool ) ),
             this, SLOT( clickShowGridCoord( bool ) ) );
    top_layout->addWidget( M_show_grid_coord_cb );

    //
    QHBoxLayout * layout = new QHBoxLayout();
    //layout->setContentsMargins( 0 );
    //layout->setSpacing( 0 );
    //
    layout->addWidget( new QLabel( tr( "Step:" ) ) );
    //
    M_grid_step_slider = new QSlider( Qt::Horizontal );
    M_grid_step_slider->setRange( 0, 1000 );
    M_grid_step_slider->setValue( 0 );
    M_grid_step_slider->setSingleStep( 1 );
    M_grid_step_slider->setPageStep( 1 );
    M_grid_step_slider->setMinimumSize( 300, 24 );
    connect( M_grid_step_slider, SIGNAL( sliderMoved( int ) ),
             this, SLOT( slideGridStep( int ) ) );
    layout->addWidget( M_grid_step_slider );

    //
    M_grid_step_text = new QLineEdit( tr( " 000.0" )  );
    M_grid_step_text->setValidator( new QDoubleValidator( 0.0, 200.0, 2, M_grid_step_text ) );
    M_grid_step_text->setMaximumSize( 48, 24 );
    connect( M_grid_step_text, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( editGridStep( const QString & ) ) );
    layout->addWidget( M_grid_step_text );

    top_layout->addLayout( layout );
    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createTraceControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Trace" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 0, 0, 0, 0 );
    top_layout->setSpacing( 0 );

    //     QHBoxLayout * first_layout = new QHBoxLayout();
    //     first_layout->setContentsMargins( 0, 0, 0, 0 );
    //     first_layout->setSpacing( 0 );
    {
        QGroupBox * ball_box = new QGroupBox( tr( "Ball" ) );

        QHBoxLayout * layout = new QHBoxLayout();
        layout->setSizeConstraint( QLayout::SetFixedSize );
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        M_ball_trace_start = new QLineEdit( tr( "0" ) );
        M_ball_trace_start->setValidator( new QIntValidator( 0, 20000, M_ball_trace_start ) );
        M_ball_trace_start->setMaximumSize( 36, 24 );
        connect( M_ball_trace_start, SIGNAL( textEdited( const QString & ) ),
                 this, SLOT( editBallTraceStart( const QString & ) ) );
        layout->addWidget( M_ball_trace_start, 0, Qt::AlignVCenter );

        layout->addWidget( new QLabel( tr( "-" ) ), 0, Qt::AlignVCenter );

        M_ball_trace_end = new QLineEdit( tr( "0" ) );
        M_ball_trace_end->setValidator( new QIntValidator( 0, 20000, M_ball_trace_end ) );
        M_ball_trace_end->setMaximumSize( 36, 24 );
        connect( M_ball_trace_end, SIGNAL( textEdited( const QString & ) ),
                 this, SLOT( editBallTraceEnd( const QString & ) ) );
        layout->addWidget( M_ball_trace_end, 0, Qt::AlignVCenter );

        QPushButton * all_btn = new QPushButton( tr( "All" ) );
        all_btn->setMaximumSize( 48, this->fontMetrics().height() + 12 );
        all_btn->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
        connect( all_btn, SIGNAL( clicked() ),
                 this, SLOT( clickBallTraceAll() ) );
        layout->addWidget( all_btn, 0, Qt::AlignVCenter );

        M_ball_trace_cb = new QCheckBox( tr( "Auto" ) );
        M_ball_trace_cb->setChecked( false );
        connect( M_ball_trace_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickAutoBallTrace( bool ) ) );
        layout->addWidget( M_ball_trace_cb, 0, Qt::AlignVCenter );

        ball_box->setLayout( layout );
        //first_layout->addWidget( ball_box );
        top_layout->addWidget( ball_box );
    }
    //first_layout->addSpacing( 2 );
    top_layout->addSpacing( 2 );
    {
        QGroupBox * player_box = new QGroupBox( tr( "Player" ) );

        QHBoxLayout * layout= new QHBoxLayout();
        layout->setSizeConstraint( QLayout::SetFixedSize );
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        M_player_trace_start = new QLineEdit( tr( "0" ) );
        M_player_trace_start->setValidator( new QIntValidator( 0, 20000, M_player_trace_start ) );
        M_player_trace_start->setMaximumSize( 36, 24 );
        connect( M_player_trace_start, SIGNAL( textEdited( const QString & ) ),
                 this, SLOT( editPlayerTraceStart( const QString & ) ) );
        layout->addWidget( M_player_trace_start, 0, Qt::AlignVCenter );

        layout->addWidget( new QLabel( tr( "-" ) ), 0, Qt::AlignVCenter );

        M_player_trace_end = new QLineEdit( tr( "0" ) );
        M_player_trace_end->setValidator( new QIntValidator( 0, 20000, M_player_trace_end ) );
        M_player_trace_end->setMaximumSize( 36, 24 );
        connect( M_player_trace_end, SIGNAL( textEdited( const QString & ) ),
                 this, SLOT( editPlayerTraceEnd( const QString & ) ) );
        layout->addWidget( M_player_trace_end, 0, Qt::AlignVCenter );

        QPushButton * all_btn = new QPushButton( tr( "All" ) );
        all_btn->setMaximumSize( 48, this->fontMetrics().height() + 12 );
        all_btn->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
        connect( all_btn, SIGNAL( clicked() ),
                 this, SLOT( clickPlayerTraceAll() ) );
        layout->addWidget( all_btn, 0, Qt::AlignVCenter );

        M_player_trace_cb =  new QCheckBox( tr( "Auto" ) );
        M_player_trace_cb->setChecked( false );
        connect( M_player_trace_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickAutoPlayerTrace( bool ) ) );
        layout->addWidget( M_player_trace_cb, 0, Qt::AlignVCenter );

        player_box->setLayout( layout );
        //first_layout->addWidget( player_box );
        top_layout->addWidget( player_box );
    }

    //top_layout->addLayout( first_layout );
    top_layout->addSpacing( 1 );

    QHBoxLayout * second_layout = new QHBoxLayout();
    second_layout->setContentsMargins( 0, 0, 0, 0 );
    second_layout->setSpacing( 0 );
    {
        QGroupBox * trace_box = new QGroupBox( tr( "Auto Trace Period" ) );

        QHBoxLayout * layout = new QHBoxLayout();
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setSpacing( 0 );

        layout->addWidget( new QLabel( tr( "Start:" ) ), 0, Qt::AlignVCenter );

        layout->addSpacing( 2 );

        M_auto_trace_start = new QLineEdit( tr( "-10" ) );
        M_auto_trace_start->setValidator( new QIntValidator( -20000, 20000, M_auto_trace_start ) );
        M_auto_trace_start->setMinimumSize( 36, this->fontMetrics().height() + 4 );
        M_auto_trace_start->setMaximumSize( 48, this->fontMetrics().height() + 4 );
        connect( M_auto_trace_start, SIGNAL( textEdited( const QString & ) ),
                 this, SLOT( editAutoTraceStart( const QString & ) ) );
        layout->addWidget( M_auto_trace_start, 0, Qt::AlignVCenter );

        layout->addSpacing( 6 );

        layout->addWidget( new QLabel( tr( "Period:" ) ), 0, Qt::AlignVCenter );

        layout->addSpacing( 2 );

        M_auto_trace_period = new QSpinBox();
        M_auto_trace_period->setValue( 10 );
        M_auto_trace_period->setRange( 1, 12000 );
        //M_auto_trace_period->setMaximumSize( 36, 24 );
        connect( M_auto_trace_period, SIGNAL( valueChanged( int ) ),
                 this, SLOT( changeAutoTracePeriod( int ) ) );
        layout->addWidget( M_auto_trace_period, 0, Qt::AlignVCenter );

        trace_box->setLayout( layout );

        second_layout->addWidget( trace_box, 1 );
    }

    second_layout->addSpacing( 12 );
    {
        //         QHBoxLayout * layout = new QHBoxLayout();
        //         layout->setContentsMargins( 0 );
        //         layout->setSpacing( 0 );

        //         layout->addStretch( 1 );

        QPushButton * line_point_btn = new QPushButton( tr( "Line/Point" ) );
        line_point_btn->setMaximumSize( 80, this->fontMetrics().height() + 12 );
        connect( line_point_btn, SIGNAL( clicked() ),
                 this, SLOT( clickLinePointButton() ) );

        second_layout->addWidget( line_point_btn, 0, Qt::AlignLeft );
        //         layout->addWidget( line_point_btn, 2 );

        //         layout->addStretch( 1 );

        //         second_layout->addLayout( layout, 1 );
    }
    second_layout->addSpacing( 6 );

    top_layout->addLayout( second_layout );

    group_box->setLayout( top_layout );
    return group_box;;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createInertiaMoveControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Move" ) );

    QHBoxLayout * top_layout = new QHBoxLayout();
    top_layout->setContentsMargins( 0, 0, 0, 0 );
    top_layout->setSpacing( 0 );

    {
        top_layout->addWidget( new QLabel( tr( "Ball:" ) ) );
        top_layout->addSpacing( 2 );

        M_ball_future = new QSpinBox();
        //M_ball_future->setMinimumSize( 60, 24 );
        M_ball_future->setValue( 0 );
        M_ball_future->setRange( 0, 50 );
        connect( M_ball_future, SIGNAL( valueChanged( int ) ),
                 this, SLOT( changeBallFutureCycle( int ) ) );
        top_layout->addWidget( M_ball_future );
    }
    top_layout->addSpacing( 6 );
    {
        top_layout->addWidget( new QLabel( tr( "Player:" ) ) );
        top_layout->addSpacing( 2 );

        M_player_future = new QSpinBox();
        //M_player_future->setMaximumSize( 60, 24 );
        M_player_future->setValue( 0 );
        M_player_future->setRange( 0, 10 );
        connect( M_player_future, SIGNAL( valueChanged( int ) ),
                 this, SLOT( changePlayerFutureCycle( int ) ) );
        top_layout->addWidget( M_player_future );
    }
    {
        top_layout->addWidget( new QLabel( tr( "Movable:" ) ) );
        top_layout->addSpacing( 2 );

        M_player_movable = new QSpinBox();
        //M_player_movable->setMaximumSize( 60, 24 );
        M_player_movable->setValue( 0 );
        M_player_movable->setRange( 0, 30 );
        connect( M_player_movable, SIGNAL( valueChanged( int ) ),
                 this, SLOT( changePlayerMovableCycle( int ) ) );
        top_layout->addWidget( M_player_movable );
    }

    group_box->setLayout( top_layout );
    return group_box;
}


/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ViewConfigDialog::createMouseMeasureControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Mouse Measure" ) );

    QHBoxLayout * top_layout = new QHBoxLayout();
    top_layout->setContentsMargins( 0, 0, 0, 0 );
    top_layout->setSpacing( 0 );


    top_layout->addWidget( new QLabel( tr( "Mode:" ) ) );
    top_layout->addSpacing( 2 );
    {
        M_mouse_measure_choice = new QComboBox();
        M_mouse_measure_choice->addItem( tr( "None" ) );
        M_mouse_measure_choice->addItem( tr( "Ball Move" ) );
        M_mouse_measure_choice->addItem( tr( "Player Move" ) );
        M_mouse_measure_choice->setMaximumSize( this->fontMetrics().width( tr( "Player Move" ) ) + 32,
                                                this->fontMetrics().height() + 12 );
        M_mouse_measure_choice->setCurrentIndex( static_cast< int >( Options::MEASURE_BALL_MOVE ) );
        connect( M_mouse_measure_choice, SIGNAL( currentIndexChanged( int ) ),
                 this, SLOT( selectMouseMeasureMode( int ) ) );
        top_layout->addWidget( M_mouse_measure_choice );
    }

    top_layout->addSpacing( 6 );

    top_layout->addWidget( new QLabel( tr( "First Length:" ) ) );
    top_layout->addSpacing( 2 );
    {
        M_mouse_measure_first_length = new QDoubleSpinBox();
        M_mouse_measure_first_length->setRange( -1.0, 5.0 );
        M_mouse_measure_first_length->setValue( -1.0 );
        M_mouse_measure_first_length->setDecimals( 3 );
        M_mouse_measure_first_length->setSingleStep( 0.01 );
        connect( M_mouse_measure_first_length, SIGNAL( valueChanged( double ) ),
                 this, SLOT( changeMouseMeasureFirstLength( double ) ) );
        top_layout->addWidget( M_mouse_measure_first_length );
    }

    group_box->setLayout( top_layout );
    return group_box;
}


/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::showEvent( QShowEvent * event )
{
    updateAll();
    event->accept();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::updateAll()
{
    const Options & opt = Options::instance();

    M_enlarge_cb->setChecked( opt.enlargeMode() );
    M_ball_size_text->setText( QString::number( opt.ballSize() ) );
    M_player_size_text->setText( QString::number( opt.playerSize() ) );

    updateFieldScale();

    M_canvas_width_text->setText( QString::number( opt.canvasWidth() ) );
    M_canvas_height_text->setText( QString::number( opt.canvasHeight() ) );

    M_team_graphic_scale->setValue( opt.teamGraphicScale() );

    M_player_number_cb->setChecked( opt.showPlayerNumber() );
    M_player_type_cb->setChecked( opt.showPlayerType() );
    M_stamina_cb->setChecked( opt.showStamina() );
    M_stamina_capacity_cb->setChecked( opt.showStaminaCapacity() );
    M_view_area_cb->setChecked( opt.showViewArea() );
    M_body_shadow_cb->setChecked( opt.showBodyShadow() );
    M_catchable_area_cb->setChecked( opt.showCatchableArea() );
    M_tackle_area_cb->setChecked( opt.showTackleArea() );
    M_kick_accel_area_cb->setChecked( opt.showKickAccelArea() );
    M_pointto_cb->setChecked( opt.showPointto() );
    M_attentionto_cb->setChecked( opt.showAttentionto() );
    M_card_cb->setChecked( opt.showCard() );

    M_anonymous_mode_cb->setChecked( opt.anonymousMode() );
    M_show_score_board_cb->setChecked( opt.showScoreBoard() );
    M_show_team_graphic_cb->setChecked( opt.showTeamGraphic() );
    M_show_ball_cb->setChecked( opt.showBall() );
    M_show_players_cb->setChecked( opt.showPlayers() );
    M_show_flags_cb->setChecked( opt.showFlags() );
    M_show_offside_line_cb->setChecked( opt.showOffsideLine() );
    M_show_draw_data_cb->setChecked( opt.showDrawData() );

    switch ( Options::instance().fieldGrassType() ) {
    case Options::GRASS_NORMAL:
        M_grass_normal_rb->setChecked( true );
        break;
    case Options::GRASS_LINES:
        M_grass_line_rb->setChecked( true );
        break;
    case Options::GRASS_CHECKER:
        M_grass_checker_rb->setChecked( true );
        break;
    default:
        M_grass_normal_rb->setChecked( true );
        break;
    }

    M_keepaway_mode_cb->setChecked( opt.keepawayMode() );

    M_draw_type_choice->setCurrentIndex( opt.paintStyle() );

    M_cursor_hide_cb->setChecked( opt.cursorHide() );
    M_anti_aliasing_cb->setChecked( opt.antiAliasing() );
    M_gradient_cb->setChecked( opt.gradient() );

    M_show_voronoi_cb->setChecked( opt.showVoronoiDiagram() );
    M_show_delaunay_cb->setChecked( opt.showDelaunayTriangulation() );

    switch ( opt.voronoiTarget() ) {
    case rcsc::NEUTRAL:
        M_voronoi_choice->setCurrentIndex( 0 );
        break;
    case rcsc::LEFT:
        M_voronoi_choice->setCurrentIndex( 1 );
        break;
    case rcsc::RIGHT:
        M_voronoi_choice->setCurrentIndex( 2 );
        break;
    default:
        M_voronoi_choice->setCurrentIndex( 0 );
        break;
    }

    switch( opt.focusType() ) {
    case Options::FOCUS_BALL:
        M_focus_ball_rb->setChecked( true );
        break;
    case Options::FOCUS_PLAYER:
        M_focus_player_rb->setChecked( true );
        break;
    case Options::FOCUS_POINT:
        M_focus_fix_rb->setChecked( true );
        break;
    default:
        M_focus_fix_rb->setChecked( true );
        break;
    }

    switch ( opt.agentSelectType() ) {
    case Options::SELECT_AUTO_ALL:
        M_select_all_rb->setChecked( true );
        break;
    case Options::SELECT_AUTO_LEFT:
        M_select_left_rb->setChecked( true );
        break;
    case Options::SELECT_AUTO_RIGHT:
        M_select_right_rb->setChecked( true );
        break;
    case Options::SELECT_UNSELECT:
        M_unselect_rb->setChecked( true );
        break;
    default:
        M_unselect_rb->setChecked( true );
        break;
    }

    M_ball_trace_start->setText( QString::number( opt.ballTraceStart() ) );
    M_ball_trace_end->setText( QString::number( opt.ballTraceEnd() ) );
    M_ball_trace_cb->setChecked( opt.ballAutoTrace() );

    M_player_trace_start->setText( QString::number( opt.playerTraceStart() ) );
    M_player_trace_end->setText( QString::number( opt.playerTraceEnd() ) );
    M_player_trace_cb->setChecked( opt.playerAutoTrace() );

    M_auto_trace_start->setText( QString::number( opt.autoTraceStart() ) );
    M_auto_trace_period->setValue( opt.autoTracePeriod() );

    M_ball_future->setValue( opt.ballFutureCycle() );
    M_player_future->setValue( opt.playerFutureCycle() );
    M_player_movable->setValue( opt.playerMovableCycle() );

    M_mouse_measure_choice->setCurrentIndex( static_cast< int >( opt.mouseMeasureMode() ) );
    M_mouse_measure_first_length->setValue( opt.mouseMeasureFirstLength() );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::updateFieldScale()
{
    int iscale = static_cast< int >( rint( Options::instance().fieldScale() * 10.0 ) );
    if ( M_scale_slider->value() != iscale )
    {
        M_scale_slider->setValue( iscale );
    }

    char buf[16];
    snprintf( buf, 16, "%.2f", Options::instance().fieldScale() );
    M_scale_text->setText( QString::fromLatin1( buf ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::zoomIn()
{
    Options::instance().zoomIn();

    updateFieldScale();

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::zoomOut()
{
    Options::instance().zoomOut();

    updateFieldScale();

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::fitToScreen()
{
    Options::instance().fitToScreen();
    updateFieldScale();
    M_focus_fix_rb->setChecked( true );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickEnlarge( bool checked )
{
    if ( Options::instance().enlargeMode() != checked )
    {
        Options::instance().toggleEnlargeMode();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleEnlarge()
{
    Options::instance().toggleEnlargeMode();
    M_enlarge_cb->setChecked( Options::instance().enlargeMode() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::editBallSize( const QString & text )
{
    bool ok = true;
    double value = text.toDouble( &ok );

    if ( ok && value > 0.0 )
    {
        Options::instance().setBallSize( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::editPlayerSize( const QString & text )
{
    bool ok = true;
    double value = text.toDouble( &ok );

    if ( ok )
    {
        Options::instance().setPlayerSize( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::slideFieldScale( int value )
{
    double scale = value * 0.1;

    if ( std::fabs( scale - Options::instance().fieldScale() ) >= 0.01 )
    {
        M_scale_text->setText( QString::number( scale ) );

        Options::instance().setFieldScale( scale );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::editFieldScale( const QString & text )
{
    bool ok = true;
    double value = text.toDouble( &ok );

    if ( ok
         && std::fabs( value - Options::instance().fieldScale() ) >= 0.01 )
    {
        int ivalue = static_cast< int >( rint( value * 10.0 ) );
        M_scale_slider->setValue( ivalue );

        Options::instance().setFieldScale( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::applyCanvasSize()
{
    bool ok_w = true;
    bool ok_h = true;
    int width = M_canvas_width_text->text().toInt( & ok_w );
    int height = M_canvas_height_text->text().toInt( & ok_h );

    if ( ok_w
         && ok_h
         && width > 0
         && height > 0 )
    {
        emit canvasResized( QSize( width, height ) );
    }
}


/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::slotTeamGraphicScaleChanged( double value )
{
    if ( std::fabs( value - Options::instance().teamGraphicScale() ) >= 0.01 )
    {
        Options::instance().setTeamGraphicScale( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickReverseSide( bool checked )
{
    if ( Options::instance().reverseSide() != checked )
    {
        Options::instance().toggleReverseSide();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleReverseSide()
{
    Options::instance().toggleReverseSide();
    M_reverse_side_cb->setChecked( Options::instance().reverseSide() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickPlayerReverseDraw( bool checked )
{
    if ( Options::instance().playerReverseDraw() != checked )
    {
        Options::instance().togglePlayerReverseDraw();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::togglePlayerReverseDraw()
{
    Options::instance().togglePlayerReverseDraw();
    M_player_reverse_draw_cb->setChecked( Options::instance().playerReverseDraw() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowPlayerNumber( bool checked )
{
    if ( Options::instance().showPlayerNumber() != checked )
    {
        Options::instance().toggleShowPlayerNumber();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowPlayerNumber()
{
    Options::instance().toggleShowPlayerNumber();
    M_player_number_cb->setChecked( Options::instance().showPlayerNumber() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowPlayerType( bool checked )
{
    if ( Options::instance().showPlayerType() != checked )
    {
        Options::instance().toggleShowPlayerType();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowPlayerType()
{
    Options::instance().toggleShowPlayerType();
    M_player_type_cb->setChecked( Options::instance().showPlayerType() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowStamina( bool checked )
{
    if ( Options::instance().showStamina() != checked )
    {
        Options::instance().toggleShowStamina();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowStamina()
{
    const Options & opt = Options::instance();

    if ( ! opt.showStamina()
         && ! opt.showStaminaCapacity() )
    {
        Options::instance().toggleShowStamina();
    }
    else if ( opt.showStamina()
              && ! opt.showStaminaCapacity() )
    {
        Options::instance().toggleShowStamina();
        Options::instance().toggleShowStaminaCapacity();
    }
    else if ( ! opt.showStamina()
              && opt.showStaminaCapacity() )
    {
        Options::instance().toggleShowStamina();
    }
    else
    {
        Options::instance().toggleShowStamina();
        Options::instance().toggleShowStaminaCapacity();
    }

    M_stamina_cb->setChecked( opt.showStamina() );
    M_stamina_capacity_cb->setChecked( opt.showStaminaCapacity() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowStaminaCapacity( bool checked )
{
    if ( Options::instance().showStaminaCapacity() != checked )
    {
        Options::instance().toggleShowStaminaCapacity();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowStaminaCapacity()
{
    Options::instance().toggleShowStaminaCapacity();
    M_stamina_capacity_cb->setChecked( Options::instance().showStaminaCapacity() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowViewArea( bool checked )
{
    if ( Options::instance().showViewArea() != checked )
    {
        Options::instance().toggleShowViewArea();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowViewArea()
{
    Options::instance().toggleShowViewArea();
    M_view_area_cb->setChecked( Options::instance().showViewArea() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowBodyShadow( bool checked )
{
    if ( Options::instance().showBodyShadow() != checked )
    {
        Options::instance().toggleShowBodyShadow();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowBodyShadow()
{
    Options::instance().toggleShowBodyShadow();
    M_body_shadow_cb->setChecked( Options::instance().showBodyShadow() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowCatchableArea( bool checked )
{
    if ( Options::instance().showCatchableArea() != checked )
    {
        Options::instance().toggleShowCatchableArea();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowCatchableArea()
{
    Options::instance().toggleShowCatchableArea();
    M_catchable_area_cb->setChecked( Options::instance().showCatchableArea() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowTackleArea( bool checked )
{
    if ( Options::instance().showTackleArea() != checked )
    {
        Options::instance().toggleShowTackleArea();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowTackleArea()
{
    Options::instance().toggleShowTackleArea();
    M_tackle_area_cb->setChecked( Options::instance().showTackleArea() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowKickAccelArea( bool checked )
{
    if ( Options::instance().showKickAccelArea() != checked )
    {
        Options::instance().toggleShowKickAccelArea();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowKickAccelArea()
{
    Options::instance().toggleShowKickAccelArea();
    M_kick_accel_area_cb->setChecked( Options::instance().showKickAccelArea() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowPointto( bool checked )
{
    if ( Options::instance().showPointto() != checked )
    {
        Options::instance().toggleShowPointto();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowPointto()
{
    Options::instance().toggleShowPointto();
    M_pointto_cb->setChecked( Options::instance().showPointto() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowAttentionto( bool checked )
{
    if ( Options::instance().showAttentionto() != checked )
    {
        Options::instance().toggleShowAttentionto();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowAttentionto()
{
    Options::instance().toggleShowAttentionto();
    M_attentionto_cb->setChecked( Options::instance().showAttentionto() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowCard( bool checked )
{
    if ( Options::instance().showCard() != checked )
    {
        Options::instance().toggleShowCard();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowCard()
{
    Options::instance().toggleShowCard();
    M_card_cb->setChecked( Options::instance().showCard() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowIllegalDefense( bool checked )
{
    if ( Options::instance().showIllegalDefenseState() != checked )
    {
        Options::instance().toggleShowIllegalDefenseState();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowIllegalDefense()
{
    Options::instance().toggleShowIllegalDefenseState();
    M_illegal_defense_cb->setChecked( Options::instance().showIllegalDefenseState() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickAnonymousMode( bool checked )
{
    if ( Options::instance().anonymousMode() != checked )
    {
        Options::instance().toggleAnonymousMode();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleAnonymousMode()
{
    Options::instance().toggleAnonymousMode();
    M_show_score_board_cb->setChecked( Options::instance().anonymousMode() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowScoreBoard( bool checked )
{
    if ( Options::instance().showScoreBoard() != checked )
    {
        Options::instance().toggleShowScoreBoard();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowScoreBoard()
{
    Options::instance().toggleShowScoreBoard();
    M_show_score_board_cb->setChecked( Options::instance().showScoreBoard() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowTeamGraphic( bool checked )
{
    if ( Options::instance().showTeamGraphic() != checked )
    {
        Options::instance().toggleShowTeamGraphic();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowTeamGraphic()
{
    Options::instance().toggleShowTeamGraphic();
    M_show_team_graphic_cb->setChecked( Options::instance().showTeamGraphic() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowBall( bool checked )
{
    if ( Options::instance().showBall() != checked )
    {
        Options::instance().toggleShowBall();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowBall()
{
    Options::instance().toggleShowBall();
    M_show_ball_cb->setChecked( Options::instance().showBall() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowPlayers( bool checked )
{
    if ( Options::instance().showPlayers() != checked )
    {
        Options::instance().toggleShowPlayers();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowPlayers()
{
    Options::instance().toggleShowPlayers();
    M_show_players_cb->setChecked( Options::instance().showPlayers() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowFlags( bool checked )
{
    if ( Options::instance().showFlags() != checked )
    {
        Options::instance().toggleShowFlags();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowFlags()
{
    Options::instance().toggleShowFlags();
    M_show_flags_cb->setChecked( Options::instance().showFlags() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowOffsideLine( bool checked )
{
    if ( Options::instance().showOffsideLine() != checked )
    {
        Options::instance().toggleShowOffsideLine();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowOffsideLine()
{
    Options::instance().toggleShowOffsideLine();
    M_show_offside_line_cb->setChecked( Options::instance().showOffsideLine() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowDrawData( bool checked )
{
    if ( Options::instance().showDrawData() != checked )
    {
        Options::instance().toggleShowDrawData();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleShowDrawData()
{
    Options::instance().toggleShowDrawData();
    M_show_draw_data_cb->setChecked( Options::instance().showDrawData() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickGrassNormal()
{
    if ( Options::instance().fieldGrassType() != Options::GRASS_NORMAL )
    {
        Options::instance().setFieldGrassType( Options::GRASS_NORMAL );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickGrassLine()
{
    if ( Options::instance().fieldGrassType() != Options::GRASS_LINES )
    {
        Options::instance().setFieldGrassType( Options::GRASS_LINES );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickGrassChecker()
{
    if ( Options::instance().fieldGrassType() != Options::GRASS_CHECKER )
    {
        Options::instance().setFieldGrassType( Options::GRASS_CHECKER );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickKeepawayMode( bool checked )
{
    if ( Options::instance().keepawayMode() != checked )
    {
        Options::instance().toggleKeepawayMode();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowGridCoord( bool checked )
{
    if ( Options::instance().showGridCoord() != checked )
    {
        Options::instance().toggleShowGridCoord();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleKeepawayMode()
{
    Options::instance().toggleKeepawayMode();
    M_keepaway_mode_cb->setChecked( Options::instance().keepawayMode() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickCursorHide( bool checked )
{
    if ( Options::instance().cursorHide() != checked )
    {
        Options::instance().toggleCursorHide();

        //emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickAntiAliasing( bool checked )
{
    if ( Options::instance().antiAliasing() != checked )
    {
        Options::instance().toggleAntiAliasing();

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickGradient( bool checked )
{
    if ( Options::instance().gradient() != checked )
    {
        Options::instance().toggleGradient();

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::slideGridStep( int value )
{
    M_grid_step_text->setText( QString::number( value * 0.1 ) );

    Options::instance().setGridStep( value );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::editGridStep( const QString & text )
{
    bool ok = true;
    double value = text.toDouble( & ok );

    if ( ok )
    {
        Options::instance().setGridStep( static_cast< int >( rint( value * 10.0 ) ) );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::selectDrawType( int index )
{
    Options::PaintStyle style = static_cast< Options::PaintStyle >( index );
    if ( Options::instance().paintStyle() != style )
    {
        Options::instance().setPaintStyle( style );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowVoronoiDiagram( bool checked )
{
    if ( Options::instance().showVoronoiDiagram() != checked )
    {
        Options::instance().toggleShowVoronoiDiagram();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickShowDelaunayTriangulation( bool checked )
{
    if ( Options::instance().showDelaunayTriangulation() != checked )
    {
        Options::instance().toggleShowDelaunayTriangulation();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::selectCompGeomSide( int index )
{
    switch ( index ) {
    case 0:
        if ( Options::instance().voronoiTarget() != rcsc::NEUTRAL )
        {
            Options::instance().setVoronoiTarget( rcsc::NEUTRAL );
            emit configured();
        }
        break;
    case 1:
        if ( Options::instance().voronoiTarget() != rcsc::LEFT )
        {
            Options::instance().setVoronoiTarget( rcsc::LEFT );
            emit configured();
        }
        break;
    case 2:
        if ( Options::instance().voronoiTarget() != rcsc::RIGHT )
        {
            Options::instance().setVoronoiTarget( rcsc::RIGHT );
            emit configured();
        }
        break;
    default:
        std::cerr << __FILE__ << ": (selectCompGeomSide) unsupported index "
                  << index
                  << std::endl;
        break;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickFocusBall()
{
    if ( Options::instance().focusType() != Options::FOCUS_BALL )
    {
        Options::instance().setFocusType( Options::FOCUS_BALL );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleFocusBall()
{
    if ( Options::instance().focusType() == Options::FOCUS_BALL )
    {
        setFocusFix();
    }
    else
    {
        Options::instance().setFocusType( Options::FOCUS_BALL );
        M_focus_ball_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickFocusPlayer()
{
    if ( Options::instance().focusType() != Options::FOCUS_PLAYER )
    {
        Options::instance().setFocusType( Options::FOCUS_PLAYER );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleFocusPlayer()
{
    if ( Options::instance().focusType() == Options::FOCUS_PLAYER )
    {
        setFocusFix();
    }
    else
    {
        Options::instance().setFocusType( Options::FOCUS_PLAYER );
        M_focus_player_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::setFocusFix()
{
    if ( Options::instance().focusType() != Options::FOCUS_POINT )
    {
        Options::instance().setFocusType( Options::FOCUS_POINT );
        M_focus_fix_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::setFocusPoint( const QPoint & point )
{
    Options::instance().setFocusPoint( point.x(), point.y() );
    M_focus_fix_rb->setChecked( true );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickSelectAutoAll()
{
    if ( Options::instance().agentSelectType() != Options::SELECT_AUTO_ALL )
    {
        Options::instance().setAgentSelectType( Options::SELECT_AUTO_ALL );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleSelectAutoAll()
{
    if ( Options::instance().agentSelectType() == Options::SELECT_AUTO_ALL )
    {
        setUnselect();
    }
    else
    {
        Options::instance().setAgentSelectType( Options::SELECT_AUTO_ALL );
        M_select_all_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickSelectAutoLeft()
{
    Options::AgentSelectType type = ( Options::instance().reverseSide()
                                      ? Options::SELECT_AUTO_RIGHT
                                      : Options::SELECT_AUTO_LEFT );

    if ( Options::instance().agentSelectType() != type )
    {
        Options::instance().setAgentSelectType( type );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleSelectAutoLeft()
{
    Options::AgentSelectType type = ( Options::instance().reverseSide()
                                      ? Options::SELECT_AUTO_RIGHT
                                      : Options::SELECT_AUTO_LEFT );

    if ( Options::instance().agentSelectType() == type )
    {
        setUnselect();
    }
    else
    {
        Options::instance().setAgentSelectType( type );
        M_select_left_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickSelectAutoRight()
{
    Options::AgentSelectType type = ( Options::instance().reverseSide()
                                      ? Options::SELECT_AUTO_LEFT
                                      : Options::SELECT_AUTO_RIGHT );

    if ( Options::instance().agentSelectType() != type )
    {
        Options::instance().setAgentSelectType( type );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleSelectAutoRight()
{
    Options::AgentSelectType type = ( Options::instance().reverseSide()
                                      ? Options::SELECT_AUTO_LEFT
                                      : Options::SELECT_AUTO_RIGHT );

    if ( Options::instance().agentSelectType() == type )
    {
        setUnselect();
    }
    else
    {
        Options::instance().setAgentSelectType( type );
        M_select_right_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickSelectFix()
{
    if ( Options::instance().agentSelectType() != Options::SELECT_FIX )
    {
        Options::instance().setAgentSelectType( Options::SELECT_AUTO_RIGHT );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::toggleSelectFix()
{
    if ( Options::instance().agentSelectType() == Options::SELECT_FIX )
    {
        setUnselect();
    }
    else
    {
        Options::instance().setAgentSelectType( Options::SELECT_FIX );
        M_select_fix_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::setUnselect()
{
    Options::instance().unselectAgent();
    M_unselect_rb->setChecked( true );
    M_agent_choice->setCurrentIndex( 0 );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::selectAgent( int number )
{
    // left side:  number = [1,12]
    // right side:  number = [13,24]

    if ( number < 1
         || 24 < number )
    {
        setUnselect();
        return;
    }

    int orig_number = number;

    if ( Options::instance().reverseSide() )
    {
        if ( number <= 12 )
        {
            number += 12;
        }
        else
        {
            number -= 12;
        }
    }

    Options::instance().setAgentSelectType( Options::SELECT_FIX );
    if ( number <= 12 )
    {
        Options::instance().setSelectedNumber( rcsc::LEFT, number );
    }
    else
    {
        Options::instance().setSelectedNumber( rcsc::RIGHT, number - 12 );
    }

    M_agent_choice->setCurrentIndex( orig_number );
    M_select_fix_rb->setChecked( true );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::choiceAgent( int number )
{
    int orig_number = number;
    if ( Options::instance().reverseSide() )
    {
        if ( number <= 12 )
        {
            number += 12;
        }
        else
        {
            number -= 12;
        }
    }

    if ( number <= 12
         && Options::instance().isSelectedAgent( rcsc::LEFT, number ) )
    {
        return;
    }

    if ( number > 12
         && Options::instance().isSelectedAgent( rcsc::RIGHT, number - 12 ) )
    {
        return;
    }

    selectAgent( orig_number );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::selectAgentWithKey()
{
    QAction * action = qobject_cast< QAction * >( sender() );

    if ( ! action )
    {
        return;
    }

    int key = action->shortcut()[0];

    for ( int i = 1; i < 10; ++i )
    {
        if ( key == Qt::Key_0 + i )
        {
            selectAgent( i );
            return;
        }
        else if (
#ifdef Q_WS_MAC
                 key == int( Qt::META + Qt::Key_0 + i )
#else
                 key == int( Qt::CTRL + Qt::Key_0 + i )
#endif
                 )
        {
            selectAgent( i + 12 );
            return;
        }
    }

    if ( key == Qt::Key_0 )
    {
        selectAgent( 10 );
        return;
    }

    if (
#ifdef Q_WS_MAC
        key == Qt::META + Qt::Key_0
#else
        key == Qt::CTRL + Qt::Key_0
#endif
        )
    {
        selectAgent( 10 + 12 );
        return;
    }

    if ( key == Qt::Key_Minus )
    {
        selectAgent( 11 );
        return;
    }

    if (
#ifdef Q_WS_MAC
        key == Qt::META + Qt::Key_Minus
#else
        key == Qt::CTRL + Qt::Key_Minus
#endif
        )
    {
        selectAgent( 11 + 12 );
        return;
    }

    if ( key == Qt::Key_Equal )
    {
        selectAgent( 12 );
        return;
    }

    if (
#ifdef Q_WS_MAC
        key == Qt::META + Qt::Key_Equal
#else
        key == Qt::CTRL + Qt::Key_Equal
#endif
        )
    {
        selectAgent( 12 + 12 );
        return;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::selectLeftCoach()
{
    selectAgent( 12 );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::selectRightCoach()
{
    selectAgent( 12 + 12 );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::editBallTraceStart( const QString & text )
{
    bool ok = true;
    int value = text.toInt( & ok );

    if ( ok )
    {
        Options::instance().setBallTraceStart( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::editBallTraceEnd( const QString & text )
{
    bool ok = true;
    int value = text.toInt( & ok );

    if ( ok )
    {
        Options::instance().setBallTraceEnd( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickBallTraceAll()
{
    const MonitorViewData::Cont & view = M_main_data.viewHolder().monitorViewCont();

    if ( ! view.empty() )
    {
        M_ball_trace_cb->setChecked( false );

        if ( Options::instance().ballTraceStart() == view.front()->time().cycle()
             && Options::instance().ballTraceEnd() == view.back()->time().cycle() )
        {
            M_ball_trace_start->setText( QString::number( 0 ) );
            M_ball_trace_end->setText( QString::number( 0 ) );

            Options::instance().setBallTraceStart( 0 );
            Options::instance().setBallTraceEnd( 0 );
        }
        else
        {
            M_ball_trace_start->setText( QString::number( view.front()->time().cycle() ) );
            M_ball_trace_end->setText( QString::number( view.back()->time().cycle() ) );

            Options::instance().setBallTraceStart( view.front()->time().cycle() );
            Options::instance().setBallTraceEnd( view.back()->time().cycle() );
        }

        if ( Options::instance().ballAutoTrace() )
        {
            Options::instance().toggleBallAutoTrace();
        }

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickAutoBallTrace( bool checked )
{
    if ( Options::instance().ballAutoTrace() != checked )
    {
        Options::instance().toggleBallAutoTrace();

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::editPlayerTraceStart( const QString & text )
{
    bool ok = true;
    int value = text.toInt( & ok );

    if ( ok )
    {
        Options::instance().setPlayerTraceStart( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::editPlayerTraceEnd( const QString & text )
{
    bool ok = true;
    int value = text.toInt( & ok );

    if ( ok )
    {
        Options::instance().setPlayerTraceEnd( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickPlayerTraceAll()
{
    const MonitorViewData::Cont & view = M_main_data.viewHolder().monitorViewCont();

    if ( ! view.empty() )
    {
        M_player_trace_cb->setChecked( false );


        if ( Options::instance().playerTraceStart() == view.front()->time().cycle()
             && Options::instance().playerTraceEnd() == view.back()->time().cycle() )
        {
            M_player_trace_start->setText( QString::number( 0 ) );
            M_player_trace_end->setText( QString::number( 0 ) );

            Options::instance().setPlayerTraceStart( 0 );
            Options::instance().setPlayerTraceEnd( 0 );
        }
        else
        {
            M_player_trace_start->setText( QString::number( view.front()->time().cycle() ) );
            M_player_trace_end->setText( QString::number( view.back()->time().cycle() ) );

            Options::instance().setPlayerTraceStart( view.front()->time().cycle() );
            Options::instance().setPlayerTraceEnd( view.back()->time().cycle() );
        }

        if ( Options::instance().playerAutoTrace() )
        {
            Options::instance().togglePlayerAutoTrace();
        }

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickAutoPlayerTrace( bool checked )
{
    if ( Options::instance().playerAutoTrace() != checked )
    {
        Options::instance().togglePlayerAutoTrace();

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::editAutoTraceStart( const QString & text )
{
    bool ok = true;
    int value = text.toInt( & ok );

    if ( ok )
    {
        Options::instance().setAutoTraceStart( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::changeAutoTracePeriod( int value )
{
    if ( value >= 1 )
    {
        Options::instance().setAutoTracePeriod( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::clickLinePointButton()
{
    Options::instance().toggleLineTrace();

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::changeBallFutureCycle( int value )
{
    Options::instance().setBallFutureCycle( value );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::changePlayerFutureCycle( int value )
{
    Options::instance().setPlayerFutureCycle( value );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::changePlayerMovableCycle( int value )
{
    Options::instance().setPlayerMovableCycle( value );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::selectMouseMeasureMode( int index )
{
    Options::MouseMeasureMode mode = static_cast< Options::MouseMeasureMode >( index );
    if ( Options::instance().mouseMeasureMode() != mode )
    {
        Options::instance().setMouseMeasureMode( mode );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ViewConfigDialog::changeMouseMeasureFirstLength( double value )
{
    if ( std::fabs( Options::instance().mouseMeasureFirstLength() - value ) > 1.0e-5 )
    {
        Options::instance().setMouseMeasureFirstLength( value );
        emit configured();
    }
}
