#pragma once
#include <vector>
#include "vec2d.h"

using std::vector;

/// <summary>
/// �������� � ��������� ������ �����
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

	vec2d position;					// ���������� �����
	vec2d viewVector;				// ��������� ������� ������

	int unitsInView = 0;			// ���������� ������� ������

	void checkUnitsInView(vector <UnitInfo>& units, double halfFovRad, double viewDistanceSqr) {			// ���������� ������
		unitsInView = 0;

		for (unsigned int i = 0; i < units.size(); i++) {
			UnitInfo cmpUnit = units.at(i);

			if (!this->equel(cmpUnit)) { // ���������� ��������� � ����� �����
				vec2d distVec = cmpUnit.position - position;					// ������ ����� ������� ������ � ������������

				if (distVec.getLenSqr() <= viewDistanceSqr) {					// � �������� ���������? (��� ���������� �����)

					if (viewVector.angleBetween(distVec) <= halfFovRad) {		// ������ � ���� ������?
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
