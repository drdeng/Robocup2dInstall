// -*-c++-*-

/*!
  \file constraint_delegate.cpp
  \brief constraint index editor delegate class Source File.
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

#include "constraint_delegate.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

 */
ConstraintDelegate::ConstraintDelegate( QObject * parent )
    : QItemDelegate( parent )
    , M_minimum( 0 )
    , M_maximum( 0 )
{

}

/*-------------------------------------------------------------------*/
/*!

 */
ConstraintDelegate::~ConstraintDelegate()
{
    //std::cerr << "delete ConstraintDelegate" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConstraintDelegate::setRange( const int minimum,
                              const int maximum )
{
    M_minimum = minimum;
    M_maximum = maximum;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ConstraintDelegate::createEditor( QWidget * parent,
                                  const QStyleOptionViewItem & /*option*/,
                                  const QModelIndex & /*index*/ ) const
{
    QSpinBox * editor = new QSpinBox( parent );
    editor->setRange( M_minimum, M_maximum );
    editor->setSingleStep( 1 );
    editor->setAccelerated( true );
    editor->setKeyboardTracking( false );
    editor->setAccelerated( true );
    editor->setWrapping( true );

    return editor;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConstraintDelegate::setEditorData( QWidget * editor,
                                   const QModelIndex & index ) const
{
    bool ok = false;
    int value = index.model()->data( index, Qt::EditRole ).toInt( &ok );
    if ( ! ok )
    {
        return;
    }

    QSpinBox * spin_box = static_cast< QSpinBox * >( editor );
    if ( spin_box )
    {
        spin_box->setValue( value );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConstraintDelegate::setModelData( QWidget * editor,
                                  QAbstractItemModel * /*model*/,
                                  const QModelIndex & index ) const
{
    QSpinBox * spin_box = static_cast< QSpinBox * >( editor );
    if ( spin_box )
    {
        spin_box->interpretText();
        int value = spin_box->value();

        //std::cerr << "ConstraintDelegate::setModelData"
        //          << "\n  index=" << index.row() << ',' << index.column()
        //          << "\n  value=" << value
        //          << std::endl;

        int old_value = index.model()->data( index, Qt::EditRole ).toInt();
        if ( old_value != value )
        {
            emit valueChanged( index, value );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConstraintDelegate::updateEditorGeometry( QWidget * editor,
                                          const QStyleOptionViewItem & option,
                                          const QModelIndex & /*index*/ ) const
{
    editor->setGeometry( option.rect );
}
