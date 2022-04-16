#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "circle.h"
#include <cstdlib>
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
int rand();

//define window dimensions
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 640;

//define SDL Window related variables
SDL_Window *window = NULL;
SDL_Renderer *windowRenderer = NULL;
SDL_Event currentEvent;

SDL_Rect rectangle;

bool quit = false;

int mouseX, mouseY;

Circle cerc;

bool goingRight;

bool animationRunning = false;

int hits;

int directionX, directionY;

float circleSpeed;

float padSpeed;


bool initWindow() {

    bool success = true;


    if(SDL_Init(SDL_INIT_VIDEO) < 0) {

        std::cout << "SDL initialization failed" << std::endl;
        success = false;

    } else {


        window = SDL_CreateWindow(
                "SDL Hello Window Example",
                
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
            
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
               
                SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

        if(window == NULL) {

            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;

        } else {

           
            windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if(windowRenderer == NULL) {

                std::cout << "Failed to create the renderer: " << SDL_GetError() << std::endl;
                success = false;

            } else {
        
                SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 0);

   
                
                SDL_RenderClear(windowRenderer);
            }
        }
    }

    return success;
}

void initObjects() {
    //init playerbar
    rectangle.x = 10;
    rectangle.y = 220;
    rectangle.w = 10;
    rectangle.h = 200;
    padSpeed = 20;

    hits = 0;
    //init circle
    cerc.radius = 10.0f;
    cerc.center.x = 640.0f;
    cerc.center.y = 320.0f;
    cerc.color = glm::vec4(255, 255, 255, 255);
    srand(time(NULL));
    directionX = rand() % 20 +(-10);
    directionY = rand() % 20 +(-10);
    circleSpeed = 0.01f;
    if (directionX * circleSpeed > 1)
        directionX = 1 / circleSpeed;
    if (directionY * circleSpeed > 1)
        directionY = 1 / circleSpeed;
}

void processEvents() {

    SDL_PollEvent(&currentEvent);

    if(currentEvent.type == SDL_QUIT) {

        quit = true;
    }

    if(currentEvent.type == SDL_KEYDOWN) {
        
        switch(currentEvent.key.keysym.sym) {
            case SDLK_UP:
                if(animationRunning)
                    if(rectangle.y>0)
                            rectangle.y -= padSpeed;
                    break;

            case SDLK_DOWN:
                if(animationRunning)
                    if(rectangle.y+rectangle.h<WINDOW_HEIGHT)
                        rectangle.y += padSpeed;
                break;

            case SDLK_LEFT:
                break;

            case SDLK_RIGHT:
                break;

            case SDLK_s:
                animationRunning = !animationRunning;
                break;

            case SDLK_r:
                initObjects();
                break;
            case SDLK_ESCAPE:

                quit = true;
                break;

            default:
                break;
        }
    }
}



void animationStep() {
  
    
    if ((cerc.center.x - cerc.radius + directionX * circleSpeed > rectangle.x + rectangle.w && cerc.center.x + cerc.radius + directionX * circleSpeed < WINDOW_WIDTH) && (cerc.center.y - cerc.radius > 0 && cerc.center.y + cerc.radius + directionY * circleSpeed < WINDOW_HEIGHT)) {
        cerc.center.x += directionX * circleSpeed;
        cerc.center.y += directionY * circleSpeed;
    }
    else
        if (cerc.center.x - cerc.radius + directionX * circleSpeed > rectangle.x + rectangle.w && cerc.center.x + cerc.radius + directionX * circleSpeed < WINDOW_WIDTH) {
            directionY = -directionY;
            cerc.center.x += directionX * circleSpeed;
            cerc.center.y += directionY * circleSpeed;
        }
        else
            if (cerc.center.y - cerc.radius > 0 && cerc.center.y + cerc.radius + directionY * circleSpeed < WINDOW_HEIGHT && cerc.center.x - cerc.radius + directionX * circleSpeed > rectangle.x + rectangle.w) {
                directionX = -directionX;
                cerc.center.x += directionX * circleSpeed;
                cerc.center.y += directionY * circleSpeed;
            }
            else if (cerc.center.x - cerc.radius + directionX * circleSpeed > 0 && (cerc.center.y - cerc.radius - directionY * circleSpeed > rectangle.y && cerc.center.y + cerc.radius + directionY * circleSpeed < rectangle.y + rectangle.h)) {
                directionX = -directionX;
                
                cerc.center.x += directionX * circleSpeed;
                cerc.center.y += directionY * circleSpeed;
                hits++;
                if (hits == 3) {
                    circleSpeed *= 2;
                    padSpeed *= 2;
                    hits = 0;
                }
            }
            else
                initObjects();
}

void drawFrame() {

    //Clear the background
    SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 0);
    SDL_RenderClear(windowRenderer);


    //Draw rectangle
    SDL_SetRenderDrawColor(windowRenderer, 255, 255,255, 255);
    SDL_RenderFillRect(windowRenderer, &rectangle);


    //Draw circle
    SDL_FillCircle(windowRenderer, cerc);

    if(animationRunning)
        animationStep();

    SDL_RenderPresent(windowRenderer);

}

void cleanup() {
    if (windowRenderer) {

        SDL_DestroyRenderer(windowRenderer);
        windowRenderer = NULL;
    }
    if (window) {

        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
}



int main(int argc, char * argv[]) {
    if(!initWindow()) {

        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }
    initObjects();
    while (!quit) {
        processEvents();
        drawFrame();
    }
    cleanup();
    return 0;
}