#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

typedef struct {
    int x;
    int y;
}point2d;

point2d next_dir();
point2d next_point(point2d oldpoint, point2d direction);
int on_boundary(point2d point, int width, int height);