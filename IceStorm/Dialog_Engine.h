#pragma once
#include <string>
#include <vector>
#include "Global_Flags.h"

namespace DialogEngine {
	void dialogSelector(string); //big switch functions name
	int choiceMode(string a, string b, string c, string d);	//Choosing mode between multiple answers

	//dialogs
	void firstTalkClean();
};
