# ifndef __CPROGRAM__H
# include <GE/symbol.h>
# include <GE/rule.h>
# include <GE/fparser.hh>
# include <interval.h>
# define SCALE	1

extern int mperror;
class Cprogram
{
	protected:
		string		vars;
		FunctionParser 	parser;
		int		dimension,pdimension;
		vector<Rule*> rule;
		Symbol		Start, Expr, Near,NotNear,nearfunction,Number,
					addN,subN,multN,divN,
					function, binaryop, terminal,
					XXlist,DigitList, Digit0, Digit1, 
				MinMax,Sin, Cos, Exp, Log, Abs, Sqrt,Avg, 
				Min, PI, Max,Delim,BooleanExpr,In,NotIn;
		Symbol		andor,Not,boolExpr,inexpr,infunction,
					boolop,Gt,Lt,Ge,Le,Eq,Neq,And,Or;
		Symbol		Plus, Minus, Mult, Div, Pow;
        Symbol		Lpar, Rpar, Dot, Comma,DoubleDot;
		Symbol		Tan, Int, Log10,Kernel,xlist;
		vector<Symbol>	Digit;
		vector<Symbol>	XX;
		int			newRule();
		void			makeTerminals();
		void			makeNonTerminals();
		void			makeRules();
	public:
		Cprogram(int dim,int pdim);
		int	Parse(string expr);
		double	Eval(const double *X);
		int	EvalError();
        string	printRandomProgram(vector<int> &genome,int &redo);
		Symbol	*getStartSymbol();
        bool evalProgram(string expr,
                         vector<Interval> &oldMargin,
                         vector<Interval> &newMargin);
		~Cprogram();
};
# define __CPROGRAM__H
# endif
