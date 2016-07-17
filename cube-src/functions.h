#include <application.h>

#define AXIS_X 1
#define AXIS_Y 2
#define AXIS_Z 3

#ifndef CUBE_ARRAY
#define CUBE_ARRAY
extern volatile unsigned char cube[8][8];
#endif

void delay_ms(uint16_t x);
void setvoxel(int x, int y, int z);
unsigned char getvoxel(int x, int y, int z);
void altervoxel(int x, int y, int z, int state);
void flpvoxel(int x, int y, int z);
void clrvoxel(int x, int y, int z);
void shift (char axis, int direction);
unsigned char inrange(int x, int y, int z);
void fill (unsigned char pattern);
void line_3d (int x1, int y1, int z1, int x2, int y2, int z2);
void setplane (char axis, unsigned char i);
void clrplane (char axis, unsigned char i);
void clrplane_z (int z);
void clrplane_x (int x);
void clrplane_y (int y);

void box_filled(int x1, int y1, int z1, int x2, int y2, int z2);
void box_walls(int x1, int y1, int z1, int x2, int y2, int z2);
void box_wireframe(int x1, int y1, int z1, int x2, int y2, int z2);
void line(int x1, int y1, int z1, int x2, int y2, int z2);
void draw_positions_axis (char axis, unsigned char positions[64], int invert);
