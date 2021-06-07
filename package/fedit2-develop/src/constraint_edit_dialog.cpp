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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "constraint_edit_dialog.h"

#include "edit_canvas.h"
#include "edit_data.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

*/
ConstraintEditDialog::ConstraintEditDialog( QWidget * parent,
                                            EditCanvas * canvas,
                                            boost::shared_ptr< EditData > data,
                                            const int origin_index,
                                            const int terminal_index )
    : QDialog( parent )
    , M_edit_canvas( canvas )
    , M_edit_data( data )
{
    Q_ASSERT( canvas );
    Q_ASSERT( data );

    this->setWindowTitle( tr( "Select Constraints" ) );

    createWidgets();

    M_origin->setValue( origin_index + 1 );
    M_terminal->setValue( terminal_index + 1 );
}

/*-------------------------------------------------------------------*/
/*!

*/
ConstraintEditDialog::~ConstraintEditDialog()
{
    //std::cerr << "delete ConstraintEditDialog" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
ConstraintEditDialog::originIndex() const
{
    return M_origin->value();
}

/*-------------------------------------------------------------------*/
/*!

*/
int
ConstraintEditDialog::terminalIndex() const
{
    return M_terminal->value();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConstraintEditDialog::createWidgets()
{
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setMargin( 2 );
    top_layout->setSpacing( 2 );
    top_layout->setSizeConstraint( QLayout::SetFixedSize );

    {
        QGridLayout * layout = new QGridLayout();

        //
        layout->addWidget( new QLabel( tr( "origin" ) ), 0, 0 );
        //
        layout->addWidget( new QLabel( tr( "terminal" ) ), 0, 1 );

        //
        M_origin = new QSpinBox();
        M_origin->setRange( 1, M_edit_data->samples()->dataCont().size() );
        M_origin->setWrapping( true );
        connect( M_origin, SIGNAL( valueChanged( int ) ),
                 this, SLOT( changeIndex() ) );
        layout->addWidget( M_origin, 1, 0 );
        //
        M_terminal = new QSpinBox();
        M_terminal->setRange( 1, M_edit_data->samples()->dataCont().size() );
        M_terminal->setWrapping( true );
        connect( M_terminal, SIGNAL( valueChanged( int ) ),
                 this, SLOT( changeIndex() ) );
        layout->addWidget( M_terminal, 1, 1 );

        top_layout->addLayout( layout );
    }

    QDialogButtonBox * button_box = new QDialogButtonBox( QDialogButtonBox::Ok
                                                          | QDialogButtonBox::Cancel );
    connect( button_box, SIGNAL( accepted() ), this, SLOT( accept() ) );
    connect( button_box, SIGNAL( rejected() ), this, SLOT( reject() ) );
    top_layout->addWidget( button_box );

    this->setLayout( top_layout );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConstraintEditDialog::changeIndex()
{
    M_edit_data->setConstraintIndex( M_origin->value() - 1,
                                     M_terminal->value() - 1 );
    M_edit_canvas->update();
}
