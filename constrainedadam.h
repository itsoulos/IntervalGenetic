#ifndef CONSTRAINEDADAM_H
#define CONSTRAINEDADAM_H
#include <vector>
#include <problem.h>

class ConstrainedAdam
{
private:
    std::vector<double> gradient;
    Problem *target;
    double learningRate;
    unsigned iterations;
    std::vector<double> M_0;
    std::vector<double> U_0;
    double b1;
    double b2;
    std::vector<double> rM;
    std::vector<double> lM;
    std::vector<double> best;
public:
    ConstrainedAdam(Problem *p);
    void Optimize();
    void setB1(double b);
    void setB2(double b);
    double getB1() const;
    double getB2() const;
    void setLearningRate(double r);
    double getLearningRate() const;
    void   setIterations(int it);
    int    getIterations() const;
    std::vector<double> getBestX();
    double getBestY();
};

#endif // CONSTRAINEDADAM_H
