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
    earth.colliderRadius = 45.0f;
    earth.isRendered = true;

    moon.gravity = MOON_GRAVITY * SCALE_FACTOR;
    moon.mass = MOON_MASS * SCALE_FACTOR;
    moon.position.x = earth.position.x + (MOON_DISTANCE_FROM_EARTH * SCALE_FACTOR);
    moon.position.y = earth.position.y;
    moon.speed = MOON_INITIAL_SPEED * SCALE_FACTOR;
    moon.velocity.x = 0;
    moon.velocity.y = moon.speed;
    moon.colliderRadius = 10.0f;
    moon.isRendered = true;
}

bool isColliding(float distance, float colliderRadius1, float colliderRadius2)
{
    if(distance < colliderRadius1 + colliderRadius2)
        return true;
    return false;
}

void resolveCollision()
{
   moon.velocity.x = 0;
   moon.velocity.y = 0;
   moon.isRendered = false;
}

void update(float deltaTime)
{
    float distanceSquared = pow((earth.position.x - moon.position.x), 2) + pow((earth.position.y - moon.position.y), 2);
    if(distanceSquared == 0) return;
    float distance = sqrt(distanceSquared);
    if(isColliding(distance, moon.colliderRadius, earth.colliderRadius))
    {
        resolveCollision();
        return;
    }
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
