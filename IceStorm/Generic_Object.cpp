#include "Objects_Manager.h"
#include "Generic_Object.h"
#include "Text_Printer.h"
#include "Dialog_Engine.h"
void GObject::trigger()
{
	flagTrigger = 1;
	for (int i = 0; i < targetnames.size(); i++) {
		if (!Objects_Manager::findObject(targetnames[i])->flagTrigger)
			Objects_Manager::findObject(targetnames[i])->trigger();
	}
	if (!type.compare("BUTTON")) {
	}

	if (!type.compare("DIALOG")) {
		if (!Text_Printer::busy) {
			if (content[0] == '|') {
				DialogEngine::dialogSelector(content);
			}
			else
				Text_Printer::addToQueue(content);
		}
	}
	flagTrigger = 0;
}

bool GObject::checkFlag(std::string flag)
{
	for (auto i = flags.begin(); i != flags.end(); ++i) {
		if (!i->compare(flag)) return true;
	}
	return false;
}
