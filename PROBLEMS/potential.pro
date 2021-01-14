TEMPLATE=lib
QMAKE_CXXFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pg
QMAKE_CFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pg

QMAKE_CXXFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pg
QMAKE_CFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pg

SOURCES+=potential.cc \
    interval.cpp

HEADERS += \
    interval.h
