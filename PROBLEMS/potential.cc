# include <math.h>
# include <math.h>
# include <interval.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
using namespace std;

extern "C"{
int natoms=20;
double leftmargin=  -20.0;
double rightmargin= 20.0;
typedef vector<double> Data;

int	getdimension()
{
    return	3 * natoms;
}
void    init(QJsonObject data)
{
    if(data.contains("natoms"))
        natoms=data["natoms"].toInt();
    if(data.contains("leftmargin"))
        leftmargin=data["leftmargin"].toDouble();
    if(data.contains("rightmargin"))
        rightmargin=data["rightmargin"].toDouble();
}

void 	getmargins(vector<Interval> &x)
{
	for(int i=0;i<getdimension();i++)
	{
            x[i]=Interval(leftmargin,rightmargin);
	}
}
/*
interval pot(interval r)
{

    interval oner=1/r;
    interval oner6=pow((oner),6);
    interval oner12=pow((oner),12);
    interval d =  4.0 *(oner12-oner6);
    return d;
}*/

double dpot(double r)
{
    double  eps=1.0;
    double  sig =1.0;
    double  eps4=4.0 *eps;
    double sbyr6=pow(sig/r,6.0);
    return eps4 * sbyr6*(sbyr6-1.0);
}
double	funmin(Data &x)
{
    double value=0.0;
    Data xx; xx.resize(natoms);
    Data yy; yy.resize(natoms);
    Data zz; zz.resize(natoms);
           for(int i=1;i<=natoms;i++)
           {
                   int i3=3 * i;
                   xx[i-1]=x[i3-2-1];
                   yy[i-1]=x[i3-1-1];
                   zz[i-1]=x[i3-1];
           }
           for(int i=1;i<=natoms;i++)
           {
                   for(int j=i+1;j<=natoms;j++)
                   {
                           double dx=xx[i-1]-xx[j-1];
                           double dy=yy[i-1]-yy[j-1];
                           double dz=zz[i-1]-zz[j-1];
                           double rij=sqrt(dx*dx+dy*dy+dz*dz);
                           value=value+dpot(rij);
                   }
           }
           return  value;

}
static double dmax(double a,double b){return a>b?a:b;}

void    granal(vector<double> &x,vector<double> &g)
{
    for(int i=0;i<x.size();i++)
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

QJsonObject    done(Data &x)
{
    return QJsonObject();
}
/*
interval ifunmin(vector<interval> &x)
{
	vector<interval> xx,yy,zz;
    interval value(0.0,0.0);
    xx.resize(natoms);
    yy.resize(natoms);
    zz.resize(natoms);
    for(int i=1;i<=natoms;i++)
	{
		int i3=3 * i;
		xx[i-1]=x[i3-2-1];
		yy[i-1]=x[i3-1-1];
		zz[i-1]=x[i3-1];
	}
    for(int i=1;i<=natoms;i++)
	{
        for(int j=i+1;j<=natoms;j++)
		{
            if(i==j) continue;
            interval dx=abs(xx[i-1]-xx[j-1]);
            interval dy=abs(yy[i-1]-yy[j-1]);
            interval dz=abs(zz[i-1]-zz[j-1]);
            interval rij=(pow(dx,2)+pow(dy,2)+pow(dz,2));
            rij=sqrt(rij);
            value = value +pot(rij);

		}
	}
    return value.right();
}
*/
}
