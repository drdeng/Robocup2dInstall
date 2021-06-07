// -*-c++-*-

/*!
  \file trainer.h
  \brief trainer for FormationDT Header File.
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

#ifndef FEDIT2_TRAINER_H
#define FEDIT2_TRAINER_H

#include <rcsc/formation/formation_dt.h>

#include <rcsc/geom/vector_2d.h>

#include <string>

class Trainer {
public:

    struct Data {
        rcsc::Vector2D ball_;
        int unum_;
        rcsc::Vector2D pos_;

        Data( const double bx,
              const double by,
              const int unum,
              const double px,
              const double py )
            : ball_( bx, by ),
              unum_( unum ),
              pos_( px, py )
          { }
    };


private:

    rcsc::FormationDT M_formation; //!< target formation
    std::vector< rcsc::formation::SampleData > M_target_data;

    std::vector< Data > M_training_data;

    double M_alpha; //!< learning rate
    double M_error_thr;
    int M_max_loop;

public:

    Trainer();

    bool readFormation( const std::string & filepath );
    bool readTrainingData( const std::string & filepath );
    std::ostream & printFormation( std::ostream & os ) const;

    void train();

private:
    /*!
      \return error value. negative value is returned if update process failed
     */
    double train( const Data & data );

};

#endif
