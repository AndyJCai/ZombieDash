#include "Actor.h"
//#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////
//                           Actor                           //
///////////////////////////////////////////////////////////////
Actor::Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth):m_world(sw),m_alive(true),GraphObject(imageID, startX, startY, dir, depth, 1.0)
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

bool Actor::overlap(const Actor &otherActor) const
{
    return sqrt((getX()-otherActor.getX())*(getX()-otherActor.getX())+(getY()-otherActor.getY())*(getY()-otherActor.getY()))<=10;
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
//                          Goodie                           //
///////////////////////////////////////////////////////////////
Goodie::Goodie(StudentWorld* sw, int imageID, double startX, double startY):Actor(sw, imageID, startX, startY, right, 1)
{
    
}

void Goodie::doSomething()
{
    if (!isAlive())
        return;
    StudentWorld* currWorld = this->getWorld();
    if (!currWorld->getPenelope()->overlap(*this))
        return;
    currWorld->increaseScore(50);
    setDead();
    currWorld->playSound(SOUND_GOT_GOODIE);
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
    Goodie::doSomething();
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
    Goodie::doSomething();
    getWorld()->getPenelope()->changeGas(5);
}
//ActorType GasCanGoodie::getType() const
//{
//    return ActorType::e_gasCanGoodie;
//}

///////////////////////////////////////////////////////////////
//                      Landmine Goodie                      //
///////////////////////////////////////////////////////////////
LandmineGoodie::LandmineGoodie(StudentWorld* sw, double startX, double startY):Goodie(sw, IID_GAS_CAN_GOODIE, startX, startY){}

void LandmineGoodie::doSomething()
{
    Goodie::doSomething();
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

Penelope::Penelope(StudentWorld* sw, double startX, double startY):m_flameCount(0),m_infectCount(0),m_vaccineCount(0),BlockMovement(sw, IID_PLAYER, startX, startY)
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
    for (int i = 0;i<3;i++)
    {
        //TODO: generate three flames
    }
}

bool Penelope::isInfected() const
{
    return m_infectCount>0;
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
    //KEY_PRESS_LEFT
    //    KEY_PRESS_RIGHT
    //    KEY_PRESS_UP
    //    KEY_PRESS_DOWN
    //    KEY_PRESS_SPACE
    //    KEY_PRESS_TAB
    //    KEY_PRESS_ENTER
    if (!isAlive())
        return;
    if (isInfected())
    {
        m_infectCount++;
    }
    if (m_infectCount >= 500)
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
                if (!getWorld()->doesBlockMovement(getX()-4, getY()))
                {
                    this->moveTo(getX()-4, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                //                ... move Penelope to the right ...
                this->setDirection(right);
                if (!getWorld()->doesBlockMovement(getX()+4, getY()))
                {
                    this->moveTo(getX()+4, getY());
                }
                break;
            case KEY_PRESS_UP:
                this->setDirection(up);
                if (!getWorld()->doesBlockMovement(getX(), getY()+4))
                {
                    this->moveTo(getX(), getY()+4);
                }
                break;
            case KEY_PRESS_DOWN:
                this->setDirection(down);
                if (!getWorld()->doesBlockMovement(getX(), getY()-4))
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
            default:
                break;
        }
    }
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
//                         Flame                             //
///////////////////////////////////////////////////////////////
Flame::Flame(StudentWorld* sw, double startX, double startY):m_tickCount(0),Actor(sw, IID_FLAME, startX, startY, right, 0)
{
    
}

ActorType Flame::getType() const
{
    return ActorType::e_flame;
}

void Flame::doSomething()
{
    if (m_tickCount == 2)
        setDead();
    m_tickCount++;
    if (!isAlive())
        return;
    //TODO: finish
}
