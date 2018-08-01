#include "Builder.h"
#include <string>
#include "Objects_Manager.h"
#include <exception>
#include "Map.h"
#include "Camera.h"
#include "Character.h"
#include <iostream>
#include "Renderer.h"
#include "Paths.h"

GObject* Builder::currentObject = NULL;
vector<int> Builder::cmdDone(123, 0);
int Builder::currentPlan = 0;

bool Builder::checkKey(int key) {
	if (key < 0 || key > cmdDone.size() - 1) return false;
	else return cmdDone[key];
}

void Builder::setKey(int key) {
	if (key < 0 || key > cmdDone.size() - 1) return;
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
		printObject = Objects_Manager::findObjectOfID(temp);
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
		if (Objects_Manager::identify(buffer, "new ")) {		////
			if (Objects_Manager::identify(buffer, "level ")) {
				newLevel(buffer);
			}
			else if (Objects_Manager::identify(buffer, "plan ")) {
				int temp;
				try {
					temp = stoi(buffer);
					newPlan(temp);
				}
				catch (exception&) {
				}
			}
			else if (Objects_Manager::identify(buffer, "ent ")) {
				createObject(buffer);
			}
			else
				goto unknownCMD;
		}

		else if (Objects_Manager::identify(buffer, "load ")) {		////
			if (Objects_Manager::identify(buffer, "ents")) {
				loadEnts();
			}
			else if (Objects_Manager::identify(buffer, "level ")) {
				loadLevel(buffer);
			}
			else
				goto unknownCMD;

		}

		else if (Objects_Manager::identify(buffer, "current ")) {		////
			if (Objects_Manager::identify(buffer, "ent")) {
				printInfo(currentObject);
			}
			else if (Objects_Manager::identify(buffer, "plan")) {
				cout << "Current plan : " << currentPlan << endl;
			}
			else
				goto unknownCMD;

		}

		else if (Objects_Manager::identify(buffer, "set ")) {			////
			if (Objects_Manager::identify(buffer, "plan ")) {
				int temp;
				try {
					temp = stoi(buffer);
					setPlan(temp);
				}
				catch (exception&) {
				}
			}
			else if (Objects_Manager::identify(buffer, "ent ")) {
				currentObject = fetchObject(buffer);
			}
			else goto unknownCMD;
		}

		else if (Objects_Manager::identify(buffer, "info ")) {
			printInfo(fetchObject(buffer));
		}

		else if (Objects_Manager::identify(buffer, "edit ")) {
			editObject(buffer);
		}

		else {
		unknownCMD:
			std::cout << "Error: Unknown cmd" << std::endl;
		}
		getline(std::cin, buffer);
	}
}

void Builder::loadEnts() {
	Objects_Manager::Init();
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
	if (printObject->x != 0 && printObject->y != 0)
		cout << "x: " << printObject->x << ", y: " << printObject->y << endl;
	cout << "-------------" << endl;
}

void Builder::createObject(string buffer)
{
	printInfo(Objects_Manager::createObject(buffer));
}

void Builder::editObject(string str)
{
	Objects_Manager::editObject(str);
}

void Builder::newPlan(int plan) {
	while (Map::matrix.size() <= plan) {
		std::vector<std::vector<int>> bitre;
		Map::matrix.push_back(bitre);
	}
}

void Builder::setPlan(int plan) {
	if (Map::matrix.size() > plan) {
		currentPlan = plan;
	}
	else
		cout << "Error: The requested plan doesn't exist\n";

}

void Builder::newLevel(std::string name)
{
	std::ofstream level(Paths::levelPath + name);
	level << "<0\n-\n\nEOF";
	level.close();
}

void Builder::clean(int plan) {
	int toPop = 0;
	int flaggy = 0;
	for (int i = (int)Map::matrix[plan].size() - 1; i >= 0; i--) {
		for (int j = (int)Map::matrix[plan][i].size() - 1; j >= 0; j--) {
			if (Map::matrix[plan][i][j] != 0) {
				flaggy = 1;
				break;
			}
			toPop++;
		}
		Map::matrix[plan][i].erase(Map::matrix[plan][i].end() - toPop, Map::matrix[plan][i].end());
		toPop = 0;
		if (flaggy == 0) {
			if (Map::matrix[plan][i].size() == 0)
				Map::matrix[plan].erase(Map::matrix[plan].begin() + i, Map::matrix[plan].begin() + i + 1);
		}
	}

}

void Builder::loadLevel(std::string name) {
	Map::loadLevel(name);
	Map::findOccurrence(69, &Character::movingUnit.hitBox.x, &Character::movingUnit.hitBox.y);
}

void Builder::placeElement(int x, int y, int plan) {
	if (currentObject == NULL)
		return;

	if ((x + Camera::getX()) / GRID_W < 0
		|| (y + Camera::getY()) / GRID_H < 0) return;

	std::vector<int> jaja(1, 0);
	while (Map::matrix[plan].size() <= (int)((y + Camera::getY()) / GRID_H))
		Map::matrix[plan].push_back(jaja);


	while (Map::matrix[plan][(int)((y + Camera::getY()) / GRID_H)].size() <= (int)((x + Camera::getX()) / GRID_W))
		Map::matrix[plan][(int)((y + Camera::getY()) / GRID_H)].push_back(0);

	Map::matrix[plan][(int)((y + Camera::getY()) / GRID_H)]
		[(int)((x + Camera::getX()) / GRID_W)] = currentObject->ID;

}

void Builder::routine(SDL_Event & e)
{
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_t: {
			Character::movingUnit.lockMovements();
			fetch();
			break;
		}
		case SDLK_p: {
			zoom(1);
			break;
		}
		case SDLK_o: {
			zoom(-1);
			break;
		}
		case SDLK_i: {
			//Character::movingUnit.xGRAVITY_ENABLED = checkKey(SDLK_i);
			Character::movingUnit.noclip = !Character::movingUnit.noclip;
			Camera::FREEDOM = !checkKey(SDLK_i);
			setKey(SDLK_i);
			break;
		}
		case SDLK_0: {
			currentObject = fetchObject("0");
			break;
		}
		case SDLK_1: {
			currentObject = fetchObject("1");
			break;
		}
		case SDLK_f: {
			if (currentObject == NULL)
				break;
			int x = -1; int y = -1;
			SDL_GetMouseState(&x, &y);
			int width, height;
			SDL_GetWindowSize(Renderer::g_Window, &width, &height);
			x = (int)((x / (double)width)*Renderer::SCREEN_W);
			y = (int)((y / (double)height)*Renderer::SCREEN_H);
			placeElement(x, y, currentPlan);
			currentObject->movingUnit.hitBox.x = ((x + Camera::getX())/GRID_W)*GRID_W;
			currentObject->movingUnit.hitBox.y = ((y + Camera::getY())/GRID_H)*GRID_H;
			currentObject->x = ((x + Camera::getX()) / GRID_W)*GRID_W;
			currentObject->y = ((y + Camera::getY()) / GRID_H)*GRID_H;
			break;
		}
		case SDLK_r: {
			trace(0, currentPlan);
			break;
		}
		case SDLK_q: {
			trace(1, currentPlan);
			break;
		}
		case SDLK_c: {
			clean(currentPlan);
			break;
		}
		case SDLK_n: {
			Map::checkMate(currentPlan);
			break;
		}
		case SDLK_z: {
			cout << Map::isItSolid(Character::movingUnit.hitBox);
		}
		}
	}
}

void Builder::zoom(int focus) {
	if (focus == -1) {
		Renderer::SCREEN_H = Camera::outerRect.h *= 2;
		//Camera::innerRect.h *= 2;
		Renderer::SCREEN_W = Camera::outerRect.w *= 2;
		//Camera::innerRect.w *= 2;
		Character::movingUnit.xCSPEED *= 2;
		//Character::movingUnit.speedY *= 10;
	}
	if (focus == 1) {
		Renderer::SCREEN_H = Camera::outerRect.h /= 2;
		//Camera::innerRect.w /= 2;
		Renderer::SCREEN_W = Camera::outerRect.w /= 2;
		//Camera::innerRect.w /= 2;
		Character::movingUnit.xCSPEED /= 2;
		//Character::movingUnit.speedY /= 10;
	}
	SDL_RenderSetLogicalSize(Renderer::g_Renderer, Camera::outerRect.w, Camera::outerRect.h);
}

void Builder::trace(int set, int plan)
{
	int retObj = 0;
	switch (Character::movingUnit.mainDirection) {
	case 1:
		for (int i = 0; i < 100; i++) {
			if (Map::getIdObject(Character::movingUnit.hitBox.y, i, Character::movingUnit.hitBox.x, 0, plan)) {
				retObj = Map::getIdObject(Character::movingUnit.hitBox.y, i, Character::movingUnit.hitBox.x, 0, plan);
				break;
			}
		}
		break;

	case -1:
		for (int i = 0; i > -100; i--) {
			if (Map::getIdObject(Character::movingUnit.hitBox.y, i, Character::movingUnit.hitBox.x, 0, plan)) {
				retObj = Map::getIdObject(Character::movingUnit.hitBox.y, i, Character::movingUnit.hitBox.x, 0, plan);
				break;
			}
		}
		break;

	case 2:
		for (int i = 0; i < 100; i++) {
			if (Map::getIdObject(Character::movingUnit.hitBox.y, 0, Character::movingUnit.hitBox.x, i, plan)) {
				retObj = Map::getIdObject(Character::movingUnit.hitBox.y, 0, Character::movingUnit.hitBox.x, i, plan);
				break;
			}
		}
		break;

	case -2:
		for (int i = 0; i > -100; i--) {
			if (Map::getIdObject(Character::movingUnit.hitBox.y, 0, Character::movingUnit.hitBox.x, i, plan)) {
				retObj = Map::getIdObject(Character::movingUnit.hitBox.y, 0, Character::movingUnit.hitBox.x, i, plan);
				break;
			}
		}
		break;
	}
	printInfo(fetchObject(std::to_string(retObj)));
	if (set) {
		currentObject = fetchObject(std::to_string(retObj));
	}
}


/*
Key mapping :
t = console mode
o = zoom out
p = zoom in
i = freedom cam + noclip
r = trace
q = trace and set as current
n = checkmate
c = clean matrice from 0
*/