#pragma once
#include <iostream>

extern const char* CommandsNames[];

enum Commands 
{
	none,
	initiaza,
	ajutor,
	savarsire,
	adauga,
	statut,
	scoate,
	scb,
	rastoarna,
	compara,
	istorie,
	ramura,
	adaugaRamura,
	afiseazaRamuri,
	CommandsSize,
};

void sintaxaInvalida();

void init(const char* c, int pos);
void ajutorf(const char* c, int pos);
void savarsestef(const char* c, int pos);
void adaugaf(const char *c, int pos);
void statutf(const char *c, int pos);
void scoatef(const char *c, int pos);
void rastoarnaf(const char *c, int pos);
void comparaf(const char *c, int pos);
void istorief(const char *c, int pos);
void ramuraf(const char *c, int pos);
void adaugaRamuraf(const char *c, int pos);
void afiseazaRamurif(const char *c, int pos);