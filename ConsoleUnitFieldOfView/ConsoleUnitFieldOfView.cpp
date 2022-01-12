#include "TaskInfo.h"
#include <time.h>

#pragma region Tests
void setTestData(TaskInfo &task) {				// тестовые данные 3 юнита
	task.units.push_back(UnitInfo(1.0, 1.0, 0.0, 1.0));
	task.units.push_back(UnitInfo(1.0, 2.0, 1.0, 0.0));
	task.units.push_back(UnitInfo(-5.0, -1.0, 0.707, 0.707));
}

void setTestData100(TaskInfo& task) {			// тестовые данные 100 юнитов
	for (int i = 0; i < 100; i++) {
		double p0 = ((double)rand() * 2 / RAND_MAX - 1.0) * 100;
		double p1 = ((double)rand() * 2 / RAND_MAX - 1.0) * 100;
		double v0 = (double)rand() * 2 / RAND_MAX - 1.0;
		double v1 = (double)rand() * 2 / RAND_MAX - 1.0;
		//std::cout << "Unit nuber - " << (i + 1) << " posVec - (" << p0 << "," << p1 << ") viewVec - (" << v0 << "," << v1 << ")\n";
		task.units.push_back(UnitInfo(p0, p1, v0, v1));
	}
}
#pragma endregion Tests

int main() {
	TaskInfo testTask;
	testTask.readFromFile("DataFile.in");

	//TaskInfo testTask = TaskInfo(135.5, 10.0);
	//setTestData100(testTask);

	//clock_t start = clock();
	testTask.checkUnitsView();
	//clock_t end = clock();
	//printf("The above code block was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC));
	
	testTask.drawUnitsView(); // выводит картинку в файл outImage.bmp

	system("pause");
}