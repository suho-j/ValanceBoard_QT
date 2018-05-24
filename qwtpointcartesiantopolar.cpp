#include "qwtpointcartesiantopolar.h"
#include <cmath>

QwtPointCartesianToPolar::QwtPointCartesianToPolar(double x, double y)
{
    double r = sqrt(x * x + y * y);
    double theta = std::atan2(y, x);
    this->rAzimuth() = theta * 180 / 3.141592653589793238462643383279502884197469399375105820974944;
    this->rRadius() = r;
}
