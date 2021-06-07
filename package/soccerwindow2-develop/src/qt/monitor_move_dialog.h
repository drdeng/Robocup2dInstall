// -*-c++-*-

/*!
  \file monitor_move_dialog.h
  \brief monitor client move control dialog class Header File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa Akiyama

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

#ifndef SOCCERWINDOW2_QT4_PLAYER_MOVE_DIALOG_H
#define SOCCERWINDOW2_QT4_PLAYER_MOVE_DIALOG_H

#include <QDialog>

class QCheckBox;
class QLineEdit;
class QRadioButton;

class MainData;
class TrainerData;


//! field canvas configuration dialog
class MonitorMoveDialog
    : public QDialog {

    Q_OBJECT

private:

    const MainData & M_main_data;
    TrainerData & M_trainer_data;

    QCheckBox * M_ball_cb;
    QLineEdit * M_ball_x;
    QLineEdit * M_ball_y;
    QCheckBox * M_ball_vel_cb;
    QLineEdit * M_ball_vx;
    QLineEdit * M_ball_vy;

    QRadioButton * M_drop_ball_rb;
    QRadioButton * M_free_kick_left_rb;
    QRadioButton * M_free_kick_right_rb;

    QCheckBox * M_left_all_cb;
    QCheckBox * M_left_cb[11];
    QLineEdit * M_left_x[11];
    QLineEdit * M_left_y[11];
    QLineEdit * M_left_body[11];

    QCheckBox * M_right_all_cb;
    QCheckBox * M_right_cb[11];
    QLineEdit * M_right_x[11];
    QLineEdit * M_right_y[11];
    QLineEdit * M_right_body[11];

public:

    MonitorMoveDialog( QWidget * parent,
                       const MainData & main_data,
                       TrainerData & trainer_data );
    ~MonitorMoveDialog();

private:

    void createWidgets();

    QWidget * createBallBox();
    QWidget * createPlayModeBox();
    QWidget * createLeftTeamBox();
    QWidget * createRightTeamBox();

private slots:

    void readFieldStatus();
    void open();
    void save();

    void toggleBallCheck( bool on );
    void toggleBallVelCheck( bool on );


    void toggleLeftAll( bool on );
    void toggleRightAll( bool on );

    void toggleLeftCheck( int index );
    void toggleRightCheck( int index );

    void sendCommand();

signals:

    void executed();

};

#endif
