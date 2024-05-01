#pragma once

#include <vector>

#include "Vehicle.h"
#include "Node.h"

class RouteCreator {

private:
    unsigned char(*map)[100][70];
public:

    RouteCreator(unsigned char(*map)[100][70]) {
        this->map = map;
    }

    void create(Point& start, Point& end, vector<Point*>* out);
};

