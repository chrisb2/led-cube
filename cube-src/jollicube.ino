#include "cube.h"
#include "text-effects.h"
#include "effects.h"
#include "functions.h"
#include <string.h>
#include <math.h>

#define REFRESH_INTERVAL_MS 8
#define SPI_CS A2 // This SPI Chip Select pin controls the MAX7219
#define FUNC_CNT 17
#define MAX_INTENSITY 15

byte value[8];
volatile int current_layer = 0;

Timer timer(REFRESH_INTERVAL_MS, display);

volatile int sequenceValue = 2; // in order
volatile int effectValue = 0; // intro
int currentEffect = effectValue;

typedef void (*FP)();

const FP effects[FUNC_CNT] = {&effect_intro, &fireworks, &effect_rain, &effect_random_sparkle, &effect_random_filler,
    &effect_wormsqueeze, &sinelines, &linespin, &effect_planboing, &effect_boxside_randsend_parallel, &effect_axis_updown_randsuspend,
    &int_ripples, &side_ripples, &mirror_ripples, &quad_ripples, &effect_rand_patharound, &pyramid};

//***********************************************************************************************************************
void setup()
{

    pinMode(SPI_CS, OUTPUT);
    SPI.begin();
    randomSeed(analogRead(0));

    maxTransferAll(0x0F, 0x00);   // 00 - Turn off Test mode
    maxTransferAll(0x09, 0x00);   // Register 09 - BCD Decoding  // 0 = No decoding
    maxTransferAll(0x0B, 0x07);   // Register B - Scan limit 1-7  // 7 = All LEDS
    maxTransferAll(0x0C, 0x01);   // 01 = on 00 = Power saving mode or shutdown
    maxTransferAll(0x0A, 0x0F);   // Set Brightness Intensity

    timer.start();

    Particle.function("effect", effect);
    Particle.function("sequence", sequence);
    Particle.function("brightness", brightness);
    Particle.function("power", power);

    Particle.variable("effect", currentEffect);
}

void loop()
{
    switch(sequenceValue) {
        case 0:
            // Single effect
            break;
        case 1:
            // Go through effects in random order
            effectValue = random(FUNC_CNT - 1);
            break;
        case 2:
            // Go through effects in order
            effectValue++;
            if (effectValue == FUNC_CNT) {
                effectValue = 0;
            }
            break;
    }

    currentEffect = effectValue;
    effects[effectValue]();
    delay(100);
}

int effect(String args)
{
    int value = args.toInt();
    if (value >= FUNC_CNT || value < 0) {
        effectValue = 0;
    } else {
        effectValue = value;
    }
    return effectValue;
}

int brightness(String args) {
    int intensity = args.toInt();
    if (intensity < 0 || intensity > MAX_INTENSITY) {
        intensity = MAX_INTENSITY;
    }
    maxTransferAll(0x0A, intensity);
    return intensity;
}

int power(String args) {
    int power = args.toInt();
    if (power == 0 || power == 1) {
        maxTransferAll(0x0C, power);
    } else {
        power = 1;
    }
    return power;
}

int sequence(String args) {
    int value = args.toInt();
    if (value >= 0 || value < 3) {
        sequenceValue = value;
    }
    return sequenceValue;
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
