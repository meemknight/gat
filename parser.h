#pragma once
#include <string>

#include "commands.h"


//pos will also return the new pos
bool validate(const std::string &s, int &pos);

Commands parseCommandType(const std::string &s, int &pos);

void execute(Commands c, const std::string &s, int pos);