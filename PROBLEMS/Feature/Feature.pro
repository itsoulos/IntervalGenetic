#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T08:13:00
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = Feature
TEMPLATE = lib
INCLUDEPATH += /usr/include/eigen3/

QMAKE_CXXFLAGS_RELEASE += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -ffast-math -fopt-info
QMAKE_CFLAGS_RELEASE += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -ffast-math

QMAKE_CXXFLAGS_RELEASE += -std=c++11
QMAKE_CFLAGS_RELEASE += -std=c++11

QMAKE_CXXFLAGS += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -ffast-math
QMAKE_CFLAGS += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -ffast-math

DEFINES += FEATURE_LIBRARY
LIBS +=  -larmadillo

SOURCES += feature.cpp \
    doublestack.cc \
    symbol.cc \
    rule.cc \
    program.cc \
    fpoptimizer.cc \
    fparser.cc \
    cprogram.cc \
    kmeans.cc \
    matrix_functions.cc \
    mapper.cc \
    model.cc \
    problem.cc \
    neural.cc \
    gensolver.cc \
    nnprogram.cc \
    gpopulation.cc \
    knn.cc \
    rbf_model.cc \
    interval.cpp \
    Rbf.cc \
    tolmin.cc

HEADERS += feature.h\
        feature_global.h \
    doublestack.h \
    interval.h \
    symbol.h \
    rule.h \
    interval.h \
    tolmin.h \
    program.h \
    grand.h \
    fptypes.hh \
    fpconfig.hh \
    gpopulation.h \
    fparser.hh \
    cprogram.h \
    kmeans.h \
    LBFGSB.h \
    matrix_functions.h \
    mapper.h \
    model.h \
    problem.h \
    neural.h \
    gensolver.h \
    nnprogram.h \
    knn.h \
    LBFGS.h \
	LBFGSpp/BFGSMat.h\
	LBFGSpp/BKLDLT.h\
	LBFGSpp/Cauchy.h\
	LBFGSpp/LineSearchBacktracking.h\
	LBFGSpp/LineSearchBracketing.h\
	LBFGSpp/LineSearchMoreThuente.h\
	LBFGSpp/LineSearchNocedalWright.h\
	LBFGSpp/Param.h\
	LBFGSpp/SubspaceMin.h\
    rbf_model.h \
    Rbf.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
