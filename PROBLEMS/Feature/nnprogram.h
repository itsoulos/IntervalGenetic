# ifndef __NNPROGRAM__H
# include <program.h>
# include <cprogram.h>
# include <model.h>
# include <neural.h>
# include <rbf_model.h>
# include <knn.h>
# include <mapper.h>
# include <vector>
# include "grand.h"
using namespace std;

# define MODEL_NEURAL		1
# define MODEL_RBF		2
# define MODEL_KNN		3

class NNprogram	:public Program
{
	private:
		vector<string> pstring;
		vector<int>    pgenome;
        vector<double> fpgenome;
		int	model_type;
		int	pattern_dimension;
		Cprogram *program;
		Model	 *model;
		Mapper	 *mapper;
		int random_seed;
		GRand rnd;	
	public:
        NNprogram();
	void	setSeed(int r);
	GRand   *getRand();
        void setData(int type,int pdimension,double scale,vector<Matrix> &xdata,Matrix &ydata);
		NNprogram(int type,int pdimension,char *filename);
        NNprogram(int type,int pdimension,double scale,vector<Matrix> &xdata,Matrix &ydata);
		string	printF(vector<int> &genome);
        string  printF(vector<double> &genome);
		virtual double 	fitness(vector<int> &genome);
        virtual double fitness(vector<double> &genome);
		Model	*getModel();
		Mapper	*getMapper();
		void	getStatistics(vector<int> &genome,vector<Matrix> &testx,Matrix &testy,double &test,double &eclass);
		~NNprogram();
};
# define __NNPROGRAM__H
# endif
