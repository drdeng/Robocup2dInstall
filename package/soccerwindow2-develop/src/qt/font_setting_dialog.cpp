// -*-c++-*-

/*!
  \file font_setting_dialog.cpp
  \brief font setting dialog for the field canvas Source File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa Akiyama

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

#include "font_setting_dialog.h"
#include "draw_config.h"

//#include <boost/bind.hpp>
#include <boost/functional.hpp>

#include <iostream>
#include <cassert>

/*-------------------------------------------------------------------*/
/*!

*/
FontButton::FontButton( const QString & name,
                        const QFont & old_font,
                        Func func,
                        FontSettingDialog * parent )
    : QPushButton( parent )
    , M_name( name )
    , M_old_font( old_font )
    , M_func( func )
{
    this->setFont( old_font );

    updateText();

    connect( this, SIGNAL( clicked() ),
             this, SLOT( fontDialog() ) );
    connect( this, SIGNAL( fontChanged() ),
             parent, SIGNAL( fontChanged() ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
FontButton::setNewFont( const QFont & font )
{
    if ( this->font() != font )
    {
        if ( ! M_func.empty() )
        {
            M_func( font );
        }
        this->setFont( font );

        updateText();

        emit fontChanged();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
FontButton::fontDialog()
{
    bool ok = true;
    QFont font = QFontDialog::getFont( &ok, this->font() );
    if ( ok )
    {
        setNewFont( font );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
FontButton::updateText()
{
    QString str = M_name;
    str += tr( " (" );
    str += this->font().family();
    str += tr( "," );
    str += QString::number( this->font().pointSize() );
    str += tr( ")" );
    this->setText( str );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
FontButton::revert()
{
    setNewFont( M_old_font );
}

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*!

*/
FontSettingDialog::FontSettingDialog( QWidget * parent )
    : QDialog( parent )
{

    this->setWindowTitle( tr( "Font Settings" ) );

    createControls();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
FontSettingDialog::createControls()
{
    QVBoxLayout * main_layout = new QVBoxLayout( this );
    main_layout->setSizeConstraint( QLayout::SetFixedSize );

    main_layout->addLayout( createFontButtons() );

    main_layout->addLayout( createAnswerButtons() );
}

/*-------------------------------------------------------------------*/
/*!

*/
FontSettingDialog::~FontSettingDialog()
{
    //std::cerr << "delete FontSettingDialog" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
QLayout *
FontSettingDialog::createFontButtons()
{
    QVBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins( 4, 4, 4, 4 );
    layout->setSpacing( 2 );

    DrawConfig * d = &DrawConfig::instance();

    {
        FontButton * btn = new FontButton( tr( "Score Board" ),
                                           d->scoreBoardFont(),
                                           //boost::bind( &DrawConfig::setScoreBoardFont, d, _1 ),
                                           boost::bind1st( std::mem_fun( &DrawConfig::setScoreBoardFont ), d ),
                                           this );
        M_font_buttons.push_back( btn );
        layout->addWidget( btn, 1, Qt::AlignLeft );
    }
    {
        FontButton * btn = new FontButton( tr( "Ball" ),
                                           d->ballFont(),
                                           //boost::bind( &DrawConfig::setBallFont, d, _1 ),
                                           boost::bind1st( std::mem_fun( &DrawConfig::setBallFont ), d ),
                                           this );
        M_font_buttons.push_back( btn );
        layout->addWidget( btn, 1, Qt::AlignLeft );
    }
    {
        FontButton * btn = new FontButton( tr( "Player" ),
                                           d->playerFont(),
                                           //boost::bind( &DrawConfig::setPlayerFont, d, _1 ),
                                           boost::bind1st( std::mem_fun( &DrawConfig::setPlayerFont ), d ),
                                           this );
        M_font_buttons.push_back( btn );
        layout->addWidget( btn, 1, Qt::AlignLeft );
    }
    {
        FontButton * btn = new FontButton( tr( "Measure" ),
                                           d->measureFont(),
                                           //boost::bind( &DrawConfig::setMeasureFont, d, _1 ),
                                           boost::bind1st( std::mem_fun( &DrawConfig::setMeasureFont ), d ),
                                           this );
        M_font_buttons.push_back( btn );
        layout->addWidget( btn, 1, Qt::AlignLeft );
    }
    {
        FontButton * btn = new FontButton( tr( "Debug Comment" ),
                                           d->debugCommentFont(),
                                           //boost::bind( &DrawConfig::setDebugCommentFont, d, _1 ),
                                           boost::bind1st( std::mem_fun( &DrawConfig::setDebugCommentFont ), d ),
                                           this );
        M_font_buttons.push_back( btn );
        layout->addWidget( btn, 1, Qt::AlignLeft );
    }
    {
        FontButton * btn = new FontButton( tr( "Debug Message" ),
                                           d->debugMessageFont(),
                                           //boost::bind( &DrawConfig::setDebugMessageFont, d, _1 ),
                                           boost::bind1st( std::mem_fun( &DrawConfig::setDebugMessageFont ), d ),
                                           this );
        M_font_buttons.push_back( btn );
        layout->addWidget( btn, 1, Qt::AlignLeft );
    }
    {
        FontButton * btn = new FontButton( tr( "Debug Log" ),
                                           d->debugLogMessageFont(),
                                           //boost::bind( &DrawConfig::setDebugLogMessageFont, d, _1 ),
                                           boost::bind1st( std::mem_fun( &DrawConfig::setDebugLogMessageFont ), d ),
                                           this );
        M_font_buttons.push_back( btn );
        layout->addWidget( btn, 1, Qt::AlignLeft );
    }

    return layout;
}

/*-------------------------------------------------------------------*/
/*!

*/
QLayout *
FontSettingDialog::createAnswerButtons()
{
    QHBoxLayout * layout = new QHBoxLayout();

    QFontMetrics fm = this->fontMetrics();
    int text_width = fm.width( tr( "Cancel" ) ) + 8;

    QPushButton * def = new QPushButton( tr( "Default" ), this );
    //cancel->resize( text_width, fm.height() );
    connect( def, SIGNAL( clicked() ),
             this, SLOT( setDefaultFonts() ) );

    QPushButton * ok = new QPushButton( tr( "OK" ), this );
    ok->resize( text_width, fm.height() );
    connect( ok, SIGNAL( clicked() ),
             this, SLOT( accept() ) );

    QPushButton * cancel = new QPushButton( tr( "Cancel" ), this );
    cancel->resize( text_width, fm.height() );
    connect( cancel, SIGNAL( clicked() ),
             this, SLOT( reject() ) );


    layout->addWidget( def, 0, Qt::AlignLeft );
    layout->addStretch( 1 );
    layout->addWidget( ok, 0, Qt::AlignRight );
    layout->addWidget( cancel, 0, Qt::AlignRight );

    return layout;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
FontSettingDialog::revert()
{
    for ( std::vector< FontButton * >::iterator it = M_font_buttons.begin();
          it != M_font_buttons.end();
          ++it )
    {
        (*it)->revert();
    }

    emit fontChanged();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
FontSettingDialog::setDefaultFonts()
{
    DrawConfig::instance().setDefaultFonts();

//     for ( std::vector< FontButton * >::iterator it = M_font_buttons.begin();
//           it != M_font_buttons.end();
//           ++it )
//     {
//         (*it)->updateText();
//     }

    emit fontChanged();

    accept();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
FontSettingDialog::reject()
{
    // reset all changes

    revert();

    QDialog::reject();
}
