#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

enum ActorType {
    e_penelope, e_actor, e_blockMovement, e_goodie, e_wall, e_exit, e_pit, e_flame, e_zombie, e_human, e_projectile
};

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth);
    virtual void doSomething()=0;
    virtual bool isAlive() const;
    virtual bool isBlocked() const;
    //Methods to check overlapping
    virtual ActorType getType() const;
    StudentWorld* getWorld() const;
    virtual bool overlap(Actor* otherActor) const;
    void setDead();
    double distance(double x, double y) const;
private:
    StudentWorld* m_world;
    bool m_alive;
    
};

class BlockMovement : public Actor //Zombies, Citizens, Walls
{
public:
    BlockMovement(StudentWorld* sw, int imageID, double startX, double startY);
    virtual bool isBlocked() const;
    virtual ActorType getType() const;
private:
};

class Human : public BlockMovement //Citizen, Penelope
{
public:
    Human(StudentWorld* sw, int imageID, double startX, double startY);
    virtual ActorType getType() const;
};

class Goodie : public Actor //Goodies
{
public:
    Goodie(StudentWorld* sw, int imageID, double startX, double startY);
    virtual ActorType getType() const;
    virtual void doSomething();
};

class Citizen: public Human
{
public:
    Citizen(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
private:
    int m_infectCount;
    int m_tickCount;
    bool m_infected;
};

class Wall : public BlockMovement
{
public:
    Wall(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
    virtual ActorType getType() const;
};

class Penelope : public Human
{
public:
    Penelope(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
    virtual ActorType getType() const;
    bool isAlive() const;
    bool isInfected() const;
    void throwFlame();
    void changeVaccine(int num);
    void changeGas(int num);
    void changeLandmine(int num);
    int getFlameCount() const;
    int getVaccineCount() const;
    int getLandmineCount() const;
    int getInfectCount() const;
    
private:
    int m_flameCount;
    int m_infectCount;
    int m_vaccineCount;
    int m_landmineCount;
};

class Exit : public Actor
{
public:
    Exit(StudentWorld* sw, double startX, double startY);
    virtual ActorType getType() const;
    virtual void doSomething();
};

class Pit : public Actor
{
public:
    Pit(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
    virtual ActorType getType() const;
};


class VaccineGoodie : public Goodie
{
public:
    VaccineGoodie(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
//    virtual ActorType getType() const;
};

class GasCanGoodie : public Goodie
{
public:
    GasCanGoodie(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
//    virtual ActorType getType() const;
};

class LandmineGoodie : public Goodie
{
public:
    LandmineGoodie(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
//    virtual ActorType getType() const;
};

class Projectile : public Actor
{
public:
    Projectile(StudentWorld* sw, int imageID, double startX, double startY);
    virtual void doSomething();
    virtual ActorType getType() const;
};

class Flame : public Projectile
{
public:
    Flame(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
    virtual ActorType getType() const;
private:
    int m_tickCount;
};

class Zombie : public BlockMovement
{
public:
    Zombie(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
    virtual ActorType getType() const;
};

class DumbZombie : public Zombie
{
public:
    DumbZombie(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
};

class SmartZombie : public Zombie
{
public:
    SmartZombie(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
};

#endif // ACTOR_H_
