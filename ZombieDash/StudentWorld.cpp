#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    m_actors = vector<Actor*>();
    m_penelope = nullptr;
    m_exitIsSteppedOn = false;
    loadLevel(getLevel());
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::loadLevel(int level){
    Level lev(assetPath());
    string levelStr = to_string(level);
    levelStr = level < 10 ? "0"+levelStr : levelStr;
    string levelFile = "level"+levelStr+".txt";
//    string levelFile = "level04.txt"; //for testing purposes only
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result == Level::load_fail_file_not_found)
        cerr << "Cannot find"<<levelFile<<" data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "Your level was improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        for (int x = 0;x<VIEW_WIDTH;x++)
        {
            for (int y = 0;y<VIEW_HEIGHT;y++)
            {
                Level::MazeEntry ge = lev.getContentsOf(x,y); // level_x=x, level_y=y
                int startX = x*SPRITE_WIDTH, startY = y*SPRITE_HEIGHT;
                switch (ge) // so x=80 and y=160
                {
                    case Level::empty:
                        break;
                    case Level::smart_zombie:
                        m_actors.push_back(new SmartZombie(this, startX, startY));
                        break;
                    case Level::dumb_zombie:
                        m_actors.push_back(new DumbZombie(this, startX, startY));
                        break;
                    case Level::player:
                        m_penelope = new Penelope(this, startX, startY);
                        break;
                    case Level::exit:
                        m_actors.push_back(new Exit(this, startX, startY));
                        break;
                    case Level::wall:
                        m_actors.push_back(new Wall(this, startX, startY));
                        break;
                    case Level::pit:
                        m_actors.push_back(new Pit(this, startX, startY));
                        break;
                    case Level::citizen:
                        m_actors.push_back(new Citizen(this, startX, startY));
                        break;
                    case Level::vaccine_goodie:
                        m_actors.push_back(new VaccineGoodie(this, startX, startY));
                        break;
                    case Level::gas_can_goodie:
                        m_actors.push_back(new GasCanGoodie(this, startX, startY));
                        break;
                    case Level::landmine_goodie:
                        m_actors.push_back(new LandmineGoodie(this, startX, startY));
                        break;
                }
            }
        }
    }
}

void StudentWorld::addActor(Actor* actor)
{
    m_actors.push_back(actor);
}

int StudentWorld::move()
{
    if (m_penelope->isAlive())
        m_penelope->doSomething();
    
    for(Actor* actor: m_actors){
        //PENELOPE DIED
        if(!m_penelope->isAlive()){
            if (getLives()>0)
                decLives();
            playSound(SOUND_PLAYER_DIE);
            return GWSTATUS_PLAYER_DIED;
        }
        //DO SOMETHING IF ALIVE
        if(actor && actor->isAlive())
            actor->doSomething();
        //LEVEL COMPLETE
        if(numberOfCitizensLeft() == 0 && m_exitIsSteppedOn){
            playSound(SOUND_LEVEL_FINISHED);
            //PLAYER WON
            if (getLevel() == 6)
                return GWSTATUS_PLAYER_WON;
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    //CLEANUP ALL THE ACTORS
    vector<Actor*>::iterator it = m_actors.begin();

    while(it != m_actors.end()){
        if(!(*it)->isAlive()){
            delete *it;
            it = m_actors.erase(it);
        }
        else{
            it++;
        }
    }
    //
    updateGameStats();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (!m_actors.empty())
    {
        for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
        {
            delete *iter;
            *iter = nullptr;
        }
    }
    delete m_penelope;
    m_penelope = nullptr;
}

void StudentWorld::setPenelope(Penelope *p)
{
    m_penelope = p;
}
                
Actor* StudentWorld::doesOverlapWithAnyActor(Actor* notThisActor)
{
    if (notThisActor!=m_penelope && m_penelope->overlap(notThisActor))
        return m_penelope;
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
    {
        if ((*iter) && notThisActor!=(*iter) && (*iter)->overlap(notThisActor))
            return *iter;
    }
    return nullptr;
}

bool StudentWorld::doesBlockMovement(double x, double y, Actor* actor)
{
    if (actor != m_penelope)
    {
        for (int i = 0;i<1;i++)
        {
            double posX = m_penelope->getX(), posY = m_penelope->getY();
            if (x + SPRITE_WIDTH - 1 < posX || posX + SPRITE_WIDTH - 1 < x)
                continue;
            if (y + SPRITE_HEIGHT - 1 < posY || posY + SPRITE_HEIGHT - 1 < y)
                continue;
            return true;
        }
    }
    
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
    {
        double posX = (*iter)->getX(), posY = (*iter)->getY();
        if (*iter == actor)
            continue;
        if (!(*iter)->isBlocked())
            continue;
        if (x + SPRITE_WIDTH - 1 < posX || posX + SPRITE_WIDTH - 1 < x)
            continue;
        if (y + SPRITE_HEIGHT - 1 < posY || posY + SPRITE_HEIGHT - 1 < y)
            continue;
        return true;
    }

    return false;
}


bool StudentWorld::doesBlockFire(double x, double y)
{
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
    {
        double posX = (*iter)->getX(), posY = (*iter)->getY();
        if ((*iter)->getType() == e_human)
            continue;
        if ((*iter)->getType() == e_zombie)
            continue;
        if ((*iter)->getType() == e_exit)
            continue;
        if (!(*iter)->isBlocked())
            continue;
        if (x + SPRITE_WIDTH - 1 < posX || posX + SPRITE_WIDTH - 1 < x)
            continue;
        if (y + SPRITE_HEIGHT - 1 < posY || posY + SPRITE_HEIGHT - 1 < y)
            continue;
        return true;
    }
    return false;
}

bool StudentWorld::doesBlockGoodie(double x, double y)
{
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
    {
        double posX = (*iter)->getX(), posY = (*iter)->getY();
        if (x + SPRITE_WIDTH - 1 < posX || posX + SPRITE_WIDTH - 1 < x)
            continue;
        if (y + SPRITE_HEIGHT - 1 < posY || posY + SPRITE_HEIGHT - 1 < y)
            continue;
        return true;
    }
    return false;
}

Penelope* StudentWorld::getPenelope()
{
    return m_penelope;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

double StudentWorld::getClosestZombie(double x, double y)
{
    double distance = -1; //default distance
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
    {
        if ((*iter) && (*iter)->getType() == ActorType::e_zombie)
        {
            if (distance == -1 || (*iter)->distance(x, y) < distance)
            {
                distance = (*iter)->distance(x, y);
            }
        }
    }
    return distance;
}

Actor* StudentWorld::getClosestHuman(double x, double y)
{
    Actor* temp_human = nullptr;
    double distance = -1; //default distance
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
    {
        if ((*iter)->getType() == ActorType::e_human)
        {
            if (distance == -1 || (*iter)->distance(x, y) < distance)
            {
                distance = (*iter)->distance(x, y);
                temp_human = *iter;
            }
        }
    }
    
    if (distance > m_penelope->distance(x, y))
    {
        distance = m_penelope->distance(x, y);
        temp_human = m_penelope;
    }
    return temp_human;
}

int StudentWorld::numberOfCitizensLeft()
{
    int count = 0;
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
    {
        if (!(*iter))
            continue;
        if ((*iter)->getType() == e_human)
            count++;
    }
    return count;
}

void StudentWorld::exitIsSteppedOn(bool isSteppedOn)
{
    m_exitIsSteppedOn = isSteppedOn;
}

bool StudentWorld::isHumanInfrontOfZombie(double x, double y, int direction)
{
    if (m_penelope->distance(x, y)<=10)
        return true;
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
    {
        if ((*iter) && (*iter)->getType() == e_human && (*iter)->distance(x, y)<=10)
            return true;
    }
    return false;
}

void StudentWorld::updateGameStats()
{
    string twoSpaces = "  ";
    ostringstream gameStatus;
    gameStatus << "Score: ";
    if (getScore() < 0)
        gameStatus << "-";
    int score = getScore() >= 0? getScore():-getScore();
    gameStatus.fill('0');
    gameStatus <<setw(6) <<score;
    gameStatus << twoSpaces;
    gameStatus << "Level: ";
    gameStatus << getLevel();
    gameStatus << twoSpaces;
    gameStatus << "Lives: ";
    gameStatus << getLives();
    gameStatus << twoSpaces;
    gameStatus << "Vaccines: ";
    gameStatus << this->getPenelope()->getVaccineCount();
    gameStatus << twoSpaces;
    gameStatus << "Flames: ";
    gameStatus << this->getPenelope()->getFlameCount();
    gameStatus << twoSpaces;
    gameStatus << "Mines: ";
    gameStatus << this->getPenelope()->getLandmineCount();
    gameStatus << twoSpaces;
    gameStatus << "Infected: ";
    gameStatus << this->getPenelope()->getInfectCount();
    setGameStatText(gameStatus.str());
}
