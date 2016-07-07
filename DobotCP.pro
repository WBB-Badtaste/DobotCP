#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T22:23:37
#
#-------------------------------------------------

QT       += core gui network xml serialport printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DobotCP
TEMPLATE = app

SOURCES += \
        src/main.cpp\
        src/MainWindow.cpp \
    src/CMouseControlDialog.cpp \
    src/E2CubicSpline.cpp \
    src/Trajectory.cpp \
    src/TrajectoryOperator.cpp \
    src/excelengine.cpp

HEADERS  += \
        src/MainWindow.h \
    src/CMouseControlDialog.h \
    src/E2CubicSpline.h \
    src/Trajectory.h \
    src/TrajectoryOperator.h \
    src/excelengine.h

SOURCES +=\
        src/CPLTProcess.cpp\
        src/CPaintWidget.cpp\
        src/CTextDesignDialog.cpp\
        src/CConfigCPDialog.cpp\
        src/CConfigUserDefDialog.cpp\

HEADERS +=\
        src/CPLTProcess.h\
        src/CPaintWidget.h\
        src/CTextDesignDialog.h\
        src/CConfigCPDialog.h\
        src/CConfigUserDefDialog.h\

INCLUDEPATH +=\
        SRC/DobotDll

DESTDIR = $$PWD/src
INCLUDEPATH += $$DESTDIR

CONFIG(debug, debug | release) {
    LIBS += -L$$DESTDIR/ -lDobotDll_d
} else {
    LIBS += -L$$DESTDIR/ -lDobotDll
}

QT+=axcontainer

RESOURCES += \
    res/res.qrc

RC_FILE = res/version.rc

SUBDIRS += \
    ../SplineTest/SplineTest.pro
