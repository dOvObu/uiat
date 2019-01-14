#include <unordered_set>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <map>
#include "module_1.h"
#include "module_2.h"

void loadCode( const char path[], std::string& line );


int main( int argc, char* argv[] )
{
	setlocale( LC_ALL, "" );
	system( "echo \"(^_^)\" > errorLog.txt" );
	std::map< std::string, Func > func;

	//int val = 42;
	//std::cout << "My test: " << (int)(!( 0 == 0 )) << std::endl;
	
	std::string line = "";

	std::string codePath = "scratch.uiat";
	if ( argc > 1 ) {
		if ( str_t( argv[1] ).find( ".uiat" ) != str_t::npos ) {
			codePath = argv[1];
		}
	}
	loadCode( codePath.c_str(), line );
	elifToElseIf( line );

	//std::cout << line << std::endl;
	//ssa( line );
	//std::cout << line << std::endl;
	takeFuncDecls( line, func );

	execFunc( "main", func, line );//*/

	system( "pause" );
	return 0;
}



void loadCode( const char path[], std::string& line )
{
	std::ifstream fin( path );
	char buff[400];
	bool first_line = true;
	while ( !fin.eof( ) ) {
		fin.getline( buff, 400 );
		std::string getRidOfComents = buff;
		if ( first_line ) {
			first_line = false;
			if ( getRidOfComents.find( "z3" ) != std::string::npos ) {
				auto start = getRidOfComents.find( "\"" );
				auto str = getRidOfComents.substr( start+1, getRidOfComents.length( ) - start-1 );
				str = str.substr( 0, str.find( "\"" ) );
				if ( str.find( "z3" ) != std::string::npos ) {
					z3_path = str;
				}
			}
		}
		line += getRidOfComents.substr( 0, getRidOfComents.find( "#" ) );
	}
	fin.close( );
	// get rid of \t and \n
	auto size = line.size( );
	for ( unsigned i = 0; i < size; ++i ) {
		if ( line[i] == '\n' || line[i] == '\t' ) {
			line[i] = ' ';
		}
	}
	bool afterColumn = false;
	for ( unsigned i = 0; i < size; ++i ) {
		if ( afterColumn ) {
			if ( line[i] != ' ' ) {
				afterColumn = false;
			} else
			{
				line.erase( i, 1 );
				--i;
				size -= 1;
			}
		}
		if ( line[i] == ';' || line[i] == '{' || line[i] == '}' ) {
			afterColumn = true;
		}
	}
	// code review;
	// std::cout << line << std::endl;
	// system( "pause" );
}
