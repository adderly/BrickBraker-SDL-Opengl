#include<iostream>
#include<string>
#include<sstream>
#include<SDL/SDL.h>
#include<SDL/SDL_opengl.h>
#include"SDL_image.h"
#include"gameInfo.cpp"
#include"Queue.h"

using namespace std;


void setupOpenGl(int width, int height);
void Draw(int &pX,int &pY,int pWidth,int pHeight,int bWidth,int bHeight,int &bX,int &bY);
void collissionCheck(int &pX,int &pY,int pWidth,int pHeight,int bWidth,int bHeight,int &bX,int &bY, int &bSpeedX,int &bSpeedY);
void collissionBrick(BRICK &brick, int bX,int bY,int bWidth,int bHeight, int &bSpeedX,int &bSpeedY);
void nextLevel();
GLuint loadTexture(const std::string &fileName);


queue* q;
Level* lvl;
bool newLevel = false;
bool endGame = false;
bool init = true;
bool lost = false;
bool paused = true;
unsigned int back = 0;
unsigned int playerbar = 0;
unsigned int ball = 0;
unsigned int bricktexture = 0;
unsigned int win = 0;
unsigned int loose = 0;
unsigned int next = 0;
unsigned int initexture = 0;

int main(int argc, char* argv[])
{
    extern int scrWidth;
    extern int scrHeight;
    q = new queue();
    lvl =  q->getCurrent();


    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout<<"Unable To Initialize The video, :(";
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,2);



    if(SDL_SetVideoMode(scrWidth,scrHeight,32,SDL_OPENGL) == 0)
    {
        std::cout<<"Unable To Set The Video Mode! Try Another Resolution";
        exit(1);
    }

    setupOpenGl(scrWidth,scrHeight);
    //move Variables
    bool left = false,right = false;
    //Player Block PX= player x-axis
    static int pHeight = 25;
    static int pWidth = 100;
    static int pX = (scrWidth-pWidth)/2;
    static int pY = 440;
    //player ball;
    static int bX = (scrWidth-pWidth)/2;
    static int bY = 200;
    static int bWidth = 25;
    static int bHeight = 25;
    // speed
    int bSpeedX = 3;
    int bSpeedY = 3;
    //Textures(images)
    back = loadTexture("images/background.png");
    playerbar = loadTexture("images/playerbar.png");
    win = loadTexture("images/win.png");
    loose = loadTexture("images/loose.png");
    bricktexture = loadTexture("images/brick.png");
    next = loadTexture("images/next.png");
    ball = loadTexture("images/ball.png");
    initexture = loadTexture("images/init.png");
    //main loop
    bool isRunning = true;
    SDL_Event event;

    while(isRunning)
    {
        //Event Handling
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) isRunning = false;

            else if(event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
            else if(event.key.keysym.sym == SDLK_RETURN)
            {
                init = false;
                paused = false;
            }
            else if(event.key.keysym.sym == SDLK_r)
            {
                q->restart();
                lvl = q->getCurrent();
                lost = false;
            }

            else if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_LEFT) left = true;
                else if(event.key.keysym.sym == SDLK_RIGHT) right = true;
            }
            else if(event.type == SDL_KEYUP)
            {
                if(event.key.keysym.sym == SDLK_LEFT) left = false;
                else if(event.key.keysym.sym == SDLK_RIGHT) right = false;
            }
        }

        //Printing The Title
        stringstream bPosX,bPosY;
        bPosX << bX;
        bPosY << bY;
        string s2 = bPosX.str()+" , "+bPosY.str();
        SDL_WM_SetCaption(s2.c_str(),NULL);
        //move direction Checking

        if(left) pX-=5;
        else if(right) pX+=5;

        //speeding the Ball
        if(!paused && !lost)
        {
            bX += bSpeedX;
            bY += bSpeedY;
        }

        //collission
        collissionCheck(pX,pY,pWidth,pHeight,bWidth,bHeight,bX,bY,bSpeedX,bSpeedY);
        for(int n = 0; n < lvl->brickAmount; n++)
        {
            if(lvl->bricks[n].active == true)
            {
                collissionBrick(lvl->bricks[n],bX,bY,bWidth,bHeight,bSpeedX,bSpeedY);
            }
        }
        //ball Constraints
        if(bY+bHeight > scrHeight)
        {
            bSpeedY = -bSpeedY;
            lost = true;
        }
        else if(bY < 0) bSpeedY = -bSpeedY;
        else if(bX+bWidth > scrWidth)bSpeedX = -bSpeedX;
        else if(bX < 0) bSpeedX = -bSpeedX;


        //-----------//


        //Drawing

        glClear(GL_COLOR_BUFFER_BIT);
        glPushMatrix();
        glColor4ub(255,255,255,255);
        glOrtho(0,680,460,0,-1,1);
		SDL_Delay(20);
        Draw(pX,pY,pWidth,pHeight,bWidth,bHeight,bX,bY);
        glPopMatrix();
        SDL_GL_SwapBuffers();

    }

    return 0;
}

void setupOpenGl(int width, int height)
{
    glViewport(0,0,width,height);
    glClearColor(1,1,1,1);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(0,width,height,0,-1,1);
    glDisable(GL_DEPTH_TEST);
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Draw(int &pX,int &pY,int pWidth,int pHeight,int bWidth,int bHeight,int &bX,int &bY)
{

    //background
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, back);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(0,0);
    glTexCoord2f(1,0);
    glVertex2f(680,0);
    glTexCoord2f(1,1);
    glVertex2f(680,460);
    glTexCoord2f(0,1);
    glVertex2f(0,460);
    glEnd();


    if(!endGame && !newLevel && !init && !lost)
    {
        //glColor4ub(255,255,255,255);
        if(pX+pWidth > 680) pX = 680 - pWidth;
        else if(pX<0) pX = 0;
        //Drwing the PlayerBar
        glBindTexture(GL_TEXTURE_2D, playerbar);
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2f(pX,pY);
        glTexCoord2f(1,0);
        glVertex2f(pX+pWidth,pY);
        glTexCoord2f(1,1);
        glVertex2f(pX+pWidth,pY+pHeight);
        glTexCoord2f(0,1);
        glVertex2f(pX,pY+pHeight);
        glEnd();
        //Drawing The Ball
        // glColor4ub(255,0,0,255);
        glBindTexture(GL_TEXTURE_2D, ball);
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2f(bX,bY);
        glTexCoord2f(1,0);
        glVertex2f(bX+bWidth,bY);
        glTexCoord2f(1,1);
        glVertex2f(bX+bWidth,bY+bHeight);
        glTexCoord2f(0,1);
        glVertex2f(bX,bY+bHeight);
        glEnd();

        //Bricks
        BRICK* bricks  = lvl->bricks;
        int deadCounter = 0;
        //glColor4ub(255,0,0,255);

        for(int n = 0; n< lvl->brickAmount; n++)
        {
            int x  = bricks[n].x;
            int y = bricks[n].y;
            int w = bricks[n].width;
            int h = bricks[n].height;
            bool active =  bricks[n].active;
            glBindTexture(GL_TEXTURE_2D, bricktexture);
            if(bricks[n].x != 0 && active == TRUE)
            {
                glBegin(GL_QUADS);
                glTexCoord2f(0,0);
                glVertex2f(x,y);
                glTexCoord2f(1,0);
                glVertex2f(x+w,y);
                glTexCoord2f(1,1);
                glVertex2f(x+w,y+h);
                glTexCoord2f(0,1);
                glVertex2f(x,y+h);
                glEnd();
            }
            else deadCounter++;
        }

        if(deadCounter == lvl->brickAmount)
        {
            nextLevel();
            newLevel = true;
            paused = true;
        }
    }
    else if(init)
    {
        glColor4ub(255,255,255,255);
        glBindTexture(GL_TEXTURE_2D, initexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2f(250,200);
        glTexCoord2f(1,0);
        glVertex2f(480,200);
        glTexCoord2f(1,1);
        glVertex2f(480,300);
        glTexCoord2f(0,1);
        glVertex2f(250,300);
        glEnd();
    }
    else if(lost)
    {
        glColor4ub(255,255,255,255);
        glBindTexture(GL_TEXTURE_2D, loose);
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2f(250,200);
        glTexCoord2f(1,0);
        glVertex2f(480,200);
        glTexCoord2f(1,1);
        glVertex2f(480,300);
        glTexCoord2f(0,1);
        glVertex2f(250,300);
        glEnd();
    }
    else if(newLevel)
    {
        static int count = 0;
        count++;

        //glColor4ub(0,255,0,255);
        glBindTexture(GL_TEXTURE_2D, next);
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2f(250,200);
        glTexCoord2f(1,0);
        glVertex2f(480,200);
        glTexCoord2f(1,1);
        glVertex2f(480,300);
        glTexCoord2f(0,1);
        glVertex2f(250,300);
        glEnd();

        if(count > 10)
        {
            newLevel = false;
            paused = false;
        }
        SDL_Delay(500);

    }
    else//end
    {
        // glColor4ub(100,100,100,100);
        glBindTexture(GL_TEXTURE_2D, win);
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2f(250,200);
        glTexCoord2f(1,0);
        glVertex2f(480,200);
        glTexCoord2f(1,1);
        glVertex2f(480,300);
        glTexCoord2f(0,1);
        glVertex2f(250,300);
        glEnd();
    }



}

void collissionCheck(int &pX,int &pY,int pWidth,int pHeight,int bWidth,int bHeight,int &bX,int &bY, int &bSpeedX,int &bSpeedY)
{
    if(bY+bHeight > pY && bX >= pX && bX+bWidth < pX+pWidth) bSpeedY = -bSpeedY;
    else if(bX+bHeight >= pX && bX+bHeight < pX+pWidth && bY+bHeight >= pY && bY+bHeight <= pY+pHeight){ bSpeedX = -bSpeedX;}
//   else if(bX+bHeight <= pX+pWidth && bX+bHeight > pY && bX+bHeight <= pY+pHeight){ bSpeedX = -bSpeedX;}
   // if(bY+bHeight > pY && bX+bWidth > pX && bX < pX+pWidth){bSpeedY = -bSpeedY;  }


}

void collissionBrick(BRICK &brick, int bX,int bY,int bWidth,int bHeight ,int &bSpeedX,int &bSpeedY)
{
    // if(bY <= brick.y+brick.height && bX <= brick.x+brick.width && bX >= brick.x && bY > brick.y) {brick.active = false; bSpeedY = -bSpeedY;}
    if(bY < brick.y+brick.height && bX < brick.x+brick.width && bX+bWidth > brick.x && bY > brick.y)
    {
        brick.active = false;
        bSpeedY = -bSpeedY;
    }
   else if(bY< brick.y && bX+bWidth > brick.x && bX < brick.x+brick.width && bY+bHeight > brick.y)
    {
        brick.active = false;
        bSpeedY = -bSpeedY;
    }
}

void nextLevel()
{
    if(q->Next() == NULL)
    {
        endGame = true;
    }
    else
    {
        newLevel = true;
        lvl = q->getCurrent();
    }
}

GLuint loadTexture(const std::string &fileName)
{
    SDL_Surface *image = IMG_Load(fileName.c_str());
    SDL_DisplayFormatAlpha(image);

    unsigned object(0);

    glGenTextures(1, &object);

    glBindTexture(GL_TEXTURE_2D, object);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h,0,GL_RGBA, GL_UNSIGNED_BYTE,image->pixels);

    //free surface
    SDL_FreeSurface(image);
    return object;

}
