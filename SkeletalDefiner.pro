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
    render.cpp \
    pointcloud.cpp

HEADERS  += mainwindow.h \
    render.h \
    pointcloud.h \
    Qt-based-common-libs/globj.h

FORMS    += mainwindow.ui
