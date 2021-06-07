// -*-c++-*-

/*!
  \file coordinate_delegate.cpp
  \brief coordinate value editor delegate class Source File.
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

#include "coordinate_delegate.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

 */
CoordinateDelegate::CoordinateDelegate( const double & minimum,
                                        const double & maximum,
                                        QObject * parent )
    : QItemDelegate( parent )
    , M_minimum( minimum )
    , M_maximum( maximum )
{

}

/*-------------------------------------------------------------------*/
/*!

 */
CoordinateDelegate::~CoordinateDelegate()
{
    //std::cerr << "delete CoordinateDelegate" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
CoordinateDelegate::createEditor( QWidget * parent,
                                  const QStyleOptionViewItem & /*option*/,
                                  const QModelIndex & /*index*/ ) const
{
    QDoubleSpinBox * editor = new QDoubleSpinBox( parent );
    editor->setRange( M_minimum, M_maximum );
    editor->setDecimals( 2 );
    editor->setSingleStep( 0.01 );
    editor->setAccelerated( true );
    editor->setKeyboardTracking( false );

    return editor;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
CoordinateDelegate::setEditorData( QWidget * editor,
                                   const QModelIndex & index ) const
{
    bool ok = false;
    double value = index.model()->data( index, Qt::EditRole ).toDouble( &ok );
    if ( ! ok )
    {
        return;
    }

    QDoubleSpinBox * spin_box = static_cast< QDoubleSpinBox * >( editor );
    if ( spin_box )
    {
        spin_box->setValue( value );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
CoordinateDelegate::setModelData( QWidget * editor,
                                  QAbstractItemModel * /*model*/,
                                  const QModelIndex & index ) const
{
    QDoubleSpinBox * spin_box = static_cast< QDoubleSpinBox * >( editor );
    if ( spin_box )
    {
        spin_box->interpretText();
        double value = spin_box->value();

        //std::cerr << "CoordinateDelegate::setModelData"
        //          << "\n  index=" << index.row() << ',' << index.column()
        //          << "\n  value=" << value
        //          << std::endl;

        double old_value = index.model()->data( index, Qt::EditRole ).toDouble();
        if ( old_value != value )
        {
            emit valueChanged( index, value );
            //model->setData( index, QString::number( value, 'f', 2 ), Qt::EditRole );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
CoordinateDelegate::updateEditorGeometry( QWidget * editor,
                                          const QStyleOptionViewItem & option,
                                          const QModelIndex & /*index*/ ) const
{
    editor->setGeometry( option.rect );
}
