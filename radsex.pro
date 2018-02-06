TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += include/

HEADERS += \
    src/analysis.h \
    src/output.h \
    src/radsex.h \
    src/sex_distribution.h \
    src/parameter.h \
    src/parameters.h \
    src/process_reads.h \
    src/utils.h \
    src/sequence_file.h \
    src/popmap_file.h \
    src/subset.h

SOURCES += \
    src/main.cpp \
    src/output.cpp \
    src/radsex.cpp \
    src/sex_distribution.cpp \
    src/process_reads.cpp \
    src/utils.cpp \
    src/sequence_file.cpp \
    src/popmap_file.cpp \
    src/subset.cpp
