#ifndef PARALLELDE_H
#define PARALLELDE_H
# include <problem.h>
# include <QStringList>
# include <chrono>

class ParallelDe
{
private:
    Problem *myProblem;
    vector<Data> population;

    double parde_F=0.8, parde_CR=0.9;
    int islands=1, agents=200,generation, parde_generations=1000,
        similarity_max_count=5, parde_islands_enable,parde_propagate_rate=5;
    vector<int> bestIslandIndex,similarity_current_count;
    vector<double> similarity_best_value,lmargin, rmargin,bestIslandValues,fitness_array,sum,newSum, MO, newMO;
    vector<double> doublebox_xx1, doublebox_xx2, doublebox_best_value, doublebox_variance, doublebox_stopat;
    QString parde_termination,
                parde_propagate_method="NtoN"//1to1,1toN,Nto1,NTON
                ,parde_weight_method="ali";//constant,ali
    std::chrono::time_point<std::chrono::system_clock> before, after;


    int     selectAtom(int islandIndex);
    int     tournament(int islandIndex, int tsize = 8);
    int     islandStartPos(int islandIndex);
    int     islandEndPos(int islandIndex);
    void    propagateIslandValues();
    void    getBestValue(int &index, double &value);
    void    getBestValue(int islandName, int &index, double &value);
    double  getDifferentialWeight();
    void    replaceValueInIsland(int islandIndex, Data &x, double &y);
    bool    checkIsland(int islandName);
    QString parde_selection_method="random";//random, tournament

    //for similarity stopping rule
    double global_sim_value;
    int global_sim_count;
    double start,end;
public:
    ParallelDe(Problem *p);
    void    setIslands(int i);
    int     getIslands() const;
    void    setAgents(int t);
    int     getAgents() const;
    void    setF(double f);
    double  getF() const;
    void    setCR(double cr);
    double  getCR() const;
    void    setGenerations(int g);
    int     getGenerations() const;
    void    setPropagateMethod(QString m);
    QString getPropagateMethod() const;
    void    setSelectionMethod(QString m);
    QString getSelectionMethod() const;
    void    setWeightMethod(QString m);
    QString getWeightMethod() const;
    void    setPropagationRate(int k);
    int     getPropagationRate() const;
    /**
     * @brief terminated
     * @return true if DE should be terminated.
     */
    virtual bool terminated();
    /**
     * @brief step Executes a step of DE method.
     */
    virtual void step();
    /**
     * @brief init Initializes the optimizer.
     */
    virtual void init();
    /**
     * @brief done It is executed when DE finishes.
     */
    virtual void done();
    void    Solve();
    void    getBest(Data &x,double &y);
    virtual ~ParallelDe();
};

#endif // PARALLELDE_H
