// -*-c++-*-

/*!
  \file action_sequence_option_window.h
  \brief action sequence data selector widget Header File.
*/

/////////////////////////////////////////////////////////////////////

#ifndef SOCCERWINDOW2_ACTION_SEQUENCE_OPTION_WINDOW_H
#define SOCCERWINDOW2_ACTION_SEQUENCE_OPTION_WINDOW_H

#include <QDialog>

class QRadioButton;
class QPushButton;
class QSpinBox;

class ActionSequenceOptionWindow
: public QDialog {

  Q_OBJECT

 public:
  ActionSequenceOptionWindow( QWidget * parent = 0 );

  ~ActionSequenceOptionWindow();

private:
  QRadioButton * radio1;
  QRadioButton * radio2;
  QRadioButton * radio3;
  QRadioButton * radio4;
  QRadioButton * radio5;
  QPushButton * loadbutton;
  QPushButton * savebutton;
  QPushButton * closebutton;
  QPushButton * coutbutton;
  QSpinBox * spinBox;
  int M_optionNum;
  int M_optionMaxNoClick;

public:
  int optionNum();
  void inOptionNum( int );
  int optionMaxNoClick();

public slots:
  void inOptionMaxNoClick( int );

private slots:
  void saveFile();
  void clickRadioButton();
  void loadFile();
  void coutNum();

private:
  void changeRadioButtonCheck();

};

#endif
