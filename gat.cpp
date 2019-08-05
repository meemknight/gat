#include "gat.h"
#include "parser.h"

#include <iostream>


void executeCommand(const char* line)
{
	
	int pos = 0;
	if (!validate(line, pos)) 
	{
		return;
	};
	
	auto a = parseCommandType(line, pos);

	execute(a, line, pos);

}