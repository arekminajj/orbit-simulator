#pragma once

/*
    Now one pixel is 1000km. Might be usefull if i wanted to add zomming and camera moving feat.
*/
#define SCALE_FACTOR 1.0e-6

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1024

#define EARTH_WIDTH 100
#define EARTH_HEIGHT 100

/*
    Default physics values for objects.
*/

#define GRAVITATIONAL_CONSTANT 6.67430e-11

#define EARTH_MASS 5.972e24f
#define EARTH_GRAVITY 9.81f
#define EARTH_AVG_RADIUS 6378
#define EARTH_INITIAL_SPEED 0

#define MOON_MASS 7.347e22f
#define MOON_GRAVITY 1.62f
#define MOON_AVG_RADIUS 1737
#define MOON_INITIAL_SPEED 1022
#define MOON_DISTANCE_FROM_EARTH 384400000