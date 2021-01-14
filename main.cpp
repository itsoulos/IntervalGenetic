# include <intervalproblem.h>
# include <stdio.h>
# include <iostream>
# include <dllproblem.h>
# include <getoptions.h>
# include <intervalpso.h>
# include <intervalgenetic.h>
# include <tolmin.h>
# include <problem.h>
# include <doublepop.h>
# include <integergenetic.h>
# include <QCoreApplication>
# include <QDebug>
#include "constrainedadam.h"
#include "boundedgradientdescent.h"
#include "tolmin.h"
using namespace  std;

#define GENETIC
// Check difference 2
// Check difference 3
// git push --set-upstream origin master

QJsonObject params;
QStringList localMethodList;
QStringList intervalMethodList;
QString version="1.01";
void printParams()
{
    printf("Interval Genetic software. Version %s\n",version.toStdString().c_str());
    printf("*********************************************************************************************\n");
    printf("--filename=<name>\tSet name as the dll file name\n");
    printf("--threads=<i>    \tSet t as the number of openmp threds.\n");
    printf("--debug=<yes|no> \tSpecify if debug will be used. Default is yes.\n");
    printf("--seed=<i>       \tSpecify the seed for the random number generator.\n");
    printf("--chromosomes=<i>\tSpecify the number of chromosomes for the genetic populations.\n");
    printf("--generations=<i>\tSpecify the maximum number of generations for the genetic populations.\n");
    printf("--selection_rate=<f> \tSpecify the value for selection rate.\n");
    printf("--mutation_rate=<f>  \tSpecify the value for mutation rate.\n");
    printf("--intervalmethod=<double|integer|none>     Specify the desired interval pre processing.\n");
    printf("--localmethod=<gradient|adam|bfgs|genetic> Specify the desired local method.\n");
    printf("--gradient_learningrate=<f>\tSet the value of gradient descent learning rate.\n");
    printf("--gradient_iterations=<i>  \tSet the number of maximum gradient descent iterations.\n");
    printf("--adam_learningrate=<f>    \tSet the learning rate for Adam method.\n");
    printf("--adam_b1=<f>              \tSet the parameter b1 of Adam method.\n");
    printf("--adam_b2=<f>              \tSet the parameter b2 of Adam method.\n");
    printf("--adam_iterations=<i>      \tSet the maximum number of iterations for the Adam method.\n");
    printf("--bfgs_iterations=<i>      \tSet the maximum number of iterations for the Bfgs method.\n");
    printf("--integer_samples=<i>      \tSet the number of samples for the integer interval method.\n");
    printf("--double_samples=<i>       \tSet the number of samples for the double intervale method.\n");
    printf("--param=<name>             \tSet a new parameter for the problem initialization.\n");
    printf("--value=<v>                \tSet the value for the previous parameter for the problem.\n");
    printf("--help           \tPrint this help screen and terminate.\n");
    exit(EXIT_SUCCESS);
}


QString checkInList(QStringList &lst,QString value)
{
  for(int i=0;i<lst.size();i++)
      if(value==lst[i])
          return value;
  return lst[0];
}

void parseCmdLine(QStringList args)
{
    intervalMethodList<<"double"<<"integer"<<"none";
    localMethodList<<"gradient"<<"adam"<<"bfgs"<<"genetic";

    QString lastParam="";
    for(int i=1;i<args.size();i++)
    {

        if(args[i]=="--help") printParams();

        QStringList sp=args[i].split("=");
        if(sp[0]=="--filename" ||  sp[0]=="-f")
            filename=sp[1];
        else
        if(sp[0]=="--intervalmethod")
            intervalMethod=checkInList(intervalMethodList,sp[1]);
        else
        if(sp[0]=="--localmethod")
            localMethod=checkInList(localMethodList,sp[1]);
        else
        if(sp[0]=="--debug")
            debug=sp[1]=="yes"?true:false;
        else
        if(sp[0]=="--integer_samples")
            integer_samples=sp[1].toInt();
        else
        if(sp[0]=="--double_samples")
            double_samples=sp[1].toInt();
        else
        if(sp[0]=="--adam_b1")
            adam_b1=sp[1].toDouble();
        else
        if(sp[0]=="--adam_b2")
            adam_b2=sp[1].toDouble();
        else
        if(sp[0]=="--adam_learningrate")
            adam_learningrate=sp[1].toDouble();
        else
        if(sp[0]=="--adam_iterations")
            adam_iterations=sp[1].toInt();
        else
        if(sp[0]=="--gradient_learningrate")
            gradient_learningrate=sp[1].toDouble();
        else
        if(sp[0]=="--gradient_iterations")
            gradient_iterations=sp[1].toDouble();
        else
        if(sp[0]=="--bfgs_iterations")
            bfgs_iterations=sp[1].toInt();
        else
        if(sp[0]=="--seed" ||  sp[0]=="-r")
            randomSeed=sp[1].toInt();
        else
        if(sp[0]=="--chromosomes")
            chromosomes=sp[1].toInt();
        else
        if(sp[0]=="--generations")
            maxGenerations=sp[1].toInt();
        else
        if(sp[0]=="--selection_rate")
            selection_rate=sp[1].toDouble();
        else
        if(sp[0]=="--mutation_rate")
            mutation_rate=sp[1].toDouble();
        else
        if(sp[0]=="--threads")
            threads=sp[1].toInt();
        else
        if(sp[0]=="--param" || sp[0]=="-p")
            lastParam=sp[1];
        else
        if(sp[0]=="--value" "" || sp[0]=="-v")
        {
            //thre cases
            //int
            //double
            //string
            //all double numbers should have .
            bool assigned=false;
            if(sp[1].contains("."))
            {
                bool b;
                qreal f=sp[1].toDouble(&b);
                if(b) {
                    assigned=true;
                    params[lastParam]=f;
                }
            }

            if(!sp[1].contains("."))
            {
                bool b1;
                qint64 ix=sp[1].toInt(&b1);
                if(b1){assigned=true; params[lastParam]=ix;}
            }
            if(!assigned)
            params[lastParam]=sp[1];
        }
        else
        {
            qDebug()<<"Wrong parameter "<<sp[0]<<endl;
            exit(printf("Wrong arguments\n"));
        }
    }
}

IntervalData runIntegerInterval(DllProblem *p,Data &bestp)
{
    IntegerGenetic gen(p,chromosomes,threads);
    gen.setSamples(integer_samples);
    gen.setMutationRate(mutation_rate);
    gen.setSelectionRate(selection_rate);
    IntervalData bestx;
    Interval besty;
    for(int g=1;g<=maxGenerations;g++)
    {
        gen.nextGeneration();
        gen.getBest(bestx,besty);
        if(debug)
        {
            cout<<"g="<<g<<" BESTY "<<besty<<endl;
            if(g%20==0)    p->printData(bestx);
        }
    }
    bestp=gen.getBestPoint();
    IntervalData bestMargin=bestx;
    return bestMargin;
}

IntervalData runDoubleInterval(DllProblem *p,Data &bestp)
{
    IntervalGenetic gen(filename,params,chromosomes,threads);
    gen.setSamples(double_samples);
    gen.setMutationRate(mutation_rate);
    gen.setSelectionRate(selection_rate);
    IntervalData bestx;
    Interval besty;

    for(int g=1;g<=maxGenerations;g++)
    {
        gen.nextGeneration();
        gen.getBest(bestx,besty);
        if(debug)
        {
            cout<<"g="<<g<<" BESTY "<<besty<<endl;
            if(g%20==0)    p->printData(bestx);
        }
    }
    bestp=gen.getBestPoint();
    IntervalData bestMargin=bestx;
    return bestMargin;
}

int main(int argc,char **argv)
{
    QCoreApplication app(argc,argv);
    parseCmdLine(app.arguments());
    if(filename=="") printParams();
    setlocale(LC_ALL,"C");
    QJsonObject neuralObject;
    DllProblem p(filename,params);
    p.setParameter("normalTrain",0);
    srand(randomSeed);
    Data bestgeneticx;
    bestgeneticx.resize(p.getDimension());
    IntervalData bestMargin=p.getMargins();
    if(intervalMethod=="integer") bestMargin=runIntegerInterval(&p,bestgeneticx);
    else
    if(intervalMethod=="double") bestMargin=runDoubleInterval(&p,bestgeneticx);

    double avgTrainError=0.0;
    double avgTestError=0.0;
    double avgClassError=0.0;
    double minTrainError=0.0;
    double minTestError=0.0;
    double minClassError=0.0;
    int tries=30;
    p.setParameter("normalTrain",1);
     Problem np(&p,bestMargin);

//#pragma omp parallel for shared(adam_b1,adam_b2,adam_learningrate,adam_iterations) num_threads(threads)
    for(int i=1;i<=tries;i++)
    {
        Data bestx;
        double besty;
        srand(randomSeed+i);

        if(localMethod=="gradient")
        {
            BoundedGradientDescent Optimizer(&np);
            Optimizer.setLearningRate(gradient_learningrate);
            Optimizer.setIterations(gradient_iterations);
            Optimizer.Optimize();
            bestx = Optimizer.getBestX();
            besty = Optimizer.getBestY();
        }
        else
        if(localMethod=="adam")
        {
            ConstrainedAdam Optimizer(&np);
            Optimizer.setB1(adam_b1);
            Optimizer.setB2(adam_b2);
            Optimizer.setLearningRate(adam_learningrate);
            Optimizer.setIterations(adam_iterations);
            Optimizer.Optimize();
            bestx = Optimizer.getBestX();
            besty = Optimizer.getBestY();
        }
        else
        if(localMethod=="genetic")
        {
            DoublePop pop(chromosomes,&np);
            pop.setSelectionRate(selection_rate);
            pop.setMutationRate(mutation_rate);
            if(intervalMethod!="none") pop.setBest(bestgeneticx,0.0);
            pop.Solve();
            bestx=pop.getBestGenome();
            besty=pop.getBestFitness();
        }
        else
        if(localMethod=="bfgs")
        {
		if(intervalMethod=="none")
             bestx=np.getUniformRandomPoint();
		else bestx=bestgeneticx;
            besty=tolmin(bestx,&np,bfgs_iterations);
        }

       QJsonObject result=p.done(bestx);
//#pragma omp critical
{
       avgTrainError+=besty;

        if(!result.isEmpty())
        {
            avgTestError+=result["testError"].toDouble();
            avgClassError+=result["classError"].toDouble();

           printf("GENETIC STEP[%4d]=%20.10lg %20.10lg%% ",i,besty,result["classError"].toDouble());
           cout<<result["string"].toString().toStdString()<<endl;
        if(i==1 || besty<minTrainError)
        {
            minTrainError=besty;
            minTestError=result["testError"].toDouble();
            minClassError=result["classError"].toDouble();
        }
       }
 }
    }
    printf("AVERAGE TRAIN ERROR: %20.10lg\n",avgTrainError/tries);
    printf("AVERAGE TEST  ERROR: %20.10lg\n",avgTestError/tries);
    printf("AVERAGE CLASS ERROR: %20.10lg%%\n",avgClassError/tries);
    printf("MINIMUM TRAIN ERROR: %20.10lg\n",minTrainError);
    printf("MINIMUM TEST  ERROR: %20.10lg\n",minTestError);
    printf("MINIMUM CLASS ERROR: %20.10lg%%\n",minClassError);
    //*/
    return 0;
}
