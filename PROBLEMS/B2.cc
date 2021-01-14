# include <math.h>
# include <gaol/gaol.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
using namespace std;


extern "C"
{

void    init(QJsonObject data)
{

}

int	getdimension()
{
	return 2;
}

void 	getmargins(vector<interval> &x)
{
    for(int i=0;i<x.size();i++)
        x[i]=interval(-100*1.0,100*1.0);
}
interval ifunmin(vector<interval> &x)
{
  return pow(x[0],2)+2.0*pow(x[1],2)-0.3*cos(3.0*M_PI*x[0])-0.4*cos(4.0*M_PI*x[1])+0.7;
}

double	funmin(vector<double> &x)
{
	return x[0]*x[0]+2.0*x[1]*x[1]-0.3*cos(3.0*M_PI*x[0])-0.4*cos(4.0*M_PI*x[1])+0.7;
}

void    granal(vector<double> &x,vector<double> &g)
{
	g[0]=2.0*x[0]+0.9*M_PI*sin(3.0*M_PI*x[0]);
	g[1]=4.0*x[1]+1.6*M_PI*sin(4.0*M_PI*x[1]);
}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}

}
