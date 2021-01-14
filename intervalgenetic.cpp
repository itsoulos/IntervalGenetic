#include "intervalgenetic.h"
# include <problem.h>
# include <tolmin.h>
#include <omp.h>

#include "dllproblem.h"
IntervalGenetic::IntervalGenetic(IntervalProblem *p,int gcount)
    : t(1)
{
    omp_set_dynamic(0);
    extern int threads;
    omp_set_num_threads(threads);
    problem = new IntervalProblem*[t];
    for(unsigned i = 0; i < t;i++) problem[i] = &(p[i]);
    count=gcount;
    generation=0;
    setSelectionRate(0.1);
    setMutationRate(0.05);
    fitnessArray.resize(count);
    chromosome.resize(count);
    children.resize(count);
    setSamples(50);
    for(int i=0;i<count;i++)
    {
        chromosome[i].resize(problem[omp_get_thread_num()]->getDimension());
        children[i].resize(problem[omp_get_thread_num()]->getDimension());

        chromosome[i]=problem[omp_get_thread_num()]->createRandomInterval();
        fitnessArray[i]=fitness(chromosome[i]);

    }

}

void    IntervalGenetic::setSamples(int n)
{
    nsamples=n;
    drandDat.resize(
                nsamples*problem[omp_get_thread_num()]->getDimension());

    for (unsigned i = 0; i < drandDat.size();i++) {
        drandDat[i]=randGen.generateDouble();
    }
}

IntervalGenetic::IntervalGenetic(QString filename,QJsonObject settings,int gcount,unsigned Threads) :t(Threads){
    //omp_set_dynamic(0);
    omp_set_num_threads(Threads);
    problem = new IntervalProblem*[t];
    for(unsigned i = 0; i < t;i++) problem[i] = new DllProblem(filename,settings);
    count=gcount;
    generation=0;
    setSelectionRate(0.1);
    setMutationRate(0.05);
    nsamples=50;
    drandDat.resize(nsamples*problem[omp_get_thread_num()]->getDimension());
    for (unsigned i = 0; i < drandDat.size();i++) {
        drandDat[i]=randGen.generateDouble();
    }
    fitnessArray.resize(count);
    chromosome.resize(count);
    children.resize(count);

    for(int i=0;i<count;i++)
    {
        chromosome[i].resize(problem[omp_get_thread_num()]->getDimension());
        children[i].resize(problem[omp_get_thread_num()]->getDimension());

        chromosome[i]=problem[omp_get_thread_num()]->createRandomInterval();
        fitnessArray[i]=fitness(chromosome[i]);

    }
}

void   IntervalGenetic::setSelectionRate(double r)
{
    if(r>=0.0 && r<=1.0)
        selection_rate=r;
}

double IntervalGenetic::getSelectionRate() const
{
    return selection_rate;
}

void   IntervalGenetic::setMutationRate(double r)
{
    if(r>=0.0 && r<=1.0)
        mutation_rate=r;
}

double IntervalGenetic::getMutationRate() const
{
    return mutation_rate;
}

int    IntervalGenetic::getChromosomeCount() const
{
    return count;
}

int    IntervalGenetic::getGenerationCount() const
{
    return generation;
}


void   IntervalGenetic::optimize(IntervalData &x)
{
    const int N=20;
    Data trialx;
    trialx.resize(x.size());
    double miny=1e+100,maxy=1e+100;
    Problem np(problem[omp_get_thread_num()]);
    IntervalData copyx=x;
    for(int k=1;k<=N;k++)
    {
    for(int i=0;i<x.size();i++)
    {
        trialx[i]=x[i].leftValue()+(x[i].rightValue()-x[i].leftValue())*drandDat[(k-1)*np.getDimension()+i];
    }
    double fx=tolmin(trialx,&np,100);

    for(int i=0;i<x.size();i++)
    {
        double d1=copyx[i].leftValue();
        double d2=copyx[i].rightValue();
        if(trialx[i]<d1) d1=trialx[i];
        if(trialx[i]>d2) d2=trialx[i];
        if(d2<d1)
        {
            double t=d1;
            d1=d2;
            d2=t;
        }
        copyx[i]=Interval(d1,d2);
    }
    if(k==1 || fx>maxy) maxy=fx;
    if(k==1 || fx<miny) miny=fx;
    }
     x=copyx;
}

Interval IntervalGenetic::fitness(IntervalData &x,unsigned ti)
{
    Data trialx;
    trialx.resize(x.size());
    double miny=1e+100,maxy=1e+100;
    Problem np(problem[ti]);
    for(int k=1;k<=nsamples;k++)
    {
    for(int i=0;i<x.size();i++)
    {
        trialx[i]=x[i].leftValue()+(x[i].rightValue()-x[i].leftValue())*drandDat[(k-1)*np.getDimension()+i];
    }
    double fx=problem[ti]->funmin(trialx);//tolmin(trialx,&np,10);

    if(k==1 || fx>maxy) maxy=fx;
    if(k==1 || fx<miny) miny=fx;
    }
    return Interval(miny,maxy);
   // return problem[omp_get_thread_num()]->IntFunmin(x);
}

void   IntervalGenetic::calcFitnessArray()
{
    omp_set_nested(0);
#pragma omp parallel for schedule(dynamic)
    for(int i=0;i<count;i++)
    {
        fitnessArray[i]=fitness(chromosome[i],omp_get_thread_num());

    }
}

int    IntervalGenetic::tournament()
{
    const int NMAX=8;
    int maxPos=-1;
    Interval maxValue=Interval(1e+10,1e+10);
    for(int i=0;i<NMAX;i++)
    {
        int randPos=rand() % count;
        if(i==0 || problem[omp_get_thread_num()]->lowerValue(fitnessArray[randPos],maxValue))
        {
            maxPos=randPos;
            maxValue=fitnessArray[randPos];
        }
    }
    return maxPos;
}

void   IntervalGenetic::crossover()
{

    int children_size=(int)((1.0-selection_rate)*count);
    int children_count=0;
    while(true)
    {
        int parent1=tournament();
        int parent2=tournament();
        unsigned size=chromosome[0].size();
        //DOUBLE CROSSOVER
  /* for(int i=0;i<size;i++)
        {
            double alfa=-0.5+2.0*problem[omp_get_thread_num()]->randomDouble();
            Interval x1,x2;
            x1=chromosome[parent1][i];
            x2=chromosome[parent2][i];

            Interval g1=alfa*x1+(1.0-alfa)*x2;
            Interval g2=alfa*x2+(1.0-alfa)*x1;
            children[children_count][i]=g1;
            children[children_count+1][i]=g2;
        }
        problem[omp_get_thread_num()]->boundInterval(children[children_count]);
        problem[omp_get_thread_num()]->boundInterval(children[children_count+1]);
*/
        //ONE POINT CROSSOVER
    int cutPoint=rand() % size;
        for(int i=0;i<cutPoint;i++)
        {
            children[children_count][i]=chromosome[parent2][i];
            children[children_count+1][i]=chromosome[parent1][i];
        }
        for(int i=cutPoint;i<size;i++)
        {
            children[children_count][i]=chromosome[parent1][i];
            children[children_count+1][i]=chromosome[parent2][i];
        }

        //UNIFORM CROSSOVER
      /* for(int i=0;i<size;i++)
        {
            children[children_count][i]=Interval(chromosome[parent1][i].leftValue(),
                                              chromosome[parent2][i].rightValue());

            children[children_count+1][i]=Interval(chromosome[parent2][i].leftValue(),
                                              chromosome[parent1][i].rightValue());
        }*/

        children_count+=2;
        if(children_count>=children_size) break;
    }
    for(int i=0;i<children_count;i++)
    {
        chromosome[chromosome.size()-i-1]=children[i];
    }
}

void   IntervalGenetic::mutate()
{
    int size=chromosome[0].size();
    for(unsigned i=1;i<chromosome.size();i++)
    {
        for(int j=0;j<size;j++)
        {
            double r=problem[omp_get_thread_num()]->randomDouble();
            if(r<mutation_rate)
            {
                problem[omp_get_thread_num()]->randomize(chromosome[i],j);
            }
        }
    }
}

void   IntervalGenetic::selection()
{
    IntervalData temp;
    Interval itemp;
    for(int i=0;i<count;i++)
    {
        for(int j=0;j<count-1;j++)
        {
            if(problem[omp_get_thread_num()]->lowerValue(fitnessArray[j+1],fitnessArray[j]))
            {
                temp=chromosome[j];
                chromosome[j]=chromosome[j+1];
                chromosome[j+1]=temp;
                itemp=fitnessArray[j];
                fitnessArray[j]=fitnessArray[j+1];
                fitnessArray[j+1]=itemp;
            }
        }
    }
}

void   IntervalGenetic::nextGeneration()
{
    if(generation) mutate();
    ++generation;
    calcFitnessArray();
    selection();
 crossover();

    if(generation%10==0)
    {

        int count=8;


        omp_set_nested(0);
#pragma omp parallel for schedule(dynamic)
        for(int i=1;i<=count;i++)
        {

           int randPos=(i-1)*(chromosome.size()/count) + randGen.generate() % (chromosome.size()/count);
           Interval fx=fitnessArray[randPos];
           localSearch(chromosome[randPos],fitnessArray[randPos]);
           cout<<"LOCALSEARCH["<<randPos<<"]="<<fx<<".....>"<<fitnessArray[randPos]<<
                    "FROM THREAD["<<omp_get_thread_num()<<"]"<<endl;

        }
 IntervalData mx=getMaximumInterval();
  
 for(int j=0;j<omp_get_max_threads();j++)
       problem[j]->setMargins(mx);
    }
 selection();
}


IntervalData IntervalGenetic::getMaximumInterval()
{
    IntervalData d=chromosome[0];
    for(int i=0;i<count;i++)
    {
        for(int j=0;j<problem[omp_get_thread_num()]->getDimension();j++)
        {
            double d1=d[j].leftValue(),d2=d[j].rightValue();
            if(chromosome[i][j].leftValue()<=d[j].leftValue())
                d1=chromosome[i][j].leftValue();
            if(chromosome[i][j].rightValue()>=d[j].rightValue())
                d2=chromosome[i][j].rightValue();
            d[j]=Interval(d1,d2);
        }
    }
    return d;
}

void    IntervalGenetic::localSearch(IntervalData &x,Interval &value)
{
    Interval fL=Interval(0,0);
    Interval fR=Interval(0,0);

    for(int k=1;k<=2;k++)
    {
        bool minFound=false;
    for(int i=0;i<x.size();i++)
    {
       Interval temp=Interval(0,0);
       int ipos=rand() % x.size();
       temp=x[ipos];
       x[ipos]=x[ipos].left();
       Interval fL=fitness(x);
       if(problem[omp_get_thread_num()]->lowerValue(fL,value))
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
       Interval fR=fitness(x);
       if(problem[omp_get_thread_num()]->lowerValue(fR,value))
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
    cout<<"ITERATION "<<k<<" NEW VALUE "<<value<<endl;
    break;
 //  if (minFound) break;
    }

}

void   IntervalGenetic::getBest(IntervalData &bestx,Interval &besty)
{
    bestx=chromosome[0];
    besty=fitnessArray[0];
}

Data    IntervalGenetic::getBestPoint()
{
    Data bestx;
    Data trialx;
    trialx.resize(chromosome[0].size());
    double miny=1e+100;
    Problem np(problem[0]);
    for(int k=1;k<=nsamples;k++)
    {
    for(int i=0;i<chromosome[0].size();i++)
    {
        trialx[i]=chromosome[0][i].leftValue()+(chromosome[0][i].rightValue()-chromosome[0][i].leftValue())*drandDat[(k-1)*np.getDimension()+i];
    }
    double fx=problem[0]->funmin(trialx);//tolmin(trialx,&np,10);

    if(k==1 || fx<miny)
    {
        miny=fx;
        bestx=trialx;
    }}
    return bestx;
}

IntervalGenetic::~IntervalGenetic()
{
    //nothing here
}
