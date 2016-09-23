#include "Map.h"

char** Map::matrix = NULL;
int Map::x = NULL;
int Map::y = NULL;
std::ifstream Map::currentLevel;


void Map::loadLevel()
{
	currentLevel = loadFromTxt(Paths::levelPath);
	x = getX();
	y = getY();

	matrix = new char*[y];
	for (int z = 0; z < y; ++z)
		matrix[z] = new char[x];

	loadMatrix();

	checkMate();
}

void Map::loadMatrix() {
	char reader = '0';
	currentLevel.clear();
	currentLevel.seekg(0);

	int h = 0, w = 0, temp = 0;
	currentLevel.get(reader);
	while (reader != '-') {
		if (reader == '\n') {
			currentLevel.get(reader);
			h++;
			w = 0;
			continue;
		}
		if (reader == ',')
		{
			matrix[h][w++] = temp;
			temp = 0;
			currentLevel.get(reader);
			continue;

		}
		temp *= 10;
		temp += reader - 48;
		currentLevel.get(reader);
	}
	matrix[h][w++] = temp;
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
	for (int i = 0; i < y; ++i)
		delete[] matrix[i];
}