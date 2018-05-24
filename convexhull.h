#ifndef CONVEXHULL_H
#define CONVEXHULL_H
#include <vector>

typedef double coord_t;         // coordinate type
typedef double coord2_t;  // must be big enough to hold 2*max(|coordinate|)^2

struct Point {
    coord_t x, y;

    bool operator <(const Point &p) const;
};

std::vector<Point> convex_hull(std::vector<Point> P);
double hull_area(std::vector<Point> hull);
#endif // CONVEXHULL_H
