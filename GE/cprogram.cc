# include <GE/cprogram.h>
# include <stdio.h>
# include <math.h>
# include <QDebug>



Cprogram::Cprogram(int dim,int pdim)
{
	dimension = dim;
	pdimension = pdim;
	makeTerminals();
	makeNonTerminals();
	makeRules();
}

int	Cprogram::newRule()
{
	int r;
	r=rule.size();
	rule.resize(r+1);
	rule[r]=new Rule();
	return r;
}

void	Cprogram::makeTerminals()
{
	Number.set("Number",1);
	In.set("in",1);
	Delim.set("#",1);
	PI.set("pi",1);
	Plus.set("+",1);
	Minus.set("-",1);
	Mult.set("*",1);
	Div.set("/",1);
	Pow.set("^",1);
	Comma.set(",",1);
	Dot.set(".",1);
	Lpar.set("(",1);
	Rpar.set(")",1);

	Sin.set("sin",1);
	Cos.set("cos",1);
	Exp.set("exp",1);
	Log.set("log",1);
	Not.set("!",1);
	addN.set("Add",1);
	subN.set("Sub",1);
	multN.set("Mult",1);
	divN.set("Div",1);
	
	Abs.set("abs",1);
	Sqrt.set("sqrt",1);
	Tan.set("tan",1);
	Int.set("int",1);
	Log10.set("log10",1);
	Gt.set(">",1);
	Ge.set(">=",1);
	Lt.set("<",1);
	Le.set("<=",1);
	Eq.set("=",1);
	Neq.set("!=",1);
	And.set("&",1);
	Or.set("|",1);
	boolExpr.set("jump",1);
	XX.resize(dimension);
	Avg.set("sig",1);
	vars="";
    DoubleDot.set(":",1);
	for(int i=0;i<dimension;i++)
	{
		char str[100];
		sprintf(str,"x%d",i+1);
		XX[i].set(str,1);
		vars=vars+str;
		if(i<dimension-1) vars=vars+",";
	}

	Digit.resize(10);
	for(int i=0;i<10;i++)
	{
		char str[100];
		sprintf(str,"%d",i);
		Digit[i].set(str,1);
	}
}

void	Cprogram::makeNonTerminals()
{
	Start.set("START",0);
	xlist.set("XLIST",0);
	DigitList.set("DIGITLIST",0);
	Digit0.set("DIGIT0",0);
	Digit1.set("DIGIT1",0);
	XXlist.set("XXLIST",0);
	Expr.set("EXPR",0);
	function.set("FUNCTION",0);
	binaryop.set("BINARYOP",0);
	terminal.set("TERMINAL",0);
	MinMax.set("MINMAX",0);
	BooleanExpr.set("BOOLEXPR",0);
	boolop.set("BOOLOP",0);
	inexpr.set("INEXPR",0);
	infunction.set("INFUNCTION",0);
	andor.set("ANDOR",0);
	nearfunction.set("NEARFUNCTION",0);
}

void	Cprogram::makeRules()
{
    int r;
    r=newRule();
    rule[r]->addSymbol(&BooleanExpr);
    Start.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&XXlist);
    rule[r]->addSymbol(&Comma);
    rule[r]->addSymbol(&Digit0);
    rule[r]->addSymbol(&Comma);
    rule[r]->addSymbol(&Digit0);
    BooleanExpr.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&XXlist);
    rule[r]->addSymbol(&Comma);
    rule[r]->addSymbol(&Digit0);
    rule[r]->addSymbol(&Comma);
    rule[r]->addSymbol(&Digit0);
    rule[r]->addSymbol(&DoubleDot);
    rule[r]->addSymbol(&BooleanExpr);
    BooleanExpr.addRule(rule[r]);


	for(int i=0;i<dimension;i++)
	{
		r=newRule();
		rule[r]->addSymbol(&XX[i]);
		XXlist.addRule(rule[r]);
	}

	r=newRule();
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Mult);
	rule[r]->addSymbol(&XXlist);
	XXlist.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&XXlist);
	rule[r]->addSymbol(&binaryop);
	rule[r]->addSymbol(&XXlist);
	XXlist.addRule(rule[r]);

    for(int i=0;i<7;i++)
	{
		r=newRule();
		rule[r]->addSymbol(&Digit[i]);
		Digit0.addRule(rule[r]);
	}

}
# define SHRINK 1
# define EXPAND 2
# define SHRINK_EXPAND 3
# define EXPAND_SHRINK 4
# define SHRINK_SHRINK 5
# define EXPAND_EXPAND 6

/** oldmargin einai to arxiko dianysma baron
 *  kai newMargin einai to neo dianysma baron **/
bool Cprogram::evalProgram(string expr,
                 vector<Interval> &oldMargin,
                 vector<Interval> &newMargin)
{
    //example program exp=(x1,0,1):(x2,1,1):
    QString st = QString::fromStdString(expr);
    if(st.contains("NONE")) return false;
    QStringList items = st.split(DoubleDot.getName().c_str());
    newMargin.resize(oldMargin.size());
    for(int i=0;i<(int)newMargin.size();i++)
        newMargin[i]=oldMargin[i];

    for(int i=0;i<items.size();i++)
    {
        QString it = items[i];
        bool ok;
        QStringList values = it.split(Comma.getName().c_str());
        int index = values[0].mid(1).toInt(&ok);
       if(!ok) return false;

        int left = values[1].toInt(&ok);
        if(!ok) return false;
        int right = values[2].toInt(&ok);
        if(!ok) return false;
        if(left==SHRINK)
        {
            newMargin[index-1].shrinkLeft();
        }
        else
        if(left==EXPAND)
        {
            newMargin[index-1].expandLeft();
        }
	else
	if(left==SHRINK_EXPAND)
	{
            newMargin[index-1].shrinkLeft();
            newMargin[index-1].expandLeft();
	}	
	else
	if(left==EXPAND_SHRINK)
	{
            newMargin[index-1].expandLeft();
            newMargin[index-1].shrinkLeft();
	}	
	else
	if(left==SHRINK_SHRINK)
	{
            newMargin[index-1].shrinkLeft();
            newMargin[index-1].shrinkLeft();
	}
	else
	if(left==EXPAND_EXPAND)
	{
            newMargin[index-1].expandLeft();
            newMargin[index-1].expandLeft();
	}	

        if(right==SHRINK)
        {
            newMargin[index-1].shrinkRight();
        }
        else
        if(right==EXPAND)
        {
            newMargin[index-1].expandRight();
        }
	else
	if(right==SHRINK_EXPAND)
	{
            newMargin[index-1].shrinkRight();
            newMargin[index-1].expandRight();
	}
	else
	if(right==EXPAND_SHRINK)
	{
            newMargin[index-1].expandRight();
            newMargin[index-1].shrinkRight();
	}
	else
	if(right==EXPAND_EXPAND)
	{
            newMargin[index-1].expandRight();
            newMargin[index-1].expandRight();
	}
	else
	if(right==SHRINK_SHRINK)
	{
            newMargin[index-1].shrinkRight();
            newMargin[index-1].shrinkRight();
	}
	//only for GE?
	    if(newMargin[index-1].leftValue()<0) 
		    newMargin[index-1]=Interval(0,newMargin[index-1].rightValue());
    }
    return true;
}

string	Cprogram::printRandomProgram(vector<int> &genome,int &redo)
{
    string str="";
    int count=0;
    Rule *r;
    //genome[count]%Start.getCountRules();
    r=Start.getRule(genome[count]%Start.getCountRules());
    redo = 0;
    str=r->printRule(genome,count,redo);
    if(redo>=1) return "NONE";
    return str;
}

int		Cprogram::Parse(string expr)
{
    int retcode=parser.Parse(expr,vars);
    if(retcode!=-1)
    {
        qDebug()<<"RetCode is "<<retcode<<" String is "<<
                  QString(expr.c_str())<<" Vars are "<<QString(vars.c_str())<<endl;
    }
    return retcode==-1;//(parser.Parse(expr,vars)==-1);
}

int		Cprogram::EvalError()
{
	return	parser.EvalError();
}

double	Cprogram::Eval( const double *X)
{
		return parser.Eval(X);
}

Symbol	*Cprogram::getStartSymbol()
{
	return &Start;
}

Cprogram::~Cprogram()
{
	for(int i=0;i<rule.size();i++)
		delete rule[i];
}
