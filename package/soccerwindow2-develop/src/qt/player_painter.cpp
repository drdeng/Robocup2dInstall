// -*-c++-*-

/*!
  \file player_painter.cpp
  \brief player painter class Source File.
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

#include "player_painter.h"

#include "draw_config.h"
// model
#include "options.h"
#include "main_data.h"

#include <rcsc/common/player_type.h>
#include <rcsc/common/server_param.h>
#include <rcsc/geom/angle_deg.h>
#include <rcsc/geom/circle_2d.h>

#include <cstring>
#include <cstdio>
#include <cmath>

/*-------------------------------------------------------------------*/
/*

 */
inline
PlayerPainter::Param::Param( const rcsc::rcg::PlayerT & player,
                             const rcsc::rcg::BallT & ball,
                             const rcsc::PlayerType & ptype )
    : x_( 0.0 ),
      y_( 0.0 ),
      body_( player.body() ),
      head_( player.head() ),
      body_radius_( 1.0 ),
      kick_radius_( 5.0 ),
      have_full_effort_( player.hasFullEffort( ptype.effortMax() ) ),
      player_( player ),
      ball_( ball ),
      player_type_( ptype )
{
    const Options & opt = Options::instance();

    x_ = opt.screenX( player.x() );
    y_ = opt.screenY( player.y() );

    if ( opt.reverseSide() )
    {
        body_ += 180.0;
        head_ += 180.0;
    }

    body_radius_ = opt.scale( ptype.playerSize() );
    kick_radius_ = opt.scale( ptype.kickableArea() );

    if ( body_radius_ < 1.0 ) body_radius_ = 1.0;
    if ( kick_radius_ < 5.0 ) kick_radius_ = 5.0;

    if ( opt.enlargeMode() )
    {
        if ( opt.playerSize() > 0.0 )
        {
            draw_radius_ = opt.scale( opt.playerSize() );
            if ( draw_radius_ < 1 ) draw_radius_ = 1;
        }
        else
        {
            draw_radius_ = kick_radius_;
        }
    }
    else
    {
        draw_radius_ = body_radius_;
    }
}

/*-------------------------------------------------------------------*/
/*

 */
PlayerPainter::PlayerPainter( const MainData & main_data )
    : M_main_data( main_data )
{

}

/*-------------------------------------------------------------------*/
/*

 */
void
PlayerPainter::draw( QPainter & painter )
{
    const Options & opt = Options::instance();

    if ( ! opt.showPlayers() )
    {
        return;
    }

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    if ( ! view )
    {
        return;
    }

    const rcsc::rcg::BallT & ball = view->ball();

    if ( opt.playerReverseDraw() )
    {
        for ( std::vector< rcsc::rcg::PlayerT >::const_reverse_iterator it = view->players().rbegin(),
                  end = view->players().rend();
              it != end;
              ++it )
        {
            drawAll( painter, *it, ball );
        }
    }
    else
    {
        for ( std::vector< rcsc::rcg::PlayerT >::const_iterator it = view->players().begin(),
                  end = view->players().end();
              it != end;
              ++it )
        {
            drawAll( painter, *it, ball );
        }
    }
}

/*-------------------------------------------------------------------*/
/*

 */
void
PlayerPainter::drawAll( QPainter & painter,
                        const rcsc::rcg::PlayerT & player,
                        const rcsc::rcg::BallT & ball ) const
{
    const Options & opt = Options::instance();

    const bool selected = opt.isSelectedAgent( player.side(), player.unum() );
    const Param param( player,
                       ball,
                       M_main_data.viewHolder().playerType( player.type() ) );

    drawBody( painter, param );
    if ( opt.showBodyShadow() )
    {
        drawShadow( painter, param );
    }
    drawEdge( painter, param );

    if ( selected
         && opt.playerFutureCycle() > 0
         && player.hasVelocity() )
    {
        drawFuture( painter, param );
    }

    if ( opt.playerMovableCycle() > 0 )
    {
        drawMovableArea( painter, param );
    }

    if ( player.hasView() )
    {
        if ( opt.showViewArea() )
        {
            drawViewArea( painter, param );
        }
        else
        {
            drawViewDir( painter, param );
        }
    }

    if ( player.isGoalie()
         && opt.showCatchableArea() )
    {
        drawCatchableArea( painter, param );
    }

    if ( opt.showTackleArea() )
    {
        drawTackleArea( painter, param );
    }

    if ( selected
         && opt.showKickAccelArea() )
    {
        drawKickAccelArea( painter, param );
    }

    if ( opt.showPointto() )
    {
        drawPointto( painter, param );
    }

    if ( opt.showAttentionto() )
    {
        drawAttentionto( painter, param );
    }

    drawText( painter, param );
}

/*-------------------------------------------------------------------*/
/*

 */
void
PlayerPainter::drawBody( QPainter & painter,
                         const PlayerPainter::Param & param ) const
{
    const DrawConfig & dconf = DrawConfig::instance();

    // decide base color
    painter.setPen( dconf.playerPen() );

    rcsc::SideID side = param.player_.side();
    if ( Options::instance().reverseSide() )
    {
        side = static_cast< rcsc::SideID >( -1 * side );
    }

    switch ( side ) {
    case rcsc::LEFT:
        if ( param.player_.isGoalie() )
        {
            painter.setBrush( dconf.leftGoalieBrush() );
        }
        else
        {
            painter.setBrush( dconf.leftTeamBrush() );
        }
        break;
    case rcsc::RIGHT:
        if ( param.player_.isGoalie() )
        {
            painter.setBrush( dconf.rightGoalieBrush() );
        }
        else
        {
            painter.setBrush( dconf.rightTeamBrush() );
        }
        break;
    case rcsc::NEUTRAL:
        painter.setBrush( dconf.shadowBrush() );
        break;
    default:
        painter.setBrush( dconf.shadowBrush() );
        break;
    }


    // decide status color
    if ( ! param.player_.isAlive() )
    {
        painter.setBrush( dconf.shadowBrush() );
    }

    if ( param.player_.isIllegalDefenseState()
         && Options::instance().showIllegalDefenseState() )
    {
        painter.setPen( dconf.illegalDefensePen() );
    }

    if ( param.player_.isKickingFault() )
    {
        painter.setPen( dconf.kickFaultPen() );
        painter.setBrush( dconf.kickFaultBrush() );
    }
    else if ( param.player_.isKicking() )
    {
        painter.setPen( dconf.kickPen() );
    }

    if ( param.player_.isCatchingFault() )
    {
        painter.setBrush( dconf.catchFaultBrush() );
    }
    else if ( param.player_.isCatching() )
    {
        painter.setBrush( dconf.catchBrush() );
    }

    if ( param.player_.isTacklingFault() )
    {
        painter.setPen( dconf.tacklePen() );
        painter.setBrush( dconf.tackleFaultBrush() );
    }
    else if ( param.player_.isTackling() )
    {
        painter.setPen( dconf.tacklePen() );
        painter.setBrush( dconf.tackleBrush() );
    }

    if ( param.player_.isFoulCharged() )
    {
        painter.setBrush( dconf.foulChargedBrush() );
    }

    if ( param.player_.isCollidedBall() )
    {
        painter.setBrush( dconf.ballCollisionBrush() );
    }

    if ( param.player_.isCollidedPlayer() )
    {
        painter.setBrush( dconf.playerCollisionBrush() );
    }

    if ( Options::instance().gradient() )
    {
        QRadialGradient gradient( param.x_,
                                  param.y_,
                                  param.draw_radius_,
                                  param.x_ - param.draw_radius_,
                                  param.y_ - param.draw_radius_ );
        //param.x_ - param.draw_radius_*0.8,
        //param.y_ - param.draw_radius_*0.8 );
        //gradient.setColorAt( 0.0, painter.brush().color().lighter( 170 ) );
        gradient.setColorAt( 0.0, painter.brush().color().lighter( 160 ) );
        gradient.setColorAt( 1.0, painter.brush().color() );
        painter.setBrush( gradient );
    }

    painter.drawEllipse( QRectF( param.x_ - param.draw_radius_ ,
                                 param.y_ - param.draw_radius_ ,
                                 param.draw_radius_ * 2,
                                 param.draw_radius_ * 2 ) );
}

/*-------------------------------------------------------------------*/
/*

 */
void
PlayerPainter::drawShadow( QPainter & painter,
                           const PlayerPainter::Param & param ) const
{
    const DrawConfig & dconf = DrawConfig::instance();

    // set size
    double shadow_radius = param.draw_radius_ * 0.9;

    // decide shadow color
#if 0
    double stamina_rate = ( param.player_.hasStamina()
                            ? ( param.player_.stamina()
                                / param.server_param_.staminaMax() )
                            : 1.0 );
    const QColour & base_col = dconf.playerShadowBrush().color();
    int r = 255 - static_cast< int >( (255 - base_col.red()) * stamina_rate );
    int g = 255 - static_cast< int >( (255 - base_col.green()) * stamina_rate );
    int b = 255 - static_cast< int >( (255 - base_col.blue()) * stamina_rate );
    QBrush tmp_brush( QColour( r, g, b ), Qt::SolidPattern );
    painter.setPen( dconf.transparentPen() );
    painter.setBrush( tmp_brush );
#elif 0
    // create temporary brush
    int col = 255
        - static_cast< int >( 255
                              * ( param.player_.hasStamina()
                                  ? ( param.player_.stamina()
                                      / rcsc::ServerParam::i().staminaMax() )
                                  : 1.0 ) );
    //QBrush tmp_brush( QColor( col, col, col ), Qt::SolidPattern );
    painter.setPen( dconf.transparentPen() );
    //painter.setBrush( tmp_brush );
    painter.setBrush( dconf.shadowBrush( col ) );
#else
    painter.setPen( dconf.transparentPen() );
    double stamina_rate = ( param.player_.hasStamina()
                            ? ( param.player_.stamina()
                                / rcsc::ServerParam::i().staminaMax() )
                            : 1.0 );
    //int level = 255 - (int)rint( 255 * rint( stamina_rate * 16.0 ) / 16.0 );
    int level = 255 - (int)rint( 255 * rint( stamina_rate * 8.0 ) / 8.0 );
    painter.setBrush( dconf.shadowBrush( level ) );
#endif

    if ( Options::instance().gradient() )
    {
        QRadialGradient gradient( param.x_,
                                  param.y_,
                                  param.draw_radius_,
                                  param.x_ - param.draw_radius_,
                                  param.y_ - param.draw_radius_ );
        gradient.setColorAt( 0.0, Qt::lightGray );
        gradient.setColorAt( 1.0, painter.brush().color() );
        painter.setBrush( gradient );
    }

    // draw half circle of shadow
#if 0
    QPainterPath shadow_path;
    shadow_path.moveTo( param.x_, param.y_ );
    shadow_path.arcTo( param.x_ - shadow_radius,
                       param.y_ - shadow_radius,
                       shadow_radius * 2,
                       shadow_radius * 2,
                       ( - param.body_ + 90.0 ),
                       180.0 );

    painter.drawPath( shadow_path );
#else
    // set angle
    int shadow_start_dir
        = static_cast< int >( rint( ( - param.body_ + 90.0 ) * 16 ) );
    //double shadow_end_dir = shadow_start_dir + 180.0;
    painter.drawPie( QRectF( param.x_ - shadow_radius, // left x
                             param.y_ - shadow_radius, // top y
                             shadow_radius * 2, // width
                             shadow_radius * 2 ), // height
                     shadow_start_dir,
                     180 * 16 ); // span angle
#endif
}

/*-------------------------------------------------------------------*/
/*!

 */
void
PlayerPainter::drawEdge( QPainter & painter,
                         const PlayerPainter::Param & param ) const
{
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    double edge_radius = 1;
    if ( opt.enlargeMode() )
    {
        if ( opt.showBodyShadow() )
        {
            painter.setPen( dconf.realBodyPen() );
        }
        else
        {
            painter.setPen( dconf.playerPen() );
        }

        edge_radius = param.body_radius_;
    }
    else
    {
        // draw kickable area edge
        rcsc::SideID side = param.player_.side();
        if ( opt.reverseSide() )
        {
            side = static_cast< rcsc::SideID >( -1 * side );
        }

        if ( side == rcsc::LEFT )
        {
            painter.setPen( dconf.leftTeamPen() );
        }
        else
        {
            painter.setPen( dconf.rightTeamPen() );
        }

        edge_radius = param.kick_radius_;
    }

    painter.setBrush( dconf.transparentBrush() );
    painter.drawEllipse( QRectF( param.x_ - edge_radius,
                                 param.y_ - edge_radius,
                                 edge_radius * 2,
                                 edge_radius * 2 ) );

    // body direction line
    if ( ! opt.showBodyShadow() )
    {
        double end_x = opt.absScreenX( param.player_.x() * opt.reverseValue()
                                        + param.player_type_.kickableArea()
                                    * std::cos( param.body_ * rcsc::AngleDeg::DEG2RAD ) );
        double end_y = opt.absScreenY( param.player_.y() * opt.reverseValue()
                                        + param.player_type_.kickableArea()
                                        * std::sin( param.body_ * rcsc::AngleDeg::DEG2RAD ) );
        // same pen & brush
        painter.drawLine( QLineF( param.x_, param.y_, end_x, end_y ) );
    }

    // draw stamina status if effort or recovery is decayed.
    if ( param.player_.hasStamina() )
    {
        if ( ! param.have_full_effort_ )
        {
            painter.setPen( dconf.effortDecayedPen() );
            painter.setBrush( dconf.transparentBrush() );
            double radius = param.draw_radius_ + 2;
            painter.drawEllipse( QRectF( param.x_ - radius,
                                         param.y_ - radius,
                                         radius * 2,
                                         radius * 2 ) );
        }
        else if ( ! param.player_.hasFullRecovery() )
        {
            painter.setPen( dconf.recoveryDecayedPen() );
            painter.setBrush( dconf.transparentBrush() );
            double radius = param.draw_radius_ + 2;
            painter.drawEllipse( QRectF( param.x_ - radius,
                                         param.y_ - radius,
                                         radius * 2,
                                         radius * 2 ) );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
PlayerPainter::drawFuture( QPainter & painter,
                           const PlayerPainter::Param & param ) const
{
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

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

    // draw future state
    rcsc::Vector2D ppos( param.player_.x(), param.player_.y() );
    rcsc::Vector2D pvel( param.player_.deltaX(), param.player_.deltaY() );
    if ( opt.reverseSide() )
    {
        ppos *= -1.0;
        pvel *= -1.0;
    }

    const int last = opt.playerFutureCycle();

    QPointF first_point( opt.absScreenX( ppos.x ),
                         opt.absScreenY( ppos.y ) ) ;
    QPointF last_point = first_point;

    // draw kickable area edge
    rcsc::SideID side = param.player_.side();
    if ( opt.reverseSide() )
    {
        side = static_cast< rcsc::SideID >( -1 * side );
    }

    if ( side == rcsc::LEFT )
    {
        painter.setPen( dconf.rightTeamPen() );
    }
    else
    {
        painter.setPen( dconf.leftTeamPen() );
    }

    painter.setBrush( dconf.transparentBrush() );

    QPainterPath path;
    for ( int i = 0; i < last; ++i )
    {
        ppos += pvel;
        pvel *= param.player_type_.playerDecay();

        QPointF pt( opt.absScreenX( ppos.x ),
                    opt.absScreenY( ppos.y ) );
        if ( std::fabs( last_point.x() - pt.x() ) < 1
             && std::fabs( last_point.y() - pt.y() ) < 1 )
        {
            break;
        }

        path.addEllipse( QRectF( pt.x() - 1,
                                 pt.y() - 1,
                                 2,
                                 2 ) );
        path.addEllipse( QRectF( pt.x() - param.kick_radius_,
                                 pt.y() - param.kick_radius_,
                                 param.kick_radius_ * 2,
                                 param.kick_radius_ * 2 ) );

        last_point = pt;
    }

    painter.drawPath( path );

    // draw move line
    painter.setPen( dconf.debugTargetPen() );
    painter.drawLine( QLineF( first_point, last_point ) );

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

/*-------------------------------------------------------------------*/
/*!

 */
void
PlayerPainter::drawMovableArea( QPainter & painter,
                                const PlayerPainter::Param & param ) const
{
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

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

    const rcsc::SideID side = ( opt.reverseSide()
                                ? static_cast< rcsc::SideID >( -1 * param.player_.side() )
                                : param.player_.side() );
    if ( side == rcsc::LEFT )
    {
        painter.setPen( dconf.leftTeamPen() );
    }
    else
    {
        painter.setPen( dconf.rightTeamPen() );
    }
    painter.setBrush( dconf.transparentBrush() );

    //
    const rcsc::Vector2D first_pos( param.player_.x(), param.player_.y() );
    const rcsc::Vector2D first_vel( param.player_.deltaX(), param.player_.deltaY() );
    const double first_speed = first_vel.r();

    // std::cerr << param.player_.unum() << " pos=" << ppos << " vel=" << pvel << std::endl;
    const int angle_divs = 24;
    const double angle_step = 360.0 / angle_divs;
    QPointF points[angle_divs];

    // 1 dash (with omnidir dash )
    {
        const rcsc::Vector2D inertia_pos = first_pos + first_vel;
        int num = 0;
        for ( double dir = -180.0; dir < 180.0; dir += 45.0, ++num )
        {
            double front_max_accel = rcsc::ServerParam::i().maxDashPower()
                * param.player_type_.dashPowerRate()
                * param.player_type_.effortMax()
                * rcsc::ServerParam::i().dashDirRate( dir );
            double back_max_accel = rcsc::ServerParam::i().minDashPower()
                * param.player_type_.dashPowerRate()
                * param.player_type_.effortMax()
                * rcsc::ServerParam::i().dashDirRate( rcsc::AngleDeg::normalize_angle( dir + 180.0 ) );
            rcsc::Vector2D pos = inertia_pos
                + rcsc::Vector2D::from_polar( std::max( front_max_accel, std::fabs( back_max_accel ) ),
                                              param.body_ + dir );
            points[num].setX( opt.screenX( pos.x ) );
            points[num].setY( opt.screenY( pos.y ) );
        }

        painter.drawPolygon( points, num );
    }

    // 2 or more dashes (no omnidir dash)
    const int max_step = std::min( 30, opt.playerMovableCycle() );
    for ( int step = 2; step <= max_step; ++step )
    {
        const rcsc::Vector2D inertia_pos = param.player_type_.inertiaPoint( first_pos, first_vel, step );

        for ( int a = 0; a < angle_divs; ++a )
        {
            rcsc::AngleDeg dir = angle_step * a;
            rcsc::AngleDeg angle = param.body_ + dir;

            int n_turn = 0;
            {
                double speed = first_speed;
                double dir_diff = dir.abs();
                if ( dir_diff > 100.0 )
                {
                    dir_diff = 180.0 - dir_diff;
                }

                while ( dir_diff > 0.5 )
                {
                    dir_diff -= param.player_type_.effectiveTurn( rcsc::ServerParam::i().maxMoment(), speed );
                    speed *= param.player_type_.playerDecay();
                    ++n_turn;
                }
            }

            int n_dash = step - n_turn;
            if ( n_dash <= 0 )
            {
                points[a].setX( opt.screenX( inertia_pos.x ) );
                points[a].setY( opt.screenY( inertia_pos.y ) );
            }
            else
            {
                rcsc::Vector2D pos = inertia_pos
                    + rcsc::Vector2D::from_polar( param.player_type_.dashDistanceTable()[n_dash-1], angle );
                points[a].setX( opt.screenX( pos.x ) );
                points[a].setY( opt.screenY( pos.y ) );
            }
        }

        painter.drawPolygon( points, angle_divs );
        painter.drawText( points[0], QString::number( step ) );
        painter.drawText( points[angle_divs/4], QString::number( step ) );
        painter.drawText( points[angle_divs*2/4], QString::number( step ) );
        painter.drawText( points[angle_divs*3/4], QString::number( step ) );
    }

    //

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

/*-------------------------------------------------------------------*/
/*!

 */
void
PlayerPainter::drawViewArea( QPainter & painter,
                             const PlayerPainter::Param & param ) const
{
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    const double view_width = param.player_.viewWidth();

    const double visible_dist = rcsc::ServerParam::i().visibleDistance();
    const double visible_radius = opt.scale( visible_dist );

    const double view_start_angle = -param.head_ - view_width * 0.5;

    // draw enlarged view cone and feel area
    if ( opt.isSelectedAgent( param.player_.side(),
                              param.player_.unum() ) )
    {
        const double UNUM_FAR = opt.scale( 20.0 );
        const double TEAM_FAR = opt.scale( 40.0 );
        const double TEAM_TOOFAR = opt.scale( 60.0 );

        painter.setPen( dconf.linePen() );
        //painter.setBrush( dconf.transparentBrush() );

        int view_start_angle_int
            = static_cast< int >( rint( view_start_angle * 16 ) );
        int span_angle = static_cast< int >( rint( view_width * 16 ) );

        painter.drawArc( QRectF( param.x_ - UNUM_FAR, // left x
                                 param.y_ - UNUM_FAR, // toop y
                                 UNUM_FAR * 2, // width
                                 UNUM_FAR * 2 ), // height
                         view_start_angle_int,
                         span_angle );
        painter.drawArc( QRectF( param.x_ - TEAM_FAR, // left x
                                 param.y_ - TEAM_FAR, // toop y
                                 TEAM_FAR * 2, // width
                                 TEAM_FAR * 2 ), // height
                         view_start_angle_int,
                         span_angle );
        // pie, no an arc
        painter.drawArc( QRectF( param.x_ - TEAM_TOOFAR, // left x
                                 param.y_ - TEAM_TOOFAR, // toop y
                                 TEAM_TOOFAR * 2, // width
                                 TEAM_TOOFAR * 2 ), // height
                         view_start_angle_int,
                         span_angle );

        // draw feeling area circle
        painter.drawArc( QRectF( param.x_ - visible_radius,
                                 param.y_ - visible_radius,
                                 visible_radius * 2,
                                 visible_radius * 2 ),
                         0,
                         360 * 16 );

        const double view_start_angle_real = ( param.head_ - view_width * 0.5 ) * rcsc::AngleDeg::DEG2RAD;
        const double view_end_angle_real = ( param.head_ + view_width * 0.5 ) * rcsc::AngleDeg::DEG2RAD;
        // left side view cone end point x
        double lx = param.x_
            + opt.scale( 60.0 * std::cos( view_start_angle_real ) );
        // left side view cone end point y
        double ly = param.y_
            + opt.scale( 60.0  * std::sin( view_start_angle_real ) );
        // right side view cone end point x
        double rx = param.x_
            + opt.scale( 60.0 * std::cos( view_end_angle_real ) );
        // right side view cone end point y
        double ry = param.y_
            + opt.scale( 60.0 * std::sin( view_end_angle_real ) );
        painter.drawLine( QLineF( lx, ly, param.x_, param.y_ ) );
        painter.drawLine( QLineF( rx, ry, param.x_, param.y_ ) );
    }
    else
    {
        // draw normal view cone
        painter.setPen( dconf.viewConePen() );
        painter.setBrush( dconf.transparentBrush() );

        QPainterPath view_cone_path;
        view_cone_path.moveTo( param.x_, param.y_ );
        view_cone_path.arcTo( param.x_ - visible_radius,
                              param.y_ - visible_radius,
                              visible_radius * 2,
                              visible_radius * 2,
                              view_start_angle,
                              view_width );
        view_cone_path.closeSubpath();

        painter.drawPath( view_cone_path );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
PlayerPainter::drawViewDir( QPainter & painter,
                            const PlayerPainter::Param & param ) const
{
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    double r = ( opt.enlargeMode()
                 ? param.player_type_.kickableArea()
                 : param.player_type_.playerSize() );

    double end_x = opt.absScreenX( param.player_.x() * opt.reverseValue()
                                    + r * std::cos( param.head_ * rcsc::AngleDeg::DEG2RAD ) );
    double end_y = opt.absScreenY( param.player_.y() * opt.reverseValue()
                                   + r * std::sin( param.head_ * rcsc::AngleDeg::DEG2RAD ) );

    painter.setPen( Qt::black );
    painter.setBrush( dconf.transparentBrush() );
    painter.drawLine( QLineF( param.x_, param.y_, end_x, end_y ) );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
PlayerPainter::drawCatchableArea( QPainter & painter,
                                  const PlayerPainter::Param & param ) const
{
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();
    const rcsc::ServerParam & SP = rcsc::ServerParam::instance();

    rcsc::SideID side = param.player_.side();
    if ( opt.reverseSide() )
    {
        side = static_cast< rcsc::SideID >( -1 * side );
    }

    double catchable = opt.scale( SP.catchableArea() );
    painter.setPen( ( side == rcsc::LEFT )
                    ? dconf.leftGoaliePen()
                    : dconf.rightGoaliePen() );
    painter.setBrush( dconf.transparentBrush() );;

    painter.drawEllipse( QRectF( param.x_ - catchable,
                                 param.y_ - catchable,
                                 catchable * 2,
                                 catchable * 2 ) );

    double delta = SP.catchAreaLength() * std::max( 0.0, param.player_type_.catchAreaLengthStretch() - 1.0 );
    double max_l = SP.catchAreaLength() + delta;
    double min_l = SP.catchAreaLength() - delta;
    double max_area = std::sqrt( std::pow( SP.catchAreaWidth() * 0.5, 2.0 )
                                 + std::pow( max_l, 2.0 ) );
    double min_area = std::sqrt( std::pow( SP.catchAreaWidth() * 0.5, 2.0 )
                                 + std::pow( min_l, 2.0 ) );
    double max_r = opt.scale( max_area );
    if ( max_r > catchable )
    {
        painter.setPen( ( side == rcsc::LEFT )
                        ? dconf.leftGoalieStretchPen()
                        : dconf.rightGoalieStretchPen() );
        painter.drawEllipse( QRectF( param.x_ - max_r,
                                     param.y_ - max_r,
                                     max_r * 2,
                                     max_r * 2 ) );
        double min_r = opt.scale( min_area );
        painter.drawEllipse( QRectF( param.x_ - min_r,
                                     param.y_ - min_r,
                                     min_r * 2,
                                     min_r * 2 ) );
    }

    //
    // catch probability
    //
#if 0
    const double ball_dist = std::sqrt( std::pow( param.player_.x() - param.ball_.x(), 2 )
                                        + std::pow( param.player_.y() - param.ball_.y(), 2 ) );
    double catch_prob = SP.catchProbability();
    if ( ball_dist > max_area )
    {
        // catch_prob = 0.0;
        return;
    }

    if ( ball_dist > min_area )
    {
        double x = ball_dist * ( max_l / max_area ); // sin(theta)
        catch_prob
            = SP.catchProbability()
            - SP.catchProbability() * ( ( x - min_l ) / ( max_l - min_l ) );
//         catch_prob
//             = SP.catchProbability()
//             - SP.catchProbability() * ( ( ball_dist - min_area )
//                                         / ( max_area - min_area ) );
    }

    double text_radius = std::min( 40.0, param.draw_radius_ );

    painter.setPen( ( param.player_.side() == rcsc::LEFT )
                    ? dconf.rightGoaliePen()
                    : dconf.leftGoaliePen() );
    painter.setFont( dconf.playerFont() );
    painter.drawText( QPointF( param.x_ + text_radius,
                               param.y_ + 4 + painter.fontMetrics().ascent() * 2 ),
                      QString( "Catch=%1" ).arg( catch_prob, 0, 'g', 3 ) );
#endif
}

/*-------------------------------------------------------------------*/
/*!

 */
void
PlayerPainter::drawTackleArea( QPainter & painter,
                               const PlayerPainter::Param & param ) const
{
    const rcsc::ServerParam & SP = rcsc::ServerParam::i();
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    rcsc::Vector2D ppos( param.player_.x(),
                         param.player_.y() );
    rcsc::Vector2D bpos( param.ball_.x(),
                         param.ball_.y() );
    if ( opt.reverseSide() )
    {
        ppos *= -1.0;
        bpos *= -1.0;
    }

    rcsc::Vector2D player_to_ball = bpos - ppos;
    player_to_ball.rotate( - param.body_ );

    // draw tackle area & probability
    double tackle_dist = ( player_to_ball.x > 0.0
                           ? SP.tackleDist()
                           : SP.tackleBackDist() );
    if ( tackle_dist < 1.0e-5 )
    {
        return;
    }

    double tackle_prob = ( std::pow( player_to_ball.absX() / tackle_dist,
                                     SP.tackleExponent() )
                           + std::pow( player_to_ball.absY() / SP.tackleWidth(),
                                       SP.tackleExponent() ) );
    double foul_prob = ( std::pow( player_to_ball.absX() / tackle_dist,
                                   SP.foulExponent() )
                         + std::pow( player_to_ball.absY() / SP.tackleWidth(), // 1.25,
                                     SP.foulExponent() ) );
    if ( tackle_prob < 1.0
         || foul_prob < 1.0 )
    {
        rcsc::AngleDeg body_angle = param.body_;
        rcsc::AngleDeg body_angle_side = body_angle + 90.0;
        double body_x = body_angle.cos();
        double body_y = body_angle.sin();
        double forward_x = SP.tackleDist() * body_x;
        double forward_y = SP.tackleDist() * body_y;
        double back_x = SP.tackleBackDist() * -body_x;
        double back_y = SP.tackleBackDist() * -body_y;
        double right_x = SP.tackleWidth() * body_angle_side.cos();
        double right_y = SP.tackleWidth() * body_angle_side.sin();

        QPointF pts[5];
        pts[0].setX( opt.absScreenX( ppos.x + forward_x + right_x ) );
        pts[0].setY( opt.absScreenY( ppos.y + forward_y + right_y ) );
        pts[1].setX( opt.absScreenX( ppos.x + forward_x - right_x ) );
        pts[1].setY( opt.absScreenY( ppos.y + forward_y - right_y ) );
        pts[2].setX( opt.absScreenX( ppos.x + back_x - right_x ) );
        pts[2].setY( opt.absScreenY( ppos.y + back_y - right_y ) );
        pts[3].setX( opt.absScreenX( ppos.x + back_x + right_x ) );
        pts[3].setY( opt.absScreenY( ppos.y + back_y + right_y ) );
        pts[4] = pts[0];

        painter.setPen( dconf.tackleAreaPen() );
        painter.setBrush( dconf.transparentBrush() );

        //painter.drawConvexPolygon( pts, 4 );
        painter.drawPolyline( pts, 5 );

        double text_radius = std::min( 40.0, param.draw_radius_ );

        painter.setFont( dconf.playerFont() );
        painter.setPen( dconf.tacklePen() );

        if ( tackle_prob < 1.0
             && foul_prob < 1.0 )
        {
            painter.drawText( QPointF( param.x_ + text_radius,
                                       param.y_ + 4 + painter.fontMetrics().ascent() ),
                              QString( "T=%1,F=%2" )
                              .arg( 1.0 - tackle_prob, 0, 'g', 3 )
                              .arg( 1.0 - foul_prob, 0, 'g', 3 )
                              );
        }
        else if ( tackle_prob < 1.0 )
        {
            painter.drawText( QPointF( param.x_ + text_radius,
                                       param.y_ + 4 + painter.fontMetrics().ascent() ),
                              QString( "Tackle=%1" )
                              .arg( 1.0 - tackle_prob, 0, 'g', 3 ) );
        }
        else if ( foul_prob < 1.0 )
        {
            painter.drawText( QPointF( param.x_ + text_radius,
                                       param.y_ + 4 + painter.fontMetrics().ascent() ),
                              QString( "Foul=%1" )
                              .arg( 1.0 - foul_prob, 0, 'g', 3 ) );
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
PlayerPainter::drawKickAccelArea( QPainter & painter,
                                  const PlayerPainter::Param & param ) const
{
    if ( ! param.ball_.hasVelocity() )
    {
        return;
    }

    const rcsc::ServerParam & SP = rcsc::ServerParam::i();
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    rcsc::Vector2D ppos( param.player_.x(), param.player_.y() );
    rcsc::Vector2D bpos( param.ball_.x(), param.ball_.y() );
    rcsc::Vector2D player_to_ball = bpos - ppos;
    player_to_ball.rotate( - param.player_.body() );

    rcsc::Vector2D bnext( bpos.x + param.ball_.deltaX(),
                          bpos.y + param.ball_.deltaY() );
    double ball_dist = player_to_ball.r();

    if ( ball_dist < param.player_type_.kickableArea() )
    {
        double max_kick_accel
            = SP.maxPower()
            * SP.kickPowerRate()
            * ( 1.0 - 0.25 * player_to_ball.th().abs() / 180.0
                - 0.25
                * ( ball_dist - param.player_type_.playerSize() - SP.ballSize() )
                / param.player_type_.kickableMargin() );

        if ( max_kick_accel > SP.ballAccelMax() )
        {
            max_kick_accel = SP.ballAccelMax();
        }

        QPointF bpos_screen( opt.screenX( bpos.x ),
                             opt.screenY( bpos.y ) );
        QPointF bnext_screen( opt.screenX( bnext.x ),
                              opt.screenY( bnext.y ) );
        double max_speed_screen = opt.scale( SP.ballSpeedMax() );
        double max_kick_accel_screen = opt.scale( max_kick_accel );

        painter.setPen( dconf.kickAccelPen() );
        painter.setBrush( dconf.transparentBrush() );

        // draw no noise next ball move only by inertia
        painter.drawLine( QLineF( bpos_screen, bnext_screen ) );

        rcsc::Circle2D max_speed_circle( bpos, SP.ballSpeedMax() );
        rcsc::Circle2D max_accel_circle( bnext, max_kick_accel );
        rcsc::Vector2D intersection_1, intersection_2;

        if ( max_speed_circle.intersection( max_accel_circle,
                                            &intersection_1,
                                            &intersection_2 ) != 2 )

        {
            // no intersection points

            // just draw a next ball reachable area by max accel
            painter.drawEllipse( QRectF( bnext_screen.x() - max_kick_accel_screen,
                                         bnext_screen.y() - max_kick_accel_screen,
                                         max_kick_accel_screen * 2,
                                         max_kick_accel_screen * 2 ) );
        }
        else
        {
            // exists 2 intersection points

            rcsc::AngleDeg bpos_to_intersection_1 = ( intersection_1 - bpos ).th();
            rcsc::AngleDeg bpos_to_intersection_2 = ( intersection_2 - bpos ).th();

            rcsc::AngleDeg bpos_to_bnext_angle = ( bnext - bpos ).th();

            rcsc::AngleDeg * bpos_start_angle = 0;
            double bpos_angle_span = 0.0;
            if ( bpos_to_intersection_1.isLeftOf( bpos_to_bnext_angle ) )
            {
                bpos_start_angle = &bpos_to_intersection_1;
                bpos_angle_span = ( bpos_to_intersection_2 - bpos_to_intersection_1 ).degree();
                if ( bpos_angle_span < 0.0 )
                {
                    bpos_angle_span += 360.0;
                }
                bpos_angle_span *= -1.0;
            }
            else
            {
                bpos_start_angle = &bpos_to_intersection_2;
                bpos_angle_span = ( bpos_to_intersection_1 - bpos_to_intersection_2 ).degree();
                if ( bpos_angle_span < 0.0 )
                {
                    bpos_angle_span += 360.0;
                }
                bpos_angle_span *= -1.0;
            }

            if ( opt.reverseSide() )
            {
                *bpos_start_angle += 180.0;
            }

            int bpos_start_angle_int
                = static_cast< int >( rint( - bpos_start_angle->degree() * 16 ) );
            int bpos_angle_span_int
                = static_cast< int >( rint( bpos_angle_span * 16 ) );
            painter.drawArc( QRectF( bpos_screen.x() - max_speed_screen,
                                     bpos_screen.y() - max_speed_screen,
                                     max_speed_screen * 2,
                                     max_speed_screen * 2 ),
                             bpos_start_angle_int,
                             bpos_angle_span_int  );

            rcsc::AngleDeg bnext_to_intersection_1 = ( intersection_1 - bnext ).th();
            rcsc::AngleDeg bnext_to_intersection_2 = ( intersection_2 - bnext ).th();

            rcsc::AngleDeg bnext_to_bpos_angle = bpos_to_bnext_angle + 180.0;

            rcsc::AngleDeg * bnext_start_angle = 0;
            double bnext_angle_span = 0.0;
            if ( bnext_to_intersection_1.isLeftOf( bnext_to_bpos_angle ) )
            {
                bnext_start_angle = &bnext_to_intersection_1;
                bnext_angle_span = ( bnext_to_intersection_2 - bnext_to_intersection_1 ).degree();
                if ( bnext_angle_span < 0.0 )
                {
                    bnext_angle_span += 360.0;
                }
                bnext_angle_span *= -1.0;
            }
            else
            {
                bnext_start_angle = &bnext_to_intersection_2;
                bnext_angle_span = ( bnext_to_intersection_1 - bnext_to_intersection_2 ).degree();
                if ( bnext_angle_span < 0.0 )
                {
                    bnext_angle_span += 360.0;
                }
                bnext_angle_span *= -1.0;
            }

            if ( opt.reverseSide() )
            {
                *bnext_start_angle += 180.0;
            }

            int bnext_start_angle_int
                = static_cast< int >( rint( - bnext_start_angle->degree() * 16 ) );
            int bnext_angle_span_int
                = static_cast< int >( rint( bnext_angle_span * 16 ) );
            painter.drawArc( QRectF( bnext_screen.x() - max_kick_accel_screen,
                                     bnext_screen.y() - max_kick_accel_screen,
                                     max_kick_accel_screen * 2,
                                     max_kick_accel_screen * 2 ),
                             bnext_start_angle_int,
                             bnext_angle_span_int );
        }

        // draw kick info text
        painter.setFont( dconf.playerFont() );
        painter.setPen( dconf.kickAccelPen() );

        //snprintf( buf, 32, "MaxAccel=%.3f", max_kick_accel );
        painter.drawText( QPointF( bnext_screen.x() + 10,
                                   bnext_screen.y() + painter.fontMetrics().ascent() ),
                          QString( "MaxAccel=%1" ).arg( max_kick_accel, 0, 'g', 4 ) );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
PlayerPainter::drawPointto( QPainter & painter,
                            const PlayerPainter::Param & param ) const
{
    if ( ! param.player_.isPointing() )
    {
        return;
    }

    double px = Options::instance().screenX( param.player_.pointX() );
    double py = Options::instance().screenY( param.player_.pointY() );

    painter.setPen( DrawConfig::instance().pointtoPen() );
    painter.setBrush( DrawConfig::instance().transparentBrush() );

    painter.drawLine( QLineF( param.x_, param.y_, px, py ) );
    painter.drawLine( QLineF( px - 2, py - 2, px + 2, py + 2 ) );
    painter.drawLine( QLineF( px - 2, py + 2, px + 2, py - 2 ) );

}

/*-------------------------------------------------------------------*/
/*!

 */
void
PlayerPainter::drawAttentionto( QPainter & painter,
                                const PlayerPainter::Param & param ) const
{
    if ( ! param.player_.isFocusing() )
    {
        return;
    }

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    if ( ! view )
    {
        return;
    }

    const Options & opt = Options::instance();

    const rcsc::SideID self_side = ( Options::instance().reverseSide()
                                     ? static_cast< rcsc::SideID >( - param.player_.side() )
                                     : param.player_.side() );

    const rcsc::SideID side = param.player_.focusSide();
    const int unum = param.player_.focusUnum();

    for ( std::vector< rcsc::rcg::PlayerT >::const_iterator p = view->players().begin(),
              end = view->players().end();
          p != end;
          ++p )
    {
        if ( p->side() == side
             && p->unum() == unum )
        {
            if ( self_side == rcsc::LEFT )
            {
                painter.setPen( DrawConfig::instance().leftGoaliePen() );
            }
            else
            {
                painter.setPen( DrawConfig::instance().rightGoaliePen() );
            }
            //painter.setPen( DrawConfig::instance().attentiontoPen() );
            painter.setBrush( DrawConfig::instance().transparentBrush() );

            double px = opt.screenX( p->x() );
            double py = opt.screenY( p->y() );
            //int r = param.draw_radius_ + 3;
            double r = opt.scale( 2.0 );
            double lx = px;
            double ly = py;

            rcsc::Vector2D rel( param.player_.x(),
                                param.player_.y() );
            rel.x -= p->x();
            rel.y -= p->y();

            double len = rel.r();
            if ( len > std::pow( 2.0, 2 ) )
            {
                rel *= 2.0 / len;
                lx = opt.screenX( p->x() + rel.x );
                ly = opt.screenY( p->y() + rel.y );
            }

            painter.drawLine( QLineF( param.x_, param.y_, lx, ly ) );
            painter.drawEllipse( QRectF( px - r, py - r, r * 2, r * 2 ) );
            break;
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
PlayerPainter::drawText( QPainter & painter,
                         const PlayerPainter::Param & param ) const
{
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    painter.setFont( dconf.playerFont() );

    const double text_radius = std::min( 40.0, param.draw_radius_ );
    int card_offset = 0;

    if ( opt.showCard()
         && ( param.player_.hasRedCard()
              || param.player_.hasYellowCard() ) )
    {
        QFontMetrics fm = painter.fontMetrics();
        int x_size = std::min( std::max( 4, fm.ascent() - 2 ), 20 );
        int y_size = std::min( std::max( 4, fm.ascent() ), 24 );

        card_offset = x_size + 2;

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
        painter.setPen( Qt::black );
        painter.setBrush( param.player_.hasRedCard()
                          ? Qt::red
                          : Qt::yellow );
        painter.drawRect( QRectF( param.x_ + text_radius,
                                  param.y_ + 4 - y_size,
                                  x_size, y_size ) );
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

    const QPen & number_pen = ( opt.showIllegalDefenseState()
                                && param.player_.isIllegalDefenseState()
                                ?  dconf.illegalDefensePen()
                                : dconf.playerNumberFontPen() );

    if ( opt.showPlayerNumber()
         && opt.showPlayerType() )
    {
        painter.setPen( number_pen );
        painter.drawText( QPointF( param.x_ + text_radius + card_offset,
                                   param.y_ + 4 ),
                          QString( "%1,t%2" ).arg( param.player_.unum() ).arg( param.player_.type() ) );
    }
    else if ( opt.showPlayerNumber() )
    {
        painter.setPen( number_pen );
        painter.drawText( QPointF( param.x_ + text_radius + card_offset,
                                   param.y_ + 4 ),
                          QString::number( param.player_.unum() ) );
    }
    else if ( opt.showPlayerType() )
    {
        painter.setPen( number_pen );
        painter.drawText( QPointF( param.x_ + text_radius + card_offset,
                                   param.y_ + 4 ),
                          QString( "t%1" ).arg( param.player_.type() ) );
    }

    if ( param.player_.hasStamina() )
    {
        QString str;
        if ( opt.showStamina() )
        {
            str += QString::number( static_cast< int >( rint( param.player_.stamina() ) ) );
        }

        if ( opt.showStaminaCapacity()
             && param.player_.hasStaminaCapacity() )
        {
            if ( ! str.isEmpty() ) str += '/';
            str += QString::number( static_cast< int >( rint( param.player_.staminaCapacity() ) ) );
        }

        if ( ! str.isEmpty() )
        {
            // this player is selected
            if ( opt.isSelectedAgent( param.player_.side(),
                                      param.player_.unum() ) )
            {
                str += QString( ",e%1,r%2" )
                    .arg( param.player_.effort(), 0, 'g', 3 )
                    .arg( param.player_.recovery(), 0, 'g', 3 );
                painter.setPen( dconf.playerNumberFontPen() );
            }
            else
            {
                painter.setPen( dconf.playerStaminaFontPen() );
            }

            painter.drawText( QPointF( param.x_ - text_radius,
                                       param.y_ - text_radius - 3 ),
                              str );
        }
    }
}
