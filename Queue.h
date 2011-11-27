#include"Level.h"


class queue
{
    Level* first;
    Level* realFirst;
    Level* current;
    int counter;
public:
    BRICK array[];
    queue();
    void restart()
    {
        current = realFirst;
    }
    Level* Next();
    void add(Level* level);
    void generateLevels();
    void setFirst(Level* level)
    {
        first = level;
    }
    Level* getCurrent()
    {
        return current;
    }
    Level* getFirst()
    {
        return first;
    }

};
