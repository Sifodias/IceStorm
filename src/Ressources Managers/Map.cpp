#include "Map.h"
#include <iostream>
#include "Paths.h"
#include "Engine_Manager.h"
#include "Objects_Manager.h"

std::vector<std::vector<std::vector<int>>> Map::matrix;

std::ifstream* currentLevel;
bool changed = 1;
std::string Map::levelname = "";

void Map::loadLevel(std::string name)
{
	saveMatrix();

	for (int i = 0; i < matrix.size(); i++) {
		matrix[i].clear();
	}
	if (name.empty()) {
		//name = "default.txt";
		//levelname = "default.txt";
	}
	currentLevel = loadFile(Paths::levelPath + name);

	loadMatrix();
	levelname = name;
}

void Map::loadMatrix() {
	if (currentLevel == NULL)
		return;
	int currentplan = 0;
	std::string line;
	std::getline(*currentLevel, line);
	while (line.compare("EOF")) {
		if (line[0] == '<') {
			line.erase(0, 1);
			currentplan = line[0] - '0';
			std::getline(*currentLevel, line);
			std::vector<int> jammy;
			std::vector<std::vector<int>> planny;
			while (matrix.size() <= currentplan)
				matrix.push_back(planny);

			matrix[currentplan].push_back(jammy);

			int h = 0, temp = 0;
			while (line[0] != '-') {
				//	
				if (line.empty()) {
					matrix[currentplan].push_back(jammy);
					h++;
					std::getline(*currentLevel, line);
					continue;
				}
				if (line[0] == ',') {
					matrix[currentplan][h].push_back(temp);
					temp = 0;
					line.erase(0, 1);
					continue;

				}
				temp *= 10;
				temp += line[0] - 48;
				line.erase(0, 1);
			}

		}
		std::getline(*currentLevel, line);
	}

}

//doubles for absolute x and y,
//ints for relative x and y
//double and ints will be additionned
int Map::getIdObject(double ay, int iy, double ax, int ix, int plan) {
	if ((int)(ay / GRID_H) + iy < matrix[plan].size() && (int)(ax / GRID_W) + ix < matrix[plan][(int)(ay / GRID_H) + iy].size()
		&& (int)(ay / GRID_H) + iy >= 0 && (int)(ax / GRID_W) + ix >= 0) {
		if (matrix[plan][(int)(ay / GRID_H) + iy]
			[(int)(ax / GRID_W) + ix] >= 0)

			return matrix[plan][(int)(ay / GRID_H) + iy]
			[(int)(ax / GRID_W) + ix];
	}
	return 0;
}
int Map::getID(int ix, int iy, int plan) {
	if (plan < matrix.size())
		if (iy < matrix[plan].size())
			if (ix < matrix[plan][iy].size()) {
				return matrix[plan][iy][ix];
			}
	return 0;
}

bool Map::isItSolid(SDL_Rect reqt)
{
	int x, y = 0;
	for (std::vector<std::vector<int>>& plan : matrix) {
		for (std::vector<int>& line : plan) {
			x = 0;
			for (int id : line) {
				if (!id) {
					x += GRID_W;
					continue;
				}
				GObject& obj = Objects_Manager::findObject(id);
				SDL_Rect obj_rect = { x, y, GRID_W, GRID_H };
				if (obj.imgIndex) {
					obj_rect.w = Textures_Manager::imgList[obj.imgIndex].surface->w;
					obj_rect.h = Textures_Manager::imgList[obj.imgIndex].surface->h;
				}

				if (SDL_HasIntersection(&obj_rect, &reqt) && obj.checkFlag("SOLID"))
					return true;


				x += GRID_W;
			}
			y += GRID_H;
		}
	}

	return false;
}


void Map::trigger(SDL_Rect reqt, int direction, bool contact)	//contact = 1 -> trigger only CONTACT type 
{
	/* Coat the reqt with 1 pixel large coating */
	reqt.x -= 1; reqt.y -= 1; reqt.w += 2; reqt.h += 2;

	int x, y = 0;
	for (std::vector<std::vector<int>> plan : matrix) {
		for (std::vector<int> line : plan) {
			x = 0;
			for (int id : line) {
				if (!id) {
					x += GRID_W;
					continue;
				}

				GObject& obj = Objects_Manager::findObject(id);
				SDL_Rect obj_rect = { x, y, GRID_W, GRID_H };
				if (obj.imgIndex) {
					obj_rect.w = Textures_Manager::imgList[obj.imgIndex].surface->w;
					obj_rect.h = Textures_Manager::imgList[obj.imgIndex].surface->h;
				}

				if (SDL_HasIntersection(&obj_rect, &reqt)) {
					if (contact) {
						if (obj.checkFlag("CONTACT"))
							obj.trigger();
					}
					else
						obj.trigger();
				}

				x += GRID_W;
			}
			y += GRID_H;
		}
	}
}

void Map::findOccurrence(int charry, double* ix, double* iy)
{
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[i].size(); j++) {
			for (int k = 0; k < matrix[i][j].size(); k++) {
				if (matrix[i][j][k] == charry) {
					*ix = k * GRID_W;
					*iy = j * GRID_H;
					return;
				}
			}
		}
	}
	std::cout << "Error: No character marker found\n";
	*ix = *iy = 0;
}

void Map::saveMatrix()
{
	if (currentLevel == NULL) return;
	currentLevel->close();
	std::ofstream ofs;
	ofs.open(Paths::levelPath + levelname, std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < matrix.size(); i++) {
		ofs.write("<", 1);
		ofs.write(to_string(i).c_str(), 1);
		ofs.write("\n", 1);
		for (int b = 0; b < matrix[i].size(); b++) {
			for (int a = 0; a < matrix[i][b].size(); a++) {
				ofs.write(to_string(matrix[i][b][a]).c_str(), to_string(matrix[i][b][a]).size());

				ofs.write(",", 1);
			}
			if (b != matrix[i].size() - 1)
				ofs.write("\n", 1);
		}
		ofs.write("-\n\n", 3);
	}
	ofs.write("EOF", 3);
	ofs.close();


}


void Map::checkMate(int plan)
{
	for (int h = 0; h < matrix[plan].size(); h++) {
		for (int w = 0; w < matrix[plan][h].size(); w++) {
			printf("%d ", matrix[plan][h][w]);
		}
		std::cout << std::endl;
	}
}
