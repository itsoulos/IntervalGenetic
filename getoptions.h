#ifndef GETOPTIONS_H
#define GETOPTIONS_H
# include <QString>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>

extern QString  filename;
extern int      chromosomes;
extern int      maxGenerations;
extern double   selection_rate;
extern double   mutation_rate;
extern int      randomSeed;
extern int      threads;
extern double   adam_b1;
extern double   adam_b2;
extern double   adam_learningrate;
extern int      adam_iterations;
extern QString  intervalMethod;
extern QString  localMethod;
extern bool     debug;
extern double   gradient_learningrate;
extern int      gradient_iterations;
extern int      bfgs_iterations;
extern int      integer_samples;
extern int      double_samples;
#endif // GETOPTIONS_H
