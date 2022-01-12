#include "TaskInfo.h"

#pragma region Tests
void setTestData(TaskInfo &task) {				// тестовые данные 3 юнита
	task.units.push_back(UnitInfo(1.0, 1.0, 0.0, 1.0));
	task.units.push_back(UnitInfo(1.0, 2.0, 1.0, 0.0));
	task.units.push_back(UnitInfo(-5.0, -1.0, 0.707, 0.707));
}

void setTestData100(TaskInfo& task) {			// тестовые данные 100 юнитов
	for (int i = 0; i < 100; i++) {
		double p0 = ((double)rand() * 2 / RAND_MAX - 1.0) * 15;
		double p1 = ((double)rand() * 2 / RAND_MAX - 1.0) * 15;
		double v0 = (double)rand() * 2 / RAND_MAX - 1.0;
		double v1 = (double)rand() * 2 / RAND_MAX - 1.0;
		std::cout << "Unit nuber - " << (i + 1) << " posVec - (" << p0 << "," << p1 << ") viewVec - (" << v0 << "," << v1 << ")\n";
		task.units.push_back(UnitInfo(p0, p1, v0, v1));
	}
}
#pragma endregion Tests

int main() {
	TaskInfo testTask;
	testTask.readFromFile("DataFile.in");

	//TaskInfo testTask = TaskInfo(135.5, 5.0);
	//setTestData100(testTask);

	testTask.checkUnitsView();

	system("pause");
}