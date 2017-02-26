#include "Map.h"


std::vector<std::vector<int>> Map::matrix;
int Map::x = 0;
int Map::y = 0;
std::ifstream Map::currentLevel;

void Map::loadLevel()
{
	currentLevel = loadFromTxt(Paths::levelPath);
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
		if (reader == ',')
		{
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

bool Map::isItSolid(C_Rect reqt)
{
	int rx1 = 0, rx2 = 0, ry1 = 0, ry2 = 0;
	if (reqt.x != 0) {
		rx1 = (int)(reqt.x / GRID_W);
		rx2 = (int)((reqt.x + reqt.w) / GRID_W);
	}
	if (reqt.y != 0) {
		ry1 = (int)(reqt.y / GRID_H);
		ry2 = (int)((reqt.y + reqt.h) / GRID_H);
	}
	if (rx1 < 0 || ry1 < 0 || rx2 > x || ry2 > y) {
		//std::cout << "S";
		return true;
	}

	//implement list of solid objects
	if (!matrix[ry1][rx1])
		if (!matrix[ry1][rx2])
			if (!matrix[ry2][rx1])
				if (!matrix[ry2][rx2]) {
					//std::cout << "N";
					return false;
				}
	//std::cout << "S";
	return true;
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
	for (int h = 0; h < y; h++)
	{
		for (int w = 0; w < x; w++)
		{
			printf("%d ", matrix[h][w]);
		}
		std::cout << std::endl;
	}
}

Map::~Map()
{

}