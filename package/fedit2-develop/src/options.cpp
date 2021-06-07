// -*-c++-*-

/*!
  \file options.cpp
  \brief global configuration class Source File.
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

#include <QSettings>
#include <QDir>

#include "options.h"

#include <rcsc/param/param_map.h>
#include <rcsc/param/cmd_line_parser.h>

#include <iostream>
#include <cmath>
#include <cstdio>

/*-------------------------------------------------------------------*/
/*!

*/
Options &
Options::instance()
{
    static Options s_instance;
    return s_instance;
}

/*-------------------------------------------------------------------*/
/*!

*/
Options::Options()
    : M_maximize( false ),
      M_full_screen( false ),
      M_window_width( 0 ),
      M_window_height( 0 ),
      M_window_pos_x( -1 ),
      M_window_pos_y( -1 ),
      M_view_width( 0 ),
      M_view_height( 0 ),
      M_focus_point( 0.0, 0.0 ),
      M_view_scale( 1.0 ),
      M_conf_file(),
      M_data_file(),
      M_background_conf_file(),
      M_auto_backup( true ),
      //
      M_player_auto_move( true ),
      M_data_auto_select( true ),
      M_symmetry_mode( true ),
      M_constraint_edit_mode( false ),
      //
      M_show_background_data( true ),
      M_enlarge( true ),
      M_show_index( true ),
      M_show_triangulation( true ),
      M_show_circumcircle( false ),
      M_show_shoot_lines( false ),
      M_antialiasing( false ),
      M_auto_fit_mode( true )
{
    readSettings();
}

/*-------------------------------------------------------------------*/
/*!

*/
Options::~Options()
{
    writeSettings();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
Options::readSettings()
{
#ifdef Q_WS_WIN
    QSettings settings( QDir::currentPath() + QObject::tr( "/fedit2.ini" ),
                        QSettings::IniFormat );
#else
    QSettings settings( QDir::homePath() + QObject::tr( "/.fedit2" ),
                        QSettings::IniFormat );
#endif

    settings.beginGroup( "Global" );


    QVariant val;

//     val = settings.value( "maximize" );
//     if ( val.isValid() ) M_maximize = val.toBool();

//     val = settings.value( "full_screen" );
//     if ( val.isValid() ) M_full_screen = val.toBool();

//     val = settings.value( "geometry" );
//     if ( val.isValid() )
//     {
//         parseWindowGeometry( val.toString().toStdString() );
//     }

    settings.endGroup();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
Options::writeSettings()
{
#ifdef Q_WS_WIN
    QSettings settings( QDir::currentPath() + QObject::tr( "/fedit2.ini" ),
                        QSettings::IniFormat );
#else
    QSettings settings( QDir::homePath() + QObject::tr( "/.fedit2" ),
                        QSettings::IniFormat );
#endif

    settings.beginGroup( "Global" );

//     settings.setValue( "maximize", M_maximize );
//     settings.setValue( "full_screen", M_full_screen );

//     if ( M_window_width > 0
//          && M_window_height > 0 )
//     {
//         settings.setValue( "geometry",
//                            QString( "%1 x %2 %3 %4" )
//                            .arg( M_window_width )
//                            .arg( M_window_height )
//                            .arg( M_window_pos_x )
//                            .arg( M_window_pos_y ) );
//     }

    settings.endGroup();
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
Options::parseCmdLine( int argc,
                       char ** argv )
{
    rcsc::ParamMap system_options( "System Options" );
    rcsc::ParamMap window_options( "Window Options" );
    rcsc::ParamMap editor_options( "Editor Options" );
    rcsc::ParamMap view_options( "View Options" );

    bool help = false;
    bool version = false;

    system_options.add()
        ( "help", "h",
          rcsc::BoolSwitch( &help ),
          "print this message." )
        ( "version", "v",
          rcsc::BoolSwitch( &version ),
          "print version." )
        ;

    std::string geometry;

    window_options.add()
        ( "maximize", "",
          rcsc::BoolSwitch( &M_maximize ),
          "start with a maximized window." )
        ( "full-screen", "",
          rcsc::BoolSwitch( &M_full_screen ),
          "start with a full screen window." )
        ( "geometry", "",
          &geometry,
          "specifies the window geometry \"[WxH][+X+Y]\". e.g. --geometry=1024x768+0+0" )
        ;

    std::string conf_file;
    std::string data_file;
    std::string background_conf_file;

    editor_options.add()
        ( "conf", "c",
          &conf_file,
          "specifies a .conf file." )
        ( "data", "d",
          &data_file,
          "specifies a .dat file." )
        ( "background-conf", "C",
          &background_conf_file,
          "specifies a .conf file as a background data." )
        ( "auto-backup", "",
          &M_auto_backup,
          "make backup files automatically" )
        ;

    view_options.add()
        ( "no-antialiasing", "",
          rcsc::NegateSwitch( &M_antialiasing ),
          "disable antialiasing painting." )
        ( "no-auto-fit", "",
          rcsc::NegateSwitch( &M_auto_fit_mode ),
          "disable automatic view area fitting." )
        ;

    rcsc::CmdLineParser parser( argc, argv );

    parser.parse( system_options );
    parser.parse( window_options );
    parser.parse( editor_options );
    parser.parse( view_options );

    //
    // analyze positional options
    //

    for ( std::vector< std::string >::const_iterator it = parser.positionalOptions().begin();
          it != parser.positionalOptions().end();
          ++it )
    {
        // ".conf"
        if ( it->length() > 5
             && it->compare( it->length() - 5, 5, ".conf" ) == 0 )
        {
            if ( conf_file.empty() )
            {
                conf_file = *it;
            }
            else if ( background_conf_file.empty() )
            {
                background_conf_file = *it;
            }
            else // if ( ! conf_file.empty() && ! background_conf_file.empty() )
            {
                help = true;
                break;
            }
        }
        else
        {
            help = true;
            break;
        }
    }

    M_conf_file = QString::fromStdString( conf_file );
    M_data_file = QString::fromStdString( data_file );
    M_background_conf_file = QString::fromStdString( background_conf_file );

    //
    // help message
    //
    if ( help
         || parser.failed()
         || parser.positionalOptions().size() > 4 )
    {
        std::cout << "Usage: " << "fedit"
                  << " [options ... ] [conf file [background conf file]] [dat file]"
                  << std::endl;
        system_options.printHelp( std::cout, false ); // with_default = false
        window_options.printHelp( std::cout );
        editor_options.printHelp( std::cout );
        view_options.printHelp( std::cout );
        return false;
    }

    //
    // version message
    //
    if ( version )
    {
        std::cout << "fedit" << " Version " << VERSION
                  << std::endl;
        return false;
    }

    //
    // window size and position
    //

    parseWindowGeometry( geometry );

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
Options::parseWindowGeometry( const std::string & val )
{
    if ( val.empty() )
    {
        return;
    }

    int w = -1, h = -1;
    int x = -1, y = -1;

    int num = std::sscanf( val.c_str(),
                           " %d x %d %d %d " ,
                           &w, &h, &x, &y );
    if ( num == 4 || num == 2 )
    {
        if ( w <= 0 || h <= 0 )
        {
            std::cerr << "Illegal window size [" << val
                      << "]" << std::endl;
            M_window_width = -1;
            M_window_height = -1;
            M_window_pos_x = -1;
            M_window_pos_y = -1;
        }
        else
        {
            M_window_width = w;
            M_window_height = h;
            M_window_pos_x = x;
            M_window_pos_y = y;
        }
    }
    else if ( std::sscanf( val.c_str(),
                           " %d %d " ,
                           &x, &y ) == 2 )
    {
        M_window_pos_x = x;
        M_window_pos_y = y;
    }
    else
    {
        std::cerr << "Illegal geometry format [" << val
                  << "]" << std::endl;
    }

}
