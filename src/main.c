#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "defines.h"
#include "orbit.h"
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"

void init();
void loop();
void kill();
void loadTexture(const char* filename, SDL_Texture** texture);

static float timeMultiplier = 100.0f;
static int cameraX = 60;
static int cameraY = 110;
static float renderScale = 1;
static bool drawPath = false;
SDL_Point* path;
static size_t pathPointIndex = 0;
const int PATH_POINT_DISTANCE = 5;
static int currentDistance = 0;

SDL_Rect earthRect;
SDL_Rect moonRect;

SDL_Window* win;
SDL_Renderer* renderer;
SDL_Texture* earthTexture;
SDL_Texture* moonTexture;

SDL_Event ev;
bool running = true;

//Nuklear
struct nk_context *ctx;
struct nk_colorf bg;
float fontScale = 1;

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

    loadTexture("assets/earth.png", &earthTexture);
    loadTexture("assets/moon.png", &moonTexture);

    initBodies();

    ctx = nk_sdl_init(win, renderer);
    {
        struct nk_font_atlas *atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font *font;
        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_from_file(atlas, "assets/roboto.ttf", 16 * fontScale, &config);
        nk_sdl_font_stash_end();
        font->handle.height /= fontScale;
        nk_style_set_font(ctx, &font->handle);
    }

    path = (SDL_Point*)malloc(MAX_POINTS * sizeof(SDL_Point));
}

static float earthMassMultiplier = 1.0f;
static float moonMassMultiplier = 1.0f;

void gui()
{
    //gui

    if (nk_begin(ctx, "Properties", nk_rect(50, 50, 230, 250),
        NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
        NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
    {
   
        nk_layout_row_static(ctx, 30, 80, 1);
        if (nk_button_label(ctx, "Reset"))
        {
            earth.mass = EARTH_MASS * SCALE_FACTOR;
                
            moon.mass = MOON_MASS * SCALE_FACTOR;
            moon.position.x = earth.position.x + (MOON_DISTANCE_FROM_EARTH * SCALE_FACTOR);
            moon.position.y = earth.position.y;
            moon.velocity.x = 0;
            moon.velocity.y = moon.speed;

            //reset path as well.
            pathPointIndex = 0;
            memset(path, 0, sizeof(SDL_Point) * MAX_POINTS);
        }
        nk_layout_row_dynamic(ctx, 30, 2);
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_property_float(ctx, "Earth mass:", 0, &earthMassMultiplier, 20.0f, 0.2f, 0.0f);
        nk_property_float(ctx, "Moon mass:", 0, &moonMassMultiplier, 20.0f, 0.2f, 0.0f);
        if (nk_button_label(ctx, "Deafault values"))
        {
            earthMassMultiplier = 1.0f;
            moonMassMultiplier = 1.0f;
        }
        if (nk_button_label(ctx, "Draw path"))
        {
            drawPath = !drawPath;
            pathPointIndex = 0;
            memset(path, 0, sizeof(SDL_Point) * MAX_POINTS);
        }

        earth.mass = earthMassMultiplier * EARTH_MASS * SCALE_FACTOR; 
        moon.mass = moonMassMultiplier * MOON_MASS * SCALE_FACTOR;
    }
    nk_end(ctx);
}

void render(int camX, int camY)
{
    moonRect.x = moon.position.x - camX;
    moonRect.y = moon.position.y - camY;
    moonRect.h = (MOON_AVG_RADIUS * EARTH_HEIGHT) / EARTH_AVG_RADIUS;
    moonRect.w = (MOON_AVG_RADIUS * EARTH_WIDTH) / EARTH_AVG_RADIUS;

    earthRect.x = earth.position.x - camX;
    earthRect.y = earth.position.y - camY;
    earthRect.h = EARTH_HEIGHT;
    earthRect.w = EARTH_WIDTH;

    SDL_RenderSetScale(renderer, renderScale, renderScale);

    if(drawPath)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Point* scaledPath = (SDL_Point*)malloc(pathPointIndex * sizeof(SDL_Point));
        for (size_t i = 0; i < pathPointIndex; i++) 
        {
            scaledPath[i].x = path[i].x - cameraX;
            scaledPath[i].y = path[i].y - cameraY;
        }
        SDL_RenderDrawLines(renderer, scaledPath, pathPointIndex);
        free(scaledPath);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    gui();

    //render
    SDL_RenderSetScale(renderer, 1, 1);
    nk_sdl_render(NK_ANTI_ALIASING_ON);
    SDL_RenderSetScale(renderer, renderScale, renderScale);
    SDL_RenderCopy(renderer, earthTexture, NULL, &earthRect);
    SDL_RenderCopy(renderer, moonTexture, NULL, &moonRect);
    SDL_RenderPresent(renderer);
}

void loop()
{
    Uint32 lastUpdate = SDL_GetTicks();
    while ( running ) {	
        Uint32 current = SDL_GetTicks();
        nk_input_begin(ctx);
	    while ( SDL_PollEvent( &ev ) != 0 ) {
            nk_sdl_handle_event(&ev);
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

        nk_sdl_handle_grab();
        nk_input_end(ctx);

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

        if(currentDistance >= PATH_POINT_DISTANCE)
        {
            path[pathPointIndex].x = moon.position.x + moonRect.w / 2;
            path[pathPointIndex].y = moon.position.y + moonRect.h / 2;
            pathPointIndex = (pathPointIndex + 1) % MAX_POINTS;
            currentDistance = 0;
        }
        
        currentDistance++;

        SDL_RenderClear(renderer);
        render(cameraX, cameraY);

        SDL_Delay(16);
    }    
}


void kill()
{
    free(path);
    path = NULL;

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

    //nuklear kill
    nk_sdl_shutdown();
}

void loadTexture(const char* filename, SDL_Texture** texture)
{
    *texture = IMG_LoadTexture(renderer, filename);

    if(*texture == NULL)
    {
        printf("Error loading earth texture from file: %s", filename);
    }
}
