// -*-c++-*-

/*!
  \file log_player_tool_bar.cpp
  \brief log player control tool bar class Source File.
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

#include "log_player_tool_bar.h"

#include "main_data.h"

#include <iostream>
#include <cassert>

class CycleEdit
    : public QLineEdit {

public:

    CycleEdit( QWidget * parent )
        : QLineEdit( tr( "0" ), parent )
      {
          this->setMaximumSize( 28, 20 );
          this->setMinimumSize( 16, 16 );

          QSizePolicy policy( QSizePolicy::Maximum, QSizePolicy::Maximum );
          policy.setHorizontalStretch( 1 );
          policy.setVerticalStretch( 1 );
          this->setSizePolicy( policy );

          QFont font = this->font();
          font.setPointSize( 6 );
          this->setFont( font );
      }

    virtual
    QSize sizeHint() const
      {
          QSize size = QLineEdit::sizeHint();
          if ( size.width() > 32 )
          {
              size.setWidth( 32 );
          }
          if ( size.height() > 24 )
          {
              size.setHeight( 24 );
          }
          return size;
      }

};


class CycleSlider
    : public QSlider {
private:

public:

    CycleSlider( Qt::Orientation orientation,
                 QWidget * parent = 0 )
        : QSlider( orientation, parent )
      { }

protected:
#if 0
    void mousePressEvent( QMouseEvent * event )
      {
          if ( event->button() == Qt::LeftButton )
          {
              double rate = 0;
              if ( this->orientation() == Qt::Horizontal )
              {
                  rate = static_cast< double >( event->pos().x() )
                      / static_cast< double >( this->width() );

              }
              else
              {
                  rate = static_cast< double >( this->height() - event->pos().y() )
                      / static_cast< double >( this->height() );
              }

              int val = this->minimum()
                  + static_cast< int >( rint( ( this->maximum() - this->minimum() ) * rate ) );

              this->setValue( val );
          }

          QSlider::mousePressEvent( event );
      }
#endif
};

/*-------------------------------------------------------------------*/
/*!

*/
LogPlayerToolBar::LogPlayerToolBar( const MainData & main_data,
                                    QWidget * parent )
    : QToolBar( tr( "LogPlayer" ),
                parent ),
      M_main_data( main_data ),
      M_cycle_slider( static_cast< QSlider * >( 0 ) ),
      M_cycle_edit( static_cast< QLineEdit * >( 0 ) )
{
    assert( parent );

    this->setIconSize( QSize( 16, 16 ) );

    //this->setAllowedAreas( Qt::TopToolBarArea
    //                       | Qt::BottomToolBarArea );
    //this->setMovable( false );

    //this->setMaximumSize( 640, 640 );

    connect( this, SIGNAL( orientationChanged( Qt::Orientation ) ),
             this, SLOT( changeOrientation( Qt::Orientation ) ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
LogPlayerToolBar::~LogPlayerToolBar()
{
    //std::cerr << "delete LogPlayerToolBar" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayerToolBar::createCycleSlider()
{
    if ( M_cycle_slider )
    {
        return;
    }

    M_cycle_slider = new CycleSlider( this->orientation(), this );
    connect( this, SIGNAL( orientationChanged( Qt::Orientation ) ),
             M_cycle_slider, SLOT( setOrientation( Qt::Orientation ) ) );
    M_cycle_slider->setStatusTip( tr( "You can select the cycle by this slider." ) );
    M_cycle_slider->setToolTip( tr( "Cycle Slider" ) );
    M_cycle_slider->setRange( 0, 0 );
    M_cycle_slider->setValue( 0 );
    M_cycle_slider->setTickInterval( 1 );
    M_cycle_slider->setMaximumSize( 640, 640 );
    M_cycle_slider->setMinimumWidth( 200 );
    //M_cycle_slider->setMinimumSize( 400, 400 );
    connect( M_cycle_slider, SIGNAL( valueChanged( int ) ),
             this, SIGNAL( indexChanged( int ) ) );

    this->addWidget( M_cycle_slider );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayerToolBar::createCycleEdit()
{
    if ( M_cycle_edit )
    {
        return;
    }

    M_cycle_edit = new CycleEdit( this );
    M_cycle_edit->setStatusTip( tr( "You can select the cycle directly." ) );
    M_cycle_edit->setToolTip( tr( "Cycle Input" ) );
    connect( M_cycle_edit, SIGNAL( returnPressed() ),
             this, SLOT( editCycle() ) );

    this->addWidget( M_cycle_edit );

    // invisible action
//     {
//         QAction * act = new QAction( tr( "Focus Cycle Edit" ), this );
// #ifdef Q_WS_MAC
//         act->setShortcut( Qt::META + Qt::Key_F );
// #else
//         act->setShortcut( Qt::CTRL + Qt::Key_F );
// #endif
//         connect( act, SIGNAL( triggered() ),
//                  M_cycle_edit, SLOT( setFocus() ) );

//         main_win->addAction( act );
//     }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayerToolBar::editCycle()
{
    bool ok = true;
    int cycle = M_cycle_edit->text().toInt( &ok );

    if ( ok
         && cycle >= 0 )
    {
        emit cycleChanged( cycle );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
#if 0
void
LogPlayerToolBar::moveEvent( QMoveEvent * event )
{
    if ( this->orientation() == Qt::Vertical )
    {
        if ( this->width() > this->minimumWidth() )
        {
            this->resize( this->minimumWidth(), 480 );
        }
    }
    else
    {
        if ( this->height() > this->minimumHeight() )
        {
            this->resize( 480, this->minimumHeight() );
        }
    }

    event->accept();
}
#endif

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayerToolBar::changeOrientation( Qt::Orientation orientation )
{
    if ( orientation == Qt::Vertical )
    {
        M_cycle_slider->setMinimumSize( 16, 100 );
        M_cycle_slider->setMaximumSize( 16, 640 );
    }
    else
    {
        M_cycle_slider->setMinimumSize( 200, 16 );
        M_cycle_slider->setMaximumSize( 640, 16 );
    }
#if 0
    std::cerr << "--------------------"
              << "Orientation = " << orientation << '\n'
              << " x, y, width, height    "
              << x() << ' ' << y() << ' '
              << width() << ' ' << height() << '\n'
              << " baseSize = width, height    "
              << baseSize().width() << ' ' << baseSize().height() << '\n'
              << " frameSize = width, height    "
              << frameSize().width() << ' ' << frameSize().height() << '\n'
              << " childrenRect = top, left, width, height    "
              << childrenRect().top() << ' ' << childrenRect().left() << ' '
              << childrenRect().width() << ' ' << childrenRect().height() << '\n'
              << " geometry = x, y, width, height    "
              << geometry().x() << ' ' << geometry().y() << ' '
              << geometry().width() << ' ' << geometry().y() << '\n'
              << " frameGeometry = x, y, width, height    "
              << frameGeometry().x() << ' ' << frameGeometry().y() << ' '
              << frameGeometry().width() << ' ' << frameGeometry().height() << '\n'
              << " normalGeometry = x, y, width, height    "
              << normalGeometry().x() << ' ' << normalGeometry().y() << ' '
              << normalGeometry().width() << ' ' << normalGeometry().height() << '\n'
              << " minimumSize = width, height    "
              << minimumWidth() << ' ' << minimumHeight() << '\n'
              << " maximumSize = width, height    "
              << maximumWidth() << ' ' << maximumHeight() << '\n'
              << " sizeHint = width, height()    "
              << sizeHint().width() << ' ' << sizeHint().height() << '\n'
              << " sizePolicy = horizontal, vertical    "
              << sizePolicy().horizontalPolicy() << ' ' << sizePolicy().verticalPolicy() << '\n'
        ;

    /*
    std::cerr << "------\n"
              << " cycleEdit "
              << " x, y, width, height    "
              << M_cycle_slider->x() << ' ' << M_cycle_slider->y() << ' '
              << M_cycle_slider->width() << ' ' << M_cycle_slider->height() << '\n'
              << " baseSize = width, height    "
              << M_cycle_slider->baseSize().width() << ' '
              << M_cycle_slider->baseSize().height() << '\n'
              << " frameSize = width, height    "
              << M_cycle_slider->frameSize().width() << ' '
              << M_cycle_slider->frameSize().height() << '\n'
              << " childrenRect = top, left, width, height    "
              << M_cycle_slider->childrenRect().top() << ' '
              << M_cycle_slider->childrenRect().left() << ' '
              << M_cycle_slider->childrenRect().width() << ' '
              << childrenRect().height() << '\n'
              << " geometry = x, y, width, height    "
              << M_cycle_slider->geometry().x() << ' '
              << M_cycle_slider->geometry().y() << ' '
              << M_cycle_slider->geometry().width() << ' '
              << geometry().y() << '\n'
              << " frameGeometry = x, y, width, height    "
              << M_cycle_slider->frameGeometry().x() << ' '
              << M_cycle_slider->frameGeometry().y() << ' '
              << M_cycle_slider->frameGeometry().width() << ' '
              << M_cycle_slider->frameGeometry().height() << '\n'
              << " normalGeometry = x, y, width, height    "
              << M_cycle_slider->normalGeometry().x() << ' '
              << M_cycle_slider->normalGeometry().y() << ' '
              << M_cycle_slider->normalGeometry().width() << ' '
              << M_cycle_slider->normalGeometry().height() << '\n'
              << " minimumSize = width, height    "
              << M_cycle_slider->minimumWidth() << ' '
              << M_cycle_slider->minimumHeight() << '\n'
              << " maximumSize = width, height    "
              << M_cycle_slider->maximumWidth() << ' '
              << M_cycle_slider->maximumHeight() << '\n'
              << " sizeHint = width, height()    "
              << M_cycle_slider->sizeHint().width() << ' '
              << M_cycle_slider->sizeHint().height() << '\n'
              << " sizePolicy = horizontal, vertical    "
              << M_cycle_slider->sizePolicy().horizontalPolicy() << ' '
              << M_cycle_slider->sizePolicy().verticalPolicy() << '\n'
        ;
    */
    std::cerr << std::flush;
#endif
}

/*-------------------------------------------------------------------*/
/*!

*/
void
LogPlayerToolBar::updateSlider()
{
    if ( ! this->isVisible() )
    {
        return;
    }

    //std::cerr << "LogPlayerToolBar::update()" << std::endl;

    int size = M_main_data.viewHolder().monitorViewCont().size();

    if ( size == 1 ) size = 0;

    if ( M_cycle_slider->maximum() != size )
    {
        M_cycle_slider->setRange( 0, size );
    }

    int val = M_main_data.viewIndex();

    if ( M_cycle_slider->value() != val )
    {
        M_cycle_slider->setValue( val );
    }
}
