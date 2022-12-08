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
	   
	   double delta = position[i][j].width()/20.0;
           velocity[i][j]=Interval(-psoRand()*delta,psoRand()*delta);

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
        //printf("FINAL MINY = %20.10lg\n",miny);

        miny=fx;
        bestx1=trialx;
    }}
    //printf("FINAL MINY = %20.10lg\n",miny);
    return bestx1;
}

void           IntervalPso::Solve()
{
    for(generationCount=1;generationCount<=100;generationCount++)
    {
        updatePositions();
        updateVelocities();
	printf("ITERATION =%5d BESTY=[%lf,%lf]\n",generationCount,besty.leftValue(),besty.rightValue());
	if(generationCount%10==0)
	{
		besty = psoLocalSearch(bestx,50);
		if(2==3)
		for(int i=0;i<20;i++)
		{
			int pos = rand() %particleCount;
			fitnessArray[pos]=psoLocalSearch(position[pos],50);
        		if(problem->lowerValue(fitnessArray[pos],bestFitnessArray[pos]))
        		{
            			bestPosition[pos]=position[pos];
           			 bestFitnessArray[pos]=fitnessArray[pos];

	    			printf("LOCAL UPDATE %d to [%lf,%lf]\n",pos,bestFitnessArray[pos].leftValue(),
				bestFitnessArray[pos].rightValue());
        			if(problem->lowerValue(bestFitnessArray[pos],besty))
        			{

            				besty=bestFitnessArray[pos];
            				bestx=bestPosition[pos];
        			}
        		}

		}
	}

    }

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
    Interval fL=Interval(0,0);
    Interval fR=Interval(0,0);

    Interval value =psoFitness(x);
    for(int k=1;k<=iters;k++)
    {
        bool minFound=false;
    for(int i=0;i<x.size();i++)
    {
       Interval temp=Interval(0,0);
       int ipos=rand() % x.size();
       temp=x[ipos];
       x[ipos]=x[ipos].left();
       Interval fL=psoFitness(x);
       if(problem->lowerValue(fL,value))
       {

           value=fL;
           minFound=true;
         break;
       }
       else
       {
           x[ipos]=temp;
       }

       temp=x[ipos];
       x[ipos]=x[ipos].right();
       Interval fR=psoFitness(x);
       if(problem->lowerValue(fR,value))
       {

           value=fR;
           minFound=true;
         break;
       }
       else
       {
           x[ipos]=temp;
       }
    }
//    cout<<"ITERATION "<<k<<" NEW VALUE "<<value<<endl;
   // break;
   
 //  if (minFound) break;
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
    const double wmax =0.9;
    const double wmin =0.5;
    const int maxgenerations = 100;
//    inertia = wmax - generationCount *1.0/maxgenerations*(wmax-wmin);

    for(int i=0;i<particleCount;i++)
    {
	    double norm = 0.0;
	    for(int j=0;j<problem->getDimension();j++)
	    {
		    norm+=pow(position[i][j].leftValue(),2.0);
	    }
	    norm = sqrt(norm);
	    norm = 1000.0;
        for(int j=0;j<problem->getDimension();j++)
        {
            Interval oldVel=velocity[i][j];
           velocity[i][j]=inertia * velocity[i][j]+c1 * psoRand()*
                   (bestPosition[i][j]-position[i][j])
                   +c2 * psoRand()*(bestx[i]-position[i][j]);
           velocity[i][j]=Interval(velocity[i][j].leftValue()/norm,
                                  velocity[i][j].rightValue()/norm);
           //printf("velocity %lf %lf\n",
           //       velocity[i][j].leftValue(),
           //       velocity[i][j].rightValue());

        /*   if(velocity[i][j].is_empty())
                velocity[i][j]=oldVel;
            if(velocity[i][j].is_empty())
            {
                double r=psoRand();
                velocity[i][j]=interval(r,r);
            }*/

        }
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
       problem->boundInterval(position[i]);

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
            bestx=bestPosition[i];
        }
    }
}
