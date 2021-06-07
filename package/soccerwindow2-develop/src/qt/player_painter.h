// -*-c++-*-

/*!
  \file player_painter.h
  \brief player painter class Header File.
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

#ifndef SOCCERWINDOW2_QT_PLAYER_PAINTER_H
#define SOCCERWINDOW2_QT_PLAYER_PAINTER_H

#include "painter_interface.h"

#include <rcsc/rcg/types.h>
#include <rcsc/types.h>

#include <boost/shared_ptr.hpp>

namespace rcsc {
class PlayerType;
}

class QPainter;

class MainData;

//#define USE_BODY_CACHE
//#define USE_CHAR_CACHE

class PlayerPainter
    : public PainterInterface {
private:

    struct Param {
        double x_; //!< screen X coordinates
        double y_; //!< screen Y coordinates
        double body_;
        double head_;
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
    PlayerPainter();
    PlayerPainter( const PlayerPainter & );
    const PlayerPainter operator=( const PlayerPainter & );
public:

    explicit
    PlayerPainter( const MainData & main_data );

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
                   const PlayerPainter::Param & param ) const;

    /*!
      \brief draw shador arc
      \param painter reference to painter instance
      \param param parameter set that defeines this players draw settings
    */
    void drawShadow( QPainter & painter,
                     const PlayerPainter::Param & param ) const;

    /*!
      \brief draw player's body edge lines.
      \param painter reference to painter instance
      \param param parameter set that defeines this players draw settings
     */
    void drawEdge( QPainter & painter,
                   const PlayerPainter::Param & param ) const;

    /*!
      \brief draw player's future points
      \param painter reference to painter instance
      \param param parameter set that defeines this players draw settings
    */
    void drawFuture( QPainter & painter,
                     const PlayerPainter::Param & param ) const;

    void drawMovableArea( QPainter & painter,
                          const PlayerPainter::Param & param ) const;

    /*!
      \brief draw view area
      \param paint reference to painter instance
      \param param parameter set that defeines this players draw settings
    */
    void drawViewArea( QPainter & painter,
                       const PlayerPainter::Param & param ) const;
    void drawViewDir( QPainter & painter,
                      const PlayerPainter::Param & param ) const;

    void drawCatchableArea( QPainter & painter,
                            const PlayerPainter::Param & param ) const;
    void drawTackleArea( QPainter & painter,
                         const PlayerPainter::Param & param ) const;
    void drawKickAccelArea( QPainter & painter,
                            const PlayerPainter::Param & param ) const;
    void drawPointto( QPainter & painter,
                      const PlayerPainter::Param & param ) const;
    void drawAttentionto( QPainter & painter,
                          const PlayerPainter::Param & param ) const;

    /*!
      \brief draw text info
      \param painter reference to painter instance
      \param param parameter set that defeines this players draw settings
    */
    void drawText( QPainter & painter,
                   const PlayerPainter::Param & param ) const;

};

#endif
