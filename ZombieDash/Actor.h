#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//Create a base class which all other classes will derive
//This base class derives from GraphObject
//It should have a doSomething() function
//Which functions should be virtual? or pure virtual? Will this class ever be instantiated?
//Add any public/private member functions and any private data members you need
//Recommendation: Start with this class

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth);
    virtual void doSomething();
    virtual bool isAlive() const;
    virtual bool isBlocked() const;
    StudentWorld* getWorld() const;
    bool overlap(const Actor& otherActor);
    void setDead();
private:
    StudentWorld* m_world;
    bool m_alive;
    
};

class BlockMovement : public Actor //Zombies, Citizens, Walls
{
public:
    BlockMovement(StudentWorld* sw, int imageID, double startX, double startY, Direction dir);
    virtual bool isBlocked() const;
private:
};

class Wall : public BlockMovement
{
public:
    Wall(StudentWorld* sw, double startX, double startY);
};

class Penelope : public BlockMovement
{
public:
    Penelope(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
    bool isAlive() const;
    void throwFlame();
    bool isInfected() const;
    
private:
    int m_flameCount = 0;
    int m_infectCount = 0;
};

class Exit : public Actor
{
public:
    Exit(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
};


#endif // ACTOR_H_
