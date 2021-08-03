#pragma once
#include <vector>
#include "Text_Printer.h"
#include <functional>

namespace Events_m {
	void routine();
	void addToQueue(std::function<void()>);

	void testTitle();
	void floweyCin();
	void etalonage();
};
