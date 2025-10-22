# include <doublepop.h>
# include <stdlib.h>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <tolmin.h>
# include <QString>
# include <omp.h>
//# define LOCALSEARCH

# define MAXTHREADS 12
int have_finished=0;
#pragma omp threadprivate(have_finished)
int iprint=1;
#pragma omp threadprivate(iprint)


extern "C"
{
long iterm()
{
	return have_finished;
}
}

//# define LAPLACE_CROSSOVER
# define DOUBLE_CROSSOVER
//# define UNIFORM_CROSSOVER
//# define RANDOM_MUTATION
# define DOUBLE_MUTATION
//# define PSO_MUTATION
//# define MPT_MUTATION

static void LocalSearch(Problem *p,double *x,double &y);

static double diff(double *x,double *y,int n)
{
	double s=0.0;
	for(int i=0;i<n;i++) s+=pow(x[i]-y[i],2.0);
	return sqrt(s);
}

extern QString  intervalMethod;
DoublePop::DoublePop(int gcount,Problem *p)
{
	problem = p;
	genome_size=p->getDimension();
	elitism=1;
	selection_rate = 0.1;
	mutation_rate  = 0.05;
	genome_count   = gcount;
	generation     = 0;

	genome.resize(genome_count);
	children.resize(genome_count);
	parent0.resize(genome_size);
	parent1.resize(genome_size);
	bestgenome.resize(genome_count);
	for(int i=0;i<genome_count;i++)
	{
		genome[i].resize(genome_size);
		children[i].resize(genome_size);
		if(intervalMethod=="none")
		{
			genome[i]=problem->getUniformRandomPoint();
		}
		else
		{
        genome[i]=problem->getRandomPoint();}
		bestgenome[i].resize(genome_size);
		bestgenome[i]=genome[i];
	}
	fitness_array.resize(genome_count);
	bestfitness_array.resize(genome_count);
	for(int i=0;i<genome_count;i++) bestfitness_array[i]=-1e+100;
	lmargin.resize(p->getDimension());
	rmargin.resize(p->getDimension());
    lmargin=problem->getLeftMargin();
    rmargin=problem->getRightMargin();
}

void    DoublePop::setLocalIterations(int iters)
{
    LI  = iters;
}
void    DoublePop::setLocalChromosomes(int g)
{
    LC= g;
}
void    DoublePop::setMaxGenerations(int g)
{
    maxGenerations=g;
}
void	DoublePop::reset()
{
}

double 	DoublePop::fitness(vector<double> &g)
{
    if(!problem->isPointIn(g)) return 1e+100;
    double v=problem->funmin(g);
	return v;
}

void	DoublePop::select()
{
	Data itemp;
	itemp.resize(genome_size);
	for(int i=0;i<genome_count;i++)
	{
		for(int j=0;j<genome_count-1;j++)
		{
            if(fitness_array[j+1]<fitness_array[j])
			{
				double dtemp;
				dtemp=fitness_array[j];
				fitness_array[j]=fitness_array[j+1];
				fitness_array[j+1]=dtemp;

				itemp=genome[j];
				genome[j]=genome[j+1];
				genome[j+1]=itemp;
				
				itemp=bestgenome[j];
				bestgenome[j]=bestgenome[j+1];
				bestgenome[j+1]=itemp;

				dtemp=bestfitness_array[j];
				bestfitness_array[j]=bestfitness_array[j+1];
				bestfitness_array[j+1]=dtemp;
			}
		}
	}
}

void	DoublePop::getTournamentElement(Data &x)
{
        const int tournament_size =(genome_count<=100)?4:10;
    double max_fitness=1e+100;
	int    max_index=-1;
        for(int j=0;j<tournament_size;j++)
        {
		int r=rand() % (genome_count);
                if(j==0 || fitness_array[r]<max_fitness)
                {
                        max_index=r;
                        max_fitness=fitness_array[r];
                }
        }
	x=genome[max_index];
}

void	DoublePop::tournament(Data &p1,Data &p2)
{
	getTournamentElement(p1);
	getTournamentElement(p2);
}

void	DoublePop::crossover()
{
        int nchildren=(int)((1.0 - selection_rate) * genome_count);
	if(!(nchildren%2==0)) nchildren++;
        int count_children=0;
	int total_success=0;
        while(1)
        {
		tournament(parent0,parent1);
		for(int i=0;i<genome_size;i++)
		{
			double alfa,b,u,g1,g2;
			double x1,x2;
			int p1,p2;
			x1=parent0[i];
			x2=parent1[i];
#ifdef LAPLACE_CROSSOVER
            u=problem->randomDouble();
			alfa=0.0;
			if(u<=0.5) b=alfa-b*log(u); else b=alfa+b*log(u);
			g1=x1+b*fabs(x1-x2);
			g2=x2+b*fabs(x1-x2);
#endif
#ifdef DOUBLE_CROSSOVER	
        alfa=-0.5+2.0*problem->randomDouble();
	g1=alfa*x1+(1.0-alfa)*x2;
	g2=alfa*x2+(1.0-alfa)*x1;
#endif
#ifdef UNIFORM_CROSSOVER
            alfa=problem->randomDouble();
			if(alfa<0.5)
			{
				g1=x1;
				g2=x2;
			}
			else
			{
				g2=x1;
				g1=x2;
			}
#endif	
			if(g1>rmargin[i] || g1<lmargin[i])  g1=x1;
			if(g2>rmargin[i] || g2<lmargin[i])  g2=x2;

			children[count_children][i]=g1;
			children[count_children+1][i]=g2;			
		}
		count_children+=2;
		if(count_children>=nchildren) break;
	}
	for(int i=0;i<nchildren;i++)
	{
		genome[genome_count-i-1]=children[i];
	}
}

void	DoublePop::setElitism(int s)
{
	elitism = s;
}


void	DoublePop::mutate()
{
	int start = elitism * (int)(genome_count*selection_rate);
	start = elitism;
	const double b=(1.0-generation*1.0/maxGenerations)*5.0;
	for(int i=start;i<genome_count;i++)
	{
		for(int j=0;j<genome_size;j++)
		{
            double r=problem->randomDouble();
			if(r<mutation_rate)
			{
				double old=genome[i][j];
#ifdef MPT_MUTATION
				double t,t2,rk,beta=5.0;
				t=(genome[i][j]-lmargin[j])/(rmargin[j]-genome[i][j]);
				do
				{
                    rk=problem->randomDouble();
					if(rk<t) t2=t-t*pow((t-rk)/t,beta);
					else
					if(rk==t) t2=t;
					else
					t2=t+(1.0-t)*pow((rk-t)/(1.0-t),beta);
					genome[i][j]=(1.0-t2)*lmargin[j]+t2*rmargin[j];
				}while(genome[i][j]<lmargin[j] || genome[i][j]>rmargin[j]);
#endif
#ifdef PSO_MUTATION
				do{
                genome[i][j]=problem->randomDouble()*(bestgenome[i][j]-genome[i][j])+drand48()*(genome[0][j]-genome[i][j]);
				}while(genome[i][j]<lmargin[j]||genome[i][j]>rmargin[j]);
#endif
#ifdef RANDOM_MUTATION
				do{
                    genome[i][j]*=2.0*problem->randomDouble()-1.0;
				}while(genome[i][j]<lmargin[j]||genome[i][j]>rmargin[j]);
#endif
#ifdef DOUBLE_MUTATION
				double xi,yi;
				do{
                    xi=problem->randomDouble();
                    yi=problem->randomDouble();
				if(xi>0.5)
				{
				genome[i][j]=genome[i][j]+(rmargin[j]-genome[i][j])*(1.0-pow(yi,b));
				}
				else
				{
				genome[i][j]=genome[i][j]-(genome[i][j]-lmargin[j])*(1.0-pow(yi,b));
				}
				}while(genome[i][j]<lmargin[j]
					|| genome[i][j]>rmargin[j]);
#endif
			if(genome[i][j]<lmargin[j]||genome[i][j]>rmargin[j]) genome[i][j]=old;
			}
		}
	}
}

void	DoublePop::calcFitnessArray()
{
	double dmin=1e+100;
#pragma omp parallel for num_threads(MAXTHREADS) schedule(static) reduction(min:dmin)
	for(int i=0;i<genome_count;i++)
	{
		fitness_array[i]=fitness(genome[i]);
		if(fabs(fitness_array[i])<dmin) dmin=fabs(fitness_array[i]);

		if(fitness_array[i]>bestfitness_array[i])
		{
			bestfitness_array[i]=fitness_array[i];
			bestgenome[i]=genome[i];
		}
	}
}

int	DoublePop::getGeneration() const
{
	return generation;
}

int	DoublePop::getCount() const
{
	return genome_count;
}

int	DoublePop::getSize() const
{
	return genome_size;
}

void	DoublePop::nextGeneration()
{
	if(generation) mutate();
	calcFitnessArray();
	select();
	/*
	replaceWorst();
	select();
	**/
	crossover();
	++generation;
}

void	DoublePop::setMutationRate(double r)
{
	if(r>=0 && r<=1) mutation_rate = r;
}

void	DoublePop::setSelectionRate(double r)
{
	if(r>=0 && r<=1) selection_rate = r;
}

double	DoublePop::getSelectionRate() const
{
	return selection_rate;
}

double	DoublePop::getMutationRate() const
{
	return mutation_rate;
}

double	DoublePop::getBestFitness() const
{
	return  fitness_array[0];
}

Data	DoublePop::getBestGenome() const
{
	vector<double> g;g.resize(genome_size);
	for(int i=0;i<genome_size;i++) g[i]=genome[0][i];
	return g;
}
double	DoublePop::evaluateBestFitness() 
{
	vector<double> g;g.resize(genome_size);
	for(int i=0;i<genome_size;i++) g[i]=genome[0][i];	
	return fitness(g);
}

void	DoublePop::setBest(Data x,double y)
{
	for(int i=0;i<genome_size;i++)
		genome[0][i]=x[i];
	fitness_array[0]=y;
}

double	DoublePop::getFitness(int pos) const
{
	return fitness_array[pos];
}

void	DoublePop::getMinimum(Data &x,double &y)
{
	for(int i=0;i<genome_size;i++)
		x[i]=genome[0][i];
	y=fitness_array[0];
}

static void LocalSearch(Problem *p,Data &x,double &y)
{
    if(x.size()>1000) return ;
    y=tolmin(x,p,2001);
}

double	DoublePop::estimateVariance()
{
	double x1=0.0;
	double x2=0.0;
	for(int i=0;i<genome_count;i++)
	{
		x1+=fitness_array[i];
		x2+=fitness_array[i]*fitness_array[i];
	}
	return sqrt(x2/genome_count-(x1/genome_count)*(x1/genome_count));
}

void	DoublePop::replaceWorst()
{
    Data xtrial;
	xtrial.resize(genome_size);
	int randpos;
	randpos=rand() % genome_count;
	for(int i=0;i<genome_size;i++)
	{
		double gamma;
		do{
        gamma=-0.5+2.0*problem->randomDouble();
		xtrial[i]=(1.0+gamma)*genome[0][i]-gamma*genome[randpos][i];
		}while(xtrial[i]<lmargin[i] || xtrial[i]>rmargin[i]);
	}
	double ftrial = fitness(xtrial);
	if(ftrial>fitness_array[genome_count-1])
	{
		genome[genome_count-1]=xtrial;
		fitness_array[genome_count-1]=ftrial;
		if(ftrial>bestfitness_array[genome_count-1])
		{
			bestfitness_array[genome_count-1]=ftrial;
			bestgenome[genome_count-1]=genome[genome_count-1];
		}
	}
}

int     DoublePop::selectWithTournament(int size)
{
    double max= 1e+100;
    int posMax= -1;
    for(int i=0;i<size;i++)
    {
        int pos = rand() % genome_count;
        if(fitness_array[pos]<max)
        {
            max = fitness_array[pos];
            posMax = pos;
        }
    }
    return posMax;
}
void    DoublePop::localSearch(int pos)
{
    vector<double> g;
       g.resize(genome[0].size());

           int randomA,randomB,randomC;
           do
           {
               randomA =  selectWithTournament(8);
               randomB =  selectWithTournament(8);
               randomC = selectWithTournament(8);
           }while(randomA == randomB || randomB == randomC ||
                  randomC == randomA);
           double CR= 0.9;
           double F = 0.8;
           int n = problem->getDimension();
           int randomIndex = rand() % n;
       for(int i=0;i<n;i++)
       {
           if(i==randomIndex || problem->randomDouble() <=CR)
           {
               double old_value = genome[pos][i];
               //F = -0.5 + 2.0 * rand()*1.0/RAND_MAX;
	       //F = 0.5+drand48()/2.0;
               genome[pos][i]=genome[randomA][i]+fabs(F*(genome[randomB][i]-genome[randomC][i]));
               if(!problem->isPointIn(genome[pos]))
               {
                   genome[pos][i]=old_value;
                   continue;
               }

               for(int j=0;j<n;j++) g[j]=genome[pos][j];
               double trial_fitness=problem->funmin(g);
               if(fabs(trial_fitness)<fabs(fitness_array[pos]))
               {
                  /* printf("DE[%d]=%20.10lg=>%20.10lg\n",pos,
                          fitness_array[pos],
                          trial_fitness);*/
                   fitness_array[pos]=trial_fitness;

               }
               else	genome[pos][i]=old_value;
           }
       }
}
void	DoublePop::Solve()
{
	double x1=0.0;
	double x2=0.0;
	double stopat=0.0;
	double old_best=1e+100;
	extern int iprint;
	int it=0;
    for(int i=2;i<=maxGenerations+1;i++)
    {
		nextGeneration();
		select();
		
        double fmin;
		fmin=fitness_array[0];
		double v=(1.0+fabs(fmin));
		x1+=v;
		x2+=v * v;
		double variance=x2/i-(x1/i)*(x1/i);
		if(fabs(fmin-old_best)>1e-5) 
		{
			it++;
			old_best = fmin;
            stopat=variance/2.0;
		}
	 

#ifdef LOCALSEARCH
            const int K_LS=20;
            if( i%K_LS==0)
            {
		Data tempg;
		double tempf;
		tempg.resize(genome_size);
#pragma omp parallel for num_threads(MAXTHREADS) schedule(dynamic)
                for(int i=0;i<5;i++)
                {
                int randPos=rand() % genome.size();
              	tempg=genome[randPos];
	        tempf=fitness_array[randPos];
			
		LocalSearch(problem,tempg,tempf);
		if(isnan(tempf) || isinf(tempf)) continue;
		else
		{
			genome[randPos]=tempg;
			fitness_array[randPos]=tempf;
		}

                printf("GETTING FROM BFGS %lf THREAD : %d \n",fitness_array[randPos],omp_get_thread_num());
                }
                select();
            }
#endif

		double diff1=fabs(fitness_array[0]-fitness_array[genome_count-1]);
		double diff2=fabs(1.0-fitness_array[0]/fitness_array[genome_count-i-1]);
        if(iprint  && i%10==0)
		printf("GENERATION:%4d\tVALUE=%15.8lg\tVARIANCE=%8.5lg\tSTOPAT=%8.5lg\n",
				i,fitness_array[0],variance,stopat);
		
		if(fabs(fitness_array[0])<1e-10) break;
        if(i%LI==0)
        {
            for(int k=0;k<LC;k++)
	    {
		    int pos = rand() % genome_count;
               	    localSearch(pos);
	    }
	    select();
        }
	if(printDebug==1)	
	{
	printf("%d ",i);
	problem->getIntervalProblem()->setParameter("printFlag","1");		
	fitness_array[0]=problem->funmin(genome[0]);
	}
	problem->getIntervalProblem()->setParameter("printFlag","0");		
    }
	LocalSearch(problem,genome[0],fitness_array[0]);
	have_finished=1;
	fitness_array[0]=problem->funmin(genome[0]);
	have_finished=0;
}

void	DoublePop::enableDebug()
{
	printDebug =1;
}

DoublePop::~DoublePop()
{
}
