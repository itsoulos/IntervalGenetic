# ifndef __NEURAL__H
# define __NEURAL__H
# include <model.h>
class Neural :public Model
{
	private:
		int nextRand;
		vector<double> randomValues;
	public:
		Neural(Mapper *m,int x);
		double  getRandom();
		double	countViolate(double limit);
		virtual double train1();
		virtual double train2();
		virtual double output(Matrix x);
		virtual void   getDeriv(Matrix x,Matrix &g);
		void	setWeights(Matrix x);
		~Neural();
};

# endif
