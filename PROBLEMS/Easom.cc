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
	static double dmax(double x,double y)
	{
		return x>y?x:y;
	}
int	getdimension()
{
	return 2;
}


void 	getmargins(vector<interval> &x)
{
    x[0]=interval(-100.0,100.0);
    x[1]=interval( 100.0,100.0);
}

interval ifunmin(vector<interval> &x)
{
    return -cos(x[0])*cos(x[1]*exp(-(pow(x[0]-M_PI,2.0)+pow(x[1]-M_PI,2.0))));

}

double	funmin(vector<double> &x)
{
	return -cos(x[0])*cos(x[1]*exp(-(pow(x[0]-M_PI,2.0)+pow(x[1]-M_PI,2.0))));
}

void    granal(vector<double> &x,vector<double> &g)
{
	 for(int i=0;i<getdimension();i++)
         {
                double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
                x[i]+=eps;
                double v1=funmin(x);
                x[i]-=2.0 *eps;
                double v2=funmin(x);
                g[i]=(v1-v2)/(2.0 * eps);
                x[i]+=eps;
        }

}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}
