#ifndef INTERVALPROBLEM_H
#define INTERVALPROBLEM_H
# include <vector>
# include <iostream>
# include <QJsonObject>
# include <interval.h>
# include <QRandomGenerator>
using namespace  std;
typedef vector<double> Data;
typedef vector<Interval> IntervalData;

extern QRandomGenerator randGen;

class IntervalProblem
{
protected:
    int                 dimension;
    IntervalData        margin;
    const int     maxSearchDepth=5;
public:
    IntervalProblem(int d);
    void                setDimension(int d);
    void                setMargins(IntervalData &m);
    IntervalData        getMargins() const;
    int                 getDimension() const;
    bool                isPointIn(Data &x) const;
    bool                isPointIn(IntervalData &x) const;
    IntervalData        createRandomInterval();
    void                randomize(IntervalData &x,int pos);
    virtual             void init(QJsonObject x);
    virtual             void setParameter(QString name,QVariant value);
    virtual double      funmin(Data &x);
    virtual void        granal(Data &x,Data &g);
    virtual QJsonObject done(Data &x);
    bool                lowerValue(Interval &a,Interval &b);
    double              getVolume() const;
    double              getVolume(IntervalData &x) const;
    void                boundInterval(IntervalData &x);
    void                printData(IntervalData &x);
    double              randomDouble();
    virtual             ~IntervalProblem();
};

#endif // INTERVALPROBLEM_H
