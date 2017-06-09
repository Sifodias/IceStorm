#include "Builder.h"
#include <string>
#include "Objects_Manager.h"
#include <exception>
#include "Map.h"
#include "Camera.h"
GObject* Builder::currentObject = NULL;

GObject* fetchObject(string name) {
	GObject* printObject;
	int temp;
	try {
		temp = stoi(name);
		if (temp > Objects_Manager::objects.size()) return NULL;
		printObject = Objects_Manager::objects[temp];
	}
	catch (exception&) {
		printObject = Objects_Manager::findObject(name);
	}
	return printObject;
}

void Builder::fetch()
{
	std::string buffer;
	getline(std::cin, buffer);
	while (buffer.compare("z")) {
		if (Objects_Manager::identify(buffer, "set ")) {
			currentObject = fetchObject(buffer);
		}
		if (Objects_Manager::identify(buffer, "info ")) {
			printInfo(fetchObject(buffer));
		}
		if (Objects_Manager::identify(buffer, "current")) {
			printInfo(currentObject);
		}
		getline(std::cin, buffer);
	}
}

void Builder::printInfo(GObject* printObject)
{
	if (printObject == NULL) return;
	cout << "-------------" << endl;
	cout << "ID: " << printObject->ID << endl;
	if (printObject->target.size())
		cout << "target: " << printObject->target << endl;
	if (printObject->targetnames.size()) {
		cout << "targetnames: ";
		for (auto i = printObject->targetnames.begin();
			i != printObject->targetnames.end(); ++i) {
			cout << *i << " ";
		}
		cout << endl;
	}
	if (printObject->flags.size()) {
		cout << "flags: ";
		for (auto i = printObject->flags.begin();
			i != printObject->flags.end(); ++i) {
			cout << *i << " ";
		}
		cout << endl;
	}
	if (printObject->type.size())
		cout << "type: " << printObject->type << endl;
	if (printObject->textureName.size())
		cout << "texture: " << printObject->textureName << endl;
	if (printObject->content.size())
		cout << "content: " << printObject->content << endl;
	cout << "-------------" << endl;
}

void Builder::placeElement(SDL_MouseButtonEvent& e) {
	if (currentObject == NULL)
		return;
	//si dans les dimensions de la mat, le placer
	if ((e.x + Camera::getX()) / GRID_W < 0
		|| (e.y + Camera::getY()) / GRID_H < 0) return;
	if ((int)((e.x + Camera::getX()) / GRID_W) < Map::x
		&& (int)((e.y + Camera::getY()) / GRID_H) < Map::y) {
		Map::matrix[(int)((e.y + Camera::getY()) / GRID_H)]
			[(int)((e.x + Camera::getX()) / GRID_W)] = currentObject->ID;
	}
	//sinon, resizer la mat
}

void Builder::routine(SDL_Event & e)
{
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_t:
			fetch();
			break;
		}
	}
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		placeElement((SDL_MouseButtonEvent&)e);
	}
}
