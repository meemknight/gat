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
#include "analise.h"


const char* CommandsNames[11] =
{
"",
"purcede",
"arata-mi-calea",
"savarseste",
"adauga",
"statut",
"scoate",
"scb",
"rastoarna",
"compara",
"", //end
};

void sintaxaInvalida()
{
	elog("Scriere eronata");
}

void(*CommandsHelp[])() =
{
	[]() {sintaxaInvalida(); },
	[]() {llog("Initializeaza pravalia pentru cod.", "Declaratie: purcede \"nume\" <facultativ>\"posizitie pravalie\""); },
	[]() {llog("Afiseaza ajutor pentru comenzi. ", "Declaratie: arata-mi-calea <facultativ>\"nume comanda\""); },
	[]() {llog("Aceasta comanda urca noua versiune in pravalie", "Declaratie: savarseste \"nume savarsire\""); },
	[]() {llog("Aceasta comanda adauga fisiere pentru a fi bagate in pravalie", "Declaratie: adauga <nr nelimitat parametri>\"nume fisier\""); },
	[]() {llog("Aceasta comanda afiseaza fisierele care apar in adauga.txt sau informatii despre o savarsire.", "Declaratie: statut <facultativ>\"nume pivnita\""); },
	[]() {llog("Aceasta comanda scoate fisiere din pravalie.", "Declaratie: scoate <nr nelimitat parametri>\"nume fisier\""); },
	[]() {llog("Sterge cu buretele.", "Declaratie: scb"); },
	[]() {llog("Intoarce la o savarsire mai vechie.", "Declaratie: rastoarna \"nume savarsire\""); },
	[]() {llog("Compara doua savarsiri.", "Declaratie: compara\"nume savarsire\" \"nume savarsire\""); },
	[]() {},
	
};

void init(const char* c, int pos)
{

	if (std::filesystem::exists(".gat/config.txt") || std::filesystem::exists(".gat/adauga.txt"))
	{
		wlog("Pravalia deja exista");
		return;
	}

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

		std::filesystem::create_directory(".gat");

	}

	
	std::string pravalie;
	if(args.size() >= 2)
	{
		pravalie = args[1] + "/" + args[0];
	}else
	{
		pravalie = args[0];
	}

	if(GetFileAttributes(pravalie.c_str()) == INVALID_FILE_ATTRIBUTES)
	if (!std::filesystem::create_directory(pravalie.c_str()))
	{
		elog("locatie pravalie invalida");
		return;
	};

	std::ofstream f(".gat/config.txt");
	f << args[0] << '\n' << pravalie;
	f.close();

	f.open(".gat/adauga.txt");
	f.close();

	f.open(pravalie + "/gat.txt");

	f.close();

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

	std::filesystem::create_directory((path).c_str());
	std::filesystem::create_directory((path + "/" + args[0]).c_str());

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

	of.open(path + "/gat.txt", std::ios::app);

	of << args[0] << "\n";

	of.close();

	ilog("minune savarsita");

	analiseLastTwo(path + "/gat.txt", path);
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
		std::ifstream f(".gat/config.txt");
		if (!f.is_open())
		{
			elog("pravalia nu a fost creata. Incearca gat un-nou-inceput");
			return;
		}

		std::string name;
		std::string path;
		f >> name;
		f >> path;
		ilog("nume:", name);
		ilog("poteca:", path);
		f.close();

		f.open(".gat/adauga.txt");
		llog("Fisiere din adauga:");

		while (!f.eof())
		{
			std::string s;
			std::getline(f, s);
			if (s != "")
			{
				glog(s);
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
		if(!std::filesystem::exists(path + "/" + args[0]))
		{
			elog("savarsirea nu exista");
			return;
		}

		f.open(path + "/" + args[0] + "/gat.txt");
		if (!f.is_open()) 
		{
			elog("savarsire corupta");
			return;
		}

		std::string comment;
		int d;

		f >> comment;
		f >> d;

		f.close();

		ilog(comment);

		time_t t = d;
		struct tm *tm = localtime(&t);
		char date[20];
		strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm);

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
		auto it = comenzi.find(i);
		if(it != comenzi.end())
		{
			comenzi.erase(it);
		}

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

void comparaf(const char *c, int pos)
{
	auto args = parseStrings(&c[pos]);
	if (args.size() < 2)
	{
		sintaxaInvalida();
		CommandsHelp[Commands::compara]();
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
	f >> path;

	f.close();

	if(!std::filesystem::exists(path + "/" + args[0]) || !std::filesystem::exists(path + "/" + args[1]))
	{
		elog("savarsire gresita");
		return;
	}

	compareReps(path + "/" + args[0], path + "/" + args[1]);

}