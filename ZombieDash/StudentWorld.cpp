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
    loadLevel(getLevel());
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::loadLevel(int level){
    Level lev(assetPath());
    string levelStr = to_string(level);
    levelStr = level < 10 ? "0"+levelStr : levelStr;
    string levelFile = "level"+levelStr+".txt";
//    string levelFile = "level04.txt";
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
                        //TODO: Spawn a Smart Zombie
                        break;
                    case Level::dumb_zombie:
                        //TODO: Spawn a Dumb Zombie
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
                        // etc…
                    case Level::citizen:
                        //TODO: Spawn a Citizen
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

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
//
//    // The term "actors" refers to all zombies, Penelope, goodies,
//    // pits, flames, vomit, landmines, etc.
//    // Give each actor a chance to do something, including Penelope
    if (m_penelope->isAlive())
        m_penelope->doSomething();
    else
        return GWSTATUS_PLAYER_DIED;
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
    {
        if ((*iter)->isAlive())
        {
            // tell each actor to do something (e.g. move)
            (*iter)->doSomething();
            if (!m_penelope->isAlive())
                return GWSTATUS_PLAYER_DIED;
            
            //Penelope completed the current level
//            if ()
//                return GWSTATUS_FINISHED_LEVEL;
        }
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();)
        {
            //TODO: finish this shit
            if (*iter && !(*iter)->isAlive())
            {
                Actor* tempIter = *iter;
                delete tempIter;
                tempIter = nullptr;
                m_actors.erase(iter);
            }
            iter++;
        }
    }
    // Update the game status line
    //    Update Display Text
    // update the score/lives/level text at screen top
    // the player hasn’t completed the current level and hasn’t died, so
    // continue playing the current level
    updateGameStats();
    return GWSTATUS_CONTINUE_GAME;
//    return GWSTATUS_PLAYER_DIED;
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
//    if (m_penelope != nullptr)
    delete m_penelope;
    m_penelope = nullptr;
}

void StudentWorld::setPenelope(Penelope *p)
{
    m_penelope = p;
}
                
Actor* StudentWorld::getActorAt(int x, int y)
{
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
    {
        if ((*iter)->getX() == x && (*iter)->getY() == y)
            return *iter;
    }
    return nullptr;
}

bool StudentWorld::doesBlockMovement(int x, int y)
{
    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
    {
        int posX = (*iter)->getX(), posY = (*iter)->getY();
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

//Actor* StudentWorld::collides(Actor* actor)
//{
//    //TODO: need to fiX!!!!!
//    for (vector<Actor*>::iterator iter = m_actors.begin();iter!=m_actors.end();iter++)
//    {
//        if ((*iter) != actor)
//        {
//            if (actor->overlap(**iter))
//                return *iter;
//        }
//    }
//    return nullptr;
//}

Penelope* StudentWorld::getPenelope()
{
    return m_penelope;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

//Score: 004500  Level: 27  Lives: 3  Vaccines:    2        Flames:    16        Mines:    1        Infected:    0
void StudentWorld::updateGameStats()
{
    string twoSpaces = "  ";
    ostringstream gameStatus;
    gameStatus << "Score: ";
    if (getScore() < 0)
        gameStatus << "-";
    gameStatus <<setw(6) <<getScore();
    gameStatus.fill('0');
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
    gameStatus << "Mines: ";
    gameStatus << this->getPenelope()->getLandmineCount();
    gameStatus << twoSpaces;
    gameStatus << "Infected: ";
    gameStatus << this->getPenelope()->getInfectCount();
    setGameStatText(gameStatus.str());
}
