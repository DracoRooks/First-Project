#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define Width 1000
#define Height 750
#define FPS 60
#define Size 25
#define Speed 200
#define Dash 500
#define mar 8

int main(int argc, char* argv[]) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initialising SDL.\nError: %s", SDL_GetError());
        return 0;
    }

    SDL_Window* wnd = SDL_CreateWindow("Test3: Wanderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, 0);
    
    if(!wnd) {
        printf("Error creating window.\nError: %s", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    
    Uint32 render_flag = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(wnd, -1, render_flag);
    
    if(!rend) {
        printf("Error creating renderer.\nError: %s", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    bool running = true;
    bool left_pressed = false, right_pressed = false, up_pressed = false, down_pressed = false;
    float x_pos = (Width - Size)/2, x_vel;
    float y_pos = (Height - Size)/2, y_vel;
    
    SDL_Rect player = {(int) x_pos, (int) y_pos, Size, Size};
    SDL_Rect shadow = {(int) x_pos - 1, (int) y_pos - 1, Size + mar, Size + mar};
    
    SDL_Event event;

    while(running) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_LEFT: case SDL_SCANCODE_A:
                            left_pressed = true;
                            break;
                        case SDL_SCANCODE_RIGHT: case SDL_SCANCODE_D:
                            right_pressed = true;
                            break;
                        case SDL_SCANCODE_DOWN: case SDL_SCANCODE_S:
                            down_pressed = true;
                            break;
                        case SDL_SCANCODE_UP: case SDL_SCANCODE_W:
                            up_pressed = true;
                            break;
                        default: break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_LEFT: case SDL_SCANCODE_A:
                            left_pressed = false;
                            break;
                        case SDL_SCANCODE_RIGHT: case SDL_SCANCODE_D:
                            right_pressed = false;
                            break;
                        case SDL_SCANCODE_DOWN: case SDL_SCANCODE_S:
                            down_pressed = false;
                            break;
                        case SDL_SCANCODE_UP: case SDL_SCANCODE_W:
                            up_pressed = false;
                            break;
                        default: break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch(event.button.button) {
                        case SDL_BUTTON_LEFT:
                            left_pressed = true;
                            break;
                        case SDL_BUTTON_RIGHT:
                            right_pressed = true;
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    switch(event.button.button) {
                        case SDL_BUTTON_LEFT:
                            left_pressed = false;
                            break;
                        case SDL_BUTTON_RIGHT:
                            right_pressed = false;
                            break;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if(event.wheel.y > 0) {
                        up_pressed = true;
                        break;
                    }
                    if(event.wheel.y < 0) {
                        down_pressed = true;
                        break;
                    }
                    if(event.wheel.y == 0) {
                        down_pressed = false;
                        up_pressed = false;
                        break;
                    }
                    break;
                default: break;
            }
        }
        SDL_SetRenderDrawColor(rend, 160, 25, 255, 255);
        SDL_RenderClear(rend);

        x_vel = (right_pressed - left_pressed)*Speed;
        y_vel = (down_pressed - up_pressed)*Speed;

        x_pos += x_vel/60;
        y_pos += y_vel/60;

        if(x_pos <= 0) {
            x_pos = 0;
        }
        if(x_pos >= Width - player.w) {
            x_pos = Width - player.w;
        }

        if(y_pos <= 0) {
            y_pos = 0;
        }
        if(y_pos >= Height - player.h) {
            y_pos = Height - player.h;
        }

        shadow.x = (int) (x_pos - 2);
        shadow.y = (int) (y_pos - 2);

        player.x = (int) x_pos;
        player.y = (int) y_pos;

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 150);
        SDL_RenderFillRect(rend, &shadow);

        SDL_SetRenderDrawColor(rend, 180, 255, 180, 255);
        SDL_RenderFillRect(rend, &player);
    
        SDL_RenderPresent(rend);
        SDL_Delay(1000/FPS);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wnd);
    SDL_Quit();

    return 0;
}