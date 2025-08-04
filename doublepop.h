# ifndef DPOP__H
# include <problem.h>
# include <vector>
using namespace std;

class DoublePop
{
	private:
		vector<Data> children;
		vector<Data> genome;
		vector<Data> bestgenome;
		Data	fitness_array;
		Data	bestfitness_array;
		Data	parent0,parent1;
		Data 	lmargin,rmargin;
		double	mutation_rate,selection_rate;
		int	genome_count;
		int	genome_size;
		int	generation;
        int  maxGenerations;

		double 	fitness(Data &g);
		void	select();
		void	crossover();
		void	mutate();
		void	calcFitnessArray();
		int	elitism;
		double	estimateVariance();
		void	replaceWorst();
		void	getTournamentElement(Data &x);
		void	tournament(Data &p1,Data &p2);
		Problem	*problem;
        int LI=20,LC=20;
	int printDebug=0;
	public:
		DoublePop(int gcount,Problem *p);
		void enableDebug();
		void	setElitism(int s);
		int	getGeneration() const;
		int	getCount() const;
		int	getSize() const;
		double	getFitness(int pos) const;
		void	nextGeneration();
		void	setMutationRate(double r);
		void	setSelectionRate(double r);
		double	getSelectionRate() const;
		double	getMutationRate() const;
		double	getBestFitness() const;
		double	evaluateBestFitness();
		Data	getBestGenome() const;
		void	reset();
		void	setBest(Data x,double y);
		void	getMinimum(Data &x,double &y);
		void	Solve();
        void    setMaxGenerations(int g);
        void    localSearch(int pos);
        int     selectWithTournament(int size);
        void    setLocalIterations(int iters);
        void    setLocalChromosomes(int g);
		~DoublePop();
		
};
# define DPOP__H
# endif
