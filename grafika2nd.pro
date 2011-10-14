#-------------------------------------------------
#
# Project created by QtCreator 2011-10-14T11:19:44
#
#-------------------------------------------------
TARGET = grafika2nd
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lopengl32 -lglu32 -lglut32

INCLUDEPATH = C:/glut-3.5/include
LIBS += C:/glut-3.7.6-bin/glut32.lib

SOURCES += main.cpp
