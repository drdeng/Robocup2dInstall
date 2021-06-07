// -*-c++-*-

/*!
  \file dir_selector.cpp
  \brief directory selector dialog class Source File.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "dir_selector.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

*/
DirSelector::DirSelector( QWidget * parent,
                          const QString & caption,
                          const QString & dir )

    : QDialog( parent, Qt::Dialog )
{
    M_dir_model = new QDirModel( this );
    M_dir_model->setFilter( QDir::Dirs );

    // create tree view for the directry hierarchy
    if ( dir.isEmpty() )
    {
        M_index = M_dir_model->index( QDir::currentPath() );
    }
    else
    {
        M_index = M_dir_model->index( dir );
    }

    QTreeView * tree = new QTreeView;
    tree->setModel( M_dir_model );

    int column_size = M_dir_model->columnCount( QModelIndex() );
    for ( int i = 1; i < column_size; ++i )
    {
        tree->setColumnHidden( i, true );
    }

    tree->header()->hide();
    tree->expand( M_dir_model->index( 0, 0 ) );
    tree->setCurrentIndex( M_index );

    connect( tree, SIGNAL( clicked( const QModelIndex & ) ),
             this, SLOT( setDirIndex( const QModelIndex & ) ) );

    // create buttons

    QPushButton * ok = new QPushButton( tr( "OK" ) );
    connect( ok, SIGNAL( clicked() ),
             this, SLOT( accept() ) );

    QPushButton * cancel = new QPushButton( tr( "Cancel" ) );
    connect( cancel, SIGNAL( clicked() ),
             this, SLOT( reject() ) );

    QFontMetrics fm = this->fontMetrics();

    int text_width = fm.width( tr( "Cancel" ) ) + 8;

    ok->resize( text_width, fm.height() );
    cancel->resize( text_width, fm.height() );


    // set layout

    QVBoxLayout * main_layout = new QVBoxLayout;

    main_layout->addWidget( tree );

    QHBoxLayout * btn_layout = new QHBoxLayout;

    btn_layout->addStretch( 1 );
    btn_layout->addWidget( ok, 0, Qt::AlignRight );
    btn_layout->addWidget( cancel, 0, Qt::AlignRight );

    main_layout->addLayout( btn_layout );

    this->setLayout( main_layout );

    if ( caption.isEmpty() )
    {
        this->setWindowTitle( tr( "Dir Selector" ) );
    }
    else
    {
        this->setWindowTitle( caption );
    }

    this->resize( 320, 320 );
}


/*-------------------------------------------------------------------*/
/*!

*/
QString
DirSelector::dirPath() const
{
    if ( M_index.isValid() )
    {
        return M_dir_model->filePath( M_index );
    }

    return QString();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DirSelector::setDirIndex( const QModelIndex & index )
{
    M_index = index;
}
