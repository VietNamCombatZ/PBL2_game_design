#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_mixer.h"
#include "include/SDL_ttf.h"
#include "item.h"
#include "item.cpp"

#undef main

SDL_Surface* loadImage(const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        printf("Error loading image: %s\n", IMG_GetError());
    }
    return surface;
}

void drawCharacter(SDL_Surface* screen, SDL_Surface* fontSheet, char character, int x, int y, int charWidth, int charHeight) {
    int charIndex = character - ' '; // Bắt đầu từ ký tự khoảng trắng
    SDL_Rect srcRect = { charIndex*charWidth, charIndex*charHeight, charWidth, charHeight };
    SDL_Rect destRect = { x, y, charWidth, charHeight };
    SDL_BlitSurface(fontSheet, &srcRect, screen, &destRect);
}

void drawString(SDL_Surface* screen, SDL_Surface* fontSheet, const char* string, int x, int y, int charWidth, int charHeight) {
    int offsetX = 0;
    while (*string) {
        drawCharacter(screen, fontSheet, *string, x + offsetX, y, charWidth, charHeight);
        offsetX += charWidth;
        string++;
    }
}


int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL2 Text", 30,30,800,600,SDL_WINDOW_SHOWN);
    SDL_Renderer *screen = SDL_CreateRenderer(window,-1,0);
    SDL_Surface* screens = SDL_GetWindowSurface(window);
    SDL_SetRenderDrawColor(screen,30,180,20,255);
    
    item bob;
    bob.setRenderer(screen);
    bob.setPos(200,200);
    bob.setSize(100,100);
    bob.loadImage("Font.png");
    SDL_Surface* fontSheet = loadImage("FontBlock.png"); // Đường dẫn đến hình ảnh chứa ký tự

    // Các thông số cho ký tự
    int charWidth = 16; // Chiều rộng ký tự
    int charHeight = 16; // Chiều cao ký tự
    int charsPerRow = 16; // Số ký tự trên mỗi hàng

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }
        SDL_RenderClear(screen);
        bob.draw();

        // SDL_FillRect(screens, NULL, SDL_MapRGB(screens->format, 0, 0, 0)); // Xóa màn hình

        // drawString(screens, fontSheet, "Hello, SDL!", 50, 50, charWidth, charHeight); // Vẽ chữ
        renderText(screen,bob.getImage()," Hello SDL",100,100,charWidth, charHeight);

        SDL_RenderPresent(screen);
    }

    SDL_FreeSurface(fontSheet);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
