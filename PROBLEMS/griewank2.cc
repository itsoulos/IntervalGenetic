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

interval ifunmin(vector<interval> &x)
{
    interval sum=interval(0.0,0.0);
    interval product=interval(1.0,1.0);
    for(int i=0;i<2;i++)
     {
        sum=sum+pow(x[i],2.0)/200.0;
        product=product * cos(x[i]/sqrt(i+1.0));
    }
    return sum+1.0-product;
}

void 	getmargins(vector<interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=interval(-100.0,100.0);
    }
}
double	funmin(vector<double> &x)
{
	 int n=2;
        double sum=0.0;
        double product=1.0;
        for(int i=0;i<n;i++)
        {
          sum+=x[i]*x[i]/200.0;
          product*=cos(x[i]/sqrt(i+1.0));
        }
        return sum+1.0-product;

}

void    granal(vector<double> &x,vector<double> &g)
{
	 int n=2;
         double a,b;
         for(int i=0;i<n;i++)
         {
            double product=1.0;
            for(int j=0;j<n;j++)
            {
              product*=cos(x[j]/sqrt(j+1.0));
             }
             a=sin(x[i]/sqrt(i+1.0));
             b=sqrt(i+1.0)*cos(x[i]/sqrt(i+1.0));
             g[i]=x[i]/100.0+a*product/b;
         }

}
QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}
