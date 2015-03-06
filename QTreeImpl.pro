#-------------------------------------------------
#
# Project created by QtCreator 2015-02-20T10:09:35
#
#-------------------------------------------------

QT       += core gui xml

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTreeImpl
TEMPLATE = app


SOURCES += main.cpp\
        qtreeimplmainwindow.cpp \
    quadtree.cpp \
    osmreader.cpp \
    rtree.cpp \
    rtreewidget.cpp

HEADERS  += qtreeimplmainwindow.h \
    quadtree.h \
    osmreader.h \
    rtree.h \
    point.h \
    rtreewidget.h

FORMS    += qtreeimplmainwindow.ui
