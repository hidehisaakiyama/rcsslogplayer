// -*-c++-*-

/*!
  \file config_dialog.cpp
  \brief Config Dialog class Source File.
*/

/*
 *Copyright:

 Copyright (C) The RoboCup Soccer Server Maintenance Group.
 Hidehisa AKIYAMA

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

#include <QtGui>

#include "config_dialog.h"

#include "options.h"
#include "main_data.h"

#include <iostream>
#include <cassert>

/*-------------------------------------------------------------------*/
/*!

*/
ConfigDialog::ConfigDialog( QWidget * parent,
                            const MainData & main_data )
    : QDialog( parent )
    , M_main_data( main_data )
{
    this->setWindowTitle( tr( "Config" ) );

    createWidgets();
}

/*-------------------------------------------------------------------*/
/*!

*/
ConfigDialog::~ConfigDialog()
{
    //std::cerr << "delete ConfigDialog" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::createWidgets()
{
    M_tab_widget = new QTabWidget();
    QVBoxLayout * top_layout = new QVBoxLayout();

    // canvas
    {
        QFrame * frame = new QFrame();
        QVBoxLayout * layout = new QVBoxLayout();
        layout->setSizeConstraint( QLayout::SetFixedSize );
        layout->setMargin( 4 );
        layout->setSpacing( 4 );

        layout->addWidget( createZoomControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createObjectSizeControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createCanvasSizeControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createMiscControls(),
                           0, Qt::AlignLeft );

        frame->setLayout( layout );
        M_tab_widget->addTab( frame, tr( "Canvas" ) );
    }
    // show
    {
        QFrame * frame = new QFrame();
        QVBoxLayout * layout = new QVBoxLayout();
        layout->setSizeConstraint( QLayout::SetFixedSize );
        layout->setMargin( 4 );
        layout->setSpacing( 4 );

        layout->addWidget( createShowControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createPlayerInfoControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createGridStepControls(),
                           0, Qt::AlignLeft );

        frame->setLayout( layout );
        M_tab_widget->addTab( frame, tr( "Show" ) );
    }
    // trace / move
    {
        QFrame * frame = new QFrame();
        QVBoxLayout * layout = new QVBoxLayout();
        layout->setSizeConstraint( QLayout::SetFixedSize );
        layout->setMargin( 4 );
        layout->setSpacing( 4 );

        layout->addWidget( createFocusControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createPlayerSelectionControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createTraceControls(),
                           0, Qt::AlignLeft );
        layout->addWidget( createInertiaMoveControls(),
                           0, Qt::AlignLeft );

        frame->setLayout( layout );
        M_tab_widget->addTab( frame, tr( "Trace" ) );
    }

    top_layout->addWidget( M_tab_widget );
    this->setLayout( top_layout );
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
ConfigDialog::createZoomControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Zoom" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setSizeConstraint( QLayout::SetFixedSize );
    top_layout->setMargin( 1 );
    top_layout->setSpacing( 0 );

    const QFontMetrics metrics = this->fontMetrics();
    const int button_height = metrics.height() + 12;

    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setSpacing( 0 );

        QPushButton * zoomin_button = new QPushButton( tr( "+" ) );
        zoomin_button->setMaximumSize( 48, button_height );
        connect( zoomin_button, SIGNAL( clicked() ),
                 this, SLOT( zoomIn() ) );
        layout->addWidget( zoomin_button );
        //
        QPushButton * zoomout_button = new QPushButton( tr( "-" ) );
        zoomout_button->setMaximumSize( 48, button_height );
        connect( zoomout_button, SIGNAL( clicked() ),
                 this, SLOT( zoomOut() ) );
        layout->addWidget( zoomout_button );
        //
        layout->addSpacing( 1 );
        layout->addStretch( 1 );
        //
        QPushButton * fit_button = new QPushButton( tr( "Fit" ) );
        fit_button->setMaximumSize( 60, button_height );
        connect( fit_button, SIGNAL( clicked() ),
                 this, SLOT( unzoom() ) );
        layout->addWidget( fit_button );

        top_layout->addLayout( layout );
    }

    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setSpacing( 0 );

        layout->addWidget( new QLabel( tr( "Scale:" ) ) );

        layout->addSpacing( 2 );

        M_scale_slider = new QSlider( Qt::Horizontal );
        M_scale_slider->setRange( static_cast< int >( Options::MIN_FIELD_SCALE * 10.0 ),
                                  static_cast< int >( Options::MAX_FIELD_SCALE * 10.0 ) );
        M_scale_slider->setValue( 80 );
        M_scale_slider->setSingleStep( 1 );
        M_scale_slider->setPageStep( 1 ); // set the minimal increment/decrement step
        M_scale_slider->setMinimumSize( 250, 24 );
        connect( M_scale_slider, SIGNAL( sliderMoved( int ) ),
                 this, SLOT( slideFieldScale( int ) ) );
        layout->addWidget( M_scale_slider );

        layout->addSpacing( 2 );

        M_scale_text = new QLineEdit( tr( "0.0" ) );
        M_scale_text->setValidator( new QDoubleValidator( 0.0, 400.0, 3, M_scale_text ) );
        M_scale_text->setMaximumSize( 48, 24 );
        connect( M_scale_text, SIGNAL( textChanged( const QString & ) ),
                 this, SLOT( editFieldScale( const QString & ) ) );
        layout->addWidget( M_scale_text );

        top_layout->addLayout( layout );
    }


    group_box->setLayout( top_layout );
    return group_box;;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
ConfigDialog::createObjectSizeControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Object Size" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setMargin( 1 );
    top_layout->setSpacing( 0 );

//     M_enlarge_cb = new QCheckBox( tr( "Enlarge Objects" ) );
//     M_enlarge_cb->setChecked( true );
//     connect( M_enlarge_cb, SIGNAL( clicked( bool ) ),
//              this, SLOT( clickEnlarge( bool ) ) );
//     top_layout->addWidget( M_enlarge_cb );


    QHBoxLayout * box = new QHBoxLayout();
    //
    box->addWidget( new QLabel( tr( "Ball Size:" ) ) );
    //
    M_ball_size_text = new QLineEdit( tr( "0.35" ) );
    M_ball_size_text->setValidator( new QDoubleValidator( 0.01, 100.0, 3, M_ball_size_text ) );
    M_ball_size_text->setMaximumSize( 48, 24 );
    connect( M_ball_size_text, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( editBallSize( const QString & ) ) );
    box->addWidget( M_ball_size_text );
    //
    box->addWidget( new QLabel( tr( " Player Size:" ) ) );
    //
    M_player_size_text = new QLineEdit( tr( "0.0" ) );
    M_player_size_text->setValidator( new QDoubleValidator( 0.0, 100.0, 3, M_ball_size_text ) );
    M_player_size_text->setMaximumSize( 48, 24 );
    connect( M_player_size_text, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( editPlayerSize( const QString & ) ) );
    box->addWidget( M_player_size_text );
    //
    top_layout->addLayout( box );

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
ConfigDialog::createCanvasSizeControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Canvas Size" ) );

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setMargin( 1 );
    layout->setSpacing( 0 );

    layout->addWidget( new QLabel( tr( " Width:" ) ) );

    M_canvas_width_text = new QLineEdit( tr( "640" ) );
    M_canvas_width_text->setValidator( new QIntValidator( 100, 3000, M_canvas_width_text ) );
    M_canvas_width_text->setMaximumSize( 48, 24 );
    layout->addWidget( M_canvas_width_text );

    layout->addWidget( new QLabel( tr( " Height:" ) ) );

    M_canvas_height_text = new QLineEdit( tr( "480" ) );
    M_canvas_height_text->setValidator( new QIntValidator( 100, 3000, M_canvas_height_text ) );
    M_canvas_height_text->setMaximumSize( 48, 24 );
    layout->addWidget( M_canvas_height_text );

    layout->addSpacing( 12 );

    QPushButton * apply_canvas_size_btn = new QPushButton( tr( "apply" ) );
    apply_canvas_size_btn->setMaximumSize( 60, this->fontMetrics().height() + 12 );
    connect( apply_canvas_size_btn, SIGNAL( clicked() ),
             this, SLOT( applyCanvasSize() ) );
    layout->addWidget( apply_canvas_size_btn );

    group_box->setLayout( layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
ConfigDialog::createPlayerInfoControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Player Info" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setMargin( 1 );
    top_layout->setSpacing( 0 );

    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setMargin( 0 );
        layout->setSpacing( 0 );

        M_player_number_cb = new QCheckBox( tr( "Unum" ) );
        M_player_number_cb->setChecked( Options::instance().showPlayerNumber() );
        connect( M_player_number_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowPlayerNumber( bool ) ) );
        layout->addWidget( M_player_number_cb );
        //
        M_player_type_cb = new QCheckBox( tr( "Type" ) );
        M_player_type_cb->setChecked( Options::instance().showPlayerType() );
        connect( M_player_type_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowPlayerType( bool ) ) );
        layout->addWidget( M_player_type_cb );
        //
        M_stamina_cb = new QCheckBox( tr( "Stamina" ) );
        M_stamina_cb->setChecked( Options::instance().showStamina() );
        connect( M_stamina_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowStamina( bool ) ) );
        layout->addWidget( M_stamina_cb );

        top_layout->addLayout( layout );
    }
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setMargin( 0 );
        layout->setSpacing( 0 );

        //
        M_view_area_cb = new QCheckBox( tr( "View Area" ) );
        M_view_area_cb->setChecked( Options::instance().showViewArea() );
        connect( M_view_area_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowViewArea( bool ) ) );
        layout->addWidget( M_view_area_cb );

        top_layout->addLayout( layout );
    }
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setMargin( 0 );
        layout->setSpacing( 0 );

        //
        M_catch_area_cb = new QCheckBox( tr( "Catch Area" ) );
        M_catch_area_cb->setChecked( Options::instance().showCatchArea() );
        connect( M_catch_area_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowCatchArea( bool ) ) );
        layout->addWidget( M_catch_area_cb );
        //
        M_tackle_area_cb = new QCheckBox( tr( "Tackle Area" ) );
        M_tackle_area_cb->setChecked( Options::instance().showTackleArea() );
        connect( M_tackle_area_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowTackleArea( bool ) ) );
        layout->addWidget( M_tackle_area_cb );
        //
        M_kick_accel_area_cb = new QCheckBox( tr( "Kick Accel Area" ) );
        M_kick_accel_area_cb->setChecked( Options::instance().showKickAccelArea() );
        connect( M_kick_accel_area_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowKickAccelArea( bool ) ) );
        layout->addWidget( M_kick_accel_area_cb );

        top_layout->addLayout( layout );
    }
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setMargin( 0 );
        layout->setSpacing( 0 );

        //
        M_pointto_cb = new QCheckBox( tr( "Pointto" ) );
        M_pointto_cb->setChecked( Options::instance().showPointto() );
        connect( M_pointto_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowPointto( bool ) ) );
        layout->addWidget( M_pointto_cb );

        top_layout->addLayout( layout );
    }

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
ConfigDialog::createShowControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Show" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setMargin( 1 );
    top_layout->setSpacing( 0 );

    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setMargin( 0 );
        layout->setSpacing( 0 );

        M_show_score_board_cb = new QCheckBox( tr( "Score Board" ) );
        M_show_score_board_cb->setChecked( Options::instance().showScoreBoard() );
        connect( M_show_score_board_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowScoreBoard( bool ) ) );
        layout->addWidget( M_show_score_board_cb );
        //
        M_show_keepaway_area_cb = new QCheckBox( tr( "Keepaway Area" ) );
        M_show_keepaway_area_cb->setChecked( Options::instance().showKeepawayArea() );
        connect( M_show_keepaway_area_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowKeepawayArea( bool ) ) );
        layout->addWidget( M_show_keepaway_area_cb );
        //
        M_show_team_graphic_cb = new QCheckBox( tr( "Team Graphic" ) );
        M_show_team_graphic_cb->setChecked( Options::instance().showTeamGraphic() );
        connect( M_show_team_graphic_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowTeamGraphic( bool ) ) );
        layout->addWidget( M_show_team_graphic_cb );
        //
        M_show_flag_cb = new QCheckBox( tr( "Flag" ) );
        M_show_flag_cb->setChecked( Options::instance().showFlag() );
        connect( M_show_flag_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowFlag( bool ) ) );
        layout->addWidget( M_show_flag_cb );

        top_layout->addLayout( layout );
    }
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setMargin( 0 );
        layout->setSpacing( 0 );

        M_show_ball_cb = new QCheckBox( tr( "Ball" ) );
        M_show_ball_cb->setChecked( Options::instance().showBall() );
        connect( M_show_ball_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowBall( bool ) ) );
        layout->addWidget( M_show_ball_cb );
        //
        M_show_player_cb = new QCheckBox( tr( "Player" ) );
        M_show_player_cb->setChecked( Options::instance().showPlayer() );
        connect( M_show_player_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowPlayer( bool ) ) );
        layout->addWidget( M_show_player_cb );
        //
        M_show_offside_line_cb = new QCheckBox( tr( "Offside Line" ) );
        M_show_offside_line_cb->setChecked( Options::instance().showOffsideLine() );
        connect( M_show_offside_line_cb, SIGNAL( clicked( bool ) ),
                 this, SLOT( clickShowOffsideLine( bool ) ) );
        layout->addWidget( M_show_offside_line_cb );

        top_layout->addLayout( layout );
    }

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
ConfigDialog::createFocusControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Focus" ) );

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setMargin( 1 );
    layout->setSpacing( 0 );

    {
        M_focus_ball_rb = new QRadioButton( tr( "Ball" ) );
        connect( M_focus_ball_rb, SIGNAL( clicked() ),
                 this, SLOT( clickFocusBall() ) );
        layout->addWidget( M_focus_ball_rb );
    }
    {
        M_focus_player_rb = new QRadioButton( tr( "Selected Player" ) );
        connect( M_focus_player_rb, SIGNAL( clicked() ),
                 this, SLOT( clickFocusPlayer() ) );
        layout->addWidget( M_focus_player_rb );
    }
    {
        M_focus_fix_rb = new QRadioButton( tr( "Fix" ) );
        connect( M_focus_fix_rb, SIGNAL( clicked() ),
                 this, SLOT( setFocusFix() ) );
        layout->addWidget( M_focus_fix_rb );
    }
    M_focus_fix_rb->setChecked( true );

    group_box->setLayout( layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
ConfigDialog::createPlayerSelectionControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Player Selection" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setMargin( 1 );
    top_layout->setSpacing( 0 );

    // selection type
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setMargin( 0 );
        layout->setSpacing( 0 );
        {
            M_select_all_rb = new QRadioButton( tr( "Auto" ) );
            connect( M_select_all_rb, SIGNAL( clicked() ),
                     this, SLOT( clickSelectAutoAll() ) );
            layout->addWidget( M_select_all_rb );
        }
        {
            M_select_left_rb = new QRadioButton( tr( "Auto left" ) );
            connect( M_select_left_rb, SIGNAL( clicked() ),
                     this, SLOT( clickSelectAutoLeft() ) );
            layout->addWidget( M_select_left_rb );
        }
        {
            M_select_right_rb = new QRadioButton( tr( "Auto right" ) );
            connect( M_select_right_rb, SIGNAL( clicked() ),
                     this, SLOT( clickSelectAutoRight() ) );
            layout->addWidget( M_select_right_rb );
        }
        {
            M_select_fix_rb = new QRadioButton( tr( "Fix" ) );
            connect( M_select_all_rb, SIGNAL( clicked() ),
                     this, SLOT( clickSelectFix() ) );
            layout->addWidget( M_select_fix_rb );
        }
        {
            M_unselect_rb = new QRadioButton( tr( "Unselect" ) );
            connect( M_unselect_rb, SIGNAL( clicked() ),
                     this, SLOT( setUnselect() ) );
            layout->addWidget( M_unselect_rb );
        }
        M_unselect_rb->setChecked( true );

        top_layout->addLayout( layout );
    }

    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->setMargin( 0 );
        layout->setSpacing( 0 );
        //layout->addSpacing( 16 );

        M_player_choice = new QComboBox();
        M_player_choice->addItem( tr ( "No Selection" ) );
        for ( int i = 1; i <= 11; ++i )
        {
            M_player_choice->addItem( QString( "Left number: %1" ).arg( i ) );
        }
        for ( int i = 1; i <= 11; ++i )
        {
            M_player_choice->addItem( QString( "Right number: %1" ).arg( i ) );
        }
        M_player_choice->setCurrentIndex( 0 );
        int id_width = this->fontMetrics().width( tr( "Right Number: 11" ) );
        M_player_choice->setMaximumWidth( id_width + 40 );
        connect( M_player_choice, SIGNAL( currentIndexChanged( int ) ),
                 this, SLOT( choicePlayer( int ) ) );
        layout->addWidget( M_player_choice, 0, Qt::AlignLeft );

        layout->addSpacing( 16 );

        top_layout->addLayout( layout );
    }

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
ConfigDialog::createMiscControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Misc" ) );
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setMargin( 1 );
    top_layout->setSpacing( 0 );

    //
    M_anti_aliasing_cb = new QCheckBox( tr( "Anti Aliasing" ) );
    M_anti_aliasing_cb->setChecked( Options::instance().antiAliasing() );
    connect( M_anti_aliasing_cb, SIGNAL( toggled( bool ) ),
             this, SLOT( clickAntiAliasing( bool ) ) );
    top_layout->addWidget( M_anti_aliasing_cb );

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
ConfigDialog::createGridStepControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Grid Lines" ) );
    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setMargin( 1 );
    top_layout->setSpacing( 0 );

    M_show_grid_coord_cb = new QCheckBox( tr( "Grid Coordinate" ) );
    M_show_grid_coord_cb->setChecked( Options::instance().showGridCoord() );
    connect( M_show_grid_coord_cb, SIGNAL( toggled( bool ) ),
             this, SLOT( clickShowGridCoord( bool ) ) );
    top_layout->addWidget( M_show_grid_coord_cb );

    //
    QHBoxLayout * layout = new QHBoxLayout();
    //layout->setMargin( 0 );
    //layout->setSpacing( 0 );
    //
    layout->addWidget( new QLabel( tr( "Step:" ) ) );
    //
    M_grid_step_slider = new QSlider( Qt::Horizontal );
    M_grid_step_slider->setRange( 0, 1000 );
    M_grid_step_slider->setValue( static_cast< int >( rint( Options::instance().gridStep() * 10.0 ) ) );
    M_grid_step_slider->setSingleStep( 1 );
    M_grid_step_slider->setPageStep( 1 );
    M_grid_step_slider->setMinimumSize( 260, 24 );
    connect( M_grid_step_slider, SIGNAL( sliderMoved( int ) ),
             this, SLOT( slideGridStep( int ) ) );
    layout->addWidget( M_grid_step_slider );

    //
    M_grid_step_text = new QLineEdit( tr( " 0.0" )  );
    M_grid_step_text->setValidator( new QDoubleValidator( 0.0, 100.0, 3, M_grid_step_text ) );
    M_grid_step_text->setMaximumSize( 48, 24 );
    M_grid_step_text->setText( QString::number( rint( Options::instance().gridStep() / 0.001 ) * 0.001 ) );
    connect( M_grid_step_text, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( editGridStep( const QString & ) ) );
    layout->addWidget( M_grid_step_text );

    top_layout->addLayout( layout );
    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
ConfigDialog::createTraceControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Trace" ) );

    QVBoxLayout * top_layout = new QVBoxLayout();
    top_layout->setMargin( 0 );
    top_layout->setSpacing( 0 );

    {
        QGroupBox * ball_box = new QGroupBox( tr( "Ball" ) );

        QHBoxLayout * layout = new QHBoxLayout();
        layout->setSizeConstraint( QLayout::SetFixedSize );
        layout->setMargin( 0 );
        layout->setSpacing( 0 );

        M_ball_trace_start = new QLineEdit( tr( "0" ) );
        M_ball_trace_start->setValidator( new QIntValidator( 0, 200000, M_ball_trace_start ) );
        M_ball_trace_start->setMaximumSize( 36, 24 );
        connect( M_ball_trace_start, SIGNAL( textEdited( const QString & ) ),
                 this, SLOT( editBallTraceStart( const QString & ) ) );
        layout->addWidget( M_ball_trace_start, 0, Qt::AlignVCenter );

        layout->addWidget( new QLabel( tr( "-" ) ), 0, Qt::AlignVCenter );

        M_ball_trace_end = new QLineEdit( tr( "0" ) );
        M_ball_trace_end->setValidator( new QIntValidator( 0, 200000, M_ball_trace_end ) );
        M_ball_trace_end->setMaximumSize( 36, 24 );
        connect( M_ball_trace_end, SIGNAL( textEdited( const QString & ) ),
                 this, SLOT( editBallTraceEnd( const QString & ) ) );
        layout->addWidget( M_ball_trace_end, 0, Qt::AlignVCenter );

        QPushButton * all_btn = new QPushButton( tr( "All" ) );
        all_btn->setMaximumSize( 48, this->fontMetrics().height() + 12 );
        all_btn->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
        connect( all_btn, SIGNAL( clicked() ),
                 this, SLOT( clickBallTraceAll() ) );
        layout->addWidget( all_btn, 0, Qt::AlignVCenter );

        ball_box->setLayout( layout );
        //first_layout->addWidget( ball_box );
        top_layout->addWidget( ball_box );
    }

    top_layout->addSpacing( 2 );
    {
        QGroupBox * player_box = new QGroupBox( tr( "Player" ) );

        QHBoxLayout * layout= new QHBoxLayout();
        layout->setSizeConstraint( QLayout::SetFixedSize );
        layout->setMargin( 0 );
        layout->setSpacing( 0 );

        M_player_trace_start = new QLineEdit( tr( "0" ) );
        M_player_trace_start->setValidator( new QIntValidator( 0, 200000, M_player_trace_start ) );
        M_player_trace_start->setMaximumSize( 36, 24 );
        connect( M_player_trace_start, SIGNAL( textEdited( const QString & ) ),
                 this, SLOT( editPlayerTraceStart( const QString & ) ) );
        layout->addWidget( M_player_trace_start, 0, Qt::AlignVCenter );

        layout->addWidget( new QLabel( tr( "-" ) ), 0, Qt::AlignVCenter );

        M_player_trace_end = new QLineEdit( tr( "0" ) );
        M_player_trace_end->setValidator( new QIntValidator( 0, 200000, M_player_trace_end ) );
        M_player_trace_end->setMaximumSize( 36, 24 );
        connect( M_ball_trace_end, SIGNAL( textEdited( const QString & ) ),
                 this, SLOT( editBallTraceEnd( const QString & ) ) );
        layout->addWidget( M_player_trace_end, 0, Qt::AlignVCenter );

        QPushButton * all_btn = new QPushButton( tr( "All" ) );
        all_btn->setMaximumSize( 48, this->fontMetrics().height() + 12 );
        all_btn->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
        connect( all_btn, SIGNAL( clicked() ),
                 this, SLOT( clickPlayerTraceAll() ) );
        layout->addWidget( all_btn, 0, Qt::AlignVCenter );

        player_box->setLayout( layout );
        //first_layout->addWidget( player_box );
        top_layout->addWidget( player_box );
    }

    top_layout->addSpacing( 2 );

    {
        QPushButton * line_point_btn = new QPushButton( tr( "Line/Point" ) );
        line_point_btn->setMaximumSize( 80, this->fontMetrics().height() + 12 );
        connect( line_point_btn, SIGNAL( clicked() ),
                 this, SLOT( clickLinePointButton() ) );

        top_layout->addWidget( line_point_btn, 0, Qt::AlignLeft );
    }

    group_box->setLayout( top_layout );
    return group_box;;
}

/*-------------------------------------------------------------------*/
/*!

*/
QWidget *
ConfigDialog::createInertiaMoveControls()
{
    QGroupBox * group_box = new QGroupBox( tr( "Ball Velocity" ) );

    QHBoxLayout * top_layout = new QHBoxLayout();
    top_layout->setMargin( 1 );
    top_layout->setSpacing( 0 );

    top_layout->addWidget( new QLabel( tr( "Cycle:" ) ) );

    top_layout->addSpacing( 2 );

    M_ball_vel_cycle = new QSpinBox();
    //M_ball_vel_cycle->setMinimumSize( 60, 24 );
    M_ball_vel_cycle->setValue( 0 );
    M_ball_vel_cycle->setRange( 0, 50 );
    connect( M_ball_vel_cycle, SIGNAL( valueChanged( int ) ),
             this, SLOT( changeBallVelCycle( int ) ) );
    top_layout->addWidget( M_ball_vel_cycle );

    group_box->setLayout( top_layout );
    return group_box;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::showEvent( QShowEvent * event )
{
    updateAll();
    event->accept();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::updateAll()
{
    const Options & opt = Options::instance();

//    M_enlarge_cb->setChecked( opt.enlarge() );
    M_ball_size_text->setText( QString::number( opt.ballSize() ) );
    M_player_size_text->setText( QString::number( opt.playerSize() ) );

    updateFieldScale();

    M_canvas_width_text->setText( QString::number( opt.canvasWidth() ) );
    M_canvas_height_text->setText( QString::number( opt.canvasHeight() ) );

    M_anti_aliasing_cb->setChecked( opt.antiAliasing() );

    M_player_number_cb->setChecked( opt.showPlayerNumber() );
    M_player_type_cb->setChecked( opt.showPlayerType() );
    M_stamina_cb->setChecked( opt.showStamina() );
    M_view_area_cb->setChecked( opt.showViewArea() );
    M_catch_area_cb->setChecked( opt.showCatchArea() );
    M_tackle_area_cb->setChecked( opt.showTackleArea() );
    M_kick_accel_area_cb->setChecked( opt.showKickAccelArea() );
    M_pointto_cb->setChecked( opt.showPointto() );

    M_show_score_board_cb->setChecked( opt.showScoreBoard() );
    M_show_keepaway_area_cb->setChecked( opt.showKeepawayArea() );
    M_show_team_graphic_cb->setChecked( opt.showTeamGraphic() );
    M_show_ball_cb->setChecked( opt.showBall() );
    M_show_player_cb->setChecked( opt.showPlayer() );
    M_show_flag_cb->setChecked( opt.showFlag() );
    M_show_offside_line_cb->setChecked( opt.showOffsideLine() );

    switch( opt.focusType() ) {
    case Options::FOCUS_BALL:
        M_focus_ball_rb->setChecked( true );
        break;
    case Options::FOCUS_PLAYER:
        M_focus_player_rb->setChecked( true );
        break;
    case Options::FOCUS_POINT:
        M_focus_fix_rb->setChecked( true );
        break;
    default:
        M_focus_fix_rb->setChecked( true );
        break;
    }

    switch ( opt.playerSelectType() ) {
    case Options::SELECT_AUTO_ALL:
        M_select_all_rb->setChecked( true );
        break;
    case Options::SELECT_AUTO_LEFT:
        M_select_left_rb->setChecked( true );
        break;
    case Options::SELECT_AUTO_RIGHT:
        M_select_right_rb->setChecked( true );
        break;
    case Options::SELECT_UNSELECT:
        M_unselect_rb->setChecked( true );
        break;
    default:
        M_unselect_rb->setChecked( true );
        break;
    }

    M_ball_trace_start->setText( QString::number( opt.ballTraceStart() ) );
    M_ball_trace_end->setText( QString::number( opt.ballTraceEnd() ) );

    M_player_trace_start->setText( QString::number( opt.playerTraceStart() ) );
    M_player_trace_end->setText( QString::number( opt.playerTraceEnd() ) );

    M_ball_vel_cycle->setValue( opt.ballVelCycle() );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::updateFieldScale()
{
    int iscale = static_cast< int >( rint( Options::instance().fieldScale() * 10.0 ) );
    if ( M_scale_slider->value() != iscale )
    {
        M_scale_slider->setValue( iscale );
    }

    char buf[16];
    std::snprintf( buf, 16, "%.2f", Options::instance().fieldScale() );
    M_scale_text->setText( QString::fromAscii( buf ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::zoomIn()
{
    Options::instance().zoomIn();

    updateFieldScale();

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::zoomOut()
{
    Options::instance().zoomOut();

    updateFieldScale();

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::unzoom()
{
    Options::instance().unzoom();
    updateFieldScale();
    M_focus_fix_rb->setChecked( true );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickEnlarge( bool checked )
{
    if ( Options::instance().enlarge() != checked )
    {
        Options::instance().toggleEnlarge();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleEnlarge()
{
    Options::instance().toggleEnlarge();
    M_enlarge_cb->setChecked( Options::instance().enlarge() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::editBallSize( const QString & text )
{
    bool ok = true;
    double value = text.toDouble( &ok );

    if ( ok && value > 0.0 )
    {
        Options::instance().setBallSize( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::editPlayerSize( const QString & text )
{
    bool ok = true;
    double value = text.toDouble( &ok );

    if ( ok )
    {
        Options::instance().setPlayerSize( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::slideFieldScale( int value )
{
    double scale = value * 0.1;

    if ( std::fabs( scale - Options::instance().fieldScale() ) >= 0.01 )
    {
        M_scale_text->setText( QString::number( scale ) );
        Options::instance().setFieldScale( scale );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::editFieldScale( const QString & text )
{
    bool ok = true;
    double value = text.toDouble( &ok );

    if ( ok
         && std::fabs( value - Options::instance().fieldScale() ) >= 0.01 )
    {
        int ivalue = static_cast< int >( rint( value * 10.0 ) );
        M_scale_slider->setValue( ivalue );
        Options::instance().setFieldScale( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::applyCanvasSize()
{
    bool ok_w = true;
    bool ok_h = true;
    int width = M_canvas_width_text->text().toInt( & ok_w );
    int height = M_canvas_height_text->text().toInt( & ok_h );

    if ( ok_w
         && ok_h
         && width > 0
         && height > 0 )
    {
        emit canvasResized( QSize( width, height ) );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickAntiAliasing( bool checked )
{
    if ( Options::instance().antiAliasing() != checked )
    {
        Options::instance().toggleAntiAliasing();

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowPlayerNumber( bool checked )
{
    if ( Options::instance().showPlayerNumber() != checked )
    {
        Options::instance().toggleShowPlayerNumber();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowPlayerNumber()
{
    Options::instance().toggleShowPlayerNumber();
    M_player_number_cb->setChecked( Options::instance().showPlayerNumber() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowPlayerType( bool checked )
{
    if ( Options::instance().showPlayerType() != checked )
    {
        Options::instance().toggleShowPlayerType();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowPlayerType()
{
    Options::instance().toggleShowPlayerType();
    M_player_type_cb->setChecked( Options::instance().showPlayerType() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowStamina( bool checked )
{
    if ( Options::instance().showStamina() != checked )
    {
        Options::instance().toggleShowStamina();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowStamina()
{
    Options::instance().toggleShowStamina();
    M_stamina_cb->setChecked( Options::instance().showStamina() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowViewArea( bool checked )
{
    if ( Options::instance().showViewArea() != checked )
    {
        Options::instance().toggleShowViewArea();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowViewArea()
{
    Options::instance().toggleShowViewArea();
    M_view_area_cb->setChecked( Options::instance().showViewArea() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowCatchArea( bool checked )
{
    if ( Options::instance().showCatchArea() != checked )
    {
        Options::instance().toggleShowCatchArea();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowCatchArea()
{
    Options::instance().toggleShowCatchArea();
    M_catch_area_cb->setChecked( Options::instance().showCatchArea() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowTackleArea( bool checked )
{
    if ( Options::instance().showTackleArea() != checked )
    {
        Options::instance().toggleShowTackleArea();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowTackleArea()
{
    Options::instance().toggleShowTackleArea();
    M_tackle_area_cb->setChecked( Options::instance().showTackleArea() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowKickAccelArea( bool checked )
{
    if ( Options::instance().showKickAccelArea() != checked )
    {
        Options::instance().toggleShowKickAccelArea();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowKickAccelArea()
{
    Options::instance().toggleShowKickAccelArea();
    M_kick_accel_area_cb->setChecked( Options::instance().showKickAccelArea() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowPointto( bool checked )
{
    if ( Options::instance().showPointto() != checked )
    {
        Options::instance().toggleShowPointto();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowPointto()
{
    Options::instance().toggleShowPointto();
    M_pointto_cb->setChecked( Options::instance().showPointto() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowScoreBoard( bool checked )
{
    if ( Options::instance().showScoreBoard() != checked )
    {
        Options::instance().toggleShowScoreBoard();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowScoreBoard()
{
    Options::instance().toggleShowScoreBoard();
    M_show_score_board_cb->setChecked( Options::instance().showScoreBoard() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowTeamGraphic( bool checked )
{
    if ( Options::instance().showTeamGraphic() != checked )
    {
        Options::instance().toggleShowTeamGraphic();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowTeamGraphic()
{
    Options::instance().toggleShowTeamGraphic();
    M_show_team_graphic_cb->setChecked( Options::instance().showTeamGraphic() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowBall( bool checked )
{
    if ( Options::instance().showBall() != checked )
    {
        Options::instance().toggleShowBall();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowBall()
{
    Options::instance().toggleShowBall();
    M_show_ball_cb->setChecked( Options::instance().showBall() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowPlayer( bool checked )
{
    if ( Options::instance().showPlayer() != checked )
    {
        Options::instance().toggleShowPlayer();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowPlayer()
{
    Options::instance().toggleShowPlayer();
    M_show_player_cb->setChecked( Options::instance().showPlayer() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowFlag( bool checked )
{
    if ( Options::instance().showFlag() != checked )
    {
        Options::instance().toggleShowFlag();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowFlag()
{
    Options::instance().toggleShowFlag();
    M_show_flag_cb->setChecked( Options::instance().showFlag() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowOffsideLine( bool checked )
{
    if ( Options::instance().showOffsideLine() != checked )
    {
        Options::instance().toggleShowOffsideLine();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowOffsideLine()
{
    Options::instance().toggleShowOffsideLine();
    M_show_offside_line_cb->setChecked( Options::instance().showOffsideLine() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowKeepawayArea( bool checked )
{
    if ( Options::instance().showKeepawayArea() != checked )
    {
        Options::instance().toggleShowKeepawayArea();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleShowKeepawayArea()
{
    Options::instance().toggleShowKeepawayArea();
    M_show_keepaway_area_cb->setChecked( Options::instance().showKeepawayArea() );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickShowGridCoord( bool checked )
{
    if ( Options::instance().showGridCoord() != checked )
    {
        Options::instance().toggleShowGridCoord();
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::slideGridStep( int value )
{
    M_grid_step_text->setText( QString::number( value * 0.1 ) );

    Options::instance().setGridStep( value * 0.1 );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::editGridStep( const QString & text )
{
    bool ok = true;
    double value = text.toDouble( & ok );

    if ( ok )
    {
        M_grid_step_slider->setValue( static_cast< int >( rint( value * 10.0 ) ) );
        Options::instance().setGridStep( rint( value / 0.001 ) * 0.001 );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickFocusBall()
{
    if ( Options::instance().focusType() != Options::FOCUS_BALL )
    {
        Options::instance().setFocusType( Options::FOCUS_BALL );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleFocusBall()
{
    if ( Options::instance().focusType() == Options::FOCUS_BALL )
    {
        setFocusFix();
    }
    else
    {
        Options::instance().setFocusType( Options::FOCUS_BALL );
        M_focus_ball_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickFocusPlayer()
{
    if ( Options::instance().focusType() != Options::FOCUS_PLAYER )
    {
        Options::instance().setFocusType( Options::FOCUS_PLAYER );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleFocusPlayer()
{
    if ( Options::instance().focusType() == Options::FOCUS_PLAYER )
    {
        setFocusFix();
    }
    else
    {
        Options::instance().setFocusType( Options::FOCUS_PLAYER );
        M_focus_player_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::setFocusFix()
{
    if ( Options::instance().focusType() != Options::FOCUS_POINT )
    {
        Options::instance().setFocusType( Options::FOCUS_POINT );
        M_focus_fix_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::setFocusPoint( const QPoint & point )
{
    Options::instance().setFocusPoint( point.x(), point.y() );
    M_focus_fix_rb->setChecked( true );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickSelectAutoAll()
{
    if ( Options::instance().playerSelectType() != Options::SELECT_AUTO_ALL )
    {
        Options::instance().setPlayerSelectType( Options::SELECT_AUTO_ALL );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleSelectAutoAll()
{
    if ( Options::instance().playerSelectType() == Options::SELECT_AUTO_ALL )
    {
        setUnselect();
    }
    else
    {
        Options::instance().setPlayerSelectType( Options::SELECT_AUTO_ALL );
        M_select_all_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickSelectAutoLeft()
{
    if ( Options::instance().playerSelectType() != Options::SELECT_AUTO_LEFT )
    {
        Options::instance().setPlayerSelectType( Options::SELECT_AUTO_LEFT );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleSelectAutoLeft()
{
    if ( Options::instance().playerSelectType() == Options::SELECT_AUTO_LEFT )
    {
        setUnselect();
    }
    else
    {
        Options::instance().setPlayerSelectType( Options::SELECT_AUTO_LEFT );
        M_select_left_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickSelectAutoRight()
{
    if ( Options::instance().playerSelectType() != Options::SELECT_AUTO_RIGHT )
    {
        Options::instance().setPlayerSelectType( Options::SELECT_AUTO_RIGHT );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleSelectAutoRight()
{
    if ( Options::instance().playerSelectType() == Options::SELECT_AUTO_RIGHT )
    {
        setUnselect();
    }
    else
    {
        Options::instance().setPlayerSelectType( Options::SELECT_AUTO_RIGHT );
        M_select_right_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickSelectFix()
{
    if ( Options::instance().playerSelectType() != Options::SELECT_FIX )
    {
        Options::instance().setPlayerSelectType( Options::SELECT_AUTO_RIGHT );
        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::toggleSelectFix()
{
    if ( Options::instance().playerSelectType() == Options::SELECT_FIX )
    {
        setUnselect();
    }
    else
    {
        Options::instance().setPlayerSelectType( Options::SELECT_FIX );
        M_select_fix_rb->setChecked( true );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::setUnselect()
{
    Options::instance().unselectPlayer();
    M_unselect_rb->setChecked( true );
    M_player_choice->setCurrentIndex( 0 );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::selectPlayer( int number )
{
    // left side:  number = [1,11]
    // right side:  number = [12,22]

    if ( number <= 0
         || 22 < number )
    {
        setUnselect();
        return;
    }

    Options::instance().setPlayerSelectType( Options::SELECT_FIX );
    if ( number <= 11 )
    {
        Options::instance().setSelectedNumber( rcss::rcg::LEFT, number );
    }
    else
    {
        Options::instance().setSelectedNumber( rcss::rcg::RIGHT, number - 11 );
    }

    M_player_choice->setCurrentIndex( number );
    M_select_fix_rb->setChecked( true );

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::choicePlayer( int number )
{
    if ( number <= 11
         && Options::instance().selectedPlayer( rcss::rcg::LEFT, number ) )
    {
        return;
    }

    if ( number > 11
         && Options::instance().selectedPlayer( rcss::rcg::RIGHT, number - 11 ) )
    {
        return;
    }

    selectPlayer( number );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::selectPlayerWithKey()
{
    QAction * action = qobject_cast< QAction * >( sender() );

    if ( ! action )
    {
        return;
    }

    int key = action->shortcut()[0];

    for ( int i = 1; i < 10; ++i )
    {
        if ( key == Qt::Key_0 + i )
        {
            selectPlayer( i );
            return;
        }
        else if (
#ifdef Q_WS_MAC
                 key == int( Qt::META + Qt::Key_0 + i )
#else
                 key == int( Qt::CTRL + Qt::Key_0 + i )
#endif
                 )
        {
            selectPlayer( i + 11 );
            return;
        }
    }

    if ( key == Qt::Key_0 )
    {
        selectPlayer( 10 );
        return;
    }

    if (
#ifdef Q_WS_MAC
         key == Qt::META + Qt::Key_0
#else
         key == Qt::CTRL + Qt::Key_0
#endif
         )
    {
        selectPlayer( 21 );
        return;
    }

    if ( key == Qt::Key_Minus )
    {
        selectPlayer( 11 );
        return;
    }

    if (
#ifdef Q_WS_MAC
        key == Qt::META + Qt::Key_Minus
#else
        key == Qt::CTRL + Qt::Key_Minus
#endif
        )
    {
        selectPlayer( 22 );
        return;
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::editBallTraceStart( const QString & text )
{
    bool ok = true;
    long value = text.toLong( & ok );

    if ( ok )
    {
        Options::instance().setBallTraceStart( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::editBallTraceEnd( const QString & text )
{
    bool ok = true;
    long value = text.toLong( & ok );

    if ( ok )
    {
        Options::instance().setBallTraceEnd( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickBallTraceAll()
{
    const std::vector< DispPtr > & cont = M_main_data.dispHolder().dispInfoCont();

    if ( ! cont.empty() )
    {
        if ( Options::instance().ballTraceStart() == cont.front()->show_.time_
             && Options::instance().ballTraceEnd() == cont.back()->show_.time_ )
        {
            M_ball_trace_start->setText( QString::number( 0 ) );
            M_ball_trace_end->setText( QString::number( 0 ) );

            Options::instance().setBallTraceStart( 0 );
            Options::instance().setBallTraceEnd( 0 );
        }
        else
        {
            M_ball_trace_start->setText( QString::number( cont.front()->show_.time_ ) );
            M_ball_trace_end->setText( QString::number( cont.back()->show_.time_ ) );

            Options::instance().setBallTraceStart( cont.front()->show_.time_ );
            Options::instance().setBallTraceEnd( cont.back()->show_.time_ );
        }

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::editPlayerTraceStart( const QString & text )
{
    bool ok = true;
    long value = text.toLong( & ok );

    if ( ok )
    {
        Options::instance().setPlayerTraceStart( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::editPlayerTraceEnd( const QString & text )
{
    bool ok = true;
    long value = text.toLong( & ok );

    if ( ok )
    {
        Options::instance().setPlayerTraceEnd( value );

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickPlayerTraceAll()
{
    const std::vector< DispPtr > & cont = M_main_data.dispHolder().dispInfoCont();
    if ( ! cont.empty() )
    {
        if ( Options::instance().playerTraceStart() == cont.front()->show_.time_
             && Options::instance().playerTraceEnd() == cont.back()->show_.time_ )
        {
            M_player_trace_start->setText( QString::number( 0 ) );
            M_player_trace_end->setText( QString::number( 0 ) );

            Options::instance().setPlayerTraceStart( 0 );
            Options::instance().setPlayerTraceEnd( 0 );
        }
        else
        {
            M_player_trace_start->setText( QString::number( cont.front()->show_.time_ ) );
            M_player_trace_end->setText( QString::number( cont.back()->show_.time_ ) );

            Options::instance().setPlayerTraceStart( cont.front()->show_.time_ );
            Options::instance().setPlayerTraceEnd( cont.back()->show_.time_ );
        }

        emit configured();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::clickLinePointButton()
{
    Options::instance().toggleLineTrace();

    emit configured();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ConfigDialog::changeBallVelCycle( int value )
{
    Options::instance().setBallVelCycle( value );

    emit configured();
}
