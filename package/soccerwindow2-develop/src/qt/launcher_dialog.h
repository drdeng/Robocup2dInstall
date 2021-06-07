// -*-c++-*-

/*!
  \file launcher_dialog.h
  \brief Launcher dialog class Header File.
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

#ifndef SOCCERWINDOW2_QT_LAUNCHER_DIALOG_H
#define SOCCERWINDOW2_QT_LAUNCHER_DIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;

class LauncherDialog
    : public QDialog {

    Q_OBJECT

private:

    static const int MAX_HISTORY;

    QLineEdit * M_rcssserver_command;

    QComboBox * M_left_team_commands;

    QComboBox * M_right_team_commands;

    // not used
    LauncherDialog();
    LauncherDialog( const LauncherDialog & );
    const LauncherDialog & operator=( const LauncherDialog & );

public:

    explicit
    LauncherDialog( QWidget * parent );

    ~LauncherDialog();

private:

    void readSettings();

    void saveSettings();

    void createControls();

    QWidget * createServerControls();

    QWidget * createLeftTeamControls();

    QWidget * createRightTeamControls();

    QWidget * createExecuteControls();

    void updateComboBox( QComboBox * combo_box );

private slots:

    void startServer();

    void startLeftTeam();

    void startRightTeam();

    void startAll();

signals:

    void launchServer( const QString & );

};

#endif
