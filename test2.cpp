#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <string.h>
#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_mixer.h"
#include "include/SDL_ttf.h"
#include "item.h"
#include "item.cpp"

#undef main

int main(int argc, char** arg){
    srand(time(NULL));
    itemInit();
    TTF_Init();
    SDL_Window *win = SDL_CreateWindow("demo",30,30,900,600,SDL_WINDOW_SHOWN);
    SDL_SetWindowResizable(win,SDL_TRUE);
    SDL_SetWindowOpacity(win,1);
    SDL_Renderer *screen = SDL_CreateRenderer(win,-1,0);

    item bkground;
    bkground.setRenderer(screen);
    bkground.loadImage("bkground.png");
    bkground.setPos(0,0);
    bkground.setSize(900,600);
    bkground.setCenter(0,0,0);
    
    animation bob;
    bob.setFPS(7);
    bob.setRenderer(screen);
    bob.loadAnimation("Ninja png/5x/idle_","0",".png");
    bob.setPos(300,550);
    bob.setSize(100,100);
    bob.setCenter(43,68,27);

    std::vector <item*> gems;
    int maxGems = 200;
    for(int i=0; i<maxGems; i++){
        gems.push_back(new item());
        gems[i]->setRenderer(screen);
        gems[i]->loadImage("crystal.png");
        gems[i]->setPos(rand()%550, rand()%400);
        gems[i]->setSize(50,50);
        gems[i]->setCenter(24,24,8);
    }

    TTF_Font *font = TTF_OpenFont("acme.ttf",48);

    plat plat_pre;
    plat plat_after;
    plat_pre.setRenderer(screen);
    plat_after.setRenderer(screen);
    plat_pre.loadImage("platform.png");
    plat_after.loadImage("platform.png");
    plat_pre.setSize(150,100);
    plat_after.setSize(150,100);
    plat_pre.setBorder(0,150,0,100);
    plat_after.setBorder(0,150,0,100);
    plat_pre.setPos(500,300);
    plat_after.setPos(100,100);

    animation box;
    box.setFPS(10);
    box.setRenderer(screen);
    box.setSize(40,40);
    box.setPos(390,40);
    box.setCenter(20,20,20);
    std::string path = "star coin animation Pack/star coin shine/star coin ";
    std::string count = "1";
    std::string ext = ".png";
    for(int i=0; i<7; i++){
        count[0] += 1;
        if(count[0]>'6'){
            count[0] = '1';
        }
        box.addImage(path + count + ext);
    }

    int checkChangeState = 0;
    int characterState = 1; //1-idle, 2-run, 3-jump_up, 4-jump_down
    int checkFlip = 0;
    bool boxHidden = false;
    bool run = true;

    int ground = 550;       // nền
    int leftWall = 0;       // tường trái
    int rightWall = 900;    // tường phải
    float jumpStr = 16.7f/1.3;
    float speedX = 0;
    float speedY = 0;
    float gravity = 0.3f;   // trọng lực
    float friction = 0.95f;  // ma sát
    float runStr = 5.0f;
    
    int left = 0;
    int right = 0;
    int up = 0;
    int onGround = 0;
    int touchLeftWall = 0;
    int touchRightWall = 0;

    // SDL_SetRenderDrawColor(screen,30,180,20,255);
    int score = 0;
    int play = 0;
    int end = 0;
    int x_distance = 400;   //x-distance maximum between 2 left edges of 2 platform
    int y_distance = 200;   //y-distance between 2 top edges of 2 platform

    double ang = 0;
    int fps = 120;
    int desiredDelta = 1000/fps;
    while(run){
        int startLoop = SDL_GetTicks();
        SDL_Event e;
        checkChangeState = 0;
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    run = false;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(box.isClicked(e.button.x,e.button.y)){
                        boxHidden = !boxHidden;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        
                        case SDLK_UP:
                            if(characterState != 3){
                                characterState = 3;
                                checkChangeState = 1;
                            }
                            if(!play){
                                play = 1;
                            }
                            up = 1;
                            break;
                        case SDLK_DOWN:
                            if(characterState != 4){
                                characterState = 4;
                                checkChangeState = 1;
                            }
                            up = -1;
                            break;
                        case SDLK_LEFT:
                            if(characterState != 2){
                                characterState = 2;
                                checkChangeState = 1;
                            }
                            checkFlip = 1;
                            left = 1;
                            right = 0;
                            break;
                        case SDLK_RIGHT:
                            if(characterState != 2){
                                characterState = 2;
                                checkChangeState = 1;
                            }
                            checkFlip = 0;
                            right = 1;
                            left = 0;
                            break;
                        case SDLK_q:
                            run = false;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(e.key.keysym.sym){
                        case SDLK_UP:
                            if(characterState != 1){
                                characterState = 1;
                                checkChangeState = 1;
                            }
                            up = 0;
                            break;
                        case SDLK_DOWN:
                            if(characterState != 1){
                                characterState = 1;
                                checkChangeState = 1;
                            }
                            up = 0;
                            break;
                        case SDLK_LEFT:
                            if(characterState != 1){
                                characterState = 1;
                                checkChangeState = 1;
                            }
                            checkFlip = 1;
                            left = 0;
                            break;
                        case SDLK_RIGHT:
                            if(characterState != 1){
                                characterState = 1;
                                checkChangeState = 1;
                            }
                            checkFlip = 0;
                            right = 0;
                            break;
                    }
                    break;
            }
        }
        if(checkChangeState){
            switch(characterState){
                case 1:
                    bob.freeImages();
                    bob.loadAnimation("Ninja png/5x/idle_","0",".png");
                    break;
                case 2:
                    bob.freeImages();
                    bob.loadAnimation("Ninja png/5x/run_","0",".png");
                    break;
                case 3:
                    bob.freeImages();
                    bob.loadAnimation("Ninja png/5x/jump-up_","0",".png");
                    break;
                case 4:
                    bob.freeImages();
                    bob.loadAnimation("Ninja png/5x/jump-down_","0",".png");
                    break;
                default:
                    bob.freeImages();
                    bob.loadAnimation("Ninja png/5x/idle_","0",".png");
            }
        }
        speedX = (right * runStr) - (left * runStr);
        speedY -= (onGround * up * jumpStr) - gravity;
        bob.move(speedX*friction,speedY);

        if(bob.getPos()->y + bob.getPos()->h >= ground){
            speedY = 0;
            bob.getPos()->y = ground - bob.getPos()->h;
            onGround = 1;
        }
        else{
            onGround = 0;
        }

        if(bob.getPos()->x <= leftWall){
            left = 0;
            bob.getPos()->x = leftWall;
            touchLeftWall = 1;
        }
        else{
            touchLeftWall = 0;
        }

        if(bob.getPos()->x + bob.getPos()->w >= rightWall){
            right = 0;
            bob.getPos()->x = rightWall - bob.getPos()->w;
            touchRightWall = 1;
        }
        else{
            touchRightWall = 0;
        }

        if(play){
            ground = 700;
            if(bob.getPos()->y + bob.getPos()->h >= ground){
                end = 1;
            }
        }

        if(end){
            bob.setPos(300,550);
            ground = 500;
            end = 0;
            play = 0;
        }

        if(plat_pre.getCollision(&bob)){
            speedY = 0;
            bob.getPos()->y = plat_pre.getPos()->y - bob.getPos()->h + 14;
            onGround = 1;
        }

        if(plat_after.getCollision(&bob)){
            speedY = 0;
            bob.getPos()->y = plat_after.getPos()->y - bob.getPos()->h + 14;
            onGround = 1;
        }

        SDL_RenderClear(screen);
        bkground.draw();
        plat_after.draw();
        plat_pre.draw();
        // SDL_RenderDrawLine(screen,0,485,900,485);
        if(checkFlip==1)
            bob.drawFlip();
        else
            bob.draw();
        
        if(!boxHidden){
            box.draw();
        }
        else{
            score += 1;
        }
        for(int i=0; i<maxGems; i++){
            if(bob.getCollision(gems[i])){
                gems.erase(gems.begin()+i);
                score++;
                maxGems--;
            }
            else{
                gems[i]->draw(ang);
            }
        }

        std::string s = "Score: " + std::to_string(score);
        SDL_Surface *textSurf = TTF_RenderText_Solid(font, s.c_str(), {0,180,180});
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(screen,textSurf);
        SDL_Rect textRect = {430,30,textSurf->w,textSurf->h};
        SDL_FreeSurface(textSurf);
        SDL_RenderCopy(screen,textTexture,NULL,&textRect);

        SDL_RenderPresent(screen);
        ang++;
        
        box.update(startLoop);
        bob.update(startLoop);

        int delta = SDL_GetTicks() - startLoop;
        if(delta < desiredDelta){
            SDL_Delay(desiredDelta-delta);
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyWindow(win);
    itemQuit();
    TTF_Quit();
    return 0;
}