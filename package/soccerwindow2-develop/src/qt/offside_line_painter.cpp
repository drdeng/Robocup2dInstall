// -*-c++-*-

/*!
  \file offside_line_painter.cpp
  \brief offside line painter class Source File.
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

#include "offside_line_painter.h"

#include "draw_config.h"
// model
#include "options.h"
#include "main_data.h"
#include "monitor_view_data.h"

#include <rcsc/common/server_param.h>

/*-------------------------------------------------------------------*/
/*!

*/
void
OffsideLinePainter::draw( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( ! opt.showOffsideLine() )
    {
        return;
    }

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    if ( ! view )
    {
        return;
    }

    const double ball_x = view->ball().x();
    const std::vector< rcsc::rcg::PlayerT > & players = view->players();

    double offside_l = 0.0;
    {
        double min_x = 0.0;
        for ( std::size_t i = 0; i < 11; ++i )
        {
            if ( players[i].isAlive() )
            {
                double x = players[i].x();
                if ( x < offside_l )
                {
                    if ( x < min_x )
                    {
                        offside_l = min_x;
                        min_x = x;
                    }
                    else
                    {
                        offside_l = x;
                    }
                }
            }
        }
        offside_l = std::min( offside_l, ball_x );
        offside_l = std::max( offside_l, - rcsc::ServerParam::i().pitchHalfLength() );
    }
    const double offside_line_l = opt.screenX( offside_l );

    double offside_r = 0.0;
    {
        double max_x = 0.0;
        for ( std::size_t i = 11; i < 22; ++i )
        {
            if ( players[i].isAlive() )
            {
                double x = players[i].x();
                if ( offside_r < x )
                {
                    if ( max_x < x )
                    {
                        offside_r = max_x;
                        max_x = x;
                    }
                    else
                    {
                        offside_r = x;
                    }
                }
            }
        }
        offside_r = std::max( offside_r, ball_x );
        offside_r = std::min( offside_r, rcsc::ServerParam::i().pitchHalfLength() );
    }
    const double offside_line_r = opt.screenX( offside_r );

    const double top_y = opt.screenY( rcsc::ServerParam::i().pitchHalfWidth() );
    const double bottom_y = opt.screenY( - rcsc::ServerParam::i().pitchHalfWidth() );

    const DrawConfig & dconf = DrawConfig::instance();

    painter.setBrush( dconf.transparentBrush() );

    painter.setPen( ( opt.reverseSide()
                      ? dconf.rightTeamPen()
                      : dconf.leftTeamPen() ) );
    painter.drawLine( QLineF( offside_line_l, top_y,
                              offside_line_l, bottom_y ) );

    painter.setPen( ( opt.reverseSide()
                      ? dconf.leftTeamPen()
                      : dconf.rightTeamPen() ) );
    painter.drawLine( QLineF( offside_line_r, top_y,
                              offside_line_r, bottom_y ) );
}
