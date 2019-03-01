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
    void setPenelope(Penelope* p); //set penelope to the current world's Penelope
    void loadLevel(int level); //Loads the indicated level
    bool doesBlockMovement(double x, double y, Actor* actor); //checks if the current actor can move to coordinate (x,y) without being blocked given that the actor is not of type projectile
    bool doesBlockFire(double x, double y); //checks if a flame can be instantiated at coordinate (x,y) without being blocked
    bool doesBlockGoodie(double x, double y); //checks if a goodie can be instantiated at coordinate (x,y) without being blocked
    Actor* doesOverlapWithAnyActor(Actor* notThisActor); //Returns the actor that the passed in actor is overlapped with, returns nullptr if there is none
    Penelope* getPenelope();
    void updateGameStats();
    void addActor(Actor* actor); //add actor to the vector of actor pointers
    double getClosestZombie(double x, double y); //get the distance to the closest zombie
    Actor* getClosestHuman(double x, double y); //get the pointer to the closest human (penelope or citizen)
    int numberOfCitizensLeft(); //returns the number of citizens left
    void exitIsSteppedOn(bool isSteppedOn); //check if the exit object is being stepped on
    bool isHumanInfrontOfZombie(double x, double y, int direction); //check if there is any Human object in the direction that the zombie is facing

private:
    std::vector<Actor*> m_actors;
    Penelope* m_penelope;
    bool m_exitIsSteppedOn;
};

#endif // STUDENTWORLD_H_
