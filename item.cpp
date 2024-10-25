#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <string>
#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_mixer.h"
#include "include/SDL_ttf.h"
#include "item.h"

void itemInit(){
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
}

void itemQuit(){
    SDL_Quit();
    IMG_Quit();
}

item::item(){
    pos.x = 100;
    pos.y = 100;
    image = NULL;
    pos.w = 100;
    pos.h = 100;
    oldTick = 0;
}

item::~item(){
    if(image!=NULL){
        SDL_DestroyTexture(image);
        image = NULL;
    }
}

void item::setRenderer(SDL_Renderer *dest){
    ren = dest;
}

bool item::loadImage(std::string filename){
    SDL_Surface *temp = IMG_Load(filename.c_str());
    if(temp!=NULL){
        image = SDL_CreateTextureFromSurface(ren,temp);
        SDL_FreeSurface(temp);
        if(image != NULL){
            return true;
        }
    }
    return false;
}

void item::freeImage(){
    if(image!=NULL){
        SDL_Texture *del = image;
        SDL_DestroyTexture(del);
        image = NULL;
    }
}

void item::setSize(int w, int h){
    pos.w = w;
    pos.h = h;
}

void item::setPos(int x, int y){
    pos.x = x;
    pos.y = y;
}

SDL_Rect* item::getPos(){
    return &pos;
}

void item::move(int x, int y){
    pos.x += x;
    pos.y += y;
}

bool item::getCollision(item* other){
    int dx, dy, rs;
    dx = pos.x + center.x - (other->getPos()->x + other->getCenter().x);
    dy = pos.y + center.y - (other->getPos()->y + other->getCenter().y);
    rs = center.r + other->getCenter().r;
    dx *= dx;
    dy *= dy;
    rs *= rs;
    if(dx+dy<rs){
        return true;
    }
    else{
        return false;
    }
}

bool item::isClicked(int x, int y){
    int dx, dy, rs;
    dx = pos.x + center.x - x;
    dy = pos.y + center.y - y;
    rs = center.r;
    dx *= dx;
    dy *= dy;
    rs *= rs;
    return(dx + dy < rs);
}

circle item::getCenter(){
    return center;
}

void item::setCenter(int x, int y, int r){
    center.x = x;
    center.y = y;
    center.r = r;
}

void item::draw(double angle){
    if(image!=NULL){
        SDL_Point pt;
        // pt.x = pos.w/2;
        // pt.y = pos.h/2;
        // pt.x += pos.x;
        // pt.y += pos.y;
        SDL_RenderCopyEx(ren,image,NULL,&pos,angle,NULL,SDL_FLIP_NONE);
    }
    else{
        std::cout << "Help, image is NULL at draw()" << std::endl;
    }
}

void item::draw(){
    if(image!=NULL){
        SDL_RenderCopy(ren,image,NULL,&pos);
    }
    else{
        std::cout << "Help, image is NULL at draw()" << std::endl;
    }
}

void item::drawFlip(){
    if(image!=NULL){
        SDL_RenderCopyEx(ren,image,NULL,&pos,0,NULL,SDL_FLIP_HORIZONTAL);
    }
    else{
        std::cout << "Help, image is NULL at draw()" << std::endl;
    }
}

void item::update(int tick){
    oldTick = tick;
}

SDL_Texture* item::getImage(){
    return this->image;
}

/////// CLASS ANIMATION

animation::animation(){

}

animation::~animation(){
    this->freeImages();
}

bool animation::addImage(std::string filepath){
    if(loadImage(filepath)){
        images.push_back(image);
        return true;
    }
    return false;
}

bool animation::loadAnimation(std::string p, std::string zb, std::string ext){
    int leng = zb.length() - 1;
    while(addImage(p + zb + ext) and !(zb[0]>'9')){
        zb[leng] ++;
        for(int i=leng; i>0; i--){
            if(zb[i] > '9'){
                zb[i-1] ++;
                zb[i] = '0';
            }
        }
    }

    if(!(images.empty())){
        return true;
    } else{
        image = NULL;
        std::cout << "Failed to load Animation: " << p << std::endl;
    }
    return false;
}

void animation::next(){
    if(images.empty()){
        std::cout << "Tried to update an empty animation" << std::endl;
    }
    else{
        frameCount++;
        image = images[frameCount % (images.size()-1)];
    }
}

void animation::freeImages(){
    for(int i=0; i<images.size(); i++){
        if(images[i]!=NULL){
            SDL_Texture *del = images[i];
            SDL_DestroyTexture(del);
            images[i] = NULL;
        }
    }
    images.clear();
}

void animation::setFPS(int FPS){
    desiredDelta = 1000 / FPS;
}

void animation::update(int tick){
    if((tick-oldTick) > desiredDelta){
        next();
        oldTick = tick;
    }
}

/////// CLASS PLAT

plat::plat(){
    leftBorder = 0;
    rightBorder = 100;
    topBorder = 0;
    lowBorder = 100;
}

plat::~plat(){

}

void plat::setBorder(int left, int right, int top, int low){
    leftBorder = left;
    rightBorder = right;
    topBorder = top;
    lowBorder = low;
}

bool plat::getCollision(animation* other){
    int leftC, rightC, topC, lowC;
    leftC = pos.x + leftBorder + 50 - (other->getPos()->x + other->getPos()->w);
    rightC = other->getPos()->x - (pos.x + rightBorder) + 50;
    topC = (pos.y + topBorder) - (other->getPos()->y + other->getPos()->h) + 14;
    lowC = other->getPos()->y - (pos.y + lowBorder) + 50;
    if((leftC<=0 and rightC>0) or (rightC<=0 and leftC>0) or (lowC<=0 and topC>0) or (topC<=0 and lowC>0)){
        return false;
    }
    else{
        return true;
    }
}

// int dx, dy, rs;
// dx = pos.x + center.x - (other->getPos()->x + other->getCenter().x);
// dy = pos.y + center.y - (other->getPos()->y + other->getCenter().y);
// rs = center.r + other->getCenter().r;
// dx *= dx;
// dy *= dy;
// rs *= rs;
// if(dx+dy<rs){
//     return true;
// }
// else{
//     return false;
// }