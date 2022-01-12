#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include "UnitInfo.h"

using std::vector;

struct image
{
	image(int width, int height)
		: w(width), h(height), rgb(w * h * 3)
	{}
	uint8_t& r(int x, int y) { return rgb[(x + y * w) * 3 + 2]; }
	uint8_t& g(int x, int y) { return rgb[(x + y * w) * 3 + 1]; }
	uint8_t& b(int x, int y) { return rgb[(x + y * w) * 3 + 0]; }

	int w, h;
	std::vector<uint8_t> rgb;
};

template<class Stream>
Stream& operator<<(Stream& out, image const& img)
{
	uint32_t w = img.w, h = img.h;
	uint32_t pad = w * -3 & 3;
	uint32_t total = 54 + 3 * w * h + pad * h;
	uint32_t head[13] = { total, 0, 54, 40, w, h, (24 << 16) | 1 };
	char const* rgb = (char const*)img.rgb.data();

	out.write("BM", 2);
	out.write((char*)head, 52);
	for (uint32_t i = 0; i < h; i++)
	{
		out.write(rgb + (3 * w * i), 3 * w);
		out.write((char*)&pad, pad);
	}
	return out;
}

/// <summary>
/// Информация о юнитах и начальных данных
/// </summary>
class TaskInfo {

private:
	//int minX, minY;					// минимальные значения поля с юнитами
	//int maxX, maxY;					// максимальные значения поля с юнитами
	double halfFovRad;					// половина угла зрения
	double viewDistanceSqr;				// расстояние видимости в квадрате

	std::ofstream outFile;

	std::string trimString(std::string trim) {
		std::string res = std::regex_replace(trim, std::regex("\t|\n"), "");
		res = std::regex_replace(res, std::regex(" +"), " ");
		res = std::regex_replace(res, std::regex("^ +| +$|( ) +"), "$1");
		return res;
	}

public:
	TaskInfo() {
		fovRad = 0;
		halfFovRad = 0;
		viewDistance = 0;
		viewDistanceSqr = 0;
	}

	TaskInfo(double fovRad, double viewDistance) {
		this->fovRad = fovRad;
		halfFovRad = fovRad * 0.5;
		this->viewDistance = viewDistance;
		viewDistanceSqr = viewDistance * viewDistance;
	}

	double fovRad;					// угол зрения
	double viewDistance;			// расстояние видимости

	vector <UnitInfo> units;		// массив юнитов

	int unitsCount() {				// количество юнитов
		return units.size();
	}

	void checkUnitsView() {			// функция нахождения области видимости для каждого юнита
		outFile.open("ResultFile.out");
		
		outFile << "\nTest units\n";
		outFile << "------------------------------\n";
		outFile << "Units count - " << unitsCount() << "\n";

		std::cout << "\nTest units\n";
		std::cout << "------------------------------\n";
		std::cout << "Units count - " << unitsCount() << "\n";

		for (unsigned int i = 0; i < units.size(); i++) {
			UnitInfo unit = units.at(i);
			unit.checkUnitsInView(units, halfFovRad, viewDistanceSqr);
			std::cout << "Unit number - " << (i + 1) << " view " << unit.unitsInView << " unit(s) \tposVec - (" << unit.position[0] << ", " << unit.position[1] << ") \tviewVec - (" << unit.viewVector[0] << ", " << unit.viewVector[1] << ")\n";
			outFile << "Unit number - " << (i + 1) << " view " << unit.unitsInView << " unit(s) \tposVec - (" << unit.position[0] << ", " << unit.position[1] << ") \tviewVec - (" << unit.viewVector[0] << ", " << unit.viewVector[1] << ")\n";
		}

		outFile.close();
	}

	void drawUnitsView() {			// рисуем области видимости для каждого юнита
		std::cout << "\nPreparing image ...\n";
		
		std::cout << "Serching for max, min values ...\n";
		vec2d minDim, maxDim, diffDim;
		minDim = vec2d(LDBL_MAX, LDBL_MAX);
		maxDim = vec2d(-LDBL_MAX, -LDBL_MAX);
		for (int i = 0; i < units.size(); i++) {
			UnitInfo unit = units.at(i);
			minDim[0] = (unit.position[0] < minDim[0]) ? unit.position[0] : minDim[0];
			minDim[1] = (unit.position[1] < minDim[1]) ? unit.position[1] : minDim[1];
			maxDim[0] = (unit.position[0] > maxDim[0]) ? unit.position[0] : maxDim[0];
			maxDim[1] = (unit.position[1] > maxDim[1]) ? unit.position[1] : maxDim[1];
		}

		minDim[0] = floor(minDim[0] - viewDistance - 1);
		minDim[1] = floor(minDim[1] - viewDistance - 1);
		maxDim[0] = floor(maxDim[0] + viewDistance + 1);
		maxDim[1] = floor(maxDim[1] + viewDistance + 1);

		std::cout << "min position values : " << minDim[0] << "," << minDim[1] << "\n";
		std::cout << "max position values : " << maxDim[0] << "," << maxDim[1] << "\n";
		
		diffDim = vec2d(-minDim[0], -minDim[1]);
		image img(diffDim[0] + maxDim[0], diffDim[1] + maxDim[1]);
		
		for (int y = minDim[1]; y < maxDim[1]; y++) {
			for (int x = minDim[0]; x < maxDim[0]; x++) {
				vec2d realCoord = vec2d(x, y);
				vec2d scrCoord = realCoord + diffDim;

				img.r(scrCoord[0], scrCoord[1]) = 255;
				img.g(scrCoord[0], scrCoord[1]) = 255;
				img.b(scrCoord[0], scrCoord[1]) = 255;

				for (int i = 0; i < units.size(); i++) {
					UnitInfo unit = units.at(i);
					vec2d unitPos = unit.position;
					unitPos[0] = floor(unitPos[0]);
					unitPos[1] = floor(unitPos[1]);

					vec2d distVec = unit.position - realCoord;

					if (distVec.getLenSqr() <= viewDistanceSqr) {					// в пределах видимости? (без извлечения корня)

						img.r(scrCoord[0], scrCoord[1]) = 255;
						img.g(scrCoord[0], scrCoord[1]) = 0;
						img.b(scrCoord[0], scrCoord[1]) = 255;

						if (unit.viewVector.angleBetween(distVec) <= halfFovRad) {		// попали в поле зрения?
							img.r(scrCoord[0], scrCoord[1]) = 0;
							img.g(scrCoord[0], scrCoord[1]) = 255;
							img.b(scrCoord[0], scrCoord[1]) = 0;
						}
					}

				}

			}
		}

		for (int y = minDim[1]; y < maxDim[1]; y++) {
			for (int x = minDim[0]; x < maxDim[0]; x++) {
				vec2d realCoord = vec2d(x, y);
				vec2d scrCoord = realCoord + diffDim;

				for (int i = 0; i < units.size(); i++) {
					UnitInfo unit = units.at(i);
					vec2d unitPos = unit.position;
					unitPos[0] = floor(unitPos[0]);
					unitPos[1] = floor(unitPos[1]);

					if (realCoord == unitPos) {
						img.r(scrCoord[0], scrCoord[1]) = 0;
						img.g(scrCoord[0], scrCoord[1]) = 0;
						img.b(scrCoord[0], scrCoord[1]) = 255;
					}

				}

			}
		}

		std::ofstream("outImage.bmp") << img;
		std::cout << "Image saved.\n";
	}


	bool readFromFile(std::string path) {
		std::string line;
		std::ifstream in(path);

		if (in.is_open()) {
			std::cout << "Reading data from file ...\n";

			while (getline(in, line)) {
				line = trimString(line) + "\n";
				int lineLen = line.length();

				// удаляем комментарии
				int commentPos = line.find("//", 0);
				if (commentPos != -1) {
					line.erase(commentPos, lineLen - commentPos);
				}

				// читаем сектор просмотра
				int viewAnglePos = line.find("ViewAngle ", 0);
				if (viewAnglePos != -1) {
					line.assign(line, viewAnglePos + 10, lineLen - viewAnglePos - 10);
					fovRad = std::stod(line);
					halfFovRad = fovRad * 0.5;
					std::cout << "ViewAngle - " << fovRad << "\n";
				}

				// читаем дальность просмотра
				int viewDistancePos = line.find("ViewDistance ", 0);
				if (viewDistancePos != -1) {
					line.assign(line, viewDistancePos + 13, lineLen - viewDistancePos - 13);
					viewDistance = std::stod(line);
					viewDistanceSqr = viewDistance * viewDistance;
					std::cout << "ViewDistance - " << viewDistance << "\n";
				}

				// читаем данные юнита
				int unitPos = line.find("Unit ", 0);
				if (unitPos != -1) {
					double p0 = 0;
					double p1 = 0;
					double v0 = 0;
					double v1 = 0;

					int posVecPos = line.find("posVec(", unitPos + 5);
					if (posVecPos != -1) {
						int commaPos = line.find(",", posVecPos + 7);
						int brPos = line.find(")", posVecPos + 7);
						if (commaPos != -1 && brPos != -1) {
							std::string data = "";
							p0 = std::stod(data.assign(line, posVecPos + 7, commaPos - posVecPos - 7));
							p1 = std::stod(data.assign(line, commaPos + 1, brPos - commaPos - 1));
						}
					}

					int viewVecPos = line.find("viewVec(", unitPos + 5);
					if (viewVecPos != -1) {
						int commaPos = line.find(",", viewVecPos + 8);
						int brPos = line.find(")", viewVecPos + 8);
						if (commaPos != -1 && brPos != -1) {
							std::string data = "";
							v0 = std::stod(data.assign(line, viewVecPos + 8, commaPos - viewVecPos - 8));
							v1 = std::stod(data.assign(line, commaPos + 1, brPos - commaPos - 1));
						}
					}

					std::cout << "Unit " << " posVec - (" << p0 << "," << p1 << ") viewVec - (" << v0 << "," << v1 << ")\n";
					units.push_back(UnitInfo(p0, p1, v0, v1));
				}

			}

			std::cout << "Close data file.\n";
			in.close();
		}

		return true;
	}
};
