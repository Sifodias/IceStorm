#include "Map.h"
#include <iostream>
#include "Paths.h"
#include "Engine_Manager.h"
#include "Objects_Manager.h"

std::vector<std::vector<int>> Map::matrix;
int Map::x = 0;
int Map::y = 0;
std::ifstream Map::currentLevel;

void Map::loadLevel()
{
	currentLevel = loadFile(Paths::levelPath);
	x = getX();
	y = getY();

	loadMatrix();

	checkMate();
}

void Map::loadMatrix() {
	char reader = '0';
	currentLevel.clear();
	currentLevel.seekg(0);
	std::vector<int> jammy;
	matrix.push_back(jammy);
	int h = 0, temp = 0;
	currentLevel.get(reader);
	while (reader != '-') {
		if (reader == '\n') {
			currentLevel.get(reader);
			matrix.push_back(jammy);
			h++;
			continue;
		}
		if (reader == ',') {
			matrix[h].push_back(temp);
			temp = 0;
			currentLevel.get(reader);
			continue;

		}
		temp *= 10;
		temp += reader - 48;
		currentLevel.get(reader);
	}
	matrix[h].push_back(temp);
}
int Map::getIdObject(double ay, int iy, double ax, int ix) {
	if ((ay / GRID_H) + iy < y && (ax / GRID_W) + ix < x) {
		if (matrix[(int)(ay / GRID_H) + iy]
			[(int)(ax / GRID_W) + ix] >= 0
			&& matrix[(int)(ay / GRID_H) + iy]
			[(int)(ax / GRID_W) + ix] < Objects_Manager::objects.size())
			return matrix[(int)(ay / GRID_H) + iy]
			[(int)(ax / GRID_W) + ix];
	}
	return 1;
}

bool Map::isItSolid(C_Rect reqt)
{
	//faire une grille gridh x gridw
	//tester pour chaque pt de la grille que pas solide
	for (int iy = 0; iy <= (int)(reqt.h / GRID_H); iy++) {
		for (int ix = 0; ix <= (int)(reqt.w / GRID_W); ix++) {
			if (Objects_Manager::objects[getIdObject(reqt.y, iy,
				reqt.x, ix)]->checkFlag("SOLID")) {
				return true;
			}
			if (ix + 1 > (int)(reqt.w / GRID_W)) {
				if (Objects_Manager::objects[getIdObject(reqt.y, 0,
					reqt.x + reqt.w, 0)]->checkFlag("SOLID"))
					return true;
			}
		}
		if (iy + 1 > (int)(reqt.h / GRID_H)) {
			for (int ix = 0; ix <= (int)(reqt.w / GRID_W); ix++) {
				if (Objects_Manager::objects[getIdObject(reqt.y + reqt.h, 0, reqt.x, ix)
				]->checkFlag("SOLID")) {
					return true;
				}
				if (ix + 1 > (int)(reqt.w / GRID_W)) {
					if (Objects_Manager::objects[getIdObject(reqt.y + reqt.h, 0, reqt.x + reqt.w, 0)
					]->checkFlag("SOLID"))
						return true;
				}
			}
		}
	}
	return false;
}

void Map::trigger(C_Rect reqt, int direction)
{
	switch (direction) {
	case 2:
		for (int iy = 0; iy <= (int)(reqt.h / GRID_H); iy++) {
			if (matrix[(int)(reqt.y / GRID_H) + iy]
				[(int)((reqt.x + reqt.w + 1) / GRID_W)]) {
				Objects_Manager::objects[matrix[(int)(reqt.y / GRID_H) + iy]
					[(int)((reqt.x + reqt.w + 1) / GRID_W)]]->trigger();
				return;
			}
		}
		if (reqt.h%GRID_H) {
			if (matrix[(int)((reqt.y + reqt.h) / GRID_H)]
				[(int)((reqt.x + reqt.w + 1) / GRID_W)]) {
				Objects_Manager::objects[matrix[(int)((reqt.y + reqt.h) / GRID_H)]
					[(int)((reqt.x + reqt.w + 1) / GRID_W)]]->trigger();
				return;
			}
		}
		break;

	case -2:
		for (int iy = 0; iy <= (int)(reqt.h / GRID_H); iy++) {
			if (matrix[(int)(reqt.y / GRID_H) + iy]
				[(int)((reqt.x - 1) / GRID_W)]) {
				Objects_Manager::objects[matrix[(int)(reqt.y / GRID_H) + iy]
					[(int)((reqt.x - 1) / GRID_W)]]->trigger();
				return;
			}
		}
		if (reqt.h%GRID_H) {
			if (matrix[(int)((reqt.y + reqt.h) / GRID_H)]
				[(int)((reqt.x - 1) / GRID_W)]) {
				Objects_Manager::objects[matrix[(int)((reqt.y + reqt.h) / GRID_H)]
					[(int)((reqt.x - 1) / GRID_W)]]->trigger();
				return;
			}
		}
		break;

	case 1:
		for (int ix = 0; ix <= (int)(reqt.w / GRID_W); ix++) {
			if (matrix[(int)((reqt.y + reqt.h + 1) / GRID_H)]
				[(int)((reqt.x) / GRID_W) + ix]) {
				Objects_Manager::objects[matrix[(int)((reqt.y + reqt.h + 1) / GRID_H)]
					[(int)((reqt.x) / GRID_W) + ix]]->trigger();
				return;
			}
		}
		if (reqt.h%GRID_H) {
			if (matrix[(int)((reqt.y + reqt.h + 1) / GRID_H)]
				[(int)((reqt.x + reqt.w) / GRID_W)]) {
				Objects_Manager::objects[matrix[(int)((reqt.y + reqt.h + 1) / GRID_H)]
					[(int)((reqt.x + reqt.w) / GRID_W)]]->trigger();
				return;
			}
		}
		break;

	case -1:
		for (int ix = 0; ix <= (int)(reqt.w / GRID_W); ix++) {
			if (matrix[(int)((reqt.y - 1) / GRID_H)]
				[(int)((reqt.x) / GRID_W) + ix]) {
				Objects_Manager::objects[matrix[(int)((reqt.y - 1) / GRID_H)]
					[(int)((reqt.x) / GRID_W) + ix]]->trigger();
				return;
			}
		}
		if (reqt.h%GRID_H) {
			if (matrix[(int)((reqt.y - 1) / GRID_H)]
				[(int)((reqt.x + reqt.w) / GRID_W)]) {
				Objects_Manager::objects[matrix[(int)((reqt.y - 1) / GRID_H)]
					[(int)((reqt.x + reqt.w) / GRID_W)]]->trigger();
				return;
			}
		}
		break;
	}
}

void Map::findOccurrence(int charry, double * ix, double * iy)
{
	for (int h = 0; h < y; h++) {
		for (int w = 0; w < x; w++) {
			if (matrix[h][w] == charry) {
				*ix = w*GRID_W;
				*iy = h*GRID_W;
				matrix[h][w] = 0;
				return;
			}
		}
	}
	*ix = -1;
	*iy = -1;
}

int Map::getY()
{
	currentLevel.clear();
	currentLevel.seekg(0);
	int lines = 0;
	std::string line;
	while (!currentLevel.eof()) {
		getline(currentLevel, line);
		lines++;
	}
	return lines;
}

int Map::getX()
{
	currentLevel.clear();
	currentLevel.seekg(0);
	int cols = 0;
	char reader = 0;
	while (reader != '\n')
	{
		if (reader == ',')
			cols++;
		currentLevel.get(reader);
	}
	return cols;
}

void Map::checkMate()
{
	for (int h = 0; h < y; h++) {
		for (int w = 0; w < x; w++) {
			printf("%d ", matrix[h][w]);
		}
		std::cout << std::endl;
	}
}