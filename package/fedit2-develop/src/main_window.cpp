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

#include "command.h"
#include "config_dialog.h"
#include "constraint_edit_dialog.h"
#include "edit_canvas.h"
#include "edit_data.h"
#include "edit_dialog.h"
#include "constraint_view.h"
#include "sample_view.h"
#include "options.h"

#include <rcsc/formation/sample_data.h>

//#include <rcsc/formation/formation_bpn.h>
//#include <rcsc/formation/formation_cdt.h>
#include <rcsc/formation/formation_dt.h>
//#include <rcsc/formation/formation_knn.h>
//#include <rcsc/formation/formation_ngnet.h>
//#include <rcsc/formation/formation_sbsp.h>
//#include <rcsc/formation/formation_rbf.h>
//#include <rcsc/formation/formation_static.h>
//#include <rcsc/formation/formation_uva.h>

#include <iostream>

#include "xpm/fedit2.xpm"
#include "xpm/chase.xpm"
#include "xpm/delete.xpm"
#include "xpm/insert.xpm"
#include "xpm/new.xpm"
#include "xpm/open.xpm"
#include "xpm/record.xpm"
#include "xpm/replace.xpm"
#include "xpm/reverse.xpm"
#include "xpm/save.xpm"
#include "xpm/symmetry.xpm"
#include "xpm/train.xpm"

using namespace rcsc;
using namespace formation;

/*-------------------------------------------------------------------*/
/*!

 */
MainWindow::MainWindow()
{
    qApp->setWindowIcon( QIcon( QPixmap( fedit2_xpm ) ) );
    this->setWindowTitle( tr( "Formation Editor" ) );
    this->setMinimumSize( 280, 220 );
    this->resize( 640, 480 );

    readSettings();

    createEditCanvas();
    createEditDialog();
    createSampleView();
    createConstraintView();

    M_config_dialog = new ConfigDialog( this );
    M_config_dialog->setVisible( false );

    QToolBox * tool_box = createToolBox();
    tool_box->addItem( M_sample_view, tr( "Samples" ) );
    tool_box->addItem( M_constraint_view, tr( "Constraints" ) );

    M_splitter = new QSplitter( this );
    M_splitter->addWidget( tool_box );
    M_splitter->addWidget( M_edit_canvas );
    M_splitter->setStretchFactor( 0, 0 );
    M_splitter->setStretchFactor( 1, 1 );
    this->setCentralWidget( M_splitter );
    tool_box->resize( 50, tool_box->height() );

    //
    createUndoStack();

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    connect( M_edit_dialog, SIGNAL( viewUpdated() ),
             M_edit_canvas, SLOT( update() ) );
    connect( M_edit_canvas, SIGNAL( objectMoved() ),
             M_edit_dialog, SLOT( updateData() ) );
    connect( M_edit_canvas, SIGNAL( objectMoved() ),
             this, SLOT( updateDataIndex() ) );

    connect( M_config_dialog, SIGNAL( viewResizeApplied( const QSize & ) ),
             this, SLOT( resizeView( const QSize & ) ) );
    connect( M_config_dialog, SIGNAL( configured() ),
             M_edit_canvas, SLOT( update() ) );

    connect( M_undo_stack, SIGNAL( canUndoChanged( bool ) ),
             M_undo_act, SLOT( setEnabled( bool ) ) );
    connect( M_undo_stack, SIGNAL( canRedoChanged( bool ) ),
             M_redo_act, SLOT( setEnabled( bool ) ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
MainWindow::~MainWindow()
{
    if ( QApplication::overrideCursor() )
    {
        QApplication::restoreOverrideCursor();
    }

    writeSettings();

    //std::cerr << "delete MainWindow." << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::init()
{
    const Options & opt = Options::instance();

    if ( opt.fullScreen() )
    {
        this->showFullScreen();
    }
    else if ( opt.maximize() )
    {
        this->showMaximized();
    }
    else
    {
        if ( opt.windowWidth() > 0 && opt.windowHeight() > 0 )
        {
            this->resize( opt.windowWidth(), opt.windowHeight() );
        }

        if ( opt.windowPosX() > 0 && opt.windowPosY() > 0 )
        {
            this->move( opt.windowPosX(),  opt.windowPosY() );
        }
    }


    if ( ! opt.confFile().isEmpty() )
    {
        openConfFile( opt.confFile() );
    }

    if ( ! opt.dataFile().isEmpty() )
    {
        openDataFile( opt.dataFile() );
    }

    if ( ! opt.backgroundConfFile().isEmpty() )
    {
        openBackgroundConfFile( opt.backgroundConfFile() );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::readSettings()
{
#ifdef Q_WS_WIN
    QSettings settings( QDir::currentPath() + tr( "/fedit2.ini" ),
                        QSettings::IniFormat );
#else
    QSettings settings( QDir::homePath() + tr( "/.fedit2" ),
                        QSettings::IniFormat );
#endif

    settings.beginGroup( "MainWindow" );


    settings.endGroup();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::writeSettings()
{
#ifdef Q_WS_WIN
    QSettings settings( QDir::currentPath() + QObject::tr( "/fedit2.ini" ),
                        QSettings::IniFormat );
#else
    QSettings settings( QDir::homePath() + QObject::tr( "/.fedit2" ),
                        QSettings::IniFormat );
#endif

    settings.beginGroup( "MainWindow" );


    settings.endGroup();
}



/*-------------------------------------------------------------------*/
/*!

 */
QToolBox *
MainWindow::createToolBox()
{
    QToolBox * tool_box = new QToolBox();

    //     tool_box->addItem( createDataTreeWidget(), tr( "Data Tree" ) );
    //     tool_box->addItem( createTransformSettingWidget(), tr( "Transform" ) );
    //     tool_box->addItem( createZLayerSettingWidget(), tr( "Z Layer" ) );
    //     tool_box->addItem( createDarumaSettingWidget(), tr( "DaRuMa Setting" ) );

    tool_box->setMaximumWidth( 512 );
    tool_box->setCurrentIndex( 1 );

    return tool_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createEditCanvas()
{
    M_edit_canvas = new EditCanvas();

    //     connect( this, SIGNAL( viewUpdated() ),
    //              M_edit_canvas, SLOT( update() ) );

    connect( M_edit_canvas, SIGNAL( mouseMoved( const QPointF & ) ),
             this, SLOT( setPositionLabel( const QPointF & ) ) );
    connect( M_edit_canvas, SIGNAL( constraintSelected( int, int ) ),
             this, SLOT( showConstraintEditDialog( int, int ) ) );
    connect( this, SIGNAL( viewUpdated() ),
             M_edit_canvas, SLOT( update() ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createEditDialog()
{
    M_edit_dialog = new EditDialog( this );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createSampleView()
{
    M_sample_view = new SampleView( this );

    connect( M_sample_view, SIGNAL( sampleSelected( int ) ),
             this, SLOT( selectSample( int ) ) );
    connect( M_sample_view, SIGNAL( sampleIndexChangeRequested( int, int ) ),
             this, SLOT( changeSampleIndex( int, int ) ) );
    connect( M_sample_view, SIGNAL( sampleDeleteRequested( int ) ),
             this, SLOT( deleteSample( int ) ) );
    connect( M_sample_view, SIGNAL( ballReplaced( int, double, double ) ),
             this, SLOT( replaceBall( int, double, double ) ) );
    connect( M_sample_view, SIGNAL( playerReplaced( int, int, double, double ) ),
             this, SLOT( replacePlayer( int, int, double, double ) ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createConstraintView()
{
    M_constraint_view = new ConstraintView( this );

    connect( M_constraint_view, SIGNAL( constraintDeleteRequested( int, int ) ),
             this, SLOT( deleteConstraint( int, int ) ) );
    connect( M_constraint_view, SIGNAL( constraintReplaced( int, int, int ) ),
             this, SLOT( replaceConstraint( int, int, int ) ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createUndoStack()
{
    M_undo_stack = new QUndoStack( this );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createToolBars()
{
    {
        M_tool_bar = new QToolBar( tr( "Edit tools" ), this );
        //M_tool_bar->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

        M_tool_bar->addAction( M_save_act );
        M_tool_bar->addAction( M_toggle_player_auto_move_act );
        M_tool_bar->addAction( M_toggle_symmetry_mode_act );
        //M_tool_bar->addAction( M_toggle_constraint_edit_mode_act );

        M_tool_bar->addSeparator();

        M_tool_bar->addAction( M_delete_data_act );
        M_tool_bar->addAction( M_reverse_y_act );

        //

        M_tool_bar->addSeparator();

        M_tool_bar->addAction( M_train_act );
        M_tool_bar->addAction( M_replace_data_act );
        M_tool_bar->addAction( M_insert_data_act );
        M_tool_bar->addAction( M_add_data_act );

        //
        {
            QFrame * dummy_frame = new QFrame();
            QHBoxLayout * layout = new QHBoxLayout();
            layout->addStretch( 1 );
            dummy_frame->setLayout( layout );
            M_tool_bar->addWidget( dummy_frame );
        }

        //
        M_index_spin_box = new QSpinBox( );
        //M_index_spin_box->setPrefix( tr( "Index: " ) );
        M_index_spin_box->setRange( 0, 0 );
        M_index_spin_box->setWrapping( true );
        M_index_spin_box->setMaximumSize( 80, 24 );
        connect( M_index_spin_box, SIGNAL( valueChanged( int ) ),
                 this, SLOT( selectSampleVisualIndex( int ) ) );
        M_tool_bar->addWidget( M_index_spin_box );


        this->addToolBar( Qt::TopToolBarArea, M_tool_bar );
    }

}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createStatusBar()
{
    this->statusBar()->showMessage( tr( "Ready" ) );

    M_position_label = new QLabel( tr( "(0.00, 0.00)" ) );

    int min_width
        = M_position_label->fontMetrics().width(  tr( "(-000.00, -000.00)" ) )
        + 8;
    M_position_label->setMinimumWidth( min_width );
    M_position_label->setAlignment( Qt::AlignRight );

    this->statusBar()->addPermanentWidget( M_position_label );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createActions()
{
    createActionsFile();
    createActionsEdit();
    createActionsView();
    createActionsHelp();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createActionsFile()
{
    M_new_file_act = new QAction( QIcon( QPixmap( new_xpm ) ),
                                  tr( "New formation" ),
                                  this );
#ifdef Q_WS_MAC
    M_new_file_act->setShortcut( Qt::META + Qt::Key_N );
#else
    M_new_file_act->setShortcut( Qt::CTRL + Qt::Key_N );
#endif
    M_new_file_act->setStatusTip( tr( "Create new formation data. (" )
                                  + M_new_file_act->shortcut().toString()
                                  + tr( ")" ) );
    connect( M_new_file_act, SIGNAL( triggered() ), this, SLOT( newFile() ) );

    //
    M_open_conf_act = new QAction( QIcon( QPixmap( open_xpm ) ),
                                   tr( "&Open formation" ),
                                   this );
#ifdef Q_WS_MAC
    M_open_conf_act->setShortcut( Qt::META + Qt::Key_O );
#else
    M_open_conf_act->setShortcut( Qt::CTRL + Qt::Key_O );
#endif
    M_open_conf_act->setStatusTip( tr( "Open formation file. (" )
                                   + M_open_conf_act->shortcut().toString()
                                   + tr( ")" ) );
    connect( M_open_conf_act, SIGNAL( triggered() ), this, SLOT( openConf() ) );

    //
    M_open_background_conf_act = new QAction( QIcon( QPixmap( open_xpm ) ),
                                              tr( "&Open background formation" ),
                                              this );
#ifdef Q_WS_MAC
    M_open_background_conf_act->setShortcut( Qt::META + Qt::SHIFT + Qt::Key_O );
#else
    M_open_background_conf_act->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_O );
#endif
    M_open_background_conf_act->setStatusTip( tr( "Open background formation file. (" )
                                              + M_open_background_conf_act->shortcut().toString()
                                              + tr( ")" ) );
    connect( M_open_background_conf_act, SIGNAL( triggered() ),
             this, SLOT( openBackgroundConf() ) );

    //
    M_open_data_act = new QAction( QIcon( QPixmap( open_xpm ) ),
                                   tr( "&Open data file." ),
                                   this );
#ifdef Q_WS_MAC
    M_open_data_act->setShortcut( Qt::META + Qt::ALT + Qt::Key_O );
#else
    M_open_data_act->setShortcut( Qt::CTRL + Qt::ALT + Qt::Key_O );
#endif
    M_open_data_act->setStatusTip( tr( "Open data file. (" )
                                   + M_open_data_act->shortcut().toString()
                                   + tr( ")" ) );
    connect( M_open_data_act, SIGNAL( triggered() ), this, SLOT( openData() ) );

    //
    M_save_act = new QAction( QIcon( QPixmap( save_xpm ) ),
                              tr( "&Save formation" ),
                              this );
#ifdef Q_WS_MAC
    M_save_act->setShortcut( Qt::META + Qt::Key_S );
#else
    M_save_act->setShortcut( Qt::CTRL + Qt::Key_S );
#endif
    M_save_act->setStatusTip( tr( "Save formation data" ) );
    connect( M_save_act, SIGNAL( triggered() ), this, SLOT( saveConf() ) );

    //
    M_save_as_act = new QAction( tr( "&Save formation as..." ),
                                 this );
    M_save_as_act->setStatusTip( tr( "Save formation data to the new file" ) );
    connect( M_save_as_act, SIGNAL( triggered() ), this, SLOT( saveConfAs() ) );

    //
    M_save_data_as_act = new QAction( tr( "&Save training data as..." ),
                                      this );
    M_save_data_as_act->setStatusTip( tr( "Save training data to the new file" ) );
    connect( M_save_data_as_act, SIGNAL( triggered() ), this, SLOT( saveDataAs() ) );

    //
    M_quit_act = new QAction( tr( "&Quit" ),
                              this );
#ifdef Q_WS_MAC
    M_quit_act->setShortcut( Qt::META + Qt::Key_Q );
#else
    M_quit_act->setShortcut( Qt::CTRL + Qt::Key_Q );
#endif
    M_quit_act->setStatusTip( tr( "Exit application.(" )
                              + M_quit_act->shortcut().toString()
                              + tr( ")" ) );
    connect( M_quit_act, SIGNAL( triggered() ),
             this, SLOT( close() ) );
    this->addAction( M_quit_act );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createActionsEdit()
{
    //
    M_undo_act = new QAction( tr( "&Undo" ), this );
#ifdef Q_WS_MAC
    M_undo_act->setShortcut( Qt::META + Qt::Key_Z );
#else
    M_undo_act->setShortcut( Qt::CTRL + Qt::Key_Z );
#endif
    M_undo_act->setEnabled( false );
    connect( M_undo_act, SIGNAL( triggered() ),
             M_undo_stack, SLOT( undo() ) );
    this->addAction( M_undo_act );

    //
    M_redo_act = new QAction( tr( "&Redo" ), this );
#ifdef Q_WS_MAC
    M_redo_act->setShortcut( Qt::META + Qt::Key_Y );
#else
    M_redo_act->setShortcut( Qt::CTRL + Qt::Key_Y );
#endif
    M_redo_act->setEnabled( false );
    connect( M_redo_act, SIGNAL( triggered() ),
             M_undo_stack, SLOT( redo() ) );
    this->addAction( M_redo_act );

    //
    M_toggle_player_auto_move_act = new QAction( QIcon( QPixmap( chase_xpm ) ),
                                                 tr( "&Auto Move" ),
                                                 this );
    M_toggle_player_auto_move_act->setShortcut( Qt::Key_A );
    M_toggle_player_auto_move_act->setToolTip( tr( "Toggle player auto move mode" ) );
    M_toggle_player_auto_move_act->setStatusTip( tr( "Toggle player auto move mode. (" )
                                                 + M_toggle_player_auto_move_act->shortcut().toString()
                                                 + tr( ")" ) );
    connect( M_toggle_player_auto_move_act, SIGNAL( toggled( bool ) ),
             this, SLOT( setPlayerAutoMove( bool ) ) );
    M_toggle_player_auto_move_act->setCheckable( true );
    M_toggle_player_auto_move_act->setChecked( true );
    this->addAction( M_toggle_player_auto_move_act );

    //
    M_toggle_data_auto_select_act = new QAction( tr( "&Data Auto Select" ),
                                                 this );
    M_toggle_data_auto_select_act->setShortcut( Qt::Key_D );
    M_toggle_data_auto_select_act->setStatusTip( tr( "Toggle data is automatically select or not when ball is moved. (" )
                                                 + M_toggle_data_auto_select_act->shortcut().toString()
                                                 + tr( ")" ) );
    connect( M_toggle_data_auto_select_act, SIGNAL( toggled( bool ) ),
             this, SLOT( setDataAutoSelect( bool ) ) );
    M_toggle_data_auto_select_act->setCheckable( true );
    M_toggle_data_auto_select_act->setChecked( true );
    this->addAction( M_toggle_data_auto_select_act );

    //
    M_toggle_symmetry_mode_act = new QAction( QIcon( QPixmap( symmetry_xpm ) ),
                                              tr( "Symmetry" ),
                                              this );
    M_toggle_symmetry_mode_act->setShortcut( Qt::Key_M );
    M_toggle_symmetry_mode_act->setToolTip( tr( "Toggle symmetry mode" ) );
    M_toggle_symmetry_mode_act->setStatusTip( tr( "Toggle symmetry mode." )
                                              + M_toggle_symmetry_mode_act->shortcut().toString()
                                              + tr( ")" ) );
    connect( M_toggle_symmetry_mode_act, SIGNAL( toggled( bool ) ),
             this, SLOT( setSymmetryMode( bool ) ) );
    M_toggle_symmetry_mode_act->setCheckable( true );
    M_toggle_symmetry_mode_act->setChecked( true );
    this->addAction( M_toggle_symmetry_mode_act );

    //
    M_toggle_constraint_edit_mode_act = new QAction( tr( "Edit Constraint" ),
                                                     this );
    //M_toggle_constraint_edit_mode_act->setShortcut( Qt::Key_C );
    M_toggle_constraint_edit_mode_act->setToolTip( tr( "Toggle constraint edge edit mode" ) );
    M_toggle_constraint_edit_mode_act->setStatusTip( tr( "Toggle constraint edge edit mode." )
                                                     + M_toggle_constraint_edit_mode_act->shortcut().toString()
                                                     + tr( ")" ) );
    connect( M_toggle_constraint_edit_mode_act, SIGNAL( toggled( bool ) ),
             this, SLOT( setConstraintEditMode( bool ) ) );
    M_toggle_constraint_edit_mode_act->setCheckable( true );
    M_toggle_constraint_edit_mode_act->setChecked( false );
    this->addAction( M_toggle_constraint_edit_mode_act );

    //
    M_add_data_act = new QAction( QIcon( QPixmap( record_xpm ) ),
                                  tr( "Add Data" ),
                                  this );
#ifdef Q_WS_MAC
    M_add_data_act->setShortcut( Qt::META + Qt::Key_R );
#else
    M_add_data_act->setShortcut( Qt::CTRL + Qt::Key_R );
#endif
    M_add_data_act->setStatusTip( tr( "Add current field status as a training data. (" )
                                  + M_add_data_act->shortcut().toString()
                                  + tr( ")" ) );
    connect( M_add_data_act, SIGNAL( triggered() ), this, SLOT( addData() ) );
    this->addAction( M_add_data_act );

    //
    M_insert_data_act = new QAction( QIcon( QPixmap( insert_xpm ) ),
                                     tr( "Insert" ),
                                     this );
    M_insert_data_act->setStatusTip( tr( "Insert the screen data after the current index" ) );
    connect( M_insert_data_act, SIGNAL( triggered() ), this, SLOT( insertData() ) );
    this->addAction( M_insert_data_act );

    //
    M_replace_data_act = new QAction( QIcon( QPixmap( replace_xpm ) ),
                                      tr( "Replace" ),
                                      this );
    M_replace_data_act->setStatusTip( tr( "Replace a current indexed training"
                                          " data by the screen status" ) );
    connect( M_replace_data_act, SIGNAL( triggered() ), this, SLOT( replaceData() ) );
    this->addAction( M_replace_data_act );

    //
    M_delete_data_act = new QAction( QIcon( QPixmap( delete_xpm ) ),
                                     tr( "Delete" ),
                                     this );
    M_delete_data_act->setShortcut( Qt::Key_Delete );
    M_delete_data_act->setStatusTip( tr( "Delete a current indexed training data. (" )
                                     + M_delete_data_act->shortcut().toString()
                                     + tr( ")" ) );
    connect( M_delete_data_act, SIGNAL( triggered() ), this, SLOT( deleteData() ) );
    this->addAction( M_delete_data_act );

    //
    M_reverse_y_act = new QAction( QIcon( QPixmap( reverse_xpm ) ),
                                   tr( "ReverseY" ),
                                   this );
    M_reverse_y_act->setShortcut( Qt::Key_R );
    M_reverse_y_act->setStatusTip( tr( "Reverse Y positions. (" )
                                   + M_reverse_y_act->shortcut().toString()
                                   + tr( ")" ) );
    connect( M_reverse_y_act, SIGNAL( triggered() ), this, SLOT( reverseY() ) );
    this->addAction( M_reverse_y_act );

    //
    M_add_constraint_act = new QAction( tr( "Add constraint" ),
                                        this );
#ifdef Q_WS_MAC
    M_add_constraint_act->setShortcut( Qt::META + Qt::Key_C );
#else
    M_add_constraint_act->setShortcut( Qt::CTRL + Qt::Key_C );
#endif
    M_add_constraint_act->setStatusTip( tr( "Add constraint. (" )
                                        + M_add_constraint_act->shortcut().toString()
                                        + tr( ")" ) );
    connect( M_add_constraint_act, SIGNAL( triggered() ),
             this, SLOT( showConstraintEditDialog() ) );
    this->addAction( M_add_constraint_act );

    //
    M_train_act = new QAction( QIcon( QPixmap( train_xpm ) ),
                               tr( "Train" ),
                               this );
    M_train_act->setStatusTip( tr( "Train formation using current trainig data set." ) );
    connect( M_train_act, SIGNAL( triggered() ), this, SLOT( train() ) );
    this->addAction( M_train_act );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createActionsView()
{
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
        QAction * act = new QAction( tr( "Full Screen" ), this );
        act->setShortcut( Qt::ALT + Qt::Key_Return );
        this->addAction( act );
        connect( act, SIGNAL( triggered() ),
                 this, SLOT( toggleFullScreen() ) );
    }
    {
        QAction * act = new QAction( tr( "Full Screen" ), this );
        act->setShortcut( Qt::ALT + Qt::Key_Enter );
        this->addAction( act );
        connect( act, SIGNAL( triggered() ),
                 this, SLOT( toggleFullScreen() ) );
    }

    //
    M_toggle_tool_bar_act = new QAction( tr( "Tool Bar" ), this );
    M_toggle_tool_bar_act->setStatusTip( tr( "Show/Hide tool bar." ) );
    M_toggle_tool_bar_act->setCheckable( true );
    M_toggle_tool_bar_act->setChecked( true );
    connect( M_toggle_tool_bar_act, SIGNAL( triggered() ),
             this, SLOT( toggleToolBar() ) );
    this->addAction( M_toggle_tool_bar_act );

    //
    M_toggle_status_bar_act = new QAction( tr( "Status Bar" ), this );
    M_toggle_status_bar_act->setStatusTip( tr( "Show/Hide Status bar." ) );
    M_toggle_status_bar_act->setCheckable( true );
    M_toggle_status_bar_act->setChecked( true );
    connect( M_toggle_status_bar_act, SIGNAL( triggered() ),
             this, SLOT( toggleStatusBar() ) );
    this->addAction( M_toggle_status_bar_act );

    //
    M_zoom_in_act = new QAction( tr( "Zoom In" ),
                                 this );
    //M_zoom_in_act->setShortcut( Qt::Key_Plus );
    M_zoom_in_act->setShortcut( Qt::Key_Z );
    M_zoom_in_act->setStatusTip( tr( "Zoom in (" )
                                 + M_zoom_in_act->shortcut().toString()
                                 + tr( ")" ) );
    connect( M_zoom_in_act, SIGNAL( triggered() ),
             M_edit_canvas, SLOT( zoomIn() ) );
    this->addAction( M_zoom_in_act );

    //
    M_zoom_out_act = new QAction( tr( "Zoom Out" ),
                                  this );
    //M_zoom_out_act->setShortcut( Qt::Key_Minus );
    M_zoom_out_act->setShortcut( Qt::Key_X );
    M_zoom_out_act->setStatusTip( tr( "Zoom out (" )
                                  + M_zoom_out_act->shortcut().toString()
                                  + tr( ")" ) );
    connect( M_zoom_out_act, SIGNAL( triggered() ),
             M_edit_canvas, SLOT( zoomOut() ) );
    this->addAction( M_zoom_out_act );

    //
    M_fit_to_screen_act = new QAction( tr( "Fit to screen" ),
                                       this );
    M_fit_to_screen_act->setShortcut( Qt::Key_I );
    M_fit_to_screen_act->setStatusTip( tr( "Reset zoom ratio and focus the origin (" )
                                       + M_fit_to_screen_act->shortcut().toString()
                                       + tr( ")" ) );
    connect( M_fit_to_screen_act, SIGNAL( triggered() ),
             M_edit_canvas, SLOT( fitToScreen() ) );
    this->addAction( M_fit_to_screen_act );

    //
    M_toggle_enlarge_act = new QAction( tr( "Enlarge Mode" ),
                                        this );
    M_toggle_enlarge_act->setShortcut( Qt::Key_E );
    M_toggle_enlarge_act->setStatusTip( tr( "Toggle enlarge mode (" )
                                        + M_toggle_enlarge_act->shortcut().toString()
                                        + tr( ")" ) );
    connect( M_toggle_enlarge_act, SIGNAL( toggled( bool ) ),
             this, SLOT( setEnlargeMode( bool ) ) );
    M_toggle_enlarge_act->setCheckable( true );
    M_toggle_enlarge_act->setChecked( true );
    this->addAction( M_toggle_enlarge_act );

    //
    M_toggle_show_index_act = new QAction( tr( "Show Index" ),
                                           this );
    M_toggle_show_index_act->setStatusTip( tr( "Toggle index painting." ) );
    connect( M_toggle_show_index_act, SIGNAL( toggled( bool ) ),
             this, SLOT( setShowIndex( bool ) ) );
    M_toggle_show_index_act->setCheckable( true );
    M_toggle_show_index_act->setChecked( true );
    this->addAction( M_toggle_show_index_act );

    //
    M_toggle_show_triangulation_act = new QAction( tr( "Show Triangulation" ),
                                                   this );
    M_toggle_show_triangulation_act->setStatusTip( tr( "Toggle triangulation painting." ) );
    connect( M_toggle_show_triangulation_act, SIGNAL( toggled( bool ) ),
             this, SLOT( setShowTriangulation( bool ) ) );
    M_toggle_show_triangulation_act->setCheckable( true );
    M_toggle_show_triangulation_act->setChecked( true );
    this->addAction( M_toggle_show_triangulation_act );

    //
    M_toggle_show_circumcircle_act = new QAction( tr( "Show Circumcircle" ),
                                                  this );
    M_toggle_show_circumcircle_act->setStatusTip( tr( "Toggle circumcircle painting." ) );
    connect( M_toggle_show_circumcircle_act, SIGNAL( toggled( bool ) ),
             this, SLOT( setShowCircumcircle( bool ) ) );
    M_toggle_show_circumcircle_act->setCheckable( true );
    M_toggle_show_circumcircle_act->setChecked( false );
    this->addAction( M_toggle_show_circumcircle_act );

    //
    M_toggle_show_shoot_lines_act = new QAction( tr( "Show Shoot Lines" ), this );
    M_toggle_show_shoot_lines_act->setStatusTip( tr( "Toggle shoot lines painting." ) );
    M_toggle_show_shoot_lines_act->setShortcut( Qt::Key_S );
    connect( M_toggle_show_shoot_lines_act, SIGNAL( toggled( bool ) ),
             this, SLOT( setShowShootLines( bool ) ) );
    M_toggle_show_shoot_lines_act->setCheckable( true );
    M_toggle_show_shoot_lines_act->setChecked( false );
    this->addAction( M_toggle_show_shoot_lines_act );

    //
    M_toggle_show_goalie_movable_area_act = new QAction( tr( "Show Goalie Movalble Area" ), this );
    M_toggle_show_goalie_movable_area_act->setStatusTip( tr( "Toggle goalie reachable area painting." ) );
    M_toggle_show_goalie_movable_area_act->setShortcut( Qt::Key_G );
    connect( M_toggle_show_goalie_movable_area_act, SIGNAL( toggled( bool ) ),
             this, SLOT( setShowGoalieMovableArea( bool ) ) );
    M_toggle_show_goalie_movable_area_act->setCheckable( true );
    M_toggle_show_goalie_movable_area_act->setChecked( false );
    this->addAction( M_toggle_show_goalie_movable_area_act );

    //
    M_toggle_antialiasing_act = new QAction( tr( "Antialiasing" ),
                                             this );
    M_toggle_antialiasing_act->setStatusTip( tr( "Toggle antialiasing" ) );
    connect( M_toggle_antialiasing_act, SIGNAL( toggled( bool ) ),
             this, SLOT( setAntialiasing( bool ) ) );
    M_toggle_antialiasing_act->setCheckable( true );
    M_toggle_antialiasing_act->setChecked( Options::instance().antialiasing() );
    this->addAction( M_toggle_antialiasing_act );

    //
    M_toggle_show_background_data_act = new QAction( tr( "Background Data" ),
                                                     this );
    M_toggle_show_background_data_act->setStatusTip( tr( "Show/Hide Background Data" ) );
    connect( M_toggle_show_background_data_act, SIGNAL( toggled( bool ) ),
             this, SLOT( toggleShowBackgroundData( bool ) ) );
    M_toggle_show_background_data_act->setCheckable( true );
    M_toggle_show_background_data_act->setChecked( true );
    this->addAction( M_toggle_show_background_data_act );

    //
    M_show_edit_dialog_act = new QAction( tr( "Show Edit Dialog" ),
                                          this );
    connect( M_show_edit_dialog_act, SIGNAL( toggled( bool ) ),
             M_edit_dialog, SLOT( setVisible( bool ) ) );
    connect( M_edit_dialog, SIGNAL( shown( bool ) ),
             M_show_edit_dialog_act, SLOT( setChecked( bool ) ) );
#ifdef Q_WS_MAC
    M_show_edit_dialog_act->setShortcut( Qt::META + Qt::Key_D );
#else
    M_show_edit_dialog_act->setShortcut( Qt::CTRL + Qt::Key_D );
#endif
    M_show_edit_dialog_act->setStatusTip( tr( "Show/Hide edit dialog. (" )
                                          + M_show_edit_dialog_act->shortcut().toString()
                                          + tr( ")" ) );
    M_show_edit_dialog_act->setToolTip( tr( "Show/Hide edit dialog. (" )
                                        + M_show_edit_dialog_act->shortcut().toString()
                                        + tr( ")" ) );
    M_show_edit_dialog_act->setCheckable( true );
    M_show_edit_dialog_act->setChecked( false );
    this->addAction( M_show_edit_dialog_act );

    //
    M_show_config_dialog_act = new QAction( tr( "Show Config Dialog" ),
                                            this );
    connect( M_show_config_dialog_act, SIGNAL( toggled( bool ) ),
             M_config_dialog, SLOT( setVisible( bool ) ) );
    connect( M_config_dialog, SIGNAL( shown( bool ) ),
             M_show_config_dialog_act, SLOT( setChecked( bool ) ) );
#ifdef Q_WS_MAC
    M_show_config_dialog_act->setShortcut( Qt::META + Qt::Key_V );
#else
    M_show_config_dialog_act->setShortcut( Qt::CTRL + Qt::Key_V );
#endif
    M_show_config_dialog_act->setStatusTip( tr( "Show/Hide config dialog. (" )
                                            + M_show_config_dialog_act->shortcut().toString()
                                            + tr( ")" ) );
    M_show_config_dialog_act->setToolTip( tr( "Show/Hide edit dialog.(" )
                                          + M_show_config_dialog_act->shortcut().toString()
                                          + tr( ")" ) );
    M_show_config_dialog_act->setCheckable( true );
    M_show_config_dialog_act->setChecked( false );
    this->addAction( M_show_config_dialog_act );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createActionsHelp()
{
    M_about_act = new QAction( tr( "&About" ),
                               this );
    M_about_act->setStatusTip( tr( "About this application" ) );
    connect( M_about_act, SIGNAL( triggered() ),
             this, SLOT( about() ) );
    this->addAction( M_about_act );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createMenus()
{
    createMenuFile();
    createMenuEdit();
    createMenuView();
    createMenuHelp();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createMenuFile()
{
    QMenu * menu = menuBar()->addMenu( tr( "&File" ) );

    menu->addAction( M_new_file_act );

    menu->addSeparator();

    menu->addAction( M_open_conf_act );
    menu->addAction( M_open_data_act );

    menu->addSeparator();

    menu->addAction( M_save_act );
    menu->addAction( M_save_as_act );

    menu->addSeparator();

    menu->addAction( M_save_data_as_act );

    menu->addSeparator();

    menu->addAction( M_quit_act );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createMenuEdit()
{
    QMenu * menu = menuBar()->addMenu( tr( "&Edit" ) );
    connect( menu, SIGNAL( aboutToShow() ),
             this, SLOT( editMenuAboutToShow() ) );

    // menu->addAction( M_undo_act );
    // menu->addAction( M_redo_act );

    // menu->addSeparator();

    menu->addAction( M_toggle_player_auto_move_act );
    menu->addAction( M_toggle_data_auto_select_act );
    menu->addAction( M_toggle_symmetry_mode_act );
    menu->addAction( M_toggle_constraint_edit_mode_act );

    menu->addSeparator();

    {
        QMenu * submenu = menu->addMenu( tr( "Data" ) );
        submenu->addAction( M_add_data_act );
        submenu->addAction( M_insert_data_act );
        submenu->addAction( M_replace_data_act );
        submenu->addAction( M_delete_data_act );
        submenu->addAction( M_train_act );

        menu->addMenu( submenu );
    }

    menu->addAction( M_reverse_y_act );
    menu->addAction( M_add_constraint_act );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createMenuView()
{
    QMenu * menu = menuBar()->addMenu( tr( "&View" ) );

    menu->addAction( M_full_screen_act );
    menu->addAction( M_toggle_tool_bar_act );
    menu->addAction( M_toggle_status_bar_act );
    menu->addAction( M_show_config_dialog_act );

    menu->addSeparator();

    menu->addAction( M_zoom_in_act );
    menu->addAction( M_zoom_out_act );
    menu->addAction( M_fit_to_screen_act );

    menu->addSeparator();

    menu->addAction( M_toggle_enlarge_act );
    menu->addAction( M_toggle_show_index_act );
    menu->addAction( M_toggle_show_shoot_lines_act );
    menu->addAction( M_toggle_show_goalie_movable_area_act );
    menu->addAction( M_toggle_show_triangulation_act );
    menu->addAction( M_toggle_show_circumcircle_act );
    menu->addAction( M_toggle_antialiasing_act );
    menu->addAction( M_toggle_show_background_data_act );

    menu->addSeparator();

    menu->addAction( M_show_edit_dialog_act );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::createMenuHelp()
{
    QMenu * menu = menuBar()->addMenu( tr( "&Help" ) );

    menu->addAction( M_about_act );
    menu->addAction( tr( "About Qt" ), qApp, SLOT( aboutQt() ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::closeEvent( QCloseEvent * event )
{
    if ( ! saveChanges() )
    {
        event->ignore();
        return;
    }

    event->ignore();
    qApp->quit();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::wheelEvent( QWheelEvent * event )
{
    //
    // set window opacity
    //
    if ( event->modifiers() == Qt::ControlModifier )
    {
        if ( event->delta() < 0 )
        {
            this->setWindowOpacity( std::max( 0.1, this->windowOpacity() - 0.05 ) );
        }
        else
        {
            this->setWindowOpacity( std::min( 1.0, this->windowOpacity() + 0.05 ) );
        }

        event->accept();
        return;
    }

    //
    // no data
    //
    if ( M_index_spin_box->maximum() <= 0 )
    {
        event->accept();
        return;
    }

    //
    // change the selection data
    //
    int idx = M_index_spin_box->value();

    if ( event->delta() > 0 )
    {
        --idx;
        if ( idx < 1 )
        {
            idx = M_index_spin_box->maximum();
        }

        M_index_spin_box->setValue( idx );
    }
    else
    {
        ++idx;
        if ( idx > M_index_spin_box->maximum() )
        {
            idx = 0;
        }

        M_index_spin_box->setValue( idx );
    }

    event->accept();
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
MainWindow::saveChanges()
{
    if ( ! M_edit_data )
    {
        return true;
    }

    if ( ! M_edit_data->isConfChanged() )
    {
        return true;
    }

    QMessageBox::StandardButton result
        = QMessageBox::question( this,
                                 tr( "Save Notify" ),
                                 tr( "Data is changed.\nSave?" ),
                                 QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::No,
                                 QMessageBox::Yes );
    if ( result == QMessageBox::Cancel )
    {
        return false;
    }

    if ( result == QMessageBox::Yes )
    {
        if ( M_edit_data->isConfChanged() )
        {
            std::cerr << "(MainWindow::saveChanges)" << std::endl;
            saveConf();
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
MainWindow::openConfFile( const QString & filepath )
{
    std::cerr << "(MainWindow::openConfFile) " << filepath.toStdString()
              << std::endl;

    if ( filepath.isEmpty() )
    {
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "Empty file path." ),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }

    QFileInfo fileinfo( filepath );

    if ( ! fileinfo.exists()
         || ! fileinfo.isReadable() )
    {
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "No such a file or not readable. \n" ) + filepath,
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }

    M_edit_data = boost::shared_ptr< EditData >( new EditData );
    if ( ! M_edit_data->openConf( filepath ) )
    {
        M_edit_data.reset();
        QMessageBox::warning( this,
                              tr( "Error" ),
                              tr( "Failed to open the file. \n" ) + filepath,
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }

    if ( ! M_edit_data->formation() )
    {
        M_edit_data.reset();
        QMessageBox::warning( this,
                              tr( "Error" ),
                              tr( "Failed to create a formation. \n" ) + filepath,
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }

    this->statusBar()->showMessage( tr( "Opened %1" ).arg( filepath ), 2000 );
    this->setWindowTitle( tr( "Formation Editor - " )
                          + fileinfo.fileName()
                          + tr( " -") );

    M_edit_canvas->setData( M_edit_data );
    M_edit_dialog->setData( M_edit_data );
    M_sample_view->setData( M_edit_data );
    M_constraint_view->setData( M_edit_data );

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );
    M_edit_canvas->update(); //emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();
    M_edit_dialog->updateData();

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
MainWindow::openBackgroundConfFile( const QString & filepath )
{
    std::cerr << "(MainWindow::openBackgroundConfFile) " << filepath.toStdString()
              << std::endl;

    if ( ! M_edit_data )
    {
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "Tried to open a background formation.\nBut no foreground data yet." ),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }

    if ( ! M_edit_data->formation() )
    {
        QMessageBox::warning( this,
                              tr( "Error" ),
                              tr( "Tried to open a background formation.\nNo formation for foreground data." ),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }

    if ( filepath.isEmpty() )
    {
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "Empty file path." ),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }

    QFileInfo fileinfo( filepath );

    if ( ! fileinfo.exists()
         || ! fileinfo.isReadable() )
    {
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "No such a file or not readable. \n" ) + filepath,
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }


    if ( ! M_edit_data->openBackgroundConf( filepath ) )
    {
        QMessageBox::warning( this,
                              tr( "Error" ),
                              tr( "Failed to open a " ) + filepath,
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
MainWindow::openDataFile( const QString & filepath )
{
    std::cerr << "(MainWindow::openDataFile) " << filepath.toStdString()
              << std::endl;

    if ( filepath.isEmpty() )
    {
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "Empty file path." ),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }

    if ( ! M_edit_data )
    {
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "No formation." ),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }

    QFileInfo fileinfo( filepath );

    if ( ! fileinfo.exists()
         || ! fileinfo.isReadable() )
    {
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "No such a file or not readable.\n" ) + filepath,
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;
    }

    if ( ! M_edit_data->openData( filepath ) )
    {
        QMessageBox::warning( this,
                              tr( "Error" ),
                              tr( "Failed to read the file.\n" ) + filepath,
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return false;

    }

    this->statusBar()->showMessage( tr( "Opened %1" ).arg( filepath ), 2000 );

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );
    M_edit_canvas->update(); //emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::showWarningMessage( const int err )
{
    QString msg;

    switch ( err ) {
    case SampleDataSet::NO_FORMATION:
        msg = tr( "No formation." );
        break;
    case SampleDataSet::TOO_MANY_DATA:
        msg = tr( "Too many data." );
        break;
    case SampleDataSet::TOO_NEAR_DATA:
        msg = tr( "Too near data." );
        break;
    case SampleDataSet::ILLEGAL_SYMMETRY_DATA:
        msg = tr( "Illegal symmetry data." );
        break;
    case SampleDataSet::TOO_NEAR_SYMMETRY_DATA:
        msg = tr( "Too near symmetry data." );
        break;
    case SampleDataSet::INSERT_RANGE_OVER:
        msg = tr( "Insert range over." );
        break;
    case SampleDataSet::INVALID_INDEX:
        msg = tr( "Invalid index." );
        break;
    case SampleDataSet::DUPLICATED_INDEX:
        msg = tr( "Origin and terminal of the constraint has a same index." );
        break;
    case SampleDataSet::DUPLICATED_CONSTRAINT:
        msg = tr( "The constraint already exists." );
        break;
    case SampleDataSet::INTERSECTS_CONSTRAINT:
        msg = tr( "Exists intersected constraints." );
        break;
    case SampleDataSet::NO_ERROR:
        //msg = tr( "No error." );
        return;
        break;
    default:
        msg = tr( "Unsupported error. error code=%1" ).arg( static_cast< int >( err ) );
        break;
    }


    QMessageBox::warning( this,
                          tr( "Warning" ),
                          msg,
                          QMessageBox::Ok,
                          QMessageBox::NoButton );
}


/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::newFile()
{
    if ( ! saveChanges() )
    {
        // data is changed, but save operation is cancelled.
        return;
    }

    QStringList names;
    // names.push_back( QString::fromStdString( FormationCDT::name() ) );
    names.push_back( QString::fromStdString( FormationDT::name() ) );
    // names.push_back( QString::fromStdString( FormationKNN::name() ) );
    //     names.push_back( QString::fromAscii( FormationBPN::name().c_str() ) );
    //     names.push_back( QString::fromAscii( FormationRBF::name().c_str() ) );
    //     names.push_back( QString::fromAscii( FormationNGNet::name().c_str() ) );

    bool ok = false;
    QString name = QInputDialog::getItem( this,
                                          tr( "Create New Formation" ),
                                          tr( "Choose a training method:" ),
                                          names,
                                          0,
                                          false, // no editable
                                          &ok );
    if ( ! ok )
    {
        return;
    }

    if ( M_edit_data )
    {
        M_edit_data.reset();
    }

    this->setWindowTitle( tr( "FormationEditor - New Formation -" ) );


    // create new data

    M_edit_data = boost::shared_ptr< EditData >( new EditData() );
    M_edit_data->createFormation( name );

    if ( ! M_edit_data->formation()
         || ! M_edit_data->samples() )
    {
        std::cerr << "***ERROR*** Failed to initialize formation data"
                  << std::endl;
        M_edit_data.reset();
        return;
    }

    M_edit_canvas->setData( M_edit_data );
    M_edit_dialog->setData( M_edit_data );
    M_sample_view->setData( M_edit_data );
    M_constraint_view->setData( M_edit_data );

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );

    M_edit_canvas->update(); // emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();
    M_edit_dialog->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::openConf()
{
    if ( ! saveChanges() )
    {
        // data changed, but save operation is canceled.
        return;
    }

    QString filter( tr( "Formation file (*.conf);;"
                        "All files (*)" ) );
    QString filepath = QFileDialog::getOpenFileName( this,
                                                     tr( "Open Formation" ),
                                                     tr( "" ),
                                                     filter );
    if ( filepath.isEmpty() )
    {
        return;
    }

    openConfFile( filepath );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::openBackgroundConf()
{
    QString filter( tr( "Background formation file (*.conf);;"
                        "All files (*)" ) );
    QString filepath = QFileDialog::getOpenFileName( this,
                                                     tr( "Open Background Formation" ),
                                                     tr( "" ),
                                                     filter );
    if ( filepath.isEmpty() )
    {
        return;
    }

    openBackgroundConfFile( filepath );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::openData()
{
    QString filter( tr( "Formation data file (*.dat);;"
                        "All files (*)" ) );
    QString filepath = QFileDialog::getOpenFileName( this,
                                                     tr( "Open Data" ),
                                                     tr( "" ),
                                                     filter );
    if ( filepath.isEmpty() )
    {
        return;
    }

    openDataFile( filepath );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::saveConf()
{
    if ( ! M_edit_data
         || ! M_edit_data->formation() )
    {
        return;
    }

    if ( M_edit_data->saveConf() )
    {
        this->statusBar()->showMessage( tr( "Saved %1" ).arg( M_edit_data->filePath() ), 2000 );
    }
    else
    {
        saveConfAs();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::saveConfAs()
{
    if ( ! M_edit_data
         || ! M_edit_data->formation() )
    {
        return;
    }

    QString filter( tr( "Formation file (*.conf);;"
                        "All files (*)" ) );
    QString filepath = QFileDialog::getSaveFileName( this,
                                                     tr( "Save Formation" ),
                                                     M_edit_data->filePath(),
                                                     filter );
    if ( filepath.isEmpty() )
    {
        return;
    }

    if ( filepath.length() <= 5
         || filepath.right( 5 ) != tr( ".conf" ) )
    {
        filepath += tr( ".conf" );
    }

    if ( M_edit_data->saveConfAs( filepath ) )
    {
        QFileInfo fileinfo( filepath );
        this->setWindowTitle( tr( "FormationEditor - " )
                              + fileinfo.fileName()
                              + tr( " -") );
        this->statusBar()->showMessage( tr( "Saved %1" ).arg( filepath ), 2000 );
    }
    else
    {
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "Failed to save the file " ) + filepath,
                               QMessageBox::Ok,
                               QMessageBox::NoButton );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::saveDataAs()
{
    if ( ! M_edit_data
         || ! M_edit_data->samples() )
    {
        return;
    }

    QString filter( tr( "Formation Data file (*.dat);;"
                        "All files (*)" ) );
    QString filepath = QFileDialog::getSaveFileName( this,
                                                     tr( "Save Training Data" ),
                                                     tr( "" ),
                                                     filter );

    if ( filepath.isEmpty() )
    {
        return;
    }

    if ( filepath.length() <= 4
         || filepath.right( 4 ) != tr( ".dat" ) )
    {
        filepath += tr( ".dat" );
    }

    if ( M_edit_data->samples()->save( filepath.toStdString() ) )
    {
        this->statusBar()->showMessage( tr( "Saved %1" ).arg( filepath ), 2000 );
    }
    else
    {
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "Failed to save the file " ) + filepath,
                               QMessageBox::Ok,
                               QMessageBox::NoButton );
    }
}


/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::setPlayerAutoMove( bool on )
{
    Options::instance().setPlayerAutoMove( on );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::setDataAutoSelect( bool on )
{
    Options::instance().setDataAutoSelect( on );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::setSymmetryMode( bool on )
{
    Options::instance().setSymmetryMode( on );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::setConstraintEditMode( bool on )
{
    Options::instance().setConstraintEditMode( on );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::addData()
{
    std::cerr << "(MainWindow::addData)" << std::endl;
    if ( ! M_edit_data
         || ! M_edit_data->samples() )
    {
        return;
    }

    SampleDataSet::ErrorType err = M_edit_data->addData();

    if ( err != SampleDataSet::NO_ERROR )
    {
        showWarningMessage( err );
        return;
    }

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );

    updateDataIndex();
    M_edit_canvas->update(); // emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::insertData()
{
    std::cerr << "(MainWindow::insertData)" << std::endl;
    if ( ! M_edit_data
         || ! M_edit_data->samples() )
    {
        return;
    }

    int index = M_index_spin_box->value() - 1;
    if ( index == -1 ) index = 0;

    SampleDataSet::ErrorType err  = M_edit_data->insertData( index );
    if ( err != SampleDataSet::NO_ERROR )
    {
        showWarningMessage( err );
        return;
    }

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );

    updateDataIndex();
    M_edit_canvas->update(); // emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::replaceData()
{
    std::cerr << "(MainWindow::replaceData)" << std::endl;
    if ( ! M_edit_data
         || ! M_edit_data->samples() )
    {
        return;
    }

    int index = M_index_spin_box->value() - 1;
    SampleDataSet::ErrorType err  = M_edit_data->replaceData( index );

    if ( err != SampleDataSet::NO_ERROR )
    {
        showWarningMessage( err );
        return;
    }

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );

    updateDataIndex();
    M_edit_canvas->update(); // emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::deleteData()
{
    if ( ! M_edit_data
         || ! M_edit_data->samples() )
    {
        return;
    }

    int index = M_edit_data->currentIndex();
    if ( index < 0 )
    {
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "No selected data." ),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        return;
    }

    std::cerr << "deleteData index=" << index << std::endl;

    SampleDataSet::ErrorType err = M_edit_data->deleteData( index );
    if ( err != SampleDataSet::NO_ERROR )
    {
        showWarningMessage( err );
        return;
    }

    M_edit_data->setCurrentIndex( -1 );

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );

    updateDataIndex();
    M_edit_canvas->update(); // emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::changeSampleIndex( int old_visual_index,
                               int new_visual_index )
{
    std::cerr << "(MainWindow::changeSampleIndex)"
              << " old_vis_idx=" << old_visual_index
              << " new_vis_idx=" << new_visual_index << std::endl;
    if ( ! M_edit_data
         || ! M_edit_data->samples() )
    {
        return;
    }

    SampleDataSet::ErrorType err = M_edit_data->changeDataIndex( old_visual_index - 1,
                                                                 new_visual_index - 1 );
    if ( err != SampleDataSet::NO_ERROR )
    {
        showWarningMessage( err );
        return;
    }

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );

    selectSample( M_edit_data->currentIndex() );
    M_edit_canvas->update(); // emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::reverseY()
{
    std::cerr << "(MainWindow::reverseY)" << std::endl;
    if ( M_edit_data )
    {
        M_edit_data->reverseY();
        M_edit_canvas->update(); // emit viewUpdated();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::train()
{
    std::cerr << "(MainWindow::train)" << std::endl;
    if ( ! M_edit_data
         || ! M_edit_data->samples() )
    {
        return;
    }

    M_edit_data->train();

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );

    M_edit_canvas->update(); // emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::resizeView( const QSize & size )
{
    if ( this->isMaximized()
         || this->isFullScreen() )
    {
        this->showNormal();
    }

    QRect rect = this->geometry();
    QList< int > sizes =  M_splitter->sizes();

    if ( sizes.size() != 2 )
    {
        std::cerr << "MainWindow::resizeView unexpected splitter size "
                  << sizes.size() << std::endl;
        return;
    }

    int width_diff = rect.width() - M_splitter->width();
    int height_diff = rect.height() - M_splitter->height();
    int first_width = sizes.at( 0 );

    rect.setWidth( width_diff + first_width + size.width() );
    rect.setHeight( height_diff + size.height() );

    this->setGeometry( rect );
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
MainWindow::toggleToolBar()
{
    QAction * act = qobject_cast< QAction * >( sender() );
    M_tool_bar->setVisible( act->isChecked() );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::toggleStatusBar()
{
    QAction * act = qobject_cast< QAction * >( sender() );
    this->statusBar()->setVisible( act->isChecked() );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::setEnlargeMode( bool on )
{
    Options::instance().setEnlarge( on );
    M_edit_canvas->update(); // emit viewUpdated();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::setShowIndex( bool on )
{
    Options::instance().setShowIndex( on );
    M_edit_canvas->update(); // emit viewUpdated();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::setShowTriangulation( bool on )
{
    Options::instance().setShowTriangulation( on );
    M_edit_canvas->update(); // emit viewUpdated();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::setShowCircumcircle( bool on )
{
    Options::instance().setShowCircumcircle( on );
    M_edit_canvas->update(); // emit viewUpdated();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::setShowShootLines( bool on )
{
    Options::instance().setShowShootLines( on );
    M_edit_canvas->update(); // emit viewUpdated();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::setShowGoalieMovableArea( bool on )
{
    Options::instance().setShowGoalieMovableArea( on );
    M_edit_canvas->update(); // emit viewUpdated();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::setAntialiasing( bool on )
{
    Options::instance().setAntialiasing( on );
    M_edit_canvas->update(); // emit viewUpdated();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::toggleShowBackgroundData( bool on )
{
    Options::instance().setShowBackgroundData( on );
    M_edit_canvas->update();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::about()
{
    QString msg( tr( "Formation Editor" ) );
    msg += tr( " " );
    msg += tr( VERSION );
    msg += tr( "\n\n" );
    msg += tr( "Formation Editor is a formation editing tool for\n"
               "a simulated soccer team team using librcsc.\n"
               "\n"
               "Formation Editor Development Site:\n"
               "  http://rctools.sourceforge.jp/\n"
               "Author:\n"
               "  Hidehisa Akiyama <akky@users.sourceforge.jp>" );

    QMessageBox::about( this,
                        tr( "About Formation Editor" ),
                        msg );

    // from Qt 4.4 documents
    /*
      QMessageBox::about() looks for a suitable icon in four locations:

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
MainWindow::setPositionLabel( const QPointF & pos )
{
    M_position_label->setText( tr( "(%1, %2)" )
                               .arg( pos.x(), 0, 'f', 2 )
                               .arg( pos.y(), 0, 'f', 2 ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::editMenuAboutToShow()
{
    M_undo_act->setText( tr( "Undo " ) + M_undo_stack->undoText() );
    M_redo_act->setText( tr( "Redo " ) + M_undo_stack->redoText() );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::selectSample( int index )
{
    if ( M_edit_data
         && M_edit_data->currentIndex() != index
         && M_edit_data->setCurrentIndex( index ) )
    {
        updateDataIndex();
        M_edit_dialog->updateData();
        M_edit_canvas->update(); // emit viewUpdated();
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::selectSampleVisualIndex( int value )
{
    selectSample( value - 1 );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::deleteSample( int index )
{
    std::cerr << "(MainWindow::deleteSample) index=" << index << std::endl;

    SampleDataSet::ErrorType err = M_edit_data->deleteData( index );
    if ( err != SampleDataSet::NO_ERROR )
    {
        showWarningMessage( err );
        return;
    }

    M_edit_data->setCurrentIndex( -1 );

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );

    updateDataIndex();
    M_edit_canvas->update(); // emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::replaceBall( int index,
                         double x,
                         double y )
{
    std::cerr << "(MainWindow::replaceBall) index=" << index
              << " (" << x << " , " << y << ")" << std::endl;

    if ( ! M_edit_data )
    {
        return;
    }

    SampleDataSet::ErrorType err  = M_edit_data->replaceBall( index, x, y );

    if ( err != SampleDataSet::NO_ERROR )
    {
        showWarningMessage( err );
        return;
    }

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );

    updateDataIndex();
    M_edit_canvas->update(); // emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::replacePlayer( int index,
                           int unum,
                           double x,
                           double y )
{
    std::cerr << "(MainWindow::replacePlayer) index=" << index
              << " unum=" << unum
              << " (" << x << " , " << y << ")" << std::endl;

    if ( ! M_edit_data )
    {
        return;
    }

    SampleDataSet::ErrorType err  = M_edit_data->replacePlayer( index, unum, x, y );

    if ( err != SampleDataSet::NO_ERROR )
    {
        showWarningMessage( err );
        return;
    }

    const int data_count = M_edit_data->samples()->dataCont().size();
    M_index_spin_box->setRange( 0, data_count );

    updateDataIndex();
    M_edit_canvas->update(); // emit viewUpdated();
    M_sample_view->updateData();
    M_constraint_view->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::deleteConstraint( int origin_idx,
                              int terminal_idx )
{
    std::cerr << "(MainWindow::deleteConstraint)"
              << " origin=" << origin_idx
              << " terminal=" << terminal_idx << std::endl;

    SampleDataSet::ErrorType err = M_edit_data->deleteConstraint( origin_idx,
                                                                  terminal_idx );
    if ( err != SampleDataSet::NO_ERROR )
    {
        showWarningMessage( err );
        return;
    }

    M_edit_canvas->update(); // emit viewUpdated();
    M_constraint_view->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::replaceConstraint( int idx,
                               int origin_idx,
                               int terminal_idx )
{
    std::cerr << "(MainWindow::ReplaceConstraint) idx=" << idx
              << " origin=" << origin_idx
              << " terminal=" << terminal_idx << std::endl;

    SampleDataSet::ErrorType err = M_edit_data->replaceConstraint( idx,
                                                                   origin_idx,
                                                                   terminal_idx );

    if ( err != SampleDataSet::NO_ERROR )
    {
        showWarningMessage( err );
        return;
    }

    M_edit_canvas->update();
    M_constraint_view->updateData();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::updateDataIndex()
{
    if ( M_edit_data )
    {
        int idx = M_edit_data->currentIndex();

        if ( 0 <= idx )
        {
            M_index_spin_box->setValue( idx + 1 );
            M_sample_view->selectSample( idx );
        }
        else
        {
            M_index_spin_box->setValue( 0 );
            M_sample_view->unselectData();
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::showConstraintEditDialog()
{
    if ( ! M_edit_data
         || M_edit_data->samples()->dataCont().size() < 2 )
    {
        return;
    }

    showConstraintEditDialog( 0, M_edit_data->samples()->dataCont().size() - 1 );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
MainWindow::showConstraintEditDialog( int first_index,
                                      int second_index )
{
    std::cerr << "(MainWindow::showConstraintEditDialog)"
              << " first=" << first_index << " second=" << second_index
              << std::endl;

    if ( ! M_edit_data
         || M_edit_data->samples()->dataCont().size() < 2 )
    {
        return;
    }

    ConstraintEditDialog dlg( this,
                              M_edit_canvas,
                              M_edit_data,
                              first_index,
                              second_index );
    int result = dlg.exec();
    if ( result == QDialog::Accepted )
    {
        int origin = dlg.originIndex() - 1;
        int terminal = dlg.terminalIndex() - 1;
        std::cerr << "  --> Accepted"
                  << " origin=" << origin
                  << " terminal=" << terminal
                  << std::endl;

        SampleDataSet::ErrorType err = M_edit_data->addConstraint( origin, terminal );

        if ( err != SampleDataSet::NO_ERROR )
        {
            M_edit_data->releaseObject();
            M_edit_canvas->update();
            showWarningMessage( err );
        }
        else
        {
            M_sample_view->updateData();
            M_constraint_view->updateData();
            M_edit_data->releaseObject();
            M_edit_canvas->update();
        }
    }
    else
    {
        M_edit_data->releaseObject();
        M_edit_canvas->update();
    }
}
