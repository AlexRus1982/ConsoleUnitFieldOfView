#pragma once
#include <vector>
#include "vec2d.h"

using std::vector;

/// <summary>
/// Храненее и обработки данных юнита
/// </summary>
class UnitInfo {

public:
	UnitInfo() {
		position = vec2d(0, 0);
		viewVector = vec2d(0, 0);
	}

	UnitInfo(double posX, double posY, double fovX, double fovY) {
		this->position = vec2d(posX, posY);
		this->viewVector = vec2d(fovX, fovY);
	}

	vec2d position;					// координаты юнита
	vec2d viewVector;				// кординаты вектора зрения

	int unitsInView = 0;			// количество видимых юнитов

	void checkUnitsInView(vector <UnitInfo>& units, double halfFovRad, double viewDistanceSqr) {			// количество юнитов
		unitsInView = 0;

		for (unsigned int i = 0; i < units.size(); i++) {
			UnitInfo cmpUnit = units.at(i);

			if (!this->equel(cmpUnit)) { // пропускаем сравнение с самим собой
				vec2d distVec = cmpUnit.position - position;					// вектор между текущим юнитом и сравнимаемым

				if (distVec.getLenSqr() <= viewDistanceSqr) {					// в пределах видимости? (без извлечения корня)

					if (viewVector.angleBetween(distVec) <= halfFovRad) {		// попали в поле зрения?
						unitsInView++;
					}
				}
			}
		}
	}

	bool equel(const UnitInfo& cmp) const {
		if (position == cmp.position && viewVector == cmp.viewVector) {
			return true;
		}
		else {
			return false;
		}
	}

};
