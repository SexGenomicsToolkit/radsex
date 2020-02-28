TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L$$PWD/include/bwa/ -lbwa -pthread -static-libstdc++ -lz
INCLUDEPATH += $$PWD/include/bwa $$PWD/include
DEPENDPATH += $$PWD/include/bwa

PRE_TARGETDEPS += $$PWD/include/bwa/libbwa.a

HEADERS += \
    src/analysis.h \
    src/arg_parser.h \
    src/depth.h \
    src/marker.h \
    src/markers_table.h \
    src/parameters.h \
    src/popmap.h \
    src/stats.h \
    src/utils.h

SOURCES += \
    src/analysis.cpp \
    src/depth.cpp \
    src/main.cpp \
    src/marker.cpp \
    src/markers_table.cpp \
    src/popmap.cpp \
    src/stats.cpp \
    include/kfun/kfun.cpp

