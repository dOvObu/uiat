#ifndef MODULE_3_H
#define MODULE_3_H
#include "module4.h"
#include <set>
#include <algorithm>
#include <iterator>

/*		Порядок объявления
	class Term;
	class Fid: public Term;
	class Id: public Term;
	class Statement: public Term;
	class Expr: public Statement;
	class Assign: public Statement;
	class N;
	class Call;
	class Var;
	class Empty;
	class BinOp;
		class Devide;
		class Multiply;
		class Minus;
		class Plus;
		class And;
		class Or;
		class Equal;
		class Less;
		class More;
		class LessOrEq;
		class MoreOrEq;
	class UnOp;
		class Minuss;
		class Not;
	class Brackets;
	class Body;
	class Array;
	class If;
	class Return;
	class Print;
	class Assert;
	class Func;
*/

class Term
{
public:
	char val;
	char getVal( )
	{
		return val;
	}
};





class Fid : public Term
{
public:
	std::string id;
	Fid( ) { this->val = 'f'; }
	Fid( const char* _id ) : id( _id )
	{
		this->val = 'f';
	}
	~Fid( ) {}
};
typedef std::shared_ptr<Fid> pFid;





class Statement : public Term
{
public:
	Statement( )
	{
		this->val = 's';
	}
	~Statement( ) {}
};
typedef std::shared_ptr<Statement> pStatement;
typedef std::vector<pStatement> vec_pStatement;





class Expr : public Statement
{
public:
	Expr( )
	{
		this->val = 'e';
	}
	~Expr( ) {}
};
typedef std::shared_ptr<Expr> pExpr;
typedef std::vector<pExpr> vec_pExpr;
typedef std::vector<vec_pExpr> vec_vec_pExpr;
typedef std::pair<pExpr, vec_pExpr> pair_pExpr_vecpExpr;
typedef std::vector<pair_pExpr_vecpExpr> vec_pair_pExpr_vecpExpr;





class ConditionFunc : public Expr
{
public:
	pExpr condition;
	
	pExpr ifFalse;
	pExpr ifTrue;

	ConditionFunc( )
	{
		this->val = '@';
	}
	~ConditionFunc( ) {}
};
typedef std::shared_ptr<ConditionFunc> pConditionFunc;





class Id : public Expr
{
public:
	std::string id;
	Id( )
	{
		this->val = 'i';
	}
	Id( const char* _id ) : id( _id )
	{
		this->val = 'i';
	}
	~Id( ) {}
};
typedef std::shared_ptr<Id> pId;
typedef std::vector<Id> vec_Id;
typedef std::vector< std::shared_ptr<Id> > vec_pId;


class CoreAssign : public Statement // хранится в мапе allAssignments и узлах дерева
{
public:
	bool active = true;
	std::vector<bool> if_expr;
	std::string name = "";
	// список	<	пар	<выражение, список_условий> >
	vec_pair_pExpr_vecpExpr meaning;
	CoreAssign( ) { this->val = ':'; }
	CoreAssign( const char _name_[] ):name(_name_), active(false) { this->val = ':'; }
	~CoreAssign( ) {}
};
typedef std::shared_ptr<CoreAssign> pCoreAssign;

class Assign : public Statement // хранится только в узлах дерева
{
public:
	bool& active;
	std::vector<bool>& if_expr;
	std::string& name;
	vec_pair_pExpr_vecpExpr& meaning;
	pCoreAssign core;
	unsigned pos;
	Assign( pCoreAssign _core_ ):
		core		(_core_),
		active	(_core_->active),
		if_expr	(_core_->if_expr),
		name		(_core_->name),
		meaning( _core_->meaning )
	{
		this->val = '~';
		pos = meaning.size( );
	}
	~Assign( ) {}
};
typedef std::shared_ptr<Assign> pAssign;
typedef std::vector<pAssign> vec_pAssign;



class N : public Expr
{
public:
	long long num = 0;
	N( int n ):num(n) { this->val = 'n'; }
	N( ) { this->val = 'n'; }
	~N( ) {}
};
typedef std::shared_ptr<N> pN;





class Call : public Expr
{
public:
	std::string nameOfFunc;
	vec_pExpr args;
	vec_pExpr pi_expr;
	pExpr data_to_return;
	Call( ) { this->val = 'c'; }
	~Call( ) {}
};
typedef std::shared_ptr<Call> pCall;





class Var : public Expr
{
public:
	std::string nameOfVar = "";
	bool withConditionFunc = false;
	pConditionFunc conditionFunc;
	pAssign argData = nullptr;
	pExpr data;
	Var( ) { this->val = 'v'; }
	
	~Var( ) {}
};
typedef std::shared_ptr<Var> pVar;





class Empty : public Expr
{
public:
	Empty( ) { this->val = '0'; }
	~Empty( ) {}
};
typedef std::shared_ptr<Empty> pEmpty;





class BinOp : public Expr
{
public:
	char type = 0;
	pExpr left, right;
	BinOp( ) { this->val = 'b'; }
	~BinOp( ) {}
};
typedef std::shared_ptr<BinOp> pBinOp;





class Devide : public BinOp
{
public:
	Devide( ) { this->val = 'b'; this->type = '/'; }
	~Devide( ) {}
};
typedef std::shared_ptr<Devide> pDevide;





class Multiply : public BinOp
{
public:
	Multiply( ) { this->val = 'b'; this->type = '*'; }
	~Multiply( ) {}
};
typedef std::shared_ptr<Multiply> pMultiply;





class Minus : public BinOp
{
public:
	Minus( ) { this->val = 'b'; this->type = '-'; }
	~Minus( ) {}
};
typedef std::shared_ptr<Minus> pMinus;





class Plus : public BinOp
{
public:
	Plus( ) { this->val = 'b'; this->type = '+'; }
	~Plus( ) {}
};
typedef std::shared_ptr<Plus> pPlus;





class And : public BinOp
{
public:
	And( ) { this->val = 'b'; this->type = '&'; }
	~And( ) {}
};
typedef std::shared_ptr<And> pAnd;





class Or : public BinOp
{
public:
	Or( ) { this->val = 'b'; this->type = '|'; }
	~Or( ) {}
};
typedef std::shared_ptr<Or> pOr;





class Equal : public BinOp
{
public:
	Equal( ) { this->val = 'b'; this->type = '='; }
	~Equal( ) {}
};
typedef std::shared_ptr<Equal> pEqual;





class Less : public BinOp
{
public:
	Less( ) { this->val = 'b'; this->type = '<'; }
	~Less( ) {}
};
typedef std::shared_ptr<Less> pLess;





class More : public BinOp
{
public:
	More( ) { this->val = 'b'; this->type = '>'; }
	~More( ) {}
};
typedef std::shared_ptr<More> pMore;





class MoreOrEq : public BinOp
{
public:
	MoreOrEq( ) { this->val = 'b'; this->type = 'm'; }
	~MoreOrEq( ) {}
};
typedef std::shared_ptr<MoreOrEq> pMoreOrEq;





class LessOrEq : public BinOp
{
public:
	LessOrEq( ) { this->val = 'b'; this->type = 'l'; }
	~LessOrEq( ) {}
};
typedef std::shared_ptr<LessOrEq> pLessOrEq;





class UnOp : public Expr
{
public:
	char type = 0;
	pExpr right;
	UnOp( ) { this->val = 'u'; }
	~UnOp( ) {}
};
typedef std::shared_ptr<UnOp> pUnOp;





class Minuss : public UnOp
{
public:
	Minuss( ) { this->val = 'u'; this->type = '-'; }
	~Minuss( ) {}
};
typedef std::shared_ptr<Minuss> pMinuss;





class Not : public UnOp
{
public:
	Not( ) { this->val = 'u'; this->type = '!'; }
	~Not( ) {}
};
typedef std::shared_ptr<Not> pNot;





class Brackets : public Expr
{
public:
	pExpr expresson;
	Brackets( ) { this->val = '('; }
	~Brackets( ) {}
};
typedef std::shared_ptr<Brackets> pBrackets;





class Body : public Statement
{
public:
	vec_pStatement statement;
	Body( ) { this->val = 'B'; }
	~Body( ) {}
};
typedef std::shared_ptr<Body> pBody;





class If : public Statement
{
public:
	Body body;
	pExpr expression;
	If( ) { this->val = '?'; }
	~If( ) {}
};
typedef std::shared_ptr<If> pIf;





// >>>Самые интересные штуки<<<

// Cпособы использования:
// return a + 3 + arg1; # ровно один раз, в конце функции
class Return : public Statement
{
public:
	pair_pExpr_vecpExpr phi_expr; // возвращаемое pExpr и "блок условий"
	Return( ) { this->val = 'r'; }
	~Return( ) {}
};
typedef std::shared_ptr<Return> pReturn;
typedef std::vector<Return> vec_Return;
typedef std::vector<pReturn> vec_pReturn;


// Cпособы применения:
// print "а";
// print "б" a + 20 + foo( b, c ) "в" foo( d );
// print("г");
// print
// {
//		"д";
//		"е" a + 20 + foo( b, c ) "ё" foo( d );
//		"ж";
// };
// Ожидаемый результат:
// а
// б42в24
// г
// д
// е42ё24
// ж
class Print : public Statement
{
public:
	vec_pExpr expressions;
	std::vector<std::string> stringsToPrint;
	std::vector<bool> order; // true~string false~expr
	Print( ) { this->val = 'P'; }
	~Print( ) {}
};
typedef std::shared_ptr<Print> pPrint;


// Спососбы применения:
// assert 1 != 2;
// assert(a > 3);
// assert{
//		1 != 2;
//		a > 3;
// };
// assert{
//		1 != 2;
//		a > 3
// };
class Assert : public Statement
{
public:
	pExpr expression;
	vec_pExpr pi_expr; // текущие для assert-а условия
	Assert( ) { this->val = 'a'; }
	~Assert( ) {}
};
typedef std::shared_ptr<Assert> pAssert;


// Способы применения:
// Func sum(arg1, arg2, arg3)
// {
//		a := arg1 + arg2 + arg3;
//		return 0; # обязательно, хотя-бы один раз
// }
class Func : public Term
{
public:
	vec_pAssign id; // список аргументов
	Body body;
	vec_pExpr return_expression;
	pExpr return_expression_monolit;
	Func( ) { this->val = 'F'; }
	~Func( ) {}
};
typedef std::shared_ptr<Func> pFunc;


// Способы применения:
// check( recursiveFoo, 9999999 );
// check( foo );
//
// Ожидаемый вывод:
// unsat
// sat
class CheckSAT : public Statement
{
public:
	std::string nameOfFunc;
	unsigned board = 7;
	CheckSAT( ) { this->val = 't'; }
	~CheckSAT( ) {}
};


// Способы применения:
// show_model( recursiveFoo, 999999 );
// show_model( foo );
//
// Ожидаемый вывод:
// unknown
// a := 10; b := 20; print( foo(a, b) );
class PrintModel : public Statement
{
public:
	std::string nameOfFunc;
	unsigned evaluations = 900;
	PrintModel( ) { this->val = 'M'; }
	~PrintModel( ) {}
};

//==================================================================//



// Сокращения для типов данных
typedef std::string::iterator str_it;
typedef std::string::reverse_iterator str_rev_it;
typedef std::string::size_type str_size_t;
typedef std::vector<pExpr> vec_pExpr;
typedef std::vector<pStatement> vec_pStatement;
typedef std::string str_t;
typedef std::vector<std::string> vec_str_t;
typedef std::vector<std::vector<std::string> > vec_vec_str_t;


std::map<str_t, pCoreAssign> allAssignments;
str_t marker = "_________жопа___";


void getLastMatch( std::map<str_t, unsigned>& lastMatch )
{
	str_size_t start;
	for ( auto& it : allAssignments ) {
		if ( ( start = it.first.find( marker ) ) == str_t::npos ) {
			if ( lastMatch.count( it.first ) == 0 ) {
				lastMatch[it.first] = 0;
			}
		}
		else {
			unsigned num = std::atoi( it.first.substr( start + marker.length( ), it.first.length( ) - ( start + marker.length( ) ) ).c_str( ) );
			if ( lastMatch.count( it.first.substr( 0, start ) ) == 0 ) { lastMatch[it.first.substr( 0, start )] = num; }
			else if ( num > lastMatch[it.first.substr( 0, start )] ) { lastMatch[it.first.substr( 0, start )] = num; }
		}
	}
}

str_t attachMarker( str_t str, unsigned i ) {
	if ( i != 0 ) { return str + marker + std::to_string( i ); }
	return str;
}

pExpr getMeanOfVar( const char* x, unsigned long rank )
{
	if ( rank != 0 )
		return ( allAssignments[x] )->meaning[rank - 1].first;
	return ( allAssignments[x] )->meaning[rank].first;
}
bool identicalConditions( vec_pExpr& a, vec_pExpr& b )
{
	bool identical = true;
	if ( a.size( ) != b.size( ) ) {
		identical = false;
	}
	else
	{
		unsigned size = a.size( );
		for ( unsigned i = 0; i < size; ++i )
		{
			if ( a[i].get( ) != b[i].get( ) )
			{
				identical = false;
				break;
			}
		}
	}

	return identical;
}
bool differenceBetweenConditions( vec_pExpr& a, vec_pExpr& b, vec_pExpr& answer )
{
	//a: if1{ if2{ if3{ if4{ } } } }
	//b: if1{ if2{ } }
	//answer: if3{ if4{ } }
	bool itsWorks = true;
	if ( a.size( ) > b.size( ) )
	{
		unsigned size = b.size( );
		for ( unsigned i = 0; i < size; ++i )
		{
			if ( a[i].get( ) != b[i].get( ) )
			{
				itsWorks = false;
				break;
			}
		}
		if ( itsWorks )
		{
			for(auto it = std::begin( a ) + size; it != std::end( a ); ++it )
			{
				answer.push_back( *it );
			}
		}
	}
	return itsWorks;
}
unsigned getNforXn( CoreAssign* these_assign, vec_pExpr& pi_expr )
{
	std::set<Expr*> piExpr;
	unsigned size = pi_expr.size( );
	
	for ( unsigned i = 0; i < size; ++i )
	{
		piExpr.insert( pi_expr[i].get( ) );
	}
	std::vector<unsigned> match;
	size = these_assign->meaning.size( );
	
	for ( unsigned i = 0; i < size; ++i )
	{
		if ( these_assign->if_expr[i] )
		{
			std::set<Expr*> Xn;
			unsigned x_size = these_assign->meaning[i].second.size( );
			for ( unsigned j = 0; j < x_size; ++j )
			{
				Xn.insert( these_assign->meaning[i].second[j].get( ) );
			}
			std::set<Expr*> intersection;
			std::set_intersection(
				std::begin( piExpr ),
				std::end( piExpr ),
				std::begin( Xn ),
				std::end( Xn ),
				std::inserter( intersection, std::begin( intersection ) ) );
			match.push_back( intersection.size( ) );
		}
		else
		{
			match.push_back( 1 );
		}
	}

	unsigned m = 0, n = 0;

	for ( unsigned  i = 0; i < size; ++i )
	{
		if ( match[i] > m )
		{
			m = match[i];
			n = i;
		}
	}

	return n;
}
void getVecOfPhiFuncs( unsigned n, CoreAssign* these_assign, vec_vec_pExpr& ret )
{
	unsigned size = these_assign->if_expr.size( );
	for ( unsigned i = n + 1; i < size; ++i )
	{
		ret.push_back( vec_pExpr( ) );
		differenceBetweenConditions(
			these_assign->meaning[i].second,
			these_assign->meaning[n].second,
			ret.back( ) );
	}
}
void getVecOfXes( unsigned n, CoreAssign* these_assign, vec_pExpr& ret )
{
	unsigned size = these_assign->if_expr.size( );
	for ( unsigned i = n; i < size; ++i )
	{
		ret.push_back( these_assign->meaning[i].first );
	}
}
pExpr getLexAND( vec_pExpr& phi )
{
	pExpr ret, buff;
	And* and_ptr = nullptr;
	unsigned size = phi.size( );
	if ( size == 0 ) { return pExpr( new N(1) ); }
	if ( size == 1 ) { ret = phi[0]; }
	else {
		--size;
		for ( unsigned i = 0; i < size; ++i )
		{
			if ( and_ptr == nullptr )
			{
				ret = pExpr( and_ptr = new And() );
				and_ptr->left = phi[i];
				and_ptr->right = phi[i + 1];
			} else
			{
				buff = pExpr( and_ptr = new And() );
				and_ptr->left = ret;
				and_ptr->right = phi[i + 1];
				ret = buff;
			}
		}
	}
	return ret;
}
pConditionFunc fillConditionFunc( vec_vec_pExpr& phi, vec_pExpr& X )
{
	pConditionFunc ret, buff;
	ConditionFunc* cond_ptr = nullptr;
	if ( X.size( ) - phi.size( ) != 1 ) throw( "error::fillConditionFunc()" );
	unsigned x_size = X.size( );
	for ( int i = phi.size( ); i > 0; )
	{
		--i;
		if ( !phi[i].empty( ) ) {
			if ( cond_ptr == nullptr )
			{
				ret = pConditionFunc( cond_ptr = new ConditionFunc( ) );
				cond_ptr->condition = getLexAND( phi[i] );
				cond_ptr->ifFalse = X[x_size - i - 2];
				cond_ptr->ifTrue = X[x_size - i - 1];
			} else
			{
				buff = pConditionFunc( cond_ptr = new ConditionFunc( ) );
				cond_ptr->condition = getLexAND( phi[i] );
				cond_ptr->ifFalse = ret;
				cond_ptr->ifTrue = X[x_size - i - 1];
				ret = buff;
			}
		}
	}
	return ret;
}
unsigned leastDeepCondition( vec_pair_pExpr_vecpExpr& vec )
{
	unsigned ret = vec[0].second.size( );
	unsigned size = vec.size( );
	for ( unsigned i = 1; i < size; ++i )
	{
		if ( vec[i].second.size( ) < ret )
		{
			ret = vec[i].second.size( );
		}
	}
	return ret;
}


// Порядок объявления в module2.h:
pExpr	lexAnalise( str_t str, vec_pAssign* all_ids, vec_pair_pExpr_vecpExpr& ret_expr, vec_pExpr& pi_expr ); // Объявление для рекурсивного вызова
void	lexGetArguments( str_t& str, str_it begin, str_it end, vec_pAssign& args );
void	lexFindBody( str_it &begin, str_it &end );
void	lexFindIf( str_t& str, str_it &begin, str_it &end, str_it &conditionBegin, str_it &conditionEnd, str_it &bodyBegin, str_it &bodyEnd );
pExpr	lexGetExpression( str_t& str, vec_pAssign* all_ids, vec_pair_pExpr_vecpExpr& ret_expr, vec_pExpr& pi_expr );
void	lexGetPartsOfSt( str_t& str, str_it begin, str_it end, vec_pStatement& args, vec_pAssign* all_ids, vec_pair_pExpr_vecpExpr& ret_expr, vec_pExpr& pi_expr ); // Объявление для рекурсивного вызова
inline void llexTypeOfStatementCh( str_t& str, vec_pStatement& stmt, str_size_t& begin1, str_size_t& begin2, str_size_t& begin3, str_size_t& begin4, str_size_t& begin4dot3, str_size_t& begin4dot4, str_size_t& begin4dot5, str_size_t& begin5, char& return_ch );
char	lexGetTypeOfStatement( str_t& str, vec_pStatement& stmt, vec_pAssign* all_ids, vec_pair_pExpr_vecpExpr& ret_expr, vec_pExpr& pi_expr );
void	lexGetPartsOfSt( str_t& str, str_it begin, str_it end, vec_pStatement& stmt, vec_pAssign* all_ids, vec_pair_pExpr_vecpExpr& ret_expr, vec_pExpr& pi_expr );
void	takeFuncDecls( str_t term, std::map<str_t, Func>& func );

// Порядок объявления в module1.h:
void lexStringWithBracketsToStrangeTree( str_t& str, vec_vec_str_t& tr );
void lexCangeBracketsOfFunctions( str_t& str, char opener, char closer );
void lexReplaseChByStr( str_t& str, char ch, str_t&& str2 );
void special_drawExpr( Expr* ex, std::map<str_t, Func>& func, str_t& line, unsigned board );
pExpr anal( str_t&& str, vec_vec_str_t& tr, vec_pAssign* all_ids, vec_pair_pExpr_vecpExpr& ret_expr, vec_pExpr& pi_expr );
pExpr deleteZeros( pExpr expr );
pExpr lexAnalise( str_t str, vec_pAssign* all_ids, vec_pair_pExpr_vecpExpr& ret_expr, vec_pExpr& pi_expr );

// в module_1.h
void check_body_sat( Body* body_st, std::map<str_t, Func>& func, CheckSAT* it, str_t& line, str_t& str );


#endif
