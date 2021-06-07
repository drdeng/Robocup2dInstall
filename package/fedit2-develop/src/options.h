// -*-c++-*-

/*!
  \file options.h
  \brief global configuration class Header File.
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

#ifndef FEDIT_OPTIONS_H
#define FEDIT_OPTIONS_H

#include <QPointF>
#include <QString>

/*!
  \class Options
  \brief global configuration holder.
*/
class Options {
public:

    static const int DEFAULT_CANVAS_WIDTH;
    static const int DEFAULT_CANVAS_HEIGHT;

    enum {
        FIRLD_GOAL_Z = 0,
        FIRLD_LINE_Z = 1,
        TRIANGULATION_Z = 10,
        PLAYER_Z = 20,
        BACKGROUND_PLAYER_Z = 25,
        BALL_Z = 30,
    };

private:

    //
    // window options
    //
    bool M_maximize;
    bool M_full_screen;

    int M_window_width;
    int M_window_height;
    int M_window_pos_x;
    int M_window_pos_y;

    int M_view_width;
    int M_view_height;

    QPointF M_focus_point;
    double M_view_scale;

    //
    // editor options
    //

    QString M_conf_file;
    QString M_data_file;
    QString M_background_conf_file;

    bool M_auto_backup;

    //
    // mode options
    //
    bool M_player_auto_move;
    bool M_data_auto_select;
    bool M_symmetry_mode;
    bool M_constraint_edit_mode;

    //
    // view options
    //

    bool M_show_background_data;
    bool M_enlarge;

    bool M_show_index;
    bool M_show_triangulation;
    bool M_show_circumcircle;
    bool M_show_shoot_lines;
    bool M_show_goalie_movable_area;

    bool M_antialiasing;
    bool M_auto_fit_mode;

    // private access for singleton
    Options();

    // not used
    Options( const Options & );
    Options & operator=( const Options & );

public:

    ~Options();

    static
    Options & instance();

    // analyze command line options
    bool parseCmdLine( int argc,
                       char ** argv );

private:
    // read/write settings
    void readSettings();
    void writeSettings();

    void parseWindowGeometry( const std::string & val );

public:

    //
    // window options
    //

    bool maximize() const
      {
          return M_maximize;
      }

    bool fullScreen() const
      {
          return M_full_screen;
      }

    int windowPosX() const
      {
          return M_window_pos_x;
      }

    int windowPosY() const
      {
          return M_window_pos_y;
      }

    int windowWidth() const
      {
          return M_window_width;
      }

    int windowHeight() const
      {
          return M_window_height;
      }

    int viewWidth() const
      {
          return M_view_width;
      }

    int viewHeight() const
      {
          return M_view_height;
      }

    const QPointF & focusPoint() const
      {
          return M_focus_point;
      }

    double viewScale() const
      {
          return M_view_scale;
      }

    void setViewSize( const int width,
                      const int height )
      {
          M_view_width = width;
          M_view_height = height;
      }

    void setFocusPoint( const QPointF & p )
      {
          M_focus_point = p;
      }

    void setViewScale( const double scale )
      {
          M_view_scale = scale;
      }

    //
    // editor options
    //

    const QString & confFile() const
      {
          return M_conf_file;
      }

    const QString & backgroundConfFile() const
      {
          return M_background_conf_file;
      }

    const QString & dataFile() const
      {
          return M_data_file;
      }

    bool autoBackup() const
      {
          return M_auto_backup;
      }

    //
    // mode options
    //

    void setPlayerAutoMove( bool on )
      {
          M_player_auto_move = on;
      }
    bool playerAutoMove() const
      {
          return M_player_auto_move;
      }

    void setDataAutoSelect( bool on )
      {
          M_data_auto_select = on;
      }
    bool dataAutoSelect() const
      {
          return M_data_auto_select;
      }

    void setSymmetryMode( bool on )
      {
          M_symmetry_mode = on;
      }
    bool symmetryMode() const
      {
          return M_symmetry_mode;
      }

    void setConstraintEditMode( bool on )
      {
          M_constraint_edit_mode = on;
      }
    bool constraintEditMode() const
      {
          return M_constraint_edit_mode;
      }

    //
    // view options
    //

    void setShowBackgroundData( const bool on )
      {
          M_show_background_data = on;
      }
    bool showBackgroundData() const
      {
          return M_show_background_data;
      }

    void setEnlarge( const bool on )
      {
          M_enlarge = on;
      }
    bool enlarge() const
      {
          return M_enlarge;
      }

    void setShowIndex( const bool on )
      {
          M_show_index = on;
      }
    bool showIndex() const
      {
          return M_show_index;
      }


    void setShowTriangulation( const bool on )
      {
          M_show_triangulation = on;
      }
    bool showTriangulation() const
      {
          return M_show_triangulation;
      }

    void setShowCircumcircle( const bool on )
      {
          M_show_circumcircle = on;
      }
    bool showCircumcircle() const
      {
          return M_show_circumcircle;
      }

    void setShowShootLines( const bool on ) { M_show_shoot_lines = on; }
    bool showShootLines() const { return M_show_shoot_lines; }

    void setShowGoalieMovableArea( const bool on ) { M_show_goalie_movable_area = on; }
    bool showGoalieMovableArea() const { return M_show_goalie_movable_area; }

    //

    void setAntialiasing( const bool on )
      {
          M_antialiasing = on;
      }
    bool antialiasing() const
      {
          return M_antialiasing;
      }

    //

    void setAutoFitMode( const bool on )
      {
          M_auto_fit_mode = on;
      }
    bool autoFitMode() const
      {
          return M_auto_fit_mode;
      }

};

#endif
