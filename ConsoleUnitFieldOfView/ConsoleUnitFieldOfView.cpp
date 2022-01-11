#include <iostream>
#include <vector>
#include <math.h>

using std::vector;

#define PI 3.14159265;

class vec2d
{
    public:
        vec2d(double x0 = 0.0, double y0 = 0.0) : _x(x0), _y(y0)
        {}

        vec2d(const vec2d& v) : _x(v._x), _y(v._y)
        {}

        bool operator==(const vec2d& v) const {
            return (_x == v._x && _y == v._y);
        }

        bool operator!=(const vec2d& v) {
            return !(*this == v);
        }

        double operator[](int i) const {
            return(i == 0 ? _x : _y);
        }

        double& operator[](int i) {
            return(i == 0 ? _x : _y);
        }

        double getLenSqr() const {
            return _x * _x + _y * _y;
        }

        double getLen() const {
            return sqrt(getLenSqr());
        }

        double dotProduct(const vec2d& v) const {
            return _x * v._x + _y * v._y;
        }

        double angleBetween(const vec2d& v) {
            double dotProd = dotProduct(v);
            double cosine = dotProd / (getLen() * v.getLen());
            return acos(cosine) * 180.0 / PI;
        }

        double get_x() {
            return _x;
        }

        double get_y() {
            return _y;
        }

        vec2d operator+() const {
            return *this;
        }

        vec2d operator+(const vec2d& v) const {
            return vec2d(_x + v._x, _y + v._y);
        }

        vec2d& operator+=(const vec2d& v) {
            _x += v._x;
            _y += v._y;
            return *this;
        }

        vec2d operator-(const vec2d& v) const {
            return *this + -v;
        }

        vec2d& operator-=(const vec2d& v) {
            _x -= v._x;
            _y -= v._y;
            return *this;
        }

        vec2d operator-() const {
            return vec2d(-_x, -_y);
        }

        vec2d operator*(const double c) const {
            return vec2d(_x * c, _y * c);
        }

        vec2d& operator*=(double c) {
            _x *= c;
            _y *= c;
            return *this;
        }

        vec2d operator/(const double c) const {
            return vec2d(_x / c, _y / c);
        }

        vec2d& operator/=(const double c) {
            *this = *this / c;
            return *this;
        }

    private:
        double _x, _y;
};

class UnitInfo {
	public:
		UnitInfo(double posX, double posY, double fovX, double fovY);
		~UnitInfo();

		vec2d position;			    // координаты юнита
		vec2d viewVector;			// кординаты вектора зрения
		
		int unitsInView = 0;		// количество видимых юнитов

		int checkUnitsInView(vector <UnitInfo> &units, double halfFovRad, double viewDistanceSqr) {			// количество юнитов
			int inView = 0;
            
			for (unsigned int i = 0; i < units.size(); i++) {
                UnitInfo cmpUnit = units.at(i);
				
                // пропускаем сравнение с самим собой
				if (!this->equel(cmpUnit)) {
                    vec2d distVec = cmpUnit.position - position;
                    if (distVec.getLenSqr() <= viewDistanceSqr) {
                        if (viewVector.angleBetween(distVec) <= halfFovRad) {
                            inView++;
                        }
                    }
				}
			}

            unitsInView = inView;
			return inView;
		}

		bool equel(const UnitInfo& cmp) const {
			if (position == cmp.position &&
				viewVector == cmp.viewVector) {
				return true;
			}
			else {
				return false;
			}
		}

		double distance(const UnitInfo& c) const {
			return 0.0;
		}

};

UnitInfo ::UnitInfo(double posX, double posY, double fovX, double fovY) {
    this->position = vec2d(posX, posY);
    this->viewVector = vec2d(fovX, fovY);
}

UnitInfo ::~UnitInfo() {
}


class TaskInfo {

	public:
		TaskInfo (double fovRad, double viewDistance);
		~TaskInfo ();

		double fovRad;				// угол зрения
		double viewDistance;			// расстояние видимости

		vector <UnitInfo> units;	// массив юнитов

		int unitsCount() {			// количество юнитов
			return units.size();
		}

        void checkUnitsView() {			// количество юнитов
            for (unsigned int i = 0; i < units.size(); i++) {
                units.at(i).checkUnitsInView(units, halfFovRad, viewDistanceSqr);
                std::cout << "Unit nuber - " << (i + 1) << " view " << units.at(i).unitsInView << " unit(s)\n";
            }
        }

//	private:
		int minX, minY;				// минимальные значения поля с юнитами
		int maxX, maxY;				// максимальные значения поля с юнитами
		double halfFovRad;			// половина угла зрения
		double viewDistanceSqr;		// расстояние видимости в квадрате

};

TaskInfo ::TaskInfo (double fovRad, double viewDistance) {
	this->fovRad = fovRad;
	halfFovRad = fovRad * 0.5;
	this->viewDistance = viewDistance;
	viewDistanceSqr = viewDistance * viewDistance;

	units.push_back(UnitInfo(1.0, 1.0, 0.0, 1.0));
	units.push_back(UnitInfo(1.0, 2.0, 1.0, 0.0));
	units.push_back(UnitInfo(-5.0, -1.0, 0.707, 1.707));

}

TaskInfo ::~TaskInfo () {
}

int main() {
	TaskInfo testTask = TaskInfo(135.5, 2.0);
	std::cout << " Test units\n";
	std::cout << "------------------------------\n";
	std::cout << " Units count - " << testTask.unitsCount() << "\n";
	testTask.checkUnitsView();
}