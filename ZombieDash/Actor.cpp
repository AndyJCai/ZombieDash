#include "Actor.h"
//#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////
//                           Actor                           //
///////////////////////////////////////////////////////////////
Actor::Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth):m_world(sw),m_alive(true),GraphObject(imageID, startX, startY, dir, depth, 1.0)
{
    sw->addActor(this);
}

bool Actor::isAlive() const
{
    return m_alive;
}

bool Actor::overlap(const Actor &otherActor)
{
    return sqrt((getX()-otherActor.getX())*(getX()-otherActor.getX())+(getY()-otherActor.getY())*(getY()-otherActor.getY()))<=10;
}

void Actor::setDead()
{
    m_alive = false;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

bool Actor::isBlocked() const
{
    return false;
}

void Actor::doSomething()
{
    
}

///////////////////////////////////////////////////////////////
//                      BlockMovement                        //
///////////////////////////////////////////////////////////////
BlockMovement::BlockMovement(StudentWorld* sw, int imageID, double startX, double startY, Direction dir):Actor(sw, imageID, startX, startY, dir, 0)
{}

bool BlockMovement::isBlocked() const
{
    return true;
}
///////////////////////////////////////////////////////////////
//                           Wall                            //
///////////////////////////////////////////////////////////////

Wall::Wall(StudentWorld* sw, double startX, double startY):BlockMovement(sw, IID_WALL, startX, startY, right)
{}

///////////////////////////////////////////////////////////////
//                           Penelope                        //
///////////////////////////////////////////////////////////////

Penelope::Penelope(StudentWorld* sw, double startX, double startY):BlockMovement(sw, IID_PLAYER, startX, startY, right)
{
    //TODO: finish
    sw->setPenelope(this);
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
                if (!getWorld()->doesOverlap(getX()-4, getY()))
                {
                    this->setDirection(left);
                    this->moveTo(getX()-4, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                //                ... move Penelope to the right ...
                if (!getWorld()->doesOverlap(getX()+4, getY()))
                {
                    this->setDirection(right);
                    this->moveTo(getX()+4, getY());
                }
                break;
            case KEY_PRESS_UP:
                if (!getWorld()->doesOverlap(getX(), getY()+4))
                {
                    this->setDirection(up);
                    this->moveTo(getX(), getY()+4);
                }
                break;
            case KEY_PRESS_DOWN:
                if (!getWorld()->doesOverlap(getX(), getY()-4))
                {
                    this->setDirection(down);
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

void Exit::doSomething()
{
    
}
