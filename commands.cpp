#include <iostream>
#include "tools.h"

const char* CommandsNames[4] =
{
"",
"initiaza",
"ajutor",
"", //end
};

void(*CommandsHelp[])() =
{
	[]() {},
	[]() {llog("Initializeaza pravalia pentru cod.", "Declaratie: initiaza \"nume\" \"posizitie pravalie\""); },
	[]() {llog("Afiseaza ajutor pentru comenzi. ", "Declaratie: ajutor\"nume comanda\""); },
	[]() {},

};

void init(const char* c, int pos)
{
	std::cout << "init";
};

void ajutorf(const char* c, int pos)
{
	std::cout << "ajutor";

}

