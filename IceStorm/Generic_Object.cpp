#include "Objects_Manager.h"
#include "Generic_Object.h"

void GObject::trigger()
{
	for (int i = 0; i < targetnames.size(); i++) {
		Objects_Manager::find(targetnames[i])->trigger();
	}
	Text_Printer::addToQueue(content);
	switch (type) {
	case BUTTON:
		break;
	case DIALOG:
		break;
	}
}
