#ifndef INTERVALPSO_H
#define INTERVALPSO_H
# include <intervalproblem.h>

class IntervalPso
{
private:
    int particleCount;
    int generationCount;
    IntervalProblem *problem;
    vector<IntervalData> position;
    vector<IntervalData> bestPosition;
    vector<IntervalData> velocity;
    IntervalData        fitnessArray;
    IntervalData        bestFitnessArray;
    IntervalData        bestx;
    Interval            besty;
    void            updateVelocities();
    void            updatePositions();
    Interval        psoFitness(IntervalData &x);
    Interval        psoLocalSearch(IntervalData &x,int iters);
public:
    IntervalPso(IntervalProblem *p,int pcount);
    int             getParticleCount() const;
    void            Solve();
    IntervalData    getBestx();
    Interval        getBesty();
};

#endif // INTERVALPSO_H
