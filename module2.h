#ifndef MODULE_2_H
#define MODULE_2_H

#include <unordered_set>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include "module3.h"


const char asser_str[] = "assert";
const char retur_str[] = "return";
const char prin_str[] = "print";
const char check_str[] = "check";
const char show_model_str[] = "show_model";
vec_pAssign* all_id_of_curr_Func = nullptr;




// Майнит деревья утверждений из строки
char lexGetTypeOfStatement(
	str_t& str,									// анализируемая строка
	vec_pStatement& stmt,					// вывод набора утверждений
	vec_pAssign* all_ids,					// имена всех аргументов
	vec_pair_pExpr_vecpExpr& ret_expr,	// варианты возвращаемых значений
	vec_pExpr& pi_expr	/*					// текущие условия		*/ )
{
	str_size_t begin1;
	auto itBegin1 = std::begin( str ), itEnd1 = std::end( str );

	if ( str.find( '{' ) != str_t::npos ) {
		lexFindBody( itBegin1, itEnd1 );
		begin1 = itBegin1 - std::begin( str );
	}
	else
	{
		begin1 = str_t::npos;
	}

	auto itBegin2 = std::begin( str ), itEnd2 = std::end( str );
	str_t::iterator bc, ec, bb, eb;
	lexFindIf( str, itBegin2, itEnd2, bc, ec, bb, eb );

	auto begin2 = ( itBegin2 != itEnd2 ) ? itBegin2 - std::begin( str ) : str_t::npos;
	auto begin3 = str.find( str_t(asser_str)+' ' ); {
		auto tmp = str.find( str_t(asser_str)+'(' );
		begin3 = tmp < begin3 ? tmp : begin3;
		tmp = str.find( str_t(asser_str)+'{' );
		begin3 = tmp < begin3 ? tmp : begin3;
	}
	auto begin4 = str.find( str_t(retur_str)+' ' ); {
		auto tmp = str.find( str_t( retur_str )+'(' );
		begin4 = tmp < begin4 ? tmp : begin4;
	}
	auto begin4dot3 = str.find( str_t(check_str )+'(' );
	auto begin4dot4 = str.find( str_t(show_model_str)+'(' );
	auto begin4dot5 = str.find( str_t(prin_str)+' ' ); {
		auto tmp = str.find( str_t(prin_str)+'(' );
		begin4dot5 = tmp < begin4dot5 ? tmp : begin4dot5;
		tmp = str.find( str_t(prin_str)+'{' );
		begin4dot5 = tmp < begin4dot5 ? tmp : begin4dot5;
		tmp = str.find( str_t(prin_str)+'\"' );
		begin4dot5 = tmp < begin4dot5 ? tmp : begin4dot5;
		tmp = str.find( str_t(prin_str)+'\'' );
		begin4dot5 = tmp < begin4dot5 ? tmp : begin4dot5;
	}
	auto begin5 = str.find( ":=" );

	char ch = 'E'; // предположили, что выражение
	llexTypeOfStatementCh( str, stmt, begin1, begin2, begin3, begin4, begin4dot3, begin4dot4, begin4dot5, begin5, ch ); // узнали, что на самом деле

	switch ( ch ) {
	case 'B': {
		Body* term;
		stmt.push_back( pStatement( term = new Body( ) ) );
		bb = std::begin( str ); eb = std::end( str );
		lexFindBody( bb, eb );
		str_t bod( bb, eb );
		// 1
		lexGetPartsOfSt( bod, std::begin( bod ), std::end( bod ), term->statement, all_ids, ret_expr, pi_expr );
	}	break; // тело

	case '?': {

		std::map<str_t, unsigned> lastMatch;
		getLastMatch( lastMatch );
		for ( auto& it : lastMatch ) {
			str_t right = attachMarker( it.first, it.second );
			if ( allAssignments.count( it.first ) != 0 ) {
				str_t tmp = attachMarker( it.first, it.second + 1 ) + " := " + right;
				lexGetTypeOfStatement( tmp, stmt, all_ids, ret_expr, pi_expr );
			}
		}

		If* term;
		stmt.push_back( pStatement( term = new If( ) ) );

		str_t expr( bc, ec );
		term->expression = lexAnalise( expr, all_ids, ret_expr, pi_expr );
		str_t bod( bb, eb );

		pi_expr.push_back( term->expression );
		lexGetPartsOfSt( bod, std::begin( bod ), std::end( bod ), term->body.statement, all_ids, ret_expr, pi_expr ); // 1

		std::map<str_t, unsigned> lastMatch2;
		getLastMatch( lastMatch2 );
		for ( auto& it : lastMatch ) {
			if ( allAssignments.count( it.first ) != 0 ) {
				if ( lastMatch[it.first] + 1 != lastMatch2[it.first] ) {
					str_t tmp = attachMarker( it.first, lastMatch[it.first] + 1 ) + " := " + attachMarker( it.first, lastMatch2[it.first] );
					lexGetTypeOfStatement( tmp, term->body.statement, all_ids, ret_expr, pi_expr );
				}
			}
		}
		pi_expr.pop_back( );

		std::map<str_t, unsigned> lastMatch3;
		getLastMatch( lastMatch3 );
		for ( auto& it : lastMatch ) {
			if ( allAssignments.count( it.first ) != 0 ) {
				str_t tmp = attachMarker( it.first, lastMatch3[it.first] + 1 ) + " := " + attachMarker( it.first, it.second + 1 );
				lexGetTypeOfStatement( tmp, stmt, all_ids, ret_expr, pi_expr );
			}
		}

		// добавляем замыкающие переменные

	}	break; // обработка "если"

	case ':': {

		std::map<str_t, unsigned> lastMatch;
		getLastMatch( lastMatch );
		for ( auto& it : lastMatch ) {
			str_t right = attachMarker( it.first, it.second );
			if ( allAssignments.count( it.first ) != 0 ) {
				str_t tmp = attachMarker( it.first, it.second + 1 ) + " := " + right;
				lexGetTypeOfStatement( tmp, stmt, all_ids, ret_expr, pi_expr );
			}
		}

		If* term;
		stmt.push_back( pStatement( term = new If( ) ) );
		Not* term_not;
		term->expression = pNot( term_not = new Not( ) );

		unsigned shift = 2;
		bool guess = ( stmt.end( ) - shift )->get( )->getVal( ) == '?';
		while ( !guess && shift < stmt.size( ) ) {
			if ( ( stmt.end( ) - shift )->get( )->getVal( ) != '~' ) { break; }
			++shift;
			guess = ( stmt.end( ) - shift )->get( )->getVal( ) == '?';
		}
		check( guess, "\nStatement_Error::else_mast_to_be_located_right_after_if\n" );

		term_not->right = ( ( (If*)( ( stmt.end( ) - shift )->get( ) ) )->expression );
		str_it start = std::find( std::begin( str ), std::end( str ), '{' );
		
		pi_expr.push_back( term->expression );
		lexGetPartsOfSt( str, start, std::end( str ), term->body.statement, all_ids, ret_expr, pi_expr ); // 1
		std::map<str_t, unsigned> lastMatch2;
		getLastMatch( lastMatch2 );
		for ( auto& it : lastMatch ) {
			if ( allAssignments.count( it.first ) != 0 ) {
				if ( lastMatch[it.first] + 1 != lastMatch2[it.first] ) {
					str_t tmp = attachMarker( it.first, lastMatch[it.first] + 1 ) + " := " + attachMarker( it.first, lastMatch2[it.first] );
					lexGetTypeOfStatement( tmp, term->body.statement, all_ids, ret_expr, pi_expr );
				}
			}
		}
		
		pi_expr.pop_back( );

		std::map<str_t, unsigned> lastMatch3;
		getLastMatch( lastMatch3 );
		for ( auto& it : lastMatch ) {
			if ( allAssignments.count( it.first ) != 0 ) {
				str_t tmp = attachMarker( it.first, lastMatch3[it.first] + 1 ) + " := " + attachMarker( it.first, it.second + 1 );
				lexGetTypeOfStatement( tmp, stmt, all_ids, ret_expr, pi_expr );
			}
		}

	}	break; // обработка "иначе"

	case 'a': {
		Assert* term;
		//stmt.push_back( pStatement( term = new Assert( ) ) );
		auto size = str.size( );
		auto start = str.find( str_t(asser_str) + " " );
		{
			auto tmp = str.find( str_t(asser_str) + "(" );
			start = tmp < start ? tmp : start;
			tmp = str.find( str_t(asser_str) + "{" );
			start = tmp < start ? tmp : start;
		}

      str_t expr;
		
		auto test = start + arrLen( asser_str ) - 1;

		if ( str[start + arrLen( asser_str ) - 1] == '{' )
		{
			expr = str.substr( start + arrLen( asser_str ), size - start - arrLen( asser_str )-1 );
			auto closer = expr.size( );
			start = 0;
			while ( start < closer )
			{
				while ( ( expr[start] == ' ' || expr[start] == '\n' || expr[start] == '\t' ) && start != closer ) { ++start; }
				if ( start == expr.size( ) ) break;
				if ( start != closer ) {
					//str_t sub_expr;
					
					auto finish = start + 1;
					while ( expr[finish] != ';' && finish < expr.size( ) ) { ++finish; }
					auto ststr = expr.substr( start, finish - start );
					stmt.push_back( pStatement( term = new Assert( ) ) );
					term->expression = lexAnalise( ststr, all_ids, ret_expr, pi_expr );
					{
						unsigned size = pi_expr.size( );
						for ( unsigned i = 0; i < size; ++i )
						{
							term->pi_expr.push_back( pi_expr[i] );
						}
						//sub_expr = expr.substr( start, finish - start );
						start = finish + 1;
					}
				} else {
					break;
				}
			}

		}
		else { // assert 1 = 1; or assert( 1 = 1 );
			expr = str.substr( start + arrLen( asser_str ), size - start - arrLen( asser_str ) );
			//auto closer = expr.size( );
			//start = 0;
			//while ( start < closer )
			//{
			//	while ( ( expr[start] == ' ' || expr[start] == '\n' || expr[start] == '\t' ) && start != closer ) { ++start; }
			//	if ( start != closer ) {
					//expr = str.substr( start + arrLen( asser_str ), size - start - arrLen( asser_str ) );
					stmt.push_back( pStatement( term = new Assert( ) ) );
					term->expression = lexAnalise( expr, all_ids, ret_expr, pi_expr ); {
						unsigned size = pi_expr.size( );
						for ( unsigned i = 0; i < size; ++i )
						{
							term->pi_expr.push_back( pi_expr[i] );
						}
					}
			//	} else {
			//		break;
			//	}
			//}

		}
		
		// special_drawExpr( term->expression.get( ) );

	}	break; // утверждение

	case 'r': {
		Return* term;
		stmt.push_back( pStatement( term = new Return() ) );
		auto size = str.size( );
		auto start = str.find( str_t(retur_str)+" " ); {
			auto tmp = str.find( str_t(retur_str)+"(" );
			start = tmp < start ? tmp : start;
		}
		str_t expr;
		if ( str[start + arrLen( retur_str ) - 2] == '(' )
		{
			expr = str.substr( start + arrLen( retur_str ) - 1, size - start - arrLen( retur_str ) );
			term->phi_expr.first = ( (Brackets*)lexAnalise( expr, all_ids, ret_expr, pi_expr ).get( ) )->expresson;
		}
		else
		{
			expr = str.substr( start + arrLen( retur_str ), size - start - arrLen( retur_str ) );
			term->phi_expr.first = lexAnalise( expr, all_ids, ret_expr, pi_expr );
		}
		unsigned sasize = pi_expr.size( );
		for ( unsigned i = 0; i < sasize; ++i )
		{
			term->phi_expr.second.push_back( pi_expr[i] );
		}
		ret_expr.push_back( term->phi_expr );
	}	break; // возврат из функции

	case 'M': {
		PrintModel* term;
		stmt.push_back( pStatement( term = new PrintModel( ) ) );
		auto size = str.size( );
		auto start = str.find( str_t( show_model_str ) + "(" );
		str_t expr = str.substr( start + arrLen( show_model_str ) - 1, size - start - arrLen( show_model_str ) + 1 );
		start = expr.find( "(" );
		auto closer = lexFindCloser( std::begin( expr ) + start, std::end( expr ), '(', ')' ) - std::begin( expr );
		auto comma = expr.find( ',' );
		if ( comma != str_t::npos ) {
			term->nameOfFunc = expr.substr( 0, comma );
			expr = expr.substr( start + arrLen( show_model_str ), closer - start - arrLen( show_model_str ) - 1 );
			std::stringstream st;
			st << expr;
			st >> term->evaluations;
		} else { 
			expr = expr.substr( 1, expr.length( ) - 1 );
			std::stringstream st;
			st << expr;
			st >> term->nameOfFunc;
		}

	}	break; // получить модель

	case 't': {
		CheckSAT* term;
		stmt.push_back( pStatement( term = new CheckSAT( ) ) );
		auto size = str.size( );
		auto start = str.find( str_t(check_str) + "(" );
		str_t expr = str.substr( start + arrLen(check_str) - 1, size - start - arrLen(check_str) + 1 );
		start = expr.find( "(" );
		auto closer = lexFindCloser( std::begin( expr ) + start, std::end( expr ), '(', ')' ) - std::begin( expr );
		auto comma = expr.find( ',' );
		if ( comma != str_t::npos ) {
			term->nameOfFunc = expr.substr( 1, comma - 1 ); {
				std::stringstream st;
				st << term->nameOfFunc;
				st >> term->nameOfFunc;
			}
			expr = expr.substr( comma + 1, closer - comma - 2 );
			std::stringstream st;
			st << expr;
			st >> term->board;
		} else {
			term->nameOfFunc = expr.substr( 1, expr.length( ) - 2 ); {
				std::stringstream st;
				st << term->nameOfFunc;
				st >> term->nameOfFunc;
			}
		}

	}	break; // проверить выполнимость

	case 'P': {
		Print* term;
		stmt.push_back( pStatement( term = new Print( ) ) );
		auto size = str.size( );
		auto start = str.find( str_t( prin_str ) + " " );
		{
			auto tmp = str.find( str_t( prin_str ) + "(" );
			start = tmp < start ? tmp : start;
			tmp = str.find( str_t( prin_str ) + "{" );
			start = tmp < start ? tmp : start;
			tmp = str.find( "{" );
			if ( tmp != str_t::npos ) {
				std::swap( str[start + arrLen( retur_str ) - 2], str[tmp] );
			}
		}
		
		str_t expr;
		if ( str[start + arrLen( retur_str ) - 2] == '{' )
		{
			expr = str.substr( start + arrLen( retur_str ) - 1, size - start - arrLen( retur_str ) );
			auto closer = expr.size( );
			start = 0;
			while ( start < closer )
			{
				while ( ( expr[start] == ' ' || expr[start] == '\n' || expr[start] == '\t' ) && start != closer ) { ++start; }
				if ( start == expr.size( ) ) break;
				if ( start != closer ) {
					str_t sub_expr;
					if ( expr[start] == '\"' )
					{
						auto finish = ++start;
						while ( expr[finish] != '\"' ) { ++finish; }
						term->order.push_back( true );
						term->stringsToPrint.push_back( expr.substr( start, finish - start ) );
						sub_expr = expr.substr( start, finish - start );
						start = finish + 1;
					} else if ( expr[start] == ';' )
					{
						term->order.push_back( true );
						term->stringsToPrint.push_back( "\n" );
						++start;
					} else if ( expr[start] == '\'' )
					{
						auto finish = ++start;
						while ( expr[finish] != '\'' ) { ++finish; }
						term->order.push_back( true );
						term->stringsToPrint.push_back( expr.substr( start, finish - start ) );
						sub_expr = expr.substr( start, finish - start );
						start = finish + 1;
					} else
					{
						auto finish = start + 1;
						while ( finish != closer && expr[finish] != '\'' && expr[finish] != '\"' && finish < expr.size( ) ) { ++finish; }
						term->order.push_back( false );
						term->expressions.push_back( lexAnalise( expr.substr( start, finish - start ), all_ids, ret_expr, pi_expr ) );
						sub_expr = expr.substr( start, finish - start );
						start = finish-1;
					}
				} else {
					break;
				}
			}

		}
		else // print "some deep shit"; or print("some deep shit"); or print"some deep shit";
		{
			if ( str[start + arrLen( retur_str ) - 2] == ' ' ) {
				expr = str.substr( start + arrLen( retur_str ) - 2, size - start - arrLen( retur_str ) + 2 );
			}
			else if ( str[start + arrLen( retur_str ) - 2] == '(' )
			{
				expr = str.substr( start + arrLen( retur_str ) - 1, size - start - arrLen( retur_str ) );
			}
			else if ( str[start + arrLen( retur_str ) - 1] == '\"' || str[start + arrLen( retur_str ) - 1] == '\'' )
			{
				expr = str.substr( start + arrLen( retur_str ) - 1, size - start - arrLen( retur_str ) + 2 );
			}

			auto closer = expr.size( );
			start = 0;
			while ( start < expr.size( ) )
			{
				while ( ( expr[start] == ' ' || expr[start] == '\n' || expr[start] == '\t' ) && start != closer ) { ++start; }
				if ( start != closer ) {
					str_t sub_expr;
					if ( expr[start] == '\"' )
					{
						auto finish = ++start;
						while ( expr[finish] != '\"' ) { ++finish; }
						term->order.push_back( true );
						term->stringsToPrint.push_back( expr.substr( start, finish - start ) );
						sub_expr = expr.substr( start, finish - start );
						start = finish + 1;
					} else if ( expr[start] == '\'' )
					{
						auto finish = ++start;
						while ( expr[finish] != '\'' ) { ++finish; }
						term->order.push_back( true );
						term->stringsToPrint.push_back( expr.substr( start, finish - start ) );
						sub_expr = expr.substr( start, finish - start );
						start = finish + 1;
					} else
					{
						auto finish = start + 1;
						while ( finish != closer && expr[finish] != '\'' && expr[finish] != '\"' && finish < expr.size( ) ) { ++finish; }
						term->order.push_back( false );
						term->expressions.push_back( lexAnalise( expr.substr( start, finish - start ), all_ids, ret_expr, pi_expr ) );
						sub_expr = expr.substr( start, finish - start );
						start = finish;
					}
				} else {
					break;
				}
			}

		}
		
	}	break; // печать

	case '~': {

		/*********************************
		* #assign								*
		*					ПРИСВОЕНИЕ			*
		*											*
		**********************************/

		str_size_t end = str.find( ":=" ) - 1, start = end + 3;
		while ( str[end] == ' ' || str[end] == '\t' || str[end] == '\n' ) --end;
		str_t name = str.substr( 0, end + 1 );
		str_t expr = str.substr( start, str.size( ) - start );

		//otstup( 5 );
		//std::cout << "expr = |" << expr <<"|" << std::endl;
		//otstup( 5 );
		
		Assign* term = nullptr;

		// Если ещё не разу не встречалось такой переменной
		auto buff = std::make_pair<pExpr, vec_pExpr>(
			lexAnalise( expr, all_ids, ret_expr, pi_expr ),
			vec_pExpr( ) );

		std::map<str_t, unsigned> lastMatch;
		getLastMatch( lastMatch );
		name = attachMarker( name, lastMatch[name] );

		if ( allAssignments.count( name ) == 0 )
		{
			allAssignments[name] = pCoreAssign( new CoreAssign( name.c_str( ) ) );
		}
		//else
		//{
		//	term = allAssignments[name].get( );
		//}
		pAssign sh( term = new Assign( allAssignments[name] ) );
		//term = allAssignments[name].get( );

		// если мы в if-блоке
		bool we_are_in_if_block = !pi_expr.empty( );
		
		term->meaning.push_back( buff );
		

		if ( we_are_in_if_block )
		{
			for ( auto it = std::begin( pi_expr ); it != std::end( pi_expr ); ++it )
			{
				term->meaning.back( ).second.push_back( *it );
			}
		}

		term->if_expr.push_back( we_are_in_if_block );
		//

		//if ( we_are_in_if_block ) {
		//	std::cout << "\nWe are in IF block " << pi_expr.back( ).get( ) << std::endl;
		//}


		stmt.push_back( pStatement( sh ) );
		//std::cout << "stmt test " << stmt.back( ).get( )->getVal( );

		term->name = name;
	}	break; // присвоение

	case 'E': {
		stmt.push_back( lexAnalise( str, all_ids, ret_expr, pi_expr ) );
	}	break; // выражение
	
	default: break;
	}

	return ch;
}


bool isNotWhiteSpace( str_t& str ) {
	for ( auto& it : str ) { if ( it != ' ' ) { return true; } }
	return false;
}

// Возвращает список аргументов объявления функции
void lexGetArguments(
	str_t& str,						// анализируемая строка
	str_it begin,					// начало '('
	str_it end,						// конец ')'
	vec_pAssign& args /*			// возврящаемый список аргументов	*/ )
{
	auto it = begin;
	pCoreAssign sh;
	while ( it != end ) {
		if ( *it == ',' ) {
			{
				args.push_back( pAssign( new Assign( sh = pCoreAssign( new CoreAssign( str.substr( begin - std::begin( str ) + 1, it - begin - 1 ).c_str( ) ) ) ) ) );
				std::stringstream s;
				s << args.back( )->name;
				s >> args.back( )->name;
				if ( !allAssignments.count( args.back( )->name ) ) { allAssignments[args.back( )->name] = sh; }
			}
			begin = it;
			++it;
		}
		++it;
	}
	//if ( str.substr( begin - std::begin( str ) + 1, it - begin - 1 ).find( ")" ) == std::string::npos ) {
	str_t tmp = str.substr( begin - std::begin( str ) + 1, it - begin - 2 );
	if ( isNotWhiteSpace( tmp ) ) {
		args.push_back(
			pAssign( new Assign(
				sh = pCoreAssign( new CoreAssign( tmp.c_str( ) ) ) ) ) );
		{
			std::stringstream s;
			s << args.back( )->name;
			s >> args.back( )->name;
		}
		if ( !allAssignments.count( args.back( )->name ) ) { allAssignments[args.back( )->name] = sh; }
	}
	//}
}


// Находит >{ тело }<
void lexFindBody( str_it &begin, str_it &end )
{
	begin = std::find( begin, end, '{' );
	end = lexFindCloser( begin, end, '}' );
}


// Ищет начала основных частей уловия
void lexFindIf(
	str_t& str,			// анализируемая строка
	str_it &begin,				//  >if
	str_it &end,				//   тело}<
	str_it &conditionBegin,	// >условие
	str_it &conditionEnd,	//  условие<
	str_it &bodyBegin,		// >{тело
	str_it &bodyEnd	/*		//   тело}<		*/ )
{
	auto pos = str.find( "if" );
	
	if ( pos == str_t::npos ) {
		begin = end;
		return;
	}
	
	begin += pos;
	if ( begin == end ) { return; }

	bool guess;

	OR( guess,
		*( begin + 2 ) == ' ',
		*( begin + 2 ) == '\n',
		*( begin + 2 ) == '\t',
		*( begin + 2 ) == '(' );

	if ( !guess ) {
		begin = end;
		return;
	}

	str_it jt( str.begin( ) );
	pos = str.find( "then" );

	OR( guess,
		pos == str_t::npos,
		pos < begin - std::begin( str ) );

	if ( guess ) {
		begin = end;
		return;
	}

	jt += pos;
	
	OR( guess,
		*( jt - 1 ) == ' ',
		*( jt - 1 ) == '\n',
		*( jt - 1 ) == '\t',
		*( jt - 1 ) == ')',
		*( jt - 4 ) == ' ',
		*( jt - 4 ) == '\n',
		*( jt - 4 ) == '\t',
		*( jt - 4 ) == '{' );
	
	if ( !guess || jt == end ) {
		begin = end;
		return;
	}

	auto bb = jt, eb = end;
	lexFindBody( bb, eb );
	
	OR( guess,
		bb == end,
		eb - 1 == end );

	if ( guess ) {
		begin = end;
		return;
	}

	end = eb;
	conditionBegin = begin + 2;
	conditionEnd = jt - 1;
	bodyBegin = bb;
	bodyEnd = eb;
}


// Майнит дерево выражений из заданной строки
pExpr lexGetExpression(
	str_t& str,									// анализируемая строка
	vec_pAssign* all_ids,					// имена всех аргументов
	vec_pair_pExpr_vecpExpr& ret_expr,	// варианты возвращаемых значений
	vec_pExpr& pi_expr /*					// список деревьев утверждений */ )
{
	if ( str.size(  ) != 0 ) {
		auto key = str.find( '(' );
		if ( key != str_t::npos ) {
			Brackets* expression;
			pExpr expr( expression = new Brackets( ) );

			auto b = std::begin( str ), e = std::end( str );
			str_t sstr( str.substr( key + 1, lexFindCloser( b + key, e, ')' ) - b - 1 ) );
			expr = lexAnalise( sstr, all_ids, ret_expr, pi_expr );

			return expr;
		} else {
			return lexAnalise( str.c_str(  ), all_ids, ret_expr, pi_expr );
		}
	}
	return pExpr( new Empty( ) );
}



// Решает что встретилось раньше: тело, условие, assert, return, или присвоение
inline void llexTypeOfStatementCh(
	str_t& str,							// анализируемая строка
	vec_pStatement& stmt,			// список утверждений
	str_size_t& begin1,				// точка начала тела		'{'тело'}'
	str_size_t& begin2,				// точка начала условия	'if'условие'then' или 'else'
	str_size_t& begin3,				// точка начала assert-а
	str_size_t& begin4,				// точка начала return-а
	str_size_t& begin4dot3,			// точка начала check
	str_size_t& begin4dot4,			// точка начала show_model
	str_size_t& begin4dot5,			// точка начала print-а
	str_size_t& begin5,				// точка начала присвоения
	char& return_ch )
{
	auto min = begin1;
	if ( !( begin1 == begin2 && begin2 == begin3 && begin3 == begin4 && begin4 == begin4dot3 && begin4dot3 == begin4dot4 && begin4dot4 == begin4dot5 && begin4dot5 == begin5 && begin5 == str_t::npos ) ) {
		if ( begin1 < begin2 ) return_ch = 'B'; else { return_ch = '?'; min = begin2; }
		if ( min > begin3 ) { return_ch = 'a'; min = begin3; }
		if ( min > begin4 ) { return_ch = 'r'; min = begin4; }
		if ( min > begin4dot3 ) { return_ch = 't'; min = begin4dot3; }
		if ( min > begin4dot4 ) { return_ch = 'M'; min = begin4dot4; }
		if ( min > begin4dot5 ) { return_ch = 'P'; min = begin4dot5; }
		if ( min > begin5 ) { return_ch = '~'; min = begin5; }
	}
	str_size_t else_pos = str.find( "else" );
	if ( stmt.size( ) != 0 && else_pos != str_t::npos ) {
		if ( stmt.back( ).get( )->getVal( ) == '?' ) {
			return_ch = ':';
		}
	}
}




// Разделяет в тексте тела на деревья утверждений
void lexGetPartsOfSt(
	str_t& str,									// анализируемая строка 
	str_it begin,									// начало анализируемого фрагмента '{'
	str_it end,										// конец анализируемого	фрагмента	 '}'
	vec_pStatement& stmt,					// список утверждений
	vec_pAssign* all_ids,					// имена всех аргументов
	vec_pair_pExpr_vecpExpr& ret_expr,	// варианты возвращаемых значений
	vec_pExpr& pi_expr	/*					// текущие условия			*/ )
{
	auto it = begin + 1;
	--end;
	unsigned c = 0;
	while ( it != end ) {
		if ( *it == '{' ) ++c;
		if ( *it == '}' ) --c;

		if ( ( *it == ';' || *it == '}' ) && c == 0 ) {
			
			str_t str2
				= str.substr(
					begin - std::begin( str ) + 1,
					it - begin - ( *it == ';' ) ).c_str( );

			//std::cout << "set: " << str2 << std::endl;
			char chhhh = lexGetTypeOfStatement( str2, stmt, all_ids, ret_expr, pi_expr );
			//std::cout << "was type: " << chhhh << std::endl;
			if ( chhhh == 'r' ) { // тело, после return-а исполняться не может
				break;
			}
			begin = it;
			++it;
		}
		if ( it != end )
			++it;
	}
}



// Проверяет наличие символа справа не обращая внимания на white space
bool leftward(str_t& str, str_size_t start, char ch )
{
	--start;
	while ( start != 0 ) {
		if ( str[start] == ' ' || str[start] == '\n' || str[start] == '\t' ) --start;
		else if ( str[start] == ch ) return true;
		else return false;
	}
}





// Переводит сокращение elif ... then{} в else { if ... then {} }
void elifToElseIf( str_t& str )
{
	str_t target = "elif";
	str_t alternative = "else { if";
	auto startElif = str.find( target );
	while ( startElif != str_t::npos ) {
		if ( !leftward( str, startElif, '}' ) && !leftward( str, startElif, ';' ) ) { break; }
		auto startOfElifBody = startElif + str.substr( startElif, str.size( ) - startElif ).find( "{" );
		auto endOfElifBody = lexFindCloser( startOfElifBody + std::begin( str ), std::end( str ), '{', '}' );
		str.insert( endOfElifBody, '}' );
		str.erase( startElif, target.size( ) );
		str.insert( std::begin( str ) + startElif, std::begin( alternative ), std::end( alternative ) );
		startElif = str.find( target );
	}
}





// Майнит функции из текста
void takeFuncDecls(
	str_t term,							// анализируемая строка
	std::map<str_t,Func>& func 	/* множество функций, доступных для вызова*/ )
{
	//std::cout << term << std::endl;
	//std::cout << std::endl;
	str_t keyWord = "Func ";
	std::vector<unsigned> fidPos;
	unsigned numOfDecls = lexCount( term, keyWord, fidPos );
	auto begin = std::begin( term ), end = std::end( term );
	for ( unsigned i = 0; i < numOfDecls; ++i ) {

		auto startName = begin + fidPos[i];
		auto rbrOpener = std::find( startName, end, '(' );
		
		check( rbrOpener != std::end( term ), "Unacceptable_Func_Declaration::Forgot_To_Open_Parentheses" );
		auto rbrCloser = lexFindCloser( rbrOpener, end, ')' );
		check( rbrCloser != std::end( term ), "Unacceptable_Func_Declaration::Forgot_To_Close_Parentheses" );
		str_t name( term.substr( startName - begin, rbrOpener - startName ) );
		{
			std::stringstream s;
			s << name;
			s >> name;
		}
		check( name.length( ), "Unacceptable_Func_Declaration::Forgot_To_Write_Name" );
		check( !lexIsDigit( name[0] ), "Unacceptable_Func_Declaration::Name_Of_Func::Cannot_Start_With_A_Digit" );
		
		func.insert( std::make_pair( name.c_str(  ), Func(  ) ) );
		lexGetArguments( term, rbrOpener, rbrCloser, func[name.c_str(  )].id );

		for ( auto& it : func[name.c_str(  )].id ) {
			check( !lexIsDigit( it->name[0] ), "Unacceptable_Func_Declaration::Name_Of_Argument::Cannot_Start_With_A_Digit" );
		}

		rbrOpener = std::find( rbrCloser, end, '{' );
		rbrCloser = lexFindCloser( rbrOpener, end, '}' );

		for ( pAssign& it : func[name.c_str( )].id ) {
			it->if_expr.push_back(false);
			it->active = false;
			it->meaning.push_back( std::make_pair<pExpr, vec_pExpr>( pExpr( new Brackets( ) ), vec_pExpr( ) ) ); // очередной костыль
			( (Brackets*)it->meaning.back( ).first.get( ) )->expresson = pExpr( (Expr*)it.get( ), []( auto i ) {} );
			//std::cout << name << "   " << it->name << std::endl;
			// system( "pause" );
			allAssignments[it->name] = it->core;//*/
		}

		vec_pExpr pi_expr; // список текущих условий
		vec_pair_pExpr_vecpExpr ret_expr;
		all_id_of_curr_Func = &( func[name.c_str( )].id );
		lexGetPartsOfSt( term, rbrOpener, rbrCloser, func[name.c_str(  )].body.statement, all_id_of_curr_Func, ret_expr, pi_expr );
		
		all_id_of_curr_Func = nullptr;
		vec_pExpr allRetExprs;
		ConditionFunc* cond_expr;
		func[name].return_expression_monolit = pExpr( cond_expr = new ConditionFunc( ) );
		{
			unsigned size = ret_expr.size( ), j = 0;
			for ( auto& it : ret_expr )
			{
				++j;
				cond_expr->condition = getLexAND( it.second );
				cond_expr->ifTrue = it.first;
				if ( j < size ) {
					cond_expr->ifFalse = pExpr( new ConditionFunc( ) );
					cond_expr = (ConditionFunc*)cond_expr->ifFalse.get( );
				}
			}
			cond_expr->ifFalse = pExpr( new N( 0 ) );
		}
		allAssignments.clear( );
	}
}


// Майнит функцию из текста
void takeFuncDecl(
	str_t term,					// анализируемая строка
	str_t& nameOfFunc,		// имя искомой функции
	Func& ret 				/*	// заполняемая функция [инициализацию производите снаружи :) ]*/ )
{
	//str_t keyWord = "Func ";
	//std::vector<unsigned> fidPos;
	//unsigned numOfDecls = lexCount( term, keyWord, fidPos );
	//auto begin = std::begin( term ), end = std::end( term );
	//for ( unsigned i = 0; i < numOfDecls; ++i ) {

	//	auto startName = begin + fidPos[i];
	//	auto rbrOpener = std::find( startName, end, '(' );

	//	auto rbrCloser = lexFindCloser( rbrOpener, end, ')' );
	//	str_t name( term.substr( startName - begin, rbrOpener - startName ) );
	//	{
	//		std::stringstream s;
	//		s << name;
	//		s >> name;
	//	}
	//	if ( name != nameOfFunc ) { continue; }

	//	lexGetArguments( term, rbrOpener, rbrCloser, ret.id );

	//	rbrOpener = std::find( rbrCloser, end, '{' );
	//	rbrCloser = lexFindCloser( rbrOpener, end, '}' );

	//	for ( auto& it : ret.id ) {
	//		it->if_expr.push_back( false );
	//		it->active = false;
	//		it->meaning.push_back( std::make_pair<pExpr, vec_pExpr>( pExpr( (Expr*)it.get( ), []( auto i ) {} ), vec_pExpr( ) ) );
	//		allAssignments[it->name] = it->core;
	//	}
		str_t keyWord = "Func ";
		std::vector<unsigned> fidPos;
		unsigned numOfDecls = lexCount( term, keyWord, fidPos );
		auto begin = std::begin( term ), end = std::end( term );
		for ( unsigned i = 0; i < numOfDecls; ++i ) {

			auto startName = begin + fidPos[i];
			auto rbrOpener = std::find( startName, end, '(' );

			check( rbrOpener != std::end( term ), "Unacceptable_Func_Declaration::Forgot_To_Open_Parentheses" );
			auto rbrCloser = lexFindCloser( rbrOpener, end, ')' );
			check( rbrCloser != std::end( term ), "Unacceptable_Func_Declaration::Forgot_To_Close_Parentheses" );
			str_t name( term.substr( startName - begin, rbrOpener - startName ) );
			if ( name == nameOfFunc ) {
				{
					std::stringstream s;
					s << name;
					s >> name;
				}
				check( name.length( ), "Unacceptable_Func_Declaration::Forgot_To_Write_Name" );
				check( !lexIsDigit( name[0] ), "Unacceptable_Func_Declaration::Name_Of_Func::Cannot_Start_With_A_Digit" );

				ret = Func( );
				lexGetArguments( term, rbrOpener, rbrCloser, ret.id );

				for ( auto& it : ret.id ) {
					check( !lexIsDigit( it->name[0] ), "Unacceptable_Func_Declaration::Name_Of_Argument::Cannot_Start_With_A_Digit" );
				}

				rbrOpener = std::find( rbrCloser, end, '{' );
				rbrCloser = lexFindCloser( rbrOpener, end, '}' );

				for ( pAssign& it : ret.id ) {
					it->if_expr.push_back( false );
					it->active = false;
					it->meaning.push_back( std::make_pair<pExpr, vec_pExpr>( pExpr( new Brackets( ) ), vec_pExpr( ) ) ); // очередной костыль
					( (Brackets*)it->meaning.back( ).first.get( ) )->expresson = pExpr( (Expr*)it.get( ), []( auto i ) {} );
					//std::cout << name << "   " << it->name << std::endl;
					// system( "pause" );
					allAssignments[it->name] = it->core;//*/
				}

				vec_pExpr pi_expr; // список текущих условий
				vec_pair_pExpr_vecpExpr ret_expr;
				all_id_of_curr_Func = &( ret.id );
				lexGetPartsOfSt( term, rbrOpener, rbrCloser, ret.body.statement, all_id_of_curr_Func, ret_expr, pi_expr );

				all_id_of_curr_Func = nullptr;
				vec_pExpr allRetExprs;
				ConditionFunc* cond_expr;
				ret.return_expression_monolit = pExpr( cond_expr = new ConditionFunc( ) );
				{
					unsigned size = ret_expr.size( ), j = 0;
					for ( auto& it : ret_expr )
					{
						++j;
						cond_expr->condition = getLexAND( it.second );
						cond_expr->ifTrue = it.first;
						if ( j < size ) {
							cond_expr->ifFalse = pExpr( new ConditionFunc( ) );
							cond_expr = (ConditionFunc*)cond_expr->ifFalse.get( );
						}
					}
					cond_expr->ifFalse = pExpr( new N( 0 ) );
				}
				allAssignments.clear( );
			}
		}
}


#endif

