// -*-c++-*-

/*!
  \file debug_log_dir_dialog.h
  \brief Debug log file directory selector dialog Header File.
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

#ifndef SOCCERWINDOW2_QT_DEBUG_LOG_DIR_DIALOG_H
#define SOCCERWINDOW2_QT_DEBUG_LOG_DIR_DIALOG_H

#include <rcsc/types.h>

#include <QDialog>

class QLineEdit;
class QPushButton;
class QRadioButton;

/*!
  \class DebugLogDirDialog
  \brief modal dialog to select a debug log directory
 */
class DebugLogDirDialog
    : public QDialog {

    Q_OBJECT

private:
    QRadioButton * M_left_rb;
    QRadioButton * M_right_rb;

    QLineEdit * M_log_dir;
    QPushButton * M_select_dir_btn;

public:

    DebugLogDirDialog( QWidget * parent );

    ~DebugLogDirDialog();

    rcsc::SideID side() const;
    QString dirPath() const;

private:

    void createControls();

    QWidget * createSideSelectionBox();
    QWidget * createDirSelectionBox();
    QLayout * createAnswerButtons();

private slots:

    void clickDirSelectionButton();
};

#endif
