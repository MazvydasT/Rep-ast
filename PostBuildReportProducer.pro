#-------------------------------------------------
#
# Project created by QtCreator 2015-09-21T21:45:54
#
#-------------------------------------------------

QT       += core gui axcontainer

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PostBuildReportProducer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

RC_ICONS = fastReport.ico
