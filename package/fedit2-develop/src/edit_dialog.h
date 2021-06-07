// -*-c++-*-

/*!
  \file edit_dialog.h
  \brief position edit Dialog class Header File.
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

#ifndef FEDIT2_EDIT_DIALOG_H
#define FEDIT2_EDIT_DIALOG_H

#include <QDialog>

#include <boost/weak_ptr.hpp>

class QCheckBox;
class QCloseEvent;
class QComboBox;
class QShowEvent;
class QLineEdit;

class EditData;

/*!
  \class EditDialog
  \brief formation editor position edit dialog
*/
class EditDialog
    : public QDialog {

    Q_OBJECT;

private:

    boost::weak_ptr< EditData > M_edit_data;

    QLineEdit * M_type_name; //! formation method type

    QLineEdit * M_ball_pos_x;
    QLineEdit * M_ball_pos_y;

    QLineEdit * M_symmetry_unum[11];
    QComboBox * M_role_type[11];
    QLineEdit * M_role_name[11];
    QCheckBox * M_marker[11];
    QCheckBox * M_setplay_marker[11];

    QLineEdit * M_pos_x[11];
    QLineEdit * M_pos_y[11];

    // not used
    EditDialog();
    EditDialog( const EditDialog & );
    EditDialog & operator=( const EditDialog & );
public:

    EditDialog( QWidget * paremt );
    ~EditDialog();

    void setData( boost::shared_ptr< EditData > ptr )
      {
          M_edit_data = ptr;
      }

private:
    void createWidgets();

protected:
    void showEvent( QShowEvent * event );
    void closeEvent( QCloseEvent * event );

private slots:
    void validateBallCoordinate();
    void resetChanges();
    void applyToField();

public slots:
    void updateData();

signals:
    void viewUpdated();
    void shown( bool on );
};

#endif
