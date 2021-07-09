INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/core.cpp \
    $$PWD/datacenter.cpp \
    $$PWD/indexmanager.cpp \
    $$PWD/indexthread.cpp \
    $$PWD/partitionindexthread.cpp \
    $$PWD/pathindexthread.cpp \
    $$PWD/searchthread.cpp

HEADERS += \
#    $$PWD/Core_global.h \
    $$PWD/core.h \
    $$PWD/datacenter.h \
    $$PWD/indexmanager.h \
    $$PWD/indexthread.h \
    $$PWD/partitionindexthread.h \
    $$PWD/pathindexthread.h \
    $$PWD/searchthread.h

TRANSLATIONS += \
    $$PWD/Core_zh_CN.ts
