#ifndef INTEGERGENETIC_H
#define INTEGERGENETIC_H
# include <intervalproblem.h>
# include <QVariant>
typedef vector<int> IDATA;

class IntegerGenetic
{
private:
    IntervalProblem *problem;
    double selection_rate,mutation_rate;
    int  gcount;
    int generation;
    vector<IDATA> genome;
    vector<IDATA> children;
    const int maxDepth=20;
    IntervalData fitnessArray;
    Interval fitness(IDATA &x);
    IntervalData produceInterval(IDATA &x);
    void mutate();
    void selection();
    void crossover();
    int tournament();
    void calcFitnessArray();
    IntervalData tempx;
    Data drandDat;
    int nsamples;
    int threads;
public:
    IntegerGenetic(IntervalProblem *p,int count,int t);
    void setSamples(int n);
    void setSelectionRate(double r);
    double getSelectionRate() const;
    void setMutationRate(double r);
    double getMutationRate() const;
    void nextGeneration();
    void getBest(IntervalData &x,Interval &y);
    IntervalData getMaximumInterval();
    Data getBestPoint();
    void    localSearch(int pos);
    ~IntegerGenetic();
};

#endif // INTEGERGENETIC_H
