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
	mainWindow.cpp \
	vector.cpp

HEADERS  += \
	matrix.h \
	parser.h \
	mainWindow.h \
	polygon.h \
	vector.h


QMAKE_LFLAGS += -fopenmp
QMAKE_CXXFLAGS += -fopenmp
