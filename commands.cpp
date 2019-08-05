#include <iostream>
#include "tools.h"
#include "parser.h"
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <set>

const char* CommandsNames[10] =
{
"",
"un-nou-inceput",
"arata-mi-calea",
"savarseste",
"adauga",
"statut",
"scoate",
"scb",
"rastoarna",
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
	[]() {ilog("Afiseaza ajutor pentru comenzi. ", "Declaratie: arata-mi-calea <facultativ>\"nume comanda\""); },
	[]() {ilog("Aceasta comanda urca noua versiune in pravalie", "Declaratie: savarseste \"nume savarsire\""); },
	[]() {ilog("Aceasta comanda adauga fisiere pentru a fi bagate in pravalie", "Declaratie: adauga <nr nelimitat parametri>\"nume fisier\""); },
	[]() {ilog("Aceasta comanda afiseaza fisierele care apar in adauga.txt sau informatii despre o savarsire", "Declaratie: statut <facultativ>\"nume pivnita\""); },
	[]() {ilog("Aceasta comanda scoate fisiere din pravalie", "Declaratie: scoate <nr nelimitat parametri>\"nume fisier\""); },
	[]() {ilog("Sterge cu buretele", "Declaratie: scb"); },
	[]() {ilog("Intoarce la o savarsire mai vechie", "Declaratie: rastoarna \"nume savarsire\""); },
	[]() {},

};

void init(const char* c, int pos)
{

	auto args = parseStrings(&c[pos]);

	if(args.size() < 1)
	{
		sintaxaInvalida();
		CommandsHelp[Commands::initiaza]();
		return;
	}

	if(GetFileAttributes(".gat") == INVALID_FILE_ATTRIBUTES)
	{
		llog("niciun director gasit, creand unul...");

		CreateDirectory(".gat", nullptr);

	}

	if(std::filesystem::exists(".gat/config.txt") || std::filesystem::exists(".gat/adauga.txt"))
	{
		wlog("Pravalia deja exista");
	}else
	{
		std::string pravalie;
		if(args.size() >= 2)
		{
			pravalie = args[1] + "/" + args[0];
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

		std::ofstream f(".gat/config.txt");
		f << args[0] << '\n' << pravalie;
		f.close();

		f.open(".gat/adauga.txt");
		f.close();

	}

};

void ajutorf(const char* c, int pos)
{
	auto a = parseCommandType(c, pos);
	if(a == Commands::none)
	{
		for(int i=1; i< Commands::CommandsSize; i++)
		{
			ilog(CommandsNames[i]);
			CommandsHelp[i]();
			ilog();
		}

	}else
	{
		CommandsHelp[a]();
	}
}

void savarsestef(const char* c, int pos)
{
	

	auto args = parseStrings(&c[pos]);
	if (args.size() == 0)
	{
		sintaxaInvalida();
		CommandsHelp[Commands::savarsire]();
		return;
	}

	//verificare daca exista repository
	std::ifstream f(".gat/config.txt");
	if(!f.is_open())
	{
		elog("pravalia nu a fost creata. Incearca gat un-nou-inceput");
		return;
	}

	llog("in curs de savarsire...");

	std::string name;
	std::string path;

	f >> name;
	///"cloud" path
	f >> path;

	f.close();

	f.open(".gat/adauga.txt");
	if(!f.is_open())
	{
		elog("pravalie corupta");
		return;
	}

	std::set<std::string> adaugari;

	while(!f.eof())
	{
		std::string s;
		std::getline(f, s);
		if (s != "")
		{
			adaugari.emplace(std::move(s));
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

	CreateDirectory((path).c_str(), nullptr);
	CreateDirectory((path + "/" + args[0]).c_str(), nullptr);

	std::ofstream of((path + "/" + args[0] + "/" + "gat.txt").c_str());
	if (args.size() >= 2)
	{
		of << args[1] << "\n";
	}else
	{
		of << "...\n";
	}

	of << time(0);

	of.close();


	for (auto &i : adaugari)
	{
		if (CopyFileA(i.c_str(), (path + "/" + args[0] + "/" + i).c_str(), false) != 0)
			glog("Urcat fisierul: ", i);
	}

	ilog("minune savarsita");
}

void adaugaf(const char *c, int pos)
{
	llog("adauga...");

	auto args = parseStrings(&c[pos]);
	if (args.size() == 0)
	{
		sintaxaInvalida();
		CommandsHelp[Commands::adauga]();
		return;
	}

	if(!std::filesystem::exists(".gat/adauga.txt"))
	{
		elog("Pravalia nu exista. Foloseste un-nou-inceput");
		return;
	}

	std::set<std::string> comenzi;
	std::ifstream file(".gat/adauga.txt");
		
	while (!file.eof())
	{
		std::string s;
		std::getline(file, s);
		if (s != "")
		{
			comenzi.emplace(std::move(s));
		}
	}
	file.close();

	for(auto &i: args)
	{
		comenzi.emplace(i);
	}

	std::ofstream of(".gat/adauga.txt");

	for(auto &i: comenzi)
	{
		of << i << "\n";
	}

}

void statutf(const char * c, int pos)
{
	if (!std::filesystem::exists(".gat/adauga.txt"))
	{
		elog("Pravalia nu exista. Foloseste un-nou-inceput");
		return;
	}

	auto args = parseStrings(&c[pos]);
	if (args.size() == 0)
	{
		std::ifstream f(".gat/adauga.txt");
		llog("Fisiere din adauga:");

		while (!f.eof())
		{
			std::string s;
			std::getline(f, s);
			if (s != "")
			{
				ilog(s);
			}
		}

		f.close();
	}else
	{
		std::ifstream f(".gat/config.txt");
		if (!f.is_open())
		{
			elog("pravalia nu a fost creata. Incearca gat un-nou-inceput");
			return;
		}

		std::string name;
		std::string path;

		f >> name;
		///"cloud" path
		f >> path;
		f.close();
		if(!std::filesystem::exists(name + "/" + args[0]))
		{
			elog("savarsirea nu exista");
			return;
		}

		f.open(name + "/" + args[0] + "/gat.txt");
		if (!f.is_open()) 
		{
			elog("savarsire corupta");
			return;
		}

		std::string comment;
		int date;

		f >> comment;
		f >> date;

		f.close();

		ilog(comment);
		ilog(date);


	}

	

	

}

void scoatef(const char * c, int pos)
{
	llog("scoate...");
	auto args = parseStrings(&c[pos]);
	if (args.size() == 0)
	{
		sintaxaInvalida();
		CommandsHelp[Commands::scoate]();
		return;
	}

	if (!std::filesystem::exists(".gat//adauga.txt"))
	{
		elog("Pravalia nu exista. Foloseste un-nou-inceput");
		return;
	}

	std::ifstream file(".gat//adauga.txt");
	std::set<std::string> comenzi;

	while (!file.eof())
	{
		std::string s;
		std::getline(file, s);
		if (s != "")
		{
			comenzi.emplace(std::move(s));
		}
	}
	file.close();


	for(auto &i: args)
	{
		comenzi.erase(comenzi.find(i));
	}

	std::ofstream f(".gat//adauga.txt");
	for (auto &i : comenzi)
	{
		f << i << "\n";
	}

}

void rastoarnaf(const char * c, int pos)
{
	
	auto args = parseStrings(&c[pos]);
	if (args.size() == 0)
	{
		sintaxaInvalida();
		CommandsHelp[Commands::rastoarna]();
		return;
	}

	//verificare daca exista repository
	std::ifstream f(".gat/config.txt");
	if (!f.is_open())
	{
		elog("pravalia nu a fost creata. Incearca gat un-nou-inceput");
		return;
	}

	

	std::string name;
	std::string path;

	f >> name;
	///"cloud" path
	f >> path;

	f.close();

	f.open(".gat/adauga.txt");
	if (!f.is_open())
	{
		elog("pravalie corupta");
		return;
	}

	std::set<std::string> adaugari;

	while (!f.eof())
	{
		std::string s;
		std::getline(f, s);
		if (s != "")
		{
			adaugari.emplace(std::move(s));
		}
	}

	f.close();

	if (adaugari.size() == 0)
	{
		elog("niciun fisier adaugat in adauga");
		return;
	}

	if(!std::filesystem::exists(path + "/" + args[0]))
	{
		elog("savarsire nu exista");
		return;
	}

	llog("in curs de rasturnare...\n");
	for (auto &i : adaugari)
	{
		if(CopyFileA((path + "/" + args[0] +"/"+ i).c_str(), (i).c_str(), 0) != 0)
		{
			ilog("Rasturnat fisier", i);
		}
	}

	llog("Fisiere rasturnate");

}
