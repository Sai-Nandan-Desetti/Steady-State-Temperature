#include <stdio.h>
#include <stdlib.h>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

typedef struct {
    int x;
    int y;
}point2d;

const point2d East = {1, 0};
const point2d West = {-1, 0};
const point2d North = {0, 1};
const point2d South = {0, -1};

point2d next_dir();
point2d next_point(point2d oldpoint, point2d direction);
int on_boundary(point2d point, int width, int height);


int main(int argc, char *argv[]) {

    point2d current, next;
    int width = 50;
    int height = 50;

    FILE *fout = fopen("values.txt", "w");
    fprintf(fout, "x\ty\n");    

    // start walk from the centre of the grid
    current.x = width / 2;
    current.y = height / 2;
    fprintf(fout, "%d\t%d\n", current.x, current.y);   

    // iterate until the walk reaches a boundary
    while (1) {
        next = next_point(current, next_dir());
        fprintf(fout, "%d\t%d\n", next.x, next.y);
        if (on_boundary(next, width, height))
            break;
        else
            current = next;
    }
    // fprintf("%d\t%d\n", next.x, next.y);    
    return 0;
}

////////////////////////////////////////// FUNCTIONS ////////////////////////////////////////

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