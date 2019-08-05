#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <iomanip>

#include "gat.h"

int main(int args, const char* argv[])
{
	
	if(args <= 1)
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
		for(int i=1; i<args; i++)
		{
			executeCommand(argv[i]);
		}
	
	}

	

	return 0;
}