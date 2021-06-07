// -*-c++-*-

/*!
  \file detail_dialog.cpp
  \brief Detail info Dialog class Source File.
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

#include "detail_dialog.h"

#include "options.h"
#include "main_data.h"

#include <rcsc/common/server_param.h>
#include <rcsc/geom/vector_2d.h>

#include <iostream>
#include <cstdio>

/*-------------------------------------------------------------------*/
/*!

 */
DetailDialog::DetailDialog( QWidget * parent,
                            const MainData & main_data )
    : QDialog( parent )
    , M_main_data( main_data )
{
    this->setWindowTitle( tr( "Detail Dialog" ) );

    //     QFont font = this->font();
    //     font.setPointSize( 8 );
    //     this->setFont( font );

    createWidgets();

    //this->setWindowOpacity( 0.8 ); // window transparency
}

/*-------------------------------------------------------------------*/
/*!

 */
DetailDialog::~DetailDialog()
{
    //std::cerr << "delete DetailDialog" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
DetailDialog::createWidgets()
{
    QVBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins( 2, 2, 2, 2 );
    layout->setSpacing( 2 );
    layout->setSizeConstraint( QLayout::SetFixedSize );

    layout->addWidget( createBallLabels(),
                       1, Qt::AlignHCenter );

    layout->addWidget( createPlayerLabels(),
                       1, Qt::AlignHCenter );

    this->setLayout( layout );
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
DetailDialog::createBallLabels()
{
    QGroupBox * group_box = new QGroupBox( tr( "Ball" ) );
    group_box->setMinimumWidth( 160 );

    QGridLayout * layout = new QGridLayout();
    layout->setContentsMargins( 1, 1, 1, 1 );
    layout->setSpacing( 0 );
    layout->setColumnMinimumWidth( 0, this->fontMetrics().width( tr( "AttentionTo " ) ) );
    layout->setColumnMinimumWidth( 1, this->fontMetrics().width( tr( "-12.345, -12.345" ) ) );
    layout->setColumnMinimumWidth( 2, this->fontMetrics().width( tr( "--(123.456, -123.4)" ) ) );

    int row = 0;

    layout->addWidget( new QLabel( tr( "Pos " ) ), row, 0, Qt::AlignRight );
    M_ball_pos = new QLabel( tr( "   0.00,   0.00" ) );
    layout->addWidget( M_ball_pos, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Vel " ) ), row, 0, Qt::AlignRight );
    M_ball_vel = new QLabel( tr( "   0.00,   0.00" ) );
    layout->addWidget( M_ball_vel, row, 1, Qt::AlignRight );
    M_ball_polar_vel = new QLabel( tr( " (  0.000,    0.0)" ) );
    layout->addWidget( M_ball_polar_vel, row, 2, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "LastMove " ) ), row, 0, Qt::AlignRight );
    M_ball_last_move = new QLabel( tr( "   0.00,   0.00" ) );
    layout->addWidget( M_ball_last_move, row, 1, Qt::AlignRight );
    M_ball_last_polar_move = new QLabel( tr( " (  0.000,    0.0)" ) );
    layout->addWidget( M_ball_last_polar_move, row, 2, Qt::AlignRight );
    ++row;

    group_box->setLayout( layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
QWidget *
DetailDialog::createPlayerLabels()
{
    QGroupBox * group_box = new QGroupBox( tr( "Player" ) );
    group_box->setMinimumWidth( 160 );

    QGridLayout * layout = new QGridLayout();
    layout->setContentsMargins( 1, 1, 1, 1 );
    layout->setSpacing( 0 );
    layout->setColumnMinimumWidth( 0, this->fontMetrics().width( tr( "AttentionTo " ) ) );
    layout->setColumnMinimumWidth( 1, this->fontMetrics().width( tr( "-12.345, -12.345" ) ) );
    layout->setColumnMinimumWidth( 2, this->fontMetrics().width( tr( "--(123.456, -123.4)" ) ) );

    int row = 0;

    layout->addWidget( new QLabel( tr( "Unum " ) ), row, 0, Qt::AlignRight );
    M_player_number = new QLabel( tr( " Neutral  0" ) );
    layout->addWidget( M_player_number, row, 1, Qt::AlignHCenter );
    ++row;

    layout->addWidget( new QLabel( tr( "Type " ) ), row, 0, Qt::AlignRight );
    M_player_type = new QLabel( tr( "0" ) );
    layout->addWidget( M_player_type, row, 1, Qt::AlignHCenter );
    ++row;

    layout->addWidget( new QLabel( tr( "Pos " ) ), row, 0, Qt::AlignRight );
    M_player_pos = new QLabel( tr( "   0.00,   0.00" ) );
    layout->addWidget( M_player_pos, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Vel " ) ), row, 0, Qt::AlignRight );
    M_player_vel= new QLabel( tr( "   0.00,   0.00" ) );
    layout->addWidget( M_player_vel, row, 1, Qt::AlignRight );
    M_player_polar_vel = new QLabel( tr( " (  0.000,    0.0)" ) );
    layout->addWidget( M_player_polar_vel, row, 2, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "MaxTurn " ) ),
                       row, 0, Qt::AlignRight );
    M_max_turn_moment = new QLabel( tr( "   0.00" ) );
    layout->addWidget( M_max_turn_moment, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "LastMove " ) ), row, 0, Qt::AlignRight );
    M_player_last_move = new QLabel( tr( "   0.00,   0.00" ) );
    layout->addWidget( M_player_last_move, row, 1, Qt::AlignRight );
    M_player_last_polar_move = new QLabel( tr( " (  0.000,    0.0)" ) );
    layout->addWidget( M_player_last_polar_move, row, 2, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Body " ) ), row, 0, Qt::AlignRight );
    M_player_body = new QLabel( tr( "   0.0" ) );
    layout->addWidget( M_player_body, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Head " ) ), row, 0, Qt::AlignRight );
    M_player_head = new QLabel( tr( "   0.0" ) );
    layout->addWidget( M_player_head, row, 1, Qt::AlignRight );
    ++row;

//     layout->addWidget( new QLabel( tr( "CatchProb " ) ),
//                        row, 0, Qt::AlignRight );
//     M_player_catch_prob = new QLabel( tr( "   0.0" ) );
//     layout->addWidget( M_player_catch_prob, row, 1, Qt::AlignRight );
//     ++row;

    layout->addWidget( new QLabel( tr( "TackleProb " ) ),
                       row, 0, Qt::AlignRight );
    M_player_tackle_prob = new QLabel( tr( "   0.0" ) );
    layout->addWidget( M_player_tackle_prob, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "FoulProb " ) ),
                       row, 0, Qt::AlignRight );
    M_player_foul_prob = new QLabel( tr( "   0.0" ) );
    layout->addWidget( M_player_foul_prob, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "PointToPos " ) ),
                       row, 0, Qt::AlignRight );
    M_player_pointto_pos = new QLabel( tr( "      -,      -" ) );
    layout->addWidget( M_player_pointto_pos, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Focus " ) ),
                       row, 0, Qt::AlignRight );
    M_player_focus_target = new QLabel( tr( "      -,      -" ) );
    layout->addWidget( M_player_focus_target, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Stamina " ) ), row, 0, Qt::AlignRight );
    M_stamina = new QLabel( tr( "    0.0" ) );
    layout->addWidget( M_stamina, row, 1, Qt::AlignRight );
    //    ++row;
    //layout->addWidget( new QLabel( tr( "Capacity " ) ), row, 0, Qt::AlignRight );
    M_stamina_capacity = new QLabel( tr( "   -1.0" ) );
    layout->addWidget( M_stamina_capacity, row, 2, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Effort " ) ), row, 0, Qt::AlignRight );
    M_effort = new QLabel( tr( " 0.000 / 0.000" ) );
    layout->addWidget( M_effort, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Recovery " ) ), row, 0, Qt::AlignRight );
    M_recovery = new QLabel( tr( " 0.000 / 1.0" ) );
    layout->addWidget( M_recovery, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Kick " ) ), row, 0, Qt::AlignRight );
    M_kick_count = new QLabel( tr( "     0" ) );
    layout->addWidget( M_kick_count, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Dash " ) ), row, 0, Qt::AlignRight );
    M_dash_count = new QLabel( tr( "     0" ) );
    layout->addWidget( M_dash_count, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Turn " ) ), row, 0, Qt::AlignRight );
    M_turn_count = new QLabel( tr( "     0" ) );
    layout->addWidget( M_turn_count, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Say " ) ), row, 0, Qt::AlignRight );
    M_say_count = new QLabel( tr( "     0" ) );
    layout->addWidget( M_say_count, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "TurnNeck " ) ), row, 0, Qt::AlignRight );
    M_turn_neck_count = new QLabel( tr( "     0" ) );
    layout->addWidget( M_turn_neck_count, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Catch " ) ), row, 0, Qt::AlignRight );
    M_catch_count = new QLabel( tr( "     0" ) );
    layout->addWidget( M_catch_count, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Move " ) ), row, 0, Qt::AlignRight );
    M_move_count = new QLabel( tr( "     0" ) );
    layout->addWidget( M_move_count, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "ChgView " ) ), row, 0, Qt::AlignRight );
    M_change_view_count = new QLabel( tr( "     0" ) );
    layout->addWidget( M_change_view_count, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "Tackle " ) ),
                       row, 0, Qt::AlignRight );
    M_tackle_count = new QLabel( tr( "     0" ) );
    layout->addWidget( M_tackle_count, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "PointTo " ) ),
                       row, 0, Qt::AlignRight );
    M_pointto_count = new QLabel( tr( "     0" ) );
    layout->addWidget( M_pointto_count, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "AttentionTo " ) ),
                       row, 0, Qt::AlignRight );
    M_attentionto_count = new QLabel( tr( "     0" ) );
    layout->addWidget( M_attentionto_count, row, 1, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "BallRelPos " ) ), row, 0, Qt::AlignRight );
    M_player_ball_rel_pos = new QLabel( tr( "   0.00,   0.00" ) );
    layout->addWidget( M_player_ball_rel_pos, row, 1, Qt::AlignRight );
    M_player_ball_rel_polar = new QLabel( tr( " (  0.000,    0.0)" ) );
    layout->addWidget( M_player_ball_rel_polar, row, 2, Qt::AlignRight );
    ++row;

    layout->addWidget( new QLabel( tr( "BallRelVel " ) ), row, 0, Qt::AlignRight );
    M_player_ball_rel_vel = new QLabel( tr( "   0.00,   0.00" ) );
    layout->addWidget( M_player_ball_rel_vel, row, 1, Qt::AlignRight );
    ++row;


    group_box->setLayout( layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
DetailDialog::showEvent( QShowEvent * event )
{
    updateLabels();

    event->accept();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
DetailDialog::wheelEvent( QWheelEvent * event )
{
    if ( event->delta() < 0 )
    {
        this->setWindowOpacity( std::max( 0.1, this->windowOpacity() - 0.05 ) );
    }
    else
    {
        this->setWindowOpacity( std::min( 1.0, this->windowOpacity() + 0.05 ) );
    }

    event->accept();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
DetailDialog::updateLabels()
{
    if ( ! this->isVisible() )
    {
        return;
    }

    MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();

    if ( ! view )
    {
        return;
    }

    const Options & opt = Options::instance();

    MonitorViewData::ConstPtr last_view
        = ( M_main_data.viewIndex() <= 0
            ? MonitorViewData::ConstPtr()
            : M_main_data.getViewData( M_main_data.viewIndex() - 1 ) );

    //std::cerr << "DetailDialog::updteLabels start" << std::endl;

    const double reverse = opt.reverseValue();

    char buf[64];

    // update ball

    const rcsc::rcg::BallT & ball = view->ball();

    snprintf( buf, 64, " %6.2f, %6.2f",
              ball.x() * reverse,
              ball.y() * reverse );
    M_ball_pos->setText( QString::fromLatin1( buf ) );

    if ( ball.hasVelocity() )
    {
        rcsc::Vector2D vel( ball.deltaX() * reverse,
                            ball.deltaY() * reverse );
        snprintf( buf, 64, " %6.2f, %6.2f",
                  vel.x, vel.y );
        M_ball_vel->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, " (%7.3f, %6.1f)",
                  vel.r(), vel.th().degree() );
        M_ball_polar_vel->setText( QString::fromLatin1( buf ) );
    }
    else
    {
        M_ball_vel->setText( tr( "      -,      -" ) );
        M_ball_polar_vel->setText( tr( " (      -,      -)" ) );
    }

    if ( ! last_view )
    {
        M_ball_last_move->setText( tr( "   0.00,   0.00" ) );
        M_ball_last_polar_move->setText( tr( " (  0.000,    0.0)" ) );
    }
    else
    {
        rcsc::Vector2D move( ball.x() - last_view->ball().x(),
                             ball.y() - last_view->ball().y() );
        move *= reverse;

        snprintf( buf, 64, " %6.2f, %6.2f",
                  move.x, move.y );
        M_ball_last_move->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, " (%7.3f, %6.1f)",
                  move.r(), move.th().degree() );
        M_ball_last_polar_move->setText( QString::fromLatin1( buf ) );
    }

    // update player

    int number = opt.selectedNumber();
    if ( number == 0 || std::abs( number ) > 11 )
    {
        return;
    }

    const int player_index = std::abs( number ) - 1 + ( number > 0 ? 0 : 11 );
    const rcsc::ServerParam & SP = rcsc::ServerParam::i();
    const rcsc::rcg::PlayerT & player = view->players()[player_index];
    const rcsc::PlayerType & player_type
        = M_main_data.viewHolder().playerType( player.type() );

    // side, number
    snprintf( buf, 64, "%8s  %2d",
              ( opt.reverseSide()
                ? ( number > 0 ? "   Right" : "   Left" )
                : ( number > 0 ? "   Left" : "   Right" ) ),
              number > 0 ? number : -number );
    M_player_number->setText( QString::fromLatin1( buf ) );

    // player type
    snprintf( buf, 64, " %d" ,
              player.type() );
    M_player_type->setText( QString::fromLatin1( buf ) );

    // pos
    snprintf( buf, 64, " %6.2f, %6.2f",
              player.x() * reverse,
              player.y() * reverse );
    M_player_pos->setText( QString::fromLatin1( buf ) );

    // velocity, speed, speed dir
    if ( player.hasVelocity() )
    {
        rcsc::Vector2D vel( player.deltaX(), player.deltaY() );
        vel *= reverse;

        snprintf( buf, 64, " %6.2f, %6.2f",
                  vel.x, vel.y );
        M_player_vel->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, " (%7.3f, %6.1f)",
                  vel.r(), vel.th().degree() );
        M_player_polar_vel->setText( QString::fromLatin1( buf ) );

        // ball relative velocity
        snprintf( buf, 64, " %6.2f, %6.2f",
                  ball.deltaX() - vel.x, ball.deltaY() - vel.y );
        M_player_ball_rel_vel->setText( QString::fromLatin1( buf ) );

        // max turn moment
        double max_turn = player_type.effectiveTurn( 180.0, vel.r() );
        snprintf( buf, 64, " %3.2f", max_turn );
        M_max_turn_moment->setText( QString::fromLatin1( buf ) );
    }
    else
    {
        M_player_vel->setText( tr( "      -,      -" ) );
    }

    if ( ! last_view )
    {
        M_player_last_move->setText( tr( "   0.00,   0.00" ) );
        M_player_last_polar_move->setText( tr( "  (  0.000,    0.0)" ) );
    }
    else
    {
        const rcsc::rcg::PlayerT & last_player = last_view->players()[player_index];
        rcsc::Vector2D last_move( player.x() - last_player.x(),
                                  player.y() - last_player.y() );
        last_move *= reverse;

        snprintf( buf, 64, " %6.2f, %6.2f",
                  last_move.x, last_move.y );
        M_player_last_move->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, " (%7.3f, %6.1f)",
                  last_move.r(), last_move.th().degree() );
        M_player_last_polar_move->setText( QString::fromLatin1( buf ) );
    }

    // body angle
    rcsc::AngleDeg body = player.body();
    if ( opt.reverseSide() ) body += 180.0;

    snprintf( buf, 64," %6.1f",
              body.degree() );
    M_player_body->setText( QString::fromLatin1( buf ) );

    // neck, head angle
    if ( player.hasView() )
    {
        rcsc::AngleDeg neck = player.head() - player.body();
        rcsc::AngleDeg head = player.head();
        if ( opt.reverseSide() ) head += 180.0;

        snprintf( buf, 64, " %6.1f(%6.1f)",
                  head.degree(), neck.degree() );
        M_player_head->setText( QString::fromLatin1( buf ) );
    }
    else
    {
        M_player_head->setText( tr( "     -" ) );
    }

    {
        rcsc::Vector2D player_to_ball( ball.x() - player.x(),
                                       ball.y() - player.y() );
        player_to_ball.rotate( - player.body() );
#if 0
        //
        // catch prob
        //
        if ( player.isGoalie() )
        {
            const double catchable_area
                = std::sqrt( std::pow( SP.catchAreaWidth() * 0.5, 2 )
                             + std::pow( SP.catchAreaLength(), 2 ) );
            const double stretch_l = SP.catchAreaLength() * player_type.catchAreaLengthStretch();
            const double stretch_area = std::sqrt( std::pow( SP.catchAreaWidth() * 0.5, 2 )
                                                   + std::pow( stretch_l, 2 ) );
            const double ball_dist = player_to_ball.r();

            double catch_prob = SP.catchProbability();
            if ( ball_dist > stretch_area )
            {
                catch_prob = 0.0;
            }
            else if ( ball_dist > catchable_area )
            {
                double x = ball_dist * ( stretch_l / stretch_area );
                catch_prob
                    = SP.catchProbability()
                    - SP.catchProbability() * ( ( x - SP.catchAreaLength() )
                                                / ( stretch_l - SP.catchAreaLength() ) );
            }

            snprintf( buf, 64, " %.4f", catch_prob );
            M_player_catch_prob->setText( QString::fromLatin1( buf ) );
        }
        else
        {
            M_player_catch_prob->setText( tr( "0.0000" ) );
        }
#endif
        //
        // tackle prob
        //
        double tackle_dist = ( player_to_ball.x > 0.0
                               ? SP.tackleDist()
                               : SP.tackleBackDist() );
        double tackle_fail_prob = 1.0;
        if ( tackle_dist > 1.0e-5 )
        {
            tackle_fail_prob = ( std::pow( std::fabs( player_to_ball.x ) / tackle_dist,
                                           SP.tackleExponent() )
                                 + std::pow( player_to_ball.absY() / SP.tackleWidth(),
                                             SP.tackleExponent() ) );
            tackle_fail_prob = std::min( 1.0, tackle_fail_prob );
            tackle_fail_prob = std::max( 0.0, tackle_fail_prob );
        }

        snprintf( buf, 64, " %.4f", 1.0 - tackle_fail_prob );
        M_player_tackle_prob->setText( QString::fromLatin1( buf ) );

        //
        // foul prob
        //
        double foul_fail_prob = 1.0;
        if ( tackle_dist > 1.0e-5 )
        {
            foul_fail_prob = ( std::pow( std::fabs( player_to_ball.x ) / tackle_dist,
                                         SP.foulExponent() )
                               + std::pow( player_to_ball.absY() / SP.tackleWidth(),
                                             SP.foulExponent() ) );
            foul_fail_prob = std::min( std::max( 0.0, foul_fail_prob ), 1.0 );
        }

        snprintf( buf, 64, " %.4f", 1.0 - foul_fail_prob );
        M_player_foul_prob->setText( QString::fromLatin1( buf ) );

    }

    // pointto point
    if ( player.isPointing() )
    {
        snprintf( buf, 64, " %6.2f, %6.2f",
                  player.pointX() * reverse,
                  player.pointY() * reverse );
        M_player_pointto_pos->setText( QString::fromLatin1( buf ) );
    }
    else
    {
        M_player_pointto_pos->setText( tr( "      -,      -") );
    }

    // focus target
    if ( player.isFocusing() )
    {
        snprintf( buf, 64, " %5s %2d",
                  ( opt.reverseSide()
                    ? player.focusSide() == rcsc::LEFT ? "Right" : "Left"
                    : player.focusSide() == rcsc::LEFT ? "Left" : "Right" ),
                  player.focus_unum_ );
        M_player_focus_target->setText( QString::fromLatin1( buf ) );
    }
    else
    {
        M_player_focus_target->setText( tr( " --- ") );
    }

    // stamina, effort, recovery
    if ( player.hasStamina() )
    {
        snprintf( buf, 64, " %6.1f",
                  player.stamina() );
        M_stamina->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, " %.0f",
                  player.staminaCapacity() );
        M_stamina_capacity->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, " %5.3f / %5.3f",
                  player.effort(), player_type.effortMax() );
        M_effort->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, " %5.3f / 1.0",
                  player.recovery() );
        M_recovery->setText( QString::fromLatin1( buf ) );
    }
    else
    {
        M_stamina->setText( tr( "      -" ) );
        M_stamina_capacity->setText( tr( "      -" ) );
        M_effort->setText( tr( "     - /     -" ) );
        M_recovery->setText( tr( "     - / 1.0" ) );
    }

    // command count
    if ( player.hasCommandCount() )
    {
        snprintf( buf, 64, "  %5d",
                  player.kickCount() );
        M_kick_count->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, "  %5d",
                  player.dashCount() );
        M_dash_count->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, "  %5d",
                  player.turnCount() );
        M_turn_count->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, "  %5d",
                  player.sayCount() );
        M_say_count->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, "  %5d",
                  player.turnNeckCount() );
        M_turn_neck_count->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, "  %5d",
                  player.catchCount() );
        M_catch_count->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, "  %5d",
                  player.moveCount() );
        M_move_count->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, "  %5d",
                  player.changeViewCount() );
        M_change_view_count->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, "  %5d",
                  player.tackleCount() );
        M_tackle_count->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, "  %5d",
                  player.pointtoCount() );
        M_pointto_count->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, "  %5d",
                  player.attentiontoCount() );
        M_attentionto_count->setText( QString::fromLatin1( buf ) );
    }
    else
    {
        M_kick_count->setText( tr( "     -" ) );
        M_dash_count->setText( tr( "     -" ) );
        M_turn_count->setText( tr( "     -" ) );
        M_say_count->setText( tr( "     -" ) );
        M_turn_neck_count->setText( tr( "     -" ) );
        M_catch_count->setText( tr( "     -" ) );
        M_move_count->setText( tr( "     -" ) );
        M_change_view_count->setText( tr( "     -" ) );
        M_tackle_count->setText( tr( "     -" ) );
        M_pointto_count->setText( tr( "     -" ) );
        M_attentionto_count->setText( tr( "     -" ) );
    }

    // ball related info
    {
        rcsc::Vector2D player_to_ball( ball.x() - player.x(),
                                       ball.y() - player.y() );
        player_to_ball *= reverse;

        snprintf( buf, 64, " %6.2f, %6.2f",
                  player_to_ball.x, player_to_ball.y );
        M_player_ball_rel_pos->setText( QString::fromLatin1( buf ) );

        snprintf( buf, 64, " (%7.3f, %6.1f)",
                  player_to_ball.r(), player_to_ball.th().degree() );
        M_player_ball_rel_polar->setText( QString::fromLatin1( buf ) );
    }
}
