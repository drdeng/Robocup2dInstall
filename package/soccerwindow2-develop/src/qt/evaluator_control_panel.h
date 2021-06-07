// -*-c++-*-

/*!
  \file debug_evaluator_chain_control_widget.h
  \brief control widget to debug evaluator and chain actions Header File.
*/

/*
 *Copyright:

 Copyright (C) Hiroki SHIMORA

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

#ifndef SOCCERWINDOW2_DEBUG_EVALUATOR_CHAIN_CONTROL_WIDGET_H
#define SOCCERWINDOW2_DEBUG_EVALUATOR_CHAIN_CONTROL_WIDGET_H

#include <QFrame>

class MainData;

class QMainWindow;
class QPushButton;
class QSlider;
class QTextLine;
class QVBoxLayout;

/*!
  \class EvaluatorControlPanel
  \brief control widget to debug evaluator and chain actions
*/
class EvaluatorControlPanel
    : public QFrame {

    Q_OBJECT

private:

    MainData & M_main_data;

    QSlider * M_evaluation_min_thr_slider;
    QSlider * M_evaluation_max_thr_slider;

    QDialog * M_chain_selector_dialog;

public:

    EvaluatorControlPanel( QWidget * parent,
                           MainData & main_data );
    ~EvaluatorControlPanel();


private slots:
    void executeEvaluator();

    void updateEvaluatorMinThreshold( int value );
    void updateEvaluatorMaxThreshold( int value );
    void clearAllEvaluation();

signals:
    void showSelector();
    void configured();
};

#endif
