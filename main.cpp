#include <iostream>
#include <SDL.h>
#include "circle.h"

//define window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

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

//Direction vector
glm::vec2 movementDirection;


float circleSpeed=0.5f;
///time, needs to be big due to elapsedTime in the movement of the circle
float dt = 1000.0f;


float elapsedTime = 0.0f;


bool initWindow() {

    bool success = true;

    //Try to initialize SDL
    /// fails if os can't do windows
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {

        std::cout << "SDL initialization failed" << std::endl;
        success = false;

    } else {

        //Try to create the window
        window = SDL_CreateWindow(
                "SDL Hello Window Example",
                ///places the windows wherever the os feels "right"
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                ///defined up up
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                ///shows the actual window, if not it's hidden
                SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

        if(window == NULL) {

            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;

        } else {

            // Create a renderer for the current window for drawing
            // fereastra prim parametru, last is must
            windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if(windowRenderer == NULL) {

                std::cout << "Failed to create the renderer: " << SDL_GetError() << std::endl;
                success = false;

            } else {
                //un fel de bckr clear

                //Set background color
                //set what color it will be used on first parameter
                SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);

                //Apply background color
                
                SDL_RenderClear(windowRenderer);
            }
        }
    }

    return success;
}

void processEvents() {

    //Check for events in queue
    //SDL_WaitEvent(..)Instructiune blocanta, doar event continua executia
    SDL_PollEvent(&currentEvent);

    //User requests quit
    //Clicking the close button
    if(currentEvent.type == SDL_QUIT) {

        quit = true;
    }

    //Mouse event -> pressed button
    if(currentEvent.type == SDL_MOUSEBUTTONDOWN) {

        if(currentEvent.button.button == SDL_BUTTON_LEFT) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse left click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }
        if (currentEvent.button.button == SDL_BUTTON_RIGHT) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse right click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }
    }


    //Mouse event -> mouse movement
    if(currentEvent.type == SDL_MOUSEMOTION) {
        //actualizare mouseX, mouseY constant
        SDL_GetMouseState(&mouseX, &mouseY);


        ///SDL_BUTTON_LMASK => left click pressed + SDL_GETMouseState=> mouse moved
        if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse move while left click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse move while right click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }
    }

    ///BUTTON_LEFT != BUTTON_LMASK


    //Keyboard event
    if(currentEvent.type == SDL_KEYDOWN) {

        switch(currentEvent.key.keysym.sym) {
            ///SDLK_name basically for every key
            case SDLK_UP:
                break;

            case SDLK_DOWN:
                break;

            case SDLK_LEFT:
                break;

            case SDLK_RIGHT:
                break;

            case SDLK_r:
                animationRunning = !animationRunning;
                break;

            case SDLK_s:
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
    //if (goingRight) {
       // cerc.center.x += 0.1f;
      //  if (cerc.center.x > 400.0f) {
      //      goingRight = false;
     //   }
   // }
   // else {
    //    cerc.center.x -= 0.1f;
    //    if (cerc.center.x < 100.0f) {
    //        goingRight = true;
     //    }
   // }
    ///vec2 is 2D vector
    glm::vec2 mousePos = glm::vec2(mouseX, mouseY);
    //diferenta de locatie intre 2 vectori
    movementDirection = glm::normalize(mousePos - cerc.center);

    ///physics formula yay direction*speed*time
    //elapsedTime is just to slow down the movement of the circle to be constant on all devices
    cerc.center += movementDirection * circleSpeed * dt * elapsedTime;
    
    

    SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(windowRenderer, mouseX, mouseY, cerc.center.x, cerc.center.y);

}

void drawFrame() {
    ///For having the same "speed" on any computer
    Uint32 start = SDL_GetTicks();

    //Clear the background
    SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
    SDL_RenderClear(windowRenderer);

    //Draw scene
    //TODO

    //Draw line
    SDL_SetRenderDrawColor(windowRenderer, 255, 0, 255, 255);
    SDL_RenderDrawLine(windowRenderer, 100, 100, 500, 100);

    //Draw rectangle
    SDL_SetRenderDrawColor(windowRenderer, 255, 0, 0, 255);
    ///Fill for filled and Draw for line only
    SDL_RenderFillRect(windowRenderer, &rectangle);


    //Draw circle
    //!!Function defined by user
    SDL_FillCircle(windowRenderer, cerc);

    if(animationRunning)
        animationStep();

    //Update window
    SDL_RenderPresent(windowRenderer);

    Uint32 end = SDL_GetTicks();
    elapsedTime = (end - start) / 1000.0f;
}

void cleanup() {

    //Destroy renderer
    if (windowRenderer) {

        SDL_DestroyRenderer(windowRenderer);
        windowRenderer = NULL;
    }

    //Destroy window
    if (window) {

        SDL_DestroyWindow(window);
        window = NULL;
    }

    //Quit SDL
    SDL_Quit();
}

void initObjects() {
    //init rectangle
    rectangle.x = 100;
    rectangle.y = 150;
    rectangle.w = 200;
    rectangle.h = 100;

    //init circle
    cerc.radius = 30.0f;
    cerc.center.x = 300.0f;
    cerc.center.y = 300.0f;
    cerc.color = glm::vec4(100, 2, 50, 255);
}

int main(int argc, char * argv[]) {
    //Initialize window
    if(!initWindow()) {

        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }
    initObjects();
    //Game loop
    //loop for the actual frame
    //the actual fps = x*times the loop executesw
    while (!quit) {

        //in each loop it checks the events and executes
        processEvents();
        drawFrame();
    }
    ///tidy exit, free resources etc
    cleanup();
    return 0;
}