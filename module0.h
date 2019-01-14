#ifndef MODULE_0_H
#define MODULE_0_H

#include <unordered_set>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include "module1.h"



long symExec( Expr* ex, std::map<str_t, Func>& func, str_t& line )
{
	char c = ex->getVal( );
	switch ( c ) {
	case 'b': {
		long left = symExec( ( (BinOp*)ex )->left.get( ), func, line );
		long right = symExec( ( (BinOp*)ex )->right.get( ), func, line );
		long data = 0;
		char t = ( (BinOp*)ex )->type;
		switch ( t )
		{
		case '/' : { data = left / right; } break;
		case '*' : { data = left * right; } break;
		case '-' : { data = left - right; } break;
		case '+' : { data = left + right; } break;
		case '&' : { data = left && right; } break;
		case '|' : { data = left || right; } break;
		case '=' : { data = left == right; } break;
		case '<' : { data = left < right; } break;
		case '>' : { data = left > right; } break;
		case 'l' : { data = left <= right; } break;
		case 'm' : { data = left >= right; } break;
		default: break;
		}
		return data;
	} break;
	case 'u': {
		char t = ( (UnOp*)ex )->type;
		long data = symExec( ( (UnOp*)ex )->right.get( ), func, line );
		if ( t == '-' ) data = -data;
		if ( t == '!' ) data = !data;
		return data;
	} break;
	case 'c': {
		if ( func.count( ( (Call*)ex )->nameOfFunc ) != 0 )
		{
			/*if ( ( (Call*)ex )->nameOfFunc != "factorial" )
			{
				vec_pExpr buff;
				for ( auto& it : func[( (Call*)ex )->nameOfFunc].id ) {
					buff.push_back( ( (Brackets*)it->meaning[0].first.get( ) )->expresson );
				}
				unsigned i = 0;
				for ( auto& it : ( (Call*)ex )->args )
				{
					( (Brackets*)func[( (Call*)ex )->nameOfFunc].id[i]->meaning[0].first.get( ) )->expresson = it;
					++i;
				}
				long data = symExec( func[( (Call*)ex )->nameOfFunc].return_expression_monolit.get( ), func, line );
				i = 0;
				for ( auto& it : buff )
				{
					( (Brackets*)func[( (Call*)ex )->nameOfFunc].id[i]->meaning[0].first.get( ) )->expresson = it;
					++i;
				}
				return data;
			}
			else {*/
			
			if ( recursionCounter.count( ( (Call*)ex )->nameOfFunc ) == 0 ) { recursionCounter[( (Call*)ex )->nameOfFunc] = 0; }
			else { ++recursionCounter[( (Call*)ex )->nameOfFunc]; }

			if ( recursionCounter[( (Call*)ex )->nameOfFunc] < 90 )
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
					++i;
				}
				long data = symExec( foo.return_expression_monolit.get( ), func, line );
				i = 0;
				for ( auto& it : buff )
				{
					( (Brackets*)foo.id[i]->meaning[0].first.get( ) )->expresson = it;
					++i;
				}

				return data;
			}
			else
			{
				return 1;
			}
			//}
		}
		else {
			return 0;
		}
	} break;
	case 'i': {
		//std::cout << ( (Id*)ex )->id;
	} break;
	case 'v': {
		std::string name = ( (Var*)ex )->nameOfVar;
		if ( ( (Var*)ex )->withConditionFunc )
		{
			long data =
				symExec( ( (Var*)ex )->conditionFunc->condition.get( ), func, line )?
					symExec( ( (Var*)ex )->conditionFunc->ifTrue.get( ), func, line ) :
					symExec( ( (Var*)ex )->conditionFunc->ifFalse.get( ), func, line );
			return data;
		} else if ( ( (Var*)ex )->argData != nullptr )
		{
			//pAssign ass = ( (Var*)ex )->argData;
			//if ( !ass->active )
			//{
			//	std::cout << ass->name;
			//}
			//else
			//{
			//}
		} else
		{
			return symExec( ( (Var*)ex )->data.get( ), func, line );
		}
	}
	case 'n': {
			return ( (N*)ex )->num;
	} break;
	case '(': {
			return symExec( ( (Brackets*)ex )->expresson.get( ), func, line );
	} break;
	case '@': {
		long data =
			symExec( ( (ConditionFunc*)ex )->condition.get( ), func, line ) != 0 ?
				symExec( ( (ConditionFunc*)ex )->ifTrue.get( ), func, line ) :
				symExec( ( (ConditionFunc*)ex )->ifFalse.get( ), func, line );
		return data;
	} break;
	default: break;
	}
}


#endif
