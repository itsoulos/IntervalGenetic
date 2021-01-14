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

	static double dmax(double a,double b)
	{
		return a>b?a:b;
	}
int	getdimension()
{
	return 4;
}

void 	getmargins(vector<interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=interval(0.0,10.0);
    }
}
interval ifunmin(vector<interval> &x)
{
    static double a[7][4]={{4,4,4,4},{1,1,1,1},{8,8,8,8},{3,7,3,7},{2,9,2,9},{5,5,3,3}};
    static double c[7]={0.1,0.2,0.2,0.4,0.4,0.6,0.3};
    interval sum=interval(0.0,0.0);
    for(int i=0;i<7;i++)
    {
        interval s=interval(0.0,0.0);
        for(int j=0;j<4;j++) s=s+pow(x[j]-a[i][j],2.0);
        sum=sum+1.0/(s+c[i]);
    }
    return -sum;
}

double	funmin(vector<double> &x)
{
	static double a[7][4]={{4,4,4,4},{1,1,1,1},{8,8,8,8},{3,7,3,7},{2,9,2,9},{5,5,3,3}};
	static double c[7]={0.1,0.2,0.2,0.4,0.4,0.6,0.3};
	double sum=0.0;
	for(int i=0;i<7;i++)
	{
		double s=0.0;
		for(int j=0;j<4;j++) s=s+pow(x[j]-a[i][j],2.0);
		sum=sum+1.0/(s+c[i]);	
	}
	return -sum;
}

void    granal(vector<double> &x,vector<double> &g)
{
	static double a[7][4]={{4,4,4,4},{1,1,1,1},{8,8,8,8},{3,7,3,7},{2,9,2,9},{5,5,3,3}};
	static double c[7]={0.1,0.2,0.2,0.4,0.4,0.6,0.3};
	for(int j=0;j<4;j++)
        {
		double s=0.0;
		for(int  i=0;i<7;i++)
		{
			double ss=0.0;
			for(int k=0;k<4;k++) ss+=pow(x[k]-a[i][k],2.0);
			ss+=c[i];
			s=s+pow(ss,-2.0)*2.0*(x[j]-a[i][j]);
		}
		g[j]=s;
        }

}
QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}
