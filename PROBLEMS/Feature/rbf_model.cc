# include <rbf_model.h>
//# define CLASSMODEL

Rbf::Rbf(Mapper *m)
	:Model(m)
{
	centers = NULL;
	variances = NULL;
	weights = NULL;
	input  = NULL;
	weight.resize(0);
	for(int i=0;i<10000;i++) randNumber.push_back(drand48());
}

int isTrain2=0;

double Rbf::neuronOutput( vector<double> &x, vector<double> &patt, unsigned pattDim, unsigned offset,
                            bool &failed)
{
    failed=false;
    double out = 0;
    for(unsigned i = 0; i < pattDim;i++){
        out += (patt[i] - x[offset*pattDim + i]) * (patt[i] - x[offset*pattDim + i]);
    }
    double df=(-out / (x[num_weights*pattDim+offset] * x[num_weights*pattDim+offset]) );
  //  if(fabs(df)>100)return 1.0;// return 1000;
    return exp(df);
}

arma::vec Rbf::train( vector<double> &x, bool &failed ){
    int nodes = num_weights;
    arma::mat A = arma::zeros(xpoint.size(),nodes);
    arma::vec B(ypoint.size());
    for(unsigned i = 0; i < xpoint.size(); i++){
        B.at(i) = ypoint[i];
        for(unsigned j = 0; j < nodes;j++){
            A.at(i,j) = neuronOutput(x, xpoint[i], xpoint[0].size() , j,failed);

        }
    }
    failed=false;
 arma::vec RetVal;
    try {
             RetVal= arma::vec(arma::pinv(A)*B);

    }  catch (exception e) {
        failed=true;
    }
    if(failed || RetVal.has_nan() || RetVal.has_inf()) {
        RetVal = arma::zeros(1);
        failed=true;
        }
    return RetVal;
}

double Rbf::train1()
{
	int noutput=1;
	if(weight.size() != noutput * num_weights)
	{
		weight.resize(num_weights*noutput);
		setDimension(num_weights*noutput);
		if(centers)
		{
		delete[] centers;
		delete[] variances;
		delete[] weights;
		delete[] input;
		}
		centers = new double[num_weights * pattern_dimension];
		variances = new double[num_weights * pattern_dimension];
		weights = new double[num_weights*noutput];
		input = new double[pattern_dimension*xpoint.size()];
	Output=new double[noutput * xpoint.size()];
	}
	

	Matrix xx;
	xx.resize(original_dimension);
	for(int i=0;i<xpoint.size();i++) 
	{
		int d=mapper->map(origx[i],xpoint[i]);
		if(!d) 
		{
			return 1e+8;
		}
		for(int j=0;j<pattern_dimension;j++)
		{
			
			input[i*pattern_dimension+j]=xpoint[i][j];
			if(fabs(xpoint[i][j])>=1e+10 || isnan(xpoint[i][j]) || isinf(xpoint[i][j]))
			{
				return 1e+8;
			}
		}
		Output[i]=ypoint[i];
	}
double v =0.0;

        Kmeans(rnd,input,centers,variances,
			xpoint.size(),pattern_dimension,num_weights);

	
        int nodes=num_weights;
        double errorSum=0.0;
        vector<double> x;
        x.resize(pattern_dimension * num_weights+num_weights);
        for(int i=0;i<nodes * pattern_dimension;i++)
            x[i]=centers[i];
        for(int i=0;i<nodes;i++)
        {
            double sum=0.0;
            for(int j=0;j<pattern_dimension;j++)
                sum+=variances[i*pattern_dimension+j];

            if(sum<0.01) sum = 0.01;
            x[nodes*pattern_dimension+i]=sum;
        }

        bool failed=false;
        Linear = train(x,failed);
        if(failed) return 1e+100;
        if(Linear.size()<=1) return 1e+100;
        for(unsigned i = 0; i < xpoint.size(); i++){
            Data pattern = xpoint[i];
            arma::vec neuronOuts(nodes);
        bool failed=false;
            for(unsigned j = 0; j < nodes;j++){
                neuronOuts[j] = neuronOutput(x,pattern,pattern.size(),j,failed);
            }
            double tempOut = arma::dot(neuronOuts,Linear);
            errorSum += ( tempOut - ypoint[i] ) * ( tempOut - ypoint[i] );
        }
  /* 
        int icode=train_rbf(pattern_dimension,num_weights,noutput,xpoint.size(),
			centers,variances,weights,input,Output);

	v=funmin(weight);
	if(icode==1) 
	{
		if(isTrain2)
	return	1e+8;
	}
	return v;
   */
    return errorSum;
}

double Rbf::train2()
{
	isTrain2=1;
	return train1();
}

double Rbf::output(Matrix &x)
{
	
        vector<double> xp;
        int nodes=num_weights;
        xp.resize(pattern_dimension * num_weights+num_weights);
        for(int i=0;i<nodes * pattern_dimension;i++)
            xp[i]=centers[i];
        for(int i=0;i<nodes;i++)
        {
            double sum=0.0;
            for(int j=0;j<pattern_dimension;j++)
                sum+=variances[i*pattern_dimension+j];

            if(sum<0.01) sum = 0.01;
            xp[nodes*pattern_dimension+i]=sum;
        }
            Data pattern = x;
            arma::vec neuronOuts(nodes);
        	bool failed=false;
            for(unsigned j = 0; j < nodes;j++){
                neuronOuts[j] = neuronOutput(xp,pattern,pattern.size(),j,failed);
            }
            double tempOut = arma::dot(neuronOuts,Linear);
	    return tempOut;
	int noutput=1;
#ifdef CLASS
	noutput=2;
#endif
	double v[noutput];
	double *xt=new double[x.size()];
	double penalty=0.0;
	int i;
	for( i=0;i<x.size();i++) 
	{
		xt[i]=x[i];
		penalty+=(fabs(x[i])>=10.0)*1000.0;
	}
	create_rbf(pattern_dimension,num_weights,noutput,
			centers,variances,weights,xt,v);
	delete[] xt;
#ifndef CLASS
	return v[0];
#else
	return (v[0]>v[1]?0:1);	
#endif
}

void   Rbf::getDeriv(Matrix x,Matrix &g)
{
}

Rbf::~Rbf()
{
	delete[] centers;
	delete[] variances;
	delete[] weights;
	delete[] input;

}
