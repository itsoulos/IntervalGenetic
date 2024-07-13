#include "integergenetic.h"
#include <omp.h>
# include <tolmin.h>


IntegerGenetic::IntegerGenetic(IntervalProblem *p,int count,int t)
{
    problem=p;
    gcount=count;
    generation=0;
    threads=t;
    nsamples=500;
    genome.resize(gcount);
    children.resize(gcount);
    setSelectionRate(0.1);
    setMutationRate(0.05);
    fitnessArray.resize(gcount);
    tempx.resize(problem->getDimension());
    setSamples(nsamples);

    for(int i=0;i<gcount;i++)
    {
        genome[i].resize( 2*maxDepth*problem->getDimension());
        for(int j=0;j<genome[i].size();j++) genome[i][j]=rand() %2;
        children[i].resize(genome[i].size());
    //    fitnessArray[i]=fitness(genome[i]);
    }

}



 void IntegerGenetic::setSamples(int n)
 {
     nsamples=n;
     drandDat.resize(10 * nsamples*problem->getDimension());
     for (unsigned i = 0; i < drandDat.size();i++) {
         drandDat[i]=randGen.generateDouble();
     }
 }
Data    IntegerGenetic::getBestPoint()
{
    Data bestx;
    Data trialx;
    double miny=1e+100;
    IntervalData xx=produceInterval(genome[0]);
    trialx.resize(xx.size());
    Problem np(problem);

    for(int k=1;k<=nsamples;k++)
    {
    for(int i=0;i<xx.size();i++)
    {
        trialx[i]=xx[i].leftValue()+(xx[i].rightValue()-xx[i].leftValue())*drandDat[(k-1)*problem->getDimension()+i];
    }
    double fx=problem->funmin(trialx);//tolmin(trialx,&np,10);

    if(k==1 || fx<miny)
    {
        miny=fx;
        bestx=trialx;
    }}
    return bestx;
}
Interval IntegerGenetic::fitness(IDATA &x)
{
    IntervalData xx=produceInterval(x);
    Data trialx;
    Problem np(problem);
    trialx.resize(xx.size());
    double miny=1e+100,maxy=1e+100;
    for(int k=1;k<=nsamples;k++)
    {
        for(int i=0;i<xx.size();i++)
        {
            trialx[i]=xx[i].leftValue()+(xx[i].rightValue()-xx[i].leftValue())*drandDat[(k-1)*problem->getDimension()+i];
        }
        double fx= problem->funmin(trialx);//tolmin(trialx,&np,10);// problem->funmin(trialx);
//        double fx= tolmin(trialx,&np,5);// problem->funmin(trialx);
        if(k==1 || fx>maxy) maxy=fx;
        if(k==1 || fx<miny) miny=fx;
    }
    return Interval(miny,maxy);
}

void  IntegerGenetic::calcFitnessArray()
{
//#pragma omp parallel for num_threads(threads) schedule(static)

    //omp_set_nested(0);
//#pragma omp parallel for schedule(dynamic)
	double dmin=1e+100;
    for(int i=0;i<gcount;i++)
    {
        fitnessArray[i]=fitness(genome[i]);
	if(fitnessArray[i].leftValue()<dmin)
		dmin=fitnessArray[i].leftValue();
	if(i%20==0)
		printf("f[%d]=%10.5lg\n",i,dmin);
    }
}

IntervalData IntegerGenetic::produceInterval(IDATA &x)
{
    int icount=0;
    IntervalData tempx1=problem->getMargins();
    IntervalData m=problem->getMargins();
    for(int j=0;j<maxDepth;j++)
    {
    for(int i=0;i<problem->getDimension();i++)
    {
         int l,r;
            l=x[icount++];
            r=x[icount++];
            double d1=tempx1[i].leftValue();
            double d2=tempx1[i].rightValue();
            if(l==1) d1=d1/2;
            if(d1<m[i].leftValue()) d1=tempx1[i].leftValue();
            if(r==1)
            {
                d2=d2/2;
                if(d2<m[i].leftValue())
                    d2=tempx1[i].rightValue();
            }
            if(d2<d1)
            {
                double t=d1;
                d1=d2;
                d2=t;
            }
            tempx1[i]=Interval(d1,d2);
        }
    }
    return tempx1;
}

void IntegerGenetic::mutate()
{
    for(int i=1;i<gcount;i++)
    {
       int size=genome[0].size();
       for(int j=0;j<size;j++)
       {
           double r=rand() *1.0/RAND_MAX;
           if(r<=mutation_rate)
           {
               int k=genome[i][j];
               if(k==1) genome[i][j]=0; else genome[i][j]=1;
           }
       }
    }
}
void IntegerGenetic::selection()
{
    IDATA temp;
    Interval itemp;
    for(int i=0;i<gcount;i++)
    {
        for(int j=0;j<gcount-1;j++)
        {
            if(problem->lowerValue(fitnessArray[j+1],fitnessArray[j]))
            {
                temp=genome[j];
                genome[j]=genome[j+1];
                genome[j+1]=temp;
                itemp=fitnessArray[j];
                fitnessArray[j]=fitnessArray[j+1];
                fitnessArray[j+1]=itemp;
            }
        }
    }
}

IntervalData IntegerGenetic::getMaximumInterval()
{
    IntervalData d=produceInterval( genome[0]);
    for(int i=0;i<(int)genome.size();i++)
    {
        IntervalData t=produceInterval(genome[i]);
        for(int j=0;j<problem->getDimension();j++)
        {
            double d1=d[j].leftValue(),d2=d[j].rightValue();
            if(t[j].leftValue()<=d1)
                d1=t[j].leftValue();
            if(t[j].rightValue()>=d2)
                d2=t[j].rightValue();
            if(d2<d1)
            {
                double tt=d1;
                d1=d2;
                d2=tt;
            }
            d[j]=Interval(d1,d2);
        }

    }
    return d;
}


void IntegerGenetic::crossover()
{
    int children_size=(int)((1.0-selection_rate)*gcount);
    int children_count=0;
    while(true)
    {
        int parent1=tournament();
        int parent2=tournament();
        unsigned size=genome[0].size();
     /*for(int i=0;i<size;i++)
         {
             int r=rand()%2;
             if(r==0)
             {
                 children[children_count][i]=genome[parent2][i];
                 children[children_count+1][i]=genome[parent1][i];
             }
             else
             {
                 children[children_count][i]=genome[parent1][i];
                 children[children_count+1][i]=genome[parent2][i];
             }

         }
     */
    
        //ONE POINT CROSSOVER
      int cutPoint=rand() % size;
        for(int i=0;i<cutPoint;i++)
        {
            children[children_count][i]=genome[parent2][i];
            children[children_count+1][i]=genome[parent1][i];
        }
        for(int i=cutPoint;i<size;i++)
        {
            children[children_count][i]=genome[parent1][i];
            children[children_count+1][i]=genome[parent2][i];
        }

         children_count+=2;
         if(children_count>=children_size) break;
     }
     for(int i=0;i<children_count;i++)
     {
         genome[genome.size()-i-1]=children[i];
     }
}

int IntegerGenetic::tournament()
{
    const int NMAX=8;
    int maxPos=-1;
    Interval maxValue=Interval(1e+10,1e+10);
    for(int i=0;i<NMAX;i++)
    {
        int randPos=rand() % gcount;
        if(i==0 || problem->lowerValue(fitnessArray[randPos],maxValue))
        {
            maxPos=randPos;
            maxValue=fitnessArray[randPos];
        }
    }
    return maxPos;
}

void IntegerGenetic::setSelectionRate(double r)
{
    if(r>=0 && r<=1) selection_rate=r;
}

double IntegerGenetic::getSelectionRate() const
{
    return selection_rate;
}

void IntegerGenetic::setMutationRate(double r)
{
    if(r>=0 && r<=1)
        mutation_rate=r;
}

double IntegerGenetic::getMutationRate() const
{
    return  mutation_rate;
}

void IntegerGenetic::nextGeneration()
{
    if(generation) mutate();
    calcFitnessArray();
    selection();
    crossover();
    ++generation;
    
    if(generation%50==0)
    {
	    int count = 50;
//#pragma omp parallel for schedule(dynamic)
	    for(int i=0;i<count;i++)
		    localSearch(rand() % genome.size());
	    selection();
    }
    if(generation%2000==0)
    {
        int icount=10;
        for(int i=0;i<icount;i++)
        {
            int randPos=(i==0)?0:rand() % gcount;
            int maxChanges=genome[randPos].size()/10;
            for(int j=0;j<maxChanges;j++)
            {
                int pos=rand() % genome[randPos].size();
                int k=genome[randPos][pos];
                genome[randPos][pos]=rand()%2;
                Interval fx=fitness(genome[randPos]);
                if(problem->lowerValue(fx,fitnessArray[randPos]))
                   {
                    fitnessArray[randPos]=fx;
                   }
                else
                    genome[randPos][pos]=k;
            }
        }
	
        selection();
        IntervalData mx=getMaximumInterval();
        problem->setMargins(mx);
        calcFitnessArray();
        selection();
    }
}

void IntegerGenetic::getBest(IntervalData &x,Interval &y)
{
    x.resize(problem->getDimension());
    x=produceInterval(genome[0]);
    y=fitnessArray[0];
}

IntegerGenetic::~IntegerGenetic()
{

}
void	IntegerGenetic::localSearch(int pos)
{
	int genome_size  = genome[0].size();
	vector<int> g;
	g.resize(genome_size);
	for(int i=0;i<genome_size;i++) g[i]=genome[pos][i];
	int genome_count = genome.size();
	
	for(int iters=1;iters<=100;iters++)
	{
		int gpos=rand() % genome_count;
		int cutpoint=rand() % genome_size;
		for(int j=0;j<cutpoint;j++) g[j]=genome[pos][j];
		for(int j=cutpoint;j<genome_size;j++) g[j]=genome[gpos][j];
                Interval fx=fitness(g);
                if(problem->lowerValue(fx,fitnessArray[pos]))
		{
			printf("NEW MIN[%4d]=[%10.4lg,%10.4lg]\n",pos,fx.leftValue(),fx.rightValue());
			for(int j=0;j<genome_size;j++) genome[pos][j]=g[j];
			fitnessArray[pos]=fx;
		}
		else
		{
			for(int j=0;j<cutpoint;j++) g[j]=genome[gpos][j];
			for(int j=cutpoint;j<genome_size;j++) g[j]=genome[pos][j];
                	fx=fitness(g);
                	if(problem->lowerValue(fx,fitnessArray[pos]))
			{
			printf("NEW MIN[%4d]=[%10.4lg,%10.4lg]\n",pos,fx.leftValue(),fx.rightValue());
				for(int j=0;j<genome_size;j++) genome[pos][j]=g[j];
				fitnessArray[pos]=fx;
			}
		}
	}
}
		
