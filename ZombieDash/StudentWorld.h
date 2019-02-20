#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Player;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void setPenelope(Actor* p);
    void addActor(Actor* p);
    Actor* getActorAt(int x, int y);
    void loadLevel(int level);
    bool doesOverlap(int x, int y);

private:
    std::vector<Actor*> m_actors;
    Actor* m_penelope;
};

#endif // STUDENTWORLD_H_
