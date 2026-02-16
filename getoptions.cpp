# include <getoptions.h>
QString filename="";
int     randomSeed=1;
int     chromosomes=500;
int     maxGenerations=200;
double  selection_rate=0.10;
double  mutation_rate=0.05;
int     threads=16;
double   adam_b1=0.9;
double   adam_b2=0.999;
double   adam_learningrate=0.25;
int      adam_iterations=100000;
QString  intervalMethod="none";
QString  localMethod="gradient";
bool     debug=true;
double   gradient_learningrate=0.00001;
int      gradient_iterations=100000;
int      bfgs_iterations=2001;
int      integer_samples=50;
int      double_samples=50;

QStringList fitnessOption;
QString     selectedFitnessOption;
