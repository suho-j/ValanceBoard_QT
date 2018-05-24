#include "convexhull.h"

// Implementation of Andrew's monotone chain 2D convex hull algorithm.
// Asymptotic complexity: O(n log n).
// Practical performance: 0.5-1.0 seconds for n=1000000 on a 1GHz machine.
#include <algorithm>
using namespace std;

// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
coord2_t cross(const Point &O, const Point &A, const Point &B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
vector<Point> convex_hull(vector<Point> P)
{
    int n = P.size(), k = 0;
    if (n == 1) return P;
    vector<Point> H(2*n);

    // Sort points lexicographically
    sort(P.begin(), P.end());

    // Build lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }

    // Build upper hull
    for (int i = n-2, t = k+1; i >= 0; i--) {
        while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }

    H.resize(k-1);
    return H;
}

bool Point::operator <(const Point &p) const {
    return x < p.x || (x == p.x && y < p.y);
}

double hull_area(std::vector<Point> hull)
{
    double area = 0.0f;
    for (int i = 0; i < hull.size() - 1; ++i)
      area += hull[i].x * hull[i+1].y - hull[i+1].x * hull[i].y;
    area += hull[hull.size()-1].x * hull[0].y - hull[0].x * hull[hull.size()-1].y;
    area = abs(area) / 2.0f;
    return area;
}
