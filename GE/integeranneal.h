#ifndef INTEGERANNEAL_H
#define INTEGERANNEAL_H
# include <GE/cprogram.h>
# include <intervalproblem.h>
class GrammarGenetic;
class IntegerAnneal
{
private:

   IntervalProblem *myProblem;
   GrammarGenetic *thisPointer;
    vector<int> xpoint;
    Interval ypoint;
    vector<int> bestx;
    Interval besty;
    int neps=100;
    double T0;
    int k;
public:
    IntegerAnneal(IntervalProblem *pr,GrammarGenetic *t);
    void    setNeps(int n);
    void    setT0(double t);
    void setPoint(vector<int> &g,Interval &y);
    void getPoint(vector<int> &g,Interval &y);
    void Solve();
    void    updateTemp();
    ~IntegerAnneal();
};
#endif // INTEGERANNEAL_H
