#include "orbit.h"

celestialBody earth;
celestialBody moon;

void initBodies()
{
    earth.gravity = EARTH_GRAVITY * SCALE_FACTOR;
    earth.mass = EARTH_MASS * SCALE_FACTOR;
    earth.position.x = WINDOW_WIDTH / 2;
    earth.position.y = WINDOW_HEIGHT / 2;
    earth.speed = 0;
    earth.velocity.x = 0;
    earth.velocity.y = 0;

    moon.gravity = MOON_GRAVITY * SCALE_FACTOR;
    moon.mass = MOON_MASS * SCALE_FACTOR;
    moon.position.x = earth.position.x + (MOON_DISTANCE_FROM_EARTH * SCALE_FACTOR);
    moon.position.y = earth.position.y;
    float distance = MOON_DISTANCE_FROM_EARTH * SCALE_FACTOR;
    float orbitalVelocity = sqrt(GRAVITATIONAL_CONSTANT * (earth.mass / SCALE_FACTOR) / (distance));
    orbitalVelocity *= SCALE_FACTOR;
    moon.velocity.x = 0;
    moon.velocity.y = orbitalVelocity;
}

void update(float deltaTime)
{
    float distanceSquared = pow((earth.position.x - moon.position.x), 2) + pow((earth.position.y - moon.position.y), 2);
    if(distanceSquared == 0) return;
    float distance = sqrt(distanceSquared);
    float force = GRAVITATIONAL_CONSTANT * SCALE_FACTOR * ((earth.mass * moon.mass) / distanceSquared);
    vec2 forceDirection = {earth.position.x - moon.position.x, earth.position.y - moon.position.y};
    vec2 normalizedForceDirection = {forceDirection.x / distance, forceDirection.y / distance};

    printf("Normalized force vector x: %f\n", normalizedForceDirection.x);
    printf("Normalized force vector y: %f\n", normalizedForceDirection.y);

    moon.velocity.x += normalizedForceDirection.x * (force / moon.mass) * deltaTime;
    moon.velocity.y += normalizedForceDirection.y * (force / moon.mass) * deltaTime;

    moon.position.x += moon.velocity.x * deltaTime;
    moon.position.y += moon.velocity.y * deltaTime;
}
