#include "simanbounds.h"

SimanBounds::SimanBounds(IntervalProblem *p)
{
    problem = p;
    left.resize(p->getDimension());
    right.resize(p->getDimension());
    margins=p->getMargins();
    for(int i=0;i<(int)left.size();i++)
    {
        left[i]= margins[i].leftValue();
        right[i]=margins[i].rightValue();
    }
    k=0;
    T0=1e+8;
    neps = 100;
    besty = fitness(left,right);
}

void    SimanBounds::randomBounds(Data &xl,Data &xr)
{
    xl.resize(problem->getDimension());
    xr.resize(problem->getDimension());
    IntervalData mm;
    mm.resize(xl.size());
    mm = problem->createRandomInterval();
    for(int i=0;i<problem->getDimension();i++)
    {

/*        double l = margins[i].leftValue();
        double r = margins[i].rightValue();
        double mid = l+(r-l)/2;
        xl[i]=l+rand()*1.0/RAND_MAX*(mid-l);
        xr[i]=mid+rand()*1.0/RAND_MAX*(r-mid);*/
        xl[i]=mm[i].leftValue();
        xr[i]=mm[i].rightValue();

    }
}

Interval  SimanBounds::fitness(Data &xl,Data &xr)
{
    const int nsamples = 100;
    srand(1);

    Data trialx;
    trialx.resize(xl.size());
    double minf=1e+100,maxf=-1e+100;
    for(int i=1;i<=nsamples;i++)
    {
       for(int j=0;j<xl.size();j++)
       {
           trialx[j]=xl[j]+(rand()*1.0/RAND_MAX)*(xr[j]-xl[j]);
       }
       double y=problem->funmin(trialx);
       if(y<minf || i==1) minf = y;
       if(y>maxf || i==1) maxf = y;
    }
    return Interval(minf,maxf);
}

Interval  SimanBounds::getBestFitness()
{
    return besty;
}

void    SimanBounds::getBounds(Data &xl,Data &xr)
{
    xl = left;
    xr = right;
}

void    SimanBounds::Solve()
{
    T0=1e+8;
    Data xl,xr;
    xl.resize(problem->getDimension());
    xr.resize(problem->getDimension());
    Interval ypoint = besty;
    Data testXR = right;
    Data testXL = left;
    while(true)
    {
        for(int i=1;i<=neps;i++)
        {
            randomBounds(xl,xr);
            Interval fy = fitness(xl,xr);

            if(problem->lowerValue( fy,ypoint))
            {
                ypoint = fy;
                testXR = xr;
                testXL = xl;
                if(problem->lowerValue(ypoint,besty))
                {
                    left = testXL;
                    right = testXR;
                    besty = ypoint;
                }
            }
            else
            {
                double r = fabs(rand()*1.0/RAND_MAX);
                double ratio = exp(-(fy.leftValue()-ypoint.leftValue())/T0);
                double xmin = ratio<1?ratio:1;
                if(r<xmin)
                {
                    ypoint = fy;
                    testXR = xr;
                    testXL = xl;
                }
            }

        }
        reduceTemp();
        printf("T=%20.10lg Ybest=[%20.10lg,%20.10lg]\n",
               T0,besty.leftValue(),besty.rightValue());
        if(T0<=1e-6) break;
        for(int i=0;i<margins.size();i++)
            margins[i]=Interval(left[i],right[i]);
        problem->setMargins(margins);
    }
}

void    SimanBounds::reduceTemp()
{
    const double alpha = 0.99;
    T0 =T0 * pow(alpha,k);
    k=k+1;
}

SimanBounds::~SimanBounds()
{

}
