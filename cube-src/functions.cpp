#include "functions.h"

volatile unsigned char cube[8][8];

//***********************************************************************************************************************
// Delay loop.
// This is not calibrated to milliseconds,
// but we had already made to many effects using this
// calibration when we figured it might be a good idea
// to calibrate it.
void delay_ms(uint16_t x)
{
    Particle.process();

    x = x * 14;
    uint8_t y, z;
    for ( ; x > 0 ; x--) {
        for ( y = 0 ; y < 90 ; y++) {
            for ( z = 0 ; z < 6 ; z++) {
                asm volatile ("nop");
            }
        }
    }
}

// Set a single voxel to ON
void setvoxel(int x, int y, int z)
{
    if (inrange(x,y,z))
        cube[z][y] |= (1 << x);
}

// Set a single voxel to ON
void clrvoxel(int x, int y, int z)
{
    if (inrange(x,y,z))
        cube[z][y] &= ~(1 << x);
}

// This function validates that we are drawing inside the cube.
unsigned char inrange(int x, int y, int z)
{
    if (x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8)
    {
        return 0x01;
    } else
    {
        // One of the coordinates was outside the cube.
        return 0x00;
    }
}

// Get the current status of a voxel
unsigned char getvoxel(int x, int y, int z)
{
    if (inrange(x,y,z))
    {
        if (cube[z][y] & (1 << x))
        {
            return 0x01;
        } else
        {
            return 0x00;
        }
    } else
    {
        return 0x00;
    }
}

// In some effect we want to just take bool and write it to a voxel
// this function calls the apropriate voxel manipulation function.
void altervoxel(int x, int y, int z, int state)
{
    if (state == 1)
    {
        setvoxel(x,y,z);
    } else
    {
        clrvoxel(x,y,z);
    }
}

// Flip the state of a voxel.
// If the voxel is 1, its turned into a 0, and vice versa.
void flpvoxel(int x, int y, int z)
{
    if (inrange(x, y, z))
        cube[z][y] ^= (1 << x);
}

// Shift the entire contents of the cube along an axis
// This is great for effects where you want to draw something
// on one side of the cube and have it flow towards the other
// side. Like rain flowing down the Z axiz.
void shift (char axis, int direction)
{
    int i, x, y;
    int ii, iii;
    int state;

    for (i = 0; i < 8; i++)
    {
        if (direction == -1)
        {
            ii = i;
        } else
        {
            ii = (7-i);
        }


        for (x = 0; x < 8; x++)
        {
            for (y = 0; y < 8; y++)
            {
                if (direction == -1)
                {
                    iii = ii+1;
                } else
                {
                    iii = ii-1;
                }

                if (axis == AXIS_Z)
                {
                    state = getvoxel(x,y,iii);
                    altervoxel(x,y,ii,state);
                }

                if (axis == AXIS_Y)
                {
                    state = getvoxel(x,iii,y);
                    altervoxel(x,ii,y,state);
                }

                if (axis == AXIS_X)
                {
                    state = getvoxel(iii,y,x);
                    altervoxel(ii,y,x,state);
                }
            }
        }
    }

    if (direction == -1)
    {
        i = 7;
    } else
    {
        i = 0;
    }

    for (x = 0; x < 8; x++)
    {
        for (y = 0; y < 8; y++)
        {
            if (axis == AXIS_Z)
                clrvoxel(x,y,i);

            if (axis == AXIS_Y)
                clrvoxel(x,i,y);

            if (axis == AXIS_X)
                clrvoxel(i,y,x);
        }
    }
}

// Fill a value into all 64 byts of the cube buffer
// Mostly used for clearing. fill(0x00)
// or setting all on. fill(0xff)
void fill (unsigned char pattern)
{
    int z;
    int y;
    for (z=0;z<8;z++)
    {
        for (y=0;y<8;y++)
        {
            cube[z][y] = pattern;
        }
    }
}

void line_3d (int x1, int y1, int z1, int x2, int y2, int z2)
{
    int i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc,
    err_1, err_2, dx2, dy2, dz2;
    int pixel[3];
    pixel[0] = x1;
    pixel[1] = y1;
    pixel[2] = z1;
    dx = x2 - x1;
    dy = y2 - y1;
    dz = z2 - z1;
    x_inc = (dx < 0) ? -1 : 1;
    l = abs(dx);
    y_inc = (dy < 0) ? -1 : 1;
    m = abs(dy);
    z_inc = (dz < 0) ? -1 : 1;
    n = abs(dz);
    dx2 = l << 1;
    dy2 = m << 1;
    dz2 = n << 1;
    if ((l >= m) && (l >= n)) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (i = 0; i < l; i++) {
            //PUT_PIXEL(pixel);
            setvoxel(pixel[0],pixel[1],pixel[2]);
            //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
            if (err_1 > 0) {
                pixel[1] += y_inc;
                err_1 -= dx2;
            }
            if (err_2 > 0) {
                pixel[2] += z_inc;
                err_2 -= dx2;
            }
            err_1 += dy2;
            err_2 += dz2;
            pixel[0] += x_inc;
        }
    } else if ((m >= l) && (m >= n)) {
        err_1 = dx2 - m;
        err_2 = dz2 - m;
        for (i = 0; i < m; i++) {
            //PUT_PIXEL(pixel);
            setvoxel(pixel[0],pixel[1],pixel[2]);
            //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
            if (err_1 > 0) {
                pixel[0] += x_inc;
                err_1 -= dy2;
            }
            if (err_2 > 0) {
                pixel[2] += z_inc;
                err_2 -= dy2;
            }
            err_1 += dx2;
            err_2 += dz2;
            pixel[1] += y_inc;
        }
    } else {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        for (i = 0; i < n; i++) {
            setvoxel(pixel[0],pixel[1],pixel[2]);
            //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
            //PUT_PIXEL(pixel);
            if (err_1 > 0) {
                pixel[1] += y_inc;
                err_1 -= dz2;
            }
            if (err_2 > 0) {
                pixel[0] += x_inc;
                err_2 -= dz2;
            }
            err_1 += dy2;
            err_2 += dx2;
            pixel[2] += z_inc;
        }
    }
    setvoxel(pixel[0],pixel[1],pixel[2]);
    //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
    //PUT_PIXEL(pixel);
}

// Sets all voxels along a X/Y plane at a given point on axis Z
void setplane_z (int z)
{
    int i;
    if (z>=0 && z<8)
    {
        for (i=0;i<8;i++)
            cube[z][i] = 0xff;
    }
}

void clrplane_z (int z)
{
    int i;
    if (z>=0 && z<8)
    {
        for (i=0;i<8;i++)
            cube[z][i] = 0x00;
    }
}

void setplane_x (int x)
{
    int z;
    int y;
    if (x>=0 && x<8)
    {
        for (z=0;z<8;z++)
        {
            for (y=0;y<8;y++)
            {
                cube[z][y] |= (1 << x);
            }
        }
    }
}

void clrplane_x (int x)
{
    int z;
    int y;
    if (x>=0 && x<8)
    {
        for (z=0;z<8;z++)
        {
            for (y=0;y<8;y++)
            {
                cube[z][y] &= ~(1 << x);
            }
        }
    }
}

void setplane_y (int y)
{
    int z;
    if (y>=0 && y<8)
    {
        for (z=0;z<8;z++)
            cube[z][y] = 0xff;
    }
}

void clrplane_y (int y)
{
    int z;
    if (y>=0 && y<8)
    {
        for (z=0;z<8;z++)
            cube[z][y] = 0x00;
    }
}

void setplane (char axis, unsigned char i)
{
    switch (axis)
    {
    case AXIS_X:
        setplane_x(i);
        break;

    case AXIS_Y:
        setplane_y(i);
        break;

    case AXIS_Z:
        setplane_z(i);
        break;
    }
}

void clrplane (char axis, unsigned char i)
{
    switch (axis)
    {
    case AXIS_X:
        clrplane_x(i);
        break;

    case AXIS_Y:
        clrplane_y(i);
        break;

    case AXIS_Z:
        clrplane_z(i);
        break;
    }
}

// ======================================================================================================================
//   Draw functions
// ======================================================================================================================

// Makes sure x1 is alwas smaller than x2
// This is usefull for functions that uses for loops,
// to avoid infinite loops
void argorder(int ix1, int ix2, int *ox1, int *ox2)
{
    if (ix1>ix2)
    {
        int tmp;
        tmp = ix1;
        ix1= ix2;
        ix2 = tmp;
    }
    *ox1 = ix1;
    *ox2 = ix2;
}

// Returns a byte with a row of 1's drawn in it.
// byteline(2,5) gives 0b00111100
char byteline (int start, int end)
{
    return ((0xff<<start) & ~(0xff<<(end+1)));
}

//***********************************************************************************************************************
// Draw a box with all walls drawn and all voxels inside set
void box_filled(int x1, int y1, int z1, int x2, int y2, int z2)
{
    int iy;
    int iz;

    argorder(x1, x2, &x1, &x2);
    argorder(y1, y2, &y1, &y2);
    argorder(z1, z2, &z1, &z2);

    for (iz=z1;iz<=z2;iz++)
    {
        for (iy=y1;iy<=y2;iy++)
        {
            cube[iz][iy] |= byteline(x1,x2);
        }
    }

}



//***********************************************************************************************************************
// Draw a hollow box with side walls.
void box_walls(int x1, int y1, int z1, int x2, int y2, int z2)
{
    int iy;
    int iz;

    argorder(x1, x2, &x1, &x2);
    argorder(y1, y2, &y1, &y2);
    argorder(z1, z2, &z1, &z2);

    for (iz=z1;iz<=z2;iz++)
    {
        for (iy=y1;iy<=y2;iy++)
        {
            if (iy == y1 || iy == y2 || iz == z1 || iz == z2)
            {
                cube[iz][iy] = byteline(x1,x2);
            } else
            {
                cube[iz][iy] |= ((0x01 << x1) | (0x01 << x2));
            }
        }
    }

}



//***********************************************************************************************************************
// Draw a wireframe box. This only draws the corners and edges,
// no walls.
void box_wireframe(int x1, int y1, int z1, int x2, int y2, int z2)
{
    int iy;
    int iz;

    argorder(x1, x2, &x1, &x2);
    argorder(y1, y2, &y1, &y2);
    argorder(z1, z2, &z1, &z2);

    // Lines along X axis
    cube[z1][y1] = byteline(x1,x2);
    cube[z1][y2] = byteline(x1,x2);
    cube[z2][y1] = byteline(x1,x2);
    cube[z2][y2] = byteline(x1,x2);

    // Lines along Y axis
    for (iy=y1;iy<=y2;iy++)
    {
        setvoxel(x1,iy,z1);
        setvoxel(x1,iy,z2);
        setvoxel(x2,iy,z1);
        setvoxel(x2,iy,z2);
    }

    // Lines along Z axis
    for (iz=z1;iz<=z2;iz++)
    {
        setvoxel(x1,y1,iz);
        setvoxel(x1,y2,iz);
        setvoxel(x2,y1,iz);
        setvoxel(x2,y2,iz);
    }

}

// Draw a line between any coordinates in 3d space.
// Uses integer values for input, so dont expect smooth animations.
void line(int x1, int y1, int z1, int x2, int y2, int z2)
{
    float xy;	// how many voxels do we move on the y axis for each step on the x axis
    float xz;	// how many voxels do we move on the y axis for each step on the x axis
    unsigned char x,y,z;
    // unsigned char lasty,lastz;

    // We always want to draw the line from x=0 to x=7.
    // If x1 is bigget than x2, we need to flip all the values.
    if (x1>x2)
    {
        int tmp;
        tmp = x2;        x2 = x1;        x1 = tmp;
        tmp = y2;        y2 = y1;        y1 = tmp;
        tmp = z2;        z2 = z1;        z1 = tmp;
    }


    if (y1>y2)
    {
        xy = (float)(y1-y2)/(float)(x2-x1);
        // lasty = y2;
    } else
    {
        xy = (float)(y2-y1)/(float)(x2-x1);
        // lasty = y1;
    }

    if (z1>z2)
    {
        xz = (float)(z1-z2)/(float)(x2-x1);
        // lastz = z2;
    } else
    {
        xz = (float)(z2-z1)/(float)(x2-x1);
        // lastz = z1;
    }
    // For each step of x, y increments by:
    for (x = x1; x<=x2;x++)
    {
        y = (xy*(x-x1))+y1;
        z = (xz*(x-x1))+z1;
        setvoxel(x,y,z);
    }

}

void draw_positions_axis (char axis, unsigned char positions[64], int invert)
{
    int x, y, p;

    fill(0x00);

    for (x=0; x<8; x++)
    {
        for (y=0; y<8; y++)
        {
            if (invert)
            {
                p = (7-positions[(x*8)+y]);
            } else
            {
                p = positions[(x*8)+y];
            }

            if (axis == AXIS_Z)
                setvoxel(x,y,p);

            if (axis == AXIS_Y)
                setvoxel(x,p,y);

            if (axis == AXIS_X)
                setvoxel(p,y,x);
        }
    }

}
