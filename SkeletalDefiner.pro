#-------------------------------------------------
#
# Project created by QtCreator 2012-07-26T20:12:20
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = SkeletalDefiner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    render.cpp

HEADERS  += mainwindow.h \
    render.h

LIBS += -lassimp

FORMS    += mainwindow.ui
