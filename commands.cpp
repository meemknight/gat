#include <iostream>
#include "tools.h"
#include "parser.h"
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <chrono>

const char* CommandsNames[5] =
{
"",
"un-nou-inceput",
"arata-mi-calea",
"savarseste",
"", //end
};

void sintaxaInvalida()
{
	elog("Scriere eronata");
}

void(*CommandsHelp[])() =
{
	[]() {sintaxaInvalida(); },
	[]() {ilog("Initializeaza pravalia pentru cod.", "Declaratie: un-nou-inceput \"nume\" <facultativ>\"posizitie pravalie\""); },
	[]() {ilog("Afiseaza ajutor pentru comenzi. ", "Declaratie: arata-mi-calea \"nume comanda\""); },
	[]() {ilog("Aceasta comanda urca noua versiune", "Declaratie: savarseste \"nume savarsire\""); },
	[]() {},

};

void init(const char* c, int pos)
{
	std::cout << "init\n";

	auto args = parseStrings(&c[pos]);

	if(args.size() < 1)
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
		std::string pravalie;
		if(args.size() >= 2)
		{
			pravalie = args[1] + "//" + args[0];
		}else
		{
			pravalie = args[0];
		}

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

void savarsestef(const char* c, int pos)
{
	std::cout << "savarseste\n";

	auto args = parseStrings(&c[pos]);
	if (args.size() == 0)
	{
		sintaxaInvalida();
		CommandsHelp[Commands::savarsire]();
		return;
	}

	//verificare daca exista repository
	std::ifstream f(".gat//config.txt");
	if(!f.is_open())
	{
		elog("pravalia nu a fost creata. Incearca gat un-nou-inceput");
		return;
	}

	std::string name;
	std::string path;

	f >> name;
	f >> path;

	f.close();

	f.open(".gat//adauga.txt");
	if(!f.is_open())
	{
		elog("pravalie corupta");
		return;
	}

	std::vector<std::string> adaugari;

	while(!f.eof())
	{
		std::string s;
		std::getline(f, s);
		if (s != "")
		{
			adaugari.push_back(std::move(s));
		}
	}

	f.close();

	if(adaugari.size() == 0)
	{
		elog("niciun fisier adaugat in adauga");
		return;
	}

	if(std::filesystem::exists(path + "//" + args[0]))
	{
		elog("savarsirea deja exista");
		return;
	}
	
	CreateDirectory((path + "//" + args[0]).c_str(), nullptr);

	std::ofstream of((path + "//" + args[0] + "//" + "gat.txt").c_str());
	if (args.size() >= 2)
	{
		of << args[1] << "\n";
	}else
	{
		of << "...\n";
	}

	of << time(0);

	of.close();

	//todo add files

}