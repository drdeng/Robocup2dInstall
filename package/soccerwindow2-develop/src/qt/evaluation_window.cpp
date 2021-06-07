// -*-c++-*-

/*!
  \file evaluation_window.cpp
  \brief field evaluation viewer class Source File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA, Hiroki SHIMORA

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

#include "evaluation_window.h"

#include "main_data.h"
#include "options.h"

#include <sstream>

/*-------------------------------------------------------------------*/
/*!

*/
EvaluationWindow::EvaluationWindow( QWidget * parent,
                                    const MainData & main_data )
    : QMainWindow( parent ),
      M_main_data( main_data ),
      M_actual_evaluation_value( 0.0 )
{
    this->setWindowTitle( tr( "Field Evaluation" ) );
    this->resize( 600, 400 );

    //
    // create widgets
    //

    QFrame * frame = new QFrame( this );
    this->setCentralWidget( frame );

    M_text = new QTextEdit();
    M_text->setReadOnly( true );

    QPushButton * up_button = new QPushButton( "Up" );
    connect( up_button, SIGNAL( clicked() ), this, SLOT( upEvaluation() ) );

    QPushButton * down_button = new QPushButton( "Down" );
    connect( down_button, SIGNAL( clicked() ), this, SLOT( downEvaluation() ) );

    QPushButton * reset_button = new QPushButton( "Reset" );
    connect( reset_button, SIGNAL( clicked() ), this, SLOT( resetEvaluation() ) );

    QPushButton * train_button = new QPushButton( "Train" );
    connect( train_button, SIGNAL( clicked() ), this, SLOT( executeTrain() ) );


    M_evaluation_point = new QLineEdit( "0, 0" );
    M_evaluation_point->setReadOnly( true );

    M_evaluation_value = new QLineEdit( "0.0" );
    //M_evaluation_value->setReadOnly( true );
    M_evaluation_value->setValidator( new QDoubleValidator( M_evaluation_value ) );

    //
    // create layouts
    //

    QHBoxLayout * button_layout = new QHBoxLayout();
    button_layout->setContentsMargins( 2, 0, 2, 0 );
    button_layout->addWidget( reset_button );
    button_layout->addStretch( 1 );
    button_layout->addWidget( train_button );
    button_layout->addStretch( 1 );
    button_layout->addWidget( up_button );
    button_layout->addWidget( down_button );

    QHBoxLayout * eval_layout = new QHBoxLayout();
    eval_layout->addWidget( new QLabel( "Pos:" ) );
    eval_layout->addWidget( M_evaluation_point );
    eval_layout->addWidget( new QLabel( "Eval:" ) );
    eval_layout->addWidget( M_evaluation_value );


    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setContentsMargins( 0, 0, 0, 0 );

    top_layout->addWidget( M_text );
    top_layout->addLayout( eval_layout );
    top_layout->addLayout( button_layout );

    frame->setLayout( top_layout );
}

/*-------------------------------------------------------------------*/
/*!

*/
EvaluationWindow::~EvaluationWindow()
{
    //std::cerr << "delete EvaluationWindow" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EvaluationWindow::evaluatePoint( const QPoint & point )
{
    const Options & opt = Options::instance();

    double x = opt.fieldX( point.x() );
    double y = opt.fieldY( point.y() );

    if ( opt.reverseSide() )
    {
        x = -x;
        y = -y;
    }

    std::cerr << __FILE__ << ": (evaluatePoint) \n"
              << "  window pos = [" << point.x() << ", " << point.y() << "]\n"
              << "  field pos  = [" << x  << ", " << y << ']' << std::endl;

    rcsc::Vector2D pos( x, y );


    const AgentID pl = opt.selectedAgent();
    if ( pl.isNull() )
    {
        M_text->clear();
        return;
    }

    //const rcsc::GameTime & current_time = M_main_data.debugLogHolder().currentTime();
    const MonitorViewData::ConstPtr view = M_main_data.getCurrentViewData();
    if ( ! view )
    {
        std::cerr << __FILE__ << ": (evaluatePoint) "
                  << "No monitor view." << std::endl;
        M_text->clear();
        return;
    }

    const rcsc::GameTime current_time = view->time();

    const DebugViewData::ConstPtr debug_view = M_main_data.viewHolder().getDebugView( current_time, pl );
    if ( ! debug_view )
    {
        std::cerr << __FILE__ << ": (evaluatePoint) "
                  << "debug view is null" << std::endl;
        M_text->clear();
        return;
    }

    if ( opt.evaluatorName().empty() )
    {
        std::cerr << __FILE__ << ": (evaluatePoint) "
                  << "No evaluator name." << std::endl;
        return;
    }

    std::ostringstream debug_message_buf;
    debug_view->print( debug_message_buf, current_time, false );

    const std::string debug_message = debug_message_buf.str();
    // std::cerr << "debug view str = [" << debug_message << "]" << std::endl;

    const std::string command = ( pl.side() == rcsc::LEFT
                                    ? opt.evaluatorCommandLeft()
                                    : opt.evaluatorCommandRight() );
    const std::string param_file = ( pl.side() == rcsc::LEFT
                                     ? opt.evaluatorParamFileLeft()
                                     : opt.evaluatorParamFileRight() );

    std::ostringstream pos_str;
    pos_str << x << "," << y;

    const QString argv0 = QString::fromStdString( command );
    QStringList argv;
    argv << "--evaluator-name" << QString::fromStdString( opt.evaluatorName() )
         << "--evaluator-param-file" << QString::fromStdString( param_file )
         << "--pos" << QString::fromStdString( pos_str.str() );

    QProcess * proc( new QProcess( this ) );
    proc->start( argv0, argv, QIODevice::ReadWrite );

    if ( ! proc->waitForStarted() )
    {
        std::cerr << __FILE__ << ": (evaluatePoint)\n"
                  << " failed to start the command [" << command << "]" << std::endl;
        M_text->clear();
        return;
    }

    proc->write( QByteArray( debug_message.data(), debug_message.size() ) );
    proc->closeWriteChannel();


    const bool success = proc->waitForFinished();
    if ( ! success )
    {
        std::cerr << __FILE__ << ": (evaluatePoint)\n"
                  << " failed to finish the command [" << command << "]" << std::endl;
        M_text->clear();
        return;
    }


    const QString buf = QString::fromLocal8Bit( proc->readAllStandardOutput() );
    const QString buf_err = QString::fromLocal8Bit( proc->readAllStandardError() );

    // std::cerr << "stdout = [" << buf.toStdString() << "]" << std::endl;
    // std::cerr << "stderr = [" << buf_err.toStdString() << "]" << std::endl;
    qDebug() << buf_err;

    M_text->setText( buf );

    parseEvaluatorLog( buf.toStdString() );
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
EvaluationWindow::parseEvaluatorLog( const std::string & msg )
{
    /*
      1,0 1048576 M ========= param_field_evaluator.cpp ==========
      1,0 1048576 M THEIR_GOAL_DIST_RATE 3.287637
      1,0 1048576 M NEAR_OFFSIDE_LINE_DIST_THRETHOLD 0.159350
      1,0 1048576 M NEAR_OFFSIDE_BONUS_RATE 0.154297
      1,0 1048576 M NEAR_DEFENSE_LINE_DIST_THRETHOLD 4.014266
      1,0 1048576 M ATTACK_LINE_X_THRESHOLD 11.621659
      1,0 1048576 M OPP_DIST_THRETHOLD 10.000000
      1,0 1048576 M OPP_DIST_FACTOR 2.500000
      1,0 1048576 M PASS_COUNT_BONUS_RATE 10.000000
      1,0 1048576 M CHAIN_SPEND_TIME_PENALTY_RATE 0.150000
      1,0 1048576 M CHAIN_PATH_LENGTH_PENALTY_RATE 3.000000
      1,0 1048576 M ========= (evaluate_state) ==========
      1,0 1048576 M (eval) : ball pos (29.787200, -25.446800)
      1,0 1048576 M (eval) initial value (134.787200)
      1,0 1048576 M (eval) bonus for over offside line 24.000400 (158.787600)
      1,0 1048576 M (eval) bonus for over attack line 2.492599 (161.280199)
      1,0 1048576 M (eval) penalty for congestion -0.000004 (161.280195)
      1,0 1048576 M (eval) penalty for path length 0.000000: length=0
      1,0 1048576 M (eval) penalty for spend time -0.000000: time=0
      1,0 1048576 M (eval) chain length penalty 0.000000 (161.280195)
      29.7872 -25.4468 161.28
    */

    std::string::size_type spos = 0;
    std::string::size_type epos = msg.find( '\n', 0 );

    while ( epos != std::string::npos )
    {
        const std::string line_buf = msg.substr( spos, epos - spos );
        //std::cout << "line = [" << line_buf << "]" << std::endl;

        double x, y, eval;
        if ( std::sscanf( line_buf.c_str(), " %lf %lf %lf ",
                          &x, &y, &eval ) == 3 )
        {
            M_evaluation_point->setText( QString( "%1, %2" ).arg( x ).arg( y ) );
            M_evaluation_value->setText( QString::number( eval ) );
            M_actual_evaluation_value = eval;
            return true;
        }

        spos = epos + 1;
        epos = msg.find( '\n', spos );
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EvaluationWindow::upEvaluation()
{
    bool ok = false;
    double value = M_evaluation_value->text().toDouble( &ok );
    if ( ok )
    {
        value += 0.1;
        M_evaluation_value->setText( QString::number( value ) );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EvaluationWindow::downEvaluation()
{
    bool ok = false;
    double value = M_evaluation_value->text().toDouble( &ok );
    if ( ok )
    {
        value -= 0.1;
        M_evaluation_value->setText( QString::number( value ) );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EvaluationWindow::resetEvaluation()
{
    M_evaluation_value->setText( QString::number( M_actual_evaluation_value ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
EvaluationWindow::executeTrain()
{
    double x, y;
    const std::string xy_str = M_evaluation_point->text().toStdString();
    if ( std::sscanf( xy_str.c_str(), " %lf , %lf ", &x, &y ) != 2 )
    {
        return;
    }

    bool ok = false;
    double value = M_evaluation_value->text().toDouble( &ok );
    if ( ok )
    {
        std::cerr << __FILE__ << ": (executeTrain) "
                  << " pos = (" << x << ", " << y << ')'
                  << " new value = " << value << std::endl;
    }
}
