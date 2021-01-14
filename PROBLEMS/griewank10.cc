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
int	getdimension()
{
	return 10;
}

void    init(QJsonObject data)
{

}

void 	getmargins(vector<interval> &x)
{
	for(int i=0;i<x.size();i++)
		x[i]=interval(-600,600);
}

interval ifunmin(vector<interval> &x)
{
	int n=getdimension();
        interval sum(0.0,0.0);
        interval product(1.0,1.0);
        for(int i=0;i<n;i++)
        {
           sum=sum+pow(x[i],2.0)/interval(4000.0,4000.0);
           product=product *cos(x[i]/interval(sqrt(i+1.0),sqrt(i+1.0)));
        }
        return sum+interval(1.0,1.0)-product;
}

double  funmin(vector<double> &x)
{
        int n=10;
        double sum=0.0;
        double product=1.0;
        for(int i=0;i<n;i++)
        {
           sum+=x[i]*x[i]/4000.0;
           product*=cos(x[i]/sqrt(i+1.0));
        }
        return sum+1.0-product;

}
void    granal(vector<double> &x,vector<double> &g)
{
         int n=10;
        for(int i=0;i<n;i++)
        {
            double product=1.0;
            for(int j=0;j<n;j++)
            {
               if(i==j) continue;
               product*=cos(x[j]/sqrt(j+1.0));
            }
            g[i]=x[i]/2000.0+sin(x[i]/sqrt(i+1.0))*product/sqrt(i+1.0);
        }
}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}

}
