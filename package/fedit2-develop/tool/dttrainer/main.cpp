// -*-c++-*-

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "trainer.h"

#include <iostream>
#include <fstream>
#include <string>

int
main( int argc,
      char ** argv )
{
    Trainer trainer;

    std::string formation_conf;
    std::string training_data;

    if ( argc < 3 )
    {
        std::cerr << "Usage: "
                  << argv[0] << " <FORMATION.CONF> <TRAINING.DAT>"
                  << std::endl;
        return 1;
    }

    formation_conf = argv[1];
    training_data = argv[2];

    if ( ! trainer.readFormation( formation_conf ) )
    {
        std::cerr << "ERROR: readFormation" << std::endl;
        return 1;
    }

    if ( ! trainer.readTrainingData( training_data ) )
    {
        std::cerr << "ERROR: readTrainingData" << std::endl;
        return 1;
    }

    std::cerr << "start" << std::endl;
    trainer.train();

    {
        std::string result_conf = "result.conf";
        std::ofstream fout( result_conf.c_str() );
        trainer.printFormation( fout );
    }
    std::cerr << "finished all" << std::endl;
    return 0;
}
