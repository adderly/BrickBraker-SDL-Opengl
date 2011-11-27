#define AMOUNT 3

typedef struct BRICK
{
    int x;
    int y;
    int width;
    int height;
    bool active;
};




class Level
{
    Level* next;
    Level* previous;
    int levelNumber;

public:
    int brickAmount;
    BRICK bricks[13];
    Level(int levelnum)
    {
        levelNumber = levelnum;
    }
    int getLevelNumber()
    {
        return levelNumber;
    }
    //BRICK* getLevelBricks(){ return levelBricks;}
    //void setLevelBricks(BRICK bricks[]){ levelBricks = bricks; }
    void setAmount(int amount)
    {
        brickAmount = amount;
    }
    int getAmout()
    {
        return brickAmount;
    }
    Level* getNext()
    {
        return next;
    }
    Level* getPrevious()
    {
        return previous;
    }
    void setNext(Level* level)
    {
        next = level;
    }
    void setPrevious(Level* level)
    {
        previous = level;
    }

};


