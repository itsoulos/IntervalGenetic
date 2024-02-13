# include <neural.h>
# include <math.h>
# include <gensolver.h>
# include <tolmin.h>
#include <Eigen/Core>
#include <iostream>
#include <LBFGSB.h>

using namespace LBFGSpp;

typedef double Scalar;
typedef Eigen::Matrix<Scalar, Eigen::Dynamic, 1> Vector;

class NeuralWrapper
{
private:
	Neural *neural;
public:
    NeuralWrapper(Neural *t) {
		neural = t;
	}
    Scalar operator()(const Vector& x, Vector& grad)
    {
		int n=neural->getDimension();
		Data xx,gg;
		xx.resize(n);
		gg.resize(n);
		for(int i=0;i<n;i++) xx[i]=x[i];
        Scalar fx = neural->funmin(xx);
		neural->granal(xx,gg);
		for(int i=0;i<n;i++)
			grad[i]=gg[i];
        return fx;
    }
};

//# define CLASSERROR
int pass=0;
double maxx=-1e+100;
static double sig(double x)
{
	return 1.0/(1.0+exp(-x));
}

static double sigder(double x)
{
	double s=sig(x);
	return s*(1-s);
}

Neural::Neural(Mapper *m,int r):Model(m)
{
#pragma omp critical
{
	srand48(r);
	/*
	int randcount=1000000;
	nextRand=0;
	for(int i=0;i<randcount;i++)
	{
		double r=drand48();
		randomValues.push_back(r);
	}*/
}
}

double  Neural::getRandom()
{
	return drand48();
	double v=randomValues[nextRand];
	nextRand=(nextRand+1) % randomValues.size();
	return v;
}

void	Neural::setWeights(Matrix x)
{
	for(int i=0;i<weight.size();i++) weight[i]=x[i];
}

double Neural::train1()
{
	for(int i=0;i<xpoint.size();i++) 
	{
		int d=mapper->map(origx[i],xpoint[i]);
		if(!d) return 1e+8;
		for(int j=0;j<pattern_dimension;j++)
		{
			if(isinf(xpoint[i][j])) return 1e+8;
		}
	}
	double v;
	MinInfo Info;
	Info.p = this;
	Info.iters=61;
	return tolmin(weight,Info);
}

double	Neural::countViolate(double limit)
{
	//metraei posoi komboi prokaloyn problima sta sigmoeidi
	int count = 0;
	int nodes=weight.size() / (pattern_dimension + 2);
	for(int i=0;i<xpoint.size();i++)
	{
		for(int n=1;n<=nodes;n++)
		{
              		double arg = 0.0;
              		for(int j=1;j<=pattern_dimension;j++)
              		{
                 	 int pos=(pattern_dimension+2)*n-(pattern_dimension+1)+j-1;
                  	 arg+=weight[pos]*xpoint[i][j-1];
			}
              	arg+=weight[(pattern_dimension+2)*n-1];
		count+=(fabs(arg)>=limit);
	      }
	}
	return count*1.0/(xpoint.size()*nodes);
}

double Neural::train2()
{
	double v;
	for(int i=0;i<xpoint.size();i++) 
	{
		mapper->map(origx[i],xpoint[i]);
	}
	MinInfo Info;
	Info.p=this;
	Info.iters=2001;
	weight.resize((pattern_dimension+2)*num_weights);
	setDimension(weight.size());
	for(int i=0;i<weight.size();i++) 
		weight[i]=2.0*getRandom()-1.0;
	//randomizeWeights();
	
	//GenSolve(this,weight,v,0,0);
	lmargin.resize(weight.size());
	rmargin.resize(weight.size());
	for(int i=0;i<weight.size();i++)
	{
		lmargin[i]=-5.0*fabs(weight[i]);
	//	if(lmargin[i]>-5) lmargin[i]=-5;
		rmargin[i]= 5.0*fabs(weight[i]);
	//	if(rmargin[i]<5) rmargin[i]=5;
		lmargin[i]=-100;
		rmargin[i]= 100;
	}
	setLeftMargin(lmargin);
	setRightMargin(rmargin);
	
	/*GenSolve(this,weight,v,1,0);
	for(int i=0;i<weight.size();i++)
	{
		lmargin[i]=-5.0*fabs(weight[i]);
		rmargin[i]= 5.0*fabs(weight[i]);
		
	}*/
	v=tolmin(weight,Info);
/*
	LBFGSBParam<Scalar> param;
	param.max_linesearch=2000;
    LBFGSBSolver<Scalar> solver(param);
    NeuralWrapper fun(this);

	int n = weight.size();
    // Variable bounds
    Vector lb = Vector::Constant(n, 2.0);
    Vector ub = Vector::Constant(n, 4.0);

	// Initial values
    Vector x = Vector::Constant(n, 3.0);
	for(int i=0;i<n;i++)
	{
		lb[i]=lmargin[i];
		ub[i]=rmargin[i];
		x[i]=weight[i];
	}

    Scalar fx;
    int niter = solver.minimize(fun, x, fx, lb, ub);

    std::cout << niter << " iterations" << std::endl;
    std::cout << "x = \n" << x.transpose() << std::endl;
    std::cout << "f(x) = " << fx << std::endl;
    std::cout << "grad = " << solver.final_grad().transpose() << std::endl;
    std::cout << "projected grad norm = " << solver.final_grad_norm() << std::endl;
*/
	return v;
}


double Neural::output(Matrix x)
{
	double arg=0.0,per=0.0;
	int dimension = pattern_dimension;
	int nodes=weight.size()/(pattern_dimension+2);
        for(int i=1;i<=nodes;i++)
        {
              arg = 0.0;
              for(int j=1;j<=dimension;j++)
              {
                  int pos=(dimension+2)*i-(dimension+1)+j-1;
                  arg+=weight[pos]*x[j-1];
              }
	      
              arg+=weight[(dimension+2)*i-1];
              per+=weight[(dimension+2)*i-(dimension+1)-1]*sig(arg);
        }
        return per;
}

void   Neural::getDeriv(Matrix x,Matrix &g)
{
  	double arg;
        double s;
        int nodes=weight.size()/(pattern_dimension+2);
	int dimension = pattern_dimension;
        double f,f2;
        for(int i=1;i<=nodes;i++)
        {
                arg = 0.0;
                for(int j=1;j<=dimension;j++)
                {
                  	int pos=(dimension+2)*i-(dimension+1)+j-1;
                        arg+=weight[pos]*x[j-1];
                }
                arg+=weight[(dimension+2)*i-1];
                f=sig(arg);
		f2=f*(1.0 -f );
                g[(dimension+2)*i-1]=weight[(dimension+2)*i-(dimension+1)-1]*f2;
                g[(dimension+2)*i-(dimension+1)-1]=f;
                for(int k=1;k<=dimension;k++)
                {
                        g[(dimension+2)*i-(dimension+1)+k-1]=
                             x[k-1]*f2*weight[(dimension+2)*i-(dimension+1)-1];
                }
        }
}

Neural::~Neural()
{
}
