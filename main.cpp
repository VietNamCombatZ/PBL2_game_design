#include <iostream>
#include <windows.h>
#include <string>
#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_mixer.h"
#include "include/SDL_ttf.h"
using namespace std;

#undef main

int WINDOW_WIDTH = 1200;
int WINDOW_HEIGHT = 800;

int main() {
	SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL không thể khởi tạo! SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    

    // Tạo cửa sổ
    window = SDL_CreateWindow("Chương trình SDL2 đơn giản",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Không thể tạo cửa sổ! SDL Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Tạo renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Không thể tạo renderer! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Vòng lặp sự kiện
    int quit = 0;
    SDL_Event e;

    while (!quit) {
        // Xử lý sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Tô màu nền thành màu đen
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        // Hiển thị mọi thứ lên màn hình
        SDL_RenderPresent(renderer);
    }

    // Dọn dẹp và thoát
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	cout << 12345;
	return 0;
}