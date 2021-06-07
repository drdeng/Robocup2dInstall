// -*-c++-*-

/*!
  \file config_dialog.h
  \brief Config Dialog class Header File.
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

#ifndef CONFIG_DIALOG_H
#define CONFIG_DIALOG_H

#include <QDialog>

class QLineEdit;

class ConfigDialog
    : public QDialog {

    Q_OBJECT

private:

    // canvas size control
    QLineEdit * M_view_width_text;
    QLineEdit * M_view_height_text;
    QLineEdit * M_scale_text;

public:

    explicit
    ConfigDialog( QWidget * parent );
    ~ConfigDialog();

private:

    void createWidgets();
    QWidget * createViewSizeControls();
    QWidget * createViewScaleControls();

protected:

    void showEvent( QShowEvent * event );
    void closeEvent( QCloseEvent * event );

private slots:

    void updateAll();

    void applyViewSize();
    void editViewScale( const QString & text );

signals:

    void viewResizeApplied( const QSize & size );
    void configured();
    void shown( bool on );

};

#endif
