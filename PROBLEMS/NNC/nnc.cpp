#include "nnc.h"
# include "interval.h"
# include "omp.h"
# include "tolmin.h"
# include "converter.h"
# include "gensolver.h"
# include <gpopulation.h>
extern "C"
{
	int double2int(double x)
	{
		return (int)(fabs(ceil(x)));
	}
    //parameters
    const int maxthreads=64;
    double leftMargin=0.0;
   double rightMargin=16;//512;
    QString trainfile="";
    QString testfile="";
    QString urlpath="http://itsoulos.teiep.gr/genclass/";
    //QString urlpath="https://app-1525680166.000webhostapp.com/";
    int chromosomeSize=50;
    //end of parameters
    //names: leftmargin, rightmargin, trainfile, testfile, chromosomesize

    int populationCount=50;
    int dimension=1;
    vector<Data> trainx;
    Data trainy;
    vector<Data> testx;
    Data testy;
    Data dclass;
    int dcount=0;
    vector<NNCNeuralProgram> program;

    QByteArray readNetworkFile(QString file)
    {
        QEventLoop loop;
        QUrl serviceUrl = QUrl(file);
        QNetworkAccessManager *networkManager = new QNetworkAccessManager();
        QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
        QNetworkRequest networkRequest(serviceUrl);
        networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"text/plain");
        QNetworkReply *rep=networkManager->get(networkRequest);

        new QReplyTimeout(rep, 30000);

        loop.exec();//hangs
        QByteArray bts = rep->readAll();
        return bts;
    }

    QByteArray readLocalFile(QString file)
    {
       //if file starts with file://
            setlocale(LC_ALL,"C");
        file=file.mid(QString("file://").size());
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
        if(obj.contains("rightmargin"))
            rightMargin=obj["rightmargin"].toDouble();
        if(obj.contains("chromosomesize"))
            chromosomeSize=obj["chromosomesize"].toInt();

        loadTrain();
        loadTest();
        program.resize(maxthreads);
        for(int i=0;i<maxthreads;i++)
        {
            program[i].setAll(dimension,trainx,trainy,testx,testy);
       // program[i].setData(MODEL_RBF,features,scale_factor,trainx,trainy);
       // program[i].getModel()->setNumOfWeights(weights);
        }
        populationCount=chromosomeSize;
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

    double	funmin(vector<double> &x)
    {
      setlocale(LC_ALL,"C");
      vector<int>  genome;
      genome.resize(getdimension());
      {
      for(int i=0;i<getdimension();i++) 
      {
	      genome[i]=double2int(x[i]);
	      //(int)fabs(x[i]);
      }
      }
	program[thread()].neuralparser->sigcount=0;
	program[thread()].neuralparser->violcount=0;
      double f=program[thread()].fitness(genome);
    //double percent=program[thread()].neuralparser->violcount*1.0/program[thread()].neuralparser->sigcount;
    //return -f * (1.0+percent);
      return -f;
    }
    double dmax(double a,double b){return a>b?a:b;}

    void    granal(vector<double> &x,vector<double> &g)
    {
        for(int i=0;i<x.size();i++)
             {
                 double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
                 x[i]+=eps;
                 double v1=funmin(x);
                 x[i]-=2.0 *eps;
                 double v2=funmin(x);
                 g[i]=(v1-v2)/(2.0 * eps);
                 x[i]+=eps;
             }
	funmin(x);
    }

    QJsonObject    done(Data &x)
    {
	    
        double sum=0.0;
        vector<int> genome;
	genome.resize(getdimension());
        for(int i=0;i<getdimension();i++)
        genome[i]=255;//double2int(x[i]);//(int)fabs(x[i]);

	
	GPopulation pop(500,getdimension(),genome,&program[thread()]);
	for(int i=1;i<=500;i++)
	{
		pop.nextGeneration();
		double f = pop.getBestFitness();
		if(i%50==0)
        {
        	fprintf(stderr,"nnc[%d]=%lf\n",i,f);
            for(int ik=1;ik<=10;ik++)
            {
            int pos = rand() % 500;
            pop.getGenome(pos,genome);
            double ff=program[thread()].fitness(genome);

            MinInfo Info1;
            Info1.iters=2001;
            Info1.problem=&program[thread()];
            Data w;
            program[thread()].neuralparser->getWeights(w);
            Data x1,x2;
            x1.resize(w.size());
            x2.resize(w.size());
            for(int i=0;i<w.size();i++)
            {
                x1[i]=-5.0 *fabs(w[i]);
                x2[i]= 5.0 *fabs(w[i]);
            }
            program[thread()].neuralparser->setleftmargin(x1);
            program[thread()].neuralparser->setrightmargin(x2);
            double tvalue = tolmin(w,Info1);
            printf("Trial fitness = %20.10lg\n",tvalue);
            Converter con(w,w.size()/(dimension+2),dimension);
            con.convert(genome);
            for(int i=0;i<genome.size();i++)
            {
                //if(abs(trial_genome[i])>255) trial_genome[i]=0;
            }

            pop.setGenome(pos,genome,-tvalue);
            }
        }
	}
	genome= pop.getBestGenome();
	
     	double ff=program[thread()].fitness(genome);
        double avg_test=program[thread()].getTestError();
        double avg_class=program[thread()].getClassTestError();
	
 	int tries=0;
   	MinInfo Info1;
        Info1.iters=2001;
        Info1.problem=&program[thread()];
	double value=0.0;
	double old_f=0.0;
	Data w;
        program[thread()].neuralparser->getWeights(w);
	Data x1,x2;
	x1.resize(w.size());
	x2.resize(w.size());
	for(int i=0;i<w.size();i++)
	{
		x1[i]=-5.0 *fabs(w[i]);
		x2[i]= 5.0 *fabs(w[i]);
	}
	program[thread()].neuralparser->setleftmargin(x1);
	program[thread()].neuralparser->setrightmargin(x2);
	program[thread()].neuralparser->setWeights(w);
    value=tolmin(w,Info1);
    program[thread()].neuralparser->setWeights(w);
	value = program[thread()].getTrainError();
    avg_test=program[thread()].getTestError();
    avg_class=program[thread()].getClassTestError();
    QJsonObject result;
    result["nodes"]=10;
    result["testError"]=avg_test;
    result["classError"]=avg_class;
	QString bestProgram=program[thread()].printProgram(genome).c_str();
	printf("Train Error %10.5lf Test Error %10.5lf Class Error %10.5lf%%\n",
			value,result["testError"].toDouble(),result["classError"].toDouble());
        result["string"]=bestProgram;

        return result;
	
    }

}
