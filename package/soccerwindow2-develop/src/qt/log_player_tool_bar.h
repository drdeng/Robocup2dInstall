// -*-c++-*-

/*!
  \file log_player_tool_bar.h
  \brief log player control tool bar class Header File.
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

#ifndef SOCCERWINDOW2_QT_LOG_PLAYER_TOOL_BAR_H
#define SOCCERWINDOW2_QT_LOG_PLAYER_TOOL_BAR_H

#include <QToolBar>

class QAction;
class QBoxLayout;
class QSlider;
class QLineEdit;
class QMoveEvent;
class QWidget;

class MainData;

class LogPlayerToolBar
    : public QToolBar {

    Q_OBJECT

private:

    const MainData & M_main_data;

    QSlider * M_cycle_slider;
    QLineEdit * M_cycle_edit;

public:

    LogPlayerToolBar( const MainData & main_data,
                      QWidget * parent );
    ~LogPlayerToolBar();

    void createCycleSlider();
    void createCycleEdit();

protected:

    //void moveEvent( QMoveEvent * event );

private slots:

    void editCycle();

public slots:

    void changeOrientation( Qt::Orientation );

    void updateSlider();

signals:

    void indexChanged( int idx );
    void cycleChanged( int cycle );

};

#endif
