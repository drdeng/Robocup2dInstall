// -*-c++-*-

/*!
  \file constraint_edit_dialog.h
  \brief Formation editor position edit Dialog class Header File.
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

#ifndef FEDIT2_CONSTRAINT_EDIT_DIALOG_H
#define FEDIT2_CONSTRAINT_EDIT_DIALOG_H

#include <QDialog>

#include <boost/shared_ptr.hpp>

class EditCanvas;
class EditData;

class QSpinBox;

/*!
  \class EditDialog
  \brief formation editor position edit dialog
*/
class ConstraintEditDialog
    : public QDialog {

    Q_OBJECT;

private:

    EditCanvas * M_edit_canvas;
    boost::shared_ptr< EditData > M_edit_data;

    QSpinBox * M_origin;
    QSpinBox * M_terminal;

    // not used
    ConstraintEditDialog();
    ConstraintEditDialog( const ConstraintEditDialog & );
    ConstraintEditDialog & operator=( const ConstraintEditDialog & );
public:

    ConstraintEditDialog( QWidget * parent,
                          EditCanvas * canvas,
                          boost::shared_ptr< EditData > data,
                          const int origin_index,
                          const int terminal_index );
    ~ConstraintEditDialog();

    int originIndex() const;
    int terminalIndex() const;

private:

    void createWidgets();


private slots:

    void changeIndex();

};

#endif
