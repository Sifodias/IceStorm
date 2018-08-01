#pragma once
#include <vector>
#include "Text_Printer.h"
#include <functional>

class Events_Manager {
public:
	Events_Manager();
	static std::vector<std::function<void()>> eventsQueue;	//Each event is a method
	static bool busy;	//1 if an event is being taken care of

	static void routine();
	static void addToQueue(std::function<void()>);

};

void testTitle();