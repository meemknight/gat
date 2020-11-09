#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <iomanip>
#include <sstream>

#include "gat.h"
#include "parser.h"

int main(int argc, const char* argv[])
{

	if(argc <= 1)
	{
		char line[300];
		while (1)
		{

			std::cin.getline(line, 250);

			executeCommand(line);

			ZeroMemory(line, 300);
		}
	}else
	{
		std::stringstream stream;

		for(int i=1; i< argc; i++)
		{
			stream << argv[i] << ' ';
		}

		executeShellCommand(stream.str().c_str());
	
	}


	return 0;
}