# include <program.h>		

Program::Program()
{
	start_symbol = NULL;
}

void	Program::setStartSymbol(Symbol *s)
{
	start_symbol = s;
}

Symbol	*Program::getStartSymbol() const
{
	return start_symbol;
}

string	Program::printRandomProgram(vector<double> &genome,int &redo)
{
    string str="";
    int count=0;
    Rule *r;
    int pos=(int)(genome[count]*start_symbol->getCountRules());
    if(pos==start_symbol->getCountRules()) pos--;
    r=start_symbol->getRule(pos);
    redo = 0;
    str=r->printRule(genome,count,redo);
    return str;
}

string	Program::printRandomProgram(vector<int> &genome,int &redo)
{
	string str="";
	int count=0;
	Rule *r;
	int pos=genome[count]%start_symbol->getCountRules();
	r=start_symbol->getRule(genome[count]%start_symbol->getCountRules());
	redo = 0;
	str=r->printRule(genome,count,redo);
	return str;
}


void	Program::updatePositive(vector<int> &genome)
{
}

void	Program::updateNegative(vector<int> &genome)
{
}

void	Program::updateAll(vector<int> &genome1,vector<int> &genome2)
{
}

double	Program::fitness(vector<int> &genome)
{
    return 0.0;
}


double	Program::fitness(vector<double> &genome)
{
    return 0.0;
}

Program::~Program()
{
}
