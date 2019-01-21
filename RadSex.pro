TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    include/edlib/edlib.cpp \
    include/kfun/kfun.cpp \
    src/barcodes_file.cpp \
    src/demultiplexing.cpp \
    src/frequencies.cpp \
    src/group_loci.cpp \
    src/main.cpp \
    src/mapping.cpp \
    src/output.cpp \
    src/parameters.cpp \
    src/popmap_file.cpp \
    src/process_reads.cpp \
    src/radsex.cpp \
    src/scaffold_lengths.cpp \
    src/sequence_file.cpp \
    src/sex_distribution.cpp \
    src/significant_sequences.cpp \
    src/stats.cpp \
    src/subset.cpp \
    src/utils.cpp \
    include/bwa/bntseq.c \
    include/bwa/bwa.c \
    include/bwa/bwamem.c \
    include/bwa/bwamem_extra.c \
    include/bwa/bwamem_pair.c \
    include/bwa/bwt.c \
    include/bwa/bwt_gen.c \
    include/bwa/bwtindex.c \
    include/bwa/is.c \
    include/bwa/kstring.c \
    include/bwa/ksw.c \
    include/bwa/kthread.c \
    include/bwa/malloc_wrap.c \
    include/bwa/QSufSort.c \
    include/bwa/rle.c \
    include/bwa/rope.c \
    include/bwa/utils.c \
    src/depth.cpp

HEADERS += \
    include/bwa/bntseq.h \
    include/bwa/bwa.h \
    include/bwa/bwamem.h \
    include/bwa/bwt.h \
    include/bwa/kbtree.h \
    include/bwa/khash.h \
    include/bwa/kseq.h \
    include/bwa/ksort.h \
    include/bwa/kstring.h \
    include/bwa/ksw.h \
    include/bwa/kvec.h \
    include/bwa/malloc_wrap.h \
    include/bwa/QSufSort.h \
    include/bwa/rle.h \
    include/bwa/rope.h \
    include/bwa/utils.h \
    include/edlib/edlib.h \
    include/kfun/kfun.h \
    include/kseq/kseq.h \
    src/analysis.h \
    src/barcodes_file.h \
    src/demultiplexing.h \
    src/frequencies.h \
    src/group_loci.h \
    src/mapping.h \
    src/output.h \
    src/parameter.h \
    src/parameters.h \
    src/popmap_file.h \
    src/process_reads.h \
    src/radsex.h \
    src/scaffold_lengths.h \
    src/sequence_file.h \
    src/sex_distribution.h \
    src/significant_sequences.h \
    src/stats.h \
    src/subset.h \
    src/utils.h \
    src/depth.h
