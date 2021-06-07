// -*-c++-*-

/*!
  \file team_graphic_painter.cpp
  \brief team logo image painter class Source File.
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

#include "team_graphic_painter.h"

// model
#include "options.h"
#include "main_data.h"

#include <iostream>
#include <cstring>

/*-------------------------------------------------------------------*/
/*!

*/
TeamGraphicPainter::TeamGraphicPainter( const MainData & main_data )
    : M_main_data( main_data )
{

}

/*-------------------------------------------------------------------*/
/*!

*/
void
TeamGraphicPainter::draw( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( opt.anonymousMode()
         || ! opt.showTeamGraphic() )
    {
        return;
    }

    // update left team pixmap
    if ( M_team_graphic_left_set.size()
         != M_main_data.viewHolder().teamGraphicLeft().tiles().size() )
    {
        if ( M_main_data.viewHolder().teamGraphicLeft().tiles().empty() )
        {
            M_team_graphic_left_set.clear();
            M_team_graphic_pixmap_left = QPixmap();
        }
        else
        {
            copyTeamGraphic( M_team_graphic_pixmap_left,
                             M_team_graphic_left_set,
                             M_main_data.viewHolder().teamGraphicLeft() );
        }
    }

    // update right team pixmap
    if ( M_team_graphic_right_set.size()
         != M_main_data.viewHolder().teamGraphicRight().tiles().size() )
    {
        if ( M_main_data.viewHolder().teamGraphicRight().tiles().empty() )
        {
            M_team_graphic_right_set.clear();
            M_team_graphic_pixmap_right = QPixmap();
        }
        else
        {
            copyTeamGraphic( M_team_graphic_pixmap_right,
                             M_team_graphic_right_set,
                             M_main_data.viewHolder().teamGraphicRight() );
        }
    }

    const double scale = Options::instance().teamGraphicScale();

    if ( ! M_team_graphic_pixmap_left.isNull() )
    {
        double left_x = 0;
        if ( opt.reverseSide() )
        {
            left_x = opt.canvasWidth() - M_team_graphic_pixmap_left.width() * scale;
        }

        QRectF source_rect = M_team_graphic_pixmap_left.rect();
        QRectF dest_rect( left_x, opt.scoreBoardHeight(),
                          source_rect.width() * scale,
                          source_rect.height() * scale );
        painter.drawPixmap( dest_rect, M_team_graphic_pixmap_left, source_rect );
        // painter.drawPixmap( QPoint( left_x, opt.scoreBoardHeight() ),
        //                     M_team_graphic_pixmap_left );
    }

    if ( ! M_team_graphic_pixmap_right.isNull() )
    {
        double left_x = opt.canvasWidth() - M_team_graphic_pixmap_right.width() * scale;
        if ( opt.reverseSide() )
        {
            left_x = 0;
        }

        QRectF source_rect = M_team_graphic_pixmap_right.rect();
        QRectF dest_rect( left_x, opt.scoreBoardHeight(),
                          source_rect.width() * scale,
                          source_rect.height() * scale );
        painter.drawPixmap( dest_rect, M_team_graphic_pixmap_right, source_rect );
        // painter.drawPixmap( QPoint( left_x, opt.scoreBoardHeight() ),
        //                     M_team_graphic_pixmap_right );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
TeamGraphicPainter::copyTeamGraphic( QPixmap & dst_pixmap,
                                     std::set< rcsc::TeamGraphic::Index > & index_set,
                                     const rcsc::TeamGraphic & team_graphic )
{
    if ( dst_pixmap.width() != team_graphic.width()
         || dst_pixmap.height() != team_graphic.height() )
    {
        dst_pixmap = QPixmap( team_graphic.width(),
                              team_graphic.height() );
        dst_pixmap.fill( Qt::transparent );
    }

    const rcsc::TeamGraphic::Map::const_reverse_iterator end = team_graphic.tiles().rend();
    for ( rcsc::TeamGraphic::Map::const_reverse_iterator tile = team_graphic.tiles().rbegin();
          tile != end;
          ++tile )
    {
        index_set.insert( tile->first );

        copyTeamGraphicXpmTile( dst_pixmap,
                                tile->first,
                                *(tile->second) );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
TeamGraphicPainter::copyTeamGraphicXpmTile( QPixmap & dst_pixmap,
                                            const rcsc::TeamGraphic::Index & index,
                                            const rcsc::TeamGraphic::XpmTile & tile )
{
    const int array_size = 1 + tile.colors().size() + tile.height();

    char ** xpm;
    xpm = new char*[ array_size ];

    // header
    xpm[0] = new char[64];
    snprintf( xpm[0], 64, "%d %d %d %d",
              tile.width(), tile.height(),
              static_cast< int >( tile.colors().size() ),
              tile.cpp() );

    // colors
    std::size_t idx = 1;
    for ( std::vector< boost::shared_ptr< std::string > >::const_iterator col = tile.colors().begin();
          col != tile.colors().end();
          ++col )
    {
        xpm[idx] = new char[ (*col)->length() + 1 ];
        std::strcpy( xpm[idx], (*col)->c_str() );
        ++idx;
    }

    // pixels
    for ( std::vector< std::string >::const_iterator line = tile.pixelLines().begin();
          line != tile.pixelLines().end();
          ++line )
    {
        xpm[idx] = new char[ tile.width() + 1 ];
        std::strcpy( xpm[idx], line->c_str() );
        ++idx;
    }

    copyTeamGraphicXpmTile( dst_pixmap, index.first, index.second, xpm );

    // release memories
    for ( int i = 0; i < array_size; ++i )
    {
        delete [] xpm[i];
    }
    delete [] xpm;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
TeamGraphicPainter::copyTeamGraphicXpmTile( QPixmap & dst_pixmap,
                                            const int x,
                                            const int y,
                                            const char * const * xpm )
{
    if ( dst_pixmap.width() < (x+1) * rcsc::TeamGraphic::TILE_SIZE
         || dst_pixmap.height() < (y+1) * rcsc::TeamGraphic::TILE_SIZE )
    {
        return;
    }

    QPixmap pixmap( xpm );

    if ( pixmap.isNull()
         || pixmap.width() != rcsc::TeamGraphic::TILE_SIZE
         || pixmap.height() != rcsc::TeamGraphic::TILE_SIZE )
    {
        std::cerr << __FILE__ << ": (copyTeamGraphicXpmTile) "
                  << "Failed to create the appropriate pixmap from the xpm tile ("
                  << x << ',' << y << ')'
                  << std::endl;
        return;
    }

    QPainter painter( &dst_pixmap );
    painter.drawPixmap( QPoint( x * 8, y * 8 ), pixmap );
    painter.end();
}
