# include <nnprogram.h>
#include <sys/types.h>
# include <omp.h>
#include <unistd.h>
NNprogram::NNprogram(int type,int pdimension,char *filename)
{
	FILE *fp=fopen(filename,"r");
	if(!fp) return;
	int d;
	fscanf(fp,"%d",&d);
    double scale_factor=10.0;
    mapper=new Mapper(d,scale_factor);
	fclose(fp);	
	model_type = type;
	if(type == MODEL_NEURAL)
		model = new Neural(mapper,1);
	else
	if(type == MODEL_RBF)
		model = new Rbf(mapper);
	else
		model = new KNN(mapper);
	pattern_dimension = pdimension;
	program = new Cprogram(d,pdimension);
	setStartSymbol(program->getStartSymbol());
	model->readPatterns(filename);
	pstring.resize(pattern_dimension);
	pgenome.resize(0);
	random_seed=1;
}
int thread()
{
    return omp_get_thread_num();
}

NNprogram::NNprogram()
{

}

void NNprogram::setData(int type,int pdimension,double scale,vector<Matrix> &xdata,Matrix &ydata)
{
    int d=xdata[0].size();
    mapper=new Mapper(d,scale);
    model_type = type;
    if(type == MODEL_NEURAL)
        model = new Neural(mapper,1);
    else
    if(type == MODEL_RBF)
        model = new Rbf(mapper);
    else
        model = new KNN(mapper);
    pattern_dimension = pdimension;
    program = new Cprogram(d,pdimension);
    setStartSymbol(program->getStartSymbol());
    model->readPatterns(xdata,ydata);
    pstring.resize(pattern_dimension);
    pgenome.resize(0);
}

NNprogram::NNprogram(int type,int pdimension,double scale,vector<Matrix> &xdata,Matrix &ydata)
{
    setData(type,pdimension,scale,xdata,ydata);
}

string  NNprogram::printF(vector<double> &genome)
{

    string ret="";
    if(fpgenome.size()!=genome.size()/pattern_dimension)
        fpgenome.resize(genome.size()/pattern_dimension);
    for(int i=0;i<pattern_dimension;i++)
    {
        for(int j=0;j<genome.size()/pattern_dimension;j++)
            fpgenome[j]=genome[i*genome.size()/pattern_dimension+j];
        int redo=0;
        pstring[i]=printRandomProgram(fpgenome,redo);
        if(redo>=REDO_MAX) return "";
        char str[10];
        sprintf(str,"f%d(x)=",i+1);
        ret+=str;
        ret+=pstring[i];
        ret+="\n";
    }
    return ret;
}
string	NNprogram::printF(vector<int> &genome)
{
	string ret="";
	if(pgenome.size()!=genome.size()/pattern_dimension)
		pgenome.resize(genome.size()/pattern_dimension);
	for(int i=0;i<pattern_dimension;i++)
	{
		for(int j=0;j<genome.size()/pattern_dimension;j++)
			pgenome[j]=genome[i*genome.size()/pattern_dimension+j];
		int redo=0;
		pstring[i]=printRandomProgram(pgenome,redo);
		if(redo>=REDO_MAX) return "";
		char str[10];
		sprintf(str,"f%d(x)=",i+1);
		ret+=str;
		ret+=pstring[i];
		ret+="\n";
	}
	return ret;
}

void	NNprogram::setSeed(int r)
{
	random_seed=r;
}

GRand *NNprogram::getRand()
{
	return &rnd;
}
double 	NNprogram::fitness(vector<int> &genome)
{
	double value=0.0;
	if(pgenome.size()!=genome.size()/pattern_dimension)
		pgenome.resize(genome.size()/pattern_dimension);
	for(int i=0;i<pattern_dimension;i++)
	{
		for(int j=0;j<pgenome.size();j++)
			pgenome[j]=genome[i*genome.size()/pattern_dimension+j];
		int redo=0;
		pstring[i]=printRandomProgram(pgenome,redo);
		if(redo>=REDO_MAX) return -1e+8;
	}
	mapper->setExpr(pstring);
    	rnd.seed(random_seed+getpid());
    	model->setRand(&rnd);
	model->setPatternDimension(pattern_dimension);
	if(model_type==MODEL_NEURAL) model->randomizeWeights();
	value=model->train1();
	if(isnan(value) || isinf(value)) return -1e+8;
	return -value;
}


void	NNprogram::getStatistics(vector<int> &genome,vector<Matrix> &testx,Matrix &testy,double &test,double &eclass)
{
	fitness(genome);
	int tries=30;
	test = 0.0;
	eclass  = 0.0;
	for(int i=1;i<=tries;i++)
	{
    		rnd.seed(random_seed+i);
    		model->setRand(&rnd);
		double d0=model->train2();
		double d1=model->testError(testx,testy);
		double d2=model->classTestError(testx,testy);
		if(d1>=1e+4) {tries--;continue;}
		printf("MODEL[%d]=%lf %lf %lf \n",i,d0,d1,d2);
		test+=d1;
		eclass+=d2;
	}
	test/=tries;
	eclass/=tries;
}

double 	NNprogram::fitness(vector<double> &genome)
{
    double value=0.0;
    if(fpgenome.size()!=genome.size()/pattern_dimension)
        fpgenome.resize(genome.size()/pattern_dimension);
    for(int i=0;i<pattern_dimension;i++)
    {
        for(int j=0;j<fpgenome.size();j++)
        {
            fpgenome[j]=genome[i*genome.size()/pattern_dimension+j];
        }
        int redo=0;
        pstring[i]=printRandomProgram(fpgenome,redo);
        if(redo>=REDO_MAX) return -1e+8;
    }
    mapper->setExpr(pstring);
        rnd.seed(random_seed+getpid());
        model->setRand(&rnd);
    model->setPatternDimension(pattern_dimension);
    if(model_type==MODEL_NEURAL) model->randomizeWeights();
    value=model->train1();
    if(isnan(value) || isinf(value)) return -1e+8;
    return -value;
}

Model	*NNprogram::getModel()
{
	return model;
}

Mapper	*NNprogram::getMapper()
{
	return mapper;
}

NNprogram::~NNprogram()
{
	delete program;
	delete model;
	delete mapper;
}

