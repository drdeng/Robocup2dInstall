// -*-c++-*-

/*!
  \file constraint_delegate.h
  \brief constraint index editor delegate class Header File.
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

#ifndef FEDIT2_CONSTRAINT_DELEGATE_H
#define FEDIT2_CONSTRAINT_DELEGATE_H


#include <QItemDelegate>

class QModelIndex;

class ConstraintDelegate
    : public QItemDelegate {

    Q_OBJECT

private:

    int M_minimum;
    int M_maximum;
public:

    ConstraintDelegate( QObject * parent = 0 );
    ~ConstraintDelegate();

    void setRange( const int minimum,
                   const int maximum );

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
                       int value ) const;
};

#endif
