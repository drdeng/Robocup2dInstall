// -*-c++-*-

/*!
  \file debug_view_data.h
  \brief debug view data to be drawn Header File.
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

#ifndef SOCCERWINDOW2_DEBUG_VIEW_DATA_H
#define SOCCERWINDOW2_DEBUG_VIEW_DATA_H

#include <rcsc/geom/vector_2d.h>
#include <rcsc/rcg/types.h>
#include <rcsc/game_time.h>

#include <boost/shared_ptr.hpp>

#include <list>
#include <vector>
#include <map>
#include <ostream>
#include <cmath>

//! one cycle debug data snapshot
class DebugViewData {
public:

    typedef boost::shared_ptr< DebugViewData > Ptr;
    typedef boost::shared_ptr< const DebugViewData > ConstPtr;
    typedef std::vector< ConstPtr > Cont;
    typedef std::map< rcsc::GameTime, Cont, rcsc::GameTime::Less > Map;


    enum PlayerRecog {
        TEAMMATE,
        OPPONENT,
        UNKNOWN_TEAMMATE,
        UNKNOWN_OPPONENT,
        UNKNOWN_PLAYER,
        MAX_PLAYER_RECOG,
    };

    struct BallT {
        rcsc::rcg::Int32 x_;
        rcsc::rcg::Int32 y_;
        rcsc::rcg::Int32 vx_;
        rcsc::rcg::Int32 vy_;
        std::string comment_;

        BallT( const float & x,
               const float & y )
            : x_( static_cast< rcsc::rcg::Int32 >( ::rint( x * rcsc::rcg::SHOWINFO_SCALE2 ) ) ),
              y_( static_cast< rcsc::rcg::Int32 >( ::rint( y * rcsc::rcg::SHOWINFO_SCALE2 ) ) ),
              vx_( 0 ),
              vy_( 0 )
          { }
        void setVel( const float & vx,
                     const float & vy )
          {
              vx_ = static_cast< rcsc::rcg::Int32 >( rint( vx * rcsc::rcg::SHOWINFO_SCALE2 ) );
              vy_ = static_cast< rcsc::rcg::Int32 >( rint( vy * rcsc::rcg::SHOWINFO_SCALE2 ) );
          }

        double x() const { return static_cast< double >( x_ ) / rcsc::rcg::SHOWINFO_SCALE2; }
        double y() const { return static_cast< double >( y_ ) / rcsc::rcg::SHOWINFO_SCALE2; }
        double vx() const { return static_cast< double >( vx_ ) / rcsc::rcg::SHOWINFO_SCALE2; }
        double vy() const { return static_cast< double >( vy_ ) / rcsc::rcg::SHOWINFO_SCALE2; }
    };

    struct SelfT {
        char side_;
        int unum_;
        int ptype_;
        rcsc::rcg::Int32 x_;
        rcsc::rcg::Int32 y_;
        rcsc::rcg::Int32 vx_;
        rcsc::rcg::Int32 vy_;
        rcsc::rcg::Int16 body_;
        rcsc::rcg::Int16 neck_;
        std::string comment_;

        /*!
          \brief created by coach
         */
        SelfT( char side )
            : side_( side ),
              unum_( 12 ),
              ptype_( -1 ),
              x_( 0 ),
              y_( 0 ),
              vx_( 0 ),
              vy_( 0 ),
              body_( 0 ),
              neck_( 0 ),
              comment_()
          { }

        /*!
          \brief created by player
         */
        SelfT( char side,
               int unum,
               int ptype,
               const float x,
               const float y,
               const float vx,
               const float vy,
               const float b,
               const float n,
               const char * comment )
            : side_( side ),
              unum_( unum ),
              ptype_( ptype ),
              x_( static_cast< rcsc::rcg::Int32 >( rint( x * rcsc::rcg::SHOWINFO_SCALE2 ) ) ),
              y_( static_cast< rcsc::rcg::Int32 >( rint( y * rcsc::rcg::SHOWINFO_SCALE2 ) ) ),
              vx_( static_cast< rcsc::rcg::Int32 >( rint( vx * rcsc::rcg::SHOWINFO_SCALE2 ) ) ),
              vy_( static_cast< rcsc::rcg::Int32 >( rint( vy * rcsc::rcg::SHOWINFO_SCALE2 ) ) ),
              body_( static_cast< rcsc::rcg::Int16 >( b ) ),
              neck_( static_cast< rcsc::rcg::Int16 >( n ) ),
              comment_( comment )
          { }

        double x() const { return static_cast< double >( x_ ) / rcsc::rcg::SHOWINFO_SCALE2; }
        double y() const { return static_cast< double >( y_ ) / rcsc::rcg::SHOWINFO_SCALE2; }
        double vx() const { return static_cast< double >( vx_ ) / rcsc::rcg::SHOWINFO_SCALE2; }
        double vy() const { return static_cast< double >( vy_ ) / rcsc::rcg::SHOWINFO_SCALE2; }
        double body() const { return static_cast< double >( body_ ); }
        double neck() const { return static_cast< double >( neck_ ); }
    };

    struct PlayerT {
        rcsc::rcg::Int16 unum_;
        rcsc::rcg::Int16 ptype_;
        rcsc::rcg::Int32 x_;
        rcsc::rcg::Int32 y_;
        rcsc::rcg::Int16 body_;
        rcsc::rcg::Int16 pointto_;
        std::string comment_;

        PlayerT( const rcsc::rcg::Int16 unum,
                 const rcsc::rcg::Int16 ptype,
                 const float x,
                 const float y,
                 const float body,
                 const float pointto,
                 const char * comment )
            : unum_( unum ),
              ptype_( ptype ),
              x_( static_cast< rcsc::rcg::Int32 >( rint( x * rcsc::rcg::SHOWINFO_SCALE2 ) ) ),
              y_( static_cast< rcsc::rcg::Int32 >( rint( y * rcsc::rcg::SHOWINFO_SCALE2 ) ) ),
              body_( static_cast< rcsc::rcg::Int16 >( body ) ),
              pointto_( static_cast< rcsc::rcg::Int16 >( pointto ) ),
              comment_( comment )
          { }

        void setBody( const float body )
          {
              body_ = static_cast< rcsc::rcg::Int16 >( body );
          }

        double x() const { return static_cast< double >( x_ ) / rcsc::rcg::SHOWINFO_SCALE2; }
        double y() const { return static_cast< double >( y_ ) / rcsc::rcg::SHOWINFO_SCALE2; }
        double body() const { return static_cast< double >( body_ ); }
        double pointto() const { return static_cast< double >( pointto_ ); }

        bool hasBody() const { return body_ != -360; }
        bool hasPointto() const { return pointto_ != -360; }
    };

    struct LineT {
        float x1_;
        float y1_;
        float x2_;
        float y2_;
        std::string color_;

        LineT( const float & x1,
               const float & y1,
               const float & x2,
               const float & y2,
               const char * c = "" )
            : x1_( x1 ),
              y1_( y1 ),
              x2_( x2 ),
              y2_( y2 ),
              color_( c )
          { }
    };

    struct TriangleT {
        float x1_;
        float y1_;
        float x2_;
        float y2_;
        float x3_;
        float y3_;
        std::string color_;

        TriangleT( const float & x1,
                   const float & y1,
                   const float & x2,
                   const float & y2,
                   const float & x3,
                   const float & y3,
                   const char * c = "" )
            : x1_( x1 ),
              y1_( y1 ),
              x2_( x2 ),
              y2_( y2 ),
              x3_( x3 ),
              y3_( y3 ),
              color_( c )
          { }
    };

    struct RectT {
        float left_x_;
        float top_y_;
        float right_x_;
        float bottom_y_;
        std::string color_;

        RectT( const float & left_x,
               const float & top_y,
               const float & right_x,
               const float & bottom_y,
               const char * c = "" )
            : left_x_( left_x ),
              top_y_( top_y ),
              right_x_( right_x ),
              bottom_y_( bottom_y ),
              color_( c )
          { }
    };

    struct CircleT {
        float center_x_;
        float center_y_;
        float radius_;
        std::string color_;

        CircleT( const float & center_x,
                 const float & center_y,
                 const float & radius,
                 const char * c = "" )
            : center_x_( center_x ),
              center_y_( center_y ),
              radius_( radius ),
              color_( c )
          { }
    };


    typedef std::vector< boost::shared_ptr< PlayerT > > PlayerCont;

private:

    boost::shared_ptr< BallT > M_ball;
    boost::shared_ptr< SelfT > M_self;

    PlayerCont M_teammates;
    PlayerCont M_opponents;
    PlayerCont M_unknown_teammates;
    PlayerCont M_unknown_opponents;
    PlayerCont M_unknown_players;

    std::list< LineT > M_lines;
    std::list< TriangleT > M_triangles;
    std::list< RectT > M_rectangles;
    std::list< CircleT > M_circles;

    int M_target_teammate;
    boost::shared_ptr< rcsc::Vector2D > M_target_point;
    std::string M_message;

    std::string M_say_message;
    std::map< int, std::string > M_hear_messages;
public:
    //! construct from rcg v3 data (this is only for rcg v3)
    DebugViewData()
        : M_target_teammate( 0 )
      { }

    void clear();

    void setBall( boost::shared_ptr< BallT > ball )
      {
          M_ball = ball;
      }

    void setSelf( boost::shared_ptr< SelfT > self )
      {
          M_self = self;
      }

    void addTeammate( boost::shared_ptr< PlayerT > player )
      {
          M_teammates.push_back( player );
      }
    void addOpponent( boost::shared_ptr< PlayerT > player )
      {
          M_opponents.push_back( player );
      }
    void addUnknownTeammate( boost::shared_ptr< PlayerT > player )
      {
          M_unknown_teammates.push_back( player );
      }
    void addUnknownOpponent( boost::shared_ptr< PlayerT > player )
      {
          M_unknown_opponents.push_back( player );
      }
    void addUnknownPlayer( boost::shared_ptr< PlayerT > player )
      {
          M_unknown_players.push_back( player );
      }

    void addLine( const float & x1, const float & y1,
                  const float & x2, const float & y2,
                  const char * color )
      {
          M_lines.push_back( LineT( x1, y1, x2, y2, color ) );
      }
    void addTriangle( const float & x1, const float & y1,
                      const float & x2, const float & y2,
                      const float & x3, const float & y3,
                      const char * color )
      {
          M_triangles.push_back( TriangleT( x1, y1, x2, y2, x3, y3, color ) );
      }
    void addRectangle( const float & left_x,
                       const float & top_y,
                       const float & right_x,
                       const float & bottom_y,
                       const char * color )

      {
          M_rectangles.push_back( RectT( left_x, top_y, right_x, bottom_y, color ) );
      }
    void addCircle( const float & center_x,
                    const float & center_y,
                    const float & radius,
                    const char * color )
      {
          M_circles.push_back( CircleT( center_x, center_y, radius, color ) );
      }

    void setTargetTeammate( int unum )
      {
          M_target_teammate = unum;
      }
    void setTargetPoint( const double & x,
                         const double & y )
      {
          M_target_point
              = boost::shared_ptr< rcsc::Vector2D >( new rcsc::Vector2D( x, y ) );
      }
    void setMessage( const char * message )
      {
          M_message.assign( message );
      }
    void setSayMessage( const char * message )
      {
          M_say_message.assign( message );
      }
    void addHearMessage( const int unum,
                         const char * message )
      {
          std::string & msg = M_hear_messages[unum];
          msg += ' ';
          msg += message;
      }

    const boost::shared_ptr< BallT > & ball() const
      {
          return M_ball;
      }
    const boost::shared_ptr< SelfT > & self() const
      {
          return M_self;
      }
    const PlayerCont & teammates() const
      {
          return M_teammates;
      }
    const PlayerCont & opponents() const
      {
          return M_opponents;
      }
    const PlayerCont & unknownTeammates() const
      {
          return M_unknown_teammates;
      }
    const PlayerCont & unknownOpponents() const
      {
          return M_unknown_opponents;
      }
    const PlayerCont & unknownPlayers() const
      {
          return M_unknown_players;
      }

    const std::list< LineT > & lines() const
      {
          return M_lines;
      }
    const std::list< TriangleT > & triangles() const
      {
          return M_triangles;
      }
    const std::list< RectT > & rectangles() const
      {
          return M_rectangles;
      }
    const std::list< CircleT > & circles() const
      {
          return M_circles;
      }

    int targetTeammate() const
      {
          return M_target_teammate;
      }
    const boost::shared_ptr< rcsc::Vector2D > & targetPoint() const
      {
          return M_target_point;
      }
    const std::string & message() const
      {
          return M_message;
      }
    const std::string & sayMessage() const
      {
          return M_say_message;
      }
    const std::map< int, std::string > & hearMessages() const
      {
          return M_hear_messages;
      }

    std::ostream & print( std::ostream & os,
                          const rcsc::GameTime & time,
                          bool print_debug_line = true ) const;
};

#endif
