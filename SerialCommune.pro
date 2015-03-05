#-------------------------------------------------
#
# Project created by QtCreator 2011-05-23T15:05:01
# Author: Sandeep "OMEGA KNOT" Manandhar
# Date: 5/27/2011
# Use it as you want
# Author shall not be held responsible for any harm
#-------------------------------------------------

QT       += core gui

TARGET = SerialCommune
TEMPLATE = app
INCLUDEPATH += c:/qextserialport-1.2win-alpha
QMAKE_LIBDIR +=c:/qextserialport-build/build
CONFIG(debug, debug/release):LIBS+= -lqextserialportd
else:LIBS += -lqextserialport
unix: DEFINES = _TTY_POSIX_
win32: DEFINES = _TTY_WIN_


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
