// -*-c++-*-

/*!
  \file debug_client_parser.cpp
  \brief debug client packet parser class Source File.
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

#include "debug_client_parser.h"
#include "view_holder.h"

#include <iostream>
#include <cstring>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstdio>

namespace {

/*-------------------------------------------------------------------*/
/*!
  \brief msg pointer is moved untill c.
  if '"' is appeared, all character is skipped until next '"' will be
  appeared.
*/
inline
int
go_to_next_char( const char ** msg, const char c )
{
    int i = 0;
    bool no_paren = true;
    while ( **msg != '\0' )
    {
        ++i;
        ++(*msg);
        if ( **msg == '\"' )
        {
            no_paren = ! no_paren;
        }
        else if ( no_paren )
        {
            if ( **msg == c )
            {
                break;
            }
        }
    }
    return i;
}
}


///////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugClientParser::parse( const char * msg,
                          ViewHolder & holder )
{
    M_side = 'n';
    M_unum = 0;
    M_time.assign( -1, 0 );

    int	version = 0;
    int	n_read = 0;

    bool result = false;
    DebugViewData::Ptr ptr( new DebugViewData );

    if ( std::sscanf( msg,
                      " ( ( debug ( format-version %d ) ) %n" ,
                      &version, &n_read ) == 1 )
    {
        switch ( version ) {
        case 1:
        case 2:
        case 3:
        case 4:
            std::cerr << __FILE__ << ": "
                      << "debug message version 1-4 are not supported anymore."
                      << std::endl;
            break;
        case 5:
            msg += n_read;
            result = parseImpl( msg, ptr );
            break;
        default:
            break;
        }
    }

    if ( result
         && ( M_side == 'l' || M_side == 'r' )
         && M_unum != 0
         && M_time.cycle() >= 0 )
    {
        holder.addDebugView( M_time, M_side, M_unum, ptr );
    }

    return result;
}

/*-------------------------------------------------------------------*/
/*!
  \brief parse debug client message body.
*/
bool
DebugClientParser::parseImpl( const char * msg,
                              DebugViewData::Ptr & debug_view )
{
    if ( msg[0] == '(' && msg[1] == '(' )
    {
        ++msg;
    }

    // read all S-Expression token that is contained in 'msg;.
    while ( *msg != '\0' && *msg != ')' )
    {
        // if first character is not '(', it may be illegal token.
        // try to skip to the next '('.
        if ( *msg != '(' )
        {
            std::cerr << __FILE__ << ": (parseV2) "
                      << ": bad message [" << std::string( msg, 16 ) << "]"
                      << std::endl;
            // skip to next paren
            go_to_next_char( &msg, '(' );
            continue;
        }

        // read token ID
        char id[32];
        if ( std::sscanf( msg,
                          " ( %s ",
                          id ) != 1 )
        {
            std::cerr << __FILE__ << ": (parseV2) "
                      << "Bad message ID [" << std::string( msg, 16 ) << "]"
                      << std::endl;
            ++msg;
            continue;
        }

        int n_read = 0;

        if ( ! std::strcmp( id, "t" )
             || ! std::strcmp( id, "o" )
             || ! std::strcmp( id, "ut" )
             || ! std::strcmp( id, "uo" )
             || ! std::strcmp( id, "u" )
             )
        {
            // other players info
            if ( ( n_read = parsePlayer( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parsePlayer" << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "line" ) )
        {
            // line draw request
            if ( ( n_read = parseLine( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseLine ["
                          << std::string( msg, 32 )
                          << "]"
                          << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "tri" ) )
        {
            // triangle draw request
            if ( ( n_read = parseTriangle( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseTriangle ["
                          << std::string( msg, 32 )
                          << "]"
                          << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "rect" ) )
        {
            // rectanble draw request
            if ( ( n_read = parseRectangle( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseRectangle["
                          << std::string( msg, 32 )
                          << "]"
                          << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "circle" ) )
        {
            // line draw request
            if ( ( n_read = parseCircle( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseCircle["
                          << std::string( msg, 32 )
                          << "]" << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "s" ) )
        {
            // self info
            if ( ( n_read = parseSelf( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseSelf" << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "b" ) )
        {
            // ball info
            if ( ( n_read = parseBall( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseBall" << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "time" ) )
        {
            // time info of this message
            if ( ( n_read = parseTime( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseTime" << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "target-teammate" ) )
        {
            if ( ( n_read = parseTargetTeammate( msg, debug_view ) ) == 0  )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseTargetTeammate" << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "target-point" ) )
        {
            if ( ( n_read = parseTargetPoint( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseTargetPoint" << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "say" ) )
        {
            if ( ( n_read = parseSayMessage( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseSayMessage" << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "hear" ) )
        {
            if ( ( n_read = parseHearMessage( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseHearMessage" << std::endl;
                return false;
            }
        }
        else if ( ! std::strcmp( id, "message" ) )
        {
            // misc. message
            if ( ( n_read = parseMessage( msg, debug_view ) ) == 0 )
            {
                std::cerr << __FILE__ << ": (parseV2) "
                          << "ERROR parseMessage" << std::endl;
                return false;
            }
        }
        else if ( id[0] == '(' )
        {
            ++msg;
            continue;
        }
        else
        {
                std::cerr << __FILE__ << ": (parseV2) "
                      << "Unsupported id [" << id << "]"
                      << std::endl;
            go_to_next_char( &msg, '(' );
            continue;
        }

        msg += n_read;

        if ( msg[0] != '(' )
        {
            go_to_next_char( &msg, '(' );
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!
  \brief parse time token
  (time <long>[,<long>])
  \retval 0 if failed.
  \return the length of read byte.
*/
int
DebugClientParser::parseTime( const char * tok,
                              DebugViewData::Ptr & )
{
    int n_read = 0;

    int cycle = 0;
    int stopped = 0;
    if ( std::sscanf( tok,
                      " ( time %d , %d ) %n",
                      &cycle, &stopped, &n_read ) == 2
         && n_read > 0 )
    {
        // ok
    }
    else if ( std::sscanf( tok,
                           " ( time %d ) %n",
                           &cycle, &n_read ) != 1
              && n_read > 0 )
    {
        // ok
    }
    else
    {

        std::cerr << __FILE__ <<": (parseTime) Failed. ["
                  << std::string( tok, 16 )
                  << "]" << std::endl;
        return 0;
    }

#if 0
    std::cerr << __FILE__ << ": (parseTime) time = " << cycle
              << "  offset=" << n_read << std::endl;
#endif
    M_time.assign( cycle, stopped );

    return n_read;
}

/*-------------------------------------------------------------------*/
/*!
  \brief parse ball token
  (b <x> <y>[ <vx> <vy>][ (c "<comment>")])
  \retval 0 if failed.
  \return the length.of read byte.
*/
int
DebugClientParser::parseBall( const char * tok,
                              DebugViewData::Ptr & debug_view )
{
    const char * start = tok;
    char * next;

    while ( *tok != '\0' && *tok == '(' ) ++tok;
    while ( *tok != '\0' && *tok == ' ' ) ++tok;
    while ( *tok != '\0' && *tok != ' ' ) ++tok; // skip 'b'

    const double x = std::strtod( tok, &next );
    if ( x == -HUGE_VAL || x == HUGE_VAL ) return 0;
    tok = next;
    const double y = std::strtod( tok, &next );
    if ( y == -HUGE_VAL || y == HUGE_VAL ) return 0;
    tok = next;

    while ( *tok != '\0' && *tok == ' ' ) ++tok;

    double vx = HUGE_VAL;
    double vy = HUGE_VAL;
    if ( std::isdigit( *tok )
         || *tok == '-'
         || *tok == '.' )
    {
        vx = std::strtod( tok, &next );
        if ( vx == -HUGE_VAL || vx == HUGE_VAL ) return 0;
        tok = next;
        vy = std::strtod( tok, &next );
        if ( vy == -HUGE_VAL || vy == HUGE_VAL ) return 0;
        tok = next;

        while ( *tok != '\0' && *tok == ' ' ) ++tok;
    }

    char comment[256+1];
    std::strcpy( comment, "" );
    if ( *tok == '('
         && *(tok+1) == 'c' )
    {
        int n_read = 0;
        if ( std::sscanf( tok,
                          " ( c \"%256[^\"]\" ) %n" ,
                          comment,
                          &n_read ) == 1
             && n_read != 0 )
        {
            tok += n_read;
        }
    }

    while ( *tok != '\0' && *tok != ')' ) ++tok;
    // skip this token
    while ( *tok != '\0' && ( *tok == ' ' || *tok == ')' ) ) ++tok;
#if 0
    std::cerr << __FILE__ << ": (parserBall) all:"
              << " x=" << x
              << " y=" << y
              << " vx=" << vx
              << " vy=" << vy
              << " c=[" << comment << "]"
              << std::endl;
#endif
    // create new instance
    boost::shared_ptr< DebugViewData::BallT >
        ball( new DebugViewData::BallT( static_cast< float >( x ),
                                        static_cast< float >( y ) ) );

    if ( vx != HUGE_VAL )
    {
        ball->setVel( static_cast< float >( vx ),
                      static_cast< float >( vy ) );
    }

    if ( comment[0] != '\0' )
    {
        ball->comment_.assign( comment );
    }

    debug_view->setBall( ball );

    return tok - start;
}

/*-------------------------------------------------------------------*/
/*!
  (s <side> <unum> <ptype> <x> <y> <vx> <vy> <body> <neck>[ (c "<comment>")])
*/
int
DebugClientParser::parseSelf( const char * tok,
                              DebugViewData::Ptr & debug_view )
{
    const char * start = tok;

    while ( *tok != '\0' && *tok == '(' ) ++tok;
    while ( *tok != '\0' && *tok == ' ' ) ++tok;
    while ( *tok != '\0' && *tok != ' ' ) ++tok; // skip 's'

    while ( *tok != '\0' && *tok == ' ' ) ++tok; // goto <side>
    const char side = *tok;
    if ( side != 'l' && side != 'r' )
    {
        std::cerr << __FILE__ << ": (parseSelf) ERROR unknown side char ["
                  << side << "]" << std::endl;
        return 0;
    }

    ++tok; // skip side
    while ( *tok != '\0' && *tok == ' ' ) ++tok; // skip space

    //
    // coach
    //
    if ( *tok == 'c' )
    {
        M_side = side;
        M_unum = 12;

        boost::shared_ptr< DebugViewData::SelfT > coach( new DebugViewData::SelfT( side ) );
        debug_view->setSelf( coach );

        // skip this token
        while ( *tok != '\0' && *tok != ')' ) ++tok;
        while ( *tok != '\0' && ( *tok == ' ' || *tok == ')' ) ) ++tok;

        return tok - start;
    }

    //
    // player
    //

    char * next;

    const long unum = std::strtol( tok, &next, 10 );
    if ( unum == LONG_MIN || unum == LONG_MAX ) return 0;
    tok = next;
    const long ptype = std::strtol( tok, &next, 10 );
    if ( ptype == LONG_MIN || ptype == LONG_MAX ) return 0;
    tok = next;
    double x = std::strtod( tok, &next );
    if ( x == -HUGE_VAL || x == HUGE_VAL ) return 0;
    tok = next;
    double y = std::strtod( tok, &next );
    if ( y == -HUGE_VAL || y == HUGE_VAL ) return 0;
    tok = next;
    double vx = std::strtod( tok, &next );
    if ( vx == -HUGE_VAL || vx == HUGE_VAL ) return 0;
    tok = next;
    double vy = std::strtod( tok, &next );
    if ( vy == -HUGE_VAL || vy == HUGE_VAL ) return 0;
    tok = next;
    double body = std::strtod( tok, &next );
    if ( body == -HUGE_VAL || body == HUGE_VAL ) return 0;
    tok = next;
    double neck = std::strtod( tok, &next );
    if ( neck == -HUGE_VAL || neck == HUGE_VAL ) return 0;
    tok = next;

    while ( *tok != '\0' && *tok == ' ' ) ++tok;

    char comment[256 + 1];
    std::strcpy( comment, "" );

    if ( *tok == '(' )
    {
        int n_read = 0;
        if ( std::sscanf( tok,
                          " ( c \"%256[^\"]\" ) %n" ,
                          comment,
                          &n_read ) == 1
             && n_read != 0 )
        {
            tok += n_read;
        }
    }

    while ( *tok != '\0' && *tok != ')' ) ++tok;
    // skip this token
    while ( *tok != '\0' && ( *tok == ' ' || *tok == ')' ) ) ++tok;

#if 0
    std::cerr << __FILE__ << ": (parserSelf) Self:"
              << " side=" << side
              << " unum=" << unum
              << " x=" << x
              << " y=" << y
              << " vx=" << vx
              << " vy=" << vy
              << " b=" << body
              << " n=" << neck
              << " c=[" << comment << "]"
              << std::endl;
#endif

    M_side = side;
    M_unum = static_cast< int >( unum );
    // create new instance
    boost::shared_ptr< DebugViewData::SelfT >
        self( new DebugViewData::SelfT( M_side,
                                        M_unum,
                                        ptype,
                                        static_cast< float >( x ),
                                        static_cast< float >( y ),
                                        static_cast< float >( vx ),
                                        static_cast< float >( vy ),
                                        static_cast< float >( body ),
                                        static_cast< float >( neck ),
                                        comment ) );
    debug_view->setSelf( self );
    return tok - start;
}

/*-------------------------------------------------------------------*/
/*!
  ({t|o} <unum> <x> <y>[ (bd <body>)][ (pt <pointto>)][ (c "<comment>")])
  ({ut|uo|u} <x> <y>[ (bd <body>)][ (c "<comment>")])
*/
int
DebugClientParser::parsePlayer( const char * tok,
                                DebugViewData::Ptr & debug_view )
{
    const char * start = tok;
    char * next;

    while ( *tok != '\0' && *tok == '(' ) ++tok;
    while ( *tok != '\0' && *tok == ' ' ) ++tok;

    // read side
    DebugViewData::PlayerRecog recog_type = DebugViewData::MAX_PLAYER_RECOG;
    if ( *tok != 'u' )
    {
        if ( *tok == 't' ) recog_type = DebugViewData::TEAMMATE;
        if ( *tok == 'o' ) recog_type = DebugViewData::OPPONENT;
    }
    else
    {
        if ( *(tok+1) == 't' ) recog_type = DebugViewData::UNKNOWN_TEAMMATE;
        if ( *(tok+1) == 'o' ) recog_type = DebugViewData::UNKNOWN_OPPONENT;
        if ( *(tok+1) == ' ' ) recog_type = DebugViewData::UNKNOWN_PLAYER;
    }
    while ( *tok != '\0' && *tok != ' ' ) ++tok; // skip side info


    // read unum & player type id
    long unum = 0;
    long ptype = rcsc::Hetero_Unknown;
    if ( recog_type == DebugViewData::TEAMMATE
         || recog_type == DebugViewData::OPPONENT )
    {
        unum = std::strtol( tok, &next, 10 );
        if ( unum == LONG_MIN || unum == LONG_MAX ) return 0;
        tok = next;

        ptype = std::strtol( tok, &next, 10 );
        if ( ptype == LONG_MIN || ptype == LONG_MAX ) return 0;
        tok = next;
    }

    const double x = std::strtod( tok, &next );
    if ( x == -HUGE_VAL || x == HUGE_VAL ) return 0;
    tok = next;
    const double y = std::strtod( tok, &next );
    if ( y == -HUGE_VAL || y == HUGE_VAL ) return 0;
    tok = next;

    while ( *tok != '\0' && *tok == ' ' ) ++tok;

    float body = -360.0;
    float pointto = -360.0;
    char comment[256+1];
    std::strcpy( comment, "" );

    while ( *tok == '(' )
    {
        int n_read = 0;

        if ( std::sscanf( tok,
                          " ( bd %f ) %n",
                          &body, &n_read ) == 1
             && n_read != 0 )
        {
            tok += n_read;
            while ( *tok == ' ' ) ++tok;
        }
        else if ( std::sscanf( tok,
                               " ( pt %f ) %n",
                               &pointto, &n_read ) == 1
                  && n_read != 0 )
        {
            tok += n_read;
            while ( *tok == ' ' ) ++tok;
        }
        else if ( std::sscanf( tok,
                               " ( c \"%256[^\"]\" ) %n" ,
                               comment, &n_read ) == 1
                  && n_read != 0 )
        {
            tok += n_read;
            while ( *tok == ' ' ) ++tok;
        }
        else
        {
            std::cerr << __FILE__ << ": (parsePlayer)"
                      << " unsupported option. ["
                      << std::string( tok, 5 ) << "]" << std::endl;
            break;
        }
    }

    while ( *tok != '\0' && *tok != ')' ) ++tok;
    // skip this token
    while ( *tok != '\0' && ( *tok == ' ' || *tok == ')' ) ) ++tok;

    boost::shared_ptr< DebugViewData::PlayerT >
        player( new DebugViewData::PlayerT( static_cast< rcsc::rcg::Int16 >( unum ),
                                            static_cast< rcsc::rcg::Int16 >( ptype ),
                                            static_cast< float >( x ),
                                            static_cast< float >( y ),
                                            body,
                                            pointto,
                                            comment ) );

    switch ( recog_type ) {
    case DebugViewData::TEAMMATE:
        debug_view->addTeammate( player );
        break;
    case DebugViewData::OPPONENT:
        debug_view->addOpponent( player );
        break;
    case DebugViewData::UNKNOWN_TEAMMATE:
        debug_view->addUnknownTeammate( player );
        break;
    case DebugViewData::UNKNOWN_OPPONENT:
        debug_view->addUnknownOpponent( player );
        break;
    case DebugViewData::UNKNOWN_PLAYER:
        debug_view->addUnknownPlayer( player );
        break;
    default:
        std::cerr << __FILE__ << ": (parsePlayer) "
                  << "ERROR Illegal Player ID. pos = ("
                  << x  << ", "<< y << ")"
                  << std::endl;
        break;
    }
    return tok - start;
}

/*-------------------------------------------------------------------*/
/*!
  (line <x1> <y1> <x2> <y2> <color>)
*/
int
DebugClientParser::parseLine( const char * tok,
                              DebugViewData::Ptr & debug_view )
{
    const char * start = tok;
    char * next;

    while ( *tok != '\0' && *tok == '(' ) ++tok;
    while ( *tok != '\0' && *tok == ' ' ) ++tok;
    while ( *tok != '\0' && *tok != ' ' ) ++tok; // skip 'line'

    const double x1 = std::strtod( tok, &next );
    if ( x1 == -HUGE_VAL || x1 == HUGE_VAL ) return 0;
    tok = next;
    const double y1 = std::strtod( tok, &next );
    if ( y1 == -HUGE_VAL || y1 == HUGE_VAL ) return 0;
    tok = next;
    const double x2 = std::strtod( tok, &next );
    if ( x2 == -HUGE_VAL || x2 == HUGE_VAL ) return 0;
    tok = next;
    const double y2 = std::strtod( tok, &next );
    if ( y2 == -HUGE_VAL || y2 == HUGE_VAL ) return 0;
    tok = next;

    char color[32];
    std::memset( color, 0, 32 );
    if ( *tok != '\0' && *tok != ')' )
    {
        int n_read = 0;
        if ( std::sscanf( tok, " \" %31[^\")] %n", color, &n_read ) == 1 )
        {
            tok += n_read;
        }
    }

    while ( *tok != '\0' && *tok != ')' ) ++tok;
    // skip this token
    while ( *tok != '\0' && ( *tok == ' ' || *tok == ')' ) ) ++tok;
#if 0
    std::cerr << __FILE__ << ": (parseLine) Line:"
              << " x1=" << x1
              << " y1=" << y1
              << " x2=" << x2
              << " y2=" << y2
              << " color=" << color
              << std::endl;
#endif
    debug_view->addLine( static_cast< float >( x1 ),
                         static_cast< float >( y1 ),
                         static_cast< float >( x2 ),
                         static_cast< float >( y2 ),
                         color );

    return tok - start;
}

/*-------------------------------------------------------------------*/
/*!
  (tri <x1> <y1> <x2> <y2> <x3> <y3> <color>)
*/
int
DebugClientParser::parseTriangle( const char * tok,
                                  DebugViewData::Ptr & debug_view )
{
    const char * start = tok;
    char * next;

    while ( *tok != '\0' && *tok == '(' ) ++tok;
    while ( *tok != '\0' && *tok == ' ' ) ++tok;
    while ( *tok != '\0' && *tok != ' ' ) ++tok; // skip 'tri'

    const double x1 = std::strtod( tok, &next );
    if ( x1 == -HUGE_VAL || x1 == HUGE_VAL ) return 0;
    tok = next;
    const double y1 = std::strtod( tok, &next );
    if ( y1 == -HUGE_VAL || y1 == HUGE_VAL ) return 0;
    tok = next;
    const double x2 = std::strtod( tok, &next );
    if ( x2 == -HUGE_VAL || x2 == HUGE_VAL ) return 0;
    tok = next;
    const double y2 = std::strtod( tok, &next );
    if ( y2 == -HUGE_VAL || y2 == HUGE_VAL ) return 0;
    tok = next;
    const double x3 = std::strtod( tok, &next );
    if ( x3 == -HUGE_VAL || x3 == HUGE_VAL ) return 0;
    tok = next;
    const double y3 = std::strtod( tok, &next );
    if ( y3 == -HUGE_VAL || y3 == HUGE_VAL ) return 0;
    tok = next;

    char color[32];
    std::memset( color, 0, 32 );
    if ( *tok != '\0' && *tok != ')' )
    {
        int n_read = 0;
        if ( std::sscanf( tok, " \" %31[^\")] %n", color, &n_read ) == 1 )
        {
            tok += n_read;
        }
    }

    while ( *tok != '\0' && *tok != ')' ) ++tok;
    // skip this token
    while ( *tok != '\0' && ( *tok == ' ' || *tok == ')' ) ) ++tok;

    debug_view->addTriangle( static_cast< float >( x1 ),
                             static_cast< float >( y1 ),
                             static_cast< float >( x2 ),
                             static_cast< float >( y2 ),
                             static_cast< float >( x3 ),
                             static_cast< float >( y3 ),
                             color );

    return tok - start;
}

/*-------------------------------------------------------------------*/
/*!
  (rect <left_x> <top_y> <right_x> <bottom_y> <color>)
*/
int
DebugClientParser::parseRectangle( const char * tok,
                                   DebugViewData::Ptr & debug_view )
{
    const char * start = tok;
    char * next;

    while ( *tok != '\0' && *tok == '(' ) ++tok;
    while ( *tok != '\0' && *tok == ' ' ) ++tok;
    while ( *tok != '\0' && *tok != ' ' ) ++tok; // skip 'rect'

    const double x1 = std::strtod( tok, &next );
    if ( x1 == -HUGE_VAL || x1 == HUGE_VAL ) return 0;
    tok = next;
    const double y1 = std::strtod( tok, &next );
    if ( y1 == -HUGE_VAL || y1 == HUGE_VAL ) return 0;
    tok = next;
    const double x2 = std::strtod( tok, &next );
    if ( x2 == -HUGE_VAL || x2 == HUGE_VAL ) return 0;
    tok = next;
    const double y2 = std::strtod( tok, &next );
    if ( y2 == -HUGE_VAL || y2 == HUGE_VAL ) return 0;
    tok = next;

    char color[32];
    std::memset( color, 0, 32 );
    if ( *tok != '\0' && *tok != ')' )
    {
        int n_read = 0;
        if ( std::sscanf( tok, " \" %31[^\")] %n", color, &n_read ) == 1 )
        {
            tok += n_read;
        }
    }

    while ( *tok != '\0' && *tok != ')' ) ++tok;
    // skip this token
    while ( *tok != '\0' && ( *tok == ' ' || *tok == ')' ) ) ++tok;

    debug_view->addRectangle( static_cast< float >( x1 ),
                              static_cast< float >( y1 ),
                              static_cast< float >( x2 ),
                              static_cast< float >( y2 ),
                              color );

    return tok - start;
}


/*-------------------------------------------------------------------*/
/*!
  (circle <center_x> <center_y> <radius> <color>)
*/
int
DebugClientParser::parseCircle( const char * tok,
                                DebugViewData::Ptr & debug_view )
{
    const char * start = tok;
    char * next;

    while ( *tok != '\0' && *tok == '(' ) ++tok;
    while ( *tok != '\0' && *tok == ' ' ) ++tok;
    while ( *tok != '\0' && *tok != ' ' ) ++tok; // skip 'rect'

    const double x = std::strtod( tok, &next );
    if ( x == -HUGE_VAL || x == HUGE_VAL ) return 0;
    tok = next;
    const double y = std::strtod( tok, &next );
    if ( y == -HUGE_VAL || y == HUGE_VAL ) return 0;
    tok = next;
    const double r = std::fabs( std::strtod( tok, &next ) );
    if ( r == -HUGE_VAL || r == HUGE_VAL ) return 0;
    tok = next;

    char color[32];
    std::memset( color, 0, 32 );
    if ( *tok != '\0' && *tok != ')' )
    {
        int n_read = 0;
        if ( std::sscanf( tok, " \" %31[^\")] %n", color, &n_read ) == 1 )
        {
            tok += n_read;
        }
    }

    while ( *tok != '\0' && *tok != ')' ) ++tok;
    // skip this token
    while ( *tok != '\0' && ( *tok == ' ' || *tok == ')' ) ) ++tok;

    debug_view->addCircle( static_cast< float >( x ),
                           static_cast< float >( y ),
                           static_cast< float >( r ),
                           color );

    return tok - start;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
DebugClientParser::parseTargetTeammate( const char * tok,
                                        DebugViewData::Ptr & debug_view )
{
    int n_read = 0;

    int number = 0;
    if ( std::sscanf( tok,
                      " ( target-teammate %d ) %n",
                      &number,
                      &n_read ) != 1
         || n_read == 0 )
    {
        std::cerr << __FILE__ << ": (parseTargetTeammate) Failed ["
                  << std::string( tok, 16 )
                  << "]"  << std::endl;
        return 0;
    }
#if 0
    std::cerr << __FILE__ << ": (parseTagetTeammate) target teammate = "
              << number << std::endl;
#endif
    if ( 0 < number && number < 12 )
    {
        debug_view->setTargetTeammate( number );
    }

    return n_read;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
DebugClientParser::parseTargetPoint( const char * tok,
                                     DebugViewData::Ptr & debug_view )
{
    int n_read = 0;

    double x = 0.0;
    double y = 0.0;
    if ( std::sscanf( tok,
                      " ( target-point %lf %lf ) %n",
                      &x, &y,
                      &n_read ) != 2
         || n_read == 0 )
    {
        std::cerr << __FILE__ << ": (parseTargetPoint) Failed. ["
                  << std::string( tok, 16 )
                  << "]"  << std::endl;
        return 0;
    }
#if 0
    std::cerr << __FILE__ << ": (parseTargetPoint) target point = "
              << x << ", " << y << std::endl;
#endif
    debug_view->setTargetPoint( x, y );

    return n_read;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
DebugClientParser::parseMessage( const char * tok,
                                 DebugViewData::Ptr & debug_view )
{
    int n_read = 0;

    char message[1024];
    std::strcpy( message, "" );
    if ( std::sscanf( tok,
                      " ( message \"%1023[^\"]\" ) %n" ,
                      message,
                      &n_read ) != 1
         || n_read == 0 )
    {
        std::cerr << __FILE__ << ": (parseMessage) Failed. ["
                  << std::string( tok, 16 )
                 << "]"  << std::endl;
        return 0;
    }
#if 0
    std::cerr << __FILE__ << ": (parseMessage) message = ["
              << message << "]" << std::endl;
#endif
    debug_view->setMessage( message );

//     if ( std::strstr( message, "Say[" ) != NULL )
//     {
//         debug_view->setHasSayMessage( true );
//     }

    return n_read;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
DebugClientParser::parseSayMessage( const char * tok,
                                    DebugViewData::Ptr & debug_view )
{
    int n_read = 0;

    char message[1024];
    std::strcpy( message, "" );
    if ( std::sscanf( tok,
                      " ( say \"%1023[^\"]\" ) %n" ,
                      message,
                      &n_read ) != 1
         || n_read == 0 )
    {
        std::cerr << __FILE__ << ": (parseSayMessage) Failed. ["
                  << std::string( tok, 16 )
                 << "]"  << std::endl;
        return 0;
    }

    debug_view->setSayMessage( message );

    return n_read;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
DebugClientParser::parseHearMessage( const char * tok,
                                     DebugViewData::Ptr & debug_view )
{
    const char * start = tok;

    while ( *tok != '\0' && *tok == ' ' ) ++tok;
    while ( *tok != '\0' && *tok == '(' ) ++tok;
    while ( *tok != '\0' && *tok == ' ' ) ++tok;
    while ( *tok != '\0' && *tok != ' ' && *tok != ')' ) ++tok;  // skip 'hear'

    while ( *tok != '\0' )
    {
        if ( *tok == ')' )
        {
            ++tok;
            break;
        }

        int n_read = 0;
        int unum = -1;
        char msg[256];

        if ( std::sscanf( tok, " ( %d \"%255[^\"]\" ) %n ",
                          &unum, msg, &n_read ) != 2 )
        {
            std::cerr << __FILE__ << ": (parseHearMessage) "
                      << "Illegal message [" << std::string( start, 16 ) << "]"
                      << std::endl;
            return 0;
        }
        tok += n_read;

        debug_view->addHearMessage( unum, msg );

        while ( *tok != '\0' && *tok == ' ' ) ++tok;
    }

    while ( *tok != '\0' && *tok == ' ' ) ++tok;

    return tok - start;
}
