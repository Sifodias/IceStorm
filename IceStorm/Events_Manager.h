#pragma once
#include <vector>
#include "Text_Printer.h"
#include <functional>

namespace Events_Manager {
	void routine();
	void addToQueue(std::function<void()>);

	void testTitle();
};
