// -*-c++-*-

/*!
  \file launcher_dialog.cpp
  \brief Launcher Dialog class Source File.
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

#include "launcher_dialog.h"

#include "options.h"

#include <iostream>
#include <cassert>
#include <cstdlib>

const int LauncherDialog::MAX_HISTORY = 20;

/*-------------------------------------------------------------------*/
/*!

*/
LauncherDialog::LauncherDialog( QWidget * parent )
    : QDialog( parent )
{
    assert( parent );

    this->setWindowTitle( tr( "Launcher" ) );

    createControls();

    readSettings();
}

/*-------------------------------------------------------------------*/
/*!

*/
LauncherDialog::~LauncherDialog()
{
    saveSettings();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LauncherDialog::readSettings()
{
#ifndef Q_WS_WIN
    QSettings settings( QDir::homePath() + "/.soccerwindow2",
                        QSettings::IniFormat );
#else
    QSettings settings( QDir::currentPath() + "/soccerwindow2.ini",
                        QSettings::IniFormat );
#endif

    settings.beginGroup( "LauncherDialog" );

    for ( int i = 0; i < MAX_HISTORY; ++i )
    {
        QVariant val = settings.value( QString( "LeftStart%1" ).arg( i ) );
        if ( ! val.isValid() )
        {
            break;
        }
        M_left_team_commands->addItem( val.toString() );
    }

    for ( int i = 0; i < MAX_HISTORY; ++i )
    {
        QVariant val = settings.value( QString( "RightStart%1" ).arg( i ) );
        if ( ! val.isValid() )
        {
            break;
        }
        M_right_team_commands->addItem( val.toString() );
    }

    settings.endGroup();

}

/*-------------------------------------------------------------------*/
/*!

*/
void
LauncherDialog::saveSettings()
{
#ifndef Q_WS_WIN
    QSettings settings( QDir::homePath() + "/.soccerwindow2",
                        QSettings::IniFormat );
#else
    QSettings settings( QDir::currentPath() + "/soccerwindow2.ini",
                        QSettings::IniFormat );
#endif

    settings.beginGroup( "LauncherDialog" );

    {
        const int count = M_left_team_commands->count();
        for ( int i = 0; i < count; ++i )
        {
            QString name = QString( "LeftStart%1" ).arg( i );
            settings.setValue( name, M_left_team_commands->itemText( i ) );
        }
    }

    {
        const int count = M_right_team_commands->count();
        for ( int i = 0; i < count; ++i )
        {
            QString name = QString( "RightStart%1" ).arg( i );
            settings.setValue( name, M_right_team_commands->itemText( i ) );
        }
    }

    settings.endGroup();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LauncherDialog::createControls()
{
    QVBoxLayout * layout = new QVBoxLayout( this );
    layout->setContentsMargins( 4, 4, 4, 4 );

    //layout->addLayout( createServerControls(), 0 );
    layout->addWidget( createServerControls(), 0 );
    layout->addWidget( createLeftTeamControls(), 0 );
    layout->addWidget( createRightTeamControls(), 0 );

    layout->addSpacing( 2 );

    layout->addWidget( createExecuteControls(), 0 );
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
LauncherDialog::createServerControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "rcssserver" ) );
    QHBoxLayout * layout = new QHBoxLayout( group_box );

//     QLabel * label = new QLabel( tr( "Command: " ) );
//     label->setMinimumSize( 80,  this->fontMetrics().height() + 12 );
//     label->setMaximumSize( 100, this->fontMetrics().height() + 12 );
//     layout->addWidget( label );

    QString sserver_path = Options::instance().serverPath().c_str();

    M_rcssserver_command = new QLineEdit( sserver_path );
    M_rcssserver_command->setMinimumSize( 240, this->fontMetrics().height() + 12 );
    //M_rcssserver_command->setMaximumSize( 240, this->fontMetrics().height() + 12 );
    layout->addWidget( M_rcssserver_command );

    QPushButton * execute_btn = new QPushButton( tr( "Start!" ) );
    execute_btn->setMinimumSize( 80, this->fontMetrics().height() + 12 );
    execute_btn->setMaximumSize( 80, this->fontMetrics().height() + 12 );
    execute_btn->setAutoDefault( false );
    connect( execute_btn, SIGNAL( clicked() ),
             this, SLOT( startServer() ) );
    layout->addWidget( execute_btn );

    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
LauncherDialog::createLeftTeamControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Left Team" ) );
    QHBoxLayout * layout = new QHBoxLayout( group_box );

//     QLabel * label = new QLabel( tr( "Command: " ) );
//     label->setMinimumSize( 80,  this->fontMetrics().height() + 12 );
//     label->setMaximumSize( 100, this->fontMetrics().height() + 12 );
//     layout->addWidget( label );

    M_left_team_commands = new QComboBox( group_box );
    M_left_team_commands->setEditable( true );
    M_left_team_commands->setAutoCompletion( true );
    M_left_team_commands->setMaxCount( MAX_HISTORY );
    M_left_team_commands->setDuplicatesEnabled( false );
    M_left_team_commands->setMinimumSize( 240, this->fontMetrics().height() + 12 );
    layout->addWidget( M_left_team_commands );

    QPushButton * execute_btn = new QPushButton( tr( "Start!" ), group_box );
    execute_btn->setMinimumSize( 80, this->fontMetrics().height() + 12 );
    execute_btn->setMaximumSize( 80, this->fontMetrics().height() + 12 );
    execute_btn->setAutoDefault( false );
    connect( execute_btn, SIGNAL( clicked() ),
             this, SLOT( startLeftTeam() ) );
    layout->addWidget( execute_btn );

    return group_box;;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
LauncherDialog::createRightTeamControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Right Team" ) );
    QHBoxLayout * layout = new QHBoxLayout( group_box );

//     QLabel * label = new QLabel( tr( "Command: " ) );
//     label->setMinimumSize( 80,  this->fontMetrics().height() + 12 );
//     label->setMaximumSize( 100, this->fontMetrics().height() + 12 );
//     layout->addWidget( label );

    M_right_team_commands = new QComboBox( group_box );
    M_right_team_commands->setEditable( true );
    M_right_team_commands->setAutoCompletion( true );
    M_right_team_commands->setMaxCount( MAX_HISTORY );
    M_right_team_commands->setDuplicatesEnabled( false );
    M_right_team_commands->setMinimumSize( 240, this->fontMetrics().height() + 12 );
    layout->addWidget( M_right_team_commands );

    QPushButton * execute_btn = new QPushButton( tr( "Start!" ), group_box );
    execute_btn->setMinimumSize( 80, this->fontMetrics().height() + 12 );
    execute_btn->setMaximumSize( 80, this->fontMetrics().height() + 12 );
    execute_btn->setAutoDefault( false );
    connect( execute_btn, SIGNAL( clicked() ),
             this, SLOT( startRightTeam() ) );
    layout->addWidget( execute_btn );

    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
LauncherDialog::createExecuteControls()
{
    QFrame * frame = new QFrame();
    QHBoxLayout * layout = new QHBoxLayout( frame );

    QPushButton * button = new QPushButton( tr( "Start All!" ), this );
    button->setAutoDefault( false );
    connect( button, SIGNAL( clicked() ),
             this, SLOT( startAll() ) );
    layout->addWidget( button, 10, Qt::AlignVCenter );

    QPushButton * cancel = new QPushButton( tr( "Close" ), this );
    cancel->setAutoDefault( false );
    connect( cancel, SIGNAL( clicked() ),
             this, SLOT( reject() ) );
    layout->addWidget( cancel, 2, Qt::AlignVCenter );

    return frame;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LauncherDialog::updateComboBox( QComboBox * combo_box )
{
    QString cur_text = combo_box->lineEdit()->text();
    if ( cur_text.isEmpty() )
    {
        return;
    }

    int index = combo_box->findText( cur_text );

    if ( index >= 0 )
    {
        for ( int i = index - 1; i >= 0; --i )
        {
            QString tmp = combo_box->itemText( i );
            combo_box->setItemText( i + 1, tmp );
        }
        combo_box->setItemText( 0, cur_text );
    }
    else
    {
        //combo_box->addItem( cur_text );
        combo_box->insertItem( 0, cur_text );
    }

    combo_box->setCurrentIndex( 0 );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LauncherDialog::startServer()
{
    QString command = M_rcssserver_command->text();

    std::cerr << "start the server [" << command.toStdString() << "]" << std::endl;
    emit launchServer( command );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LauncherDialog::startLeftTeam()
{
    //std::cerr << "start left team" << std::endl;

    updateComboBox( M_left_team_commands );

    QString command = M_left_team_commands->currentText();
    if ( command.isEmpty() )
    {
        return;
    }

    QProcess::startDetached( command );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LauncherDialog::startRightTeam()
{
    //std::cerr << "start right team" << std::endl;

    updateComboBox( M_right_team_commands );

    QString command = M_right_team_commands->currentText();
    if ( command.isEmpty() )
    {
        return;
    }

    QProcess::startDetached( command );

    this->hide();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LauncherDialog::startAll()
{
    updateComboBox( M_left_team_commands );
    updateComboBox( M_right_team_commands );

    QString command = M_rcssserver_command->text();

    QString left_command = M_left_team_commands->currentText();
    QString right_command = M_right_team_commands->currentText();

    if ( ! left_command.isEmpty() )
    {
        command += " server::team_l_start = \'";
        command += left_command;
        command += "\'";
    }

    if ( ! right_command.isEmpty() )
    {
        command += " server::team_r_start = \'";
        command += right_command;
        command += "\'";
    }

    std::cerr << "start all [" << command.toStdString() << "]" << std::endl;
    emit launchServer( command );

    this->hide();
}
