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

static float timeMultiplier = 100.0f;
static int cameraX = 60;
static int cameraY = 110;
static float renderScale = 1;

SDL_Rect earthRect;
SDL_Rect moonRect;

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

void render(int camX, int camY)
{
    SDL_RenderSetScale(renderer, renderScale, renderScale);
    moonRect.x = moon.position.x - camX;
    moonRect.y = moon.position.y - camY;
    moonRect.h = (MOON_AVG_RADIUS * EARTH_HEIGHT) / EARTH_AVG_RADIUS;
    moonRect.w = (MOON_AVG_RADIUS * EARTH_WIDTH) / EARTH_AVG_RADIUS;

    earthRect.x = earth.position.x - camX;
    earthRect.y = earth.position.y - camY;
    earthRect.h = EARTH_HEIGHT;
    earthRect.w = EARTH_WIDTH;

    SDL_RenderCopy(renderer, earthTexture, NULL, &earthRect);
    SDL_RenderCopy(renderer, moonTexture, NULL, &moonRect);
    SDL_RenderPresent(renderer);
}

void loop()
{
    Uint32 lastUpdate = SDL_GetTicks();
    while ( running ) {	
        Uint32 current = SDL_GetTicks();
	    while ( SDL_PollEvent( &ev ) != 0 ) {
		    switch (ev.type) {
			    case SDL_QUIT:
				    running = false;
				    break;
                case SDL_KEYDOWN:
                    switch (ev.key.keysym.sym)
                    {
                        case SDLK_w:
                            cameraY-=10;
                            break;
                        case SDLK_a:
                            cameraX-=10;
                            break;
                        case SDLK_s:
                            cameraY+=10;
                            break;
                        case SDLK_d:
                            cameraX+=10;
                            break;
                    }        
                    break;
                case SDL_MOUSEWHEEL:
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    
                    float worldX = cameraX + mouseX / renderScale;
                    float worldY = cameraY + mouseY / renderScale;
                    
                    float newScale = renderScale + (renderScale * ev.wheel.y * 0.1f);
                    if (newScale < 0.1f)
                        newScale = 0.1f;
                    renderScale = newScale;
                    
                    cameraX = worldX - mouseX / renderScale;
                    cameraY = worldY - mouseY / renderScale;
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
        printf("Camera x potition: %i\n", cameraX);
        printf("Camera y potition: %i\n", cameraY);

        float deltaTime = (current - lastUpdate) / 1000.0f;
        update(deltaTime * timeMultiplier);
        lastUpdate = current;

        SDL_RenderClear(renderer);

        render(cameraX, cameraY);

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
