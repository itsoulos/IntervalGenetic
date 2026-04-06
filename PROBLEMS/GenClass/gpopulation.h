# ifndef __GPOPULATION__H
# include <program.h>
class GPopulation
{
	private:
		int	**children;
		vector<int>	rightMargin;
		int	**genome;
		double *fitness_array;
		double	mutation_rate,selection_rate;
		int	genome_count;
		int	genome_size;
		int	generation;
		Program	*program;

		double 	fitness(vector<int> &g);
		void	crossover();
		void	mutate();
		void	calcFitnessArray();
		int	elitism;
	public:
		GPopulation(int gcount,int gsize,vector<int> &rm,Program *p);
		void	setElitism(int s);
		int	getGeneration() const;
		int	getCount() const;
		int	getSize() const;
		void	nextGeneration();
		void	setMutationRate(double r);
		void	setSelectionRate(double r);
		double	getSelectionRate() const;
		double	getMutationRate() const;
		double	getBestFitness() const;
		double	evaluateBestFitness();
		vector<int> getBestGenome() const;
		void	reset();
		void	setBest(vector<int> &g,double f);
		void	getGenome(int pos,vector<int> &genome);
		void	setGenome(int pos,vector<int> &genome,double f);
		void	setGenome(int pos,vector<int> &genome,double f,int k);
		void	select();
		void	localSearch(int x);
        vector<int> discreteGradient( vector<int>& x);
        vector<int> discreteStep(vector<int>& x,vector<int>& grad);
        void integerLocalSearch(vector<int> &x,int maxSteps = 20);
        vector<int> integerAdam(
            vector<int> x,
            int steps = 50,
            double alpha = 0.5,
            double beta1 = 0.9,
            double beta2 = 0.999,
            double eps = 1e-8
            ) ;
        vector<int> neighbor(const vector<int>& x, int stepSize = 1);
        vector<int> simulatedAnnealing(
            vector<int> &x,
            double T0 = 100.0,
            double Tmin = 1e-3,
            double alpha = 0.95,
            int iterPerTemp = 20
            );
		~GPopulation();
		
};
# define __GPOPULATION__H
# endif
