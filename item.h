#ifndef IMAGESANDITEMS
#define IMAGESANDITEMS

#include <string>
#include <vector>
#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_mixer.h"
#include "include/SDL_ttf.h"

void itemInit();
void itemQuit();

struct circle{
    int x, y, r;
};

class item{
    public:
        item();
        virtual ~item();
        void setRenderer(SDL_Renderer *dest);
        bool loadImage(std::string filename);
        void freeImage();
        void setSize(int w, int h);
        void setPos(int x, int y);
        SDL_Rect* getPos();
        SDL_Texture* getImage();
        void move(int x, int y);
        virtual bool getCollision(item* other);
        bool isClicked(int x, int y);
        circle getCenter();
        void setCenter(int x, int y, int r);
        void draw(double angle);
        void draw();
        void drawFlip();
        virtual void update(int tick);
    protected:
        SDL_Renderer *ren;
        SDL_Texture *image;
        SDL_Rect pos;
        circle center;
        int oldTick;
};

class animation : public item{
    public:
        animation();
        virtual ~animation();
        bool addImage(std::string filePath);
        bool loadAnimation(std::string pref, std::string zbuf, std::string ext);
        void next();
        void freeImages();
        void setFPS(int FPS);
        virtual void update(int tick);
    protected:
        int frameCount;
        std::vector <SDL_Texture*> images;
        int desiredDelta;
};

class plat : public item{
    public:
        plat();
        virtual ~plat();
        void setBorder(int left, int right, int top, int low);
        virtual bool getCollision(animation* other);
    protected:
        int leftBorder;
        int rightBorder;
        int topBorder;
        int lowBorder;
};

#endif