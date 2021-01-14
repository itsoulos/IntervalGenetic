#ifndef BOUNDEDGRADIENTDESCENT_H
#define BOUNDEDGRADIENTDESCENT_H

#include <vector>
#include <problem.h>

class BoundedGradientDescent
{
private :
    std::vector<double> gradient;
    Problem *target;
    double learningRate;
    unsigned iterations;
    std::vector<double> rM;
    std::vector<double> lM;
    std::vector<double> best;
public:
    BoundedGradientDescent(Problem * p);
    void Optimize();
    void setLearningRate(double r);
    double getLearningRate() const;
    void   setIterations(int it);
    int    getIterations() const;
    std::vector<double> getBestX();
    double getBestY();
};

#endif // BOUNDEDGRADIENTDESCENT_H
