#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

enum ActorType {
    e_penelope, e_actor, e_blockMovement, e_goodie, e_wall, e_exit, e_pit, e_flame, e_zombie, e_human, e_projectile, e_landmine, e_vomit
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
    void increaseTickCount();
    int getTickCount() const;
    bool isEvenTick() const;
private:
    StudentWorld* m_world;
    bool m_alive;
    int m_tickCount;
    
};

class BlockMovement : public Actor //Zombies, Citizens, Walls
{
public:
    BlockMovement(StudentWorld* sw, int imageID, double startX, double startY);
    virtual bool isBlocked() const;
    virtual ActorType getType() const;
};

class Human : public BlockMovement //Citizen, Penelope
{
public:
    Human(StudentWorld* sw, int imageID, double startX, double startY);
    virtual ActorType getType() const;
    int getInfectCount() const;
    void increaseInfectCount(int num);
    void getInfected();
    void getCured();
    bool isInfected() const;
private:
    int m_infectCount;
    bool m_infected;
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
//    virtual ActorType getType() const;
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
//    bool isAlive() const;
//    bool isInfected() const;
    void throwFlame();
    void useVaccine();
    void putLandmine();
    void changeVaccine(int num);
    void changeGas(int num);
    void changeLandmine(int num);
    int getFlameCount() const;
    int getVaccineCount() const;
    int getLandmineCount() const;
//    int getInfectCount() const;
    
private:
    int m_flameCount;
//    int m_infectCount;
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
};

class Vomit : public Projectile
{
public:
    Vomit(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
    virtual ActorType getType() const;
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
private:
    int m_plan;
};

class SmartZombie : public Zombie
{
public:
    SmartZombie(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
};

class Landmine : public Actor
{
public:
    Landmine(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
private:
    int m_safetyTick;
    bool m_isActive;
};

#endif // ACTOR_H_
