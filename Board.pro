#-------------------------------------------------
#
# Project created by QtCreator 2018-01-29T11:27:30
#
#-------------------------------------------------

win32 {
    include( "C:/qwt-6.1.3/features/qwt.prf" )
    include( "C:/QwtPolar-1.1.1/features/qwtpolar.prf" )
}
unix {
    include( "/usr/local/qwt-6.1.3/features/qwt.prf" )
    include( "/usr/local/qwtpolar-1.1.1/features/qwtpolar.prf" )
}

win32 {
    QMAKE_CXXFLAGS += -utf-8
}

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Board
TEMPLATE = app

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    boardplot.cpp \
    pointpolarstream.cpp \
    convexhull.cpp \
    pointpolardegreeseries.cpp \
    sideplot.cpp \
    pointstream.cpp \
    histogramdialog.cpp \
    histogramplot.cpp \
    bluetooththread.cpp \
    radiusscaleengine.cpp \
    histogramcreator.cpp

HEADERS += \
        mainwindow.h \
    boardplot.h \
    pointpolarstream.h \
    convexhull.h \
    pointpolardegreeseries.h \
    sideplot.h \
    pointstream.h \
    histogramdialog.h \
    histogramplot.h \
    bluetooththread.h \
    radiusscaleengine.h \
    histogramcreator.h

FORMS += \
        mainwindow.ui \
    histogramdialog.ui

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/bluetooth/lib/bluetoothserialport.lib
else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/bluetooth/lib/libbluetoothserialport.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/bluetooth/lib/ -lbluetoothserialport
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/bluetooth/lib/ -lbluetoothserialportd
else:unix:!macx: LIBS += -L$$PWD/bluetooth/lib/ -lbluetoothserialport

unix: LIBS += -lbluetooth
win32: LIBS += -lws2_32 -lbthprops

INCLUDEPATH += $$PWD/bluetooth/include
DEPENDPATH += $$PWD/bluetooth/include
