
TEMPLATE = app
TARGET = fedit2
DESTDIR = ../bin

win32 {
  DEPENDPATH += ../rcsc
}

INCLUDEPATH += . ..
win32 {
  INCLUDEPATH +=  ../boost
}
unix {
  INCLUDEPATH += ${HOME}/local/include
  INCLUDEPATH += /opt/local/include
}
macx {
  INCLUDEPATH += /opt/local/include
}

win32 {
  LIBS += -L../lib
  LIBS += -lrcsc ../zlib/zlib1.dll -lwsock32
}
unix {
  LIBS += -L${HOME}/local/lib
  LIBS += -lrcsc_agent -lrcsc_time -lrcsc_ann -lrcsc_param -lrcsc_net -lrcsc_gz -lrcsc_rcg -lrcsc_geom -lz
}

DEFINES += HAVE_LIBRCSC_GZ
win32 {
  DEFINES += HAVE_WINDOWS_H NO_TIMER
}
unix {
  DEFINES += HAVE_NETINET_IN_H
}
DEFINES += PACKAGE="\\\"fedit2\\\"" VERSION="\\\"0.0.0\\\""

CONFIG += qt warn_on release
win32 {
  CONFIG += windows
}

QMAKE_CXXFLAGS += -static

# Input
HEADERS += \
	command.h \
	constraint_delegate.h \
	constraint_edit_dialog.h \
	constraint_view.h \
	coordinate_delegate.h \
	edit_canvas.h \
	edit_data.h \
	edit_dialog.h \
	main_window.h \
	mouse_state.h \
	options.h \
	sample_view.h

SOURCES += \
	command.cpp \
	constraint_delegate.cpp \
	constraint_edit_dialog.cpp \
	constraint_view.cpp \
	coordinate_delegate.cpp \
	edit_canvas.cpp \
	edit_data.cpp \
	edit_dialog.cpp \
	main.cpp \
	main_window.cpp \
	options.cpp \
	sample_view.cpp \

nodist_soccerwindow2_qt4_SOURCES = \
	moc_constraint_delegate.cpp \
	moc_constraint_edit_dialog.cpp \
	moc_constraint_view.cpp \
	moc_coordinate_delegate.cpp \
	moc_edit_canvas.cpp \
	moc_edit_dialog.cpp \
	moc_main_window.cpp \
	moc_sample_view.cpp

RC_FILE = fedit2.rc
