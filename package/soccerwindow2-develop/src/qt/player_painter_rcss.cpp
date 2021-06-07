// -*-c++-*-

/*!
  \file player_painter_rcss.cpp
  \brief rcssmonitor style: player painter class Source File.
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

#include "player_painter_rcss.h"

#include "player_painter.h"
#include "draw_config.h"

// model
#include "options.h"
#include "main_data.h"

#include <rcsc/common/player_type.h>
#include <rcsc/common/server_param.h>
#include <rcsc/geom/angle_deg.h>

#include <cstring>
#include <cstdio>
#include <cmath>


const QPen PlayerPainterRCSS::PLAYER_PEN( QColor( 0, 0, 0 ),
                                          0, Qt::SolidLine );
const QPen PlayerPainterRCSS::NECK_PEN( QColor( "#ff0000" ),
                                        0, Qt::SolidLine );

const QPen PlayerPainterRCSS::LEFT_TEAM_PEN( QColor( "#ff0000" ),
                                             0, Qt::SolidLine );
const QBrush PlayerPainterRCSS::LEFT_TEAM_BRUSH( QColor( "#ffff00" ),
                                                 Qt::SolidPattern );
const QBrush PlayerPainterRCSS::LEFT_GOALIE_BRUSH( QColor( "#00ff00" ),
                                                   Qt::SolidPattern );

const QPen PlayerPainterRCSS::RIGHT_TEAM_PEN( QColor( "#00008b" ),
                                              0, Qt::SolidLine );
const QBrush PlayerPainterRCSS::RIGHT_TEAM_BRUSH( QColor( "#00ffff" ),
                                                  Qt::SolidPattern );
const QBrush PlayerPainterRCSS::RIGHT_GOALIE_BRUSH( QColor( "#ff99ff" ),
                                                    Qt::SolidPattern );

const QBrush PlayerPainterRCSS::BALL_COLLIDE_BRUSH( QColor( "#ff0000" ),
                                                    Qt::SolidPattern );

const QBrush PlayerPainterRCSS::VIEW_AREA_TOOFAR_BRUSH( QColor( 127, 255, 127, 23 ),
                                                        //QColor( 0, 0, 0, 23 ),
                                                        //QColor( 0, 180, 0, 63 ),
                                                        //QColor( "#00b400" ),
                                                        Qt::SolidPattern );
const QBrush PlayerPainterRCSS::VIEW_AREA_BRUSH( QColor( 127, 255, 127, 23 ),
                                                 //QColor( 0, 0, 0, 15 ),
                                                 //QColor( 0, 170, 0, 63 ),
                                                 //QColor( "#00aa00" ),
                                                 Qt::SolidPattern );

const QBrush PlayerPainterRCSS::KICK_FAULT_BRUSH( QColor( "#0000ff" ),
                                                  Qt::SolidPattern );

/*-------------------------------------------------------------------*/
/*

*/
inline
PlayerPainterRCSS::Param::Param( const rcsc::rcg::PlayerT & player,
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

    draw_radius_ =  kick_radius_;
}

/*-------------------------------------------------------------------*/
/*

*/
PlayerPainterRCSS::PlayerPainterRCSS( const MainData & main_data )
    : M_main_data( main_data )
{

}

/*-------------------------------------------------------------------*/
/*

*/
void
PlayerPainterRCSS::draw( QPainter & painter )
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

    if ( opt.showViewArea()
         && opt.selectedNumber() != 0 )
    {
        for ( std::vector< rcsc::rcg::PlayerT >::const_iterator it = view->players().begin(),
                  end = view->players().end();
              it != end;
              ++it )
        {
            if ( ! it->hasView() )
            {
                break;
            }

            if ( opt.isSelectedAgent( it->side(), it->unum() ) )
            {
                drawViewAreaBackground( painter, *it );
                break;
            }
        }
    }

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
PlayerPainterRCSS::drawAll( QPainter & painter,
                            const rcsc::rcg::PlayerT & player,
                            const rcsc::rcg::BallT & ball ) const
{
    const Options & opt = Options::instance();

    const Param param( player,
                       ball,
                       M_main_data.viewHolder().playerType( player.type() ) );

    drawBody( painter, param );
    //drawShadow( painter, param );
    drawEdge( painter, param );

    if ( opt.isSelectedAgent( player.side(), player.unum() )
         && opt.playerFutureCycle() > 0
         && player.hasVelocity() )
    {
        drawFuture( painter, param );
    }

    if ( player.hasView()
         && opt.showViewArea() )
    {
        drawViewArea( painter, param );
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

    drawText( painter, param );
}

/*-------------------------------------------------------------------*/
/*

*/
void
PlayerPainterRCSS::drawBody( QPainter & painter,
                             const PlayerPainterRCSS::Param & param ) const
{
    const DrawConfig & dconf = DrawConfig::instance();

    // decide base color
    painter.setPen( PLAYER_PEN );

    rcsc::SideID side = param.player_.side();
    if ( Options::instance().reverseSide() )
    {
        side = static_cast< rcsc::SideID >( -1 * side );
    }

    switch ( side ) {
    case rcsc::LEFT:
        if ( param.player_.isGoalie() )
        {
            painter.setBrush( LEFT_GOALIE_BRUSH );
        }
        else
        {
            painter.setBrush( LEFT_TEAM_BRUSH );
        }
        break;
    case rcsc::RIGHT:
        if ( param.player_.isGoalie() )
        {
            painter.setBrush( RIGHT_GOALIE_BRUSH );
        }
        else
        {
            painter.setBrush( RIGHT_TEAM_BRUSH );
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
    if ( param.player_.isKicking() )
    {
        painter.setPen( dconf.kickPen() );
    }
    if ( param.player_.isKickingFault() )
    {
        painter.setPen( dconf.kickFaultPen() );
        painter.setBrush( KICK_FAULT_BRUSH );
    }
    if ( param.player_.isCatching() )
    {
        painter.setBrush( dconf.catchBrush() );
    }
    if ( param.player_.isCatchingFault() )
    {
        painter.setBrush( dconf.catchFaultBrush() );
    }
    if ( param.player_.isTackling() )
    {
        painter.setPen( dconf.tacklePen() );
        painter.setBrush( dconf.tackleBrush() );
    }
    if ( param.player_.isTacklingFault() )
    {
        painter.setPen( dconf.tacklePen() );
        painter.setBrush( dconf.tackleFaultBrush() );
    }
    if ( param.player_.isFoulCharged() )
    {
        painter.setBrush( dconf.foulChargedBrush() );
    }
    if ( param.player_.isCollidedBall() )
    {
        painter.setBrush( BALL_COLLIDE_BRUSH );
    }
//     if ( param.player_.isCollidedPlayer() )
//     {
//         painter.setBrush( dconf.collidePlayerBrush() );
//     }

    painter.drawEllipse( QRectF( param.x_ - param.kick_radius_ ,
                                 param.y_ - param.kick_radius_ ,
                                 param.kick_radius_ * 2 ,
                                 param.kick_radius_ * 2 ) );

}

/*-------------------------------------------------------------------*/
/*

*/
void
PlayerPainterRCSS::drawShadow( QPainter &,
                               const PlayerPainterRCSS::Param & ) const
{

}

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerPainterRCSS::drawEdge( QPainter & painter,
                             const PlayerPainterRCSS::Param & param ) const
{
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    // draw body edge
    {

        // draw real body edge
        painter.setPen( PLAYER_PEN );
        painter.setBrush( dconf.transparentBrush() );
        painter.drawEllipse( QRectF( param.x_ - param.body_radius_,
                                     param.y_ - param.body_radius_,
                                     param.body_radius_ * 2 ,
                                     param.body_radius_ * 2  ) );
    }

    // draw stamina status if effort or recovery is decayed.
    if ( param.player_.hasStamina() )
    {
        if ( param.player_.stamina() < 1500.0 )
        {
            painter.setBrush( dconf.shadowBrush() );
            painter.drawEllipse( QRectF( param.x_ - param.body_radius_,
                                         param.y_ - param.body_radius_,
                                         param.body_radius_ * 2 ,
                                         param.body_radius_ * 2  ) );
        }
    }

    // body direction line
    {
        double end_x = opt.absScreenX( param.player_.x() * opt.reverseValue()
                                        + param.player_type_.kickableArea()
                                        * std::cos( param.body_ * rcsc::AngleDeg::DEG2RAD ) );
        double end_y = opt.absScreenY( param.player_.y() * opt.reverseValue()
                                        + param.player_type_.kickableArea()
                                        * std::sin( param.body_ * rcsc::AngleDeg::DEG2RAD ) );

        painter.setPen( PLAYER_PEN );
        painter.setBrush( dconf.transparentBrush() );
        painter.drawLine( QLineF( param.x_, param.y_, end_x, end_y ) );

    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerPainterRCSS::drawFuture( QPainter & painter,
                               const PlayerPainterRCSS::Param & param ) const
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

        painter.drawEllipse( QRectF( pt.x() - 1,
                                     pt.y() - 1,
                                     3,
                                     3 ) );
        painter.drawEllipse( QRectF( pt.x() - param.kick_radius_,
                                     pt.y() - param.kick_radius_,
                                     param.kick_radius_ * 2,
                                     param.kick_radius_ * 2 ) );

        last_point = pt;
    }

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
PlayerPainterRCSS::drawViewAreaBackground( QPainter & painter,
                                           const rcsc::rcg::PlayerT & player ) const
{
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    const double x = opt.screenX( player.x() );
    const double y = opt.screenY( player.y() );
    const double visible_radius = opt.scale( rcsc::ServerParam::i().visibleDistance() );
    const int view_start_angle
        = qRound( ( - player.head() - player.viewWidth()*0.5
                    + ( opt.reverseSide() ? 180.0 : 0.0 ) ) * 16 );
    const int view_width = qRound( player.viewWidth() * 16 );

    const double TEAM_FAR = opt.scale( 40.0 );
    const double TEAM_TOOFAR = opt.scale( 60.0 );

    //painter.setRasterOp( Qt::OrROP );
    //QPainter::CompositionMode mode = painter.compositionMode();
    //painter.setCompositionMode( QPainter::CompositionMode_SourceOver );

    painter.setPen( dconf.transparentPen() );
    painter.setBrush( VIEW_AREA_TOOFAR_BRUSH );
    //painter.setOpacity( 0.2 );

    painter.drawPie( QRectF( x - TEAM_TOOFAR,
                             y - TEAM_TOOFAR,
                             TEAM_TOOFAR * 2,
                             TEAM_TOOFAR * 2 ),
                     view_start_angle,
                     view_width );

    painter.drawPie( QRectF( x - visible_radius,
                             y - visible_radius,
                             visible_radius * 2,
                             visible_radius * 2 ),
                     view_start_angle,
                     view_width - 360*16 );

    painter.setBrush( VIEW_AREA_BRUSH );
    //painter.setOpacity( 0.1 );

    painter.drawPie( QRectF( x - TEAM_FAR,
                             y - TEAM_FAR,
                             TEAM_FAR * 2,
                             TEAM_FAR * 2 ),
                     view_start_angle,
                     view_width );

    //painter.setRasterOp( Qt::CopyROP );
    //painter.setCompositionMode( mode );
    //painter.setOpacity( 1.0 );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerPainterRCSS::drawViewArea( QPainter & painter,
                                 const PlayerPainterRCSS::Param & param ) const
{
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();

    // draw face direction
    painter.setPen( NECK_PEN );
    painter.setBrush( dconf.transparentBrush() );

    double end_x = opt.absScreenX( param.player_.x() * opt.reverseValue()
                                    + param.player_type_.kickableArea()
                                    * std::cos( param.head_ * rcsc::AngleDeg::DEG2RAD ) );
    double end_y = opt.absScreenY( param.player_.y() * opt.reverseValue()
                                    + param.player_type_.kickableArea()
                                    * std::sin( param.head_ * rcsc::AngleDeg::DEG2RAD ) );;

    painter.drawLine( QLineF( param.x_, param.y_, end_x, end_y ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerPainterRCSS::drawCatchableArea( QPainter & painter,
                                      const PlayerPainterRCSS::Param & param ) const
{
    const Options & opt = Options::instance();
    const DrawConfig & dconf = DrawConfig::instance();
    const rcsc::ServerParam & SP = rcsc::ServerParam::i();

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
    if ( ball_dist > stretch_area )
    {
        // catch_prob = 0.0;
        return;
    }

    if ( ball_dist > SP.catchableArea() )
    {
        double x = ball_dist * ( stretch_l / stretch_area );
        catch_prob
            = SP.catchProbability()
            - SP.catchProbability() * ( ( x - SP.catchAreaLength() )
                                        / ( stretch_l - SP.catchAreaLength() ) );
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
PlayerPainterRCSS::drawTackleArea( QPainter & painter,
                                   const PlayerPainterRCSS::Param & param ) const
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
    double tackle_prob = ( std::pow( player_to_ball.absX() / tackle_dist,
                                     SP.tackleExponent() )
                           + std::pow( player_to_ball.absY() / SP.tackleWidth(),
                                       SP.tackleExponent() ) );
    if ( tackle_prob < 1.0 )
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

        double text_radius = param.draw_radius_;
        if ( text_radius > 40 )
        {
            text_radius = 40;
        }

        painter.setFont( dconf.playerFont() );
        painter.setPen( dconf.tacklePen() );

        //snprintf( msg, 32, "TakleProb=%.3f", 1.0 - tackle_prob );
        painter.drawText( param.x_ + text_radius,
                          param.y_ + 4 + painter.fontMetrics().ascent(),
                          QString( "TackleProb=%1" ).arg( 1.0 - tackle_prob, 0, 'g', 3 ) );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
PlayerPainterRCSS::drawText( QPainter & painter,
                             const PlayerPainterRCSS::Param & param ) const
{
    static bool first = true;
    static QFont font( "6x13bold", 9, QFont::Bold );
    if ( first )
    {
        font.setPointSize( 9 );
        font.setBold( true );
        //font.setStyleHint( QFont::System, QFont::PreferBitmap );
        font.setBold( true );
        font.setFixedPitch( true );
        first = false;
    }
    painter.setFont( font );

    const Options & opt = Options::instance();


    QString str;

    if ( opt.showPlayerNumber() )
    {
        str += QString::number( param.player_.unum(), 16 ).toUpper();
    }

    if ( param.player_.hasStamina() )
    {
        if ( opt.showStamina() )
        {
            if ( ! str.isEmpty() ) str += ' ';
            str += QString::number( static_cast< int >( rint ( param.player_.stamina() ) ) );
        }

        if ( opt.showStaminaCapacity()
             && param.player_.hasStaminaCapacity() )
        {
            if ( ! str.isEmpty() ) str += '/';
            str += QString::number( static_cast< int >( rint( param.player_.staminaCapacity() ) ) );
        }
    }

    if ( opt.showPlayerType() )
    {
        if ( ! str.isEmpty() )
        {
            str += ", t";
        }
        else
        {
            str += 't';
        }
        str += QString::number( param.player_.type() );
    }

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
                                  param.y_ - y_size,
                                  x_size,
                                  y_size ) );
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

    if ( ! str.isEmpty() )
    {
        rcsc::SideID side = param.player_.side();
        if ( opt.reverseSide() )
        {
            side = static_cast< rcsc::SideID >( -1 * side );
        }

        if ( param.player_.isIllegalDefenseState()
             && opt.showIllegalDefenseState() )
        {
            painter.setPen( DrawConfig::instance().illegalDefensePen() );
        }
        else
        {
            painter.setPen( side == rcsc::LEFT
                            ? LEFT_TEAM_PEN
                            : RIGHT_TEAM_PEN );
        }

        painter.drawText( QPointF( param.x_ + text_radius + card_offset,
                                   param.y_ ),
                          str );
    }
}
