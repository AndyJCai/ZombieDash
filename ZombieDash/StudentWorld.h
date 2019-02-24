#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Penelope;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void setPenelope(Penelope* p);
    void loadLevel(int level);
    bool doesBlockMovement(double x, double y); //Makes sure objects don't overstep on each other
    Actor* getActorAt(double x, double y);
//    Actor* collides(Actor* actor);
    Penelope* getPenelope();
    void updateGameStats();
    Actor* getClosestZombie(double x, double y);

private:
    std::vector<Actor*> m_actors;
    Penelope* m_penelope;
};

#endif // STUDENTWORLD_H_
