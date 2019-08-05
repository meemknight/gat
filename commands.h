#pragma once
#include <iostream>

extern const char* CommandsNames[];

enum Commands 
{
	none,
	initiaza,
	ajutor,
	CommandsSize,
};

void init(const char* c, int pos);
void ajutorf(const char* c, int pos);
