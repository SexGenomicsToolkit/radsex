TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/main.cpp \
    src/arg_parser.cpp \
    src/utils.cpp \
    src/analysis.cpp \
    src/analysis.cpp \
    src/input_dir.cpp \
    src/process_file.cpp \
    src/gzstream.cpp \
    src/output.cpp

HEADERS += \
    src/arg_parser.h \
    src/parameters.h \
    src/utils.h \
    src/analysis.h \
    src/analysis.h \
    src/input_dir.h \
    src/process_file.h \
    src/input_file.h \
    src/gzstream.h \
    src/output.h
