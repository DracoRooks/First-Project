#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define W 1000
#define H 600
#define G 80
#define FPS 60
#define Size 100
#define Speed 400
#define Jump -1400
#define mar 10

int main(int argc, char* argv[]) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error Initiallising.\nError: %s\n", SDL_GetError());
        return 0;
    }

    SDL_Window* wnd = SDL_CreateWindow("Test2: Jumper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, 0);

    if(!wnd) {
        printf("Error creating window.\nError: %s", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(wnd, -1, render_flags);

    if(!rend) {
        printf("Error creating renderer.\nError: %s", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    bool running = true;
    bool jump_pressed = false, can_jump = true, left_pressed = false, right_pressed = false;
    float x_pos = (W-Size)/2, x_vel;
    float y_pos = (H-Size)/2, y_vel;

    SDL_Rect rect = {(int) x_pos, (int) y_pos, Size, Size};
    SDL_Rect bndry = {(int) x_pos, (int) y_pos, Size+mar, Size+mar};

    SDL_Event event;

    while(running) {
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_SPACE:
                            jump_pressed = true;
                            break;
                        case SDL_SCANCODE_A: case SDL_SCANCODE_LEFT:
                            left_pressed = true;
                            break;
                        case SDL_SCANCODE_D: case SDL_SCANCODE_RIGHT:
                            right_pressed = true;
                            break;
                        default: break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_SPACE:
                            jump_pressed = false;
                            break;
                        case SDL_SCANCODE_A: case SDL_SCANCODE_LEFT:
                            left_pressed = false;
                            break;
                        case SDL_SCANCODE_D: case SDL_SCANCODE_RIGHT:
                            right_pressed = false;
                            break;
                        default: break;
                    }
                    break;
                default: break;
            }
        }

        SDL_SetRenderDrawColor(rend, 120, 25, 255, 255);
        SDL_RenderClear(rend);

        x_vel = (right_pressed - left_pressed)*Speed;
        y_vel += G;

        if(jump_pressed && can_jump) {
            can_jump = false;
            y_vel = Jump;
        }

        x_pos += x_vel/60;
        y_pos += y_vel/60;

        if(x_pos <= 0) {
            x_pos = 0;
        }
        if(x_pos >= W - rect.w) {
            x_pos = W - rect.w;
        }
        if(y_pos <= 0) {
            y_pos = 0;
        }
        if(y_pos >= H - rect.h) {
            y_vel = 0;
            y_pos = H - rect.h;
            if(!jump_pressed) {
                can_jump = true;
            }
        }

        bndry.x = (int) x_pos;
        bndry.y = (int) y_pos;

        rect.x = (int) x_pos;
        rect.y = (int) y_pos;

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderFillRect(rend, &bndry);
        
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        SDL_RenderFillRect(rend, &rect);

        SDL_RenderPresent(rend);
        SDL_Delay(1000/FPS);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wnd);
    SDL_Quit();

    return 0;
}