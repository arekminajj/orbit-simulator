#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "defines.h"
#include "orbit.h"

void init();
void loop();
void kill();
void loadTexture(const char* filename, SDL_Texture** texture);

SDL_Window* win;
SDL_Renderer* renderer;
SDL_Texture* earthTexture;
SDL_Texture* moonTexture;

SDL_Event ev;
bool running = true;

int main()
{
    init();
    loop();
    kill();

    return 0;
}

void init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to init SDL!\n");
    }

    win = SDL_CreateWindow("Orbit Simulator", 500, 500, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if(win == NULL)
    {
        printf("Failed to create a window!\n");
    }

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
    {
        printf("Failed to create a renderer!\n");
    }

    if(IMG_Init(IMG_INIT_JPG) < 0)
    {
        printf("Failed to init SDL_imae!\n");
    }

    loadTexture("assets/earth.jpg", &earthTexture);
    loadTexture("assets/moon.jpg", &moonTexture);

    initBodies();
}

float timeMultiplier = 100.0f;

void loop()
{
    Uint32 lastUpdate = SDL_GetTicks();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect earthRect;
    earthRect.h = EARTH_HEIGHT;
    earthRect.w = EARTH_WIDTH;

    SDL_Rect moonRect;
    moonRect.h = (MOON_AVG_RADIUS * EARTH_HEIGHT) / EARTH_AVG_RADIUS;
    moonRect.w = (MOON_AVG_RADIUS * EARTH_WIDTH) / EARTH_AVG_RADIUS;
    
    while ( running ) {	
        Uint32 current = SDL_GetTicks();
	    while ( SDL_PollEvent( &ev ) != 0 ) {
		    switch (ev.type) {
			    case SDL_QUIT:
				    running = false;
				    break;
		    }
	    }	

        printf("Earth x position: %f\n", earth.position.x);
        printf("Earth y position: %f\n", earth.position.y);
        printf("Moon mass: %f\n", moon.mass);
        printf("Moon x position: %f\n", moon.position.x);
        printf("Moon y position: %f\n", moon.position.y);
        printf("Moon speed: %f\n", moon.speed);
        printf("Moon x velocity: %f\n", moon.velocity.x);
        printf("Moon y velocity: %f\n", moon.velocity.y);

        float deltaTime = (current - lastUpdate) / 1000.0f;
        update(deltaTime * timeMultiplier);
        lastUpdate = current;

        SDL_RenderClear(renderer);

        earthRect.x = earth.position.x - (EARTH_WIDTH / 2);
        earthRect.y = earth.position.y - (EARTH_HEIGHT / 2);

        moonRect.x = moon.position.x;
        moonRect.y = moon.position.y;

        SDL_RenderCopy(renderer, earthTexture, NULL, &earthRect);
        SDL_RenderCopy(renderer, moonTexture, NULL, &moonRect);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
}

void kill()
{
    SDL_DestroyWindow(win);
    win = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyTexture(earthTexture);
    earthTexture = NULL;
    SDL_DestroyTexture(moonTexture);
    moonTexture = NULL;
    SDL_Quit();
    IMG_Quit();
}

void loadTexture(const char* filename, SDL_Texture** texture)
{
    *texture = IMG_LoadTexture(renderer, filename);

    if(*texture == NULL)
    {
        printf("Error loading earth texture from file: %s", filename);
    }
}
