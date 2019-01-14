#ifndef MODULE_n2_H
#define MODULE_n2_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>



struct IfPosTree
{
	std::vector<IfPosTree> progenys;
	std::string::size_type start;
	std::string::size_type start_of_body;
	std::string::size_type finish;
	bool full = false;
};

void drawVarNames( std::map<std::string, std::vector< std::pair<std::string::size_type, std::vector<double> > > >& varNames )
{
	for ( auto& jt : varNames ) {
		std::cout << jt.first << ": ";
		bool tr = true;
		for ( auto& it : jt.second ) {
			if ( tr ) {
				tr = false;
			} else {
				std::cout << ", ";
			}
			std::cout << it.first;
			std::cout << "{";
			bool tr2 = true;
			for ( auto& kt : it.second ) {
				if ( tr2 ) {
					tr2 = false;
				} else {
					std::cout << "| ";
				}
				std::cout << kt;
			}
			std::cout << "}";
		}
		std::cout << std::endl;
	}
}
void drawIfPosTree( unsigned tab, std::string& str, std::vector<IfPosTree>& root )
{
	unsigned t = 0;
	for ( auto& it : root ) {
		for ( unsigned i = 0; i < tab; ++i ) { std::cout << "   "; }
		std::cout << str.substr( it.start+t, it.finish - it.start+t ) << std::endl;
		drawIfPosTree( tab + 1, str, it.progenys );
	}
}

void findAllAssignments(
	std::string& str,
	std::map<std::string, std::vector< std::pair<std::string::size_type, std::vector<double> > > >& varNames )
{
	std::string::size_type it = str.find( ":=" );

	while ( it != std::string::npos )
	{
		std::string::size_type start = it;
		bool allright = false;

		while ( start != 0 ) // ����� � ����� ����� ����������
		{
			--start;
			if ( ( str[start] != ' ' ) && ( str[start] != '\n' ) && ( str[start] != '\t' ) )
			{
				break;
			}
		}
		std::string::size_type finish = start + 1;
		while ( start != 0 ) // ����� � ������ ����� ����������
		{
			--start;
			if ( ( str[start] == ' ' ) || ( str[start] == '}' ) || ( str[start] == '{' ) || ( str[start] == ';' ) )
			{
				allright = true;
				break;
			}
		}
		++start;


		if ( it + 2 < str.size( ) )
		{
			if ( allright && it + 2 < str.size() )
			{
				std::string::size_type s = str.substr( it + 2, str.size( ) - ( it + 2 ) ).find( ";" ) + it + 2;
				auto strr = str.substr( start, finish - start );
				if ( !varNames.count( strr ) ) { varNames[strr] = std::vector< std::pair<std::string::size_type,std::vector<double> > >( ); }
				varNames[strr].push_back( std::pair<std::string::size_type, std::vector<double> >( s, std::vector<double>( ) ) );
			}
			auto jt = it;
			it = str.substr( jt + 2, str.size( ) - ( jt + 2 ) ).find( ":=" );
			if ( it != std::string::npos ) { it += jt + 2; }
		}
		else
		{
			it = std::string::npos;
		}
	}

}


void numCorrectionRecu( IfPosTree& node, std::string::size_type& start, std::string::size_type& shift ) {

	if ( node.start > start ) node.start += shift;
	if ( node.start_of_body > start ) node.start_of_body += shift;
	if ( node.finish > start ) node.finish += shift;
	for ( auto& it : node.progenys ) { numCorrectionRecu( it, start, shift ); }
}
void numCorrection( std::vector<IfPosTree>& root, std::vector< std::pair<std::string::size_type, std::vector<double> > >& allvars, std::string::size_type start, std::string::size_type& shift ) {
	for ( auto& it : root ) { numCorrectionRecu( it, start, shift ); }
	for ( auto& jt : allvars ) { if ( jt.first > start ) { jt.first += shift; } }
}

void numCorrection( IfPosTree& node, std::string::size_type start ) {

	node.start += start;
	node.start_of_body += start;
	node.finish += start;

	for ( auto& it : node.progenys )
	{
		numCorrection( it, start );
	}
}

void fillIfPosTree( std::string str, IfPosTree& node )
{
	if ( node.progenys.empty( ) )
	{
		std::string::size_type i0 = str.find( " if " ), i1 = str.find( ";if " );
		std::string::size_type i2 = str.find( "}if " ), i3 = str.find( "{if " );
		std::string::size_type i_ = std::string::npos;
		i0 = i0 < i1 ? i0 : i1;
		i0 = i0 < i2 ? i0 : i2;
		i0 = i0 < i3 ? i0 : i3;
		i_ = i0;
		i0 = str.find( " if(" );
		i1 = str.find( ";if(" );
		i2 = str.find( "}if(" );
		i3 = str.find( "{if(" );
		i0 = i0 < i1 ? i0 : i1;
		i0 = i0 < i2 ? i0 : i2;
		i0 = i0 < i3 ? i0 : i3;
		i_ = i_ < i0 ? i_ : i0;

		i0 = str.find( "if(" );
		i1 = str.find( "if " );
		i0 = i0 < i1 ? i0 : i1;
		i0 = i0 < i2 ? i0 : i2;
		i0 = i0 < i3 ? i0 : i3;
		i_ = i_ < i0 ? i_ : i0;

		i0 = str.find( " else " );
		i1 = str.find( ";else " );
		i2 = str.find( "}else " );
		i0 = i0 < i1 ? i0 : i1;
		i0 = i0 < i2 ? i0 : i2;
		i_ = i_ < i0 ? i_ : i0;

		i0 = str.find( " else{" );
		i1 = str.find( ";else{" );
		i2 = str.find( "}else{" );
		i0 = i0 < i1 ? i0 : i1;
		i0 = i0 < i2 ? i0 : i2;
		i_ = i_ < i0 ? i_ : i0;

		i0 = str.find( "else " );
		i1 = str.find( "else{" );
		i0 = i0 < i1 ? i0 : i1;
		i0 = i_ < i0 ? i_ : i0;


		if ( i0 != std::string::npos )
		{
			node.start = i0;
			if ( str[node.start] == ' ' || str[node.start] == '}' || str[node.start] == '{' || str[node.start] == ';' ) { ++( node.start ); }
			node.start_of_body = str.substr( node.start, str.size( ) - node.start ).find( "{" ) + node.start;
			{
				node.finish = saveFindCloser( str, node.start_of_body, '{', '}' ) + 1;
			}

			if (
				node.finish != std::string::npos
				&& node.start_of_body != std::string::npos
				&& node.start != std::string::npos ) // ���� ��� ��������� �������� �������
			{
				node.full = true;

				node.progenys.push_back( IfPosTree( ) ); // ��������� ����� ��������
				fillIfPosTree(
					str.substr( node.start_of_body, node.finish - node.start_of_body ),
					node.progenys.back( ) ); // �������� � ���� �����
				
				if ( node.progenys.back( ).full ) { numCorrection( node.progenys.back( ), node.start_of_body ); }
				/*if ( node.progenys.back( ).full ) {
					std::cout << str.substr( node.progenys.back( ).start, node.progenys.back( ).finish - node.progenys.back( ).start ) << std::endl;
					std::cout << node.progenys.back( ).start << " " << node.progenys.back( ).finish - node.progenys.back( ).start << " " << str.size( ) << std::endl;
				}
				system( "pause" );//*/
				std::string::size_type substart = 0; // ������� �������

				// ����������, ���� ������� �� ����� �� ������� ������, � ��������� ������� ����� �����
				while ( substart < str.size( ) && node.progenys.back( ).full )
				{
					{
						auto tmp = substart;
						substart += node.progenys.back( ).finish - 1;
						if ( substart >= str.size( ) && substart < tmp ) {
							break;
						}
					}
					node.progenys.push_back( IfPosTree( ) );
					if ( node.finish - substart < str.size( ) )
					{
						fillIfPosTree(
							str.substr( substart, node.finish - substart ),
							node.progenys.back( ) );
						if ( node.progenys.back( ).full ) { numCorrection( node.progenys.back( ), substart ); }
						/*if ( node.progenys.back( ).full ) {
							std::cout << str.substr( node.progenys.back( ).start, node.progenys.back( ).finish - node.progenys.back( ).start ) << std::endl;
							std::cout << node.progenys.back( ).start << " " << node.progenys.back( ).finish - node.progenys.back( ).start << " " << str.size( ) << std::endl;
						}
						system( "pause" );//*/
					}
					else
					{
						break;
					}
				}
				if ( !node.progenys.back( ).full ) {
					node.progenys.pop_back( );
				}
			}
		}
	}
}
void fillVecOfIfPosTree( std::string& str, std::vector<IfPosTree>& root )
{
	std::string::size_type i0 = str.find( " Func " ), i1 = str.find( ";Func " );
	std::string::size_type i2 = str.find( "}Func " ), i3 = str.find( "Func " );
	i0 = i0 < i1 ? i0 : i1;
	i0 = i0 < i2 ? i0 : i2;
	i0 = i0 < i3 ? i0 : i3;

	if ( i0 != std::string::npos && i0 < str.size() )
	{
		++i0;
		std::string::size_type start_of_body, finish;
		start_of_body = str.substr( i0, str.size( ) - i0 ).find( "{" ) + i0;
		finish = saveFindCloser( str, start_of_body, '{', '}' ) + 1;
		if ( finish == start_of_body + 1 ) return;

		root.push_back( IfPosTree( ) );

		fillIfPosTree(
			str.substr( start_of_body, finish - start_of_body ),
			root.back( ) );
		if ( root.back().full ) { numCorrection( root.back( ), start_of_body ); }
		/* if ( root.back( ).full ) {
			std::cout << str.substr( root.back( ).start, root.back( ).finish - root.back( ).start ) << std::endl;
			std::cout << root.back( ).start << " " << root.back( ).finish - root.back( ).start << " " << str.size( ) << std::endl;
		}
		system( "pause" );*/

		std::string::size_type substart = 0;//start_of_body;

		while ( root.back( ).full && substart < str.size( ) )
		{
			root.push_back( IfPosTree( ) );
			substart += root[root.size()-2].finish;
			if ( substart < str.size() )
			{
				fillIfPosTree(
					str.substr( substart, str.size( ) - substart ),
					root.back( ) );
				if ( root.back( ).full ) { numCorrection( root.back( ), substart ); }
				/* if ( root.back( ).full ) {
					std::cout << str.substr( root.back( ).start, root.back( ).finish - root.back( ).start ) << std::endl;
					std::cout << root.back( ).start << " " << root.back( ).finish - root.back( ).start << " " << str.size( ) << std::endl;
				}
				system( "pause" );*/
			}
			else
			{
				break;
			}
		}

		// if ( substart < str.size( ) ) root.pop_back( );
		if ( !root.back( ).full ) { root.pop_back( ); }
	}
}



void setPathForAssignment(
	std::vector<IfPosTree>& progenys, // ���� ������� 
	std::string::size_type& pos,	// ��������� ����������
	std::vector<double>& ret /*	//	���� � ���� ������� */ )
{
	unsigned i = 0;
	for ( auto& node : progenys )
	{
		if ( pos < node.start ) {
			ret.push_back( i - 0.5 );
			return;
		}
		else if ( pos > node.start_of_body && pos < node.finish ) {
			ret.push_back( i );
			setPathForAssignment( node.progenys, pos, ret );
			return;
		}
		++i;
	}
	if ( !progenys.empty( ) ) {
		if ( pos > progenys.back( ).finish ) {
			ret.push_back( (double)( progenys.size( ) ) - 0.5 );
		}
	}
}



void addPathToAssignments(
	std::map<std::string, // �������� ����������.
				std::vector< // ������
					std::pair<//		���
						std::string::size_type,	// ��������������
						std::vector<double>		// � ���� �� ����� �������. {-0.5, [0], +0.5, [1], 1.5, [2], 2.5}
					>
				>
	>& varNames,
	std::vector<IfPosTree>& root )
{
	for ( auto& jt : varNames ) // ��� ������� �������� ����������
	{
		for ( auto& it : jt.second ) // ��� ������ ���� 
		{
			auto pos = it.first;
			setPathForAssignment( root, pos, it.second );
		}
	}
}



void addSubAssignments(
	std::string& str,						// �������������� ������
	std::string& varname,				// ��� ������� ����������
	std::vector<double>& varpos,		// ��������� ����������
	std::vector<IfPosTree>& root,		// ������ � ����� �������
	std::string::size_type from,		// ������ ��������������� �����
	std::string::size_type to,			// ����� ��������������� �����
	std::vector< std::pair<std::string::size_type, std::vector<double> > >& allvars )
{
	IfPosTree* node = nullptr;
	std::vector<IfPosTree>* neighbors = nullptr;

	unsigned steps = varpos.size( );
	//std::cout << "varpos.size() " << " " << varpos.size( ) << std::endl;
	double order;
	int type = 1;
	for ( unsigned i = 0; i < steps; ++i ) // ����������� �� ������� � ��� ���� �������
	{
		order = varpos[i];
		if ( (double)( (int)( order ) ) == order ) {
			if ( i == 0 ) {
				node = &( root[(unsigned)order] ); // ����� ���� ������
				neighbors = &( root );
			} else {
				node = &( node->progenys[(unsigned)order] );
				neighbors = &( node->progenys );
			}
			continue;
		}
		if( order == -0.5 ) {
			type = 0;// ����� ���� ����� ������� ����� �������
			if ( node == nullptr ) {
				node = &( root[0] );
				neighbors = &( root );
			} else {
				node = &( node->progenys[0] );
				neighbors = &( node->progenys );
			}
			break; // ����� ����� ���� ������ � ����� �����
		}
		else {
			type = 2;// ����� ���� ����� ���� � ������� (unsigned)order
			if ( node == nullptr ) {
				node = &( root[(unsigned)order+1] );
				neighbors = &( root );
			}
			else {
				node = &( node->progenys[(unsigned)order+1] );
				neighbors = &( node->progenys );
			}
			break; // ����� ����� ���� ������ � ����� �����
		}
	}
	// ������� ���� �������
	if ( node != nullptr ) {
		// ������ ������ �� ����� ����������������� ����������
		if ( type == 0 ) { // ���������� - ��������� � ���������� ��� ���� ������� � ���� ����� ����
			std::string newStr = str + std::to_string( 0 ) + " := " + str + ";";
			auto len = varname.size( );
			str.insert(std::begin( str ) + node->start, std::begin( varname ), std::end( varname ) );
			numCorrection( root, allvars, node->start, len ); if ( to > node->start ) { to += len; }
			newStr = str + std::to_string( 1 ) + " := " + str + std::to_string( 0 ) + ";";
			str.insert( std::begin( str ) + node->start_of_body+1, std::begin( varname ), std::end( varname ) );
			numCorrection( root, allvars, node->start_of_body + 1, len ); if ( to > node->start_of_body ) { to += len; }

			unsigned highest = 1;
			// � �������������� ����� ������� ������ ���������� � ������ ���������� [varname] � ��������� � ��� �����
			// �������� � ������� ����� �������, ������ � �������� �� ��� �����
			

			str.insert( std::begin( str ) + node->finish-1, std::begin( varname ), std::end( varname ) );
			numCorrection( root, allvars, node->finish - 1, len );
			newStr = str + std::to_string( 0 ) + " := " + str + std::to_string( 1 ) + ";";
			str.insert( std::begin( str ) + node->finish, std::begin( varname ), std::end( varname ) );
			newStr = str + std::to_string( highest + 1 ) + " := " + str + std::to_string( 0 ) + ";";
			numCorrection( root, allvars, node->finish, len ); if ( to > node->finish ) { to += len; }


			neighbors;
			return;
		}
		if ( type == 1 ) { // ���������� - ��������� ������ ������-�� ����� �������
			str[node->start_of_body + 1] = 'X';
		}
		if ( type == 2 ) { // ���������� - ��������� � ��������� ����� ������� �������
			str[node->start - 1] = 'X';
		}
	}
}





void ssa( std::string& str )
{	
	std::map<std::string, std::vector< std::pair<std::string::size_type, std::vector<double> > > > varNames;
	findAllAssignments( str, varNames );
	
	std::vector<IfPosTree> root;
	fillVecOfIfPosTree( str, root );

	otstup( ); drawIfPosTree( 0, str, root ); otstup( );

	addPathToAssignments( varNames, root );
	drawVarNames( varNames ); otstup( );

	for ( auto& jt : varNames ) {
		std::string varname = jt.first;
		addSubAssignments( str, varname, jt.second[0].second, root, jt.second[0].first, str.size() - 1, jt.second );
	}

	//std::cout << str << std::endl;

	std::cout << "root.size: " << root.size() << std::endl;
}


#endif
