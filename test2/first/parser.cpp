#include "parser.h"
#include <algorithm>
#include <sstream>
#include <iterator>

bool isBreakChar(char c)
{

return isspace(c) || c == '\n' || c == '\r' || c == 0;
}

bool validate(const char *c, int &pos)
{
	if (c[pos] == 0)
		return false;

	for (; isBreakChar(c[pos]); pos++);
	
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

Commands getCommandType(std::string &s)
{

	for (int i = 0; i < Commands::CommandsSize; i++)
	{
		
		if (s.find(CommandsNames[i]) != std::string::npos && CommandsNames[i][0] != 0)
		{
			return (Commands)i;
		}
	}
	return Commands::none;
}

Commands parseCommandType(const char *c, int &pos)
{

	for (; isBreakChar(c[pos]); pos++)
	{
		if(c[pos] == 0)
		{
			return Commands::none;
		}
	}

	int endIt = pos;

	for (; !isBreakChar(c[endIt]); endIt++);

	std::string s(&c[pos], &c[endIt]);
	pos = endIt;

	return getCommandType(s);

}

void execute(Commands command, const char *c, int &pos)
{
	switch (command)
	{
	case none:
		sintaxaInvalida();
		break;
	case initiaza:
		init(c, pos);
		break;
	case ajutor:
		ajutorf(c, pos);
		break;
	case savarsire:
		savarsestef(c, pos);
		break;
	case adauga:
		adaugaf(c, pos);
		break;
	case statut:
		statutf(c, pos);
		break;
	case scoate:
		scoatef(c, pos);
		break;
	case scb:
		system("cls");
		break;
	case rastoarna:
		rastoarnaf(c, pos);
		break;
	case compara:
		comparaf(c, pos);
		break;
	case istorie:
		istorief(c, pos);
		break;
	case ramura:
		ramuraf(c, pos);
		break;
	case adaugaRamura:
		adaugaRamuraf(c, pos);
		break;
	case afiseazaRamuri:
		afiseazaRamurif(c, pos);
		break;
	case uneste:
		unestef(c, pos);
		break;
	case CommandsSize:
		break;
	default:
		break;
	}

}

std::vector<std::string> parseStrings(std::string s)
{
	
	std::istringstream iss(s);
	std::vector<std::string> vect((std::istream_iterator<std::string>(iss)),
		std::istream_iterator<std::string>());

	
	return vect;
}
