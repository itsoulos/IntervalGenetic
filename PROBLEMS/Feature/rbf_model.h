# ifndef __FC_RBF__H
# define __FC_RBF__H
# include <model.h>
# include <Rbf.h>
#include <armadillo>

class Rbf :public Model
{
	private:

        double *input,*centers,*variances,*weights;
		vector<double> randNumber;
        vector<double> params;
double *Output;
	public:
		Rbf(Mapper *m);
        double neuronOutput( vector<double> &x, vector<double> &patt, unsigned pattDim, unsigned offset,
                                bool &failed);
        arma::vec train( vector<double> &x,bool &failed );
		virtual double train1();
		virtual double train2();
		virtual double output(Matrix x);
		virtual void   getDeriv(Matrix x,Matrix &g);
		~Rbf();
};

# endif
