#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <iomanip>

#include "gat.h"

int main(int args, const char* argv[])
{
	
	if(args <= 1)
	{
		std::string line{};
		while (1)
		{

			std::getline(std::cin, line);

			executeCommand(line);

		}
	}else
	{
		for(int i=1; i<args; i++)
		{
			executeCommand(argv[i]);
		}
	
	}

	

	return 0;
}