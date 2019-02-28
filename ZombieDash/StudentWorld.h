#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
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
    bool doesBlockMovement(double x, double y, Actor* actor); //Makes sure objects don't overstep on each other
    bool doesBlockFire(double x, double y);
    bool doesBlockGoodie(double x, double y);
    Actor* doesOverlapWithAnyActor(Actor* notThisActor);
//    Actor* collides(Actor* actor);
    Penelope* getPenelope();
    void updateGameStats();
    void addActor(Actor* actor);
    double getClosestZombie(double x, double y);
    Actor* getClosestHuman(double x, double y);
    int numberOfCitizensLeft();
    void exitIsSteppedOn(bool isSteppedOn);
    bool isHumanInfrontOfZombie(double x, double y, int direction);

private:
    std::vector<Actor*> m_actors;
    Penelope* m_penelope;
    bool m_exitIsSteppedOn;
};

#endif // STUDENTWORLD_H_
