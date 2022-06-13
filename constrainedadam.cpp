#include "constrainedadam.h"
#include <math.h>
#include <omp.h>

ConstrainedAdam::ConstrainedAdam(Problem *p)
{
    target = p;
    learningRate = 0.25;
    iterations = 100000;
    gradient.resize(p->getDimension(),0.0);
    M_0.resize(p->getDimension(),0.0);
    U_0.resize(p->getDimension(),0.0);
    rM = target->getRightMargin();
    lM = target->getLeftMargin();
    b1=0.9;
    b2=0.999;

}

void    ConstrainedAdam::setB1(double b)
{
    if(b>=0.0 && b<=1.0)
        b1=b;
}

void ConstrainedAdam::setB2(double b)
{
    if(b>=0.0 && b<=1.0)
        b2=b;
}

double  ConstrainedAdam::getB1() const
{
    return b1;
}

double ConstrainedAdam::getB2() const
{
    return b2;
}

void ConstrainedAdam::setLearningRate(double r)
{
    if(r>=0.0 && r<=1.0)
        learningRate=r;
}

double ConstrainedAdam::getLearningRate() const
{
    return learningRate;
}

void  ConstrainedAdam::setIterations(int it)
{
    if(it>=0) iterations=it;
}

int  ConstrainedAdam::getIterations() const
{
    return iterations;
}

void ConstrainedAdam::Optimize(){
    std::vector<double> initial;
#pragma omp critical
{
    initial =target->getUniformRandomPoint();// target->getRandomPoint();// target->getUniformRandomPoint();
}
    //for(auto b : initial) std::cout << b << " ";
    for(unsigned i = 0; i < iterations; i++){
        double a = learningRate * sqrt( 1.0 - pow(b2,i+1) ) / (1.0 - pow(b1,i+1));
        //std::cout << "a: " << a << std::endl;
        target->granal(initial,gradient);
        //for(auto test1 : gradient) std::cout << test1 << " ";
        //getchar();
        for(unsigned j = 0; j < gradient.size(); j++){
            M_0[j] = b1 * M_0[j] + (1-b1) * gradient[j];
            U_0[j] = b2 * U_0[j] + (1-b2) * gradient[j] * gradient[j];
            //std::cout << "M_0: " << M_0[j] << " U_0: " << U_0[j] << std::endl;
            //std::cout << initial[j] << " -> ";
            initial[j] -= a * M_0[j] / (sqrt(U_0[j]) + 1e-1) ;
            //std::cout << initial[j] << std::endl;
            if(initial[j] > rM[j]) initial[j] = rM[j];
            if(initial[j] < lM[j]) initial[j] = lM[j];
        }
        best = initial;
        extern bool debug;
        if(debug)
        if(i % (iterations/10) == 0)std::cout << "ADAM ITER: " << i << " ERROR: " << target->funmin(initial) << std::endl;

        //getchar();
    }
}

std::vector<double> ConstrainedAdam::getBestX(){
    return best;
}

double ConstrainedAdam::getBestY(){
    return target->funmin(best);
}
