#include "Actor.h"
//#include <cmath>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////
//                           Actor                           //
///////////////////////////////////////////////////////////////
Actor::Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth):m_world(sw),m_alive(true),m_tickCount(1),GraphObject(imageID, startX, startY, dir, depth, 1.0)
{
    
}

bool Actor::isAlive() const
{
    return m_alive;
}

bool Actor::isBlocked() const
{
    return false;
}

bool Actor::overlap(Actor* otherActor) const
{
    return sqrt((getX()-otherActor->getX())*(getX()-otherActor->getX())+(getY()-otherActor->getY())*(getY()-otherActor->getY()))<=10;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

void Actor::setDead()
{
    m_alive = false;
}

ActorType Actor::getType() const
{
    return ActorType::e_actor;
}

double Actor::distance(double x, double y) const
{
    return sqrt((getX()-x)*(getX()-x)+(getY()-y)*(getY()-y));
}

void Actor::increaseTickCount()
{
    m_tickCount++;
}

int Actor::getTickCount() const
{
    return m_tickCount;
}

bool Actor::isEvenTick() const
{
    return m_tickCount%2 == 0;
}

///////////////////////////////////////////////////////////////
//                      BlockMovement                        //
///////////////////////////////////////////////////////////////
BlockMovement::BlockMovement(StudentWorld* sw, int imageID, double startX, double startY):Actor(sw, imageID, startX, startY, right, 0)
{}

ActorType BlockMovement::getType() const
{
    return ActorType::e_blockMovement;
}

bool BlockMovement::isBlocked() const
{
    return true;
}

///////////////////////////////////////////////////////////////
//                         Human                             //
///////////////////////////////////////////////////////////////
Human::Human(StudentWorld* sw, int imageID, double startX, double startY):m_infected(false),m_infectCount(0),BlockMovement(sw, imageID, startX, startY){}

ActorType Human::getType() const
{
    return ActorType::e_human;
}

void Human::increaseInfectCount(int num)
{
    m_infectCount++;
}

int Human::getInfectCount() const
{
    return m_infectCount;
}

void Human::getInfected()
{
    m_infected = true;
}

bool Human::isInfected() const
{
    return m_infected;
}


///////////////////////////////////////////////////////////////
//                         Citizen                           //
///////////////////////////////////////////////////////////////
Citizen::Citizen(StudentWorld* sw, double startX, double startY):Human(sw, IID_CITIZEN, startX, startY)
{}

void Citizen::doSomething()
{
    if (!isAlive())
        return;
    if (isInfected())
        increaseInfectCount(1);
    if (getInfectCount() >= 500)
    {
        setDead();
        this->getWorld()->playSound(SOUND_ZOMBIE_BORN);
        this->getWorld()->increaseScore(-1000);
        int rand = randInt(1, 10);
        if  (rand <= 3)
        {
            this->getWorld()->addActor(new SmartZombie(this->getWorld(),getX(), getY()));
        }
        else
            this->getWorld()->addActor(new DumbZombie(this->getWorld(),getX(), getY()));
        return;
    }
    increaseTickCount();
    if (isEvenTick())
        return;
    double currX = this->getX();
    double currY = this->getY();
    Penelope* p = this->getWorld()->getPenelope();
    double dist_p = sqrt((currX - p->getX())*(currX - p->getX()) + (currY - p->getY())*(currY - p->getY()));
    double dist_z = this->getWorld()->getClosestZombie(currX, currY);
    
    if (dist_z == -1 || dist_p < dist_z)
    {
        if (dist_p <= 80)
        {
            if (currX == p->getX() || currY == p->getY())
            {
                //Version 1 of setting direction
//                int dir = p->getDirection() >= 180 ? p->getDirection()-180 : p->getDirection()+180;
                //Version 2 of setting direction
                int dir = -1;
                if (currX > p->getX())
                    dir = left;
                else if (currX < p->getX())
                    dir = right;
                if (currY > p->getY())
                    dir = down;
                else if (currY < p->getY())
                    dir = up;
                switch (dir)
                {
                    case right:
                        if (!getWorld()->doesBlockMovement(currX+2, currY, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX+2, currY);
                            return;
                        }
                        break;
                    case up:
                        
                        this->setDirection(dir);
                        if (!getWorld()->doesBlockMovement(currX, currY+2, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX, currY+2);
                            return;
                        }
                        break;
                    case left:
                        
                        this->setDirection(dir);
                        if (!getWorld()->doesBlockMovement(currX-2, currY, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX-2, currY);
                            return;
                        }
                        break;
                    case down:
                        
                        this->setDirection(dir);
                        if (!getWorld()->doesBlockMovement(currX, currY-2, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX, currY-2);
                            return;
                        }
                        break;
                    default:
                        break;
                }
            }
            else
            {
                int rand = randInt(0, 1);
                int dir1 = p->getX() > currX ? right: left;
                int dir2 = p->getY() > currY ? up : down;
                int dir = rand == 0 ? dir1 : dir2;
                int otherDir = rand == 1 ? dir2 : dir1;
                switch (dir)
                {
                    case right:
                        if (!getWorld()->doesBlockMovement(currX+2, currY, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX+2, currY);
                            return;
                        }
                        break;
                    case up:
                        if (!getWorld()->doesBlockMovement(currX, currY+2, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX, currY+2);
                            return;
                        }
                        break;
                    case left:
                        if (!getWorld()->doesBlockMovement(currX-2, currY, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX-2, currY);
                            return;
                        }
                        break;
                    case down:
                        if (!getWorld()->doesBlockMovement(currX, currY-2, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX, currY-2);
                            return;
                        }
                        break;
                    default:
                        break;
                }
                dir = otherDir;
                switch (dir)
                {
                    case right:
                        if (!getWorld()->doesBlockMovement(currX+2, currY, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX+2, currY);
                            return;
                        }
                        break;
                    case up:
                        if (!getWorld()->doesBlockMovement(currX, currY+2, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX, currY+2);
                            return;
                        }
                        break;
                    case left:
                        if (!getWorld()->doesBlockMovement(currX-2, currY, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX-2, currY);
                            return;
                        }
                        break;
                    case down:
                        if (!getWorld()->doesBlockMovement(currX, currY-2, this))
                        {
                            this->setDirection(dir);
                            this->moveTo(currX, currY-2);
                            return;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        else if (dist_z <= 80)
        {
            double projectedDistance;
            int dir = -1;
            if (!getWorld()->doesBlockMovement(currX+2, currY, this))
            {
                projectedDistance = getWorld()->getClosestZombie(currX+2, currY);
                if (dist_z < projectedDistance)
                {
                    dir = right;
                    dist_z = projectedDistance;
                }
            }
            if (!getWorld()->doesBlockMovement(currX, currY+2, this))
            {
                projectedDistance = getWorld()->getClosestZombie(currX, currY+2);
                if (dist_z < projectedDistance)
                {
                    dir = up;
                    dist_z = projectedDistance;
                }
            }
            if (!getWorld()->doesBlockMovement(currX-2, currY, this))
            {
                projectedDistance = getWorld()->getClosestZombie(currX-2, currY);
                if (dist_z < projectedDistance)
                {
                    dir = left;
                    dist_z = projectedDistance;
                }
            }
            if (!getWorld()->doesBlockMovement(currX, currY-2, this))
            {
                projectedDistance = getWorld()->getClosestZombie(currX, currY-2);
                if (dist_z < projectedDistance)
                {
                    dir = down;
                    dist_z = projectedDistance;
                }
            }
            if (dir == -1)
                return;
            switch (dir)
            {
                case right:
                    if (!getWorld()->doesBlockMovement(currX+2, currY, this))
                    {
                        this->setDirection(dir);
                        this->moveTo(currX+2, currY);
                        return;
                    }
                    break;
                case up:
                    if (!getWorld()->doesBlockMovement(currX, currY+2, this))
                    {
                        this->setDirection(dir);
                        this->moveTo(currX, currY+2);
                        return;
                    }
                    break;
                case left:
                    if (!getWorld()->doesBlockMovement(currX-2, currY, this))
                    {
                        this->setDirection(dir);
                        this->moveTo(currX-2, currY);
                        return;
                    }
                    break;
                case down:
                    if (!getWorld()->doesBlockMovement(currX, currY-2, this))
                    {
                        this->setDirection(dir);
                        this->moveTo(currX, currY-2);
                        return;
                    }
                    break;
                default:
                    break;
            }
            return;
        }
        else
            return;
    }
}

///////////////////////////////////////////////////////////////
//                          Goodie                           //
///////////////////////////////////////////////////////////////
Goodie::Goodie(StudentWorld* sw, int imageID, double startX, double startY):Actor(sw, imageID, startX, startY, right, 1)
{
    
}

void Goodie::doSomething()
{
}

ActorType Goodie::getType() const
{
    return ActorType::e_goodie;
}

///////////////////////////////////////////////////////////////
//                      Vaccine Goodie                       //
///////////////////////////////////////////////////////////////

VaccineGoodie::VaccineGoodie(StudentWorld* sw, double startX, double startY):Goodie(sw, IID_VACCINE_GOODIE, startX, startY)
{
    
}

void VaccineGoodie::doSomething()
{
    if (!isAlive())
    return;
    StudentWorld* currWorld = this->getWorld();
    if (!currWorld->getPenelope()->overlap(this))
        return;
    currWorld->increaseScore(50);
    setDead();
    currWorld->playSound(SOUND_GOT_GOODIE);
    getWorld()->getPenelope()->changeVaccine(1);
}

//ActorType VaccineGoodie::getType() const
//{
//    return ActorType::e_vaccineGoodie;
//}

///////////////////////////////////////////////////////////////
//                      Gas Can Goodie                       //
///////////////////////////////////////////////////////////////

GasCanGoodie::GasCanGoodie(StudentWorld* sw, double startX, double startY):Goodie(sw, IID_GAS_CAN_GOODIE, startX, startY){}

void GasCanGoodie::doSomething()
{
    if (!isAlive())
        return;
    StudentWorld* currWorld = this->getWorld();
    if (!currWorld->getPenelope()->overlap(this))
        return;
    currWorld->increaseScore(50);
    setDead();
    currWorld->playSound(SOUND_GOT_GOODIE);
    getWorld()->getPenelope()->changeGas(5);
}
//ActorType GasCanGoodie::getType() const
//{
//    return ActorType::e_gasCanGoodie;
//}

///////////////////////////////////////////////////////////////
//                      Landmine Goodie                      //
///////////////////////////////////////////////////////////////
LandmineGoodie::LandmineGoodie(StudentWorld* sw, double startX, double startY):Goodie(sw, IID_LANDMINE_GOODIE, startX, startY){}

void LandmineGoodie::doSomething()
{
    if (!isAlive())
    return;
    StudentWorld* currWorld = this->getWorld();
    if (!currWorld->getPenelope()->overlap(this))
        return;
    currWorld->increaseScore(50);
    setDead();
    currWorld->playSound(SOUND_GOT_GOODIE);
    getWorld()->getPenelope()->changeLandmine(2);
}


///////////////////////////////////////////////////////////////
//                           Wall                            //
///////////////////////////////////////////////////////////////

Wall::Wall(StudentWorld* sw, double startX, double startY):BlockMovement(sw, IID_WALL, startX, startY)
{}

ActorType Wall::getType() const
{
    return ActorType::e_wall;
}

void Wall::doSomething()
{}

///////////////////////////////////////////////////////////////
//                           Penelope                        //
///////////////////////////////////////////////////////////////

Penelope::Penelope(StudentWorld* sw, double startX, double startY):m_flameCount(0),m_vaccineCount(0),m_landmineCount(0),Human(sw, IID_PLAYER, startX, startY)
{
    //TODO: finish
}

ActorType Penelope::getType() const
{
    return ActorType::e_penelope;
}

void Penelope::throwFlame()
{
    if (m_flameCount <= 0)
        return;
    
    m_flameCount--;
    getWorld()->playSound(SOUND_PLAYER_FIRE);
    int dir = this->getWorld()->getPenelope()->getDirection();
//    for (int i = 1;i<4;i++)
//    {
//        //TODO: generate three flames
//        switch (dir)
//        {
//            case up:
//                if (!this->getWorld()->doesBlockMovement(this->getX(), this->getY()+i*SPRITE_HEIGHT, this))
//                    this->getWorld()->addActor(new Flame(this->getWorld(),this->getX(), this->getY()+i*SPRITE_HEIGHT));
//                else
//                    continue;
//                break;
//            case down:
//                if (!this->getWorld()->doesBlockMovement(this->getX(), this->getY()-i*SPRITE_HEIGHT, this))
//                    this->getWorld()->addActor(new Flame(this->getWorld(),this->getX(), this->getY()-i*SPRITE_HEIGHT));
//                else
//                        continue;
//                break;
//            case left:
//                if (!this->getWorld()->doesBlockMovement(this->getX()-i*SPRITE_WIDTH, this->getY(), this))
//                    this->getWorld()->addActor(new Flame(this->getWorld(),this->getX()-i*SPRITE_WIDTH, this->getY()));
//                else
//                    continue;
//                break;
//            case right:
//                if (!this->getWorld()->doesBlockMovement(this->getX()+i*SPRITE_WIDTH, this->getY(), this))
//                    this->getWorld()->addActor(new Flame(this->getWorld(),this->getX()+i*SPRITE_WIDTH, this->getY()));
//                else
//                    continue;
//                break;
//            default:
//                break;
//        }
//    }
    switch (dir)
    {
        case up:
            for (int i = 1;i<4;i++)
            if (!this->getWorld()->doesBlockMovement(this->getX(), this->getY()+i*SPRITE_HEIGHT, this))
                this->getWorld()->addActor(new Flame(this->getWorld(),this->getX(), this->getY()+i*SPRITE_HEIGHT));
            else
                break;
            break;
        case down:
            for (int i = 1;i<4;i++)
            if (!this->getWorld()->doesBlockMovement(this->getX(), this->getY()-i*SPRITE_HEIGHT, this))
                this->getWorld()->addActor(new Flame(this->getWorld(),this->getX(), this->getY()-i*SPRITE_HEIGHT));
            else
                break;
            break;
        case left:
            for (int i = 1;i<4;i++)
            if (!this->getWorld()->doesBlockMovement(this->getX()-i*SPRITE_WIDTH, this->getY(), this))
                this->getWorld()->addActor(new Flame(this->getWorld(),this->getX()-i*SPRITE_WIDTH, this->getY()));
            else
                break;
            break;
        case right:
            for (int i = 1;i<4;i++)
            if (!this->getWorld()->doesBlockMovement(this->getX()+i*SPRITE_WIDTH, this->getY(), this))
                this->getWorld()->addActor(new Flame(this->getWorld(),this->getX()+i*SPRITE_WIDTH, this->getY()));
            else
                break;
        
            break;
    }
}

bool Penelope::isAlive() const
{
    return getWorld()->getLives() > 0;
}

void Penelope::changeVaccine(int num)
{
    m_vaccineCount+=num;
}

void Penelope::changeGas(int num)
{
    m_flameCount+=num;
}

void Penelope::changeLandmine(int num)
{
    m_landmineCount+=num;
}

void Penelope::doSomething()
{
    if (!isAlive())
        return;
    if (isInfected())
    {
        increaseInfectCount(1);
    }
    if (getInfectCount() >= 500)
    {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
        return;
    }
    
    int ch;
    if (getWorld()->getKey(ch))
    {
        // user hit a key during this tick!
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                //... move Penelope to the left ...
                this->setDirection(left);
                if (!getWorld()->doesBlockMovement(getX()-4, getY(), this))
                {
                    this->moveTo(getX()-4, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                //                ... move Penelope to the right ...
                this->setDirection(right);
                if (!getWorld()->doesBlockMovement(getX()+4, getY(), this))
                {
                    this->moveTo(getX()+4, getY());
                }
                break;
            case KEY_PRESS_UP:
                this->setDirection(up);
                if (!getWorld()->doesBlockMovement(getX(), getY()+4, this))
                {
                    this->moveTo(getX(), getY()+4);
                }
                break;
            case KEY_PRESS_DOWN:
                this->setDirection(down);
                if (!getWorld()->doesBlockMovement(getX(), getY()-4, this))
                {
                    this->moveTo(getX(), getY()-4);
                }
                break;
            case KEY_PRESS_SPACE:
                //                ... add flames in front of Penelope...
                throwFlame();
                break;
            case KEY_PRESS_ENTER:
                //...
                break;
            case KEY_PRESS_TAB:
                //...
                break;
            default:
                break;
        }
    }
}

int Penelope::getFlameCount() const
{
    return m_flameCount;
}

//int Penelope::getInfectCount() const
//{
//    return m_infectCount;
//}

int Penelope::getVaccineCount() const
{
    return m_vaccineCount;
}

int Penelope::getLandmineCount() const
{
    return m_landmineCount;
}
///////////////////////////////////////////////////////////////
//                           Exit                            //
///////////////////////////////////////////////////////////////
Exit::Exit(StudentWorld* sw, double startX, double startY):Actor(sw, IID_EXIT,startX, startY, right,1)
{
    
}

ActorType Exit::getType() const
{
    return ActorType::e_exit;
}

void Exit::doSomething()
{
    //TODO: finish this
    if (overlap(this->getWorld()->getPenelope()))
    {
//        if (finishedLevel())
        if (false)//TODO: fix
        this->getWorld()->advanceToNextLevel();
    }
}

///////////////////////////////////////////////////////////////
//                           Pit                             //
///////////////////////////////////////////////////////////////
Pit::Pit(StudentWorld* sw, double startX, double startY):Actor(sw, IID_PIT, startX, startY, right, 0)
{
    
}

ActorType Pit::getType() const
{
    return ActorType::e_pit;
}

void Pit::doSomething()
{
    //TODO: finish this
}

///////////////////////////////////////////////////////////////
//                      Projectile                           //
///////////////////////////////////////////////////////////////
Projectile::Projectile(StudentWorld* sw, int imageID, double startX, double startY):Actor(sw, imageID, startX, startY, right, 0)
{
    
}

ActorType Projectile::getType() const
{
    return ActorType::e_projectile;
}

void Projectile::doSomething()
{
    //TODO: finish this
}

///////////////////////////////////////////////////////////////
//                         Flame                             //
///////////////////////////////////////////////////////////////
Flame::Flame(StudentWorld* sw, double startX, double startY):Projectile(sw, IID_FLAME, startX, startY)
{
    
}

ActorType Flame::getType() const
{
    return ActorType::e_flame;
}

void Flame::doSomething()
{
    if (getTickCount() == 2)
        setDead();
    increaseTickCount();
    if (!isAlive())
        return;
    Actor* actor = this->getWorld()->getActorAt(getX(), getY());
    if (!actor)
        return;
    if (actor->getType() == ActorType::e_penelope)
        return;
    if (actor->getType() == ActorType::e_goodie)
    {
        actor->setDead();
    }
    if (actor->getType() == ActorType::e_zombie)
    {
        actor->setDead();
        this->getWorld()->playSound(SOUND_ZOMBIE_DIE);
        this->getWorld()->increaseScore(1000);
    }
    if (actor->getType() == ActorType::e_landmine)
    {
        //TODO: finish this part: burning the landmine, all that type of stuff
    }
    if (actor->getType() == ActorType::e_human)
    {
        actor->setDead();
        this->getWorld()->playSound(SOUND_CITIZEN_DIE);
        this->getWorld()->increaseScore(-1000);
    }
}

///////////////////////////////////////////////////////////////
//                        Zombie                             //
///////////////////////////////////////////////////////////////
Zombie::Zombie(StudentWorld* sw, double startX, double startY):BlockMovement(sw, IID_ZOMBIE, startX, startY)
{
    
}

void Zombie::doSomething()
{
    //TODO: finish
    if (!isAlive())
        return;
    
}

ActorType Zombie::getType() const
{
    return ActorType::e_zombie;
}

///////////////////////////////////////////////////////////////
//                        DumbZombie                         //
///////////////////////////////////////////////////////////////
DumbZombie::DumbZombie(StudentWorld* sw, double startX, double startY):Zombie(sw, startX, startY)
{
    
}

void DumbZombie::doSomething()
{
    //TODO: finish
    
    increaseTickCount();
    if (!isAlive())
        return;
    if (isEvenTick())
        return;
    
}

///////////////////////////////////////////////////////////////
//                       SmartZombie                         //
///////////////////////////////////////////////////////////////
SmartZombie::SmartZombie(StudentWorld* sw, double startX, double startY):Zombie(sw, startX, startY)
{
    
}

void SmartZombie::doSomething()
{
    //TODO: finish
}

///////////////////////////////////////////////////////////////
//                       Landmine                            //
///////////////////////////////////////////////////////////////



