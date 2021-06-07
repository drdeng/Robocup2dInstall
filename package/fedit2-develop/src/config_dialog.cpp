// -*-c++-*-

/*!
  \file config_dialog.cpp
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

#include "config_dialog.h"

#include "options.h"

#include <iostream>
#include <cassert>
#include <cmath>

/*-------------------------------------------------------------------*/
/*!

 */
ConfigDialog::ConfigDialog( QWidget * parent )
    : QDialog( parent )
{
    this->setWindowTitle( tr( "Config Dialog" ) );
    createWidgets();
}

/*-------------------------------------------------------------------*/
/*!

 */
ConfigDialog::~ConfigDialog()
{
    //std::cerr << "delete ConfigDialog" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConfigDialog::createWidgets()
{
    QVBoxLayout * layout = new QVBoxLayout();
    layout->setSizeConstraint( QLayout::SetFixedSize );
    layout->setContentsMargins( 4, 4, 4, 4 );
    layout->setSpacing( 4 );

    layout->addWidget( createViewSizeControls(),
                       0, Qt::AlignLeft );
    layout->addWidget( createViewScaleControls(),
                       0, Qt::AlignLeft );

    this->setLayout( layout );
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ConfigDialog::createViewSizeControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "View Size" ) );

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins( 1, 1, 1, 1 );
    layout->setSpacing( 0 );

    layout->addWidget( new QLabel( tr( " Width:" ) ) );

    M_view_width_text = new QLineEdit( tr( "640" ) );
    M_view_width_text->setValidator( new QIntValidator( 100, 3000, M_view_width_text ) );
    M_view_width_text->setMaximumSize( 48, 24 );
    layout->addWidget( M_view_width_text );

    layout->addWidget( new QLabel( tr( " Height:" ) ) );

    M_view_height_text = new QLineEdit( tr( "480" ) );
    M_view_height_text->setValidator( new QIntValidator( 100, 3000, M_view_height_text ) );
    M_view_height_text->setMaximumSize( 48, 24 );
    layout->addWidget( M_view_height_text );

    layout->addSpacing( 12 );

    QPushButton * apply_view_size_btn = new QPushButton( tr( "apply" ) );
    apply_view_size_btn->setMaximumSize( 60, this->fontMetrics().height() + 12 );
    connect( apply_view_size_btn, SIGNAL( clicked() ),
             this, SLOT( applyViewSize() ) );
    layout->addWidget( apply_view_size_btn );

    group_box->setLayout( layout );
    return group_box;
}


/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
ConfigDialog::createViewScaleControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "View Scale" ) );

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins( 1, 1, 1, 1 );
    layout->setSpacing( 0 );

    layout->addWidget( new QLabel( tr( " Scale:" ) ) );

    M_scale_text = new QLineEdit( tr( "0.0" ) );
    M_scale_text->setValidator( new QDoubleValidator( 0.0, 400.0, 3, M_scale_text ) );
    M_scale_text->setMaximumSize( 48, 24 );
    connect( M_scale_text, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( editViewScale( const QString & ) ) );
    layout->addWidget( M_scale_text );

    group_box->setLayout( layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConfigDialog::showEvent( QShowEvent * event )
{
    updateAll();
    event->accept();
    emit shown( true );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::closeEvent( QCloseEvent * event )
{
    event->accept();
    emit shown( false );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConfigDialog::updateAll()
{
    const Options & opt = Options::instance();

    M_view_width_text->setText( QString::number( opt.viewWidth() ) );
    M_view_height_text->setText( QString::number( opt.viewHeight() ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConfigDialog::applyViewSize()
{
    bool ok_w = true;
    bool ok_h = true;
    int width = M_view_width_text->text().toInt( &ok_w );
    int height = M_view_height_text->text().toInt( &ok_h );

    if ( ok_w
         && ok_h
         && width > 0
         && height > 0 )
    {
        emit viewResizeApplied( QSize( width, height ) );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ConfigDialog::editViewScale( const QString & text )
{
    bool ok = true;
    double value = text.toDouble( &ok );

    if ( ok
         && std::fabs( value - Options::instance().viewScale() ) >= 0.01 )
    {
        Options::instance().setAutoFitMode( false );
        Options::instance().setViewScale( value );

        emit configured();
    }
}
