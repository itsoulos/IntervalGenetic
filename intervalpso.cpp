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

void           IntervalPso::Solve()
{
    for(generationCount=1;generationCount<=500;generationCount++)
    {
        updatePositions();
        updateVelocities();
        double v=problem->getVolume(bestx);
        v=v/problem->getDimension();
        printf("print bestx \n");
        for(int i=0;i<problem->getDimension();i++)
        cout<<">> "<<bestx[i]<<endl;
        printf("SOLUTION VOLUME IS %lf\n",v);
        cout<<"best y ="<<besty<<endl;

    }
    besty=psoLocalSearch(bestx,100);
    printf("print bestx \n");
    for(int i=0;i<problem->getDimension();i++)
    cout<<">> "<<bestx[i]<<endl;

    cout<<"best y ="<<besty<<endl;
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
 //   if(rand()%10<=8) return problem->IntFunmin(x);
    IntervalData left=x;
    IntervalData right=x;
    IntervalData trialx=x;
    for(int i=0;i<x.size();i++)
    {
        left[i]=x[i].left();
        right[i]=x[i].right();
      //  x[i].split(left[i],right[i]);
    }
    Interval mv;//=problem->IntFunmin(x);
    for(int i=0;i<x.size();i++)
    {
       Interval t=x[i];
        x[i]=left[i];
        Interval mf;//=problem->IntFunmin(x);
        if(!problem->lowerValue(mf,mv))
            x[i]=t;
        else
            mv=mf;
        t=x[i];
        x[i]=right[i];
        //mf=problem->IntFunmin(x);
        if(!problem->lowerValue(mf,mv))
            x[i]=t;
        else
            mv=mf;
    }
    return mv;//(rand()%2==0)?2:5);
}

void           IntervalPso::updateVelocities()
{
    double inertia=0.50+psoRand()/2.0;
    double c1=1.0,c2=1.0;
    for(int i=0;i<particleCount;i++)
    {
        for(int j=0;j<problem->getDimension();j++)
        {/*
            Interval oldVel=velocity[i][j];
           velocity[i][j]=inertia * velocity[i][j]+c1 * psoRand()*
                   (bestPosition[i][j]-position[i][j])
                   +c2 * psoRand()*(bestx[i]-position[i][j]);

           if(velocity[i][j].is_empty())
                velocity[i][j]=oldVel;
            if(velocity[i][j].is_empty())
            {
                double r=psoRand();
                velocity[i][j]=interval(r,r);
            }*/

        }
    //   problem->boundInterval(velocity[i]);
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

          //  position[i][j]=position[i][j]+velocity[i][j];
        }

        if(!problem->isPointIn(position[i]))
        {
            failcount++;
            position[i]=oldPos;

            continue;
        }
            fitnessArray[i]=psoFitness(position[i]);
        if(problem->lowerValue(fitnessArray[i],bestFitnessArray[i]))
        {
            bestPosition[i]=position[i];
            bestFitnessArray[i]=fitnessArray[i];
        }

        if(problem->lowerValue(bestFitnessArray[i],besty))
        {

            besty=bestFitnessArray[i];
            bestx=bestPosition[i];
        }
    }
    printf("failcount = %d \n",failcount);
}
