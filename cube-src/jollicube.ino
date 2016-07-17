#include "cube.h"
#include "text-effects.h"
#include "effects.h"
#include "functions.h"
#include <string.h>
#include <math.h>

/*
You may check out our instructables for more detail at http://www.instructables.com/id/JolliCube-an-8x8x8-LED-Cube-SPI/
https://docs.particle.io/reference/firmware/photon/#spi
*/

// SYSTEM_MODE(SEMI_AUTOMATIC);

#define REFRESH_INTERVAL_MS 8
#define SPI_CS A2 // This SPI Chip Select pin controls the MAX7219

byte value[8];
volatile int current_layer = 0;

Timer timer(REFRESH_INTERVAL_MS, display);

volatile int commandValue = 0;
int effectValue = 0;

//***********************************************************************************************************************
void setup()
{
    pinMode(SPI_CS, OUTPUT);
    SPI.begin();

    maxTransferAll(0x0F, 0x00);   // 00 - Turn off Test mode
    maxTransferAll(0x09, 0x00);   // Register 09 - BCD Decoding  // 0 = No decoding
    maxTransferAll(0x0B, 0x07);   // Register B - Scan limit 1-7  // 7 = All LEDS
    maxTransferAll(0x0C, 0x01);   // 01 = on 00 = Power saving mode or shutdown
    maxTransferAll(0x0A, 0x0F);   // Set Brightness Intensity

    timer.start();
    
    Particle.function("ledcube", control);
}

void loop()
{
    if (commandValue == 1) {
        fireworks(15, 15, 500);
        delay(100);
    } else {
        switch (effectValue) {
            case 0:
                effect_intro();
                break;
            case 1:
                effect_rain(100);
                delay(100);
                effect_random_sparkle();
                delay(100);
                effect_random_filler(75, 1);
                break;
            case 2:
                fireworks(15, 15, 500);
                break;
        }
        
        effectValue++;
        if (effectValue == 3) {
            effectValue = 0;
        }
        
        delay(100);
        
        // effect_rand_patharound(200,500);
        // delay(100);
        // effect_wormsqueeze (2, AXIS_Z, -1, 100, 1000);
        // delay(100);
        // zoom_pyramid();
        // delay(100);
        // zoom_pyramid_clear();
        // delay(100);
        // sinelines(2000,10);
        // delay(100);
        // linespin(1500,20);
        // delay(150);
        // effect_planboing(AXIS_Z, 900);
        // delay(100);
        // effect_planboing(AXIS_Y, 900);
        // delay(100);
        // effect_planboing(AXIS_X, 900);
        // delay(100);
        // mirror_ripples(600,400);
        // delay(100);
        // side_ripples(600,400);
        // delay(100);
        // int_ripples(600,400);
        // delay(100);
        // quad_ripples(15, 500);
        // delay(100);
        // effect_axis_updown_randsuspend(AXIS_Z, 550,5000,0);
        // delay(100);
        // effect_axis_updown_randsuspend(AXIS_Z, 550,5000,1);
        // delay(100);
        // effect_axis_updown_randsuspend(AXIS_Z, 550,5000,0);
        // delay(100);
        // effect_axis_updown_randsuspend(AXIS_Z, 550,5000,1);
        // delay(100);
        // effect_axis_updown_randsuspend(AXIS_X, 550,5000,0);
        // delay(100);
        // effect_axis_updown_randsuspend(AXIS_X, 550,5000,1);
        // delay(100);
        // effect_axis_updown_randsuspend(AXIS_Y, 550,5000,0);
        // delay(100);
        // effect_axis_updown_randsuspend(AXIS_Y, 550,5000,1);
        // delay(100);
        // effect_boxside_randsend_parallel (AXIS_X, 0, 150, 1);
        // delay(100);
        // effect_boxside_randsend_parallel (AXIS_X, 1, 150, 1);
        // delay(100);
        // effect_boxside_randsend_parallel (AXIS_Y, 0, 150, 1);
        // delay(100);
        // effect_boxside_randsend_parallel (AXIS_Y, 1, 150, 1);
        // delay(100);
        // effect_boxside_randsend_parallel (AXIS_Z, 0, 150, 1);
        // delay(100);
        // effect_boxside_randsend_parallel (AXIS_Z, 1, 150, 1);
        // delay(100);
    }
}

int control(String args)
{
    commandValue = args.toInt();
    return commandValue;
}

void display()
{
    for (byte y = 0 ; y < CUBE_SIZE ; y++)
    {
        byte b = 0;
        for (byte x = 0 ; x < CUBE_SIZE ; x++)
        {
            //form data byte
            b = b << 1;
            if (getvoxel(x,y,current_layer)==1)
            {
                b |= 1;
            }
        }
        value[y] = b;
    }

    maxTransferLEDCube(CUBE_SIZE - current_layer);

    current_layer++;
    if (current_layer == 8)
        current_layer = 0;
}

void maxTransferAll(uint8_t address, uint8_t value)
{
    digitalWrite(SPI_CS, LOW);

    for ( int c=1; c<= CUBE_SIZE;c++) {
        SPI.transfer(address);
        SPI.transfer(value);
    }

    digitalWrite(SPI_CS, HIGH);
}

void maxTransferLEDCube(uint8_t address)
{
    digitalWrite(SPI_CS, LOW);

    for (int i=0; i<CUBE_SIZE; i++)   // Loop through our number of Max72xx ICs
    {
        SPI.transfer(address);
        SPI.transfer(value[CUBE_SIZE-i-1]);
    }

    digitalWrite(SPI_CS, HIGH);
}