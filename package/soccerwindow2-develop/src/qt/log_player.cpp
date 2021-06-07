// -*-c++-*-

/*!
  \file log_player.cpp
  \brief log player class Source File.
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

#include <QTimer>

#include "log_player.h"

#include "options.h"
#include "main_data.h"

#include <iostream>

/*-------------------------------------------------------------------*/
/*!

*/
LogPlayer::LogPlayer( MainData & main_data,
                      QObject * parent )
    : QObject( parent ),
      M_main_data( main_data ),
      M_timer( new QTimer( this ) ),
      M_forward( true ),
      M_live_mode( false )
{
    connect( M_timer, SIGNAL( timeout() ),
             this, SLOT( handleTimer() ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
LogPlayer::~LogPlayer()
{
    //std::cerr << "delete LogPlayer" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::handleTimer()
{
    //std::cerr << "LogPlayer::handleTimer" << std::endl;
    if ( M_forward )
    {
        stepForwardImpl();
    }
    else
    {
        stepBackImpl();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
LogPlayer::isLiveMode() const
{
    return M_live_mode;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::stepBackImpl()
{
    if ( M_main_data.setViewDataStepBack() )
    {
        emit updated();
    }
    else
    {
        M_timer->stop();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::stepForwardImpl()
{
    if ( M_main_data.setViewDataStepForward() )
    {
        //std::cerr << "LogPlayer::stepFormawdImpl  index = "
        //          << M_main_data.viewIndex()
        //          << std::endl;
        emit updated();
    }
    else
    {
        M_timer->stop();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::stepBack()
{
    //std::cerr << "LogPlayer::stepBack" << std::endl;
    M_live_mode = false;
    M_timer->stop();

    stepBackImpl();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::stepForward()
{
    //std::cerr << "LogPlayer::stepForward" << std::endl;
    M_live_mode = false;
    M_timer->stop();

    stepForwardImpl();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::playOrStop()
{
    //std::cerr << "LogPlayer::playOrStop" << std::endl;
    M_live_mode = false;

    if ( M_timer->isActive() )
    {
        M_timer->stop();
    }
    else
    {
        playForward();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::stop()
{
    //std::cerr << "LogPlayer::stop" << std::endl;
    M_live_mode = false;
    M_timer->stop();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::playBack()
{
    //std::cerr << "LogPlayer::playBack" << std::endl;
    M_live_mode = false;
    M_forward = false;

    if ( ! M_timer->isActive() )
    {
        M_timer->start( Options::instance().timerInterval() );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::playForward()
{
    //std::cerr << "LogPlayer::playForward" << std::endl;
    M_live_mode = false;
    M_forward = true;

    if ( ! M_timer->isActive() )
    {
        M_timer->start( Options::instance().timerInterval() );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::goToPrevScore()
{
    //std::cerr << "LogPlayer::goToPrevScore" << std::endl;
    M_live_mode = false;

    const ViewHolder & holder = M_main_data.viewHolder();

    const std::size_t cur_idx = M_main_data.viewIndex();
    for ( std::vector< std::size_t >::const_reverse_iterator it = holder.scoreChangeIndices().rbegin(),
              rend = holder.scoreChangeIndices().rend();
          it != rend;
          ++it )
    {
        if ( *it < cur_idx )
        {
            std::size_t new_idx = *it;
            new_idx -= ( new_idx < 50 ? new_idx : 50 );
            goToIndex( new_idx );
            return;
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::goToNextScore()
{
    //std::cerr << "LogPlayer::goToNextScore" << std::endl;
    M_live_mode = false;

    const ViewHolder & holder = M_main_data.viewHolder();

    const std::size_t cur_idx = M_main_data.viewIndex();
    for ( std::vector< std::size_t >::const_iterator it = holder.scoreChangeIndices().begin(),
              end = holder.scoreChangeIndices().end();
          it != end;
          ++it )
    {
        if ( 50 < *it && cur_idx < *it - 50 )
        {
            std::size_t new_idx = *it;
            new_idx -= ( new_idx < 50 ? 0 : 50 );
            goToIndex( new_idx );
            return;
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::goToFirst()
{
    //std::cerr << "LogPlayer::goToFirst" << std::endl;
    if ( M_main_data.setViewDataIndexFirst() )
    {
        M_live_mode = false;
        M_timer->stop();

        emit updated();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::goToLast()
{
    //std::cerr << "LogPlayer::goToLast" << std::endl;
    if ( M_main_data.setViewDataIndexLast() )
    {
        M_live_mode = false;
        M_timer->stop();

        emit updated();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::decelerate()
{
    //    std::cerr << "LogPlayer::decelerate" << std::endl;
    if ( M_timer->isActive() )
    {
        int interval = M_timer->interval() * 2;
        if ( 5000 < interval ) interval = 5000;
        M_timer->start( interval );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::accelerate()
{
    //std::cerr << "LogPlayer::accelerate" << std::endl;
    if ( M_timer->isActive() )
    {
        int interval = M_timer->interval() / 2;
        if ( interval < 5 ) interval = 5;
        M_timer->start( interval );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::goToIndex( int index )
{
    //std::cerr << "LogPlayer::goToIndex " << index << std::endl;
    if ( M_main_data.setViewDataIndex( index ) )
    {
        M_live_mode = false;
        //M_timer->stop();

        emit updated();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::goToCycle( int cycle )
{
    //std::cerr << "LogPlayer::goToCycle " << cycle << std::endl;
    if ( M_main_data.setViewDataCycle( cycle ) )
    {
        M_live_mode = false;
        //M_timer->stop();

        emit updated();
    }
}


/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::goToCycle( const rcsc::GameTime & t )
{
    //std::cerr << "LogPlayer::goToCycle " << cycle << std::endl;
    if ( M_main_data.setViewDataCycle( t ) )
    {
        M_live_mode = false;
        //M_timer->stop();

        emit updated();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::showLive()
{
    if ( M_main_data.setViewDataIndexLast() )
    {
        M_timer->stop();

        emit updated();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayer::setLiveMode()
{
    //    std::cerr << "LogPlayer::setLiveMode" << std::endl;
    M_main_data.setViewDataIndexLast();
    M_live_mode = true;
    M_timer->stop();

    //emit updated();
}
