#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T22:17:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3D
TEMPLATE = app


SOURCES += main.cpp\
		widget.cpp \
	model.cpp

HEADERS  += widget.h \
	model.h \
    vertex.h \
    triangle.h \
    matrix.h
