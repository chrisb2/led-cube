#include <application.h>
#include <math.h>
#include "cube.h"
#include "functions.h"

void effect_rain();
void effect_rain (int iterations);
void fireworks();
void fireworks (int iterations, int n, int delay);
void effect_random_sparkle (void);
void effect_wormsqueeze (int size, int axis, int direction, int iterations, int delay);
void sinelines (int iterations, int delay);
void effect_planboing (int plane, int speed);
void effect_random_filler();
void effect_random_filler (int delay, int state);
void zoom_pyramid_clear();
void zoom_pyramid();
void effect_boxside_randsend_parallel (char axis, int origin, int delay, int mode);
void effect_axis_updown_randsuspend (char axis, int delay, int sleep, int invert);
void linespin (int iterations, int delay);
void effect_intro();
void effect_blinky2();
void int_ripples(int iterations, int delay);
void side_ripples(int iterations, int delay);
void mirror_ripples(int iterations, int delay);
void quad_ripples(int iterations, int delay);
void effect_telcstairs (int invert, int delay, int val);
void effect_rand_patharound (int iterations, int delay);
