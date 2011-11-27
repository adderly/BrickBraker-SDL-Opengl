#include"Queue.h"
#include<iostream>
#define LEVEL_COUNT 4

int level[LEVEL_COUNT][2][13] =
{
    {
        {100,205,310,415,520,100,205,310,415,520,0,0,0},
        {92,92,92,92,92,126,126,126,126,126,0,0,0}
    },
    {
        {100,200,300,400,500,200,300,400,200,300,400,200,400},
        {92,92,92,92,92,126,126,126,160,160,160,194,194}
    },
    {
        {100,500,200,300,400,200,300,400,100,500,200,400,300},
        {92,91,126,126,126,160,160,160,194,194,194,194,194}
    }
};

queue::queue()
{
    first = 0;
    generateLevels();
    counter = 0;
}


Level* queue::Next()
{
    current = current->getNext();
    if(current == 0)return current;

    return current;
}

void queue::add(Level* level)
{
    if(first == 0)
    {
        realFirst = level;
        first = level;
        current = level;
        std::cout<<"nop";
    }
    else
    {

        first->setNext(level);
        first = level;
        first->setNext(0);
    }
    counter++;
    std::cout<<":S";

}

void queue::generateLevels()
{

    // BRICK bricks[15];
    int brickCounter = 0;

    for(int n = 0; n<LEVEL_COUNT; n++)
    {
        Level* lvl = new Level(counter+1);

        for(int n2 = 0; n2 < 13; n2++)
        {
            if(level[n][0][n2] != 0)
            {
                lvl->bricks[n2].x = level[n][0][n2];
                lvl->bricks[n2].y = level[n][1][n2];
                lvl->bricks[n2].height = 25;
                lvl->bricks[n2].width = 100;
                lvl->bricks[n2].active = true;
                brickCounter++;
            }
        }

        lvl->setAmount(brickCounter);
        add(lvl);
        brickCounter = 0;
    }


}

