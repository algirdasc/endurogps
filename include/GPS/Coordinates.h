#pragma once

#include <stdint.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct Point
{
    int32_t lat;
    int32_t lon;
};

struct Line
{
    Point p1;
    Point p2;
};

class Coordinates
{
public:
    static int _orientation(Point a, Point b, Point c);
    static bool _isOnSegment(Point a, Point c, Point b);

    static bool intersects(Line l1, Line l2);    
};

