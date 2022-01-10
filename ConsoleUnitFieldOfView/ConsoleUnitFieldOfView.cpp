#include <iostream>

class TaskInfo {

public:
	TaskInfo ();
	~TaskInfo ();

	int dimX;			// ширина 2-х мерного поля с юнитами
	int dimY;			// высота 2-х мерного поля с юнитами

};

TaskInfo ::TaskInfo () {
}

TaskInfo ::~TaskInfo () {
}

int main()
{
    std::cout << "Hello World!\n";
}