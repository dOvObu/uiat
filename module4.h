#ifndef MODULE_4_H
#define MODULE_4_H

#include <unordered_set>
#include <strstream>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <map>


#ifdef _WIN32
#define REMOVE_FILE "del"
#elif _unix_
#define REMOVE_FILE "rm"
#else
#define REMOVE_FILE "rm"
#endif

const std::string remove_file_command( REMOVE_FILE );
std::string z3_path = "z3";


// Порядок объявления:
void check( bool notShit, std::string&& logInfo );
void otstup( int n );
bool lexIsDigit( char c );
bool lexIsLetter( char c );
template<typename T, typename Y>
inline void lexParse( const T& from, Y& to );
std::string::iterator lexFindCloser( std::string::iterator itOpener, std::string::iterator end, char opener, char closer );
std::string::iterator lexFindCloser( std::string::iterator itOpener, std::string::iterator end, char closer );
std::string::size_type findCloser( std::string& str, std::string::size_type openerPos, std::string::size_type endPos, char closer );
unsigned lexCount( std::string& from, std::string& what );
unsigned lexCount( std::string& from, std::string& what, std::vector<unsigned>& fidPos );
template <typename T>
void lexRemoveIf( std::string& str, std::string::iterator&& begin, std::string::iterator&& end, T foo );

//const char assert_str[] = "assert";
template <typename T, size_t N>
size_t arrLen( T( &arr )[N] )
{
	return N;
}

template <typename T>
bool wasAddedToVec( std::vector<T>& vec, T thing ) {
	for ( T& it : vec ) {
		if ( it == thing ) { return true; }
	}
	return false;
}

void check( bool notShit, std::string&& logInfo )
{
	if ( !notShit ) {
		system( ( std::string( "echo " ) + logInfo + std::string( " >> errorLog.txt" ) ).c_str( ) );
		abort( );
	}
}

void otstup( int n = 1 )
{
	for ( int i = 0; i < n; ++i ) std::cout << std::endl;
}


bool lexIsDigit( char c ) { return c >= '0' && c <= '9'; }
bool lexIsLetter( char c ) { return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ); }

template<typename T, typename Y>
inline void lexParse( const T& from, Y& to )
{
	std::stringstream st;
	st << from;
	st >> to;
}

std::string::iterator lexFindCloser(
		std::string::iterator itOpener,
		std::string::iterator end,
		char opener,
		char closer )
{
	unsigned c = 1;
	auto it = itOpener;
	++it;
	while ( it != end && c != 0 ) {

		if ( *it == opener ) {
			++c;
		} else if ( *it == closer ) {
			//std::cout << "lex " << *it << std::endl;
			--c;
		}
		++it;
	}
	return it;
}


std::string::size_type saveFindCloser(
	std::string& str,
	std::string::size_type itOpen,
	char opener,
	char closer )
{
	unsigned c = 1;
	std::string::size_type it = itOpen;
	std::string::size_type end = str.size( );
	++it;
	if ( str[it] != closer )
	while ( c != 0 && it < end ) {

		if ( str[it] == opener ) {
			++c;
		} else if ( str[it] == closer ) {
			--c;
		}
		if ( str[it] == closer && c == 0 )
		{
			break;
		}
		++it;
	}
	return it;
}

std::string::iterator lexFindCloser(
		std::string::iterator itOpener,
		std::string::iterator end,
		char closer )
{
	return lexFindCloser( itOpener, end, *itOpener, closer );
}

std::string::size_type findCloser(
		std::string& str,
		std::string::size_type openerPos,
		std::string::size_type endPos,
		char closer )
{
	return lexFindCloser(
			str.begin( ) + openerPos,
			str.begin( ) + endPos,
			*(str.begin( ) + openerPos),
			closer ) - str.begin( );
}


/*template<typename T>
void getListOfDevidetThings( std::string& str, std::vector<T>& args, std::string::size_type start, std::string::size_type end, char devider )
{
	std::string tmp = str.substr( start + 1, end - start - 1 );
	std::stringstream s;
	while ( tmp.size( ) ) {
		start = 0;
		end = tmp.find_first_of( devider );
		int not_end = 1;
		if ( end == std::string::npos ) {
			end = tmp.length( );
			not_end = 0;
		}
		std::string ttmp = tmp.substr( start, end - start );
		deletePrefWhiteSpace( ttmp );
		deleteBackWhiteSpace( ttmp );
		args.push_back( ttmp.c_str( ) );
		s << tmp.substr( start, end - start );
		tmp.erase( start, end + not_end );
	}
}*/



unsigned lexCount( std::string& from, std::string& what )
{
	unsigned c = 0;
	auto it = std::begin( from ), end = std::end( from );
	auto wIt = std::begin( what ), wEnd = std::end( what );
	unsigned size = wEnd - wIt, s = 0;
	while ( it != end ) {
		if ( size != s ) {
			if ( *it == *( wIt + s ) ) {
				++s;
			} else {
				s = 0;
			}
		} else {
			s = 0;
			++c;
		}
		++it;
	}
	if ( size == s ) {
		++c;
	}

	return c;
}

unsigned lexCount( std::string& from, std::string& what, std::vector<unsigned>& fidPos )
{
	unsigned c = 0;
	auto it = std::begin( from ), end = std::end( from );
	auto wIt = std::begin( what ), wEnd = std::end( what );
	unsigned size = wEnd - wIt, s = 0;
	while ( it != end ) {
		if ( size != s ) {
			if ( *it == *( wIt + s ) ) {
				++s;
			} else {
				s = 0;
			}
		} else {
			fidPos.push_back( it - std::begin( from ) );
			s = 0;
			++c;
		}
		++it;
	}
	if ( size == s ) {
		fidPos.push_back( it - std::begin( from ) );
		++c;
	}

	return c;
}

template <typename T>
void lexRemoveIf( std::string& str, std::string::iterator&& begin, std::string::iterator&& end, T foo )
{
	unsigned hmm = 0;
	std::remove_if( begin, end, [&hmm, &foo]( auto& it ) { hmm += foo( it ); return foo( it ); } );
	str.erase( end - hmm, end );
}

// Шаблонные сокращения
template<typename... Booleans>
inline void AND( bool& ret, Booleans&& ...vars )
{
	ret = true;
	const unsigned size = sizeof...( vars );
	bool res[size] = { vars... };
	for ( unsigned i = 0; i < size; ++i ) {
		ret = ret && res[i];
	}
}

template<typename... Booleans>
inline void OR( bool& ret, Booleans&& ...vars )
{
	ret = false;
	const unsigned size = sizeof...( vars );
	bool res[size] = { vars... };
	for ( unsigned i = 0; i < size; ++i ) {
		ret = ret || res[i];
	}
}

template<typename T, typename... Ts>
inline void MIN( T& ret, Ts&& ...vars )
{
	const unsigned size = sizeof...( vars );
	T res[size] = { vars... };
	for ( unsigned i = 0; i < size; ++i ) {
		ret = ret < res[i] ? ret : res[i];
	}
}

template<typename T, typename... Ts>
inline void MAX( T& ret, Ts&& ...vars )
{
	const unsigned size = sizeof...( vars );
	T res[size] = { vars... };
	for ( unsigned i = 0; i < size; ++i ) {
		ret = ret > res[i] ? ret : res[i];
	}
}


#endif
