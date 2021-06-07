// -*-c++-*-

/*!
  \file dir_selector.h
  \brief directory selector dialog class Header File.
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

#ifndef SOCCERWINDOW2_QT_DIR_SELECTOR_H
#define SOCCERWINDOW2_QT_DIR_SELECTOR_H

#include <QDialog>
#include <QModelIndex>
#include <QString>

class QDirModel;

class DirSelector : public QDialog {
    Q_OBJECT

private:

    QModelIndex M_index;
    QDirModel * M_dir_model;

public:

    DirSelector( QWidget * parent,
                 const QString & caption = QString(),
                 const QString & dir = QString() );


    QString dirPath() const;

public slots:

    void setDirIndex( const QModelIndex & index );

};

#endif
