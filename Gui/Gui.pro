QT       += core gui network
include(../Core/Core.pri)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# -- 调试 --
QMAKE_CFLAGS_RELEASE += -g
QMAKE_CXXFLAGS_RELEASE += -g
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_LFLAGS_RELEASE = -mthreads -W
# -- 调试 --

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    ccrashstack.cpp \
    copymovefiledialog.cpp \
    main.cpp \
    mainwindow.cpp \
    resultlistitem.cpp

HEADERS += \
    aboutdialog.h \
    ccrashstack.h \
    copymovefiledialog.h \
    mainwindow.h \
    resultlistitem.h

FORMS += \
    aboutdialog.ui \
    copymovefiledialog.ui \
    mainwindow.ui \
    resultlistitem.ui

TRANSLATIONS += \
    Gui_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS = bsearch.ico
