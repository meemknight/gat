#pragma once
#include <iostream>

extern const char* CommandsNames[];

enum Commands 
{
	none,
	initiaza,
	ajutor,
	savarsire,
	CommandsSize,
};

void init(const char* c, int pos);
void ajutorf(const char* c, int pos);
void savarsestef(const char* c, int pos);

