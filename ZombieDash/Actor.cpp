#include "Actor.h"
//#include <cmath>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////
//                           Actor                           //
///////////////////////////////////////////////////////////////
Actor::Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth):m_world(sw),m_alive(true),m_tickCount(0),GraphObject(imageID, startX, startY, dir, depth, 1.0)
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
    m_infectCount+=num;
}

int Human::getInfectCount() const
{
    return m_infectCount;
}

void Human::getInfected()
{
    m_infected = true;
    m_infectCount++;
}

void Human::getCured()
{
    m_infected = false;
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
    //TODO: finish this because the zombies are turning really quickly
    if (!isAlive())
        return;
    if (isInfected())
        increaseInfectCount(1);
    Actor* actor = this->getWorld()->doesOverlapWithAnyActor(this);
    if (actor)
    {
        if (actor->getType() == ActorType::e_vomit)
        {
            this->getInfected();
            this->getWorld()->playSound(SOUND_CITIZEN_INFECTED);
        }
        else if (actor->getType() ==ActorType::e_flame)
        {
            this->setDead();
            this->getWorld()->playSound(SOUND_CITIZEN_DIE);
            this->getWorld()->increaseScore(-1000);
        }
    }
    //Following code checks the interaction with vomit
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
                else
                    dir = right;
                if (currY > p->getY())
                    dir = down;
                else
                    dir = up;
                switch (dir)
                {
                    case right:
                        if (!getWorld()->doesBlockMovement(currX+2, currY, this))
                        {
//                            cout<<"right called"<<endl;
                            this->setDirection(dir);
                            this->moveTo(currX+2, currY);
                            return;
                        }
                        break;
                    case up:
                        if (!getWorld()->doesBlockMovement(currX, currY+2, this))
                        {
//                            cout<<"up called"<<endl;
                            this->setDirection(dir);
                            this->moveTo(currX, currY+2);
                            return;
                        }
                        break;
                    case left:
                        if (!getWorld()->doesBlockMovement(currX-2, currY, this))
                        {
//                            cout<<"left called"<<endl;
                            this->setDirection(dir);
                            this->moveTo(currX-2, currY);
                            return;
                        }
                        break;
                    case down:
                        if (!getWorld()->doesBlockMovement(currX, currY-2, this))
                        {
//                            cout<<"down called"<<endl;
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
//                int otherDir = rand == 0 ? dir2 : dir1;
                switch (dir)
                {
                    case right:
                        if (!getWorld()->doesBlockMovement(currX+2, currY, this))
                        {
                            cout<<"right 2 called"<<endl;
                            this->setDirection(dir);
                            this->moveTo(currX+2, currY);
                            return;
                        }
                        break;
                    case up:
                        if (!getWorld()->doesBlockMovement(currX, currY+2, this))
                        {
                            cout<<"up 2 called"<<endl;
                            this->setDirection(dir);
                            this->moveTo(currX, currY+2);
                            return;
                        }
                        break;
                    case left:
                        
                        if (!getWorld()->doesBlockMovement(currX-2, currY, this))
                        {
                            cout<<"left 2 called"<<endl;
                            this->setDirection(dir);
                            this->moveTo(currX-2, currY);
                            return;
                        }
                        break;
                    case down:
                        if (!getWorld()->doesBlockMovement(currX, currY-2, this))
                        {
                            cout<<"down 2 called"<<endl;
                            this->setDirection(dir);
                            this->moveTo(currX, currY-2);
                            return;
                        }
                        break;
                    default:
                        break;
                }
//                dir = otherDir;
//                switch (dir)
//                {
//                    case right:
//                        if (!getWorld()->doesBlockMovement(currX+2, currY, this))
//                        {
//                            cout<<"right 3 called"<<endl;
//                            this->setDirection(dir);
//                            this->moveTo(currX+2, currY);
//                            return;
//                        }
//                        break;
//                    case up:
//                        if (!getWorld()->doesBlockMovement(currX, currY+2, this))
//                        {
//                            cout<<"up 3 called"<<endl;
//                            this->setDirection(dir);
//                            this->moveTo(currX, currY+2);
//                            return;
//                        }
//                        break;
//                    case left:
//                        if (!getWorld()->doesBlockMovement(currX-2, currY, this))
//                        {
//                            cout<<"left 3 called"<<endl;
//                            this->setDirection(dir);
//                            this->moveTo(currX-2, currY);
//                            return;
//                        }
//                        break;
//                    case down:
//                        if (!getWorld()->doesBlockMovement(currX, currY-2, this))
//                        {
//                            cout<<"down 3 called"<<endl;
//                            this->setDirection(dir);
//                            this->moveTo(currX, currY-2);
//                            return;
//                        }
//                        break;
//                    default:
//                        break;
//                }
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
    Actor* actor = this->getWorld()->doesOverlapWithAnyActor(this);
    if (actor && actor->getType() == e_flame)
    {
        this->setDead();
    }
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
    Goodie::doSomething();
    StudentWorld* currWorld = this->getWorld();
    if (!currWorld->getPenelope()->overlap(this))
        return;
    currWorld->increaseScore(50);
    setDead();
    currWorld->playSound(SOUND_GOT_GOODIE);
    getWorld()->getPenelope()->changeVaccine(1);
}

///////////////////////////////////////////////////////////////
//                      Gas Can Goodie                       //
///////////////////////////////////////////////////////////////

GasCanGoodie::GasCanGoodie(StudentWorld* sw, double startX, double startY):Goodie(sw, IID_GAS_CAN_GOODIE, startX, startY){}

void GasCanGoodie::doSomething()
{
    if (!isAlive())
        return;
    Goodie::doSomething();
    StudentWorld* currWorld = this->getWorld();
    if (!currWorld->getPenelope()->overlap(this))
        return;
    currWorld->increaseScore(50);
    setDead();
    currWorld->playSound(SOUND_GOT_GOODIE);
    getWorld()->getPenelope()->changeGas(5);
}

///////////////////////////////////////////////////////////////
//                      Landmine Goodie                      //
///////////////////////////////////////////////////////////////
LandmineGoodie::LandmineGoodie(StudentWorld* sw, double startX, double startY):Goodie(sw, IID_LANDMINE_GOODIE, startX, startY){}

void LandmineGoodie::doSomething()
{
    if (!isAlive())
        return;
    Goodie::doSomething();
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
    switch (dir)
    {
        case up:
            for (int i = 1;i<4;i++)
            if (!this->getWorld()->doesBlockFire(this->getX(), this->getY()+i*SPRITE_HEIGHT))
                this->getWorld()->addActor(new Flame(this->getWorld(),this->getX(), this->getY()+i*SPRITE_HEIGHT));
            else
                break;
            break;
        case down:
            for (int i = 1;i<4;i++)
            if (!this->getWorld()->doesBlockFire(this->getX(), this->getY()-i*SPRITE_HEIGHT))
                this->getWorld()->addActor(new Flame(this->getWorld(),this->getX(), this->getY()-i*SPRITE_HEIGHT));
            else
                break;
            break;
        case left:
            for (int i = 1;i<4;i++)
            if (!this->getWorld()->doesBlockFire(this->getX()-i*SPRITE_WIDTH, this->getY()))
                this->getWorld()->addActor(new Flame(this->getWorld(),this->getX()-i*SPRITE_WIDTH, this->getY()));
            else
                break;
            break;
        case right:
            for (int i = 1;i<4;i++)
            if (!this->getWorld()->doesBlockFire(this->getX()+i*SPRITE_WIDTH, this->getY()))
                this->getWorld()->addActor(new Flame(this->getWorld(),this->getX()+i*SPRITE_WIDTH, this->getY()));
            else
                break;
        
            break;
    }
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

void Penelope::useVaccine()
{
    if (m_vaccineCount <= 0)
        return;
    m_vaccineCount--;
    getCured();
    increaseInfectCount(-getInfectCount());
}

void Penelope::putLandmine()
{
    this->getWorld()->addActor(new Landmine(this->getWorld(), this->getX(), this->getY()));
}

void Penelope::doSomething()
{
    if (!isAlive())
        return;
    
    
    Actor* actor = this->getWorld()->doesOverlapWithAnyActor(this);
    if (actor)
    {
        if (actor->getType() == e_flame)
        {
            this->setDead();
            this->getWorld()->decLives();
        }
        if (actor->getType() == e_vomit)
        {
            this->getInfected();
        }
    }
    
    if (isInfected())
    {
        increaseInfectCount(1);
    }
    if (getInfectCount() >= 500)
    {
        setDead();
        getWorld()->decLives();
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
                useVaccine();
                break;
            case KEY_PRESS_TAB:
                //...
                putLandmine();
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
    Actor* actorSteppedOnExit = this->getWorld()->doesOverlapWithAnyActor(this);
    if (actorSteppedOnExit && actorSteppedOnExit->getType() == e_human)
    {
        actorSteppedOnExit->setDead();
        this->getWorld()->playSound(SOUND_CITIZEN_SAVED);
        this->getWorld()->increaseScore(500);
    }
    if (overlap(this->getWorld()->getPenelope()))
    {
        this->getWorld()->exitIsSteppedOn(true);
    }
    else
        this->getWorld()->exitIsSteppedOn(false);
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
    if (this->overlap(this->getWorld()->getPenelope()))
    {
        this->getWorld()->getPenelope()->setDead();
        this->getWorld()->playSound(SOUND_PLAYER_DIE);
    }
    Actor* actor = getWorld()->doesOverlapWithAnyActor(this);
    if (!actor)
        return;
    if (actor->getType() == e_human)
    {
        actor->setDead();
        this->getWorld()->playSound(SOUND_CITIZEN_DIE);
        
    }
    if (actor->getType() == e_zombie)
    {
        actor->setDead();
        this->getWorld()->playSound(SOUND_ZOMBIE_DIE);
    }
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
}

///////////////////////////////////////////////////////////////
//                          Vomit                            //
///////////////////////////////////////////////////////////////
Vomit::Vomit(StudentWorld* sw, double startX, double startY):Projectile(sw, IID_VOMIT, startX, startY)
{
    
}

ActorType Vomit::getType() const
{
    return ActorType::e_vomit;
}

void Vomit::doSomething()
{
    if (!isAlive())
        return;
    if (getTickCount() == 2)
        setDead();
    increaseTickCount();
}

///////////////////////////////////////////////////////////////
//                        Zombie                             //
///////////////////////////////////////////////////////////////
Zombie::Zombie(StudentWorld* sw, double startX, double startY):m_plan(0),BlockMovement(sw, IID_ZOMBIE, startX, startY)
{
    
}

void Zombie::doSomething()
{
    
}

ActorType Zombie::getType() const
{
    return ActorType::e_zombie;
}

void Zombie::decrementMPlan()
{
    m_plan--;
}

int Zombie::getMovementPlan() const
{
    return m_plan;
}

void Zombie::setMPlan(int num)
{
    m_plan = num;
}

///////////////////////////////////////////////////////////////
//                        DumbZombie                         //
///////////////////////////////////////////////////////////////
DumbZombie::DumbZombie(StudentWorld* sw, double startX, double startY):Zombie(sw, startX, startY)
{
    
}

void DumbZombie::doSomething()
{
    if (!isAlive())
        return;
    Actor* actor = this->getWorld()->doesOverlapWithAnyActor(this);
    if (actor && actor->getType() == e_flame)
    {
        this->setDead();
        this->getWorld()->playSound(SOUND_ZOMBIE_DIE);
        this->getWorld()->increaseScore(1000);
    }
    increaseTickCount();
    if (isEvenTick())
        return;
    double currX = getX(), currY = getY();
    double vomitX = 0, vomitY = 0;
    switch (this->getDirection()) {
        case right:
            vomitX = currX+SPRITE_WIDTH;
            vomitY= currY;
            break;
        case left:
            vomitX = currX - SPRITE_WIDTH;
            vomitY = currY;
            break;
        case up:
            vomitX = currX;
            vomitY = currY+SPRITE_HEIGHT;
            break;
        case down:
            vomitX = currX;
            vomitY = currY-SPRITE_HEIGHT;
            break;
        default:
            break;
    }
    StudentWorld* currW = this->getWorld();
    if (currW->isHumanInfrontOfZombie(vomitX, vomitY, this->getDirection()) && randInt(1, 3) == 1)
    {
        currW->addActor(new Vomit(currW, vomitX, vomitY));
        currW->playSound(SOUND_ZOMBIE_VOMIT);
        return;
    }
    
    if (getMovementPlan() == 0)
    {
        setMPlan(randInt(3, 10));
        this->setDirection(randInt(0, 3)*90);
    }
    double dist_x = 0, dist_y = 0;
    switch (this->getDirection()) {
        case right:
            dist_x = 1;
            break;
        case left:
            dist_x = -1;
            break;
        case up:
            dist_y = 1;
            break;
        case down:
            dist_y = -1;
            break;
        default:
            break;
    }
    if (!currW->doesBlockMovement(currX+dist_x, currY+dist_y, this))
    {
//        cout<<"dist_x "<<dist_x<<" dist_y "<<dist_y<<endl;
        this->moveTo(currX+dist_x, currY+dist_y);
        decrementMPlan();
    }
    else
        setMPlan(0);
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
    if (!isAlive())
        return;
    Actor* actor = this->getWorld()->doesOverlapWithAnyActor(this);
    if (actor && actor->getType() == e_flame)
    {
        this->setDead();
        this->getWorld()->playSound(SOUND_ZOMBIE_DIE);
        this->getWorld()->increaseScore(2000);
    }
    increaseTickCount();
    if (isEvenTick())
        return;
    double currX = getX(), currY = getY();
    double vomitX = 0, vomitY = 0;
    switch (this->getDirection()) {
        case right:
            vomitX = currX+SPRITE_WIDTH;
            vomitY= currY;
            break;
        case left:
            vomitX = currX - SPRITE_WIDTH;
            vomitY = currY;
            break;
        case up:
            vomitX = currX;
            vomitY = currY+SPRITE_HEIGHT;
            break;
        case down:
            vomitX = currX;
            vomitY = currY-SPRITE_HEIGHT;
            break;
        default:
            break;
    }
    StudentWorld* currW = this->getWorld();
    if (currW->isHumanInfrontOfZombie(vomitX, vomitY, this->getDirection()) && randInt(1, 3) == 1)
    {
        currW->addActor(new Vomit(currW, vomitX, vomitY));
        currW->playSound(SOUND_ZOMBIE_VOMIT);
        return;
    }
    
    if (getMovementPlan() == 0)
    {
        setMPlan(randInt(3, 10));
        Actor* closest_human = currW->getClosestHuman(currX, currY);
        double humanX = closest_human->getX(), humanY = closest_human->getY();
        double dist_human = this->distance(humanX, humanY);
        if (dist_human > 80)
        {
            this->setDirection(randInt(0, 3)*90);
        }
        else
        {
            if (currX == humanX)
            {
                if (currY<humanY)
                    this->setDirection(up);
                else
                    this->setDirection(down);
            }
            else if (currY == humanY)
            {
                if (currX<humanX)
                    this->setDirection(right);
                else
                    this->setDirection(left);
            }
        }
    }
    double dist_x = 0, dist_y = 0;
    switch (this->getDirection()) {
        case right:
            dist_x = 1;
            break;
        case left:
            dist_x = -1;
            break;
        case up:
            dist_y = 1;
            break;
        case down:
            dist_y = -1;
            break;
        default:
            break;
    }
    if (!currW->doesBlockMovement(currX+dist_x, currY+dist_y, this))
    {
        //        cout<<"dist_x "<<dist_x<<" dist_y "<<dist_y<<endl;
        this->moveTo(currX+dist_x, currY+dist_y);
        decrementMPlan();
    }
    else
        setMPlan(0);
    
}

///////////////////////////////////////////////////////////////
//                       Landmine                            //
///////////////////////////////////////////////////////////////


Landmine::Landmine(StudentWorld* sw, double startX, double startY):m_isActive(false),m_safetyTick(30),Actor(sw, IID_LANDMINE, startX, startY, right, 1)
    {}

void Landmine::doSomething()
{
    if (!isAlive())
        return;
    if (!m_isActive)
    {
        m_safetyTick--;
        if (m_safetyTick == 0)
        {
            m_isActive = true;
        }
        return;
    }
    Actor* actor = this->getWorld()->doesOverlapWithAnyActor(this);
    if (actor)
    {
        if (actor->getType() == e_penelope || actor->getType() == e_zombie || actor->getType() == e_human || actor->getType() == e_flame)
        {
            StudentWorld* currW = this->getWorld();
            double currX = this->getX(), currY = this->getY();
            setDead();
            currW->playSound(SOUND_LANDMINE_EXPLODE);
            if (!currW->doesBlockFire(currX, currY))
                currW->addActor(new Flame(currW, currX, currY));
            if (!currW->doesBlockFire(currX-SPRITE_WIDTH, currY))
                currW->addActor(new Flame(currW, currX-SPRITE_WIDTH, currY));
            if (!currW->doesBlockFire(currX+SPRITE_WIDTH, currY))
                currW->addActor(new Flame(currW, currX+SPRITE_WIDTH, currY));
            if (!currW->doesBlockFire(currX, currY-SPRITE_HEIGHT))
                currW->addActor(new Flame(currW, currX, currY-SPRITE_HEIGHT));
            if (!currW->doesBlockFire(currX, currY+SPRITE_HEIGHT))
                currW->addActor(new Flame(currW, currX, currY+SPRITE_HEIGHT));
            if (!currW->doesBlockFire(currX+SPRITE_WIDTH, currY+SPRITE_HEIGHT))
                currW->addActor(new Flame(currW, currX+SPRITE_WIDTH, currY+SPRITE_HEIGHT));
            if (!currW->doesBlockFire(currX+SPRITE_WIDTH, currY-SPRITE_HEIGHT))
                currW->addActor(new Flame(currW, currX+SPRITE_WIDTH, currY-SPRITE_HEIGHT));
            if (!currW->doesBlockFire(currX-SPRITE_WIDTH, currY+SPRITE_HEIGHT))
                currW->addActor(new Flame(currW, currX-SPRITE_WIDTH, currY+SPRITE_HEIGHT));
            if (!currW->doesBlockFire(currX-SPRITE_WIDTH, currY-SPRITE_HEIGHT))
                currW->addActor(new Flame(currW, currX-SPRITE_WIDTH, currY-SPRITE_HEIGHT));
            currW->addActor(new Pit(currW, currX, currY));
        }
    }
}
