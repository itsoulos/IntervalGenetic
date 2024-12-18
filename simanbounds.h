#ifndef SIMANBOUNDS_H
#define SIMANBOUNDS_H

# include <intervalproblem.h>
# include <QVariant>
class SimanBounds
{
private:
    IntervalProblem *problem;
    double T0;
    int k,neps;
    Data left,right;
    Interval besty;
    IntervalData margins;
public:
    SimanBounds(IntervalProblem *p);
    Interval  fitness(Data &xl,Data &xr);
    Interval  getBestFitness();
    void    randomBounds(Data &xl,Data &xr);
    void    getBounds(Data &xl,Data &xr);
    void    Solve();
    void    reduceTemp();
    ~SimanBounds();
};

#endif // SIMANBOUNDS_H
