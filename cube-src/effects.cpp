#include "effects.h"

void effect_rain (int iterations)
{
    int i, ii;
    int rnd_x;
    int rnd_y;
    int rnd_num;

    for (ii=0;ii<iterations;ii++)
    {
        rnd_num = rand()%4;

        for (i=0; i < rnd_num;i++)
        {
            rnd_x = rand()%8;
            rnd_y = rand()%8;
            setvoxel(rnd_x,rnd_y,7);
        }

        delay_ms(1000);
        shift(AXIS_Z,-1);
    }
}

void fireworks (int iterations, int n, int delay)
{
    fill(0x00);

    int i,f,e;

    float origin_x = 3;
    float origin_y = 3;
    float origin_z = 3;

    int rand_y, rand_x, rand_z;

    float slowrate, gravity;

    // Particles and their position, x,y,z and their movement, dx, dy, dz
    float particles[n][6];

    for (i=0; i<iterations; i++)
    {
        origin_x = rand()%4;
        origin_y = rand()%4;
        origin_z = rand()%2;
        origin_z +=5;
        origin_x +=2;
        origin_y +=2;

        // shoot a particle up in the air
        for (e=0;e<origin_z;e++)
        {
            setvoxel(origin_x,origin_y,e);
            delay_ms(600+500*e);
            fill(0x00);
        }

        // Fill particle array
        for (f=0; f<n; f++)
        {
            // Position
            particles[f][0] = origin_x;
            particles[f][1] = origin_y;
            particles[f][2] = origin_z;

            rand_x = rand()%200;
            rand_y = rand()%200;
            rand_z = rand()%200;

            // Movement
            particles[f][3] = 1-(float)rand_x/100; // dx
            particles[f][4] = 1-(float)rand_y/100; // dy
            particles[f][5] = 1-(float)rand_z/100; // dz
        }

        // explode
        for (e=0; e<25; e++)
        {
            slowrate = 1+tan((e+0.1)/20)*10;

            gravity = tan((e+0.1)/20)/2;

            for (f=0; f<n; f++)
            {
                particles[f][0] += particles[f][3]/slowrate;
                particles[f][1] += particles[f][4]/slowrate;
                particles[f][2] += particles[f][5]/slowrate;
                particles[f][2] -= gravity;

                setvoxel(particles[f][0],particles[f][1],particles[f][2]);
            }

            delay_ms(delay);
            fill(0x00);
        }
    }
}

void effect_random_sparkle_flash (int iterations, int voxels, int delay)
{
    int i;
    int v;
    for (i = 0; i < iterations; i++)
    {
        for (v=0;v<=voxels;v++)
            setvoxel(rand()%8,rand()%8,rand()%8);

        delay_ms(delay);
        fill(0x00);
    }
}

// blink 1 random voxel, blink 2 random voxels..... blink 20 random voxels
// and back to 1 again.
void effect_random_sparkle (void)
{
    int i;

    for (i=1;i<20;i++)
    {
        effect_random_sparkle_flash(5,i,200);
    }

    for (i=20;i>=1;i--)
    {
        effect_random_sparkle_flash(5,i,200);
    }

}

void effect_wormsqueeze (int size, int axis, int direction, int iterations, int delay)
{
    int x, y, i,j,k, dx, dy;
    int cube_size;
    int origin = 0;

    if (direction == -1)
        origin = 7;

    cube_size = 8-(size-1);

    x = rand()%cube_size;
    y = rand()%cube_size;

    for (i=0; i<iterations; i++)
    {
        dx = ((rand()%3)-1);
        dy = ((rand()%3)-1);

        if ((x+dx) > 0 && (x+dx) < cube_size)
            x += dx;

        if ((y+dy) > 0 && (y+dy) < cube_size)
            y += dy;

        shift(axis, direction);


        for (j=0; j<size;j++)
        {
            for (k=0; k<size;k++)
            {
                if (axis == AXIS_Z)
                    setvoxel(x+j,y+k,origin);

                if (axis == AXIS_Y)
                    setvoxel(x+j,origin,y+k);

                if (axis == AXIS_X)
                    setvoxel(origin,y+j,x+k);
            }
        }

        delay_ms(delay);
    }
}

void sinelines (int iterations, int delay)
{
    int i,x;

    float left, right, sine_base, x_dividor,ripple_height;

    for (i=0; i<iterations; i++)
    {
        for (x=0; x<8 ;x++)
        {
            x_dividor = 2 + sin((float)i/100)+1;
            ripple_height = 3 + (sin((float)i/200)+1)*6;

            sine_base = (float) i/40 + (float) x/x_dividor;

            left = 4 + sin(sine_base)*ripple_height;
            right = 4 + cos(sine_base)*ripple_height;
            right = 7-left;

            //printf("%i %i \n", (int) left, (int) right);

            line_3d(0-3, x, (int) left, 7+3, x, (int) right);
            //line_3d((int) right, 7, x);
        }

        delay_ms(delay);
        fill(0x00);
    }
}

// Draw a plane on one axis and send it back and forth once.
void effect_planboing (int plane, int speed)
{
    int i;
    for (i=0;i<8;i++)
    {
        fill(0x00);
        setplane(plane, i);
        delay_ms(speed);
    }

    for (i=7;i>=0;i--)
    {
        fill(0x00);
        setplane(plane,i);
        delay_ms(speed);
    }
}

// Set or clear exactly 512 voxels in a random order.
void effect_random_filler (int delay, int state)
{
    int x,y,z;
    int loop = 0;


    if (state == 1)
    {
        fill(0x00);
    } else
    {
        fill(0xff);
    }

    while (loop<511)
    {
        x = rand()%8;
        y = rand()%8;
        z = rand()%8;

        if ((state == 0 && getvoxel(x,y,z) == 0x01) || (state == 1 && getvoxel(x,y,z) == 0x00))
        {
            altervoxel(x,y,z,state);
            delay_ms(delay);
            loop++;
        }
    }
}

void zoom_pyramid_clear() {
    //1
    box_walls(0,0,0,7,0,7);
    delay_ms(500);

    //2
    //Pyramid
    box_wireframe(0,0,0,7,0,1);

    clrplane_y(0);
    delay_ms(500);

    //3
    //Pyramid
    clrplane_y(1);
    box_walls(0,2,0,7,2,7);
    delay_ms(500);

    //4
    //Pyramid
    clrplane_y(2);
    box_walls(0,3,0,7,3,7);
    delay_ms(500);

    //5
    //Pyramid
    clrplane_y(3);
    box_walls(0,4,0,7,4,7);
    delay_ms(500);

    //5
    //Pyramid
    clrplane_y(4);
    box_walls(0,5,0,7,5,7);
    delay_ms(500);

    //6
    //Pyramid
    clrplane_y(5);
    box_walls(0,6,0,7,6,7);
    delay_ms(500);

    //7
    //Pyramid
    clrplane_y(6);
    box_walls(0,7,0,7,7,7);
    delay_ms(500);

    clrplane_y(7);
    delay_ms(10000);
}

void zoom_pyramid() {
    //1
    fill(0x00);

    box_walls(0,0,0,7,0,7);
    delay_ms(500);

    //2
    fill(0x00);
    //Pyramid
    box_wireframe(0,0,0,7,0,1);

    box_walls(0,1,0,7,1,7);
    delay_ms(500);

    //3
    fill(0x00);
    //Pyramid
    box_wireframe(0,0,0,7,1,1);
    box_wireframe(1,1,2,6,1,3);

    box_walls(0,2,0,7,2,7);
    delay_ms(500);

    //4
    fill(0x00);
    //Pyramid
    box_wireframe(0,0,0,7,2,1);
    box_wireframe(1,1,2,6,2,3);
    box_wireframe(2,2,4,5,2,5);

    box_walls(0,3,0,7,3,7);
    delay_ms(500);

    //5
    fill(0x00);
    //Pyramid
    box_wireframe(0,0,0,7,3,1);
    box_wireframe(1,1,2,6,3,3);
    box_wireframe(2,2,4,5,3,5);
    box_wireframe(3,3,6,4,3,7);

    box_walls(0,4,0,7,4,7);
    delay_ms(500);

    //5
    fill(0x00);
    //Pyramid
    box_wireframe(0,0,0,7,4,1);
    box_wireframe(1,1,2,6,4,3);
    box_wireframe(2,2,4,5,4,5);
    box_wireframe(3,3,6,4,4,7);

    box_walls(0,5,0,7,5,7);
    delay_ms(500);
    //6

    fill(0x00);
    //Pyramid
    box_wireframe(0,0,0,7,5,1);
    box_wireframe(1,1,2,6,5,3);
    box_wireframe(2,2,4,5,5,5);
    box_wireframe(3,3,6,4,4,7);

    box_walls(0,6,0,7,6,7);
    delay_ms(500);
    //7
    fill(0x00);
    //Pyramid
    box_wireframe(0,0,0,7,6,1);
    box_wireframe(1,1,2,6,6,3);
    box_wireframe(2,2,4,5,5,5);
    box_wireframe(3,3,6,4,4,7);

    box_walls(0,7,0,7,7,7);
    delay_ms(500);

    fill(0x00);
    box_wireframe(0,0, 0, 7, 7, 1);
    box_wireframe(1,1, 2, 6, 6, 3);
    box_wireframe(2,2, 4, 5, 5, 5);
    box_wireframe(3,3, 6, 4, 4, 7);

    delay_ms(10000);
}

void effect_boxside_randsend_parallel (char axis, int origin, int delay, int mode)
{
    int i;
    int done;
    unsigned char cubepos[64];
    unsigned char pos[64];
    int notdone = 1;
    int notdone2 = 1;
    int sent = 0;

    for (i=0;i<64;i++)
    {
        pos[i] = 0;
    }

    while (notdone)
    {
        if (mode == 1)
        {
            notdone2 = 1;
            while (notdone2 && sent<64)
            {
                i = rand()%64;
                if (pos[i] == 0)
                {
                    sent++;
                    pos[i] += 1;
                    notdone2 = 0;
                }
            }
        } else if (mode == 2)
        {
            if (sent<64)
            {
                pos[sent] += 1;
                sent++;
            }
        }

        done = 0;
        for (i=0;i<64;i++)
        {
            if (pos[i] > 0 && pos[i] <7)
            {
                pos[i] += 1;
            }

            if (pos[i] == 7)
                done++;
        }

        if (done == 64)
            notdone = 0;

        for (i=0;i<64;i++)
        {
            if (origin == 0)
            {
                cubepos[i] = pos[i];
            } else
            {
                cubepos[i] = (7-pos[i]);
            }
        }


        delay_ms(delay);
        draw_positions_axis(axis,cubepos,0);
    }

}

void effect_axis_updown_randsuspend (char axis, int delay, int sleep, int invert)
{
    unsigned char positions[64];
    unsigned char destinations[64];

    int i,px;

    // Set 64 random positions
    for (i=0; i<64; i++)
    {
        positions[i] = 0; // Set all starting positions to 0
        destinations[i] = rand()%8;
    }

    // Loop 8 times to allow destination 7 to reach all the way
    for (i=0; i<8; i++)
    {
        // For every iteration, move all position one step closer to their destination
        for (px=0; px<64; px++)
        {
            if (positions[px]<destinations[px])
            {
                positions[px]++;
            }
        }
        // Draw the positions and take a nap
        draw_positions_axis (axis, positions,invert);
        delay_ms(delay);
    }

    // Set all destinations to 7 (opposite from the side they started out)
    for (i=0; i<64; i++)
    {
        destinations[i] = 7;
    }

    // Suspend the positions in mid-air for a while
    delay_ms(sleep);

    // Then do the same thing one more time
    for (i=0; i<8; i++)
    {
        for (px=0; px<64; px++)
        {
            if (positions[px]<destinations[px])
            {
                positions[px]++;
            }
            if (positions[px]>destinations[px])
            {
                positions[px]--;
            }
        }
        draw_positions_axis (axis, positions,invert);
        delay_ms(delay);
    }
}

void linespin (int iterations, int delay)
{
    float top_x, top_y, top_z, bot_x, bot_y, bot_z, sin_base;
    float center_x; //, center_y;

    center_x = 4;
    // center_y = 4;

    int i, z;
    for (i=0;i<iterations;i++)
    {

        //printf("Sin base %f \n",sin_base);

        for (z = 0; z < 8; z++)
        {

            sin_base = (float)i/50 + (float)z/(10+(7*sin((float)i/200)));

            top_x = center_x + sin(sin_base)*5;
            top_y = center_x + cos(sin_base)*5;
            //top_z = center_x + cos(sin_base/100)*2.5;

            bot_x = center_x + sin(sin_base+3.14)*10;
            bot_y = center_x + cos(sin_base+3.14)*10;
            //bot_z = 7-top_z;

            bot_z = z;
            top_z = z;

            // setvoxel((int) top_x, (int) top_y, 7);
            // setvoxel((int) bot_x, (int) bot_y, 0);

            //printf("P1: %i %i %i P2: %i %i %i \n", (int) top_x, (int) top_y, 7, (int) bot_x, (int) bot_y, 0);

            //line_3d((int) top_x, (int) top_y, (int) top_z, (int) bot_x, (int) bot_y, (int) bot_z);
            line_3d((int) top_z, (int) top_x, (int) top_y, (int) bot_z, (int) bot_x, (int) bot_y);
        }

        delay_ms(delay);
        fill(0x00);
    }

}

void effect_intro() {
    int cnt,cnt_2,time;

    //Bottom To Top

    for (cnt=0;cnt<=7;cnt++) {
        box_wireframe(0, 0, 0, 7, 7, cnt);
        delay_ms(2000);
    }
    for (cnt=0;cnt<7;cnt++) {
        clrplane_z(cnt);
        delay_ms(2000);
    }

    //Shift Things Right
    //1
    shift(AXIS_Y,-1);
    for (cnt=0;cnt<=7;cnt++) {
        setvoxel(cnt,0,6);
    }
    delay_ms(2000);
    //2
    shift(AXIS_Y,-1);
    for (cnt=0;cnt<=7;cnt++) {
        setvoxel(cnt,0,5);
    }
    setvoxel(0,0,6);
    setvoxel(7,0,6);
    delay_ms(2000);
    //3
    shift(AXIS_Y,-1);
    for (cnt=0;cnt<=7;cnt++) {
        setvoxel(cnt,0,4);
    }
    setvoxel(0,0,5);
    setvoxel(7,0,5);
    setvoxel(0,0,6);
    setvoxel(7,0,6);
    delay_ms(2000);

    //4
    shift(AXIS_Y,-1);
    for (cnt=0;cnt<=7;cnt++) {
        setvoxel(cnt,0,3);
    }
    setvoxel(0,0,4);
    setvoxel(7,0,4);
    setvoxel(0,0,5);
    setvoxel(7,0,5);
    setvoxel(0,0,6);
    setvoxel(7,0,6);
    delay_ms(2000);

    //5
    shift(AXIS_Y,-1);
    for (cnt=0;cnt<=7;cnt++) {
        setvoxel(cnt,0,2);
    }
    setvoxel(0,0,3);
    setvoxel(7,0,3);
    setvoxel(0,0,4);
    setvoxel(7,0,4);
    setvoxel(0,0,5);
    setvoxel(7,0,5);
    setvoxel(0,0,6);
    setvoxel(7,0,6);
    delay_ms(2000);

    //6
    shift(AXIS_Y,-1);
    for (cnt=0;cnt<=7;cnt++) {
        setvoxel(cnt,0,1);
    }
    setvoxel(0,0,2);
    setvoxel(7,0,2);
    setvoxel(0,0,3);
    setvoxel(7,0,3);
    setvoxel(0,0,4);
    setvoxel(7,0,4);
    setvoxel(0,0,5);
    setvoxel(7,0,5);
    delay_ms(2000);


    //7
    shift(AXIS_Y,-1);
    for (cnt=0;cnt<=7;cnt++) {
        setvoxel(cnt,0,0);
    }
    setvoxel(0,0,1);
    setvoxel(7,0,1);
    setvoxel(0,0,2);
    setvoxel(7,0,2);
    setvoxel(0,0,3);
    setvoxel(7,0,3);
    setvoxel(0,0,4);
    setvoxel(7,0,4);
    setvoxel(0,0,5);
    setvoxel(7,0,5);
    delay_ms(2000);

    //Right To Left
    for (cnt=0;cnt<=7;cnt++) {
        box_wireframe(0, 0, 0, 7, cnt, 7);
        delay_ms(2000);
    }
    for (cnt=0;cnt<7;cnt++) {
        clrplane_y(cnt);
        delay_ms(2000);
    }

    //Shift to the bottom
    for (cnt_2=6;cnt_2>=0;cnt_2--) {
        shift(AXIS_Z,-1);
        for (cnt=0;cnt<=7;cnt++) {
            setvoxel(cnt,cnt_2,0);
        }
        for (cnt=6;cnt>cnt_2;cnt--) {
            setvoxel(0,cnt,0);
            setvoxel(7,cnt,0);
        }

        delay_ms(2000);
    }

    //Make All Wall Box

    for (cnt=0;cnt<=6;cnt++) {
        fill(0x00);
        box_walls(0,0,0,7,7,cnt);
        delay_ms(2000);
    }

    time = 2000;
    for (cnt_2=0;cnt_2<5;cnt_2++) {
        time = time - 300;
        //Make Box Smaller
        for (cnt=0;cnt<=3;cnt++) {
            fill(0x00);
            box_walls(cnt,cnt,cnt,7-cnt,7-cnt,7-cnt);
            delay_ms(time);
        }

        //Make Box Bigger
        for (cnt=0;cnt<=3;cnt++) {
            fill(0x00);
            box_walls(3-cnt,3-cnt,3-cnt,4+cnt,4+cnt,4+cnt);
            delay_ms(time);
        }
    }
    for (cnt_2=0;cnt_2<5;cnt_2++) {
        time = time + 300;
        //Make Box Smaller
        for (cnt=0;cnt<=3;cnt++) {
            fill(0x00);
            box_walls(cnt,cnt,cnt,7-cnt,7-cnt,7-cnt);
            delay_ms(time);
        }

        //Make Box Bigger
        for (cnt=0;cnt<=3;cnt++) {
            fill(0x00);
            box_walls(3-cnt,3-cnt,3-cnt,4+cnt,4+cnt,4+cnt);
            delay_ms(time);
        }
    }
    delay_ms(2000);

}

void effect_blinky2()
{
    int i,r;
    fill(0x00);

    for (r=0;r<2;r++)
    {
        i = 750;
        while (i>0)
        {
            fill(0x00);
            delay_ms(i);

            fill(0xff);
            delay_ms(100);

            i = i - (15+(1000/(i/10)));
        }

        delay_ms(1000);

        i = 750;
        while (i>0)
        {
            fill(0x00);
            delay_ms(751-i);

            fill(0xff);
            delay_ms(100);

            i = i - (15+(1000/(i/10)));
        }
    }
}

void init_LUT(unsigned char LUT[65])
{
    unsigned char i;
    float sin_of,sine;
    for (i=0;i<65;i++)
    {
        sin_of=i*M_PI/64; // Just need half a sin wave
        sine=sin(sin_of);
        // Use 181.0 as this squared is <32767, so we can multiply two sin or cos without overflowing an int.
        LUT[i]=sine*181.0;
    }
}

int totty_sin(unsigned char LUT[65],int sin_of)
{
    unsigned char inv=0;
    if (sin_of<0)
    {
        sin_of=-sin_of;
        inv=1;
    }
    sin_of&=0x7f; //127
    if (sin_of>64)
    {
        sin_of-=64;
        inv=1-inv;
    }
    if (inv)
        return -LUT[sin_of];
    else
        return LUT[sin_of];
}

int totty_cos(unsigned char LUT[65],int cos_of)
{
    unsigned char inv=0;
    cos_of+=32;// Simply rotate by 90 degrees for COS
    cos_of&=0x7f;//127
    if (cos_of>64)
    {
        cos_of-=64;
        inv=1;
    }
    if (inv)
        return -LUT[cos_of];
    else
        return LUT[cos_of];
}

void int_ripples(int iterations, int delay)
{
    // 16 values for square root of a^2+b^2.  index a*4+b = 10*sqrt
    // This gives the distance to 3.5,3.5 from the point
    unsigned char sqrt_LUT[]={49,43,38,35,43,35,29,26,38,29,21,16,35,25,16,7};
    //LUT_START // Macro from new tottymath.  Commented and replaced with full code
    unsigned char LUT[65];
    init_LUT(LUT);
    int i;
    unsigned char x,y,height,distance;
    for (i=0;i<iterations*4;i+=4)
    {
        fill(0x00);
        for (x=0;x<4;x++)
            for (y=0;y<4;y++)
            {
                // x+y*4 gives no. from 0-15 for sqrt_LUT
                distance=sqrt_LUT[x+y*4];// distance is 0-50 roughly
                // height is sin of distance + iteration*4
                //height=4+totty_sin(LUT,distance+i)/52;
                height=(196+totty_sin(LUT,distance+i))/49;
                // Use 4-way mirroring to save on calculations
                setvoxel(x,y,height);
                setvoxel(7-x,y,height);
                setvoxel(x,7-y,height);
                setvoxel(7-x,7-y,height);
            }
        delay_ms(delay);
    }
}

void side_ripples(int iterations, int delay)
{
    // 16 values for square root of a^2+b^2.  index a*4+b = 10*sqrt
    // This gives the distance to 3.5,3.5 from the point
    unsigned char sqrt_LUT[]={49,43,38,35,43,35,29,26,38,29,21,16,35,25,16,7};
    //LUT_START // Macro from new tottymath.  Commented and replaced with full code
    unsigned char LUT[65];
    init_LUT(LUT);
    int i;
    unsigned char x,y,height,distance;
    for (i=0;i<iterations*4;i+=4)
    {
        fill(0x00);
        for (x=0;x<4;x++)
            for (y=0;y<4;y++)
            {
                // x+y*4 gives no. from 0-15 for sqrt_LUT
                distance=sqrt_LUT[x+y*4];// distance is 0-50 roughly
                // height is sin of distance + iteration*4
                //height=4+totty_sin(LUT,distance+i)/52;
                height=(196+totty_sin(LUT,distance+i))/49;
                // Use 4-way mirroring to save on calculations
                setvoxel(x,height,y);
                setvoxel(7-x,height,y);
                setvoxel(x,height,7-y);
                setvoxel(7-x,height,7-y);
                setvoxel(x,7-height,y);
                setvoxel(7-x,7-height,y);
                setvoxel(x,7-height,7-y);
                setvoxel(7-x,7-height,7-y);

            }
        delay_ms(delay);
    }
}

void mirror_ripples(int iterations, int delay)
{
    // 16 values for square root of a^2+b^2.  index a*4+b = 10*sqrt
    // This gives the distance to 3.5,3.5 from the point
    unsigned char sqrt_LUT[]={49,43,38,35,43,35,29,26,38,29,21,16,35,25,16,7};
    //LUT_START // Macro from new tottymath.  Commented and replaced with full code
    unsigned char LUT[65];
    init_LUT(LUT);
    int i;
    unsigned char x,y,height,distance;
    for (i=0;i<iterations*4;i+=4)
    {
        fill(0x00);
        for (x=0;x<4;x++)
            for (y=0;y<4;y++)
            {
                // x+y*4 gives no. from 0-15 for sqrt_LUT
                distance=sqrt_LUT[x+y*4];// distance is 0-50 roughly
                // height is sin of distance + iteration*4
                //height=4+totty_sin(LUT,distance+i)/52;
                height=(196+totty_sin(LUT,distance+i))/49;
                // Use 4-way mirroring to save on calculations
                setvoxel(x,y,height);
                setvoxel(7-x,y,height);
                setvoxel(x,7-y,height);
                setvoxel(7-x,7-y,height);
                setvoxel(x,y,7-height);
                setvoxel(7-x,y,7-height);
                setvoxel(x,7-y,7-height);
                setvoxel(7-x,7-y,7-height);

            }
        delay_ms(delay);
    }
}

void quad_ripples(int iterations, int delay)
{
    // 16 values for square root of a^2+b^2.  index a*4+b = 10*sqrt
    // This gives the distance to 3.5,3.5 from the point
    unsigned char roots_LUT[]={49,43,38,35,43,35,29,26,38,29,21,16,35,25,16,7};
    //LUT_START // Macro from new tottymath.  Commented and replaced with full code
    unsigned char LUT[65];
    init_LUT(LUT);
    int i;
    unsigned char x,y,height,distance;
    for (i=0;i<iterations*4;i+=4)
    {
        fill(0x00);
        for (x=0;x<4;x++)
            for (y=0;y<4;y++)
            {
                // x+y*4 gives no. from 0-15 for sqrt_LUT
                distance=roots_LUT[x+y*4];// distance is 0-50 roughly
                // height is sin of distance + iteration*4
                //height=4+totty_sin(LUT,distance+i)/52;
                height=(196+totty_sin(LUT,distance+i))/49;
                // Use 4-way mirroring to save on calculations
                setvoxel(x,y,height);
                setvoxel(7-x,y,height);
                setvoxel(x,7-y,height);
                setvoxel(7-x,7-y,height);
                setvoxel(x,y,7-height);
                setvoxel(7-x,y,7-height);
                setvoxel(x,7-y,7-height);
                setvoxel(7-x,7-y,7-height);
                setvoxel(x,height,y);
                setvoxel(7-x,height,y);
                setvoxel(x,height,7-y);
                setvoxel(7-x,height,7-y);
                setvoxel(x,7-height,y);
                setvoxel(7-x,7-height,y);
                setvoxel(x,7-height,7-y);
                setvoxel(7-x,7-height,7-y);
            }
        delay_ms(delay);
    }
}

int effect_telcstairs_do(int x, int val, int delay)
{
    int y,z;

    for (y = 0, z = x; y <= z; y++, x--)
    {
        if (x < CUBE_SIZE && y < CUBE_SIZE)
        {
            cube[x][y] = val;
        }
    }
    delay_ms(delay);
    return z;
}

void effect_telcstairs (int invert, int delay, int val)
{
    int x;

    if (invert)
    {
        for (x = CUBE_SIZE*2; x >= 0; x--)
        {
            x = effect_telcstairs_do(x,val,delay);
        }
    }
    else
    {
        for (x = 0; x < CUBE_SIZE*2; x++)
        {
            x = effect_telcstairs_do(x,val,delay);
        }
    }
}

const unsigned char paths[44] PROGMEM = {0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x67,0x57,0x47,0x37,0x27,0x17,
                                        0x04,0x03,0x12,0x21,0x30,0x40,0x51,0x62,0x73,0x74,0x65,0x56,0x47,0x37,0x26,0x15}; // circle, len 16, offset 28

void font_getpath (unsigned char path, unsigned char *destination, int length)
{
    int i;
    int offset = 0;

    if (path == 1)
        offset=28;

    for (i = 0; i < length; i++)
        destination[i] = pgm_read_byte(&paths[i+offset]);
}

void effect_pathmove (unsigned char *path, int length)
{
    int i,z;
    unsigned char state;

    for (i=(length-1);i>=1;i--)
    {
        for (z=0;z<8;z++)
        {

            state = getvoxel(((path[(i-1)]>>4) & 0x0f), (path[(i-1)] & 0x0f), z);
            altervoxel(((path[i]>>4) & 0x0f), (path[i] & 0x0f), z, state);
        }
    }
    for (i=0;i<8;i++)
        clrvoxel(((path[0]>>4) & 0x0f), (path[0] & 0x0f),i);
}

void effect_rand_patharound (int iterations, int delay)
{
    int z, dz, i;
    z = 4;
    unsigned char path[28];

    font_getpath(0,path,28);

    for (i = 0; i < iterations; i++)
    {
        dz = ((rand()%3)-1);
        z += dz;

        if (z>7)
            z = 7;

        if (z<0)
            z = 0;

        effect_pathmove(path, 28);
        setvoxel(0,7,z);
        delay_ms(delay);
    }
}
