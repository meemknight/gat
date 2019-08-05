#include "parser.h"

const char* CommandsNames[] =
{
"",
"initiaza",
"",
};

bool validate(const char *c, int &pos)
{
	
	for (; isspace(c[pos]); pos++);
	
	const char command[] = "gat ";

	for(int i=0; i<3; i++)
	{
		if(c[pos] != command[i])
		{
			return false;
		}

		pos++;
	}

	return true;
}

Commands parseCommandType(const char *c, int &pos)
{
	for (; isspace(c[pos]); pos++);
	int endIt = pos;

	for (; isspace(c[pos]); endIt++);

	std::string s(&c[pos], &c[endIt]);
	
	for(int i =0; i< Commands::CommandsSize; i++)
	{
		if(s.find(CommandsNames[i]))
		{
			return (Commands)i;
		}
	}

	pos = endIt;

	return Commands::none;
}

void execute(Commands command, const char *c, int &pos)
{
	switch (command)
	{
	case none:
		break;
	case initiaza:
		init();
		break;
	case CommandsSize:
		break;
	default:
		break;
	}

}
