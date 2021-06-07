// -*-c++-*-

/*!
  \file draw_data_holder.h
  \brief draw data holder class Header File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa Akiyama

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

#ifndef SOCCERWINDOW2_DRAW_DATA_HOLDER_H
#define SOCCERWINDOW2_DRAW_DATA_HOLDER_H

#include <rcsc/game_time.h>

#include <vector>
#include <map>
#include <string>

class DrawDataHolder {
public:

    struct TextT {
        double x_;
        double y_;
        std::string color_;
        std::string msg_;

        TextT( const double & x,
               const double & y,
               const std::string & color,
               const std::string & msg )
            : x_( x ),
              y_( y ),
              color_( color ),
              msg_( msg )
          { }
    };

    struct PointT {
        double x_;
        double y_;
        std::string color_;

        PointT( const double x,
                const double y,
                const std::string & color )
            : x_( x ),
              y_( y ),
              color_( color )
          { }
    };

    struct LineT {
        double x1_;
        double y1_;
        double x2_;
        double y2_;
        std::string color_;

        LineT( const double x1,
               const double y1,
               const double x2,
               const double y2,
               const std::string & color )
            : x1_( x1 ),
              y1_( y1 ),
              x2_( x2 ),
              y2_( y2 ),
              color_( color )
          { }

    };

    struct RectT {
        double left_;
        double top_;
        double width_;
        double height_;
        std::string line_color_;
        std::string fill_color_;

        RectT( const double left,
               const double top,
               const double width,
               const double height,
               const std::string & line_color,
               const std::string & fill_color )
            : left_( left ),
              top_( top ),
              width_( width ),
              height_( height ),
              line_color_( line_color ),
              fill_color_( fill_color )
          { }
    };

    struct CircleT {
        double x_;
        double y_;
        double r_;
        std::string line_color_;
        std::string fill_color_;

        CircleT( const double x,
                 const double y,
                 const double r,
                 const std::string & line_color,
                 const std::string & fill_color )
            : x_( x ),
              y_( y ),
              r_( r ),
              line_color_( line_color ),
              fill_color_( fill_color )
          { }
    };

    typedef std::vector< TextT > TextCont;
    typedef std::vector< PointT > PointCont;
    typedef std::vector< LineT > LineCont;
    typedef std::vector< RectT > RectCont;
    typedef std::vector< CircleT > CircleCont;

    struct Data {
        TextCont texts_;
        PointCont points_;
        LineCont lines_;
        RectCont rects_;
        CircleCont circles_;
    };

    typedef std::map< rcsc::GameTime, Data, rcsc::GameTime::Less > DataMap;

private:

    DataMap M_data_map;

public:

    DrawDataHolder();
    ~DrawDataHolder();

    void clear();
    bool open( const std::string & filepath );

    const DataMap & dataMap() const { return M_data_map; }

    void addText( const rcsc::GameTime & time,
                  const TextT & text );
    void addPoint( const rcsc::GameTime & time,
                   const PointT & point );
    void addLine( const rcsc::GameTime & time,
                  const LineT & line );
    void addRect( const rcsc::GameTime & time,
                  const RectT & rect );
    void addCircle( const rcsc::GameTime & time,
                    const CircleT & circle );

};

#endif
