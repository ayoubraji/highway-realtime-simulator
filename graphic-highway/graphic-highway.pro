#-------------------------------------------------
#
# Project created by QtCreator 2018-03-11T16:36:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphic-highway
TEMPLATE = app

SOURCES += main.cpp\
        Graphics\mainwindow.cpp \
    #../main.c
    Graphics/mainwindow.cpp

LIBS += -lpthread

HEADERS  += mainwindow.h
