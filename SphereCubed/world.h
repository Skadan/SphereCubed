/*! \file world.h
\brief World class declaration file.
This file contains the World class declaration for the application.
*/

#ifndef WORLD_H
#define WORLD_H

// camera.h header file required for Camera Machine.
#include "camera.h"
// level.h header file required for Level.
#include "level.h"
// light.h header file required for Light.
#include "light.h"
// Menu.h header file required for Menu.
#include "menu.h"
// Player.h header file required for Player.
#include "player.h"

//! World class.
/*!
This class declaration encapsulates the World functionality.
The Engine instantiates a World to manage the game components.
*/
class World
{
    //! Friend of Engine so that only Engine can instantiate the World.
    friend class Engine;

public: // Member Functions
    //! Access function to get reference to Camera functionality.
    //! \return Camera reference to the Camera functionality.
    //! \sa Camera
    Camera & camera() { return mCamera; }

    //! \todo Refactor into the Player Class
    //! Access function to get Player Died condition trigger.
    //! \return bool is the trigger state.
    //! \sa Player
    bool getDied() { return mDied; }

    //! \todo- Refactor into the Level Class
    //! Access function to get Player Finished condition trigger.
    //! \return bool is the trigger state.
    //! \sa Level
    bool getFinished() { return mFinished; }

    //! \todo Refactor into the Level Class
    //! Access function to get Level Last condition trigger.
    //! \return bool is the trigger state.
    //! \sa Level
    bool getLastLevel() { return mLastLevel; }

    //! \todo Refactor into the Player Class
    //! Access function to get Player Last Life condition trigger.
    //! \return bool is the state.
    //! \sa Player
    bool getLastLife() { return mLastLife; }

    //! Access function to get Return Key condition trigger.
    //! \return bool is the state.
    //! \sa Game
    bool getReturn() { return mReturnKeyPressed; }

    //! Access function to get Space Key condition trigger.
    //! \return bool is the trigger state.
    //! \sa Game
    bool getSpace() { return mSpaceKeyPressed; }

    //! Access function to get reference to Level functionality.
    //! \return Menureference to the Level functionality.
    //! \sa Level
    Level & level() { return mLevel; }

    //! Access function to get reference to Light functionality.
    //! \return Menureference to the Light functionality.
    //! \sa Light
    Light & light() { return mLight; }

    //! Access function to get reference to Menu functionality.
    //! \return Menureference to the Menu functionality.
    //! \sa Menu
    Menu & menu() { return mMenu; }

    //! Access function to get reference to Player functionality.
    //! \return Menureference to the Player functionality.
    //! \sa Player
    Player & player() { return mPlayer; }

    //! \todo Refactor into the Player Class
    //! Access function to set the Player Died condition trigger.
    //! \param value is the state to set trigger to.
    //! \return void
    //! \sa Player
    void setDied( bool value ) { mDied = value; }

    //! \todo Refactor into the Level Class
    //! Access function to set the Player Finished condition trigger.
    //! \param value is the state to set trigger to.
    //! \return void
    //! \sa Level
    void setFinished( bool value ) { mFinished = value; }

    //! \todo Refactor into the Level Class
    //! Access function to set the Level Last condition trigger.
    //! \param value is the state to set trigger to.
    //! \return void
    //! \sa Level
    void setLastLevel( bool value ) { mLastLevel = value; }

    //! \todo Refactor into the Player Class
    //! Access function to set the Player Last Life condition trigger.
    //! \param value is the state to set trigger to.
    //! \return void
    //! \sa Player
    void setLastLife( bool value ) { mLastLife = value; }

    //! Access function to set the Return Key condition trigger.
    //! \param value is the state to set trigger to.
    //! \return void
    //! \sa Game
    void setReturn( bool value ) { mReturnKeyPressed = value; }

    //! Access function to set the Space Key condition trigger.
    //! \param value is the state to set trigger to.
    //! \return void
    //! \sa Game
    void setSpace( bool value ) { mSpaceKeyPressed = value; }

private: // Constructors / Destructors
    //! World class constructor.
    World();

    //! World class destructor.
    ~World();

private: // Member Variables
    //! Used to store the Camera functionality.
    Camera mCamera;

    //! \todo Refactor into the Player Class
    //! Used to trigger the Player Died Event.
    bool mDied;

    //! \todo Refactor into the Level Class
    //! Used to trigger the Player Finished Event.
    bool mFinished;

    //! \todo Refactor into the Level Class
    //! Used to trigger the Level Last Event.
    bool mLastLevel;

    //! \todo Refactor into the Player Class
    //! Used to trigger the Player Last Life Event.
    bool mLastLife;

    //! Used to store the Level functionality.
    Level mLevel;

    //! Used to store the Light functionality.
    Light mLight;

    //! Used to store the Menu functionality.
    Menu mMenu;

    //! Used to store the Player functionality.
    Player mPlayer;

    //! Used to trigger the Return Key Event.
    bool mReturnKeyPressed;

    //! Used to trigger the Space Key Event.
    bool mSpaceKeyPressed;
}; // class World

#endif // WORLD_H
