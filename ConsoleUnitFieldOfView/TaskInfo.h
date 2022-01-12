#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include "UnitInfo.h"

using std::vector;

/// <summary>
/// Информация о юнитах и начальных данных
/// </summary>
class TaskInfo {

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
			units.at(i).checkUnitsInView(units, halfFovRad, viewDistanceSqr);
			std::cout << "Unit number - " << (i + 1) << " view " << units.at(i).unitsInView << " unit(s)\n";
			outFile << "Unit number - " << (i + 1) << " view " << units.at(i).unitsInView << " unit(s)\n";
		}

		outFile.close();
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

};
