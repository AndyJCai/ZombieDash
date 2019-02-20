#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
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
    if (level < 10)
    {
        levelStr = "0"+levelStr;
    }
    string levelFile = "level"+levelStr+".txt";
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
                        //TODO: Spawn a Empty Spot
                        cout << "Location "<<startX<<", "<<startY<<" is empty" << endl;
                        break;
                    case Level::smart_zombie:
                        //TODO: Spawn a Smart Zombie
                        cout << "Location "<<startX<<", "<<startY<<" starts with a smart zombie" << endl;
                        break;
                    case Level::dumb_zombie:
                        //TODO: Spawn a Dumb Zombie
                        cout << "Location "<<startX<<", "<<startY<<" starts with a dumb zombie" << endl;
                        break;
                    case Level::player:
                        //TODO: Spawn a Penelope
                        m_penelope = new Penelope(this, startX, startY);
                        cout << "Location "<<startX<<", "<<startY<<" is where Penelope starts" << endl;
                        break;
                    case Level::exit:
                        //TODO: Spawn an Exit
                        cout << "Location "<<startX<<", "<<startY<<" is where an exit is" << endl;
                        break;
                    case Level::wall:
                        m_actors.push_back(new Wall(this, startX, startY));
                        cout << "Location "<<startX<<", "<<startY<<" holds a Wall" << endl;
                        break;
                    case Level::pit:
                        cout << "Location "<<startX<<", "<<startY<<" has a pit in the ground" << endl;
                        break;
                        // etc…
                    case Level::citizen:
                        cout << "Location "<<startX<<", "<<startY<<" starts with a citizen" << endl;
                        break;
                    case Level::vaccine_goodie:
                        cout << "Location "<<startX<<", "<<startY<<" is where Vaccine Goodie" <<endl;
                        break;
                    case Level::gas_can_goodie:
                        cout << "Location "<<startX<<", "<<startY<<" is where Gas Can Goodie" <<endl;
                        break;
                    case Level::landmine_goodie:
                        cout << "Location "<<startX<<", "<<startY<<" is where a Landmine Goodie." <<endl;
                        break;
                }
            }
        }
    }
}

int StudentWorld::move()
{
//    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
//    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
//
//    // The term "actors" refers to all zombies, Penelope, goodies,
//    // pits, flames, vomit, landmines, etc.
//    // Give each actor a chance to do something, including Penelope
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
    }
//    // Remove newly-dead actors after each tick
//    // Update the game status line
//    for (int i = 0;i<m_actors.size();i++)
//    {
//
//    }
//////    Update Display Text // update the score/lives/level text at screen top
////    // the player hasn’t completed the current level and hasn’t died, so
////    // continue playing the current level
    return GWSTATUS_CONTINUE_GAME;
    decLives();
//    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
    m_actors.clear();
}

void StudentWorld::setPenelope(Actor *p)
{
    m_penelope = p;
}

void StudentWorld::addActor(Actor *p)
{
    m_actors.push_back(p);
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

bool StudentWorld::doesOverlap(int x, int y)
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

