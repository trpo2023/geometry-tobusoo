#include <math.h>

#include <libgeometry/geom_parser.h>
#include <libgeometry/intersect.h>

double distance(Circle a, Circle b)
{
    double d;
    d = sqrt(pow(b.point.x - a.point.x, 2) + pow(b.point.y - a.point.y, 2));
    return d;
}

bool is_intersect_circles(Circle a, Circle b)
{
    double d = distance(a, b);
    if (d > a.radius + b.radius) {
        return false;
    }
    return true;
}

bool is_intersect_triangles(Triangle a, Triangle b)
{
    double f1 = a.p1.x * b.p2.y - a.p1.y * b.p2.x;
    double f2 = a.p2.x * b.p3.y - a.p2.y * b.p3.x;
    double f3 = a.p3.x * b.p1.y - a.p3.y * b.p1.x;

    return (f1 > 0 || f2 > 0 || f3 > 0);
}