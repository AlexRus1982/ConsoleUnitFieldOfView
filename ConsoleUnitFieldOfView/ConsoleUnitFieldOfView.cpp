#include <iostream>
#include <vector>

using std::vector;

class UnitInfo {
	public:
		UnitInfo(float posX, float posY, float fovX, float fovY);
		~UnitInfo();

		float posX, posY;			// координаты юнита
		float fovX, fovY;			// кординаты вектора зрения

};

UnitInfo ::UnitInfo(float posX, float posY, float fovX, float fovY) {
	this->posX = posX;
	this->posY = posY;
	this->fovX = fovX;
	this->fovY = fovY;
}

UnitInfo ::~UnitInfo() {
}


class TaskInfo {

	public:
		TaskInfo (float fovRad, float viewDistance);
		~TaskInfo ();

		int unitCount;				// количество юнитов
		float fovRad;				// угол зрения
		float viewDistance;			// расстояние видимости

		vector <UnitInfo> units;	// массив юнитов
	
	private:
		int minX, minY;				// минимальные значения поля с юнитами
		int maxX, maxY;				// максимальные значения поля с юнитами
		float halfFovRad;			// половина угла зрения
		float viewDistance2;		// расстояние видимости в квадрате

};

TaskInfo ::TaskInfo (float fovRad, float viewDistance) {
	this->fovRad = fovRad;
	halfFovRad = fovRad * 0.5f;
	this->viewDistance = viewDistance;
	viewDistance2 = viewDistance * viewDistance;

	units.push_back(UnitInfo(1.0f, 1.0f, 0.0f, 1.0f));
	units.push_back(UnitInfo(1.0f, 2.0f, 1.0f, 0.0f));
	units.push_back(UnitInfo(-5.0f, -1.0f, 0.707f, 1.707f));

}

TaskInfo ::~TaskInfo () {
}

int main() {
	TaskInfo testTask = TaskInfo(135.5f, 2.0f);
    std::cout << "Hello World!\n" << testTask.units.size();
}