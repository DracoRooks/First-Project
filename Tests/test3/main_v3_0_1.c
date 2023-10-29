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

int doInitCheck(SDL_Window* wnd, SDL_Renderer* rend);
void setEventCalls(SDL_Event* event, bool* run, bool* l, bool* r, bool* u, bool* d);
void setKeyDown(SDL_Event* event, bool* l, bool* r, bool* u, bool* d);
void setKeyUp(SDL_Event* event, bool* l, bool* r, bool* u, bool* d);
void setMouseButtonDown(SDL_Event* event, bool* l, bool* r, bool *u, bool* d);
void setMouseButtonUp(SDL_Event* event, bool* l, bool* r);
void setMouseWheel(SDL_Event* event, bool* u, bool* d);
void doDestroyEverything(SDL_Window* wnd, SDL_Renderer* rend);

int main(int argc, char* argv[]) {
    SDL_Window* wnd = SDL_CreateWindow("Test3: Wanderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, 0);
    Uint32 render_flag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(wnd, -1, render_flag);

    int* check = (int*)malloc(sizeof(int));
    *check = doInitCheck(wnd, rend);
    if(*check == 1) {
        return 0;
    }
    free(check);

    bool running = true;
    bool left_pressed = false, right_pressed = false, up_pressed = false, down_pressed = false;
    int l=0, r=0;
    float x_pos = (Width - Size)/2, x_vel;
    float y_pos = (Height - Size)/2, y_vel;
    
    SDL_Rect player = {(int) x_pos, (int) y_pos, Size, Size};
    SDL_Rect shadow = {(int) x_pos - 1, (int) y_pos - 1, Size + mar, Size + mar};
    
    SDL_Event event;

    while(running) {
        
        setEventCalls(&event, &running, &left_pressed, &right_pressed, &up_pressed, &down_pressed);

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

    doDestroyEverything(wnd, rend);
    return 0;
}

int doInitCheck(SDL_Window* wnd, SDL_Renderer* rend) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initialising SDL.\nError: %s", SDL_GetError());
        return 1;
    }
    if(!wnd) {
        printf("Error creating window.\nError: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if(!rend) {
        printf("Error creating renderer.\nError: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
}

void setEventCalls(SDL_Event* event, bool* run, bool* l, bool* r, bool* u, bool* d) {
    while(!SDL_PollEvent(event)) {
        return;
    }
    switch(event -> type) {
        case SDL_QUIT:
            *run = false;
            break;
        case SDL_KEYDOWN: {
            setKeyDown(event, l, r, u, d);
            break;
        }
        case SDL_KEYUP: {
            setKeyUp(event, l, r, u, d);
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            setMouseButtonDown(event, l, r, u, d);
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            setMouseButtonUp(event, l, r);
            break;
        }
        case SDL_MOUSEWHEEL: {
            setMouseWheel(event, u, d);
            break;
        }
        default: break;
    }
}

void setKeyDown(SDL_Event* event, bool* l, bool* r, bool* u, bool* d) {
    switch(event -> key.keysym.scancode) {
        case SDL_SCANCODE_LEFT: case SDL_SCANCODE_A:
            *l = true;
            return;
        case SDL_SCANCODE_RIGHT: case SDL_SCANCODE_D:
            *r = true;
            return;
        case SDL_SCANCODE_DOWN: case SDL_SCANCODE_S:
            *d = true;
            return;
        case SDL_SCANCODE_UP: case SDL_SCANCODE_W:
            *u = true;
            return;
        default: return;
    }
}

void setKeyUp(SDL_Event* event, bool* l, bool* r, bool* u, bool* d) {
    switch(event -> key.keysym.scancode) {
        case SDL_SCANCODE_LEFT: case SDL_SCANCODE_A:
            *l = false;
            return;
        case SDL_SCANCODE_RIGHT: case SDL_SCANCODE_D:
            *r = false;
            return;
        case SDL_SCANCODE_DOWN: case SDL_SCANCODE_S:
            *d = false;
            return;
        case SDL_SCANCODE_UP: case SDL_SCANCODE_W:
            *u = false;
            return;
        default: return;
    }
}

void setMouseButtonDown(SDL_Event* event, bool* l, bool* r, bool *u, bool* d) {
    switch(event -> button.button) {
        case SDL_BUTTON_LEFT:
            *l = true;
            return;
        case SDL_BUTTON_RIGHT:
            *r = true;
            return;
        default: return;
    }
}

void setMouseButtonUp(SDL_Event* event, bool* l, bool* r) {
    switch(event -> button.button) {
        case SDL_BUTTON_LEFT:
            *l = false;
            return;
        case SDL_BUTTON_RIGHT:
            *r = false;
            return;
        default: return;
    }
}

void setMouseWheel(SDL_Event* event, bool* u, bool* d) {
    if(event -> wheel.y > 0) {
        *u = true;
        return;
    }
    if(event -> wheel.y < 0) {
        *d = true;
        return;
    }
    if(event -> wheel.y == 0) {
        *d = false;
        *u = false;
        return;
    }
}

void doDestroyEverything(SDL_Window* wnd, SDL_Renderer* rend) {
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wnd);
    SDL_Quit();
}