#include "GE/grammargenetic.h"
# define MAX_RULE 255

GrammarGenetic::GrammarGenetic(int count,int size,IntervalProblem *p)
{
    problem = p;
    program = new Cprogram(p->getDimension(),1);
    selection_rate = 0.1;
    mutation_rate  = 0.05;
    gcount = count;
    gsize = size;
    chromosome.resize(count);
    children.resize(count);
    for(int i=0;i<count;i++)
    {
        chromosome[i].resize(size);
        children[i].resize(size);
        for(int j=0;j<size;j++)
            chromosome[i][j]=rand() % MAX_RULE;
    }
    fitnessArray.resize(count);
    generation = 0;
    generation = 0;
    drandDat.resize(10 * nsamples*problem->getDimension());
    for (unsigned i = 0; i < drandDat.size();i++) {
        drandDat[i]=randGen.generateDouble();
    }
}

void    GrammarGenetic::setSelectionRate(double r)
{
    selection_rate = r;
}

void    GrammarGenetic::setMutationRate(double r)
{
    mutation_rate = r;
}

void    GrammarGenetic::setMaxGenerations(int m)
{
    maxGenerations = m;
}

double  GrammarGenetic::getSelectionRate() const
{
    return selection_rate;
}

double  GrammarGenetic::getMutationRate() const
{
    return mutation_rate;
}

int     GrammarGenetic::getMaxGenerations() const
{
    return maxGenerations;
}

Interval  GrammarGenetic::fitness(IDATA &genome)
{
    vector<Interval> oldMargin = problem->getMargins();
    vector<Interval> newMargin = problem->getMargins();
    int redo = 1;


    string st = program->printRandomProgram(genome,redo);
      double miny=1e+100,maxy=1e+100;
    if(!program->evalProgram(st,oldMargin,newMargin))
        return Interval(miny,maxy);
    Data trialx;
    trialx.resize(problem->getDimension());

    for(int k=1;k<=nsamples;k++)
    {

        for(int i=0;i<(int)trialx.size();i++)
        {


            trialx[i]=newMargin[i].leftValue()+
                    (newMargin[i].rightValue()-newMargin[i].leftValue())*drandDat[(k-1)*problem->getDimension()+i];
        }
        double fx= problem->funmin(trialx);
        if(k==1 || fx>maxy) maxy=fx;
        if(k==1 || fx<miny) miny=fx;
    }
    return Interval(miny,maxy);
}

void    GrammarGenetic::calcFitnessArray()
{
    for(int i=0;i<gcount;i++)
        fitnessArray[i]=fitness(chromosome[i]);
}

void    GrammarGenetic::select()
{
    vector<int> tx;
    tx.resize(gsize);

    for(int i=0;i<gcount;i++)
    {
        for(int j=0;j<gcount-1;j++)
        {
            if( problem->lowerValue(fitnessArray[j+1],fitnessArray[j]))
            {
                Interval dtemp = fitnessArray[j];
                fitnessArray[j]=fitnessArray[j+1];
                fitnessArray[j+1]=dtemp;
                tx = chromosome[j];
                chromosome[j]=chromosome[j+1];
                chromosome[j+1]=tx;
            }
        }
    }
}

int     GrammarGenetic::getParent()
{
    int tsize = 8;
    Interval dmin(1e+100,1e+100);
    int index =-1;
    for(int i=0;i<tsize;i++)
    {
        int randPos = rand() % gcount;
        if(i==0 || problem->lowerValue(fitnessArray[randPos],dmin))
        {
            dmin = fitnessArray[randPos];
            index = randPos;
        }
    }
    return index;
}
void    GrammarGenetic::crossover()
{
    int children_size=(int)((1.0-selection_rate)*gcount);
    int children_count=0;
    while(true)
    {
        int parent1=getParent();
        int parent2=getParent();
        unsigned size=gsize;

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

         children_count+=2;
         if(children_count>=children_size) break;
     }
     for(int i=0;i<children_count;i++)
     {
         chromosome[chromosome.size()-i-1]=children[i];
     }
}

void    GrammarGenetic::mutate()
{
    int start = 1;
    for(int i=start;i<gcount;i++)
    {
        for(int j=0;j<gsize;j++)
        {
            double r= rand() *1.0/RAND_MAX;
            if(r<mutation_rate)
                chromosome[i][j]=rand() % MAX_RULE;
        }
    }
}

void    GrammarGenetic::nextGeneration()
{
    if(generation) mutate();
    calcFitnessArray();
    select();
    crossover();
    ++generation;
    printf("Generation=%4d Best Value=[%20.10lg,%20.10lg]\n",
           generation,fitnessArray[0].leftValue(),
            fitnessArray[0].rightValue());
   //if(generation%5==0)
    {
        IntervalData xpoint;
        Interval ypoint;
        getBest(xpoint,ypoint);
        problem->setMargins(xpoint);
    }
    if(generation%20==0)
    {
        int count = 20;

        for(int i=0;i<count;i++)
            localSearch(rand() % chromosome.size());
        select();
    }
}

void    GrammarGenetic::Solve()
{
    generation = 0;
    drandDat.resize(10 * nsamples*problem->getDimension());
    for (unsigned i = 0; i < drandDat.size();i++) {
        drandDat[i]=randGen.generateDouble();
    }
    do
    {
        nextGeneration();
    }while(generation<=maxGenerations);
}

Data    GrammarGenetic::getBestPoint()
{
    Data bestx;
    Data trialx;
    double miny=1e+100;
    IntervalData xx;
    Interval fminy;
    getBest(xx,fminy);
    trialx.resize(xx.size());

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
void GrammarGenetic::getBest(IntervalData &x,Interval &y)
{
    x.resize(problem->getDimension());
    IntervalData oldMargin = problem->getMargins();
    int redo  =1;
    string st = program->printRandomProgram(chromosome[0],redo);
    program->evalProgram(st,oldMargin,x);
    y=fitnessArray[0];
}

void    GrammarGenetic::localSearch(int pos)
{
    int genome_size  = chromosome[0].size();
    vector<int> g;
    g.resize(genome_size);
    for(int i=0;i<genome_size;i++) g[i]=chromosome[pos][i];
    int genome_count = chromosome.size();

    for(int iters=1;iters<=100;iters++)
    {
        int gpos=rand() % genome_count;
        int cutpoint=rand() % genome_size;
        for(int j=0;j<cutpoint;j++) g[j]=chromosome[pos][j];
        for(int j=cutpoint;j<genome_size;j++) g[j]=chromosome[gpos][j];
                Interval fx=fitness(g);
                if(problem->lowerValue(fx,fitnessArray[pos]))
        {
            printf("NEW MIN[%4d]=[%10.4lg,%10.4lg]\n",pos,fx.leftValue(),fx.rightValue());
            for(int j=0;j<genome_size;j++) chromosome[pos][j]=g[j];
            fitnessArray[pos]=fx;
        }
        else
        {
            for(int j=0;j<cutpoint;j++) g[j]=chromosome[gpos][j];
            for(int j=cutpoint;j<genome_size;j++) g[j]=chromosome[pos][j];
                    fx=fitness(g);
                    if(problem->lowerValue(fx,fitnessArray[pos]))
            {
            printf("NEW MIN[%4d]=[%10.4lg,%10.4lg]\n",pos,fx.leftValue(),fx.rightValue());
                for(int j=0;j<genome_size;j++) chromosome[pos][j]=g[j];
                fitnessArray[pos]=fx;
            }
        }
    }
}

GrammarGenetic::~GrammarGenetic()
{
    delete program;
}
