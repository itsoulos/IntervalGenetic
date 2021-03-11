#ifndef INTERVALGENETIC_H
#define INTERVALGENETIC_H
# include <intervalproblem.h>
# include <QLibrary>

class IntervalGenetic
{
private:
    IntervalProblem     **problem;
    IntervalData        fitnessArray;
    vector<IntervalData> chromosome;
    vector<IntervalData> children;
    vector<Data>         trainData;
    unsigned t;
    int nsamples;
    int                  count,generation;
    double              selection_rate,mutation_rate;
    int     tournament();
    Interval fitness(IntervalData &x,unsigned ti = 0);
    void    calcFitnessArray();
    void    crossover();
    void    mutate();
    void    selection();
    void    makeSamples(IntervalData &x,vector<Data> &s,int ti);
    Data drandDat;
public:
    IntervalGenetic(IntervalProblem *p,int gcount);
    IntervalGenetic(QString filename,QJsonObject settings,int gcount,unsigned Threads=8);
    void    setSelectionRate(double r);
    double  getSelectionRate() const;
    void    setMutationRate(double r);
    double  getMutationRate() const;
    int     getChromosomeCount() const;
    int     getGenerationCount() const;
    void    nextGeneration();
    void    getBest(IntervalData &bestx,Interval &besty);
    void    localSearch(IntervalData &x,Interval &value);
    IntervalData getMaximumInterval();
    Data    getBestPoint();
    void    optimize(IntervalData &x);
    void    setSamples(int n);
    ~IntervalGenetic();

};

#endif // INTERVALGENETIC_H
