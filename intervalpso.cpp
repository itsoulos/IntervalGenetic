#include "intervalpso.h"

double psoRand()
{
    return rand()*1.0/RAND_MAX;
}

IntervalPso::IntervalPso(IntervalProblem *p,int pcount)
{
    problem=p;
    particleCount=pcount;
    bestx.resize(p->getDimension());
    besty=Interval(0.0,0.0);
    position.resize(particleCount);
    fitnessArray.resize(particleCount);
    bestFitnessArray.resize(particleCount);
    bestPosition.resize(particleCount);
    velocity.resize(particleCount);


    for(int i=0;i<particleCount;i++)
    {
       position[i]=problem->createRandomInterval();
       velocity[i]=position[i];
       for(int j=0;j<problem->getDimension();j++)
       {
           double r=psoRand();
           velocity[i][j]=Interval(0.0,0.0);

       }
       bestPosition[i]=position[i];
       fitnessArray[i]=psoFitness(position[i]);

       bestFitnessArray[i]=fitnessArray[i];
       if(i==0 || problem->lowerValue(fitnessArray[i],besty))
       {
           bestx=position[i];
           besty=fitnessArray[i];
       }
    }
    generationCount=0;
}

int            IntervalPso::getParticleCount() const
{
    return particleCount;
}

Data    IntervalPso::getBestPoint()
{
    Data bestx1;
    Data trialx;
    trialx.resize(position[0].size());
    double miny=1e+100;
    const int nsamples = 50;
    srand48(1);
    for(int k=1;k<=nsamples;k++)
    {
    for(int i=0;i<bestx.size();i++)
    {
        trialx[i]=bestx[i].leftValue()+(bestx[i].rightValue()-bestx[i].leftValue())*drand48();
    }
    double fx=problem->funmin(trialx);//tolmin(trialx,&np,10);

    if(k==1 || fx<miny)
    {
        printf("FINAL MINY = %20.10lg\n",miny);

        miny=fx;
        bestx1=trialx;
    }}
    printf("FINAL MINY = %20.10lg\n",miny);
    return bestx1;
}

void           IntervalPso::Solve()
{
    for(generationCount=1;generationCount<=20;generationCount++)
    {
        updatePositions();
        updateVelocities();


    }
  //  besty=psoLocalSearch(bestx,100);

}

IntervalData   IntervalPso::getBestx()
{
    return bestx;
}

Interval       IntervalPso::getBesty()
{
    return besty;
}

Interval       IntervalPso::psoLocalSearch(IntervalData &x,int iters)
{
    IntervalData left=x;
    IntervalData right=x;
    Interval value;//=problem->IntFunmin(x);
    for(int k=1;k<=iters;k++)
    {
    for(int i=0;i<x.size();i++)
    {

        left[i]=x[i].right();
        right[i]=x[i].left();
    }
    Interval fL;//=problem->IntFunmin(left);
    Interval fR;//=problem->IntFunmin(right);

    if(problem->lowerValue(fL,fR))
    {
        x=left;
        value=fL;
    }
    else
    {
        x=right;
        value=fR;
     }

     if(problem->getVolume(x)/problem->getDimension()<1e-5) break;
    }
    return value;
}
Interval       IntervalPso::psoFitness(IntervalData &x)
{

    Data trialxx;
    trialxx.resize(x.size());
    double miny=1e+100,maxy=1e+100;
     const int nsamples = 50;

     srand48(1);
    for(int k=1;k<=nsamples;k++)
    {
    for(int i=0;i<x.size();i++)
    {
        trialxx[i]=x[i].leftValue()+(x[i].rightValue()-x[i].leftValue())*drand48();

//        trialx[i]=x[i].leftValue()+(x[i].rightValue()-x[i].leftValue())*drandDat[(k-1)*np.getDimension()+i];
    }
    double fx=problem->funmin(trialxx);//tolmin(trialx,&np,10);

    if(k==1 || fx>maxy) maxy=fx;
    if(k==1 || fx<miny) miny=fx;
    }
    return Interval(miny,maxy);

}

void           IntervalPso::updateVelocities()
{
    double inertia=0.50+psoRand()/2.0;
    double c1=1.0,c2=1.0;
    for(int i=0;i<particleCount;i++)
    {
        for(int j=0;j<problem->getDimension();j++)
        {
            Interval oldVel=velocity[i][j];
           velocity[i][j]=inertia * velocity[i][j]+c1 * psoRand()*
                   (bestPosition[i][j]-position[i][j])
                   +c2 * psoRand()*(bestx[i]-position[i][j]);
           velocity[i][j]=Interval(velocity[i][j].leftValue()/100.0,
                                   velocity[i][j].rightValue()/100.0);
        /*   printf("velocity %lf %lf\n",
                  velocity[i][j].leftValue(),
                  velocity[i][j].rightValue());*/

        /*   if(velocity[i][j].is_empty())
                velocity[i][j]=oldVel;
            if(velocity[i][j].is_empty())
            {
                double r=psoRand();
                velocity[i][j]=interval(r,r);
            }*/

        }
       problem->boundInterval(velocity[i]);
    }
}

void           IntervalPso::updatePositions()
{
    int failcount=0;
    for(int i=0;i<particleCount;i++)
    {
        IntervalData oldPos=position[i];
        for(int j=0;j<problem->getDimension();j++)
        {

            position[i][j]=position[i][j]+velocity[i][j];
        }

        /*if(!problem->isPointIn(position[i]))
        {
            failcount++;
            position[i]=oldPos;

            continue;
        }*/
            fitnessArray[i]=psoFitness(position[i]);
        if(problem->lowerValue(fitnessArray[i],bestFitnessArray[i]))
        {
            bestPosition[i]=position[i];
            bestFitnessArray[i]=fitnessArray[i];

        }

        if(problem->lowerValue(bestFitnessArray[i],besty))
        {

            besty=bestFitnessArray[i];
            printf("BESTY [%lf %lf]\n",besty.leftValue(),
                   besty.rightValue());
            bestx=bestPosition[i];
            Interval ff = psoFitness(bestx);
            printf("Best fitness [%lf,%lf]\n ",ff.leftValue(),ff.rightValue());
        }
    }
    printf("failcount = %d \n",failcount);
}
