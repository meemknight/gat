#include <iostream>
#include "tools.h"
#include "parser.h"
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <fstream>
#include <filesystem>

const char* CommandsNames[4] =
{
"",
"un-nou-inceput",
"arata-mi-calea",
"", //end
};

void sintaxaInvalida()
{
	elog("Scriere eronata");
}

void(*CommandsHelp[])() =
{
	[]() {sintaxaInvalida(); },
	[]() {llog("Initializeaza pravalia pentru cod.", "Declaratie: un-nou-inceput \"nume\" \"posizitie pravalie\""); },
	[]() {llog("Afiseaza ajutor pentru comenzi. ", "Declaratie: arata-mi-calea \"nume comanda\""); },
	[]() {},

};

void init(const char* c, int pos)
{
	std::cout << "init\n";

	auto args = parseStrings(&c[pos]);

	if(args.size() < 2)
	{
		sintaxaInvalida();
		CommandsHelp[Commands::initiaza]();
		return;
	}

	if(GetFileAttributes(".gat") == INVALID_FILE_ATTRIBUTES)
	{
		glog("niciun director gasit, creand unul...");

		CreateDirectory(".gat", nullptr);

	}

	if(std::filesystem::exists(".gat//config.txt") || std::filesystem::exists(".gat//adauga.txt"))
	{
		wlog("Pravalia deja exista");
	}else
	{
		std::string pravalie = args[1] + "//" + args[0];
		if(GetFileAttributes(pravalie.c_str()) == INVALID_FILE_ATTRIBUTES)
		if (!CreateDirectory(pravalie.c_str(), nullptr))
		{
			elog("locvatie pravalie invalid");
			return;
		};

		std::ofstream f(".gat//config.txt");
		f << args[0] << '\n' << pravalie;
		f.close();

		f.open(".gat//adauga.txt");
		f.close();

	}

};

void ajutorf(const char* c, int pos)
{
	auto a = parseCommandType(c, pos);
	CommandsHelp[a]();
}

