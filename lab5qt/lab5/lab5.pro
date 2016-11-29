#-------------------------------------------------
#
# Project created by QtCreator 2016-11-20T21:31:47
#
#-------------------------------------------------

QT       += core gui
QT += serialport
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab5
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    console.cpp \
    settingsdialog.cpp \
    qcustomplot.cpp \
    spectrum.cpp

HEADERS  += mainwindow.h \
    console.h \
    settingsdialog.h \
    qcustomplot.h \
    spectrum.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

SUBDIRS += \
    terminal.pro

RESOURCES += \
    terminal.qrc

DISTFILES += \
    lab5.pro.user \
    terminal.pro.user
