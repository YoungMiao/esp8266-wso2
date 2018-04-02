#-------------------------------------------------
#
# Project created by QtCreator 2018-01-10T13:35:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml network sql webenginewidgets

TARGET = SmartStreetLampManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD\\include
win32:LIBS += $$PWD\\lib\\MapView.lib

include("third\\QtitanRibbon\\src\\shared\\qtitanribbon.pri")


include("SmartStreetLampControlAPI\\SmartStreetLampControlAPI.pri")


SOURCES += main.cpp\
        mainwindow.cpp \
    component/mapoperation.cpp \
    component/lampinfotreemanager.cpp \
    component/clampdataservermanager.cpp \
    setting/csystemsettingdlg.cpp \
    ui/clampbasicinfodialog.cpp \
    ui/clamprealtimedatadialog.cpp \
    ui/clamphistorydatadialog.cpp \
    setting/clightingstrategydialog.cpp \
    third/echarts/echartswebview.cpp \
    include/commonprocess.cpp \
    init/loginpage.cpp

HEADERS  += mainwindow.h \
    component/mapoperation.h \
    component/lampinfotreemanager.h \
    include/lampinfo.h \
    component/clampdataservermanager.h \
    setting/csystemsettingdlg.h \
    ui/clampbasicinfodialog.h \
    ui/clamprealtimedatadialog.h \
    ui/clamphistorydatadialog.h \
    setting/clightingstrategydialog.h \
    third/echarts/echartswebview.h \
    init/loginpage.h

FORMS += \
    setting/syssetting.ui \
    ui/clampbasicinfodialog.ui \
    ui/clamphistorydatadialog.ui \
    ui/clamprealtimedatadialog.ui \
    setting/clightingstrategydialog.ui \
    init/loginpage.ui

win32 {
    DEFINES *= CORE_EXPORT=__declspec(dllimport)
    DEFINES *= GUI_EXPORT=__declspec(dllimport)
    DEFINES *= ANALYSISI_EXPORT=__declspec(dllimport)

    LIBS += $$PWD\\third\\qgis\\lib\\qgis_core.lib
    LIBS += $$PWD\\third\\qgis\\lib\\qgis_gui.lib
    LIBS += $$PWD\\third\\qgis\\lib\\qgis_analysis.lib
    LIBS += $$PWD\\third\\qgis\\lib\\qgis_native.lib

    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    PWD_WIN = $${PWD}
    PWD_WIN ~= s,/,\\,g
    CONFIG(debug, debug|release) {
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\third\\qgis\\debug $${DESTDIR_WIN}debug$$escape_expand(\\n\\t))
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\third\\qgis\\plugins\\debug $${DESTDIR_WIN}debug\\qgis\\plugins$$escape_expand(\\n\\t))
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\third\\libwebTiles\\dll\\debug $${DESTDIR_WIN}debug$$escape_expand(\\n\\t))
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\third\\qgis\\resources $${DESTDIR_WIN}debug\\qgis\\resources$$escape_expand(\\n\\t))
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\map $${DESTDIR_WIN}debug\\map$$escape_expand(\\n\\t))
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\dll $${DESTDIR_WIN}debug$$escape_expand(\\n\\t))
}
    else {
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\third\\qgis\\release $${DESTDIR_WIN}release$$escape_expand(\\n\\t))
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\third\\qgis\\plugins\\release $${DESTDIR_WIN}release\\qgis\\plugins$$escape_expand(\\n\\t))
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\third\\libwebTiles\\dll\\release $${DESTDIR_WIN}release$$escape_expand(\\n\\t))
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\third\\qgis\\resources $${DESTDIR_WIN}release\\qgis\\resources$$escape_expand(\\n\\t))
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\map $${DESTDIR_WIN}release\\map$$escape_expand(\\n\\t))
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\dll $${DESTDIR_WIN}release$$escape_expand(\\n\\t))
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\resource\\echart $${DESTDIR_WIN}release\\resource\\echart$$escape_expand(\\n\\t))
    #拷贝配置文件
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\conf  $${DESTDIR_WIN}release\\conf$$escape_expand(\\n\\t))
    #拷贝路灯坐标文件
        QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /E /I $${PWD_WIN}\\streetLonLatData\\coordinate $${DESTDIR_WIN}release\\coordinate$$escape_expand(\\n\\t))
        }
}

RESOURCES     = SmartStreetLamp.qrc


