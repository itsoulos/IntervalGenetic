QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle
LIBS+=  -lm -fopenmp

QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp
QMAKE_CFLAGS_RELEASE += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp

QMAKE_CXXFLAGS += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp
QMAKE_CFLAGS += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp

QMAKE_LFLAGS += -O3


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boundedgradientdescent.cpp \
    constrainedadam.cpp \
        dllproblem.cpp \
    doublepop.cpp \
        getoptions.cpp \
    integergenetic.cpp \
    interval.cpp \
        intervalgenetic.cpp \
        intervalproblem.cpp \
        intervalpso.cpp \
        main.cpp \
    tolmin.cc \
    problem.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    boundedgradientdescent.h \
    constrainedadam.h \
    dllproblem.h \
    doublepop.h \
    getoptions.h \
    integergenetic.h \
    interval.h \
    intervalgenetic.h \
    intervalproblem.h \
    intervalpso.h \
    tolmin.h \
    problem.h
