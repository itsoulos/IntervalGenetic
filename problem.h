#ifndef PROBLEM_H
#define PROBLEM_H
# include <intervalproblem.h>

class Problem
{
private:
    IntervalProblem *problem;
    int dimension;
    Data lmargin,rmargin;
public:
    Problem(IntervalProblem *p);
    Problem(IntervalProblem *p,IntervalData &m);
    int         getDimension() const;
    Data        getLeftMargin() const;
    Data        getRightMargin() const;
    double      funmin(Data &x);
    void        granal(Data &x,Data &g);
    Data        getRandomPoint();
    Data        getUniformRandomPoint();
    QJsonObject done(Data &x);
    double      randomDouble();
    IntervalProblem	*getIntervalProblem();
    bool        isPointIn(Data &x);
};

#endif // PROBLEM_H
