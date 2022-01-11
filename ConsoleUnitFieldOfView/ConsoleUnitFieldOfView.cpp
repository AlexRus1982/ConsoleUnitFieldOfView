#include <Windows.h>
#include <iostream>
#include <vector>
#include "vec2d.h"

using std::vector;

/// <summary>
/// Храненее и обработки данных юнита
/// </summary>
class UnitInfo {
public:
	UnitInfo(double posX, double posY, double fovX, double fovY);

	vec2d position;					// координаты юнита
	vec2d viewVector;				// кординаты вектора зрения
		
	int unitsInView = 0;			// количество видимых юнитов

	void checkUnitsInView(vector <UnitInfo> &units, double halfFovRad, double viewDistanceSqr) {			// количество юнитов
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

UnitInfo ::UnitInfo(double posX, double posY, double fovX, double fovY) {
    this->position = vec2d(posX, posY);
    this->viewVector = vec2d(fovX, fovY);
}


/// <summary>
/// Информация о юнитах и начальных данных
/// </summary>
class TaskInfo {

public:
	TaskInfo (double fovRad, double viewDistance);

	double fovRad;					// угол зрения
	double viewDistance;			// расстояние видимости

	vector <UnitInfo> units;		// массив юнитов

	int unitsCount() {				// количество юнитов
		return units.size();
	}

    void checkUnitsView() {			// функция нахождения области видимости для каждого юнита
        for (unsigned int i = 0; i < units.size(); i++) {
            units.at(i).checkUnitsInView(units, halfFovRad, viewDistanceSqr);
            std::cout << "Unit nuber - " << (i + 1) << " view " << units.at(i).unitsInView << " unit(s)\n";
        }
    }

	void setTestData() {			// тестовые данные 3 юнита
		units.push_back(UnitInfo(1.0, 1.0, 0.0, 1.0));
		units.push_back(UnitInfo(1.0, 2.0, 1.0, 0.0));
		units.push_back(UnitInfo(-5.0, -1.0, 0.707, 0.707));
	}

	void setTestData100() {			// тестовые данные 100 юнитов
		for (int i = 0; i < 100; i++) {
			double p0 = ((double)rand() * 2 / RAND_MAX - 1.0) * 15;
			double p1 = ((double)rand() * 2 / RAND_MAX - 1.0) * 15;
			double v0 = (double)rand() * 2 / RAND_MAX - 1.0;
			double v1 = (double)rand() * 2 / RAND_MAX - 1.0;
			std::cout << "Unit nuber - " << (i + 1) << " posVec - (" << p0 << "," << p1 << ") viewVec - (" << v0 << "," << v1 << ")\n";
			units.push_back(UnitInfo(p0, p1, v0, v1));
		}
	}


private:
	//int minX, minY;					// минимальные значения поля с юнитами
	//int maxX, maxY;					// максимальные значения поля с юнитами
	double halfFovRad;				// половина угла зрения
	double viewDistanceSqr;			// расстояние видимости в квадрате

};

TaskInfo ::TaskInfo (double fovRad, double viewDistance) {
	this->fovRad = fovRad;
	halfFovRad = fovRad * 0.5;
	this->viewDistance = viewDistance;
	viewDistanceSqr = viewDistance * viewDistance;
}


float foo(float x)
{
	return sin(x);
}

void drawGraph() {
	HWND hWnd = GetConsoleWindow();
	HDC hDC = GetDC(hWnd);

	RECT rect;
	GetClientRect(hWnd, &rect);

	POINT oldPoint;
	const float d = 0.1;
	const float min = -10.0f;
	const float max = 10.0f;
	const float multV = 20.0f;
	const float multH = 10.0f;

	POINT center;
	center.x = (rect.right - rect.left) / 2;
	center.y = (rect.bottom - rect.top) / 2;

	HDC hBufferDC = CreateCompatibleDC(hDC);
	HBITMAP hBufferBmp = CreateBitmap(rect.right, rect.bottom, 1, 32, NULL);
	HBITMAP hBufferBmpOld = (HBITMAP)SelectObject(hBufferDC, hBufferBmp);
	FillRect(hBufferDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hBufferDC, hPen);

	MoveToEx(hBufferDC, center.x, 0, &oldPoint);
	LineTo(hBufferDC, center.x, rect.bottom);
	MoveToEx(hBufferDC, 0, center.y, &oldPoint);
	LineTo(hBufferDC, rect.right, center.y);

	hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	DeleteObject(SelectObject(hBufferDC, hPen));

	MoveToEx(hBufferDC, multH * min + center.x, -multV * foo(min) + center.y, &oldPoint);
	for (float x = min + d; x < max; x += d) {
		float y = foo(x);
		LineTo(hBufferDC, multH * x + center.x, -multV * y + center.y);
	}

	BitBlt(hDC, 0, 0, rect.right, rect.bottom, hBufferDC, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(hBufferDC, hOldPen));
	SelectObject(hBufferDC, hBufferBmpOld);
	DeleteObject(hBufferBmp);
	DeleteDC(hBufferDC);
	ReleaseDC(hWnd, hDC);
}

int main() {
	TaskInfo testTask = TaskInfo(135.5, 5.0);
	testTask.setTestData100();
	
	std::cout << " Test units\n";
	std::cout << "------------------------------\n";
	std::cout << " Units count - " << testTask.unitsCount() << "\n";
	testTask.checkUnitsView();

	system("pause");
	drawGraph();
}