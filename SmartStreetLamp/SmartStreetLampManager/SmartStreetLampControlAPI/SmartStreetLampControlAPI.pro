#-------------------------------------------------
#
# Project created by QtCreator 2018-01-15T15:54:56
#
#-------------------------------------------------

QT       += core
QT       += qml
QT       -= gui

TARGET = SmartStreetLampControlAPI
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++11
TEMPLATE = app

include(qtrest/com_github_kafeg_qtrest.pri)
SOURCES += \
    main.cpp \
    smartstreetcontrolbase.cpp \
    smartstreetcontrolkaa.cpp \
    kaasubject.cpp \
    kaasubjectmqtt.cpp \
    sslamphistorydatabase.cpp \
    sslamphistorydata.cpp

HEADERS += \
    smartstreetcontrolbase.h \
    smartstreetcontrolkaa.h \
    lampcontrolcommanddef.h \
    kaasubject.h \
    kaasubjectmqtt.h \
    sslamphistorydatabase.h \
    sslamphistorydata.h

INCLUDEPATH += $$PWD/qmqtt/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lqmqtt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lqmqttd

win32 {

    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    PWD_WIN = $${PWD}
    PWD_WIN ~= s,/,\\,g
    CONFIG(debug, debug|release) {
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\bin  $${DESTDIR_WIN}debug$$escape_expand(\\n\\t))
    }
    else {
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\bin  $${DESTDIR_WIN}release$$escape_expand(\\n\\t))
        }
}


DISTFILES += \
    SmartStreetLampControlAPI.pri





