TEMPLATE = app
QT += network
# QT += opengl
TARGET = soccerwindow2
DESTDIR = ../../bin

DEPENDPATH += . ../model
win32 {
  DEPENDPATH += ../../rcsc
}

INCLUDEPATH += . ..  ../.. ../model
win32 {
  INCLUDEPATH +=  ../../boost
}
unix {
  INCLUDEPATH += ${HOME}/local/include
  INCLUDEPATH += /opt/local/include
}
macx {
  INCLUDEPATH += /opt/local/include
}

LIBS += -L../../lib
LIBS += -lsoccerwindow2_model
win32 {
  LIBS += -lrcsc ../../zlib/zlib1.dll -lwsock32
}
unix {
  LIBS += -L${HOME}/local/lib
  LIBS += -lrcsc_agent -lrcsc_time -lrcsc_ann -lrcsc_param -lrcsc_gz -lrcsc_rcg -lrcsc_geom -lz
}

DEFINES += HAVE_LIBRCSC_GZ
win32 {
  DEFINES += HAVE_WINDOWS_H NO_TIMER
#  DEFINES += USE_GLWIDGET
}
unix {
  DEFINES += HAVE_NETINET_IN_H
}
DEFINES += PACKAGE_NAME="\\\"soccerwindow2\\\"" VERSION="\\\"5.0.0\\\""

CONFIG += qt warn_on release
win32 {
  CONFIG += windows
}

#QMAKE_LFLAGS_DEBUG += -static
#QMAKE_LFLAGS_DEBUG += -pg
#QMAKE_CFLAGS_DEBUG += -pg -static -DQABSTRACTSOCKET_DEBUG
QMAKE_CXXFLAGS += -static
#QMAKE_CXXFLAGS_DEBUG += -pg -static -DQABSTRACTSOCKET_DEBUG

# Input
HEADERS += \
	ball_painter.h \
	ball_painter_rcss.h \
	ball_trace_painter.h \
	color_setting_dialog.h \
	debug_log_dir_dialog.h \
	debug_log_painter.h \
	debug_message_window.h \
	debug_painter.h \
	debug_server.h \
	detail_dialog.h \
	dir_selector.h \
	draw_config.h \
	field_canvas.h \
	field_painter.h \
	font_setting_dialog.h \
	image_save_dialog.h \
	launcher_dialog.h \
	log_player.h \
	log_player_tool_bar.h \
	main_window.h \
	monitor_client.h \
	monitor_move_dialog.h \
	mouse_state.h \
	offside_line_painter.h \
	painter_interface.h \
	player_control_painter.h \
	player_painter.h \
	player_painter_rcss.h \
	player_trace_painter.h \
	player_type_dialog.h \
	score_board_painter.h \
	score_board_painter_rcss.h \
	team_graphic_painter.h \
	view_config_dialog.h \
	voronoi_diagram_painter.h

SOURCES += \
	ball_painter.cpp \
	ball_painter_rcss.cpp \
	ball_trace_painter.cpp \
	color_setting_dialog.cpp \
	debug_log_dir_dialog.cpp \
	debug_log_painter.cpp \
	debug_message_window.cpp \
	debug_painter.cpp \
	debug_server.cpp \
	detail_dialog.cpp \
	dir_selector.cpp \
	draw_config.cpp \
	field_canvas.cpp \
	field_painter.cpp \
	font_setting_dialog.cpp \
	image_save_dialog.cpp \
	launcher_dialog.cpp \
	log_player.cpp \
	log_player_tool_bar.cpp \
	main.cpp \
	main_window.cpp \
	monitor_client.cpp \
	monitor_move_dialog.cpp \
	offside_line_painter.cpp \
	player_control_painter.cpp \
	player_painter.cpp \
	player_painter_rcss.cpp \
	player_trace_painter.cpp \
	player_type_dialog.cpp \
	score_board_painter.cpp \
	score_board_painter_rcss.cpp \
	team_graphic_painter.cpp \
	view_config_dialog.cpp \
	voronoi_diagram_painter.cpp

nodist_soccerwindow2_qt4_SOURCES = \
	moc_color_setting_dialog.cpp \
	moc_debug_log_dir_dialog.cpp \
	moc_debug_message_window.cpp \
	moc_debug_server.cpp \
	moc_detail_dialog.cpp \
	moc_dir_selector.cpp \
	moc_field_canvas.cpp \
	moc_font_setting_dialog.cpp \
	moc_image_save_dialog.cpp \
	moc_launcher_dialog.cpp \
	moc_log_player.cpp \
	moc_log_player_tool_bar.cpp \
	moc_main_window.cpp \
	moc_monitor_client.cpp \
	moc_monitor_move_dialog.cpp \
	moc_player_type_dialog.cpp \
	moc_view_config_dialog.cpp

RC_FILE = soccerwindow2.rc
