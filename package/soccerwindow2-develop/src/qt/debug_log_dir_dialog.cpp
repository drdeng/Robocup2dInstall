// -*-c++-*-

/*!
  \file debug_log_dir_dialog.cpp
  \brief View Config Dialog class Source File.
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

#include "debug_log_dir_dialog.h"
#include "dir_selector.h"

#include "options.h"

#include <iostream>
#include <cassert>

/*-------------------------------------------------------------------*/
/*!

*/
DebugLogDirDialog::DebugLogDirDialog( QWidget * parent )
    : QDialog( parent )
{

    this->setWindowTitle( tr( "Select Log Directory" ) );

    createControls();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogDirDialog::createControls()
{
    QVBoxLayout * main_layout = new QVBoxLayout();

    main_layout->addWidget( createSideSelectionBox() );

    main_layout->addWidget( createDirSelectionBox() );

    main_layout->addLayout( createAnswerButtons() );

    this->setLayout( main_layout );
}

/*-------------------------------------------------------------------*/
/*!

*/
DebugLogDirDialog::~DebugLogDirDialog()
{
    //std::cerr << "delete DebugLogDirDialog" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
rcsc::SideID
DebugLogDirDialog::side() const
{
    if ( M_left_rb->isChecked() )
    {
        return rcsc::LEFT;
    }

    if ( M_right_rb->isChecked() )
    {
        return rcsc::RIGHT;
    }

    return rcsc::LEFT;
}

/*-------------------------------------------------------------------*/
/*!

*/
QString
DebugLogDirDialog::dirPath() const
{
    if ( ! M_log_dir )
    {
        return QString();
    }

    return M_log_dir->text();
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
DebugLogDirDialog::createSideSelectionBox()
{
    // create side selection buttons
    QGroupBox * selection_box = new QGroupBox( tr( "Side Selection" ) );

    QHBoxLayout * layout = new QHBoxLayout();

    M_left_rb = new QRadioButton( tr( "Left" ) );
    M_left_rb->setChecked( true );
    layout->addWidget( M_left_rb );

    M_right_rb = new QRadioButton( tr( "Right" ) );
    layout->addWidget( M_right_rb );

    layout->addStretch( 1 );

    selection_box->setLayout( layout );

    return selection_box;
}


/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
DebugLogDirDialog::createDirSelectionBox()
{
    // create dir selection line edit
    QGroupBox * dir_box = new QGroupBox( tr( "Log Directry" ) );

    QHBoxLayout * layout = new QHBoxLayout();

    QString dir_str;
    if ( Options::instance().debugLogDir().empty() )
    {
        dir_str = QDir::tempPath();
    }
    else
    {
        dir_str = QString::fromStdString( Options::instance().debugLogDir() );
    }

    M_log_dir = new QLineEdit( dir_str );
    M_log_dir->setMinimumWidth( qMax( 240,
                                      M_log_dir->fontMetrics().width( dir_str ) + 32 ) );

    layout->addWidget( M_log_dir );

    QPushButton * button = new QPushButton( tr( "..." ) );
    button->setMaximumSize( this->fontMetrics().width( tr( "..." ) ) + 6,
                            this->fontMetrics().height() + 12 );
    button->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    connect( button, SIGNAL( clicked() ),
             this, SLOT( clickDirSelectionButton() ) );
    layout->addWidget( button );

    dir_box->setLayout( layout );

    return dir_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
QLayout *
DebugLogDirDialog::createAnswerButtons()
{
    QHBoxLayout * layout = new QHBoxLayout();

    QFontMetrics fm = this->fontMetrics();
    int text_width = fm.width( tr( "Cancel" ) ) + 8;

    QPushButton * ok = new QPushButton( tr( "OK" ) );
    ok->resize( text_width, fm.height() );
    ok->setDefault( true );
    connect( ok, SIGNAL( clicked() ),
             this, SLOT( accept() ) );

    QPushButton * cancel = new QPushButton( tr( "Cancel" ) );
    cancel->resize( text_width, fm.height() );
    connect( cancel, SIGNAL( clicked() ),
             this, SLOT( reject() ) );


    layout->addStretch( 1 );
    layout->addWidget( ok, 0, Qt::AlignRight );
    layout->addWidget( cancel, 0, Qt::AlignRight );

    return layout;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogDirDialog::clickDirSelectionButton()
{
    DirSelector selector( this,
                          tr( "Choose a log directory" ),
                          M_log_dir->text() );  // default path

    if ( ! selector.exec() )
    {
        return;
    }

    M_log_dir->setText( selector.dirPath() );
}
