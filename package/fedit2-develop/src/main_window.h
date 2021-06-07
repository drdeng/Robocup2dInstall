// -*-c++-*-

/*!
  \file main_window.h
  \brief main application window class Header File.
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

#ifndef FEDIT2_MAIN_WINDOW_H
#define FEDIT2_MAIN_WINDOW_H

#include <QMainWindow>

#include <boost/shared_ptr.hpp>

class QCloseEvent;
class QLabel;
class QModelIndex;
class QSpinBox;
class QSplitter;
class QToolBar;
class QToolBox;
class QUndoStack;

class ConfigDialog;
class EditCanvas;
class EditData;
class EditDialog;
class ConstraintView;
class SampleView;

/*!
  \class MainWindow
 */
class MainWindow
    : public QMainWindow {

    Q_OBJECT

private:

    boost::shared_ptr< EditData > M_edit_data;

    QToolBar * M_tool_bar;
    QSpinBox * M_index_spin_box;

    QSplitter * M_splitter;
    SampleView * M_sample_view;
    ConstraintView * M_constraint_view;
    EditCanvas * M_edit_canvas;
    EditDialog * M_edit_dialog;
    ConfigDialog * M_config_dialog;

    QLabel * M_position_label;

    // file actions
    QAction * M_new_file_act;
    QAction * M_open_conf_act;
    QAction * M_open_background_conf_act;
    QAction * M_open_data_act;
    QAction * M_save_act;
    QAction * M_save_as_act;
    QAction * M_save_data_as_act;
    QAction * M_quit_act;

    // edit actions
    QAction * M_undo_act;
    QAction * M_redo_act;

    QAction * M_add_data_act;
    QAction * M_insert_data_act;
    QAction * M_replace_data_act;
    QAction * M_delete_data_act;
    QAction * M_reverse_y_act;
    QAction * M_add_constraint_act;
    QAction * M_train_act;

    QAction * M_toggle_player_auto_move_act;
    QAction * M_toggle_data_auto_select_act;
    QAction * M_toggle_symmetry_mode_act;
    QAction * M_toggle_constraint_edit_mode_act;

    // view actions
    QAction * M_full_screen_act;
    QAction * M_toggle_tool_bar_act;
    QAction * M_toggle_status_bar_act;
    QAction * M_zoom_in_act;
    QAction * M_zoom_out_act;
    QAction * M_fit_to_screen_act;

    QAction * M_toggle_enlarge_act;
    QAction * M_toggle_show_index_act;
    QAction * M_toggle_show_triangulation_act;
    QAction * M_toggle_show_circumcircle_act;
    QAction * M_toggle_show_shoot_lines_act;
    QAction * M_toggle_show_goalie_movable_area_act;

    QAction * M_toggle_antialiasing_act;
    QAction * M_toggle_show_background_data_act;
    QAction * M_show_edit_dialog_act;

    QAction * M_show_config_dialog_act;

    // help actions
    QAction * M_about_act;

    //
    // undo
    //

    QUndoStack * M_undo_stack;


    // not used
    MainWindow( const MainWindow & );
    const MainWindow & operator=( const MainWindow & );

public:

    MainWindow();
    ~MainWindow();

    void init();

private:

    void readSettings();
    void writeSettings();

    QToolBox * createToolBox();
    void createEditCanvas();
    void createEditDialog();
    void createSampleView();
    void createConstraintView();

    void createUndoStack();

    void createToolBars();
    void createStatusBar();

    void createActions();
    void createActionsFile();
    void createActionsEdit();
    void createActionsView();
    void createActionsHelp();

    void createMenus();
    void createMenuFile();
    void createMenuEdit();
    void createMenuView();
    void createMenuHelp();

protected:

    void closeEvent( QCloseEvent * event );
    void wheelEvent( QWheelEvent * event );

private:

    bool saveChanges();

    bool openConfFile( const QString & filepath );
    bool openBackgroundConfFile( const QString & filepath );
    bool openDataFile( const QString & filepath );

    void showWarningMessage( const int err );

    void updateEditModel();

private slots:

    // file
    void newFile();

    void openConf();
    void openBackgroundConf();
    void saveConf();
    void saveConfAs();

    void openData();
    void saveDataAs();

    // edit
    void setPlayerAutoMove( bool on );
    void setDataAutoSelect( bool on );
    void setSymmetryMode( bool on );
    void setConstraintEditMode( bool on );

    void addData();
    void insertData();
    void replaceData();
    void deleteData();
    void changeSampleIndex( int old_visual_index,
                            int new_visual_index );
    void reverseY();
    void train();

    // view
    void resizeView( const QSize & size );
    void toggleFullScreen();
    void toggleToolBar();
    void toggleStatusBar();
    void setEnlargeMode( bool on );
    void setShowIndex( bool on );
    void setShowTriangulation( bool on );
    void setShowCircumcircle( bool on );
    void setShowShootLines( bool on );
    void setShowGoalieMovableArea( bool on );
    void setAntialiasing( bool on );
    void toggleShowBackgroundData( bool on );

    // help
    void about();

    // other
    void setPositionLabel( const QPointF & pos );
    void editMenuAboutToShow();

    void selectSample( int index );
    void selectSampleVisualIndex( int value );
    void deleteSample( int index );
    void replaceBall( int index,
                      double x,
                      double y );
    void replacePlayer( int index,
                        int unum,
                        double x,
                        double y );

    void deleteConstraint( int origin_idx,
                           int terminal_idx );
    void replaceConstraint( int idx,
                            int origin_idx,
                            int terminal_idx );

    void updateDataIndex();

    void showConstraintEditDialog();
    void showConstraintEditDialog( int first_index,
                                   int second_index );
signals:
    void viewUpdated();

};

#endif
