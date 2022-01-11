#include <math.h>
#include <float.h>

#define PI 3.14159265;

/// <summary>
/// Класс для работы с двумерным вектором
/// </summary>
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
        return sqrt(_x * _x + _y * _y);
    }

    double dotProduct(const vec2d& v) const {
        return _x * v._x + _y * v._y;
    }

    double angleBetween(const vec2d& v) {
        double dotProd = dotProduct(v);
        double mulLen = getLen() * v.getLen();
        double cosine = (mulLen != 0) ? dotProd / mulLen : DBL_MAX;
        return acos(cosine) * 180.0 / PI;
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