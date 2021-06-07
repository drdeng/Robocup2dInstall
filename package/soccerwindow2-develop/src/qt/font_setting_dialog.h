// -*-c++-*-

/*!
  \file font_setting_dialog.h
  \brief font setting dialog for the field canvas Header File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa Akiyama

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

#ifndef SOCCERWINDOW2_QT4_FONT_SETTING_DIALOG_H
#define SOCCERWINDOW2_QT4_FONT_SETTING_DIALOG_H

#include <QDialog>
#include <QFont>
#include <QPushButton>

#include <rcsc/types.h>

#include <boost/function.hpp>

#include <vector>

class FontButton;
class FontSettingDialog;

class FontButton
    : public QPushButton {

    Q_OBJECT

public:

    typedef boost::function< void ( const QFont & ) > Func;

private:
    QString M_name;
    QFont M_old_font;
    Func M_func;

public:

    FontButton( const QString & name,
                const QFont & old_font,
                Func func,
                FontSettingDialog * parent );

    void setNewFont( const QFont & font );

    void revert();
    void updateText();

private slots:

    void fontDialog();

signals:

    void fontChanged();
};


/*-------------------------------------------------------------------*/

/*!
  \class FontSettingDialog
  \brief modal dialog to change font settings
 */
class FontSettingDialog
    : public QDialog {

    Q_OBJECT

private:

    std::vector< FontButton * > M_font_buttons;

public:

    FontSettingDialog( QWidget * parent );

    ~FontSettingDialog();

private:

    void createControls();

    QLayout * createFontButtons();
    QLayout * createAnswerButtons();

    void revert();

private slots:

    void setDefaultFonts();

public slots:

    virtual
    void reject();

signals:

    void fontChanged();
};

#endif
