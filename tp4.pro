#-------------------------------------------------
#
# Project created by QtCreator 2015-03-06T14:17:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tp4
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        editorwindow.cpp \
    editorqsplitter.cpp \
    viewedit.cpp \
    linenumber.cpp \
    qtexteditnumber.cpp \
    htmlhighlighter.cpp

HEADERS  += editorwindow.h \
    editorqsplitter.h \
    viewedit.h \
    linenumber.h \
    qtexteditnumber.h \
    htmlhighlighter.h

RESOURCES += \
    ressources.qrc

TRANSLATIONS += qteditor_fr.ts qteditor_de.ts
