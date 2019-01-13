#ifndef MODULE_1_H
#define MODULE_1_H
#define FUNC_ARGS_FROM_LEFT_TO_RIGHT
#include "module2.h"
#include <functional>
#include "module3.h"


std::map<str_t, unsigned> recursionCounter;

// Майнит  выражения из строки и странного дерева
pExpr anal(
	str_t&& str,								// анализируемая строка
	vec_vec_str_t& tr,						// странное дерево
	vec_pAssign* all_ids,					// имена всех аргументов
	vec_pair_pExpr_vecpExpr& ret_expr,	// варианты возвращаемых значений
	vec_pExpr& pi_expr	/*					// текущие условия */ )
{
	// Бинарные сравнения
	{
		str_size_t lessOrEqPos = str.find( "<=" );
		str_size_t moreOrEqPos = str.find( ">=" );
		str_size_t lessPos = str.find( '<' );
		str_size_t morePos = str.find( '>' );
		str_size_t equalPos = str.find( '=' );
		str_size_t mx = lessOrEqPos;
		MIN( mx, moreOrEqPos, lessPos, morePos, equalPos );

		if ( mx != str_t::npos )
		{
			if ( mx == lessOrEqPos )
			{
				LessOrEq* expr;
				pExpr sh( expr = new LessOrEq( ) );
				str_size_t from = mx + 2;
				expr->right = anal( str.substr( from, str.length( ) - from ), tr, all_ids, ret_expr, pi_expr );
				expr->left = anal( str.substr( 0, from - 2 ), tr, all_ids, ret_expr, pi_expr );
				return sh;
			}
			else if ( mx == moreOrEqPos )
			{
				MoreOrEq* expr;
				pExpr sh( expr = new MoreOrEq( ) );
				auto from = mx + 2;
				expr->right = anal( str.substr( from, str.length( ) - from ), tr, all_ids, ret_expr, pi_expr );
				expr->left = anal( str.substr( 0, from - 2 ), tr, all_ids, ret_expr, pi_expr );
				return sh;
			}
			else if ( mx == lessPos )
			{
				Less* expr;
				pExpr sh( expr = new Less( ) );
				auto from = mx + 1;
				expr->right = anal( str.substr( from, str.length( ) - from ), tr, all_ids, ret_expr, pi_expr );
				expr->left = anal( str.substr( 0, from - 1 ), tr, all_ids, ret_expr, pi_expr );
				return sh;
			}
			else if ( mx == morePos )
			{
				More* expr;
				pExpr sh( expr = new More( ) );
				auto from = mx + 1;
				expr->right = anal( str.substr( from, str.length( ) - from ), tr, all_ids, ret_expr, pi_expr );
				expr->left = anal( str.substr( 0, from - 1 ), tr, all_ids, ret_expr, pi_expr );
				return sh;
			}
			else if ( mx == equalPos )
			{
				Equal* expr;
				pExpr sh( expr = new Equal( ) );
				auto from = mx + 1;
				expr->right = anal( str.substr( from, str.length( ) - from ), tr, all_ids, ret_expr, pi_expr );
				str_t test = str.substr( 0, from - 1 );
				expr->left = anal( test.c_str( ), tr, all_ids, ret_expr, pi_expr );
				return sh;
			}
		}
	}

	str_t k = "&|";
	str_rev_it key = std::find_first_of( std::rbegin( str ), std::rend( str ), std::begin( k ), std::end( k ) );

	// Бинарыне арифмитические и предикатные
	{
		if ( key != std::rend( str ) )
		{
			if ( *key == '&' )
			{
				And* expr;
				pExpr sh( expr = new And( ) );
				str_size_t from = key + 1 - std::rbegin( str );
				expr->right = anal( str.substr( str.length( ) - from + 1, str.length( ) ), tr, all_ids, ret_expr, pi_expr );
				expr->left = anal( str.substr( 0, str.length( ) - from ), tr, all_ids, ret_expr, pi_expr );
				return sh;
			}
			if ( *key == '|' )
			{
				Or* expr;
				pExpr sh( expr = new Or( ) );
				str_size_t from = key + 1 - std::rbegin( str );
				expr->right = anal( str.substr( str.length( ) - from + 1, str.length( ) ), tr, all_ids, ret_expr, pi_expr );
				expr->left = anal( str.substr( 0, str.length( ) - from ), tr, all_ids, ret_expr, pi_expr );
				return sh;
			}
		}

		k = "+-";
		key = std::find_first_of( std::rbegin( str ), std::rend( str ), std::begin( k ), std::end( k ) );

		if ( key != std::rend( str ) )
		{
			if ( *key == '+' )
			{
				Plus* expr;
				pExpr sh( expr = new Plus( ) );
				str_size_t from = key + 1 - std::rbegin( str );
				expr->right = anal( str.substr( str.length( ) - from + 1, str.length( ) ), tr, all_ids, ret_expr, pi_expr );
				expr->left = anal( str.substr( 0, str.length( ) - from ), tr, all_ids, ret_expr, pi_expr );
				return sh;
			}
			if ( *key == '-' && key != std::rend( str ) - 1 )
			{
				Minus* expr;
				pExpr sh( expr = new Minus( ) );
				str_size_t from = key + 1 - std::rbegin( str );
				expr->right = anal( str.substr( str.length( ) - from + 1, str.length( ) ), tr, all_ids, ret_expr, pi_expr );
				expr->left = anal( str.substr( 0, str.length( ) - from ), tr, all_ids, ret_expr, pi_expr );
				return sh;
			}
		}

		k = "/*";
		key = std::find_first_of( std::rbegin( str ), std::rend( str ), std::begin( k ), std::end( k ) );

		if ( key != std::rend( str ) )
		{
			if ( *key == '/' )
			{
				Devide* expr;
				pExpr sh( expr = new Devide( ) );
				str_size_t from = key + 1 - std::rbegin( str );
				expr->right = anal( str.substr( str.length( ) - from + 1, str.length( ) ), tr, all_ids, ret_expr, pi_expr );
				expr->left = anal( str.substr( 0, str.length( ) - from ), tr, all_ids, ret_expr, pi_expr );
				return sh;
			}
			if ( *key == '*' )
			{
				Multiply* expr;
				pExpr sh( expr = new Multiply( ) );
				str_size_t from = key + 1 - std::rbegin( str );
				expr->right = anal( str.substr( str.length( ) - from + 1, str.length( ) ), tr, all_ids, ret_expr, pi_expr );
				expr->left = anal( str.substr( 0, str.length( ) - from ), tr, all_ids, ret_expr, pi_expr );
				return sh;
			}
		}
	}

	// Унарные операции
	{
		if ( str[0] == '-' )
		{
			Minuss* expr;
			pExpr sh( expr = new Minuss( ) );
			expr->right = anal( str.substr( 1, str.length( ) - 1 ), tr, all_ids, ret_expr, pi_expr );
			return sh;
		}

		if ( str[0] == '!' )
		{
			Not* expr;
			pExpr sh( expr = new Not( ) );
			expr->right = anal( str.substr( 1, str.length( ) - 1 ), tr, all_ids, ret_expr, pi_expr );
			return sh;
		}
	}

	// Перевод странного дерева в вызов функции
	// и вызов полных значений переменных
	{
		// сохранение числа в дерево
		if ( lexIsDigit( *std::begin( str ) ) )
		{
			N* expr;
			pExpr sh( expr = new N( ) );
			lexParse( str, expr->num );
			//if (pi_expr.size()) std::cout << "in if ";
			//std::cout << expr->num << std::endl;
			return sh;
		}
		// сохранение вызова функции в дерево
		else if ( std::find( std::begin( str ), std::end( str ), '[' ) != std::end( str ) )
		{
			Call* expr;
			pExpr sh( expr = new Call( ) );
			lexParse( str.substr( 0, str.find( '[' ) ), expr->nameOfFunc );
			//std::cout << "Call: " << expr->nameOfFunc << "(...)" << std::endl;

			// добавление условий срабатывания вызова функции
			for ( auto& it : pi_expr ) {
				expr->pi_expr.push_back( it );
			}
			for ( auto& it : ret_expr ) {
				for ( auto& jt : it.second ) {
					Not* not_expr;
					expr->pi_expr.push_back( pExpr( not_expr = new Not( ) ) );
					not_expr->right = jt;
				}
			}
#ifdef FUNC_ARGS_FROM_RIGHT_TO_LEFT

			str_rev_it it = std::find( str.rbegin( ), str.rend( ), '#' );
			str_rev_it jt = std::find( it + 1, str.rend( ), '#' );

			while ( it != str.rend( ) )
			{
				str_t rev( it, jt + 1 );
				expr->args.push_back( anal( std::string( rev.rbegin( ), rev.rend( ) ), tr, all_ids, ret_expr, pi_expr ) );
				Brackets* wall = (Brackets*)( expr->args.back( ).get( ) );
				
				if ( wall->expresson->val == '0' )
				{
					expr->args.pop_back( );
				}
				it = std::find( jt + 2, str.rend( ), '#' );
				
				if ( it != str.rend( ) )
				{
					jt = std::find( it + 1, str.rend( ), '#' );
				}
			}
#else
#ifdef FUNC_ARGS_FROM_LEFT_TO_RIGHT

			auto it = std::find( str.begin( ), str.end( ), '#' );
			auto jt = std::find( it + 1, str.end( ), '#' );
			while ( it != str.end( ) ) {

				// добавление значений аргументов для вызова функции
				expr->args.push_back( anal( std::string( it, jt + 1 ), tr, all_ids, ret_expr, pi_expr ) );
				//

				Brackets* wall = (Brackets*)( expr->args.back( ).get( ) );
				if ( wall->expresson->val == '0' ) {
					expr->args.pop_back( );
				}
				it = std::find( jt + 2, str.end( ), '#' );
				if ( it != str.end( ) ) {
					jt = std::find( it + 1, str.end( ), '#' );
				}
			}
#endif	// LEFT_TO_RIGHT
#endif // RIGHT_TO_LEFT

			for ( unsigned i = 0; i < expr->args.size( ); ++i )
			{
				expr->args[i] = ( (Brackets*)expr->args[i].get( ) )->expresson;
			}

			return sh;
		}
		// сохранение скобок в дерево
		else if ( std::find( std::begin( str ), std::end( str ), '#' ) != std::end( str ) )
		{
			Brackets* expr;
			pExpr sh( expr = new Brackets( ) );
			std::stringstream sstr;
			sstr << std::string( str.begin( ) + 1, str.end( ) - 1 );
			unsigned mm;
			sstr >> mm;
			unsigned nn;
			sstr >> nn;
			if ( tr[mm][nn].size( ) )
			{
				expr->expresson = anal( str_t( tr[mm][nn] ), tr, all_ids, ret_expr, pi_expr );
			}
			else
			{
				expr->expresson = pExpr( new Empty( ) );
			}

			return sh;
		}
		// сохранение переменных в дерево
		else
		{
			str_t name( std::begin( str ), std::end( str ) );

			std::map<str_t, unsigned> lastMatch;
			getLastMatch( lastMatch );
			name = attachMarker( name, lastMatch[name] );

			if ( allAssignments.count( name ) != 0 )
			{
				Assign these_assign = allAssignments[name]; // alias
				Var* p_var;
				pExpr sh( p_var = new Var( ) );
				p_var->nameOfVar = name;

				// p_var->phi.first;				~ PhiFunc	pExpr
				// p_var->phi.second.first;	~ X[n-1]		pExpr если PhiFunc=False
				// p_var->phi.second.second;	~ X[n]		pExpr если PhiFunc=True
				// p_var->rank;					~ n			unsigned

				if ( name == "a2" ) {
					system("echo kek");
				}

				if ( these_assign.if_expr.empty( ) )// тут;a:=1;if{a:=a+1;}
				{
					// если такая переменная ни разу не инициализировалась
					if ( all_ids != nullptr ) // если есть аргументы
					{
						bool allright = false;
						pAssign argAssign = nullptr;
						for ( pAssign& it : *all_ids ) // и одним из этих аргументов является рассматриваемая переменная
						{
							if ( name == it->name )
							{
								argAssign = it;
								allright = true; // то всё хорошо
								break;
							}
						}
						check( allright, ( "ERROR::Asking_for_(" + name + ")_variable_without_initializing" ).c_str( ) );
						Var* p_argument;
						pExpr sh( p_argument = new Var( ) ); // мы сохраняем эту переменную в дерево
						p_argument->nameOfVar = name;
						p_argument->withConditionFunc = false; // устанавливая, что пока что никаких условных функций для неё - нет,
						p_argument->argData = argAssign; // а её значение целиком и полностью зависит от значения аргумента
						// p_argument->argData->active; Если аргумент будет не активен
						// p_argument->argData->meaning; то его значение будет пустым
						// Иначе (при вызове, рекурсивном или нет), мы сюда не сможем попасть,
						//		т.к. к таким аргументам уже будет добавляться информация о условиях (есть они, или их нет)
						return sh;
					}//*/

					throw( "error::analise::these_assign->if_expr.size()==0" );
				}


				//check( pi_expr.size( ) >= leastDeepCondition( these_assign.meaning ),
				//	( "ERROR::Asking_for_(" + name + ")_variable_when_it_out_of_sight" ).c_str( ) );

				
				if ( these_assign.if_expr.back( ) && these_assign.if_expr.size() != 1 )
				{
					if ( identicalConditions( these_assign.meaning.back( ).second, pi_expr ) )
					{
						p_var->withConditionFunc = false;
						p_var->data = these_assign.meaning.back( ).first;
					}
					else
					{
						p_var->withConditionFunc = true;
					}
				}
				else
				{
					p_var->withConditionFunc = false;
					p_var->data = these_assign.meaning.back( ).first;
					//p_var->withConditionFunc = true;
				}

				if ( p_var->withConditionFunc )
				{
					// Находим n для X[n], т.е. these_assign->meaning[n]
					unsigned n = getNforXn( these_assign.core.get(), pi_expr );
					// Записываем функции ф1, ф2, ф3, ф4 ...
					std::vector<vec_pExpr> phi;
					getVecOfPhiFuncs( n, these_assign.core.get(), phi );
					// Записываем значения X[n], X[n+1], X[n+2], X[n+3], X[n+4] ...
					vec_pExpr X;
					getVecOfXes( n, these_assign.core.get(), X );
					// Собираем функцию выбора значения фn-1(...ф4(ф3(ф2(ф1(X[n],X[n+1]),X[n+2]),X[n+3]),X[n+4])...,X[n+(n-1)]);
					p_var->conditionFunc = fillConditionFunc( phi, X );
				}

				//special_drawExpr( p_var );
				//std::cout << std::endl;

				return sh;
			}
			else if ( all_id_of_curr_Func != nullptr ) // если такая переменная ни разу не инициализировалась
			{
				// если такая переменная ни разу не инициализировалась
				if ( all_ids != nullptr ) // если есть аргументы
				{
					bool allright = false;
					pAssign argAssign = nullptr;
					for ( pAssign& it : *all_ids ) // и одним из этих аргументов является рассматриваемая переменная
					{
						//std::cout << it->name << std::endl;
						if ( name == it->name )
						{
							argAssign = it;
							allright = true; // то всё хорошо
							break;
						}
					}
					check( allright, ( "ERROR::Asking_for_(" + name + ")_variable_without_initializing" ).c_str( ) );
					Var* p_argument;
					pExpr sh( p_argument = new Var( ) ); // мы сохраняем эту переменную в дерево
					p_argument->nameOfVar = name;
					p_argument->withConditionFunc = false; // устанавливая, что пока что никаких условных функций для неё - нет,
					p_argument->argData = argAssign; // а её значение целиком и полностью зависит от значения аргумента
					// p_argument->argData->active; Если аргумент будет не активен
					// p_argument->argData->meaning; то его значение будет пустым
					// Иначе (при вызове, рекурсивном или нет), мы сюда не сможем попасть,
					//		т.к. к таким аргументам уже будет добавляться информация о условиях (есть они, или их нет)
					return sh;
				}//*/
			}
			else
			{
				check( false, ( "ERROR::Asking_for_(" + name + ")_variable_without_initializing" ).c_str( ) );
				return pEmpty( );
			}

			return pEmpty( );
		}
	}
}
str_t nameOfFuncForCheck;

void very_special_drawExpr( Expr* ex, std::map<str_t, Func>& func, str_t& line, CheckSAT* check_it, str_t& ret )
{
	char c = ex->getVal( );
	if ( c == 'b' )
	{
		very_special_drawExpr( ( (BinOp*)ex )->left.get( ), func, line, check_it, ret );
		very_special_drawExpr( ( (BinOp*)ex )->right.get( ), func, line, check_it, ret );
	} else if ( c == 'u' )
	{
		std::cout << "(";
		char t = ( (UnOp*)ex )->type;
		very_special_drawExpr( ( (UnOp*)ex )->right.get( ), func, line, check_it, ret );
	} else if ( c == 'c' )
	{
		if ( func.count( ( (Call*)ex )->nameOfFunc ) != 0 )
		{
			if ( recursionCounter.count( ( (Call*)ex )->nameOfFunc ) == 0 ) { recursionCounter[( (Call*)ex )->nameOfFunc] = 0; } else { ++recursionCounter[( (Call*)ex )->nameOfFunc]; }

			if ( recursionCounter[( (Call*)ex )->nameOfFunc] < check_it->board )
			{
				Func foo;
				takeFuncDecl( line, ( (Call*)ex )->nameOfFunc, foo );

				vec_pExpr buff;
				for ( auto& it : foo.id ) {
					buff.push_back( ( (Brackets*)it->meaning[0].first.get( ) )->expresson );
				}
				unsigned i = 0;
				for ( auto& it : ( (Call*)ex )->args )
				{
					( (Brackets*)foo.id[i]->meaning[0].first.get( ) )->expresson = it;
					foo.id[i]->active = true;
					++i;
				}
				check_body_sat( &( foo.body ), func, check_it, line, ret );
				very_special_drawExpr( foo.return_expression_monolit.get( ), func, line, check_it,ret );
				i = 0;
				for ( auto& it : buff )
				{
					( (Brackets*)foo.id[i]->meaning[0].first.get( ) )->expresson = it;
					foo.id[i]->active = false;
					++i;
				}

			}
		}
	} else if ( c == 'v' )
	{
		bool isArg = false;
		for ( auto& it : func[nameOfFuncForCheck].id ) { if ( it->name == ( (Var*)ex )->nameOfVar ) { isArg = true; break; } }

		if ( ( (Var*)ex )->withConditionFunc )
		{
			very_special_drawExpr( ( (Var*)ex )->conditionFunc->condition.get( ), func, line, check_it, ret );
			very_special_drawExpr( ( (Var*)ex )->conditionFunc->ifFalse.get( ), func, line, check_it, ret );
			very_special_drawExpr( ( (Var*)ex )->conditionFunc->ifTrue.get( ), func, line, check_it, ret );

		} else
		{
			very_special_drawExpr( ( (Var*)ex )->data.get( ), func, line, check_it, ret );
		}
	} else if ( c == '(' )
	{
		very_special_drawExpr( ( (Brackets*)ex )->expresson.get( ), func, line, check_it, ret );
	} else if ( c == '@' )
	{
		very_special_drawExpr( ( (ConditionFunc*)ex )->condition.get( ), func, line, check_it, ret );
		very_special_drawExpr( ( (ConditionFunc*)ex )->ifFalse.get( ), func, line, check_it, ret );
		very_special_drawExpr( ( (ConditionFunc*)ex )->ifTrue.get( ), func, line, check_it, ret );
	}
};

void special_drawExpr_forChecking( Expr* ex, std::map<str_t, Func>& func, str_t& line, unsigned board, str_t& ret )
{
	char c = ex->getVal( );
	if ( c == 'b' )
	{
		char t = ( (BinOp*)ex )->type;
		if ( t == '/' || t == '*' || t == '-' || t == '+' )
		{
			ret += "(";
			if ( t == '/' ) { ret += "/"; }
			if ( t == '*' ) { ret += "*"; }
			if ( t == '-' ) { ret += "-"; }
			if ( t == '+' ) { ret += "+"; }
			ret += " ";
			special_drawExpr_forChecking( ( (BinOp*)ex )->left.get( ), func, line, board, ret );
			ret += " ";
			special_drawExpr_forChecking( ( (BinOp*)ex )->right.get( ), func, line, board, ret );
			ret += ")";
		}
		else if ( t == '=' || t == '<' || t == '>' || t == 'l' || t == 'm' )
		{
			ret += "(ite (";
			if ( t == '=' ) { ret += "="; }
			if ( t == '<' ) { ret += "<"; }
			if ( t == '>' ) { ret += ">"; }
			if ( t == 'l' ) { ret += "<="; }
			if ( t == 'm' ) { ret += ">="; }
			ret += " ";
			special_drawExpr_forChecking( ( (BinOp*)ex )->left.get( ), func, line, board, ret );
			ret += " ";
			special_drawExpr_forChecking( ( (BinOp*)ex )->right.get( ), func, line, board, ret );
			ret += ") 1 0)";
		}
		else {
			ret += "(ite (";
			if ( t == '&' ) { ret += "and"; }
			if ( t == '|' ) { ret += "or"; }
			ret += " (not (= ";
			special_drawExpr_forChecking( ( (BinOp*)ex )->left.get( ), func, line, board, ret );
			ret += " 0)) (not (= ";
			special_drawExpr_forChecking( ( (BinOp*)ex )->right.get( ), func, line, board, ret );
			ret += " 0))) 1 0)";
		}

	} else if ( c == 'u' )
	{
		char t = ( (UnOp*)ex )->type;
		if ( t == '-' ) ret += "(- ";
		if ( t == '!' ) ret += "(ite (= ";
		special_drawExpr_forChecking( ( (UnOp*)ex )->right.get( ), func, line, board, ret );
		if ( t == '!' ) ret += " 0) 1 0)";
		if ( t == '-' ) ret += ")";
	} else if ( c == 'c' )
	{
		if ( func.count( ( (Call*)ex )->nameOfFunc ) != 0 )
		{
			if ( recursionCounter.count( ( (Call*)ex )->nameOfFunc ) == 0 ) { recursionCounter[( (Call*)ex )->nameOfFunc] = 0; } else { ++recursionCounter[( (Call*)ex )->nameOfFunc]; }

			if ( recursionCounter[( (Call*)ex )->nameOfFunc] < board )
			{
				Func foo;
				takeFuncDecl( line, ( (Call*)ex )->nameOfFunc, foo );

				vec_pExpr buff;
				for ( auto& it : foo.id ) {
					buff.push_back( ( (Brackets*)it->meaning[0].first.get( ) )->expresson );
				}
				unsigned i = 0;
				for ( auto& it : ( (Call*)ex )->args )
				{
					( (Brackets*)foo.id[i]->meaning[0].first.get( ) )->expresson = it;
					foo.id[i]->active = true;
					++i;
				}
				special_drawExpr_forChecking( foo.return_expression_monolit.get( ), func, line, board, ret );
				i = 0;
				for ( auto& it : buff )
				{
					( (Brackets*)foo.id[i]->meaning[0].first.get( ) )->expresson = it;
					foo.id[i]->active = false;
					++i;
				}
			} else { ret += "1"; }
		}
	}
	else if ( c == 'v' )
	{
		if ( ( (Var*)ex )->withConditionFunc )
		{
			ret += "(ite (not (= ";
			special_drawExpr_forChecking( ( (Var*)ex )->conditionFunc->condition.get( ), func, line, board, ret );
			ret += " 0)) ";
			special_drawExpr_forChecking( ( (Var*)ex )->conditionFunc->ifFalse.get( ), func, line, board, ret );
			ret += " ";
			special_drawExpr_forChecking( ( (Var*)ex )->conditionFunc->ifTrue.get( ), func, line, board, ret );
			ret += ")";
		} else if ( ( (Var*)ex )->data->getVal( ) == '(' )
		{
			if ( ( (Brackets*)( ( (Var*)ex )->data.get( ) ) )->expresson->val == '~' ) {
				ret += ( (Var*)ex )->nameOfVar;
			} else
			{
				special_drawExpr_forChecking( ( (Var*)ex )->data.get( ), func, line, board, ret );
			}
		} else
		{
			special_drawExpr_forChecking( ( (Var*)ex )->data.get( ), func, line, board, ret );
		}
	} else if ( c == 'n' )
	{
		ret += std::to_string( ( (N*)ex )->num );
	} else if ( c == '(' )
	{
		special_drawExpr_forChecking( ( (Brackets*)ex )->expresson.get( ), func, line, board, ret );
	} else if ( c == '@' )
	{
		ret += "(ite (not (= ";
		special_drawExpr_forChecking( ( (ConditionFunc*)ex )->condition.get( ), func, line, board, ret );
		ret += " 0)) ";
		special_drawExpr_forChecking( ( (ConditionFunc*)ex )->ifFalse.get( ), func, line, board, ret );
		ret += " ";
		special_drawExpr_forChecking( ( (ConditionFunc*)ex )->ifTrue.get( ), func, line, board, ret );
		ret += ")";
	}
};

void special_drawExpr( Expr* ex, std::map<str_t, Func>& func, str_t& line, unsigned board )
{
	char c = ex->getVal( );
	if ( c == 'b' )
	{
		std::cout << "(";
		char t = ( (BinOp*)ex )->type;
		if ( t == '/' ) { std::cout << "/"; }
		if ( t == '*' ) { std::cout << "*"; }
		if ( t == '-' ) { std::cout << "-"; }
		if ( t == '+' ) { std::cout << "+"; }
		if ( t == '&' ) { std::cout << "and"; }
		if ( t == '|' ) { std::cout << "or"; }
		if ( t == '=' ) { std::cout << "="; }
		if ( t == '<' ) { std::cout << "<"; }
		if ( t == '>' ) { std::cout << ">"; }
		if ( t == 'l' ) { std::cout << "<="; }
		if ( t == 'm' ) { std::cout << ">="; }
		std::cout << " ";
		special_drawExpr( ( (BinOp*)ex )->left.get( ), func, line, board );
		std::cout << " ";
		special_drawExpr( ( (BinOp*)ex )->right.get( ), func, line, board );
		std::cout << ")";
	}
	else if ( c == 'u' )
	{
		std::cout << "(";
		char t = ( (UnOp*)ex )->type;
		if ( t == '-' ) std::cout << "-";
		if ( t == '!' ) std::cout << "not";
		special_drawExpr( ( (UnOp*)ex )->right.get( ), func, line, board );
		std::cout << ")";
	}
	else if ( c == 'c' )
	{
		if ( func.count( ( (Call*)ex )->nameOfFunc ) != 0 )
		{
			if ( recursionCounter.count( ( (Call*)ex )->nameOfFunc ) == 0 ) { recursionCounter[( (Call*)ex )->nameOfFunc] = 0; }
			else { ++recursionCounter[( (Call*)ex )->nameOfFunc]; }

			if ( recursionCounter[( (Call*)ex )->nameOfFunc] < board )
			{
				Func foo;
				takeFuncDecl( line, ( (Call*)ex )->nameOfFunc, foo );

				vec_pExpr buff;
				for ( auto& it : foo.id ) {
					buff.push_back( ( (Brackets*)it->meaning[0].first.get( ) )->expresson );
				}
				unsigned i = 0;
				for ( auto& it : ( (Call*)ex )->args )
				{
					( (Brackets*)foo.id[i]->meaning[0].first.get( ) )->expresson = it;
					foo.id[i]->active = true;
					++i;
				}
				special_drawExpr( foo.return_expression_monolit.get( ), func, line, board );
				i = 0;
				for ( auto& it : buff )
				{
					( (Brackets*)foo.id[i]->meaning[0].first.get( ) )->expresson = it;
					foo.id[i]->active = false;
					++i;
				}
				//std::cout << ")";
			} else { std::cout << "1"; }
		}
		
		/*
		std::cout << ( (Call*)ex )->nameOfFunc << "(";
		bool first = true;
		for ( auto& it : ( (Call*)ex )->args )
		{
			if ( !first )
			{
				std::cout << ", ";
			}
			special_drawExpr( it.get( ), func, line );
			first = false;
		}
		std::cout << ")";//*/
	}
	//else if ( c == 'i' )
	//{
	//	std::cout << ( (Id*)ex )->id;
	//}
	else if ( c == 'v' )
	{
		if ( ( (Var*)ex )->withConditionFunc )
		{
			std::cout << "(ite ";
			special_drawExpr( ( (Var*)ex )->conditionFunc->condition.get( ), func, line, board );
			std::cout << " ";
			special_drawExpr( ( (Var*)ex )->conditionFunc->ifFalse.get( ), func, line, board );
			std::cout << " ";
			special_drawExpr( ( (Var*)ex )->conditionFunc->ifTrue.get( ), func, line, board );
			std::cout << ")";
			
			//std::cout << ")*(";
		}
		else if ( ( (Var*)ex )->data->getVal( ) == '(' )
		{
			if ( ( (Brackets*)( ( (Var*)ex )->data.get( ) ) )->expresson->val == '~' ) {
				std::cout << ( (Var*)ex )->nameOfVar;
			}
			else
			{
				//std::cout << "(";
				special_drawExpr( ( (Var*)ex )->data.get( ), func, line, board );
				//std::cout << ")";
			}
		}
		else
		{
			//std::cout << "(";
			special_drawExpr( ((Var*)ex)->data.get( ), func, line, board );
			//std::cout << ")";
		}
	}
	else if ( c == 'n' )
	{
		std::cout << ( (N*)ex )->num;
	}
	else if ( c == '(' )
	{
		//std::cout << "(";
		special_drawExpr( ( (Brackets*)ex )->expresson.get( ), func, line, board );
		//std::cout << ")";
	}
	else if ( c == '@' )
	{

		//std::cout << "(!(";
		std::cout << "(ite ";
		special_drawExpr( ( (ConditionFunc*)ex )->condition.get( ), func, line, board );
		std::cout << " ";
		special_drawExpr( ( (ConditionFunc*)ex )->ifFalse.get( ), func, line, board );
		std::cout << " ";
		special_drawExpr( ( (ConditionFunc*)ex )->ifTrue.get( ), func, line, board );
		std::cout << ")";
		//std::cout << ")*(";


		/*std::cout << " [";
		special_drawExpr( ( (ConditionFunc*)ex )->condition.get( ), line );
		std::cout << "](";
		special_drawExpr( ( (ConditionFunc*)ex )->ifFalse.get( ), line );
		std::cout << ", ";
		special_drawExpr( ( (ConditionFunc*)ex )->ifTrue.get( ), line );
		std::cout << ") ";*/
	}
	/*else if ( c == '=' )
	{
		// std::cout << " " << ( (Assign*)ex )->name << " := ";
		special_drawExpr( ( (Assign*)ex )->meaning.back( ).first.get( ), line );
		std::cout << "[";
		unsigned size = ( (Assign*)ex )->meaning.back( ).second.size( );
		for ( unsigned i = 0; i < size; ++i )
		{
			if ( i != 0 )
			{
				std::cout << " AND ";
			}
			special_drawExpr( ( (Assign*)ex )->meaning.back( ).second[i].get( ), line );
		}
		std::cout << "]";
	}*/

};



void lexCangeBracketsOfFunctions( str_t& str, char opener, char closer )
{
	str_it start = std::begin( str ), end = std::end( str );

	while ( start != end ) {

		start = std::find( start, end, '(' );

		str_it it = start;
		
		while ( it != str.begin( ) && it - 1 != str.begin( ) ) {
			
			--it;
			bool guess;
			OR( guess,
				*it == '+', *it == '-',
				*it == '/', *it == '*',
				*it == '=', *it == '<',
				*it == '>', *it == '&',
				*it == '|', *it == '!',
				*it == '(', *it == ')',
				*it == '[', *it == ']' );

			if ( guess ) {

				if ( it + 1 != end ) {

					AND( guess,
						*( it + 1 ) != '(',
						!lexIsDigit( *(it + 1) ) );
					
					if ( guess ) {
						*start = opener;
						*( lexFindCloser( start, std::end( str ), '(', ')' ) - 1 ) = closer;
					}
				}

				break;
			}
		}
		if ( start != end ) ++start;
	}
}


void lexReplaseChByStr( str_t& str, char ch, str_t&& str2 )
{
	unsigned size = str.length( );

	for ( str_size_t jt = 0; jt != size; ++jt ) {

		if ( str[jt] == ch ) {

			str_size_t kill = jt + str2.length( );

			for ( auto&& kt = str2.rbegin( ); kt != str2.rend( ); ++kt ) {
				str.insert( std::begin( str ) + jt, *kt );
			}
			str.erase( std::begin(str) + kill );
			jt += str2.length( );
		}
	}
}


// Майнит из строки утверждения
pExpr lexAnalise(
	str_t str,									// анализируемая строка
	vec_pAssign* all_ids,					// имена всех аргументов
	vec_pair_pExpr_vecpExpr& ret_expr,	// варианты возвращаемых значений
	vec_pExpr& pi_expr	/*					// текущие значения */ )
{

	// Семь бед -
	// один ответ:
	vec_vec_str_t tr;
	lexRemoveIf(
				str,
				std::begin( str ),
				std::end( str ),
				[]( auto& it ) { return it == ' '; } );

	str = "(0)+(" + str + ")+(0)";// КОСТЫЛЬ и ВЕЛОСИПЕД!
	lexCangeBracketsOfFunctions( str, '[', ']' );
	lexReplaseChByStr( str, '[', "[(" );
	lexReplaseChByStr( str, ']', ")]" );
	lexReplaseChByStr( str, ',', "),(" );
	lexStringWithBracketsToStrangeTree( str, tr );

	return deleteZeros( anal( std::move( str ), tr, all_ids, ret_expr, pi_expr ) );
}


// превращает строку в странное дерево с решётками
void lexStringWithBracketsToStrangeTree( str_t& str, vec_vec_str_t& tr )
{
	while ( str.find( '(' ) != str_t::npos ) {

		str_size_t start = 0;
		str_size_t end = str.length( );
		str_size_t start1 = start, end1 = end;

		tr.push_back( vec_str_t( ) );

		while ( start1 != end1 ) {

			auto b = std::begin( str );
			start = std::find( start + b, end + b, '(' ) - b;

			while ( start != end ) {

				b = std::begin( str );
				end = lexFindCloser( start + b, end + b, '(', ')' ) - b;
				++start;
				--end;
				b = std::begin( str );

				if ( std::find( b + start, b + end, '(' ) == b + end ) {
					tr.back( ).push_back( std::string( b + start, b + end ) );
					unsigned m = tr.size( ) - 1, n = tr.back( ).size( ) - 1;
					str_size_t ins = start;

					std::for_each(
						b + start - 1,
						b + end + 1,
						[]( auto& it ) { it = '#'; } );

					++start;
					str.erase( b + start - 1, b + end + 1 );
					std::stringstream sst;
					sst << m; sst << ' '; sst << n;
					str_t str2 = sst.str( );
					str.insert( std::begin( str ) + ins, '#' );

					for ( auto it = str2.length( ) - 1; it != str_t::npos; --it ) {
						str.insert( std::begin( str ) + ins, str2[it] );
					}

					end = sst.str( ).size( ) + ( --start );
					end1 = str.length( );
				}
				b = std::begin( str );
				start = std::find( start + b, end + b, '(' ) - b;
			}
			start1 = end;
			end = std::end( str ) - std::begin( str );
			start = start1;
		}
	}
	tr.push_back( vec_str_t( ) );
	tr.back( ).push_back( str );
}

// удаляет два нуля из финального варианта выражения
pExpr deleteZeros( pExpr expr ) {
	return ( (Brackets*)( ( (Plus*)( ( (Plus*)expr.get( ) )->left.get( ) ) )->right.get( ) ) )->expresson;
}

#endif
