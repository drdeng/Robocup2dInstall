// -*-c++-*-

/*!
  \file edit_data.h
  \brief formation editor data class Header File.
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

#ifndef FEDIT2_EDIT_DATA_H
#define FEDIT2_EDIT_DATA_H

#include <QString>

#include <rcsc/formation/sample_data.h>
#include <rcsc/formation/formation.h>
//#include <rcsc/geom/cdt/triangulation.h>
#include <rcsc/geom/triangulation.h>
#include <rcsc/geom/vector_2d.h>

#include <boost/shared_ptr.hpp>

#include <vector>
#include <utility>
#include <iostream>


class EditData {
public:

    static const double MAX_X;
    static const double MAX_Y;

    enum SelectType {
        SELECT_BALL,
        SELECT_PLAYER,
        SELECT_SAMPLE,
        NO_SELECT,
    };

private:

    QString M_filepath;
    bool M_conf_changed;
    QString M_saved_datetime;

    rcsc::formation::SampleData M_state; //!< current state on the edit canvas.

    rcsc::Formation::Ptr M_formation;
    rcsc::formation::SampleDataSet::Ptr M_samples; //!< training data
    rcsc::Triangulation M_triangulation;

    rcsc::Formation::Ptr M_background_formation;
    rcsc::Triangulation M_background_triangulation;

    int M_current_index;

    SelectType M_select_type;
    size_t M_select_index;

    size_t M_constraint_origin_index;
    size_t M_constraint_terminal_index;
    rcsc::Vector2D M_constraint_terminal;

    // not used
    EditData( const EditData & );
    EditData & operator=( const EditData & );
public:

    EditData();
    ~EditData();

    void createFormation( const QString & type_name );
private:

    void init();
    void backup( const QString & filepath );

public:

    const
    QString & filePath() const
      {
          return M_filepath;
      }

    bool isConfChanged() const
      {
          return M_conf_changed;
      }

    const
    rcsc::formation::SampleData & state() const
      {
          return M_state;
      }

    rcsc::Formation::ConstPtr formation() const
      {
          return M_formation;
      }
    rcsc::formation::SampleDataSet::Ptr samples() const
      {
          return M_samples;
      }
    const
    rcsc::Triangulation & triangulation() const
      {
          return M_triangulation;
      }

    rcsc::Formation::ConstPtr backgroundFormation() const
      {
          return M_background_formation;
      }
    const
    rcsc::Triangulation & backgroundTriangulation() const
      {
          return M_background_triangulation;
      }

    int currentIndex() const
      {
          return M_current_index;
      }

    SelectType selectType() const
      {
          return M_select_type;
      }
    size_t selectIndex() const
      {
          return M_select_index;
      }

    size_t constraintOriginIndex() const
      {
          return M_constraint_origin_index;
      }

    size_t constraintTerminalIndex() const
      {
          return M_constraint_terminal_index;
      }

    const
    rcsc::Vector2D & constraintTerminal() const
      {
          return M_constraint_terminal;
      }

    bool openConf( const QString & filepath );
    bool saveConf();
    bool saveConfAs( const QString & filepath );

    bool openData( const QString & filepath );

    bool openBackgroundConf( const QString & filepath );

private:
    void updatePlayerPosition();
    void updateTriangulation();

public:

    void updateRoleData( const int unum,
                         const int symmetry_unum,
                         const std::string & role_name );
    void updateRoleType( const int unum,
                         const int type_index );
    void updateMarkerData( const int unum,
                           const bool marker,
                           const bool setplay_marker );

    void moveBallTo( const double & x,
                     const double & y );
    void movePlayerTo( const int unum,
                       const double & x,
                       const double & y );
    void setConstraintTerminal( const double & x,
                                const double & y );
    void setConstraintIndex( const int origin_idx,
                             const int terminal_idx );
    bool moveSelectObjectTo( const double & x,
                             const double & y );

    bool selectObject( const double & x,
                       const double & y );
    bool releaseObject();

    rcsc::formation::SampleDataSet::ErrorType addData();
    rcsc::formation::SampleDataSet::ErrorType insertData( const int idx );
    rcsc::formation::SampleDataSet::ErrorType replaceData( const int idx );
    rcsc::formation::SampleDataSet::ErrorType replaceBall( const int idx,
                                                           const double & x,
                                                           const double & y );
    rcsc::formation::SampleDataSet::ErrorType replacePlayer( const int idx,
                                                             const int unum,
                                                             const double & x,
                                                             const double & y );
    rcsc::formation::SampleDataSet::ErrorType deleteData( const int idx );
    rcsc::formation::SampleDataSet::ErrorType changeDataIndex( const int old_idx,
                                                               const int new_idx );

    rcsc::formation::SampleDataSet::ErrorType addConstraint( const int origin_idx,
                                                             const int terminal_idx );
    rcsc::formation::SampleDataSet::ErrorType replaceConstraint( const int idx,
                                                                 const int origin_idx,
                                                                 const int terminal_idx );
    rcsc::formation::SampleDataSet::ErrorType deleteConstraint( const int origin_idx,
                                                                const int terminal_idx );


    bool setCurrentIndex( const int idx );
    void reverseY();
    void train();
};

#endif
