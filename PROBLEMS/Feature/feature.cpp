#include "feature.h"
#include "nnprogram.h"
# include "gpopulation.h"
#    include <interval.h>
# include <omp.h>
# include <adept.h>
# include <adept_arrays.h>


extern "C"
{
    //parameters
    const int maxthreads=64;
    double leftMargin=0.0;
    double rightMargin=5012.0;
    int random_seed=1;
    QString trainfile="";
    QString testfile="";
    QString urlpath="http://itsoulos.teiep.gr/feature/";
    //QString urlpath="file://home/sheridan/CLASSIFICATION/TENFOLD/";
    int chromosomeSize=40;
    int features=1;
    double scale_factor=10.0;
    QString logfile="feature.txt";
    QString outputdirectory="CONSTRUCTED/";//where the program will print the new features
    //end of parameters
    //names: leftmargin, rightmargin, logifle,trainfile, testfile,
    //       chromosomesize, features,weights,factor

    int populationCount=0;
    int dimension=1;
    int weights=20;
    int dcount=0;
    QVector<NNprogram> program;
    vector<int> genome;

    typedef vector<double> Data;
    vector<Data> trainx;
    Data trainy;
    vector<Data> testx;
    Data testy;
    Data dclass;

//This method reads a file from http location file
QByteArray readNetworkFile(QString file)
{
    QEventLoop loop;
    QUrl serviceUrl = QUrl(file);
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
    QNetworkRequest networkRequest(serviceUrl);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"text/plain");
    QNetworkReply *rep=networkManager->get(networkRequest);
    loop.exec();
    QByteArray bts = rep->readAll();
    rep->close();
    delete rep;
    return bts;
}

QByteArray readLocalFile(QString file)
{
   //if file starts with file://
   //file=file.mid(QString("file:/").size());
   QFile fp(file);
   fp.open(QIODevice::ReadOnly|QIODevice::Text);
   QByteArray st=fp.readAll();
   fp.close();
   return st;
}

void loadTrain()
{
   QFile fp(trainfile);
   fp.open(QIODevice::ReadOnly|QIODevice::Text);
   QTextStream st(&fp);
   st>>dimension;
   int count;
   st>>count;
   trainx.resize(count);
   trainy.resize(count);
   for(int i=0;i<count;i++)
   {
       trainx[i].resize(dimension);
       for(int j=0;j<dimension;j++)
       {
           st>>trainx[i][j];
       }
       st>>trainy[i];
       bool found=false;
       for(int j=0;j<dclass.size();j++)
       {
           if(fabs(dclass[j]-trainy[i])<1e-7)
           {
               found=true;
               break;
           }

       }
       if(!found)
           dclass.push_back(trainy[i]);
   }
   fp.close();
}



void loadTest()
{
    QFile fp(testfile);
    fp.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream st(&fp);
    st>>dimension;
    int count;
    st>>count;
    testx.resize(count);
    testy.resize(count);
    for(int i=0;i<count;i++)
    {
        testx[i].resize(dimension);
        for(int j=0;j<dimension;j++)
        {
            st>>testx[i][j];
        }
        st>>testy[i];
    }
    fp.close();
}





void    init(QJsonObject obj)
{
  setlocale(LC_ALL,"C");
    if(obj.contains("urlpath")) urlpath=obj["urlpath"].toString();
    if(!obj.contains("trainName") || !obj.contains("testName"))
    {
        trainfile="wine.train";
        testfile="wine.test";
    }
        else
    {
        trainfile=obj["trainName"].toString();
        testfile=obj["testName"].toString();
    }
    if(obj.contains("leftmargin"))
        leftMargin=obj["leftmargin"].toDouble();
    if(obj.contains("rightMargin"))
        rightMargin=obj["rightMargin"].toDouble();
    if(obj.contains("chromosomesize"))
        chromosomeSize=obj["chromosomesize"].toInt();
    if(obj.contains("features"))
        features=obj["features"].toInt();

    if(obj.contains("weights"))
        weights=obj["weights"].toInt();

    if(obj.contains("factor"))
        scale_factor=obj["factor"].toDouble();

    if(obj.contains("seed"))
        random_seed=obj["seed"].toInt();

    if(obj.contains("logfile"))
        logfile=obj["logfile"].toString();

    loadTrain();
    loadTest();
    program.resize(maxthreads);
    for(int i=0;i<maxthreads;i++)
    {
    program[i].setData(MODEL_RBF,features,scale_factor,trainx,trainy);
    program[i].getModel()->setNumOfWeights(weights);
    }
    populationCount=chromosomeSize * features;
    genome.resize(populationCount);
}

void setParameter(QString name,QVariant value)
{
    if(name=="normalTrain")
	{
		if(value.toInt()==1)
	for(int i=0;i<maxthreads;i++)
		program[i].setSeed(random_seed+i);
	}
}

int	getdimension()
{
   return populationCount;
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<getdimension();i++)
    {
            x[i]=Interval(leftMargin,rightMargin);
    }
}

QString replacePlus(QString s)
{
    return s.replace("+","#");
}


int thread()
{
    return omp_get_thread_num();
}
QJsonObject    done(Data &x)
{
    double sum=0.0;
	vector<int> genome;
	genome.resize(getdimension());
    for(int i=0;i<getdimension();i++)
        genome[i]=255;//(int)fabs(x[i]);
    double ff;
    string lastExpr="";

	GPopulation pop(500,getdimension(),genome,&program[thread()]);
	for(int i=1;i<=200;i++)
	{
		pop.nextGeneration();
		double f = pop.getBestFitness();
		if(i%20==0) printf("fc[%d]=%lf\n",i,f);
	//	if(pop.shouldTerminate()) break;
	}
	genome= pop.getBestGenome();
    for(int i=0;i<omp_get_num_threads();i++)
    {
         ff=program[i].fitness(genome);
         lastExpr=program[i].printF(genome);
	 printf("lastExpr = %s \n",lastExpr.c_str());
    }
 double avg_test_error=0.0;
 double avg_class_error=0.0;

 int ntimes=30;
 
 QString bestProgram=QString::fromStdString(program[0].printF(genome));
 
int threads=24;
 ntimes= threads;
     vector<string> pstring;
	 pstring.resize(features);
	for(int i=0;i<features;i++)
	{
        vector<int> pgenome;
        pgenome.resize(x.size()/features);
		for(int j=0;j<pgenome.size();j++)
		{
			
            		pgenome[j]=genome[i*genome.size()/features+j];
		}
		int redo=0;
		pstring[i]=program[0].printRandomProgram(pgenome,redo);
		printf("pstring[%d]=%s\n",i,pstring[i].c_str());
	}
	double etest,eclass;
	program[0].getStatistics(genome,testx,testy,etest,eclass);
	avg_test_error= etest;
	avg_class_error = eclass;
	/*
	vector<double> cclass;
	vector<double> ctest;
	cclass.resize(ntimes);
	ctest.resize(ntimes);
//#pragma omp parallel for num_threads(threads)
 for(int i=1;i<=ntimes;i++)
 {
	// Mapper *myMapper=new Mapper(dimension,0.0);
	//myMapper->setExpr(pstring);
//Neural *neural = new Neural(myMapper,i);
Rbf *neural = (Rbf *)program[0].getModel();// new Rbf(program[0].getMapper());
 neural->setRand(program[0].getRand());
 neural->readPatterns(trainx,trainy);
 neural->setPatternDimension(features);
 neural->setNumOfWeights(10);
 double ff=neural->train2();
 double testError=neural->testError(testx,testy);
 double classTestError=neural->classTestError(testx,testy);
 if(testError>1e+4) {ntimes--;continue;}
 cclass[i]=classTestError;
 ctest[i]=testError;
 printf("Starting thread %d  Values: %lf  %.2lf%% %lf \n",thread(),
		ff,	 
		 classTestError,testError);

 avg_test_error+=testError;
 avg_class_error+=classTestError;
 //delete neural;
 //delete myMapper;
 }
 avg_test_error/=ntimes;
 avg_class_error/=ntimes;*/
    QJsonObject result;
    result["nodes"]=10;
    result["testError"]=avg_test_error;
    result["classError"]=avg_class_error;
    result["string"]=bestProgram;
    return result;
}


double	funmin(vector<double> &x)
{
  setlocale(LC_ALL,"C");
  //return -program[thread()].fitness(x);
  
  vector<int>  genome;
  genome.resize(getdimension());
  for(int i=0;i<getdimension();i++) {
	  genome[i]=(int)fabs(x[i]);
	  if(genome[i]<0) genome[i]=0;
//	  if(genome[i]>255) genome[i]=255;
  }
  double f=program[thread()].fitness(genome);
  return -f;
}
double dmax(double a,double b){return a>b?a:b;}

void    granal(vector<double> &x,vector<double> &g)
{
    for(int i=0;i<x.size();i++)
         {
             double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
	     eps = 0.01;
             x[i]+=eps;
             double v1=funmin(x);
             x[i]-=2.0 *eps;
             double v2=funmin(x);
             g[i]=(v1-v2)/(2.0 * eps);
//	     printf("g = %lf f = %lf %lf  \n",g[i],v1,v2);
             x[i]+=eps;
         }

}

}
