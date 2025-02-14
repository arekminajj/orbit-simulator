#pragma once
#include "defines.h"
#include "math.h"
#include "stdio.h"
#include "stdbool.h"

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
    float colliderRadius;
    bool isRendered;
} celestialBody;

extern celestialBody earth;
extern celestialBody moon;

void initBodies();
void update(float deltaTime);
