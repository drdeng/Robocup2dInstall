// -*-c++-*-

/*!
	\file edit_canvas.h
	\brief main edit canvas class Header File.
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
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code; see the file COPYING.	If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////

#ifndef FEDIT2_EDIT_CANVAS_H
#define FEDIT2_EDIT_CANVAS_H

#ifdef USE_GLWIDGET
#include <QGLWidget>
#else
#include <QWidget>
#endif

#include <QColor>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QPointF>

#include "mouse_state.h"

#include <boost/weak_ptr.hpp>

class QPainter;

class EditData;
class MainWindow;

/*!
  \class EditCanvas
 */
class EditCanvas
    :
#ifdef USE_GLWIDGET
    public QGLWidget
#else
    public QWidget
#endif
{
    Q_OBJECT

private:

    boost::weak_ptr< EditData > M_edit_data;

    QTransform M_transform;

    //
    // graphics context objects
    //
    QColor M_field_color;
    //QBrush M_field_brush;
    QBrush M_field_dark_brush;
    QPen M_line_pen;
    QPen M_triangle_pen;
    QPen M_constraint_pen;
    QFont M_triangle_font;
    QPen M_area_pen;
    QPen M_ball_pen;
    QBrush M_ball_brush;
    QPen M_player_pen;
    QPen M_select_pen;
    QBrush M_player_brush;
    QBrush M_symmetry_brush;
    QFont M_player_font;

    QBrush M_background_contained_area_brush;
    QPen M_background_triangle_pen;
    QPen M_background_player_pen;
    QBrush M_background_left_team_brush;
    QBrush M_background_right_team_brush;
    QBrush M_background_symmetry_brush;
    QPen M_background_font_pen;

    QPen M_shoot_line_pen;

    //! 0: left, 1: middle, 2: right
    MouseState M_mouse_state[3];

    // not used
    EditCanvas( const EditCanvas & );
    const EditCanvas & operator=( const EditCanvas & );

public:

    explicit
    EditCanvas( QWidget * parent = 0 );
    ~EditCanvas();


    void setData( boost::shared_ptr< EditData > ptr )
      {
          M_edit_data = ptr;
      }

private:

    void setAntialiasFlag( QPainter & painter,
                           bool on );

    void drawField( QPainter & painter );
    void drawContainedArea( QPainter & painter );
    void drawData( QPainter & painter );
    void drawPlayers( QPainter & painter );
    void drawBall( QPainter & painter );
    void drawShootLines( QPainter & painter );
    void drawGoalieMovableArea( QPainter & painter );
    void drawConstraintSelection( QPainter & painter );

    void drawBackgroundContainedArea( QPainter & painter );
    void drawBackgroundData( QPainter & painter );
    void drawBackgroundPlayers( QPainter & painter );

    void setFocusPoint( const QPoint & pos );

protected:

    void paintEvent( QPaintEvent * );

    void mouseDoubleClickEvent( QMouseEvent * event );
    void mousePressEvent( QMouseEvent * event );
    void mouseReleaseEvent( QMouseEvent * event );
    void mouseMoveEvent( QMouseEvent * event );

public slots:

    //void setScale( double scale );
    void zoomIn();
    void zoomOut();
    void fitToScreen();

signals:
    void objectMoved();
    void mouseMoved( const QPointF & pos );
    void constraintSelected( int first_index, int second_index );

};

#endif
