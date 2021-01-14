# include <math.h>
# include <gaol/gaol.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
using namespace std;
/*	Exponential Problem.
 * */
extern "C"
{

int dimension=8;
int	getdimension()
{
    return dimension;
}

void    init(QJsonObject data)
{
    if(data.contains("dimension"))
        dimension=data["dimension"].toInt();
}
void 	getmargins(vector<interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=interval(-1.0,1.0);
    }
}

interval ifunmin(vector<interval> &x)
{
    interval s1=interval(0.0,0.0);
    for(int i=0;i<dimension;i++) s1+=pow(x[i],2.0);
    return -exp(-0.5 * s1);
}

double	funmin(vector<double> &x)
{
	double s1=0.0;
    for(int i=0;i<dimension;i++) s1+=x[i]*x[i];
	return -exp(-0.5 * s1);
}

void    granal(vector<double> &x,vector<double> &g)
{
	double s1=0.0;
    for(int i=0;i<dimension;i++) s1+=x[i]*x[i];
    for(int i=0;i<dimension;i++)
	{
		g[i]=0.5 * 2.0 *x[i] *  exp(-0.5 * s1);
	}
}
QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}
