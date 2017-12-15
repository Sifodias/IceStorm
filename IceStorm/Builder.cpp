#include "Builder.h"
#include <string>
#include "Objects_Manager.h"
#include <exception>
#include "Map.h"
#include "Camera.h"
#include "Character.h"
#include <iostream>
#include "Renderer.h"

GObject* Builder::currentObject = NULL;
vector<int> Builder::cmdDone(123, 0);

bool Builder::checkKey(int key) {
	if (key <0 || key > cmdDone.size() - 1) return false;
	else return cmdDone[key];
}
void Builder::setKey(int key) {
	if (key <0 || key > cmdDone.size() - 1) return;
	else {
		if (cmdDone[key]) cmdDone[key] = 0;
		else cmdDone[key] = 1;
	}
}
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

void Builder::createObject()
{

}

void Builder::editObject()
{

}
void Builder::newLevel(std::string name)
{

}

void Builder::loadLevel(std::string name)
{

}

void Builder::placeElement(SDL_MouseButtonEvent& e) {
	if (currentObject == NULL)
		return;

	if ((e.x + Camera::getX()) / GRID_W < 0
		|| (e.y + Camera::getY()) / GRID_H < 0) return;
	//il y a 3 rakat
	//y bon mais x trop
	/*
	if (Map::matrix.size() > (int)((e.y + Camera::getY()) / GRID_H)
		&& Map::matrix[0].size() < (int)((e.x + Camera::getX()) / GRID_W)) {

	}
	//x bon mais y trop
	if (Map::matrix.size() > (int)((e.y + Camera::getY()) / GRID_H)
		&& Map::matrix[0].size() < (int)((e.x + Camera::getX()) / GRID_W)) {

	}
	//x trop y trop
	if (Map::matrix.size() > (int)((e.y + Camera::getY()) / GRID_H)
		&& Map::matrix[0].size() < (int)((e.x + Camera::getX()) / GRID_W)) {

	}
	*/
	int i = 0;
	while (Map::matrix.size() <= (int)((e.y + Camera::getY()) / GRID_H)) {
		while (Map::matrix[i].size() <= (int)((e.x + Camera::getX()) / GRID_W))
		{
			Map::matrix[i++].push_back(0);
			if (i >= Map::matrix.size())
				break;
		}
		i = 0;
		std::vector<int> jaja(Map::matrix[0].size(), 0);
		Map::matrix.push_back(jaja);
	}
	for (int i = 0; i < Map::matrix.size(); i++) {
		if (i >= Map::matrix.size())
			break;
		while (Map::matrix[i].size() <= (int)((e.x + Camera::getX()) / GRID_W)) {
			Map::matrix[i].push_back(0);
		}
	}

	//si dans les dimensions de la mat, le placer
	if ((int)((e.x + Camera::getX()) / GRID_W) < Map::matrix[i].size()
		&& (int)((e.y + Camera::getY()) / GRID_H) < Map::matrix.size()) {
		Map::matrix[(int)((e.y + Camera::getY()) / GRID_H)]
			[(int)((e.x + Camera::getX()) / GRID_W)] = currentObject->ID;
	}
}

void Builder::routine(SDL_Event & e)
{
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_t:
			Character::movingUnit.lockMovements();
			fetch();
			break;
		case SDLK_p:
			zoom(1);
			break;
		case SDLK_o:
			zoom(-1);
			break;
		case SDLK_i:
			Character::movingUnit.noclip = !checkKey(SDLK_i);
			Camera::FREEDOM = !checkKey(SDLK_i);
			setKey(SDLK_i);
			break;
		case SDLK_0:
			currentObject = fetchObject("0");
			break;
		case SDLK_1:
			currentObject = fetchObject("1");
			break;
		}
	}

	if (e.type == SDL_MOUSEBUTTONDOWN) {
		placeElement((SDL_MouseButtonEvent&)e);
	}
}
void::Builder::zoom(int focus) {
	if (focus == -1) {
		Renderer::SCREEN_H = Camera::outerRect.h *= 2;
		Renderer::SCREEN_W = Camera::outerRect.w *= 2;
	}
	if (focus == 1) {
		Renderer::SCREEN_H = Camera::outerRect.h /= 2;
		Renderer::SCREEN_W = Camera::outerRect.w /= 2;
	}
	SDL_RenderSetLogicalSize(Renderer::g_Renderer, Camera::outerRect.w, Camera::outerRect.h);
}


/*
Key mapping :
t = console mode
o = zoom out
p = zoom in
i = freedom cam + noclip
*/