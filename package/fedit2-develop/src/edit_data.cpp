// -*-c++-*-

/*!
  \file feditor_data.cpp
  \brief formation editor data class Source File.
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

#include <QDateTime>
#include <QFile>

#include "edit_data.h"

#include "options.h"

#include <rcsc/common/server_param.h>
#include <rcsc/geom/rect_2d.h>
#include <rcsc/math_util.h>

#include <fstream>
#include <iostream>

using namespace rcsc;
using namespace rcsc::formation;

const double EditData::MAX_X = ServerParam::DEFAULT_PITCH_LENGTH * 0.5 + 2.0;
const double EditData::MAX_Y = ServerParam::DEFAULT_PITCH_WIDTH * 0.5 + 2.0;

namespace {

inline
Vector2D
round_coordinates( const double & x,
                   const double & y )
{
    return Vector2D( rint( bound( - EditData::MAX_X, x, EditData::MAX_X )
                           / SampleData::PRECISION )
                     * SampleData::PRECISION,
                     rint( bound( - EditData::MAX_Y, y, EditData::MAX_Y )
                           / SampleData::PRECISION )
                     * SampleData::PRECISION );
}

}


/*-------------------------------------------------------------------*/
/*!

 */
EditData::EditData()
    : M_filepath()
    , M_conf_changed( false )
    , M_current_index( -1 )
    , M_select_type( NO_SELECT )
    , M_select_index( 0 )
    , M_constraint_origin_index( -1 )
    , M_constraint_terminal_index( -1 )
    , M_constraint_terminal( Vector2D::INVALIDATED )
{
    init();
}

/*-------------------------------------------------------------------*/
/*!

 */
EditData::~EditData()
{
    //saveChanged();
    //std::cerr << "delete EditData" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::init()
{
    M_conf_changed = false;
    M_samples.reset();

    M_state.ball_.assign( 0.0, 0.0 );
    for ( size_t i = 0; i < M_state.players_.size(); ++i )
    {
        M_state.players_[i].assign( -3.0 * i + 3.0, -37.0 );
    }

    M_constraint_origin_index = size_t( -1 );
    M_constraint_terminal_index = size_t( -1 );
    M_constraint_terminal = Vector2D::INVALIDATED;

    M_select_type = NO_SELECT;
    M_select_index = 0;

    M_triangulation.clear();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::createFormation( const QString & type_name )
{
    std::cerr << "createFormation init" << std::endl;
    init();

    std::cerr << "createFormation create" << std::endl;
    M_formation = Formation::create( type_name.toStdString() );

    if ( ! M_formation )
    {
        std::cerr << __FILE__ << ":" << __LINE__
                  << " ***ERROR*** Failed to create formation."
                  << std::endl;
        return;
    }

    M_conf_changed = true;
    M_samples = M_formation->samples();

    M_formation->createDefaultData();
    train();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::backup( const QString & filepath )
{
    if ( filepath.isEmpty() )
    {
        return;
    }

    QString backup_filepath = filepath;
    if ( M_saved_datetime.isEmpty() )
    {
        backup_filepath += QString( "_" );
        backup_filepath += QDateTime::currentDateTime().toString( QString( "yyyyMMdd-hhmmss" ) );
        backup_filepath += QString( ".conf" );
    }
    else
    {
        backup_filepath += QString( "_" );
        backup_filepath += M_saved_datetime;
        //backup_filepath += QString( ".conf" );
    }

    if ( ! QFile::copy( filepath, backup_filepath ) )
    {
        std::cerr << "Failed to backup the file [" << filepath.toStdString()  << "]"
                  << std::endl;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
EditData::openConf( const QString & filepath )
{
    std::ifstream fin( filepath.toStdString().c_str() );
    if ( ! fin.is_open() )
    {
        std::cerr << "Failed to open formation file ["
                  << filepath.toStdString() << "]"
                  << std::endl;
        return false;
    }

    M_samples.reset();

    M_formation = Formation::create( fin );
    if ( ! M_formation )
    {
        std::cerr << "Failed to read a formation. ["
                  << filepath.toStdString() << "]"
                  << std::endl;
        return false;
    }

    fin.seekg( 0 );
    if ( ! M_formation->read( fin ) )
    {
        fin.close();
        M_formation.reset();
        return false;
    }
    fin.close();

    init();
    M_filepath = filepath;
    M_samples = M_formation->samples();
    updateTriangulation();
    updatePlayerPosition();
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
EditData::openData( const QString & filepath )
{
    if ( ! M_formation )
    {
        std::cerr << "No formation! create a new one or open a exsiting one." << std::endl;
        return false;
    }

    M_triangulation.clear();

    M_samples = SampleDataSet::Ptr( new SampleDataSet() );
    if ( ! M_samples->open( filepath.toStdString() ) )
    {
        return false;
    }

    M_current_index = -1;

    M_formation->setSamples( M_samples );
    train();

    return true;
}


/*-------------------------------------------------------------------*/
/*!

 */
bool
EditData::openBackgroundConf( const QString & filepath )
{
    std::ifstream fin( filepath.toStdString().c_str() );
    if ( ! fin.is_open() )
    {
        std::cerr << "Failed to open formation file ["
                  << filepath.toStdString() << "]"
                  << std::endl;
        return false;
    }

    M_background_formation = Formation::create( fin );
    if ( ! M_background_formation )
    {
        std::cerr << "Failed to read a background formation. ["
                  << filepath.toStdString() << "]"
                  << std::endl;
        return false;
    }

    fin.seekg( 0 );
    if ( ! M_background_formation->read( fin ) )
    {
        fin.close();
        M_background_formation.reset();
        return false;
    }
    fin.close();

    {
        M_background_triangulation.clear();

        const SampleDataSet::DataCont::const_iterator end = M_background_formation->samples()->dataCont().end();
        for ( SampleDataSet::DataCont::const_iterator it = M_background_formation->samples()->dataCont().begin();
              it != end;
              ++it )
        {
            M_background_triangulation.addPoint( it->ball_ );
        }

        for ( SampleDataSet::Constraints::const_iterator it = M_background_formation->samples()->constraints().begin();
              it != M_background_formation->samples()->constraints().end();
              ++it )
        {
            //M_background_triangulation.addConstraint( cdt::Triangulation::IndexPair( size_t( it->first->index_ ),
            //                                                                         size_t( it->second->index_ ) ) );
            M_background_triangulation.addConstraint( static_cast< size_t >( it->first->index_ ),
                                                      static_cast< size_t >( it->second->index_ ) );
        }

        M_background_triangulation.compute();
        //M_background_triangulation.updateHalfEdges();
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
EditData::saveConf()
{
    if ( M_filepath.isEmpty() )
    {
        return false;
    }

    return saveConfAs( M_filepath );
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
EditData::saveConfAs( const QString & filepath )
{
    if ( ! M_formation )
    {
        return false;
    }

    M_saved_datetime = QDateTime::currentDateTime().toString( QString( "yyyyMMdd-hhmmss" ) );

    if ( Options::instance().autoBackup() )
    {
        backup( M_filepath );
    }

    std::ofstream fout( filepath.toStdString().c_str() );
    if ( ! fout.is_open() )
    {
        fout.close();
        return false;
    }

    M_formation->printComment( fout, M_saved_datetime.toStdString() );

    if ( ! M_formation->print( fout ) )
    {
        return false;
    }
    fout.flush();
    fout.close();

    M_conf_changed = false;
    M_filepath = filepath;
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::updatePlayerPosition()
{
    if ( ! Options::instance().playerAutoMove() )
    {
        return;
    }

    if ( ! M_formation )
    {
        return;
    }

    M_formation->getPositions( M_state.ball_, M_state.players_ );

    for ( std::vector< Vector2D >::iterator it = M_state.players_.begin();
          it != M_state.players_.end();
          ++it )
    {
        *it = round_coordinates( it->x, it->y );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::updateTriangulation()
{
    if ( ! M_samples )
    {
        return;
    }

    M_triangulation.clear();

    const SampleDataSet::DataCont::const_iterator end = M_samples->dataCont().end();
    for ( SampleDataSet::DataCont::const_iterator it = M_samples->dataCont().begin();
          it != end;
          ++it )
    {
        M_triangulation.addPoint( it->ball_ );
    }

    for ( SampleDataSet::Constraints::const_iterator it = M_samples->constraints().begin();
          it != M_samples->constraints().end();
          ++it )
    {
        //M_triangulation.addConstraint( cdt::Triangulation::IndexPair( size_t( it->first->index_ ),
        //                                                              size_t( it->second->index_ ) ) );
        M_triangulation.addConstraint( static_cast< size_t >( it->first->index_ ),
                                       static_cast< size_t >( it->second->index_ ) );
    }

    M_triangulation.compute();
    //M_triangulation.updateHalfEdges();

    std::cerr << "updateTriangulation"
//               << "\n  vertices=" << M_triangulation.indexedVertices().size()
//               << "\n  edges=" << M_triangulation.halfEdges().size()
              << "\n  edges=" << M_triangulation.edges().size()
              << "\n  triangles=" << M_triangulation.triangles().size()
              << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::updateRoleData( const int unum,
                          const int symmetry_unum,
                          const std::string & role_name )
{
    if ( ! M_formation )
    {
        return;
    }

    if ( M_formation->updateRole( unum, symmetry_unum, role_name ) )
    {
        M_conf_changed = true;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::updateRoleType( const int unum,
                          const int type_index )
{
    if ( ! M_formation )
    {
        return;
    }

    if ( M_formation->updateRoleType( unum, static_cast< RoleType::Type >( type_index ) ) )
    {
        M_conf_changed = true;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::updateMarkerData( const int unum,
                            const bool marker,
                            const bool setplay_marker )
{
    if ( ! M_formation )
    {
        return;
    }

    if ( M_formation->updateMarker( unum, marker, setplay_marker ) )
    {
        M_conf_changed = true;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::moveBallTo( const double & x,
                      const double & y )
{
    Vector2D pos = round_coordinates( x, y );

    M_state.ball_ = pos;

    if ( Options::instance().dataAutoSelect() )
    {
        if ( pos.absY() < 1.0 )
        {
            M_state.ball_.y = 0.0;
        }

        if ( M_samples )
        {
            SampleDataSet::IndexData d = M_samples->nearestData( pos, 1.0 );
            if ( d.second )
            {
                M_current_index = d.first;
                M_state.ball_ = d.second->ball_;
            }
        }
    }

    updatePlayerPosition();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::movePlayerTo( const int unum,
                        const double & x,
                        const double & y )
{
    if ( unum < 1 || 11 < unum )
    {
        std::cerr << __FILE__ << ':' << __LINE__ << ':'
                  << " movePlayerTo() Illegal unum " << unum
                  << std::endl;
        return;
    }

    try
    {
        Vector2D pos = round_coordinates( x, y );

        M_state.players_.at( unum - 1 ) = pos;

        if ( Options::instance().symmetryMode()
             && M_state.ball_.absY() < 0.5
             && M_formation )
        {
            if ( M_formation->isSymmetryType( unum ) )
            {
                int u = M_formation->getSymmetryNumber( unum );
                if ( u != 0 )
                {
                    M_state.players_.at( u - 1 ).assign( x, -y );
                }
            }
            else // if ( M_formation->isSideType( unum ) )
            {
                for ( int u = 1; u <= 11; ++u )
                {
                    if ( M_formation->getSymmetryNumber( u ) == unum )
                    {
                        M_state.players_.at( u - 1 ).assign( x, -y );
                    }
                }
            }
        }
    }
    catch ( std::exception & e )
    {
        std::cerr << __FILE__ << ':' << __LINE__ << ':'
                  << " movePlayerTo() exception caught! " << e.what()
                  << std::endl;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::setConstraintTerminal( const double & x,
                                 const double & y )
{
    Vector2D pos = round_coordinates( x, y );

    M_constraint_terminal_index = size_t( -1 );
    M_constraint_terminal = pos;

    // automatically select terminal vertex
    if ( M_samples )
    {
        SampleDataSet::IndexData d = M_samples->nearestData( pos, 1.0 );
        if ( M_constraint_origin_index != d.first
             && d.second )
        {
            M_constraint_terminal_index = d.first;
            M_constraint_terminal = d.second->ball_;
        }
    }
}


/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::setConstraintIndex( const int origin_idx,
                              const int terminal_idx )
{
    if ( ! M_samples )
    {
        return;
    }

    const size_t origin = static_cast< size_t >( origin_idx );
    const size_t terminal = static_cast< size_t >( terminal_idx );

    if ( origin_idx < 0 )
    {
        M_select_type = NO_SELECT;
        M_select_index = 0;
        M_constraint_origin_index = size_t( -1 );
        M_constraint_terminal_index = size_t( -1 );
        M_constraint_terminal = Vector2D::INVALIDATED;
        return;
    }

    if ( M_samples->dataCont().size() < origin + 1 )
    {
        std::cerr << __FILE__ << ':' << __LINE__ << ':'
                  << " EditData::setConstraintIndex() origin index over range."
                  << " origin=" << origin
                  << " terminal=" << terminal
                  << std::endl;
        return;
    }

    if ( terminal_idx < 0 )
    {
        M_select_type = SELECT_SAMPLE;
        M_select_index = origin;
        M_constraint_origin_index = origin;
        M_constraint_terminal_index = size_t( -1 );
        return;
    }

    if ( M_samples->dataCont().size() < terminal + 1 )
    {
        std::cerr << __FILE__ << ':' << __LINE__ << ':'
                  << " EditData::setConstraintIndex() terminal index over range."
                  << " origin=" << origin
                  << " terminal=" << terminal
                  << std::endl;
        return;
    }

    if ( origin == terminal )
    {
        M_select_type = SELECT_SAMPLE;
        M_select_index = origin;
        M_constraint_origin_index = origin;
        return;
    }

    M_select_type = SELECT_SAMPLE;
    M_select_index = origin;
    M_constraint_origin_index = origin;
    M_constraint_terminal_index = terminal;

    SampleDataSet::DataCont::const_iterator it = M_samples->dataCont().begin();
    std::advance( it, terminal );

    M_constraint_terminal = it->ball_;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
EditData::moveSelectObjectTo( const double & x,
                              const double & y )
{
    switch ( selectType() ) {
    case NO_SELECT:
        return false;
    case SELECT_BALL:
        moveBallTo( x, y );
        return true;
    case SELECT_PLAYER:
        movePlayerTo( selectIndex() + 1, x, y );
        return true;
    case SELECT_SAMPLE:
        setConstraintTerminal( x, y );
        return true;
    default:
        break;
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
EditData::selectObject( const double & x,
                        const double & y )
{
    const Vector2D pos( x, y );
    const double dist2_thr = 1.5 * 1.5;

    double mindist2 = 200.0 * 200.0;

    if ( Options::instance().constraintEditMode() )
    {
        //
        // samples
        //
        if ( M_samples )
        {
            size_t index = 0;
            const SampleDataSet::DataCont::const_iterator d_end = M_samples->dataCont().end();
            for ( SampleDataSet::DataCont::const_iterator d = M_samples->dataCont().begin();
                  d != d_end;
                  ++d, ++index )
            {
                double d2 = d->ball_.dist2( pos );
                if ( d2 < dist2_thr
                     && d2 < mindist2 )
                {
                    M_select_type = SELECT_SAMPLE;
                    M_select_index = index; //std::distance( M_triangulation.indexedVertices().begin(), v );
                    M_constraint_origin_index = index;
                    mindist2 = d2;
                }
            }
        }
    }
    else
    {
        //
        // ball
        //
        {
            double d2 = M_state.ball_.dist2( pos );
            if ( d2 < dist2_thr )
            {
                //std::cerr << "selection update ball" << std::endl;
                M_select_type = SELECT_BALL;
                mindist2 = d2;
            }
        }

        //
        // players
        //
        size_t index = 0;
        for ( std::vector< Vector2D >::iterator it = M_state.players_.begin();
              it != M_state.players_.end();
              ++it, ++index )
        {
            double d2 = it->dist2( pos );
            if ( d2 < dist2_thr
                 && d2 < mindist2 )
            {
                M_select_type = SELECT_PLAYER;
                M_select_index = index; //std::distance( M_state.players_.begin(), it );
                mindist2 = d2;
            }
        }
    }

    if ( M_select_type == SELECT_BALL )
    {
        M_state.ball_ = pos;
    }
    else if ( M_select_type == SELECT_PLAYER )
    {
        M_state.players_[M_select_index] = pos;
    }
    else if ( M_select_type == SELECT_SAMPLE )
    {
        M_constraint_terminal_index = size_t( -1 );
        M_constraint_terminal = Vector2D::INVALIDATED;
    }

    return ( M_select_type != NO_SELECT );
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
EditData::releaseObject()
{
    if ( M_select_type != NO_SELECT )
    {
        M_select_type = NO_SELECT;
        M_constraint_origin_index = size_t( -1 );
        M_constraint_terminal_index = size_t( -1 );
        M_constraint_terminal = Vector2D::INVALIDATED;
        return true;
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

 */
SampleDataSet::ErrorType
EditData::addData()
{
    if ( ! M_formation
         || ! M_samples )
    {
        return SampleDataSet::NO_FORMATION;
    }

    SampleDataSet::ErrorType err
        = M_samples->addData( *M_formation,
                              M_state,
                              Options::instance().symmetryMode() );
    if ( err != SampleDataSet::NO_ERROR )
    {

    }

    M_state = M_samples->dataCont().back();
    M_current_index = M_samples->dataCont().size() - 1;

    train();

    return SampleDataSet::NO_ERROR;
}

/*-------------------------------------------------------------------*/
/*!

 */
SampleDataSet::ErrorType
EditData::insertData( const int idx )
{
    if ( ! M_formation
         || ! M_samples )
    {
        return SampleDataSet::NO_FORMATION;
    }

    if ( idx < 0 )
    {
        return SampleDataSet::INVALID_INDEX;
    }

    SampleDataSet::ErrorType err
        = M_samples->insertData( *M_formation,
                                 static_cast< size_t >( idx ),
                                 M_state,
                                 Options::instance().symmetryMode() );

    if ( err != SampleDataSet::NO_ERROR )
    {
        return err;
    }

    M_current_index = idx;

    train();

    return SampleDataSet::NO_ERROR;
}

/*-------------------------------------------------------------------*/
/*!

 */
SampleDataSet::ErrorType
EditData::replaceData( const int idx )
{
    if ( ! M_formation
         || ! M_samples )
    {
        return SampleDataSet::NO_FORMATION;
    }

    SampleDataSet::ErrorType err
        = M_samples->replaceData( *M_formation,
                                  static_cast< size_t >( idx ),
                                  M_state,
                                  Options::instance().symmetryMode() );

    if ( err != SampleDataSet::NO_ERROR )
    {
        return err;
    }

    train();

    return SampleDataSet::NO_ERROR;
}

/*-------------------------------------------------------------------*/
/*!

 */
SampleDataSet::ErrorType
EditData::replaceBall( const int idx,
                       const double & x,
                       const double & y )
{
    if ( ! M_formation
         || ! M_samples )
    {
        return SampleDataSet::NO_FORMATION;
    }

    const SampleData * d = M_samples->data( static_cast< size_t >( idx ) );

    if ( ! d )
    {
        return SampleDataSet::INVALID_INDEX;
    }

    SampleData tmp = *d;
    tmp.ball_.assign( x, y );

    SampleDataSet::ErrorType err
        = M_samples->replaceData( *M_formation,
                                  static_cast< size_t >( idx ),
                                  tmp,
                                  Options::instance().symmetryMode() );

    if ( err != SampleDataSet::NO_ERROR )
    {
        return err;
    }

    train();

    return SampleDataSet::NO_ERROR;
}

/*-------------------------------------------------------------------*/
/*!

 */
SampleDataSet::ErrorType
EditData::replacePlayer( const int idx,
                         const int unum,
                         const double & x,
                         const double & y )
{
    if ( ! M_formation
         || ! M_samples )
    {
        return SampleDataSet::NO_FORMATION;
    }

    const SampleData * d = M_samples->data( static_cast< size_t >( idx ) );

    if ( ! d )
    {
        return SampleDataSet::INVALID_INDEX;
    }

    SampleData tmp = *d;
    try
    {
        tmp.players_.at( unum - 1 ).assign( x, y );
    }
    catch ( std::exception & e )
    {
        std::cerr << e.what()
                  << ": EditData::replacePlayer() illegal player number. "
                  << unum << std::endl;
        return SampleDataSet::INVALID_INDEX;
    }

    SampleDataSet::ErrorType err
        = M_samples->replaceData( *M_formation,
                                  static_cast< size_t >( idx ),
                                  tmp,
                                  Options::instance().symmetryMode() );

    if ( err != SampleDataSet::NO_ERROR )
    {
        return err;
    }

    train();

    return SampleDataSet::NO_ERROR;
}

/*-------------------------------------------------------------------*/
/*!

 */
SampleDataSet::ErrorType
EditData::deleteData( const int idx )
{
    if ( ! M_formation
         || ! M_samples )
    {
        return SampleDataSet::NO_FORMATION;
    }

    SampleDataSet::ErrorType err
        = M_samples->removeData( static_cast< size_t >( idx ) );

    if ( err != SampleDataSet::NO_ERROR )
    {
        return err;
    }

    M_current_index = -1;

    train();

    return SampleDataSet::NO_ERROR;
}

/*-------------------------------------------------------------------*/
/*!

 */
SampleDataSet::ErrorType
EditData::changeDataIndex( const int old_idx,
                           const int new_idx )
{
    if ( ! M_formation
         || ! M_samples )
    {
        return SampleDataSet::NO_FORMATION;
    }

    size_t old_index = static_cast< size_t >( old_idx );
    size_t new_index = static_cast< size_t >( new_idx );

    if ( new_idx > old_idx )
    {
        new_index += 1;
    }

    SampleDataSet::ErrorType err = M_samples->changeDataIndex( old_index, new_index );

    if ( err != SampleDataSet::NO_ERROR )
    {
        return err;
    }

    std::cerr << "move data from " << old_idx << " to " << new_idx
              << std::endl;

    M_current_index = new_idx;

    train();

    return SampleDataSet::NO_ERROR;
}

/*-------------------------------------------------------------------*/
/*!

 */
rcsc::formation::SampleDataSet::ErrorType
EditData::addConstraint( const int origin_idx,
                         const int terminal_idx )
{
    if ( ! M_formation
         || ! M_samples )
    {
        return SampleDataSet::NO_FORMATION;
    }

    size_t origin = static_cast< size_t >( std::min( origin_idx, terminal_idx ) );
    size_t terminal = static_cast< size_t >( std::max( origin_idx, terminal_idx ) );

    SampleDataSet::ErrorType err = M_samples->addConstraint( origin, terminal );

    if ( err != SampleDataSet::NO_ERROR )
    {
        return err;
    }

    std::cerr << "add constraint (" << origin << ',' << terminal << ')'
              << std::endl;

    train();

    return SampleDataSet::NO_ERROR;
}

/*-------------------------------------------------------------------*/
/*!

 */
rcsc::formation::SampleDataSet::ErrorType
EditData::replaceConstraint( const int idx,
                             const int origin_idx,
                             const int terminal_idx )
{
    if ( ! M_formation
         || ! M_samples )
    {
        return SampleDataSet::NO_FORMATION;
    }

    SampleDataSet::ErrorType err
        = M_samples->replaceConstraint( static_cast< size_t >( idx ),
                                        static_cast< size_t >( origin_idx ),
                                        static_cast< size_t >( terminal_idx ) );

    if ( err != SampleDataSet::NO_ERROR )
    {
        return err;
    }

    std::cerr << "replace constraint " << idx
              << " to (" << origin_idx << ',' << terminal_idx << ')'
              << std::endl;

    train();

    return SampleDataSet::NO_ERROR;
}

/*-------------------------------------------------------------------*/
/*!

 */
rcsc::formation::SampleDataSet::ErrorType
EditData::deleteConstraint( const int origin_idx,
                            const int terminal_idx )
{
    if ( ! M_formation
         || ! M_samples )
    {
        return SampleDataSet::NO_FORMATION;
    }

    SampleDataSet::ErrorType err
        = M_samples->removeConstraint( static_cast< size_t >( origin_idx ),
                                       static_cast< size_t >( terminal_idx ) );

    if ( err != SampleDataSet::NO_ERROR )
    {
        return err;
    }

    std::cerr << "delete constraint (" << origin_idx << ',' << terminal_idx << ')'
              << std::endl;

    train();

    return SampleDataSet::NO_ERROR;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
EditData::setCurrentIndex( const int idx )
{
    if ( ! M_formation
         || ! M_samples )
    {
        return false;
    }

    if ( idx == -1 )
    {
        M_current_index = idx;
        return true;
    }

    const SampleData * d = M_samples->data( idx );
    if ( ! d )
    {
        std::cerr << __FILE__ << ':' << __LINE__ << ':'
                  << "Index range over. " << idx
                  << std::endl;
        return false;
    }

    M_current_index = idx;

    if ( Options::instance().playerAutoMove() )
    {
        M_state = *d;
    }
    else
    {
        M_state.ball_ = d->ball_;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::reverseY()
{
    if ( ! M_formation )
    {
        std::cerr << __FILE__ << ':' << __LINE__ << ':'
                  << " ***ERROR*** No formation!!"
                  << std::endl;
        return;
    }

    M_state.ball_.y *= -1.0;

    std::vector< Vector2D > old_positions = M_state.players_;

    int unum = 1;
    for ( std::vector< Vector2D >::iterator p = M_state.players_.begin();
          p != M_state.players_.end();
          ++p, ++unum )
    {
        if ( M_formation->isCenterType( unum ) )
        {
            p->y *= -1.0;
        }
        else if ( M_formation->isSymmetryType( unum ) )
        {
            int symmetry_unum = M_formation->getSymmetryNumber( unum );
            if ( symmetry_unum == 0 ) continue;
            p->x = old_positions[symmetry_unum - 1].x;
            p->y = old_positions[symmetry_unum - 1].y * -1.0;
        }
        else if ( M_formation->isSideType( unum ) )
        {
            p->y *= -1.0;
            for ( int iunum = 1; iunum <= 11; ++iunum )
            {
                if ( M_formation->getSymmetryNumber( iunum ) == unum )
                {
                    p->x = old_positions[iunum - 1].x;
                    p->y = old_positions[iunum - 1].y * -1.0;
                }
            }
        }
    }

#if 0
    //
    // update index
    //
    if ( Options::instance().dataAutoSelect() )
    {
        int index = 0;
        const SampleDataSet::DataCont::const_iterator end = M_samples.dataCont().end();
        for ( SampleDataSet::DataCont::const_iterator it = M_samples.dataCont().begin();
              it != end;
              ++it, ++index )
        {
            SampleDataSet::IndexData d = M_samples.nearestData( pos, 1.0 );
            if ( d.second )
            {
                M_current_index = d.first;
                M_state = *(d.second);
            }
        }
    }
#endif
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EditData::train()
{
    if ( ! M_formation )
    {
        return;
    }

    M_formation->train();
    M_conf_changed = true;
    updatePlayerPosition();
    updateTriangulation();
}
