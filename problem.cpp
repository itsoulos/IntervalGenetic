#include "problem.h"
Problem::Problem(IntervalProblem *p)
{
    problem=p;
    dimension=p->getDimension();
    lmargin.resize(dimension);
    rmargin.resize(dimension);
    IntervalData bound=p->getMargins();
    for(int i=0;i<dimension;i++)
    {
        lmargin[i]=bound[i].leftValue();
        rmargin[i]=bound[i].rightValue();
    }
}

Data       Problem::getUniformRandomPoint()
{
    Data x;
    x.resize(dimension);
    for(int i=0;i<dimension;i++)
    {
        double r=randomDouble();
        x[i]=0.1*(2.0*r-1.0);
    }
    return x;
}

Problem::Problem(IntervalProblem *p,IntervalData &m)
{
    problem=p;
    dimension=p->getDimension();
    lmargin.resize(dimension);
    rmargin.resize(dimension);

    for(int i=0;i<dimension;i++)
    {
        lmargin[i]=m[i].leftValue();
        rmargin[i]=m[i].rightValue();
    }
}

int  Problem::getDimension() const
{
    return dimension;
}

Data Problem::getLeftMargin() const
{
    return lmargin;
}

Data Problem::getRightMargin() const
{
    return rmargin;
}

double  Problem::randomDouble()
{
    return problem->randomDouble();
}

double Problem::funmin(Data &x)
{
    return problem->funmin(x);
}
void Problem::granal(Data &x,Data &g)
{
    problem->granal(x,g);
}

Data    Problem::getRandomPoint()
{
    Data x;
    x.resize(dimension);
    for(int i=0;i<dimension;i++)
    {
        double a=lmargin[i];
        double b=rmargin[i];
        if(a<-1) a=-1;
        if(b>1)  b=1;
        x[i]=a+(b-a)*randomDouble();
        /*
        double r=randomDouble();
	double middle=lmargin[i]+(rmargin[i]-lmargin[i])/2.0;
	double delta = (r-1.0);
	x[i]=middle+delta*(0.1 * (rmargin[i]-lmargin[i]));
	double a=-1.0,b=1.0;
	a=(lmargin[i]<a)?lmargin[i]:a;
    b=(rmargin[i]<b)?rmargin[i]:b;*/
        //x[i]=lmargin[i]+r*(rmargin[i]-lmargin[i]);
    }
    return x;
}


bool    Problem::isPointIn(Data &x)
{
    for(int i=0;i<x.size();i++)
        if(x[i]<lmargin[i] || x[i]>rmargin[i])
            return false;
    return true;
}

QJsonObject    Problem::done(Data &x)
{
    return problem->done(x);
}
