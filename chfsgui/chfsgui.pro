#-------------------------------------------------
#
# Project created by QtCreator 2018-01-20T18:25:12
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chfsgui
TEMPLATE = app

RC_FILE += app.rc
QTPLUGIN  += qsvg
PRECOMPILED_HEADER = preCompile.h
CONFIG += c++11

DEFINES += "GUIVER=1"

SOURCES += main.cpp\
        mainwindow.cpp \
    processdecorator.cpp \
    toolbar.cpp \
    configwgt.cpp \
    monitorwgt.cpp \
    uicomponents/configwgt/cfgsharedpathwgt.cpp \
    uicomponents/configwgt/cfgallowip.cpp \
    uicomponents/configwgt/cfgrules.cpp \
    utilities.cpp \
    uicomponents/configwgt/cfgportwgt.cpp \
    uicomponents/configwgt/cfgbase.cpp \
    uicomponents/configwgt/accdlg.cpp \
    uicomponents/configwgt/ruletablewidget.cpp \
    aboutdlg.cpp \
    uicomponents/qrpopup.cpp \
    QR-Code-generator-master/BitBuffer.cpp \
    QR-Code-generator-master/QrCode.cpp \
    QR-Code-generator-master/QrSegment.cpp \
    qtsingleapplication/qtlocalpeer.cpp \
    qtsingleapplication/qtsingleapplication.cpp \
    qtsingleapplication/qtlockedfile.cpp \
    qtsingleapplication/qtlockedfile_unix.cpp \
    qtsingleapplication/qtlockedfile_win.cpp

HEADERS  += mainwindow.h \
    processdecorator.h \
    precompile.h \
    toolbar.h \
    configwgt.h \
    monitorwgt.h \
    uicomponents/configwgt/cfgsharedpathwgt.h \
    uicomponents/configwgt/cfgallowip.h \
    uicomponents/configwgt/cfgrules.h \
    utilities.h \
    uicomponents/configwgt/cfgportwgt.h \
    uicomponents/configwgt/cfgbase.h \
    uicomponents/configwgt/accdlg.h \
    uicomponents/configwgt/ruletablewidget.h \
    aboutdlg.h \
    uicomponents/qrpopup.h \
    QR-Code-generator-master/BitBuffer.hpp \
    QR-Code-generator-master/QrCode.hpp \
    QR-Code-generator-master/QrSegment.hpp \
    qtsingleapplication/qtlocalpeer.h \
    qtsingleapplication/qtsingleapplication.h \
    qtsingleapplication/qtlockedfile.h

RESOURCES += \
    chfs.qrc \
    gui.qrc \
    BreezeStyleSheets\breeze.qrc \
    skin/qss.qrc

FORMS += \
    aboutdlg.ui
