#-------------------------------------------------
#
# Project created by QtCreator 2018-03-11T16:36:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphic-highway
TEMPLATE = app

SOURCES +=\
    #../main.c
    vehicle.cpp \
    highway.cpp \
    Graphics/highway_gui.cpp \
    Graphics/button.cpp \
    threads_controller.cpp \
    main.cpp

LIBS += -lpthread

HEADERS  += \
    vehicle.h \
    highway.h \
    Graphics/highway_gui.h \
    Graphics/button.h \
    threads_controller.h
