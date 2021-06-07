// -*-c++-*-

/*!
  \file voronoi_diagram_painter.cpp
  \brief Voronoi Diagram & Delaunay Triangulation painter class Source File.
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

#include "voronoi_diagram_painter.h"

#include "draw_config.h"
// model
#include "options.h"
#include "main_data.h"

#include <rcsc/common/server_param.h>
#include <rcsc/geom/line_2d.h>
#include <rcsc/geom/rect_2d.h>
#include <rcsc/geom/voronoi_diagram.h>
#include <rcsc/geom/delaunay_triangulation.h>
#include <rcsc/geom/triangulation.h>
#ifndef NO_TIMER
#include <rcsc/timer.h>
#endif

/*-------------------------------------------------------------------*/
/*!

 */
namespace  {

/*!

 */
struct VoronoiEdge {
    std::size_t player1;
    std::size_t player2;
    rcsc::Vector2D point1;
    rcsc::Vector2D point2;

    VoronoiEdge( std::size_t i1,
                 std::size_t i2,
                 const double & p1x,
                 const double & p1y,
                 const double & p2x,
                 const double & p2y )
        : player1( i1 )
        , player2( i2 )
        , point1( p1x, p1y )
        , point2( p2x, p2y )
      { }
};

}


/*-------------------------------------------------------------------*/
/*!

 */
void
VoronoiDiagramPainter::draw( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( ! opt.showVoronoiDiagram()
         && ! opt.showDelaunayTriangulation() )
    {
        return;
    }

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();
    if ( ! view )
    {
        return;
    }

    //
    // create player positions
    //
    std::vector< rcsc::Vector2D > players;
    players.reserve( 22 );

    for ( std::vector< rcsc::rcg::PlayerT >::const_iterator p = view->players().begin(),
              p_end = view->players().end();
          p != p_end;
          ++p )
    {
        if ( opt.voronoiTarget() != rcsc::NEUTRAL
             && p->side() != opt.voronoiTarget() )
        {
            continue;
        }

        if ( p->isAlive() )
        {
            players.push_back( rcsc::Vector2D( p->x() * opt.reverseValue(),
                                               p->y() * opt.reverseValue() ) );
        }
    }

    //
    // create and draw voronoi diagram
    //
    if ( opt.showVoronoiDiagram() )
    {
        drawVoronoiDiagram( painter, players );
    }

    //
    // create and draw delaunay triangulation
    //
    if ( opt.showDelaunayTriangulation() )
    {
        drawDelaunayTriangulation( painter, players );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
VoronoiDiagramPainter::drawVoronoiDiagram( QPainter & painter,
                                           const std::vector< rcsc::Vector2D > & players )
{
    const Options & opt = Options::instance();

    const rcsc::Rect2D PITCH_RECT( rcsc::Vector2D( - rcsc::ServerParam::i().pitchHalfLength(),
                                                   - rcsc::ServerParam::i().pitchHalfWidth() ),
                                   rcsc::Size2D( rcsc::ServerParam::i().pitchLength(),
                                                 rcsc::ServerParam::i().pitchWidth() ) );
    //rcsc::Timer timer;
    rcsc::VoronoiDiagram voronoi( players );
    voronoi.setBoundingRect( PITCH_RECT );
    voronoi.compute();
    //std::cerr << "voronoi elapsed " << timer.elapsedReal() << " [ms]." << std::endl;


    QPainterPath path;

    painter.setPen( DrawConfig::instance().measurePen() );
    painter.setBrush( DrawConfig::instance().transparentBrush() );

    const rcsc::VoronoiDiagram::Segment2DCont::const_iterator s_end = voronoi.resultSegments().end();
    for ( rcsc::VoronoiDiagram::Segment2DCont::const_iterator s = voronoi.resultSegments().begin();
          s != s_end;
          ++s )
    {
        rcsc::Vector2D pos1, pos2;
        int n = PITCH_RECT.intersection( *s, &pos1, &pos2 );
        if ( n == 2 )
        {
            path.moveTo( opt.absScreenX( pos1.x ),
                         opt.absScreenY( pos1.y ) );
            path.lineTo( opt.absScreenX( pos2.x ),
                         opt.absScreenY( pos2.y ) );
        }
        else if ( n == 1 )
        {
            if ( PITCH_RECT.contains( s->origin() ) )
            {
                path.moveTo( opt.absScreenX( s->origin().x ),
                             opt.absScreenY( s->origin().y ) );
                path.lineTo( opt.absScreenX( pos1.x ),
                             opt.absScreenY( pos1.y ) );
            }
            else
            {
                path.moveTo( opt.absScreenX( s->terminal().x ),
                             opt.absScreenY( s->terminal().y ) );
                path.lineTo( opt.absScreenX( pos1.x ),
                             opt.absScreenY( pos1.y ) );
            }
        }
        else if ( PITCH_RECT.contains( s->origin() ) )
        {
            path.moveTo( opt.absScreenX( s->origin().x ),
                         opt.absScreenY( s->origin().y ) );
            path.lineTo( opt.absScreenX( s->terminal().x ),
                         opt.absScreenY( s->terminal().y ) );
        }
    }

#if 0
    const rcsc::VoronoiDiagram::Ray2DCont::const_iterator r_end = voronoi.resultRays().end();
    for ( rcsc::VoronoiDiagram::Ray2DCont::const_iterator r = voronoi.resultRays().begin();
          r != r_end;
          ++r )
    {
        rcsc::Vector2D pos;
        if ( PITCH_RECT.intersection( *r, &pos, static_cast< rcsc::Vector2D * >( 0 ) ) == 1 )
        {
            path.moveTo( opt.absScreenX( r->origin().x ),
                         opt.absScreenY( r->origin().y ) );
            path.lineTo( opt.absScreenX( pos.x ),
                         opt.absScreenY( pos.y ) );
        }
    }
#endif
    painter.drawPath( path );

}

/*-------------------------------------------------------------------*/
/*!

 */
void
VoronoiDiagramPainter::drawDelaunayTriangulation( QPainter & painter,
                                                  const std::vector< rcsc::Vector2D > & players )
{
    const Options & opt = Options::instance();

#if 0
    //rcsc::Timer timer;
    rcsc::DelaunayTriangulation triangulation;
    triangulation.init( rcsc::Rect2D( rcsc::Vector2D( - rcsc::ServerParam::i().pitchHalfLength(),
                                                      - rcsc::ServerParam::i().pitchHalfWidth() ),
                                      rcsc::Size2D( rcsc::ServerParam::i().pitchLength(),
                                                    rcsc::ServerParam::i().pitchWidth() ) ) );
    triangulation.addVertices( players );
    triangulation.compute();
    //std::cerr << "delaunay elapsed " << timer.elapsedReal() << " [ms]." << std::endl;

    QPainterPath path;

    painter.setPen( DrawConfig::instance().linePen() );
    painter.setBrush( DrawConfig::instance().transparentBrush() );

    const rcsc::DelaunayTriangulation::EdgeCont::const_iterator e_end = triangulation.edges().end();
    for ( rcsc::DelaunayTriangulation::EdgeCont::const_iterator e = triangulation.edges().begin();
          e != e_end;
          ++e )
    {
        path.moveTo( opt.absScreenX( e->second->vertex( 0 )->pos().x ),
                     opt.absScreenY( e->second->vertex( 0 )->pos().y ) );
        path.lineTo( opt.absScreenX( e->second->vertex( 1 )->pos().x ),
                     opt.absScreenY( e->second->vertex( 1 )->pos().y ) );
    }

    painter.drawPath( path );
#else
    //rcsc::Timer timer;
    rcsc::Triangulation triangulation;
    triangulation.setUseTriangles( false );
    triangulation.addPoints( players );
    triangulation.compute();
    //std::cerr << "delaunay elapsed " << timer.elapsedReal() << " [ms]." << std::endl;

    QPainterPath path;

    painter.setPen( DrawConfig::instance().linePen() );
    painter.setBrush( DrawConfig::instance().transparentBrush() );

    const rcsc::Triangulation::PointCont & points = triangulation.points();
    const rcsc::Triangulation::SegmentCont::const_iterator e_end = triangulation.edges().end();
    for ( rcsc::Triangulation::SegmentCont::const_iterator e = triangulation.edges().begin();
          e != e_end;
          ++e )
    {
        path.moveTo( opt.absScreenX( points[e->first].x ),
                     opt.absScreenY( points[e->first].y ) );
        path.lineTo( opt.absScreenX( points[e->second].x ),
                     opt.absScreenY( points[e->second].y ) );
    }

    painter.drawPath( path );
#endif
}

/*-------------------------------------------------------------------*/
/*!

 */
void
VoronoiDiagramPainter::drawOld( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( ! opt.showVoronoiDiagram()
         && ! opt.showDelaunayTriangulation() )
    {
        return;
    }

    static const rcsc::Rect2D PITCH_RECT( rcsc::Vector2D( - rcsc::ServerParam::i().pitchHalfLength(),
                                                          - rcsc::ServerParam::i().pitchHalfWidth() ),
                                          rcsc::Size2D( rcsc::ServerParam::i().pitchLength(),
                                                        rcsc::ServerParam::i().pitchWidth() ) );
    static const double max_x_abs = 5000.0;
    static const double max_y_abs = 5000.0;
    static const rcsc::Rect2D MAX_RECT( rcsc::Vector2D( - max_x_abs,
                                                        - max_y_abs ),
                                        rcsc::Size2D( max_x_abs * 2.0,
                                                      max_y_abs * 2.0 ) );

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();
    if ( ! view )
    {
        return;
    }

    //rcsc::Timer timer;
    //timer.restart();

    std::vector< rcsc::Vector2D > players_pos;
    players_pos.reserve( 22 );

    {
        for ( std::vector< rcsc::rcg::PlayerT >::const_iterator it = view->players().begin(),
                  end = view->players().end();
              it != end;
              ++it )
        {
            if ( opt.voronoiTarget() != rcsc::NEUTRAL
                 && it->side() != opt.voronoiTarget() )
            {
                continue;
            }
            if ( it->isAlive() )
            {
                players_pos.push_back( rcsc::Vector2D( it->x() * opt.reverseValue(),
                                                       it->y() * opt.reverseValue() ) );
            }
        }
    }

    const std::size_t MAX_POINT = players_pos.size();
    if ( MAX_POINT <= 2 )
    {
        return;
    }

    if ( opt.antiAliasing() )
    {
#ifdef USE_HIGH_QUALITY_ANTIALIASING
        painter.setRenderHints( QPainter::HighQualityAntialiasing |
                                QPainter::Antialiasing,
                                false );
#else
        painter.setRenderHint( QPainter::Antialiasing, false );
#endif
    }

    std::vector< rcsc::Vector2D > segment_points;
    std::vector< boost::shared_ptr< VoronoiEdge > > edges;
    for( std::size_t i = 0; i < MAX_POINT - 1; ++i )
    {
        for ( std::size_t j = i + 1; j < MAX_POINT; ++j )
        {
            segment_points.clear();

            // make candidate line
            const rcsc::Line2D first_perpend
                = rcsc::Line2D::perpendicular_bisector( players_pos[i],
                                                        players_pos[j] );
            rcsc::Vector2D left_edge, right_edge;
            MAX_RECT.intersection( first_perpend, &left_edge, &right_edge );
            segment_points.push_back( left_edge );
            segment_points.push_back( right_edge );

            // to make the segment, check all other points
            // register segment points
            for ( std::size_t k = 0; k < MAX_POINT; ++k )
            {
                if ( k == i || k == j )
                {
                    continue;
                }

                rcsc::Line2D k_perpend
                    = rcsc::Line2D::perpendicular_bisector( players_pos[i],
                                                            players_pos[k] );
                rcsc::Vector2D perpend_intersect
                    = rcsc::Line2D::intersection( first_perpend, k_perpend );
                if ( ! perpend_intersect.isValid() )
                {
                    continue; // intersection does not exist
                }
                if ( perpend_intersect.absX() > max_x_abs
                     || perpend_intersect.absY() > max_y_abs )
                {
                    continue; // intersection is out of pitch
                }
                segment_points.push_back( perpend_intersect );
            }
            // sort segment points
            if ( std::fabs( first_perpend.b() ) < 0.001 )
            {
                std::sort( segment_points.begin(), segment_points.end(),
                           rcsc::Vector2D::YCmp() );
            }
            else
            {
                std::sort( segment_points.begin(), segment_points.end(),
                           rcsc::Vector2D::XCmp() );
            }

            const std::vector< rcsc::Vector2D >::iterator end = segment_points.end() - 1;
            for ( std::vector< rcsc::Vector2D >::iterator it = segment_points.begin();
                  it != end;
                  ++it )
            {
                std::vector< rcsc::Vector2D >::iterator it_next = it + 1;
                rcsc::Vector2D segment_center = *it + *it_next;
                segment_center *= 0.5;
                bool is_draw_segment = true;

                const double dist2_to_first = segment_center.dist2( players_pos[i] );
                for ( std::size_t l = 0; l < MAX_POINT; ++l )
                {
                    if ( l == i || l == j )
                    {
                        continue;
                    }
                    if ( segment_center.dist2( players_pos[l] ) < dist2_to_first )
                    {
                        is_draw_segment = false;
                        break;
                    }
                }

                if ( is_draw_segment )
                {
                    boost::shared_ptr< VoronoiEdge > ptr( new VoronoiEdge( i, j,
                                                                           it->x, it->y,
                                                                           it_next->x, it_next->y ) );
                    if ( ptr->point1.x < PITCH_RECT.left() )
                    {
                        ptr->point1.x = PITCH_RECT.left();
                        ptr->point1.y = first_perpend.getY( ptr->point1.x );
                    }
                    else if ( PITCH_RECT.right() < ptr->point1.x )
                    {
                        ptr->point1.x = PITCH_RECT.right();
                        ptr->point1.y = first_perpend.getY( ptr->point1.x );
                    }
                    if ( ptr->point1.y < PITCH_RECT.top() )
                    {
                        ptr->point1.y = PITCH_RECT.top();
                        ptr->point1.x = first_perpend.getX( ptr->point1.y );
                    }
                    else if ( PITCH_RECT.bottom() < ptr->point1.y )
                    {
                        ptr->point1.y = PITCH_RECT.bottom();
                        ptr->point1.x = first_perpend.getX( ptr->point1.y );
                    }

                    if ( ptr->point2.x < PITCH_RECT.left() )
                    {
                        ptr->point2.x = PITCH_RECT.left();
                        ptr->point2.y = first_perpend.getY( ptr->point2.x );
                    }
                    else if ( PITCH_RECT.right() < ptr->point2.x )
                    {
                        ptr->point2.x = PITCH_RECT.right();
                        ptr->point2.y = first_perpend.getY( ptr->point2.x );
                    }
                    if ( ptr->point2.y < PITCH_RECT.top() )
                    {
                        ptr->point2.y = PITCH_RECT.top();
                        ptr->point2.x = first_perpend.getX( ptr->point2.y );
                    }
                    else if ( PITCH_RECT.bottom() < ptr->point2.y )
                    {
                        ptr->point2.y = PITCH_RECT.bottom();
                        ptr->point2.x = first_perpend.getX( ptr->point2.y );
                    }
                    edges.push_back( ptr );
                }
            }
        }
    }

    //std::cerr << "voronoi elapsed " << timer.elapsedReal() << std::endl;
    if ( opt.showVoronoiDiagram() )
    {
        QPainterPath path;
        painter.setPen( DrawConfig::instance().measurePen() );
        painter.setBrush( DrawConfig::instance().transparentBrush() );

        const std::vector< boost::shared_ptr< VoronoiEdge > >::iterator end = edges.end();
        for ( std::vector< boost::shared_ptr< VoronoiEdge > >::iterator it = edges.begin();
              it != end;
              ++it )
        {
            path.moveTo( opt.absScreenX( (*it)->point1.x ),
                         opt.absScreenY( (*it)->point1.y ) );
            path.lineTo( opt.absScreenX( (*it)->point2.x ),
                         opt.absScreenY( (*it)->point2.y ) );
        }

        painter.drawPath( path );
    }

    if ( opt.showDelaunayTriangulation() )
    {
        QPainterPath path;
        painter.setPen( DrawConfig::instance().linePen() );
        painter.setBrush( DrawConfig::instance().transparentBrush() );

        const std::vector< boost::shared_ptr< VoronoiEdge > >::iterator end = edges.end();
        for ( std::vector< boost::shared_ptr< VoronoiEdge > >::iterator it = edges.begin();
              it != end;
              ++it )
        {
            path.moveTo( opt.absScreenX( players_pos[(*it)->player1].x ),
                         opt.absScreenY( players_pos[(*it)->player1].y ) );
            path.lineTo( opt.absScreenX( players_pos[(*it)->player2].x ),
                         opt.absScreenY( players_pos[(*it)->player2].y ) );
        }

        painter.drawPath( path );
    }


    if ( opt.antiAliasing() )
    {
#ifdef USE_HIGH_QUALITY_ANTIALIASING
        painter.setRenderHints( QPainter::HighQualityAntialiasing |
                                QPainter::Antialiasing,
                                true );
#else
        painter.setRenderHint( QPainter::Antialiasing, true );
#endif
    }
}
