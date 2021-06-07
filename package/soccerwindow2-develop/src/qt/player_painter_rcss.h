// -*-c++-*-

/*!
  \file player_painter_rcss.h
  \brief rcssmonitor style: player painter class Header File.
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

#ifndef SOCCERWINDOW2_QT4_PLAYER_PAINTER_RCSS_H
#define SOCCERWINDOW2_QT4_PLAYER_PAINTER_RCSS_H

#include <QBrush>
#include <QColor>
#include <QFont>
#include <QPen>

#include "painter_interface.h"

#include <rcsc/rcg/types.h>
#include <rcsc/types.h>

#include <boost/shared_ptr.hpp>

namespace rcsc {
class PlayerType;
}

class QPainter;

class MainData;

class PlayerPainterRCSS
    : public PainterInterface {
private:

    static const QPen PLAYER_PEN;
    static const QPen NECK_PEN;

    static const QPen LEFT_TEAM_PEN;
    static const QBrush LEFT_TEAM_BRUSH;
    static const QBrush LEFT_GOALIE_BRUSH;

    static const QPen RIGHT_TEAM_PEN;
    static const QBrush RIGHT_TEAM_BRUSH;
    static const QBrush RIGHT_GOALIE_BRUSH;

    static const QBrush BALL_COLLIDE_BRUSH;
    //static const QBrush PLAYER_COLLIDE_BRUSH;

    static const QBrush VIEW_AREA_TOOFAR_BRUSH;
    static const QBrush VIEW_AREA_BRUSH;

    static const QBrush KICK_FAULT_BRUSH;

    struct Param {
        double x_; //!< screen X coordinates
        double y_; //!< screen Y coordinates
        double body_; //!< body direction
        double head_; //!< head direction
        double body_radius_; //!< pixel of body radius
        double kick_radius_; //!< pixel of kick area radius
        double draw_radius_; //!< pixel of main draw radius.
        bool have_full_effort_; //!< flag to check effort value
        const rcsc::rcg::PlayerT & player_;
        const rcsc::rcg::BallT & ball_;
        const rcsc::PlayerType & player_type_;

        /*!
          \brief constructor
          \param p reference to parent class
          \param draw_param set of draw parameters
          \param sparam server parameters
          \param ptypes set of player type parameters
         */
        Param( const rcsc::rcg::PlayerT & player,
               const rcsc::rcg::BallT & ball,
               const rcsc::PlayerType & ptype );

    private:
        //! not used
        Param();
    };


    const MainData & M_main_data;

    // not used
    PlayerPainterRCSS();
    PlayerPainterRCSS( const PlayerPainterRCSS & );
    const PlayerPainterRCSS & operator=( const PlayerPainterRCSS & );
public:

    explicit
    PlayerPainterRCSS( const MainData & main_data );

    void draw( QPainter & dc );

private:

    void drawAll( QPainter & painter,
                  const rcsc::rcg::PlayerT & player,
                  const rcsc::rcg::BallT & ball ) const;

    /*!
      \brief draw body circle
      \param painter reference to painter instance
      \param param parameter set that defeines this players draw settings
    */
    void drawBody( QPainter & painter,
                   const PlayerPainterRCSS::Param & param ) const;

    /*!
      \brief draw shador arc
      \param painter reference to painter instance
      \param param parameter set that defeines this players draw settings
    */
    void drawShadow( QPainter & painter,
                     const PlayerPainterRCSS::Param & param ) const;

    /*!
      \brief draw player's body edge lines.
      \param painter reference to painter instance
      \param param parameter set that defeines this players draw settings
     */
    void drawEdge( QPainter & painter,
                   const PlayerPainterRCSS::Param & param ) const;

    /*!
      \brief draw player's future points
      \param painter reference to painter instance
      \param param parameter set that defeines this players draw settings
    */
    void drawFuture( QPainter & painter,
                     const PlayerPainterRCSS::Param & param ) const;

    /*!
      \brief draw view cone
      \param painter reference to painter instance
      \param param parameter set that defeines this players draw settings
    */
    void drawViewArea( QPainter & painter,
                       const PlayerPainterRCSS::Param & param ) const;

    /*!
      \brief draw whole view area
      \param painter reference to painter instance
      \param param parameter set that defeines this players draw settings
    */
    void drawViewAreaBackground( QPainter & painter,
                                 const rcsc::rcg::PlayerT & player ) const;

    void drawCatchableArea( QPainter & painter,
                            const PlayerPainterRCSS::Param & param ) const;
    void drawTackleArea( QPainter & painter,
                         const PlayerPainterRCSS::Param & param ) const;

    /*!
      \brief draw text info
      \param painter reference to painter instance
      \param param parameter set that defeines this players draw settings
    */
    void drawText( QPainter & painter,
                   const PlayerPainterRCSS::Param & param ) const;

};

#endif
