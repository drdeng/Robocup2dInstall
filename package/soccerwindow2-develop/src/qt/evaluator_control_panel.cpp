// -*-c++-*-

/*!
  \file debug_evaluator_chain_control_widget.cpp
  \brief control widget to debug evaluator and chain actions Source File.
*/

/*
 *Copyright:

 Copyright (C) Hiroki SHIMORA, Hidehisa AKIYAMA

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

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "evaluator_control_panel.h"

#include "agent_id.h"
#include "action_sequence_log_parser.h"
#include "debug_log_data.h"
#include "field_evaluator_message_parser.h"
#include "grid_field_evaluation_data.h"
#include "main_data.h"
#include "options.h"

#include "action_sequence_selector.h"

#include <rcsc/game_time.h>
#include <rcsc/common/logger.h>

#include <iostream>
#include <sstream>

/*-------------------------------------------------------------------*/
/*!

 */
EvaluatorControlPanel::EvaluatorControlPanel( QWidget * parent,
                                              MainData & main_data )
    : QFrame( parent ),
      M_main_data( main_data )
{
    //
    // create layout
    //
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 2, 2, 2, 2 );
    this->setLayout( top_layout );


    //
    // evaluate range sliders
    //
    M_evaluation_min_thr_slider = new QSlider( Qt::Horizontal );
    M_evaluation_max_thr_slider = new QSlider( Qt::Horizontal );
    M_evaluation_min_thr_slider->setTracking( true );
    M_evaluation_max_thr_slider->setTracking( true );

    M_evaluation_min_thr_slider->setValue( M_evaluation_min_thr_slider->minimum() );
    M_evaluation_max_thr_slider->setValue( M_evaluation_min_thr_slider->maximum() );

    connect( M_evaluation_min_thr_slider, SIGNAL( valueChanged( int ) ),
             this, SLOT( updateEvaluatorMinThreshold( int ) ) );
    connect( M_evaluation_max_thr_slider, SIGNAL( valueChanged( int ) ),
             this, SLOT( updateEvaluatorMaxThreshold( int ) ) );

    top_layout->addWidget( M_evaluation_min_thr_slider );
    top_layout->addWidget( M_evaluation_max_thr_slider );

    M_evaluation_min_thr_slider->hide();
    M_evaluation_max_thr_slider->hide();
#if 0
    {
        QHBoxLayout * evaluation_layout = new QHBoxLayout();
        top_layout->addLayout( evaluation_layout );

        //
        // evaluate field button
        //
        QPushButton * execute_btn = new QPushButton( tr( "execute evaluator" ) );
        connect( execute_btn, SIGNAL( clicked() ), this, SLOT( executeEvaluator() ) );
        evaluation_layout->addWidget( execute_btn, 1 );

        //
        // clear all evaluation button
        //
        QPushButton * clear_btn = new QPushButton( tr( "clear all field evaluation" ) );
        connect( clear_btn, SIGNAL( clicked() ), this, SLOT( clearAllEvaluation() ) );
        evaluation_layout->addWidget( clear_btn, 1 );
    }
#endif
    {
        QHBoxLayout * layout = new QHBoxLayout();
        top_layout->addLayout( layout );

        QPushButton * btn = new QPushButton( tr( "show action sequence selector" ) );
        connect( btn, SIGNAL( clicked() ), this, SIGNAL( showSelector() ) );
        layout->addWidget( btn, 1 );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
EvaluatorControlPanel::~EvaluatorControlPanel()
{
    // std::cerr << "delete EvaluatorControlPanel" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EvaluatorControlPanel::updateEvaluatorMinThreshold( int value )
{
    const int s_max = M_evaluation_min_thr_slider->maximum();
    const int s_min = M_evaluation_min_thr_slider->minimum();
    const int d = s_max - s_min;

    if ( d == 0 )
    {
        return;
    }

    if ( value > M_evaluation_max_thr_slider->value() )
    {
        M_evaluation_max_thr_slider->setValue( value );
    }

    const int v = M_evaluation_min_thr_slider->value();
    const double rate = static_cast< double >( v - s_min ) / static_cast< double >( d );

    Options::instance().setEvaluationViewMinRate( rate );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EvaluatorControlPanel::updateEvaluatorMaxThreshold( int value )
{
    const int s_max = M_evaluation_max_thr_slider->maximum();
    const int s_min = M_evaluation_max_thr_slider->minimum();
    const int d = s_max - s_min;

    if ( d == 0 )
    {
        return;
    }

    const int v = M_evaluation_max_thr_slider->value();
    const double rate = static_cast< double >( v - s_min ) / static_cast< double >( d );

    Options::instance().setEvaluationViewMaxRate( rate );

    if ( value < M_evaluation_min_thr_slider->value() )
    {
        M_evaluation_min_thr_slider->setValue( value );
        Options::instance().setEvaluationViewMinRate( rate );
    }

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EvaluatorControlPanel::executeEvaluator()
{
    const Options & opt = Options::instance();

    if ( opt.evaluatorName().empty() )
    {
        std::cerr << __FILE__ << ": (executeEvaluator) "
                  << "No evaluator name." << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "No evaluator name." ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    const AgentID pl = opt.selectedAgent();
    if ( pl.isNull() )
    {
        std::cerr << __FILE__ << ": (executeEvaluator) "
                  << "side not selected!" << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "side not selected!" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    const MonitorViewData::ConstPtr view_ptr = M_main_data.getCurrentViewData();
    if ( ! view_ptr )
    {
        std::cerr << __FILE__ << ": (executeEvaluator) "
                  << "no monitor view found!" << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "no monitor view found!" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    const rcsc::GameTime current_time = view_ptr->time();

    const DebugViewData::ConstPtr debug_view = M_main_data.viewHolder().getDebugView( current_time, pl );
    if ( ! debug_view )
    {
        std::cerr << __FILE__ << ": (executeEvaluator) "
                  << "no debug view found!" << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "no debug view found!" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    std::ostringstream debug_message_buf;
    debug_view->print( debug_message_buf, current_time, false );

    const std::string debug_message = debug_message_buf.str();
    std::cerr << __FILE__ << ": (executeEvaluator)\n"
              << " debug view string = [" << debug_message << "]" << std::endl;


    const double grid_size = std::max( 0.01, opt.evaluatorGridSize() );
    const std::string command = ( pl.side() == rcsc::LEFT
                                  ? opt.evaluatorCommandLeft()
                                  : opt.evaluatorCommandRight() );
    const std::string param_path = ( pl.side() == rcsc::LEFT
                                     ? opt.evaluatorParamFileLeft()
                                     : opt.evaluatorParamFileRight() );

    if ( command.empty() )
    {
        std::cerr << __FILE__ << ": (executeEvaluator) "
                  << "empty evaluator command!" << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "Empty command string!" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    const QString argv0( command.c_str() );

    QStringList argv;

    argv << "--evaluator-name" << QString::fromStdString( opt.evaluatorName() );

    if ( ! param_path.empty() )
    {
        argv << "--evaluator-param-file" << QString::fromStdString( param_path );
    }

    std::cerr << __FILE__ << ": (executeEvaluator) \n"
              << " invoking command [" << argv0.toStdString() << " "
              << argv.join( " " ).toStdString() << "]" << std::endl;

    QProcess * proc( new QProcess( this ) );
    proc->start( argv0, argv, QIODevice::ReadWrite );

    if ( ! proc->waitForStarted() )
    {
        std::cerr << __FILE__ << ": (executeEvaluator)\n"
                  << " failed to start the command [" << command << "]" << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "Command execution failed[" )
                               + QString::fromStdString( command )
                               + tr( "]" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    proc->write( QByteArray( debug_message.data(), debug_message.size() ) );
    proc->closeWriteChannel();


    std::cerr << __FILE__ << ": (executeEvaluator) evaluating field grid ...";
    const bool success = proc->waitForFinished();
    if ( ! success )
    {
        std::cerr << __FILE__ << ": (executeEvaluator)\n"
                  << " failed to finish the command [" << command << "]" << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "Command execution failed[" )
                               + QString::fromStdString( command )
                               + tr( "]" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    const QString buf = QString::fromLocal8Bit( proc->readAllStandardOutput() );
    const QString buf_err = QString::fromLocal8Bit( proc->readAllStandardError() );

    std::cerr << __FILE__ << ": (executeEvaluator) done\n"
        // << "stdout = [" << buf.toStdString() << "]\n";
              << "stderr = [" << buf_err.toStdString() << ']' << std::endl;

    boost::shared_ptr< GridFieldEvaluationData > data = FieldEvaluatorMessageParser().parse( buf.toStdString(), grid_size );
    if ( ! data )
    {
        std::cerr << __FILE__ << ": (executeEvaluator) "
                  << "field evaluation parsing failed" << std::endl;
        QMessageBox::critical( this,
                               tr( "Error" ),
                               tr( "Failed field evaluation parsing" ),
                               QMessageBox::Ok, QMessageBox::NoButton );
        return;
    }

    M_main_data.setGridFieldEvaluation( current_time, pl, data );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
EvaluatorControlPanel::clearAllEvaluation()
{
    M_main_data.clearGridFieldEvaluation();
}
