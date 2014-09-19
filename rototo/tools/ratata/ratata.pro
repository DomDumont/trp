#-------------------------------------------------
#
# Project created by QtCreator 2014-06-03T22:06:31
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ratata
TEMPLATE = app

INCLUDEPATH += ./src

SOURCES += main.cpp\
    highlighter.cpp \
    mdimainwindow.cpp \
    mdichild.cpp \
    projectwidget.cpp \
    watchwidget.cpp \
    callstackwidget.cpp \
    outputwidget.cpp \
    consolewidget.cpp \
    dlgnewproject.cpp \
    tcpclient.cpp \
    errorwidget.cpp \
    prefdialog.cpp \
    breakpoint.cpp \
    breakpointwidget.cpp

HEADERS  += \
    highlighter.h \
    mdimainwindow.h \
    mdichild.h \
    projectwidget.h \
    watchwidget.h \
    callstackwidget.h \
    outputwidget.h \
    consolewidget.h \
    dlgnewproject.h \
    tcpclient.h \
    errorwidget.h \
    prefdialog.h \
    breakpoint.h \
    breakpointwidget.h

RESOURCES += \
    qtresources/timeresources.qrc

RC_FILE = myapp.rc
ICON = myicon.icns


DESTDIR = ../ratata_distrib/bin
UI_DIR = .

CONFIG(debug, debug|release) {
        QMAKE_INFO_PLIST = RatataD.plist
        TARGET = RatataDebug
        OBJECTS_DIR = ../ratata_distrib/obj
        MOC_DIR = ../ratata_distrib/moc
}

CONFIG(release, debug|release) {
        QMAKE_INFO_PLIST = Ratata.plist
        TARGET = Ratata
        OBJECTS_DIR = ../ratata_distrib/obj
        MOC_DIR = ../ratata_distrib/moc
}
