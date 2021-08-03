#include <iostream>
#include <string>
#include <tuple>
#include <exception>
#include "Builder.h"
#include "Objects_m.h"
 
#include "Camera.h"
#include "Character.h"
#include "Renderer.h"
#include "Paths.h"

GObject* Builder::currentObject = NULL;
GObject* Builder::lastObject = NULL;
std::string Builder::door;
int Builder::currentPlan = 0;
vector<int> cmdDone(123, 0);
int Builder::idToPlace = 0;
int Builder::lastIdToPlace = 0;

using namespace std;

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

GObject& Builder::fetchObject(string name) {
	try {
		return Objects_m::findObject(stoi(name));
	}
	catch (exception&) {
		return Objects_m::findObject(name);
	}
}

void Builder::fetch() {
	std::string buffer;
	getline(std::cin, buffer);
	while (buffer.compare("z")) {
		if (Objects_m::identify(buffer, "new ")) {		////
			if (Objects_m::identify(buffer, "level ")) {
				newLevel(buffer);
			}
			else if (Objects_m::identify(buffer, "plan ")) {
				int temp;
				try {
					temp = stoi(buffer);
					newPlan(temp);
				}
				catch (exception&) {
				}
			}
			else if (Objects_m::identify(buffer, "ent ")) {
				createObject(buffer);
			}
			else
				goto unknownCMD;
		}

		else if (Objects_m::identify(buffer, "load ")) {		////
			if (Objects_m::identify(buffer, "ents")) {
				loadEnts();
			}
			else if (Objects_m::identify(buffer, "level ")) {
				loadLevel(buffer);
			}
			else
				goto unknownCMD;

		}

		else if (Objects_m::identify(buffer, "current ")) {		////
			if (Objects_m::identify(buffer, "ent")) {
				printInfo(*currentObject);
			}
			else if (Objects_m::identify(buffer, "plan")) {
				std::cout << "Current plan : " << currentPlan << endl;
			}
			else
				goto unknownCMD;

		}

		else if (Objects_m::identify(buffer, "set ")) {			////
			if (Objects_m::identify(buffer, "plan ")) {
				int temp;
				try {
					temp = stoi(buffer);
					setPlan(temp);
				}
				catch (exception&) {
				}
			}
			else if (Objects_m::identify(buffer, "ent ")) {
				lastObject = currentObject;
				currentObject = &fetchObject(buffer);
			}
			else goto unknownCMD;
		}

		else if (Objects_m::identify(buffer, "info ")) {
			printInfo(fetchObject(buffer));
		}

		else if (Objects_m::identify(buffer, "edit ")) {
			editObject(buffer);
		}

		else if (Objects_m::identify(buffer, "newdoor ")) {
			newDoor(buffer);
		}

		else {
		unknownCMD:
			std::cout << "Error: Unknown cmd" << std::endl;
		}
		getline(std::cin, buffer);
	}
}

void Builder::loadEnts() {
	Objects_m::init();
}

void Builder::printInfo(GObject& printObject) {
	cout << "-------------" << endl;
	cout << "ID: " << printObject.ID << endl;
	if (printObject.target.size())
		cout << "target: " << printObject.target << endl;
	if (printObject.targetnames.size()) {
		cout << "targetnames: ";
		for (string& name : printObject.targetnames) {
			cout << name << " ";
		}
		cout << endl;
	}
	if (printObject.flags.size()) {
		cout << "flags: ";
		for (string& flag : printObject.flags) {
			cout << flag << " ";
		}
		cout << endl;
	}
	if (printObject.type.size())
		cout << "type: " << printObject.type << endl;
	// if (printObject.textureName.size())
		// cout << "texture: " << printObject.textureName << endl;
	//if (printObject.rect.w > 0) {
	//	cout << "width: " << printObject.rect.w << endl;
	//	cout << "height: " << printObject.rect.h << endl;
	//}
	if (printObject.meta.size())
		cout << "meta: " << printObject.meta << endl;
	// if (printObject.x != 0 && printObject.y != 0)
		// cout << "x: " << printObject.x << ", y: " << printObject.y << endl;
	cout << "-------------" << endl;
}

void Builder::createObject(string buffer) {
	currentObject = &Objects_m::createObject(buffer);
	lastIdToPlace = idToPlace;
	idToPlace = currentObject->ID;
}

void Builder::editObject(string str) {
	Objects_m::editObject(str);
}

void Builder::newPlan(int plan) {
	// while (Map::matrix.size() <= plan) {
	// 	std::vector<std::vector<int>> bitre;
	// 	Map::matrix.push_back(bitre);
	// }
}

void Builder::setPlan(int plan) {
	// if (Map::matrix.size() > plan) {
	// 	currentPlan = plan;
	// }
	// else
	// 	cout << "Error: The requested plan doesn't exist\n";

}

void Builder::newLevel(std::string name) {
	std::ofstream level(Paths::levelPath + name);
	level << "<0\n-\n\nEOF";
	level.close();
}

void Builder::clean() {
	// for (std::vector<std::vector<int>>& plan : Map::matrix) {
	// 	int toPop = 0;
	// 	int flaggy = 0;
	// 	for (int i = (int)plan.size() - 1; i >= 0; i--) {
	// 		for (int j = (int)plan[i].size() - 1; j >= 0; j--) {
	// 			if (plan[i][j] != 0) {
	// 				flaggy = 1;
	// 				break;
	// 			}
	// 			toPop++;
	// 		}
	// 		plan[i].erase(plan[i].end() - toPop, plan[i].end());
	// 		toPop = 0;
	// 		if (flaggy == 0) {
	// 			if (plan[i].size() == 0)
	// 				plan.erase(plan.begin() + i, plan.begin() + i + 1);
	// 		}
	// 	}
	// }
}

void Builder::loadLevel(std::string name) {
	if (name.empty()) {
		std::cout << "ERROR: level name empty" << std::endl;
		return;
	}
	Objects_m::loadLevel(name);
	// Map::loadLevel(name);
	// Map::findOccurrence(69, &Character::movingUnit.hitBox.x, &Character::movingUnit.hitBox.y);
}

void Builder::placeElement(int x, int y, int plan = 0, bool secondary = false) {
	// if ((x + Camera::getX()) / GRID_W < 0
	// 	|| (y + Camera::getY()) / GRID_H < 0) return;

	// std::vector<int> jaja(1, 0);
	// while (Map::matrix[plan].size() <= (int)((y + Camera::getY()) / GRID_H))
	// 	Map::matrix[plan].push_back(jaja);


	// while (Map::matrix[plan][(int)((y + Camera::getY()) / GRID_H)].size() <= (int)((x + Camera::getX()) / GRID_W))
	// 	Map::matrix[plan][(int)((y + Camera::getY()) / GRID_H)].push_back(0);

	// Map::matrix[plan][(int)((y + Camera::getY()) / GRID_H)]
	// 	[(int)((x + Camera::getX()) / GRID_W)] = secondary ? lastIdToPlace : idToPlace;

}

tuple<double, double> getCoord() {
	int x = -1; int y = -1;
	SDL_GetMouseState(&x, &y);
	int width, height;
	SDL_GetWindowSize(Renderer::g_Window, &width, &height);

	double factor = (double)height / Renderer::SCREEN_H;

	double offset = (width - factor * Renderer::SCREEN_W) / 2;
	offset /= factor;

	x = (x / factor) - offset;
	y /= factor;
	return { x, y };
}

void Builder::routine(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_t: {
			//Character::movingUnit.lockMovements(true);
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
			//Character::movingUnit.gravity_affected = checkKey(SDLK_i);
			Character::movingUnit.noclip = !Character::movingUnit.noclip;
			// Camera::FREEDOM = !checkKey(SDLK_i);
			// setKey(SDLK_i);
			break;
		}
		case SDLK_0: {
			lastObject = currentObject;
			currentObject = &fetchObject("0");
			lastIdToPlace = idToPlace;
			idToPlace = currentObject->ID;
			break;
		}
		case SDLK_1: {
			lastObject = currentObject;
			currentObject = &fetchObject("1");
			lastIdToPlace = idToPlace;
			idToPlace = currentObject->ID;
			break;
		}
		case SDLK_2: {
			newDoor(door);
			break;
		}
		case SDLK_g: {
			// if (lastObject == NULL)
			// 	break;
			auto [x, y] = getCoord();

			placeElement(x, y, currentPlan, true);
			lastObject->movingUnit.hitBox.x = x + Camera::getX();
			lastObject->movingUnit.hitBox.y = y + Camera::getY();
			lastObject->movingUnit.savedCoord = lastObject->movingUnit.getCoord();
			break;
		}

		case SDLK_f: {
			// if (currentObject == NULL)
			// 	break;
			auto [x, y] = getCoord();

			placeElement(x, y, currentPlan);
			currentObject->movingUnit.hitBox.x = x + Camera::getX();
			currentObject->movingUnit.hitBox.y = y + Camera::getY();
			currentObject->movingUnit.savedCoord = currentObject->movingUnit.getCoord();
			break;
		}
		case SDLK_r: {
			currentObject = &(Objects_m::findObject(Objects_m::duplicate(*currentObject)));
			lastIdToPlace = idToPlace;
			idToPlace = currentObject->ID;
			break;
		}
		case SDLK_q: {
			trace(1, currentPlan);
			break;
		}
		case SDLK_EQUALS: {
			auto [x, y] = getCoord();
			std::cout << x + Camera::getX() << " " << y + Camera::getY() << std::endl;
			break;
		}
		case SDLK_c: {
			clean();
			break;
		}
		case SDLK_n: {
			// Map::checkMate(currentPlan);
			break;
		}
		case SDLK_x: {
			Textures_m::showInvisibleEnts = !Textures_m::showInvisibleEnts;
			break;
		}
		case SDLK_z: {
			createObject("");
			break;
		}
		case SDLK_v: {
			Textures_m::showGrid = !Textures_m::showGrid;
			break;
		}
		}
	}
	currentObject = &Objects_m::findObject(idToPlace);
}

void Builder::zoom(int focus) {
	Renderer::SCREEN_H = Camera::outerRect.h *= focus == -1 ? 2 : 0.5;
	Renderer::SCREEN_W = Camera::outerRect.w *= focus == -1 ? 2 : 0.5;

	Character::movingUnit.move_speed *= focus == -1 ? 2 : 0.5;

	SDL_RenderSetLogicalSize(Renderer::g_Renderer, Camera::outerRect.w, Camera::outerRect.h);
}

void Builder::trace(int set, int plan) {
	// int retObj = 0;
	// switch (Character::movingUnit.mainDirection) {
	// case 1:
	// for (int i = 0; i < 100; i++) {
	// 	if (Map::getIdObject(Character::movingUnit.hitBox.y, i, Character::movingUnit.hitBox.x, 0, plan)) {
	// 		retObj = Map::getIdObject(Character::movingUnit.hitBox.y, i, Character::movingUnit.hitBox.x, 0, plan);
	// 		break;
	// 	}
	// }
	// break;

	// case -1:
	// for (int i = 0; i > -100; i--) {
	// 	if (Map::getIdObject(Character::movingUnit.hitBox.y, i, Character::movingUnit.hitBox.x, 0, plan)) {
	// 		retObj = Map::getIdObject(Character::movingUnit.hitBox.y, i, Character::movingUnit.hitBox.x, 0, plan);
	// 		break;
	// 	}
	// }
	// break;

	// case 2:
	// for (int i = 0; i < 100; i++) {
	// 	if (Map::getIdObject(Character::movingUnit.hitBox.y, 0, Character::movingUnit.hitBox.x, i, plan)) {
	// 		retObj = Map::getIdObject(Character::movingUnit.hitBox.y, 0, Character::movingUnit.hitBox.x, i, plan);
	// 		break;
	// 	}
	// }
	// break;

	// case -2:
	// for (int i = 0; i > -100; i--) {
	// 	if (Map::getIdObject(Character::movingUnit.hitBox.y, 0, Character::movingUnit.hitBox.x, i, plan)) {
	// 		retObj = Map::getIdObject(Character::movingUnit.hitBox.y, 0, Character::movingUnit.hitBox.x, i, plan);
	// 		break;
	// 	}
	// }
	// break;
	// }
	// printInfo(fetchObject(std::to_string(retObj)));
	// if (set) {
	// 	currentObject = &fetchObject(std::to_string(retObj));
	// 	lastIdToPlace = idToPlace;
	// 	idToPlace = currentObject->ID;

	// }
}

void Builder::newDoor(string levelname) {
	auto [a, b] = Objects_m::newDoors(levelname);
	// printInfo(a); printInfo(b);
	// currentObject = &a;
	// lastObject = &b;
	idToPlace = a;
	lastIdToPlace = b;
	std::cout << a << " " << b << std::endl;
}
