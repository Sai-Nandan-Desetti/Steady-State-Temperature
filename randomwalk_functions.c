#include <stdlib.h>
#include "randomwalk.h"

const point2d East = {1, 0};
const point2d West = {-1, 0};
const point2d North = {0, 1};
const point2d South = {0, -1};

// generate a random number between 0 and 1
double uniform_random() {

    return (double)rand() / (double)RAND_MAX;
}

// randomly generate a new direction to walk
point2d next_dir() {

    double u = uniform_random();
    if      (u < 0.25) return North;
    else if (u < 0.50) return East;
    else if (u < 0.75) return South;
    else               return West;
}

// generate next point from current point and direction
point2d next_point(point2d oldpoint, point2d direction) {

    point2d temp;
    temp.x = oldpoint.x + direction.x;
    temp.y = oldpoint.y + direction.y;

    return temp;
}

// test if given point is on a boundary.
// returns non-zero if on a boundary, 0 if not.
int on_boundary(point2d point, int width, int height) {

    if      (0 == point.x)        return WEST;
    else if (width-1 == point.x)  return EAST;
    else if (0 == point.y)        return NORTH;
    else if (height-1 == point.y) return SOUTH;
    else                          return 0;
}