// -*-c++-*-

/*!
  \file evaluation_window.h
  \brief field evaluation viewer class Header File.
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

#ifndef SOCCERWINDOW2_QT_EVALUATION_WINDOW_H
#define SOCCERWINDOW2_QT_EVALUATION_WINDOW_H

#include <QMainWindow>

class QLineEdit;
class QTextEdit;

class MainData;

/*
  \brief FieldEvaluator debugger using input point
*/
class EvaluationWindow
    : public QMainWindow {

    Q_OBJECT

private:

    const MainData & M_main_data;

    double M_actual_evaluation_value;

    QTextEdit * M_text;

    QLineEdit * M_evaluation_point;
    QLineEdit * M_evaluation_value;

    //! not used
    EvaluationWindow();
    //! noncopyable
    EvaluationWindow( const EvaluationWindow & );
    //! noncopyable
    EvaluationWindow & operator=( const EvaluationWindow & );

public:

    EvaluationWindow( QWidget * parent,
                      const MainData & main_data );

    ~EvaluationWindow();


private:

    bool parseEvaluatorLog( const std::string & msg );


public slots:

    void evaluatePoint( const QPoint & point );

private slots:

    void upEvaluation();
    void downEvaluation();
    void resetEvaluation();

    void executeTrain();

};

#endif
