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
    {
    x[i]=interval(-10.0,10.0);
    }
}

interval ifunmin(vector<interval> &x)
{
    interval sum1=interval(0.0,0.0);
    interval sum2=interval(0.0,0.0);
    for(int i=1;i<=5;i++)
    {
        sum1=sum1+i*cos((i-1)*x[0]+i);
        sum2=sum2+i*cos((i+1)*x[1]+i);
    }
    return sum1 * sum2;
}

double	funmin(vector<double> &x)
{
	double sum1=0.0;
        double sum2=0.0;
        for(int i=1;i<=5;i++)
        {
           sum1+=i*cos((i-1)*x[0]+i);
           sum2+=i*cos((i+1)*x[1]+i);
         }
        return sum1 * sum2;

}

void    granal(vector<double> &x,vector<double> &g)
{
	 double sum1=0.0;
         double sum2=0.0;
         for(int i=1;i<=5;i++)
         {
            sum1+=-i*(i-1)*sin((i-1)*x[0]+i);
            sum2+=i*cos((i+1)*x[1]+i);
          }
          g[0]=sum1 * sum2;
          sum1=sum2=0;
          for(int i=1;i<=5;i++)
          {
            sum1+=i*cos((i-1)*x[0]+i);
            sum2+=-i*(i+1)*sin((i+1)*x[1]+i);
           }
           g[1]=sum1*sum2;

}
QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}
