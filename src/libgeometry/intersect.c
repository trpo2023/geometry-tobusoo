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
