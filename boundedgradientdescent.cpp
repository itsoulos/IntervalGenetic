#include "boundedgradientdescent.h"
#include "omp.h"

BoundedGradientDescent::BoundedGradientDescent(Problem * p)
{
    target = p;
    learningRate = 0.01;
    iterations = 100000;
    gradient.resize(p->getDimension(),0.0);
    rM = target->getRightMargin();
    lM = target->getLeftMargin();
}

void BoundedGradientDescent::setLearningRate(double r)
{
    if(r>=0.0 && r<=1.0)
        learningRate=r;
}

double BoundedGradientDescent::getLearningRate() const
{
    return learningRate;
}

void  BoundedGradientDescent::setIterations(int it)
{
    if(it>=0) iterations=it;
}

int  BoundedGradientDescent::getIterations() const
{
    return iterations;
}

std::vector<double> BoundedGradientDescent::getBestX(){
    return best;
}

double BoundedGradientDescent::getBestY(){
    return target->funmin(best);
}

void BoundedGradientDescent::Optimize(){
    std::vector<double> initial;
#pragma omp critical
{
    initial = target->getUniformRandomPoint();
}
    for(unsigned i = 0; i < iterations; i++){
        target->granal(initial,gradient);
        for(unsigned j = 0; j < gradient.size(); j++){
            initial[j] -= learningRate * gradient[j];
            if(initial[j] > rM[j]) initial[j] = rM[j];
            if(initial[j] < lM[j]) initial[j] = lM[j];
        }
        best = initial;
        extern bool debug;
        if(debug)
        if(i % (iterations/10) == 0)std::cout << "BGD ITER: " << i << " ERROR: " << target->funmin(initial) << std::endl;

        //getchar();
    }
}
