// -*-c++-*-

/*!
  \file coordinate_delegate.h
  \brief coordinate value editor delegate class Header File.
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

#ifndef FEDIT2_COORDINATE_DELEGATE_H
#define FEDIT2_COORDINATE_DELEGATE_H


#include <QItemDelegate>

class QModelIndex;

class CoordinateDelegate
    : public QItemDelegate {

    Q_OBJECT

private:

    const double M_minimum;
    const double M_maximum;
public:

    CoordinateDelegate( const double & mininum,
                        const double & maximum,
                        QObject * parent = 0 );
    ~CoordinateDelegate();

    virtual
    QWidget * createEditor( QWidget * parent,
                            const QStyleOptionViewItem & option,
                            const QModelIndex & index ) const;
    virtual
    void setEditorData( QWidget * editor,
                        const QModelIndex & index ) const;
    virtual
    void setModelData( QWidget * editor,
                       QAbstractItemModel * model,
                       const QModelIndex & index ) const;
    virtual
    void updateEditorGeometry( QWidget * editor,
                               const QStyleOptionViewItem & option,
                               const QModelIndex & index ) const;

signals:

    void valueChanged( const QModelIndex & index,
                       double value ) const;
};

#endif
