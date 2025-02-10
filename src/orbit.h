#pragma once
#include "defines.h"
#include "math.h"
#include "stdio.h"

typedef struct {
    float x;
    float y;
} vec2;

typedef struct {
    vec2 position;
    vec2 velocity;
    double gravity;
    double speed;
    double mass;
} celestialBody;

extern celestialBody earth;
extern celestialBody moon;

void initBodies();
void update(float deltaTime);
