// -*-c++-*-

/*!
  \file color_setting_dialog.h
  \brief color setting dialog for the field canvas Header File.
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

#ifndef SOCCERWINDOW2_QT4_COLOR_SETTING_DIALOG_H
#define SOCCERWINDOW2_QT4_COLOR_SETTING_DIALOG_H

#include <QDialog>

#include <boost/function.hpp>

#include <vector>

class QListWidget;
class QListWidgetItem;

struct ColorEntry {
    typedef boost::function< void ( const QColor & ) > Func;

    QColor old_color_;
    QColor color_;
    Func func_;

    ColorEntry( const QColor & old_color,
                Func func );

    bool setColor( const QColor & color );
    bool revert();
};

/*-------------------------------------------------------------------*/

/*!
  \class ColorSettingDialog
  \brief modal dialog to change color settings
 */
class ColorSettingDialog
    : public QDialog {

    Q_OBJECT

private:

    QListWidget * M_list_box;

    std::vector< ColorEntry > M_color_entries;

public:

    ColorSettingDialog( QWidget * parent );

    ~ColorSettingDialog();

private:

    void createControls();

    void addEntry( const QString & name,
                   const QColor & old_color,
                   ColorEntry::Func func );

    QLayout * createColorList();
    QLayout * createAnswerButtons();

    void createColorItems();

    void revert();

    void selectEntry( int index );

private slots:

    void setDefaultColors();

    void selectEntry();
    void selectEntry( QListWidgetItem * item );

public slots:

    virtual
    void reject();

signals:

    void colorChanged();
};

#endif
