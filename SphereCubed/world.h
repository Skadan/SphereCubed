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
// Physics.h header file required for Physics.
#include "physics.h"
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
    //! \return Light reference to the Light functionality.
    //! \sa Light
    Light & light() { return mLight; }

    //! Access function to get reference to Menu functionality.
    //! \return Menu reference to the Menu functionality.
    //! \sa Menu
    Menu & menu() { return mMenu; }

    //! Access function to get reference to Physics functionality.
    //! \return Physics reference to the Menu functionality.
    //! \sa Menu
    Physics & physics() { return mPhysics; }

    //! Access function to get reference to Player functionality.
    //! \return Player reference to the Player functionality.
    //! \sa Player
    Player & player() { return mPlayer; }

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
    World( int timeInterval );

    //! World class destructor.
    ~World();

private: // Member Variables
    //! Used to store the Camera functionality.
    Camera mCamera;

    //! Used to store the Level functionality.
    Level mLevel;

    //! Used to store the Light functionality.
    Light mLight;

    //! Used to store the Menu functionality.
    Menu mMenu;

    //! Used to store the Physics functionality.
    Physics mPhysics;

    //! Used to store the Player functionality.
    Player mPlayer;

    //! Used to trigger the Return Key Event.
    bool mReturnKeyPressed = false;

    //! Used to trigger the Space Key Event.
    bool mSpaceKeyPressed  = false;
}; // class World

#endif // WORLD_H
