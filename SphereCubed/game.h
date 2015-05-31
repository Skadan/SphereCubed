/*! \file game.h
\brief Game class declaration file.
This file contains the Game class declaration for the application.
*/
#ifndef GAME_H
#define GAME_H

// machine.h header file required for Machine class definition.
#include "machine.h"

// world.h header file required for World class definition.
#include "world.h"


//------------------------------------------------------------------------------
// States


//! GameDiedState class.
/*!
This class declaration encapsulates the GameDiedState functionality.
*/
class GameDiedState : public State
{
    //! Friend of Game so that only Game can instantiate the GameDiedState.
    friend class Game;

private: // Constructors / Destructors
    //! GameDiedState class constructor.
    GameDiedState( World & world );

    //! GameDiedState class destructor.
    ~GameDiedState();

private: // Member Function
    //! Triggered when entering.
    virtual void enter();

    //! Triggered when an event message is received.
    virtual bool event( QEvent * event );

    //! Triggered when exiting.
    virtual void exit();

    //! Triggered periodically.
    virtual void render();

    //! Triggered periodically.
    virtual void tick();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GameDiedState

//! GameFinishedState class.
/*!
This class declaration encapsulates the GameFinishedState functionality.
*/
class GameFinishedState : public State
{
    //! Friend of Game so that only Game can instantiate the GameFinishedState.
    friend class Game;

private: // Constructors / Destructors
    //! GameFinishedState class constructor.
    GameFinishedState( World & world );

    //! GameFinishedState class destructor.
    ~GameFinishedState();

private: // Member Function
    //! Triggered when entering.
    virtual void enter();

    //! Triggered when an event message is received.
    virtual bool event( QEvent * event );

    //! Triggered when exiting.
    virtual void exit();

    //! Triggered periodically.
    virtual void render();

    //! Triggered periodically.
    virtual void tick();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GameFinishedState

//! GameMenuState class.
/*!
This class declaration encapsulates the GameMenuState functionality.
*/
class GameMenuState : public State
{
    //! Friend of Game so that only Game can instantiate the GameMenuState.
    friend class Game;

private: // Constructors / Destructors
    //! GameMenuState class constructor.
    GameMenuState( World & world );

    //! GameMenuState class destructor.
    ~GameMenuState();

private: // Member Function
    //! Triggered when entering.
    virtual void enter();

    //! Triggered when an event message is received.
    virtual bool event( QEvent * event );

    //! Triggered when exiting.
    virtual void exit();

    //! Triggered periodically.
    virtual void render();

    //! Triggered periodically.
    virtual void tick();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GameMenuState

//! GameOverState class.
/*!
This class declaration encapsulates the GameOverState functionality.
*/
class GameOverState : public State
{
    //! Friend of Game so that only Game can instantiate the GameOverState.
    friend class Game;

private: // Constructors / Destructors
    //! GameOverState class constructor.
    GameOverState( World & world );

    //! GameOverState class destructor.
    ~GameOverState();

private: // Member Function
    //! Triggered when entering.
    virtual void enter();

    //! Triggered when an event message is received.
    virtual bool event( QEvent * event );

    //! Triggered when exiting.
    virtual void exit();

    //! Triggered periodically.
    virtual void render();

    //! Triggered periodically.
    virtual void tick();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GameOverState

//! GamePlayState class.
/*!
This class declaration encapsulates the GamePlayState functionality.
*/
class GamePlayState : public State
{
    //! Friend of Game so that only Game can instantiate the GamePlayState.
    friend class Game;

private: // Constructors / Destructors
    //! GamePlayState class constructor.
    GamePlayState( World & world );

    //! GamePlayState class destructor.
    ~GamePlayState();

private: // Member Function
    //! Triggered when entering.
    virtual void enter();

    //! Triggered when an event message is received.
    virtual bool event( QEvent * event );

    //! Triggered when exiting.
    virtual void exit();

    //! Triggered periodically.
    virtual void render();

    //! Triggered periodically.
    virtual void tick();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GamePlayState

//! GameWonState class.
/*!
This class declaration encapsulates the GameWonState functionality.
*/
class GameWonState : public State
{
    //! Friend of Game so that only Game can instantiate the GameWonState.
    friend class Game;

private: // Constructors / Destructors
    //! GameWonState class constructor.
    GameWonState( World & world );

    //! GameWonState class destructor.
    ~GameWonState();

private: // Member Function
    //! Triggered when entering.
    virtual void enter();

    //! Triggered when an event message is received.
    virtual bool event( QEvent * event );

    //! Triggered when exiting.
    virtual void exit();

    //! Triggered periodically.
    virtual void render();

    //! Triggered periodically.
    virtual void tick();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GameWonState



//------------------------------------------------------------------------------
// Events


//! GameDiedEvent class.
/*!
This class declaration encapsulates the GameDiedEvent functionality.
*/
class GameDiedEvent : public Event
{
    //! Friend of Game so that only Game can instantiate the GameDiedEvent.
    friend class Game;

private: // Constructors / Destructors
    //! GameDiedEvent class constructor.
    GameDiedEvent( World & world );

    //! GameDiedEvent class destructor.
    ~GameDiedEvent();

private: // Member Function
    //! Test Event condition triggered.
    virtual State * test();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GameDiedEvent

//! GameReturnEvent class.
/*!
This class declaration encapsulates the GameReturnEvent functionality.
*/
class GameReturnEvent : public Event
{
    //! Friend of Game so that only Game can instantiate the GameReturnEvent.
    friend class Game;

private: // Constructors / Destructors
    //! GameReturnEvent class constructor.
    GameReturnEvent( World & world );

    //! GameReturnEvent class destructor.
    ~GameReturnEvent();

private: // Member Function
    //! Test Event condition triggered.
    virtual State * test();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GameReturnEvent

//! GameFinishedEvent class.
/*!
This class declaration encapsulates the GameFinishedEvent functionality.
*/
class GameFinishedEvent : public Event
{
    //! Friend of Game so that only Game can instantiate the GameFinishedEvent.
    friend class Game;

private: // Constructors / Destructors
    //! GameFinishedEvent class constructor.
    GameFinishedEvent( World & world );

    //! GameFinishedEvent class destructor.
    ~GameFinishedEvent();

private: // Member Function
    //! Test Event condition triggered.
    virtual State * test();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GameFinishedEvent

//! GameLastLevelEvent class.
/*!
This class declaration encapsulates the GameLastLevelEvent functionality.
*/
class GameLastLevelEvent : public Event
{
    //! Friend of Game so that only Game can instantiate the GameLastLevelEvent.
    friend class Game;

private: // Constructors / Destructors
    //! GameLastLevelEvent class constructor.
    GameLastLevelEvent( World & world );

    //! GameLastLevelEvent class destructor.
    ~GameLastLevelEvent();

private: // Member Function
    //! Test Event condition triggered.
    virtual State * test();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GameLastLevelEvent

//! GameLastLifeEvent class.
/*!
This class declaration encapsulates the GameLastLifeEvent functionality.
*/
class GameLastLifeEvent : public Event
{
    //! Friend of Game so that only Game can instantiate the GameLastLifeEvent.
    friend class Game;

private: // Constructors / Destructors
    //! GameLastLifeEvent class constructor.
    GameLastLifeEvent( World & world );

    //! GameLastLifeEvent class destructor.
    ~GameLastLifeEvent();

private: // Member Function
    //! Test Event condition triggered.
    virtual State * test();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GameLastLifeEvent

//! GameSpaceEvent class.
/*!
This class declaration encapsulates the GameSpaceEvent functionality.
*/
class GameSpaceEvent : public Event
{
    //! Friend of Game so that only Game can instantiate the GameSpaceEvent.
    friend class Game;

private: // Constructors / Destructors
    //! GameSpaceEvent class constructor.
    GameSpaceEvent( World & world );

    //! GameSpaceEvent class destructor.
    ~GameSpaceEvent();

private: // Member Function
    //! Test Event condition triggered.
    virtual State * test();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class GameSpaceEvent



//------------------------------------------------------------------------------
// Machine


//! Game class.
/*!
This class declaration encapsulates the Game functionality.
The Engine instantiates a Game to manage the play states.
*/
class Game : public Machine
{
    //! Friend of Engine so that only Engine can instantiate the Game.
    friend class Engine;

private: // Constructors / Destructors
    //! Game class constructor.
    Game( World & world );

    //! Game class destructor.
    ~Game();

private: // Member Function
    //! Configure.
    void configure();

private: // Member Variables
    //! Used to reference the World functionality.
    World & mWorld;
}; // class Game

#endif // GAME_H

