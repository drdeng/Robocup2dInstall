// -*-c++-*-

/*!
  \file action_sequence_option_window.h
  \brief action sequence data selector widget Source File.
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

#include "action_sequence_option_window.h"

#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>

/*-------------------------------------------------------------------*/
/*!

 */
ActionSequenceOptionWindow::ActionSequenceOptionWindow( QWidget * parent )
    : QDialog( parent )
{
    radio1 = new QRadioButton( tr( "Normal" ) );
    radio2 = new QRadioButton( tr( "1:n" ) );
    radio3 = new QRadioButton( tr( "1:nRandom" ) );
    radio4 = new QRadioButton( tr( "1:nPRandom" ) );
    radio5 = new QRadioButton( tr( "All" ) );

    spinBox = new QSpinBox;
    spinBox->setRange( 1,130 );

    changeRadioButtonCheck();
    connect( radio1, SIGNAL( clicked() ),
            this, SLOT( clickRadioButton() ) );
    connect( radio2, SIGNAL( clicked() ),
             this, SLOT( clickRadioButton() ) );
    connect( radio3, SIGNAL( clicked() ),
             this, SLOT( clickRadioButton() ) );
    connect( radio4, SIGNAL( clicked() ),
             this, SLOT( clickRadioButton() ) );
    connect( radio5, SIGNAL( clicked() ),
             this, SLOT( clickRadioButton() ) );
    connect( spinBox, SIGNAL( valueChanged( int ) ),
             this, SLOT( inOptionMaxNoClick( int ) ) ) ;

    savebutton = new QPushButton( tr( "Save" ) );
    closebutton = new QPushButton( tr( "Close" ) );
    loadbutton = new QPushButton( tr("Load" ) );
    coutbutton = new QPushButton( tr("coutNum") );

    connect( loadbutton, SIGNAL( clicked() ),
             this, SLOT( loadFile() ) );
    connect( closebutton, SIGNAL( clicked() ),
             this, SLOT( close() ) );
    connect( savebutton, SIGNAL( clicked() ),
             this, SLOT( saveFile() ) );
    connect( coutbutton, SIGNAL( clicked() ),
             this, SLOT( coutNum() ) );

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget( radio1 );
    topLayout->addWidget( radio2 );
    topLayout->addWidget( radio3 );
    topLayout->addWidget( radio4 );
    topLayout->addWidget( radio5 );
    topLayout->addWidget( spinBox );

    QHBoxLayout * bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget( loadbutton );
    bottomLayout->addWidget( savebutton );
    bottomLayout->addWidget( closebutton );
    bottomLayout->addWidget( coutbutton );

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout( topLayout );
    mainLayout->addLayout( bottomLayout );
    this->setLayout( mainLayout );

    this->setWindowTitle( tr( "ActionSequenceOptionWindow" ) );
    this->setFixedHeight( sizeHint().height() );

}

/*-------------------------------------------------------------------*/
/*!

 */
ActionSequenceOptionWindow::~ActionSequenceOptionWindow()
{

}


/*-------------------------------------------------------------------*/
/*!

 */
void
ActionSequenceOptionWindow::saveFile()
{
    std::ofstream wfile;
    wfile.open("ASOW.txt", std::ios::out);

    wfile << M_optionNum << std::endl;

    wfile.close();

    std::cout << "writing" << std::endl;
}


/*-------------------------------------------------------------------*/
/*!

 */
void
ActionSequenceOptionWindow::clickRadioButton()
{
    QRadioButton * radio = qobject_cast< QRadioButton * >( sender() );
    if ( radio )
    {
        if ( QString::compare( radio->text(), "Normal") == 0 )
        {
            M_optionNum = 0;
            M_optionMaxNoClick = 1;
            spinBox->setReadOnly( true );
        }
        else if ( QString::compare( radio->text(), "1:n" ) == 0 )
        {
            M_optionNum = 1;
            M_optionMaxNoClick = spinBox->value();
            spinBox->setReadOnly( false );
        }
        else if ( QString::compare( radio->text(), "1:nRandom" ) == 0 )
        {
            M_optionNum = 2;
            M_optionMaxNoClick = spinBox->value();
            spinBox->setReadOnly( false );
        }
        else if ( QString::compare( radio->text(), "1:nPRandom" ) == 0 )
        {
            M_optionNum = 3;
            M_optionMaxNoClick = spinBox->value();
            spinBox->setReadOnly( false );
        }
        else if( QString::compare( radio->text(), "All" ) == 0 )
        {
            M_optionNum = 4;
            M_optionMaxNoClick = spinBox->value();
            spinBox->setReadOnly( false );
        }
        else
        {
            M_optionNum = 0;
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
int
ActionSequenceOptionWindow::optionNum()
{
    return M_optionNum;
}


/*-------------------------------------------------------------------*/
/*!

 */
void
ActionSequenceOptionWindow::inOptionNum( int tmpnum )
{
    M_optionNum = tmpnum;
}


/*-------------------------------------------------------------------*/
/*!

 */
int
ActionSequenceOptionWindow::optionMaxNoClick()
{
    return M_optionMaxNoClick;
}


/*-------------------------------------------------------------------*/
/*!

 */
void
ActionSequenceOptionWindow::inOptionMaxNoClick( int tmpnum )
{
    M_optionMaxNoClick = tmpnum;
    spinBox->setValue( tmpnum );
}


/*-------------------------------------------------------------------*/
/*!

 */
void
ActionSequenceOptionWindow::loadFile()
{
    std::ifstream ifs;
    ifs.open("ASOW.txt", std::ios::in);

    if ( ! ifs.is_open() )
    {
        std::cout << "not File" << std::endl;
    }
    else
    {
        ifs >> M_optionNum;
        std::cout << "file Loaded" << std::endl;
        changeRadioButtonCheck();
    }
    ifs.close();
}


/*-------------------------------------------------------------------*/
/*!

 */
void
ActionSequenceOptionWindow::coutNum()
{
    std::cout << M_optionNum << std::endl;
    std::cout << M_optionMaxNoClick << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
ActionSequenceOptionWindow::changeRadioButtonCheck()
{
    if ( M_optionNum == 0 )
    {
        radio1->setChecked( true );
        spinBox->setReadOnly( true );
    }
    else if ( M_optionNum == 1 )
    {
        radio2->setChecked( true );
        spinBox->setReadOnly( false );
    }
    else if( M_optionNum == 2 )
    {
        radio3->setChecked( true );
        spinBox->setReadOnly( false );
    }
    else if ( M_optionNum == 3 )
    {
        radio4->setChecked( true );
        spinBox->setReadOnly( false );
    }
    else if ( M_optionNum == 4 )
    {
        radio5->setChecked( true );
        spinBox->setReadOnly( false );
    }
    else
    {
        std::cout << "empty" << std::endl;
        M_optionNum=0;
        radio1->setChecked( true );
    }
}
