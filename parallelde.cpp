#include "parallelde.h"
# include <tolmin.h>

# include "omp.h"

ParallelDe::ParallelDe(Problem *p)

{
    myProblem = p;
    before = std::chrono::system_clock::now();
    QStringList parde_weight_method;
}

int ParallelDe::selectAtom(int islandIndex)
{
    if (parde_selection_method == "random")
    {
        int a = islandStartPos(islandIndex);
        int b = islandEndPos(islandIndex);
        int r = a + rand() % (b - a);
        return r;
    }
    else
        return tournament(islandIndex);
}

void ParallelDe::getBestValue(int &index, double &value)
{

    value = 1e+100;
    index = 0;
    for (int i = 0; (int)i < population.size(); i++)
    {
        if (fitness_array[i] < value || i == 0)
        {
            value = fitness_array[i];
            index = i;
        }
    }
}

void ParallelDe::getBestValue(int islandName, int &index, double &value)
{
    value = bestIslandValues[islandName];
    index = bestIslandIndex[islandName];
    return;
    value = 1e+100;
    index = 0;
    int i = islandStartPos(islandName);
    int end = this->islandEndPos(islandName);
    for ( ; (int)i <= end; i++)
    {
        if (fitness_array.at(i) < value)
        {
            value = fitness_array.at(i);
            index = i;
        }
    }
}

int ParallelDe::islandStartPos(int islandIndex)
{
    return islandIndex * population.size() / islands;
}

int ParallelDe::islandEndPos(int islandIndex)
{
    return (islandIndex + 1) * population.size() / islands - 1;
}

void ParallelDe::setIslands(int i)
{
    islands = i;
}
int  ParallelDe::getIslands() const
{
    return islands;
}

void    ParallelDe::setAgents(int t)
{
    agents = t;
}
int     ParallelDe::getAgents() const
{
    return agents;
}

void    ParallelDe::setF(double f)
{
    parde_F = f;
}


double  ParallelDe::getF() const
{
    return parde_F;
}
void    ParallelDe::setCR(double cr)
{
    parde_CR = cr;
}

double  ParallelDe::getCR() const
{
    return parde_CR;
}

void    ParallelDe::setGenerations(int g)
{
    parde_generations=g;
}
int     ParallelDe::getGenerations() const
{
    return parde_generations;
}

void    ParallelDe::setPropagateMethod(QString m)
{
    parde_propagate_method=m;
}

QString ParallelDe::getPropagateMethod() const
{
    return parde_propagate_method;
}

void    ParallelDe::setSelectionMethod(QString m)
{
    parde_selection_method=m;
}

QString ParallelDe::getSelectionMethod() const
{
    return parde_selection_method;
}

void    ParallelDe::setWeightMethod(QString m)
{
    parde_weight_method = m;
}

QString ParallelDe::getWeightMethod() const
{
    return parde_weight_method;
}

void    ParallelDe::setPropagationRate(int k)
{
    parde_propagate_rate=k;
}

int     ParallelDe::getPropagationRate() const
{
    return parde_propagate_rate;
}

void ParallelDe::init()
{


    generation = 0;
    similarity_max_count = 10;
    global_sim_value = 1e+100;
    global_sim_count = 0;

    omp_set_dynamic(0);
    omp_set_num_threads(islands);
    start = omp_get_wtime();
    doublebox_xx1.resize(islands);
    doublebox_xx2.resize(islands);
    doublebox_best_value.resize(islands);
    doublebox_stopat.resize(islands);
    doublebox_variance.resize(islands);
    for (int i=0;i<islands;i++)
    {
        doublebox_xx1.at(i) = 0.0;
        doublebox_xx2.at(i) = 0.0;
        doublebox_best_value.at(i) = 1e+100;
        doublebox_stopat.at(i) = 1e+100;
        doublebox_variance.at(i) = 1e+100;
    }


    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    population.resize(agents * islands);
    fitness_array.resize(agents * islands);

    int N = agents * islands;
    for(int i=0;i<N;i++)
    {
        population[i].resize(myProblem->getDimension());
	int island = i/agents;
	bool isFirst = (i % agents)==0;
	for(int j=0;j<myProblem->getDimension();j++)
	{
		double a=lmargin[j];
        double b=rmargin[j];/*
        double width=(b-a);
		double mid = a+width/2;
		double delta = (island+1.0) *(width/2)/islands;

		a=mid-delta;
		b=mid+delta;
		if(a>b) {double t= a; a=b;b=t;}
		if(j==0 && isFirst)
        printf("Delta = %lf Island %d range [%lf %lf ]\n",delta,island,a,b);*/
		a=-0.1;
		b= 0.1;
		if(lmargin[i]>a) a=lmargin[i];
		if(rmargin[i]<b) b=rmargin[i];
		population[i][j]=a+(b-a)*myProblem->randomDouble();
	}
        fitness_array[i]=myProblem->funmin(population[i]);
    }


    bestIslandIndex.resize(islands);
    bestIslandValues.resize(islands);
    similarity_best_value.resize(islands);
    similarity_current_count.resize(islands);
    sum.resize(islands);
    newSum.resize(islands);
    MO.resize(islands);
    newMO.resize(islands);
    for (int i=0;i<islands;i++)
    {
        similarity_best_value.at(i) = 1e+100;
        similarity_current_count.at(i) = 0;
        //sum.at(j) = accumulate(fitness_array.at(this->islandStartPos(j)), fitness_array.at(this->islandEndPos(j)), 0);
        //sum.at(j) = accumulate(fitness_array.begin() + this->islandStartPos(j), fitness_array.begin() + this->islandEndPos(j), 0);
        //sum.at(j) = accumulate(this->islandStartPos(j), this->islandEndPos(j), 0);
        sum.at(i) = 0;
        MO.at(i) = 0;
    }

    //parde_islands_enable =      getParam("parde_islands_enable").getValue().toInt();
}

void ParallelDe::replaceValueInIsland(int islandIndex, Data &x, double &y)
{
    // find worst value
    int minIndex = 0;
    double minValue = -1e+100;
    for (int pos = islandStartPos(islandIndex); pos <= islandEndPos(islandIndex); pos++)
    {
        if (fitness_array[pos] > minValue)
        {
            minIndex = pos;
            minValue = fitness_array[pos];
        }
    }
    if (fitness_array[minIndex] > y)
    {
        fitness_array[minIndex] = y;
        population[minIndex] = x;
    }
}

void ParallelDe::propagateIslandValues()
{
    if (parde_propagate_method == "1to1")
    {
        int island1 = rand() % islands;
        int island2 = rand() % islands;
        if (island1 == island2)
            return;
        Data xx = population[bestIslandIndex[island1]];
        double yy = bestIslandValues[island1];
        replaceValueInIsland(island2, xx, yy);
    }
    else if (parde_propagate_method == "1toN")
    {
        int island1 = rand() % islands;
        Data xx = population[bestIslandIndex[island1]];
        double yy = bestIslandValues[island1];
        for (int i = 0; i < islands; i++)
        {
            if (i == island1)
                continue;
            replaceValueInIsland(i, xx, yy);
        }
    }
    else if (parde_propagate_method == "Nto1")
    {
        int island2 = rand() % islands;
        for (int i = 0; i < islands; i++)
        {
            if (i == island2)
                continue;
            int island1 = i;
            Data xx = population[bestIslandIndex[island1]];
            double yy = bestIslandValues[island1];
            replaceValueInIsland(island2, xx, yy);
        }
    }
    else if (parde_propagate_method == "NtoN")
    {
        for (int i = 0; i < islands; i++)
        {
            for (int j = 0; j < islands; j++)
            {
                if (i == j)
                    continue;
                int island1 = i;
                int island2 = j;
                Data xx = population[bestIslandIndex[island1]];
                double yy = bestIslandValues[island1];
                replaceValueInIsland(island2, xx, yy);
            }
        }
    }
}

int ParallelDe::tournament(int islandIndex, int tsize)
{
    int bestIndex = 0;
    double bestValue = 1e+100;
    for (int i = 0; i < tsize; i++)
    {
        int startPos = islandStartPos(islandIndex);
        int endPos = islandEndPos(islandIndex);
        int r = rand() % (endPos - startPos) + startPos;
        if (fitness_array[r] < bestValue || i == 0)
        {
            bestValue = fitness_array[r];
            bestIndex = r;
        }
    }
    return bestIndex;
}
bool ParallelDe::checkIsland(int islandName)
{
    /*parde_termination = terminationMethod;
    if(parde_termination == "doublebox" &&
            threadDoublebox[islandName].terminate(bestIslandValues[islandName])) return true;
    if(parde_termination == "similarity" &&
            threadSimilarity[islandName].terminate(bestIslandValues[islandName])
            ) return true;                             //sum similarity
    if(generation>=parde_generations) return true;
*/
    return false;

}
bool ParallelDe::terminated()
{

    if(generation%50==0)
    {
        int index;
        double bestValue;
        getBestValue(index,bestValue);
        printf("PARDE. Iteration = %4d Best Value=%20.10lg \n",generation,bestValue);
    }
    if(generation>=parde_generations) return true;

    return false;
/*
    int parde_islands_enable = getParam("parde_islands_enable").getValue().toInt();
    if(parde_islands_enable==0)
    {
        int index;
        double bestValue;
        getBestValue(index,bestValue);
        if(generation%50==0)
        {
            getBestValue(index,bestValue);
            printf("PARDE. Iteration = %4d Best Value=%20.10lg \n",generation,bestValue);
        }
        int maxGenerations = getParam("parde_generations").getValue().toInt();
        if(generation>=maxGenerations) return true;
        if(terminationMethod=="doublebox")
            return doubleBox.terminate(bestValue);
        else
            if(terminationMethod=="similarity")
                return similarity.terminate(bestValue);
        return false;
    }
    if(generation%50==0)
    {
        int index;
        double bestValue;
        getBestValue(index,bestValue);
        printf("PARDE. Iteration = %4d Best Value=%20.10lg \n",generation,bestValue);
    }
    int c = 0;

    for (int i = 0; i < islands; i++)
        if (this->checkIsland(i))
            c++;
    //printf("c = %d islands = %d \n",c,islands);
    //printf("c = %d islands = %d parde_islands_enable = %d\n",c,islands,parde_islands_enable);
    return generation >= parde_generations || c >= parde_islands_enable;
*/
}

double ParallelDe::getDifferentialWeight()
{

    if (parde_weight_method == "constant")
        return parde_F;
    else if (parde_weight_method == "ali")
    {
    }
    else
        return -0.5 + 2.0 * rand() * 1.0 / RAND_MAX;
}

void ParallelDe::step()
{
    ++generation;
    int t;
#pragma omp parallel for private(t) num_threads(islands)
    for (t = 0; t < islands; t++)
    {
        bestIslandIndex[t] = 0;
        bestIslandValues[t] = 1e+100;
        int j;
        //#pragma omp parallel for private(j) num_threads(islands)
        for (j = islandStartPos(t); j <= islandEndPos(t); j++)
        {
            double differentialWeight = getDifferentialWeight();
            int indexA, indexB, indexC;
            do
            {
                indexA = selectAtom(t);
                indexB = selectAtom(t);
                indexC = selectAtom(t);
            } while (indexA == indexB || indexB == indexC || indexA == indexC);
            Data y;

            y.resize(myProblem->getDimension());
            int R = rand() % y.size();
            for (int k = 0; k < y.size(); k++)
            {
                int ri = rand() * 1.0 / RAND_MAX;
                if (ri < parde_CR || k == R)
                {
                    y[k] = population[indexA][k] +
                           differentialWeight * (population[indexB][k] - population[indexC][k]);
                    if (y[k] < lmargin[k] || y[k] > rmargin[k])
                        y[k] = population[j][k];
                }
                else
                {
                    y[k] = population[j][k];
                }
            }
            double yy = myProblem->funmin(y);

            if (yy <= fitness_array[j])
            {
                population[j] = y;
                fitness_array[j] = yy;
            }

            if (fitness_array[j] < bestIslandValues[t])
            {
                bestIslandIndex[t] = j;
                bestIslandValues[t] = fitness_array[j];
            }

            //newSum.at(t) = accumulate(fitness_array.begin() + this->islandStartPos(t), fitness_array.begin() + this->islandEndPos(t), 0);
            //newSum.at(j) = accumulate(fitness_array.at(j), fitness_array.begin(j) + this->islandEndPos(j), 0);
            //this->checkIsland(t);
        }
        //newSum.at(t) = accumulate(fitness_array.begin() + this->islandStartPos(t), fitness_array.begin() + this->islandEndPos(t), 0);
        //this->checkIsland(t);
    }

    for (int j = 0; j < islands; j++)
        newSum.at(j) = accumulate(&fitness_array.at(this->islandStartPos(j)), &fitness_array.at(this->islandEndPos(j)), 0);

    for (int j = 0; j < islands; j++)
    {
        newMO.at(j) = (double)newSum.at(j)/islands;
      //  this->checkIsland(j);
    }
    if (generation % parde_propagate_rate)
        propagateIslandValues();
}

static void LocalSearch(Problem *p,Data &x,double &y)
{
    if(x.size()>1000) return ;
    y=tolmin(x,p,2001);
}
void ParallelDe::done()
{
    int bestIndex = 0, bestI = 0;
    double bestValue = 1e+100, bestV = 1e+100;
    getBestValue(bestIndex, bestValue);
    LocalSearch(myProblem,population[bestIndex],bestValue);
    fitness_array[bestIndex]=bestValue;
}

void    ParallelDe::Solve()
{
    init();
    do
    {
        step();
    }while(!terminated());
    done();
}
void    ParallelDe::getBest(Data &x,double &y)
{
    int bestIndex = 0;
    getBestValue(bestIndex, y);
    x=population[bestIndex];
}

ParallelDe::~ParallelDe()
{
    // nothing here
}
