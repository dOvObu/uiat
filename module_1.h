#ifndef MODULE_n1_H
#define MODULE_n1_H

#include <fstream>
#include "module0.h"

std::map<str_t, long> runtimeVariables;
std::map<str_t, unsigned> assignCounter;
std::map<str_t, pFunc> func_to_eval;
std::map<str_t, unsigned> counter_call;

void check_body_sat( Body* body_st, std::map<str_t, Func>& func, CheckSAT* it, str_t& line, str_t& ret );
void check_sat( std::map<str_t, Func>& func, CheckSAT* chek_it, str_t& line, str_t& ret );
long execFunc( const char name[], std::map<str_t, Func>& func, str_t& line );
long exe( Expr* expr, std::map<str_t, Func>& func, str_t& line );



void writeExprToString( Expr* ex, str_t& str )
{
	char c = ex->getVal( );
	if ( c == 'b' )
	{
		writeExprToString( ( (BinOp*)ex )->left.get( ), str );
		str += " ";
		char t = ( (BinOp*)ex )->type;
		if ( t == '/' ) { str += "/"; }
		if ( t == '*' ) { str += "*"; }
		if ( t == '-' ) { str += "-"; }
		if ( t == '+' ) { str += "+"; }
		if ( t == '&' ) { str += "AND"; }
		if ( t == '|' ) { str += "OR"; }
		if ( t == '=' ) { str += "=="; }
		if ( t == '<' ) { str += "<"; }
		if ( t == '>' ) { str += ">"; }
		if ( t == 'l' ) { str += "<="; }
		if ( t == 'm' ) { str += ">="; }
		str += " ";
		writeExprToString( ( (BinOp*)ex )->right.get( ), str );
	} 
	else if ( c == 'u' )
	{
		str += "(";
		char t = ( (UnOp*)ex )->type;
		if ( t == '-' ) str += "-";
		if ( t == '!' ) str += "NOT";
		str += "(";
		writeExprToString( ( (UnOp*)ex )->right.get( ), str );
		str += "))";
	} 
	else if ( c == 'c' )
	{
		str += ( (Call*)ex )->nameOfFunc + "(";
		bool first = true;
		for ( auto& it : ( (Call*)ex )->args )
		{
			if ( !first )
			{
				str += ", ";
			}
			writeExprToString( it.get( ), str );
			first = false;
		}
		str += ")";
	} 
	else if ( c == 'i' )
	{
		str += ( (Id*)ex )->id;
	} 
	else if ( c == 'v' )
	{
		str += "(";
		if ( ( (Var*)ex )->withConditionFunc )
		{
			str += "(NOT(";
			writeExprToString( ( (Var*)ex )->conditionFunc->condition.get( ), str );
			str += "))*(";
			writeExprToString( ( (Var*)ex )->conditionFunc->ifFalse.get( ), str );
			str += ") + (";
			writeExprToString( ( (Var*)ex )->conditionFunc->condition.get( ), str );
			str += ")*(";
			writeExprToString( ( (Var*)ex )->conditionFunc->ifTrue.get( ), str );
			str += ")";
		}
		/*else if ( ( (Var*)ex )->argData != nullptr )
		{
			Assign* ass = ( (Var*)ex )->argData;
			if ( !ass->active )
			{
				str += ass->name;
			}
			else
			{
			}
		}*/
		else
		{
			writeExprToString( ( (Var*)ex )->data.get( ), str );
		}
		str += ")";
	}
	else if ( c == '~') {
		Assign* assign_st = (Assign*)ex;
		if ( !assign_st->active ) {
			str += assign_st->name;
		}
	}
	else if ( c == 'n' )
	{
		std::stringstream st;
		st << ( (N*)ex )->num;
		str_t str_num;
		st >> str_num;
		str += str_num;
	}
	else if ( c == '(' )
	{
		str += "(";
		writeExprToString( ( (Brackets*)ex )->expresson.get( ), str );
		str += ")";
	}
	else if ( c == '@' )
	{
		str += "(";
		str += "(!(";
		writeExprToString( ( (ConditionFunc*)ex )->condition.get( ), str );
		str += "))*(";
		writeExprToString( ( (ConditionFunc*)ex )->ifFalse.get( ), str );
		str += ") + (";
		writeExprToString( ( (ConditionFunc*)ex )->condition.get( ), str );
		str += ")*(";
		writeExprToString( ( (ConditionFunc*)ex )->ifTrue.get( ), str );
		str += "))";

		/*std::cout << " [";
		writeExprToString( ( (ConditionFunc*)ex )->condition.get( ) );
		std::cout << "](";
		writeExprToString( ( (ConditionFunc*)ex )->ifFalse.get( ) );
		std::cout << ", ";
		writeExprToString( ( (ConditionFunc*)ex )->ifTrue.get( ) );
		std::cout << ") ";*/
	}
	/*else if ( c == '=' )
	{
		// std::cout << " " << ( (Assign*)ex )->name << " := ";
		writeExprToString( ( (Assign*)ex )->meaning.back( ).first.get( ) );
		std::cout << "[";
		unsigned size = ( (Assign*)ex )->meaning.back( ).second.size( );
		for ( unsigned i = 0; i < size; ++i )
		{
			if ( i != 0 )
			{
				std::cout << " AND ";
			}
			writeExprToString( ( (Assign*)ex )->meaning.back( ).second[i].get( ) );
		}
		std::cout << "]";
	}*/
};




void check_body_sat( Body* body_st, std::map<str_t, Func>& func, CheckSAT* it, str_t& line, str_t& ret )
{
	for ( auto& jt : body_st->statement ) {
		char ch = jt->getVal( );
		switch ( ch ) {
		case 'a': {
			auto buff = recursionCounter[it->nameOfFunc];
			Assert* assert_st = (Assert*)jt.get( );
			pExpr test = getLexAND( assert_st->pi_expr );
			ret += "(assert ";
			ret += "(ite (not (= ";
			recursionCounter[it->nameOfFunc] = buff;
			special_drawExpr_forChecking( test.get( ), func, line, it->board, ret );
			ret += " 0)) (not (= ";
			special_drawExpr_forChecking( assert_st->expression.get( ), func, line, it->board, ret );
			ret += " 0)) true))\n";
			recursionCounter[it->nameOfFunc] = buff;
		} break;
		case '?': {
			If* if_st = (If*)jt.get( );
			check_body_sat( &( if_st->body ), func, it, line, ret );
		} break;
		case 'r': {
			Return* ret_st = (Return*)jt.get( );
			very_special_drawExpr( ret_st->phi_expr.first.get( ), func, line, it, ret );
			//std::cout << std::endl;
		} break;
		case 'b': {
			Body* body_st = (Body*)jt.get( );
			check_body_sat( body_st, func, it, line, ret );
		} break;
			//case 'c': {
			//Call* call_st = (Call*)jt.get( );
			//if ( counter_call.count( call_st->nameOfFunc ) == 0 )
			//{
			//	counter_call[call_st->nameOfFunc] = 0;
			//	func_to_eval[call_st->nameOfFunc] = pFunc( new Func( func[it->nameOfFunc] ) );
			//}
			//++counter_call[call_st->nameOfFunc];
			//if ( counter_call[it->nameOfFunc] < it->board )
			//{
			//	if ( call_st->nameOfFunc == it->nameOfFunc )
			//	{
			//	}
			//}
			//} break;
		default: break;
		}
	}
}

void check_sat( std::map<str_t, Func>& func, CheckSAT* it, str_t& line, str_t& ret )
{
	check_body_sat( &(func[it->nameOfFunc].body), func, it, line, ret );
}

long exe( Expr* expr, std::map<str_t, Func>& func, str_t& line )
{
	char type = expr->getVal( );
	switch ( type )
	{
	case 'n': {
		N* n_expr = (N*)expr;
		return n_expr->num;
	} break;
	case 'v': {
		Var* v_expr = (Var*)expr;
		return runtimeVariables[v_expr->nameOfVar];
	} break;
	case 'i': {
		Id* id_expr = (Id*)expr;
		return runtimeVariables[id_expr->id];
	} break;
	case 'c': {
		Call* c_expr = (Call*)expr;
		std::map<str_t, long> buff1;
		std::map<str_t, unsigned> buff2;
		for ( unsigned i = c_expr->args.size( ), size = i; i > 0; ) {
			--i;
			buff1[func[c_expr->nameOfFunc].id[i]->name] = exe( c_expr->args[i].get( ), func, line );
		}
		std::swap( runtimeVariables, buff1 );
		std::swap( assignCounter, buff2 );
		long data = execFunc( c_expr->nameOfFunc.c_str( ), func, line );
		std::swap( assignCounter, buff2 );
		std::swap( runtimeVariables, buff1 );
		return data;
	} break;
	case 'b': {
		BinOp* b_expr = (BinOp*)expr;
		char sub_type = b_expr->type;
		switch ( sub_type )
		{
		case '/': {
			return exe( b_expr->left.get( ), func, line ) / exe( b_expr->right.get( ), func, line );
		} break;
		case '*': {
			return exe( b_expr->left.get( ), func, line ) * exe( b_expr->right.get( ), func, line );
		} break;
		case '-': {
			return exe( b_expr->left.get( ), func, line ) - exe( b_expr->right.get( ), func, line );
		} break;
		case '+': {
			return exe( b_expr->left.get( ), func, line ) + exe( b_expr->right.get( ), func, line );
		} break;
		case '=': {
			long left = exe( b_expr->left.get( ), func, line ), right = exe( b_expr->right.get( ), func, line );
			return left == right;
		} break;
		case '<': {
			long left = exe( b_expr->left.get( ), func, line ), right = exe( b_expr->right.get( ), func, line );
			return left < right;
		} break;
		case '>': {
			long left = exe( b_expr->left.get( ), func, line ), right = exe( b_expr->right.get( ), func, line );
			return left > right;
		} break;
		case 'l': {
			return exe( b_expr->left.get( ), func, line ) <= exe( b_expr->right.get( ), func, line );
		} break;
		case 'm': {
			return exe( b_expr->left.get( ), func, line ) >= exe( b_expr->right.get( ), func, line );
		} break;
		default:
			break;
		}
	} break;
	case 'u': {
		UnOp* u_expr = (UnOp*)expr;
		char sub_type = u_expr->type;
		switch ( sub_type )
		{
		case '!': {
			Not* n_expr = (Not*)u_expr;
			return !exe( n_expr->right.get( ), func, line );
		} break;
		case '-': {
			Minuss* n_expr = (Minuss*)u_expr;
			return -exe( n_expr->right.get( ), func, line );
		} break;
		default:
			break;
		}
	} break;
	case '(': {
		Brackets* n_expr = (Brackets*)expr;
		return exe(n_expr->expresson.get( ), func, line);
	} break;
	default:
		break;
	}
	return 0;
}

long exe( Statement* st, std::map<str_t, Func>& func, bool& IsReturn, str_t& line )
{
	char type = st->getVal( );
	switch ( type )
	{
	case 'P': {
		Print* p_st = (Print*)st;
		unsigned size = p_st->order.size( );
		for ( unsigned i = 0, j = 0, k = 0; i < size; ++i ) {
			if ( p_st->order[i] ) {
				std::cout << p_st->stringsToPrint[j];
				++j;
			} else {
				std::cout << exe( p_st->expressions[k].get( ), func, line );
				//std::cout << " {sym:" << symExec( p_st->expressions[k].get( ), func, line ) << "}";
				//recursionCounter.clear( );
				//std::cout << " { "; special_drawExpr( p_st->expressions[k].get( ), func, line ); std::cout << " }";
				++k;
			}
		}
		std::cout << '\n';
	} break; // print
	case '~': {
		Assign* a_expr = (Assign*)st;
		//if ( assignCounter.count( a_expr->name ) == 0 ) { assignCounter[a_expr->name] = 0; }
		long data = exe( a_expr->meaning[a_expr->pos].first.get( ), func, line );
		runtimeVariables[a_expr->name] = data;
		//++assignCounter[a_expr->name];

	} break; // assign
	case '?': {
		If* if_st = (If*)st;
		long data = 0;
		long data2 = exe( if_st->expression.get( ), func, line );
		if ( data2 )
		{
			data = exe( &(if_st->body), func, IsReturn, line );
		}
		return data;
	} break; // if
	case 'B': {
		Body* body_st = (Body*)st;
		long data = 0;
		unsigned size = body_st->statement.size( );
		for ( unsigned i = 0; i < size; ++i ) {
			data = exe( body_st->statement[i].get( ), func, IsReturn, line );
			if ( IsReturn ) { return data; }
		}
	} break; // body
	case 'r': {
		Return* r_st = (Return*)st;
		IsReturn = true;
		long data = exe( r_st->phi_expr.first.get( ), func, line );// << std::endl;
		return data;
	} break; // return
	case 'a': {
		Assert* ass_st = (Assert*)st;
		//special_drawExpr( ass_st->expression.get( ) );
		auto data = exe( ass_st->expression.get( ), func, line );
		if ( !data ) { std::cout << "\nBy!" << std::endl; abort( ); }
		//std::cout << std::endl;
	} break; // assert
	case 'c': {
		Call* c_expr = (Call*)st;
		std::map<str_t, long> buff1;
		std::map<str_t, unsigned> buff2;
		for ( unsigned i = c_expr->args.size( ), size = i; i > 0; ) {
			--i;
			buff1[func[c_expr->nameOfFunc].id[i]->name] = exe( c_expr->args[i].get( ), func, line );
		}

		std::swap( runtimeVariables, buff1 );
		std::swap( assignCounter, buff2 );
		long data = execFunc( c_expr->nameOfFunc.c_str( ), func, line );
		std::swap( assignCounter, buff2 );
		std::swap( runtimeVariables, buff1 );
		return data;
	} break; // function call
	case 't': {
		CheckSAT* check_st = (CheckSAT*)st;
		std::cout << "check " << check_st->nameOfFunc << std::endl;
		str_t str = "";
		nameOfFuncForCheck = check_st->nameOfFunc;
		for ( auto& it : func[nameOfFuncForCheck].id ) {
			str += "(declare-const " + it->name + " Int)\n";
		}
		//std::cout << " { "; special_drawExpr( func[check_st->nameOfFunc].return_expression_monolit.get( ), func, line, check_st->board ); std::cout << " }";
		check_sat( func, check_st, line, str );
		nameOfFuncForCheck = "";
		recursionCounter.clear( );
		str += "(check-sat)";
		//std::cout << str << std::endl;
		std::ofstream fout( "______check_sat_file" , std::ios::trunc );
		fout << str;
		fout.close( );
		system( "z3 ______check_sat_file > ______ret" );
		std::ifstream fin( "______ret" );
		fin >> str;
		fin.close( );
		//system( "pause" );
		system( ( remove_file_command + " ______check_sat_file" ).c_str( ) );
		system( ( remove_file_command + " ______ret" ).c_str( ) );
		std::cout << str << std::endl;

	} break; // check-sat
	case 'M': {
		PrintModel* show_model = (PrintModel*)st;
		CheckSAT chsat;
		chsat.board = show_model->evaluations;
		chsat.nameOfFunc = show_model->nameOfFunc;
		std::cout << "# model for " << chsat.nameOfFunc << ":" << std::endl;
		str_t str = "";
		nameOfFuncForCheck = chsat.nameOfFunc;
		for ( auto& it : func[nameOfFuncForCheck].id ) {
			str += "(declare-const " + it->name + " Int)\n";
		}
		check_sat( func, &chsat, line, str );
		nameOfFuncForCheck = "";
		recursionCounter.clear( );
		str += "(check-sat)\n";
		str += "(get-model)";
		std::ofstream fout( "______check_sat_and_get_model_file", std::ios::trunc );
		fout << str;
		fout.close( );
		str.clear( );
		system( "z3 ______check_sat_and_get_model_file > ______ret" );
		std::ifstream fin( "______ret" );
		char buff[280];
		bool first_line = true;
		str_t printOut;
		while ( !fin.eof( ) )
		{
			fin.getline( buff, 280 );
			str += buff;
			if ( first_line ) { str += "\n"; printOut = str; first_line = false; }
		}
		//std::cout << str << std::endl;
		auto start = str.find( "(define-fun " ) + 12;
		while ( start - 12 != str_t::npos )
		{
			auto firstEnd = str.substr( start, str.length( ) - start ).find( " () Int" );
			auto secondEnd = str.substr( start + firstEnd + 7, str.length( ) - ( start + firstEnd + 7 ) ).find( ")" )+2;
			auto thirdEnd = str.substr( start + firstEnd + secondEnd, str.length( ) - ( start + firstEnd + secondEnd ) ).find( ")" );
			printOut += str.substr( start, firstEnd ) + " := " + std::to_string(std::atoi(str.substr( start + firstEnd + secondEnd, thirdEnd ).c_str())) + ";\n";
			//std::cout << printOut << std::endl;
			//system( "pause" );
			start += secondEnd;
			auto shift = str.substr( start, str.length( ) - start ).find( "(define-fun " );
			if ( shift != str_t::npos && start ) {
				start += shift + 12;
				continue;
			}
			break;
		}

		fin.close( );
		//system( "pause" );
		system( ( remove_file_command + " ______check_sat_and_get_model_file" ).c_str( ) );
		system( ( remove_file_command + " ______ret" ).c_str( ) );



		//std::cout << str << std::endl;
		std::cout << printOut << std::endl;
	} break; // get-model
	default:
		break;
	}
	return 0;
}

long execFunc( const char name[], std::map<str_t, Func>& func, str_t& line )
{
	Func* foo = &(func[name]);
	unsigned size = foo->body.statement.size( );
	bool IsReturn = false;
	long data = 0;
	for ( unsigned i = 0; i < size; ++i )
	{
		data = exe( foo->body.statement[i].get( ), func, IsReturn, line );
		//std::cout << "exF " << data << " IsRet " << IsReturn << std::endl;
		if ( IsReturn ) {
			return data;
		}
	}
	return data;
}


#endif
