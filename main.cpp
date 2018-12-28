#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>



class Print
{
public:
	std::vector< std::shared_ptr<std::string> > arrayToPrint;
};



bool lexNextIs(
		const std::string& tartget,
		std::string::size_type start,
		std::string& str )
{
	auto end = str.substr( start+1, str.size()-start-1 ).find( tartget )+start;
	if ( end == std::string::npos ) return false;

	for ( auto i = start; i <= end; ++i ) {
		if (str[i] == ';'){
			return true;
		}
		if ( str[i] != ' ' && str[i] != '\n' && str[i] != '\t' ) {
			return false;
		}
	}

	return false;
}



bool lexGetPrint(std::string& str, Print& pr)
{
	auto begin = str.find("print ");

	if ( begin == std::string::npos ) { begin = str.find("print("); }
	if ( begin == std::string::npos ) { begin = str.find("print\t"); }
	if ( begin == std::string::npos ) { begin = str.find("print\n"); }
	if ( begin == std::string::npos ) { begin = str.find("print;"); }
	if ( begin == std::string::npos ) { begin = str.find("print{"); }

	bool body =
		(str.find("{")!=std::string::npos) && (str.find("}")!=std::string::npos);

	if ( begin != std::string::npos ) {

		auto size = str.size(  );
		std::string::size_type start = 0, end = size;
		bool in_str = false;
		int needNewLineOnNextStep = 0;

		for ( std::string::size_type i = 0; i < size; ++i ) {

			if ( str[i] == '\"' ) {
				in_str = !in_str;

				if ( in_str ) {
					start = i;
				} else {

					end = i;

					if ( pr.arrayToPrint.size() == 0 || needNewLineOnNextStep ) {

						pr.arrayToPrint.push_back(
								std::shared_ptr<std::string>(
									new std::string(str.substr(start+1, end-start-1))));

					} else {
						(*pr.arrayToPrint.back()) += str.substr(start+1,end-start-1);
					}
					needNewLineOnNextStep= lexNextIs( ";",i+1,str );
					//if (needNewLineOnNextStep!=0) --needNewLineOnNextStep;
				}
			}
		}
	}
	return false;
}



void forPrintPrint(Print& pr){
	for( auto& it : pr.arrayToPrint ) {
		std::cout << *it << std::endl;
	}
	std::cout << std::endl;
}



int main()
{
	std::ifstream fin("test.code");
	std::string code="";
	while ( !fin.eof() ){
		char buff[255];
		fin.getline(buff,255);
		code = code + buff;
	}
	fin.close();
	Print pr;
	lexGetPrint(code, pr);
	forPrintPrint(pr);
	

	return 0;
}
