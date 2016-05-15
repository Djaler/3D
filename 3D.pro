#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T22:17:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3D
TEMPLATE = app


SOURCES += main.cpp \
	mainWindow.cpp

HEADERS  += \
	matrix.h \
	mainWindow.h \
	vector.h \
	object.h \
	polygon.h \
	drawing.h \
    camera.h \
    tardis.h \
    addwidget.h


QMAKE_LFLAGS += -fopenmp
QMAKE_CXXFLAGS += -fopenmp

FORMS +=
