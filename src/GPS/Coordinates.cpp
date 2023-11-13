#include "GPS/Coordinates.h"

int Coordinates::_orientation(Point a, Point b, Point c)
{
    int val = (b.lon - a.lon) * (c.lat - b.lat) - (b.lat - a.lat) * (c.lon - b.lon);

    if (val == 0)
    {
        return 0;
    }

    return (val > 0) ? 1 : 2;
}

bool Coordinates::_isOnSegment(Point a, Point c, Point b)
{
    return (c.lat <= MAX(a.lat, b.lat) && c.lat >= MIN(a.lat, b.lat)) && (c.lon <= MAX(a.lon, b.lon) && c.lon >= MIN(a.lon, b.lon));
}

bool Coordinates::intersects(Line l1, Line l2)
{
    int o1 = Coordinates::_orientation(l1.p1, l1.p2, l2.p1);
    int o2 = Coordinates::_orientation(l1.p1, l1.p2, l2.p2);
    int o3 = Coordinates::_orientation(l2.p1, l2.p2, l1.p1);
    int o4 = Coordinates::_orientation(l2.p1, l2.p2, l1.p2);

    if (o1 != o2 && o3 != o4)
    {
        return true;
    }

    if (o1 == 0 && Coordinates::_isOnSegment(l1.p1, l2.p1, l1.p2))
    {
        return true;
    }

    if (o2 == 0 && Coordinates::_isOnSegment(l1.p1, l2.p2, l1.p2))
    {
        return true;
    }

    if (o3 == 0 && Coordinates::_isOnSegment(l2.p1, l1.p1, l2.p2))
    {
        return true;
    }

    if (o4 == 0 && Coordinates::_isOnSegment(l2.p1, l1.p2, l2.p2))
    {
        return true;
    }

    return false;
}
