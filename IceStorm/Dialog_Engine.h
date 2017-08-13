#pragma once
#include <string>
#include <vector>
#include "Global_Flags.h"

class DialogEngine {
public:
	static void dialogSelector(string); //big switch functions name
	static int choiceMode(string a, string b, string c, string d);	//Choosing mode between multiple answers

	//dialogs
	static void firstTalk();
};
