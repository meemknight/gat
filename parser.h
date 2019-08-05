#pragma once
#include <string>
#include <string.h>
#include <ctype.h>
#include <algorithm>

#include "commands.h"

//pos will also return the new pos
bool validate(const char *c, int &pos);

Commands parseCommandType(const char *c, int &pos);

void execute(Commands command, const char *c, int &pos);