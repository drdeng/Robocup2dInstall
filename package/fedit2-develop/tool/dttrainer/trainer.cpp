// -*-c++-*-

/*!
  \file trainer.cpp
  \brief trainer for FormationDT Source File.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "trainer.h"

#include <rcsc/geom/line_2d.h>
#include <rcsc/geom/segment_2d.h>

#include <iostream>
#include <fstream>
#include <cstdio>

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

 */
Trainer::Trainer()
    : M_alpha( 0.1 ),
      M_error_thr( 0.1 ),
      M_max_loop( 500 )
{

}

/*-------------------------------------------------------------------*/
/*!

 */
bool
Trainer::readFormation( const std::string & filepath )
{
    std::ifstream fin( filepath.c_str() );

    if ( ! fin )
    {
        return false;
    }

    if ( ! M_formation.read( fin ) )
    {
        std::cerr << "Failed to read formation data" << std::endl;
        return false;
    }

    M_target_data = M_formation.sampleVector();

    fin.close();

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
Trainer::readTrainingData( const std::string & filepath )
{
    // format:
    //
    //  (b <X> <Y>) (<UNUM> <X> <Y>)
    //

    std::ifstream fin( filepath.c_str() );

    if ( ! fin )
    {
        return false;
    }

    std::string line;
    int n_line = 0;
    while ( std::getline( fin, line ) )
    {
        ++n_line;

        int unum;
        double bx, by, px, py;
        if ( std::sscanf( line.c_str(),
                          " ( b %lf %lf ) ( %d %lf %lf )",
                          &bx, &by, &unum, &px, &py ) != 5 )
        {
            std::cerr << "line " << n_line
                      << " illegal data [" << line << "]" << std::endl;
            return false;
        }

        if ( unum < 0 || 11 < unum )
        {
            std::cerr << "line " << n_line
                      << " illegal unum [" << line << "]" << std::endl;
            return false;
        }

        M_training_data.push_back( Data( bx, by, unum, px, py ) );
    }

    return true;
}


/*-------------------------------------------------------------------*/
/*!

 */
std::ostream &
Trainer::printFormation( std::ostream & os ) const
{
    M_formation.print( os );
    return os;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Trainer::train()
{
    for ( int i = 0; i < M_max_loop; ++i )
    {
        double max_error = 0.0;

        std::cerr << "loop " << i + 1 << std::endl;

        for ( std::vector< Data >::const_iterator it = M_training_data.begin(), end = M_training_data.end();
              it != end;
              ++it )
        {
            double error_value = train( *it );
            if ( error_value >= 0.0
                 && error_value > max_error )
            {
                max_error = error_value;
            }
        }

         M_formation.train();

        if ( max_error < M_error_thr )
        {
            break;
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
double
Trainer::train( const Data & data )
{
    const DelaunayTriangulation::Triangle * t = M_formation.triangulation().findTriangleContains( data.ball_ );

    if ( ! t )
    {
        std::cerr << "Could not find the triangle that contains " << data.ball_ << std::endl;
        return -1.0;
    }

    const Vector2D old_pos = M_formation.getPosition( data.unum_, data.ball_ );

    if ( ! old_pos.isValid() )
    {
        std::cerr << "Could not calculate the position."
                  << " ball=" << data.ball_ << " unum=" << data.unum_ << std::endl;
        return -1.0;
    }

    size_t idx0 = size_t( t->vertex( 0 )->id() );
    size_t idx1 = size_t( t->vertex( 1 )->id() );
    size_t idx2 = size_t( t->vertex( 2 )->id() );

    if ( idx0 >= M_target_data.size()
         || idx1 >= M_target_data.size()
         || idx2 >= M_target_data.size() )
    {
        std::cerr << "Could not find the target vertex ball=" << data.ball_ << std::endl;
        return -1.0;
    }
    Vector2D diff = data.pos_ - old_pos;
    double error_value = diff.r();

#if 1
    std::cerr << "train:\n"
              << " ball=" << data.ball_  << " unum=" << data.unum_ << data.pos_ << '\n';
    std::cerr << " target vertices: " << idx0 << ' ' << idx1 << ' ' << idx2 << '\n';
    std::cerr << " diff=" << diff << " error=" << error_value << std::endl;
#endif

    diff *= M_alpha;
    M_target_data[idx0].players_[data.unum_ - 1] += diff;
    M_target_data[idx1].players_[data.unum_ - 1] += diff;
    M_target_data[idx2].players_[data.unum_ - 1] += diff;

    formation::SampleDataSet::Ptr ptr = M_formation.samples();
    ptr->replaceData( M_formation, idx0, M_target_data[idx0], true );
    ptr->replaceData( M_formation, idx1, M_target_data[idx1], true );
    ptr->replaceData( M_formation, idx2, M_target_data[idx2], true );

    return error_value;
}
