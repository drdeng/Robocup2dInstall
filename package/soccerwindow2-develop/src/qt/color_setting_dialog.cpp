// -*-c++-*-

/*!
  \file color_setting_dialog.cpp
  \brief color setting dialog for the field canvas Source File.
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

#include "color_setting_dialog.h"
#include "draw_config.h"

//#include <boost/bind.hpp>
#include <boost/functional.hpp>

#include <functional>
#include <iostream>
#include <cassert>

/*-------------------------------------------------------------------*/
/*!

*/
ColorEntry::ColorEntry( const QColor & old_color,
                        Func func )
    : old_color_( old_color ),
      color_( old_color ),
      func_( func )
{

}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ColorEntry::setColor( const QColor & color )
{
    if ( color_ != color )
    {
        color_ = color;

        if ( ! func_.empty() )
        {
            func_( color );
        }

        return true;
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ColorEntry::revert()
{
    return setColor( old_color_ );
}

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*!

*/
ColorSettingDialog::ColorSettingDialog( QWidget * parent )
    : QDialog( parent )
{
    this->setWindowTitle( tr( "Color Settings" ) );

    createControls();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ColorSettingDialog::createControls()
{
    QVBoxLayout * main_layout = new QVBoxLayout( this );

    main_layout->addLayout( createColorList() );

    main_layout->addSpacing( 5 );

    main_layout->addLayout( createAnswerButtons() );

    this->setLayout( main_layout );
}

/*-------------------------------------------------------------------*/
/*!

*/
ColorSettingDialog::~ColorSettingDialog()
{
    //std::cerr << "delete ColorSettingDialog" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
QLayout *
ColorSettingDialog::createColorList()
{
    QHBoxLayout * layout = new QHBoxLayout();

    M_list_box = new QListWidget();

    createColorItems();

    int font_width = this->fontMetrics().width( tr( "Debug Unknown Opponent" ) );
    M_list_box->setMinimumWidth( font_width + 16 + 10 );
    M_list_box->setMinimumHeight( 300 );
    connect( M_list_box, SIGNAL( itemDoubleClicked( QListWidgetItem * ) ),
             this, SLOT( selectEntry( QListWidgetItem * ) ) );

    layout->addWidget( M_list_box );

    QPushButton * select_btn = new QPushButton( tr( "Select" ), this );
    connect( select_btn, SIGNAL( clicked() ),
             this, SLOT( selectEntry() ) );
    layout->addWidget( select_btn );

    return layout;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ColorSettingDialog::createColorItems()
{
    DrawConfig * d = &DrawConfig::instance();

    addEntry( tr( "Score Board" ),
              d->scoreBoardBrush().color(),
              //boost::bind( &DrawConfig::setScoreBoardColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setScoreBoardColor ), d ) );
    addEntry( tr( "Score Board Font" ),
              d->scoreBoardFontPen().color(),
              //boost::bind( &DrawConfig::setScoreBoardFontColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setScoreBoardFontColor ), d ) );
    addEntry( tr( "Field" ),
              d->fieldBrush().color(),
              //boost::bind( &DrawConfig::setFieldColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setFieldColor ), d ) );
    addEntry( tr( "Field Dark" ),
              d->fieldDarkBrush().color(),
              //boost::bind( &DrawConfig::setFieldDarkColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setFieldDarkColor ), d ) );
    addEntry( tr( "Line" ),
              d->linePen().color(),
              //boost::bind( &DrawConfig::setLineColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setLineColor ), d ) );
    addEntry( tr( "Ball" ),
              d->ballBrush().color(),
              //boost::bind( &DrawConfig::setBallColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setBallColor ), d ) );
    addEntry( tr( "Measure" ),
              d->measurePen().color(),
              //boost::bind( &DrawConfig::setMeasureColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setMeasureColor ), d ) );
    addEntry( tr( "Measure Font" ),
              d->measureFontPen().color(),
              //boost::bind( &DrawConfig::setMeasureFontColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setMeasureFontColor ), d ) );
    //
    addEntry( tr( "Left Team" ),
              d->leftTeamPen().color(),
              //boost::bind( &DrawConfig::setLeftTeamColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setLeftTeamColor ), d ) );
    addEntry( tr( "Left Goalie" ),
              d->leftGoaliePen().color(),
              //boost::bind( &DrawConfig::setLeftGoalieColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setLeftGoalieColor ), d ) );
    addEntry( tr( "Right Team" ),
              d->rightTeamPen().color(),
              //boost::bind( &DrawConfig::setRightTeamColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setRightTeamColor ), d ) );
    addEntry( tr( "Right Goalie" ),
              d->rightGoaliePen().color(),
              //boost::bind( &DrawConfig::setRightGoalieColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setRightGoalieColor ), d ) );
    addEntry( tr( "Player Number Font" ),
              d->playerNumberFontPen().color(),
              //boost::bind( &DrawConfig::setPlayerNumberFontColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setPlayerNumberFontColor ), d ) );
    addEntry( tr( "Stamina Font" ),
              d->playerStaminaFontPen().color(),
              //boost::bind( &DrawConfig::setPlayerStaminaFontColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setPlayerStaminaFontColor ), d ) );
    addEntry( tr( "Player Detail Font" ),
              d->playerDetailFontPen().color(),
              //boost::bind( &DrawConfig::setPlayerDetailFontColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setPlayerDetailFontColor ), d ) );
    addEntry( tr( "Selected Player Font" ),
              d->selectedPlayerFontPen().color(),
              //boost::bind( &DrawConfig::setSelectedPlayerFontColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setSelectedPlayerFontColor ), d ) );
    addEntry( tr( "Effort Decayed" ),
              d->effortDecayedPen().color(),
              //boost::bind( &DrawConfig::setEffortDecayedColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setEffortDecayedColor ), d ) );
    addEntry( tr( "Recovery Decayed" ),
              d->recoveryDecayedPen().color(),
              //boost::bind( &DrawConfig::setRecoveryDecayedColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setRecoveryDecayedColor ), d ) );
    addEntry( tr( "View Cone" ),
              d->viewConePen().color(),
              //boost::bind( &DrawConfig::setViewConeColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setViewConeColor ), d ) );
    addEntry( tr( "View Area" ),
              d->viewAreaPen().color(),
              //boost::bind( &DrawConfig::setViewAreaColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setViewAreaColor ), d ) );
    //
    addEntry( tr( "Kick Pen" ),
              d->kickPen().color(),
              //boost::bind( &DrawConfig::setKickColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setKickColor ), d ) );
    addEntry( tr( "Kick Fault" ),
              d->kickFaultPen().color(),
              //boost::bind( &DrawConfig::setKickFaultColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setKickFaultColor ), d ) );
    addEntry( tr( "Catch" ),
              d->catchBrush().color(),
              //boost::bind( &DrawConfig::setCatchColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setCatchColor ), d ) );
    addEntry( tr( "Catch Fault" ),
              d->catchFaultBrush().color(),
              //boost::bind( &DrawConfig::setCatchFaultColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setCatchFaultColor ), d ) );
    addEntry( tr( "Ball Collision" ),
              d->ballCollisionBrush().color(),
              //boost::bind( &DrawConfig::setBallCollisionColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setBallCollisionColor ), d ) );
    addEntry( tr( "Player Collision" ),
              d->playerCollisionBrush().color(),
              //boost::bind( &DrawConfig::setPlayerCollisionColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setPlayerCollisionColor ), d ) );
    addEntry( tr( "Tackle" ),
              d->tacklePen().color(),
              //boost::bind( &DrawConfig::setTackleColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setTackleColor ), d ) );
    addEntry( tr( "Tackle Fault" ),
              d->tackleFaultBrush().color(),
              //boost::bind( &DrawConfig::setTackleFaultColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setTackleFaultColor ), d ) );
    addEntry( tr( "Foul Charged" ),
              d->foulChargedBrush().color(),
              //boost::bind( &DrawConfig::setFoulChargedColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setFoulChargedColor ), d ) );
    addEntry( tr( "PointTo" ),
              d->pointtoPen().color(),
              //boost::bind( &DrawConfig::setPointtoColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setPointtoColor ), d ) );
    addEntry( tr( "Kick Accel" ),
              d->kickAccelPen().color(),
              //boost::bind( &DrawConfig::setKickAccelColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setKickAccelColor ), d ) );
    addEntry( tr( "Tackle Area" ),
              d->tackleAreaPen().color(),
              //boost::bind( &DrawConfig::setTackleAreaColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setTackleAreaColor ), d ) );
    //
    addEntry( tr( "Debug Ball" ),
              d->debugBallBrush().color(),
              //boost::bind( &DrawConfig::setDebugBallColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugBallColor ), d ) );
    addEntry( tr( "Debug Ball Trace" ),
              d->debugBallTracePen().color(),
              //boost::bind( &DrawConfig::setDebugBallTraceColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugBallTraceColor ), d ) );
    addEntry( tr( "Debug Player Pen" ),
              d->debugPlayerPen().color(),
              //boost::bind( &DrawConfig::setDebugPlayerPenColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugPlayerPenColor ), d ) );
    addEntry( tr( "Debug Self" ),
              d->debugSelfBrush().color(),
              //boost::bind( &DrawConfig::setDebugSelfColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugSelfColor ), d ) );
    addEntry( tr( "Debug Teammate" ),
              d->debugTeammateBrush().color(),
              //boost::bind( &DrawConfig::setDebugTeammateColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugTeammateColor ), d ) );
    addEntry( tr( "Debug Opponent" ),
              d->debugOpponentBrush().color(),
              //boost::bind( &DrawConfig::setDebugOpponentColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugOpponentColor ), d ) );
    addEntry( tr( "Debug Unknown Teammate" ),
              d->debugUnknownTeammateBrush().color(),
              //boost::bind( &DrawConfig::setDebugUnknownTeammateColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugUnknownTeammateColor ), d ) );
    addEntry( tr( "Debug Unknown Opponent" ),
              d->debugUnknownOpponentBrush().color(),
              //boost::bind( &DrawConfig::setDebugUnknownOpponentColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugUnknownOpponentColor ), d ) );
    addEntry( tr( "Debug Unknown Player" ),
              d->debugUnknownPlayerBrush().color(),
              //boost::bind( &DrawConfig::setDebugUnknownPlayerColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugUnknownPlayerColor ), d ) );

    addEntry( tr( "Debug Pointto" ),
              d->debugPointtoPen().color(),
              //boost::bind( &DrawConfig::setDebugPointtoColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugPointtoColor ), d ) );
    addEntry( tr( "Debug Target" ),
              d->debugTargetPen().color(),
              //boost::bind( &DrawConfig::setDebugTargetColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugTargetColor ), d ) );
    addEntry( tr( "Debug Comment" ),
              d->debugCommentFontPen().color(),
              //boost::bind( &DrawConfig::setDebugCommentColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugCommentColor ), d ) );
    addEntry( tr( "Debug Message" ),
              d->debugMessageFontPen().color(),
              //boost::bind( &DrawConfig::setDebugMessageColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugMessageColor ), d ) );
    addEntry( tr( "Debug Log Font" ),
              d->debugLogMessageFontPen().color(),
              //boost::bind( &DrawConfig::setDebugLogMessageColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugLogMessageColor ), d ) );
    addEntry( tr( "Debug Shapes" ),
              d->debugShapePen().color(),
              //boost::bind( &DrawConfig::setDebugShapeColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugShapeColor ), d ) );
    addEntry( tr( "Debug Action Sequence" ),
              d->debugActionSequencePen().color(),
              //boost::bind( &DrawConfig::setDebugActionSequenceColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugActionSequenceColor ), d ) );
    addEntry( tr( "Debug Action Pass" ),
              d->debugActionPassPen().color(),
              //boost::bind( &DrawConfig::setDebugActionPassColor, d, _1 ) );
              boost::bind1st( std::mem_fun( &DrawConfig::setDebugActionPassColor ), d ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ColorSettingDialog::addEntry( const QString & name,
                              const QColor & old_color,
                              ColorEntry::Func func )
{
    QPixmap pixmap( 16, 16 );
    pixmap.fill( old_color );

    M_list_box->addItem( new QListWidgetItem( QIcon( pixmap ), name ) );

    M_color_entries.push_back( ColorEntry( old_color, func ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
QLayout *
ColorSettingDialog::createAnswerButtons()
{
    QHBoxLayout * layout = new QHBoxLayout();

    QFontMetrics fm = this->fontMetrics();
    int text_width = fm.width( tr( "Cancel" ) ) + 8;

    QPushButton * def = new QPushButton( tr( "Default" ) );
    //cancel->resize( text_width, fm.height() );
    connect( def, SIGNAL( clicked() ),
             this, SLOT( setDefaultColors() ) );

    QPushButton * ok = new QPushButton( tr( "OK" ) );
    ok->resize( text_width, fm.height() );
    connect( ok, SIGNAL( clicked() ),
             this, SLOT( accept() ) );

    QPushButton * cancel = new QPushButton( tr( "Cancel" ) );
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
ColorSettingDialog::revert()
{
    bool changed = false;

    for ( int i = 0; i < M_list_box->count(); ++i )
    {
        if ( M_color_entries[i].revert() )
        {
            QListWidgetItem * item = M_list_box->item( i );
            QPixmap pixmap( 16, 16 );
            pixmap.fill( M_color_entries[i].color_ );
            item->setIcon( QIcon( pixmap ) );
            changed = true;
        }
    }

    if ( changed )
    {
        emit colorChanged();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ColorSettingDialog::setDefaultColors()
{
    DrawConfig::instance().setDefaultColors();

    M_list_box->clearSelection();
    M_list_box->clear();

    createColorItems();

//     bool changed = false;
//     for ( int i = 0; i < M_list_box->count(); ++i )
//     {
//         if ( M_color_entries[i].revert() )
//         {
//             QListWidgetItem * item = M_list_box->item( i );
//             QPixmap pixmap( 16, 16 );
//             pixmap.fill( M_color_entries[i].color_ );
//             item->setIcon( QIcon( pixmap ) );
//             changed = true;
//         }
//     }

//     if ( changed )
    {
        emit colorChanged();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ColorSettingDialog::selectEntry( int index )
{
    if ( index < 0 )
    {
        return;
    }

    if ( index >= static_cast< int >( M_color_entries.size() ) )
    {
        std::cerr << __FILE__ << ": (selectEntry) Illegal index "
                  << index << std::endl;
        return;
    }

    QColor col = QColorDialog::getColor( M_color_entries[index].color_ );
    if ( ! col.isValid() )
    {
        return;
    }

    if ( M_color_entries[index].setColor( col ) )
    {
        QListWidgetItem * item = M_list_box->item( index );
        QPixmap pixmap( 16, 16 );
        pixmap.fill( col );
        item->setIcon( QIcon( pixmap ) );

        emit colorChanged();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ColorSettingDialog::selectEntry()
{
    selectEntry( M_list_box->currentRow() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ColorSettingDialog::selectEntry( QListWidgetItem * item )
{
    selectEntry( M_list_box->row( item ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ColorSettingDialog::reject()
{
    // reset all changes

    revert();

    QDialog::reject();
}
