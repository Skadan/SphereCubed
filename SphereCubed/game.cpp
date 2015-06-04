/*! \file game.cpp
\brief Game class implementation file.
This file contains the Game class implementation for the application.
*/

// game.h header file required for Game class definition.
#include "game.h"


// QKeyEvent header file required for processing key events.
#include <QKeyEvent>


// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION false


//------------------------------------------------------------------------------
// States


// GameDiedState ---------------------------------------------------------------

//! The GameDiedState execution begins here.
//! Create a GameDiedState and pass it a reference to the World data.
//! Passes a name for the state as a QString to the base class.
//! \param world is a reference to the World data.
GameDiedState::GameDiedState( World & world ) :
    State( "GameDiedState" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameDiedState( World & world )...";
} // GameDiedState::GameDiedState( World & world )

//! The GameDiedState execution ends here.
//! Destroy the GameDiedState.
//! Required to free the GameDiedState allocated memory.
GameDiedState::~GameDiedState()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GameDiedState()...";
} // GameDiedState::~GameDiedState()

//! Enter the State.
//! \return void
//! \sa State, Event, Machine
void GameDiedState::enter()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameDiedState::enter...";

    //! Set the Camera Menu flag to trigger the Menu Event to Switch to the Menu State.
    mWorld.camera().setMenu( true );

    //! Load the Menu with the died background image.
    mWorld.menu().load(":ImageMenuDied");
} // GameDiedState::enter()

//! Process received events.
//! \param event is a pointer to a QEvent argument identifing the event.
//! \return bool is set to true when processed.
//! \sa QObject, Machine, State
bool GameDiedState::event( QEvent * event )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameDiedState::event( QEvent * event)...";

    //! Return value, assume not processed until processed.
    bool rVal = false;

    //! Determine what type of event was received.
    switch( event->type() )
    {
    //! When it is a KeyRelease event.
    case QEvent::KeyRelease:
        //! Determine what key was released.
        switch( ((QKeyEvent*)event)->key() )
        {
        //! When it is the space key. Set flag in World. Set return value to processed.
        case Qt::Key_Space:
            TraceOut( TRACE_FILE_EXECUTION ) << "Space Key - Set flag in World...";
            mWorld.setSpace( true );
            rVal = true;
            break;

        //! When it is any other key. Do nothing.
        default:
            TraceOut( TRACE_FILE_EXECUTION ) << "Key " << ((QKeyEvent*)event)->key() << " NOT processed in GameDiedState...";
            break;
        } // switch( ((QKeyEvent*)event)->key() )
        break;

    //! When it is any other event. Do nothing.
    default:
        TraceOut( TRACE_FILE_EXECUTION ) << "Event " << event->type() << " NOT processed in GameDiedState...";
        break;
    } // switch( event->type() )

    //! Return status of event processing.
    return rVal;
} // GameDiedState::event( QEvent * event )

//! Exit the State.
//! \return void
//! \sa State, Event, Machine
void GameDiedState::exit()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameDiedState::exit...";

    //! Unload the menu image.
    mWorld.menu().unload();
} // GameDiedState::exit()

//! Render the State.
//! \return void
//! \sa State, Event, Machine
void GameDiedState::render()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameDiedState::render...";

    //! Setup the Camera for rendering the Menu.
    mWorld.camera().render();

    //! Render the Menu
    mWorld.menu().render( mWorld.camera() );
} // GameDiedState::render()

//! Update the State.
//! \return void
//! \sa State, Event, Machine
void GameDiedState::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameDiedState::tick...";

    //! Update the Camera
    mWorld.camera().tick();

    //! Update the Menu
    mWorld.menu().tick();
} // GameDiedState::tick()

// GameFinishedState ---------------------------------------------------------------

//! The GameFinishedState execution begins here.
//! Create a GameFinishedState and pass it a reference to the World data.
//! Passes a name for the state as a QString to the base class.
//! \param world is a reference to the World data.
GameFinishedState::GameFinishedState( World & world ) :
    State( "GameFinishedState" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameFinishedState( World & world )...";
} // GameFinishedState::GameFinishedState( World & world )

//! The GameFinishedState execution ends here.
//! Destroy the GameFinishedState.
//! Required to free the GameFinishedState allocated memory.
GameFinishedState::~GameFinishedState()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GameFinishedState()...";
} // GameFinishedState::~GameFinishedState()

//! Enter the State.
//! \return void
//! \sa State, Event, Machine
void GameFinishedState::enter()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameFinishedState::enter...";

    //! Set the Camera Menu flag to trigger the Menu Event to Switch to the Menu State.
    mWorld.camera().setMenu( true );

    //! Load the Menu with the finished background image.
    mWorld.menu().load(":ImageMenuFinished");
} // GameFinishedState::enter()

//! Process received events.
//! \param event is a pointer to a QEvent argument identifing the event.
//! \return bool is set to true when processed.
//! \sa QObject, Machine, State
bool GameFinishedState::event( QEvent * event )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameFinishedState::event( QEvent * event)...";

    //! Return value, assume not processed until processed.
    bool rVal = false;

    //! Determine what type of event was received.
    switch( event->type() )
    {
    //! When it is a KeyRelease event.
    case QEvent::KeyRelease:
        //! Determine what key was released.
        switch( ((QKeyEvent*)event)->key() )
        {
        //! When it is the space key. Set flag in World. Set return value to processed.
        case Qt::Key_Space:
            TraceOut( TRACE_FILE_EXECUTION ) << "Space Key - Set flag in World...";
            mWorld.setSpace( true );
            rVal = true;
            break;

        //! When it is any other key. Do nothing.
        default:
            TraceOut( TRACE_FILE_EXECUTION ) << "Key " << ((QKeyEvent*)event)->key() << " NOT processed in GameFinishedState...";
            break;
        } // switch( ((QKeyEvent*)event)->key() )
        break;

    //! When it is any other event. Do nothing.
    default:
        TraceOut( TRACE_FILE_EXECUTION ) << "Event " << event->type() << " NOT processed in GameFinishedState...";
        break;
    } // switch( event->type() )

    //! Return status of event processing.
    return rVal;
} // GameFinishedState::event( QEvent * event )

//! Exit the State.
//! \return void
//! \sa State, Event, Machine
void GameFinishedState::exit()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameFinishedState::exit...";

    //! Unload the menu image.
    mWorld.menu().unload();
} // GameFinishedState::exit()

//! Render the State.
//! \return void
//! \sa State, Event, Machine
void GameFinishedState::render()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameFinishedState::render...";

    //! Setup the Camera for rendering the Menu.
    mWorld.camera().render();

    //! Render the Menu
    mWorld.menu().render( mWorld.camera() );
} // GameFinishedState::render()

//! Update the State.
//! \return void
//! \sa State, Event, Machine
void GameFinishedState::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameFinishedState::tick...";

    //! Update the Camera
    mWorld.camera().tick();

    //! Update the Menu
    mWorld.menu().tick();
} // GameFinishedState::tick()

// GameMenuState ---------------------------------------------------------------

//! The GameMenuState execution begins here.
//! Create a GameMenuState and pass it a reference to the World data.
//! Passes a name for the state as a QString to the base class.
//! \param world is a reference to the World data.
GameMenuState::GameMenuState( World & world ) :
    State( "GameMenuState" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameMenuState( World & world )...";
} // GameMenuState::GameMenuState( World & world )

//! The GameMenuState execution ends here.
//! Destroy the GameMenuState.
//! Required to free the GameMenuState allocated memory.
GameMenuState::~GameMenuState()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GameMenuState()...";
} // GameMenuState::~GameMenuState()

//! Enter the State.
//! \return void
//! \sa State, Event, Machine
void GameMenuState::enter()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameMenuState::enter...";

    //! Set the Camera Menu flag to trigger the Menu Event to Switch to the Menu State.
    mWorld.camera().setMenu( true );

    //! Load the Menu with the menu background image.
    mWorld.menu().load(":ImageMenuMenu");
} // GameMenuState::enter()

//! Process received events.
//! \param event is a pointer to a QEvent argument identifing the event.
//! \return bool is set to true when processed.
//! \sa QObject, Machine, State
bool GameMenuState::event( QEvent * event )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameMenuState::event( QEvent * event)...";

    //! Return value, assume not processed until processed.
    bool rVal = false;

    //! Determine what type of event was received.
    switch( event->type() )
    {
    //! When it is a KeyRelease event.
    case QEvent::KeyRelease:
        //! Determine what key was released.
        switch( ((QKeyEvent*)event)->key() )
        {
        //! When it is the space key. Set flag in World. Set return value to processed.
        case Qt::Key_Space:
            TraceOut( TRACE_FILE_EXECUTION ) << "Space Key - Set flag in World...";
            mWorld.setSpace( true );
            rVal = true;
            break;

        //! When it is any other key. Do nothing.
        default:
            TraceOut( TRACE_FILE_EXECUTION ) << "Key " << ((QKeyEvent*)event)->key() << " NOT processed in GameMenuState...";
            break;
        } // switch( ((QKeyEvent*)event)->key() )
        break;

    //! When it is any other event. Do nothing.
    default:
        TraceOut( TRACE_FILE_EXECUTION ) << "Event " << event->type() << " NOT processed in GameMenuState...";
        break;
    } // switch( event->type() )

    //! Return status of event processing.
    return rVal;
} // GameMenuState::event( QEvent * event )

//! Exit the State.
//! \return void
//! \sa State, Event, Machine
void GameMenuState::exit()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameMenuState::exit...";

    //! Unload the menu image.
    mWorld.menu().unload();
} // GameMenuState::exit()

//! Render the State.
//! \return void
//! \sa State, Event, Machine
void GameMenuState::render()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameMenuState::render...";

    //! Setup the Camera for rendering the Menu.
    mWorld.camera().render();

    //! Render the Menu
    mWorld.menu().render( mWorld.camera() );
} // GameMenuState::render()

//! Update the State.
//! \return void
//! \sa State, Event, Machine
void GameMenuState::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameMenuState::tick...";

    //! Update the Camera
    mWorld.camera().tick();

    //! Update the Menu
    mWorld.menu().tick();
} // GameMenuState::tick()

// GameOverState ---------------------------------------------------------------

//! The GameOverState execution begins here.
//! Create a GameOverState and pass it a reference to the World data.
//! Passes a name for the state as a QString to the base class.
//! \param world is a reference to the World data.
GameOverState::GameOverState( World & world ) :
    State( "GameOverState" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameOverState( World & world )...";
} // GameOverState::GameOverState( World & world )

//! The GameOverState execution ends here.
//! Destroy the GameOverState.
//! Required to free the GameOverState allocated memory.
GameOverState::~GameOverState()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GameOverState()...";
} // GameOverState::~GameOverState()

//! Enter the State.
//! \return void
//! \sa State, Event, Machine
void GameOverState::enter()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameOverState::enter...";

    //! Set the Camera Menu flag to trigger the Menu Event to Switch to the Menu State.
    mWorld.camera().setMenu( true );

    //! Load the Menu with the over background image.
    mWorld.menu().load(":ImageMenuOver");
} // GameOverState::enter()

//! Process received events.
//! \param event is a pointer to a QEvent argument identifing the event.
//! \return bool is set to true when processed.
//! \sa QObject, Machine, State
bool GameOverState::event( QEvent * event )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameOverState::event( QEvent * event)...";

    //! Return value, assume not processed until processed.
    bool rVal = false;

    //! Determine what type of event was received.
    switch( event->type() )
    {
    //! When it is a KeyRelease event.
    case QEvent::KeyRelease:
        //! Determine what key was released.
        switch( ((QKeyEvent*)event)->key() )
        {
        //! When it is the return key. Set flag in World. Set return value to processed.
        case Qt::Key_Return:
            TraceOut( TRACE_FILE_EXECUTION ) << "Return Key - Set flag in World...";
            mWorld.setReturn( true );
            rVal = true;
            break;

        //! When it is any other key. Do nothing.
        default:
            TraceOut( TRACE_FILE_EXECUTION ) << "Key " << ((QKeyEvent*)event)->key() << " NOT processed in GameOverState...";
            break;
        } // switch( ((QKeyEvent*)event)->key() )
        break;

    //! When it is any other event. Do nothing.
    default:
        TraceOut( TRACE_FILE_EXECUTION ) << "Event " << event->type() << " NOT processed in GameOverState...";
        break;
    } // switch( event->type() )

    //! Return status of event processing.
    return rVal;
} // GameOverState::event( QEvent * event )

//! Exit the State.
//! \return void
//! \sa State, Event, Machine
void GameOverState::exit()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameOverState::exit...";

    //! Unload the menu image.
    mWorld.menu().unload();
} // GameOverState::exit()

//! Render the State.
//! \return void
//! \sa State, Event, Machine
void GameOverState::render()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameOverState::render...";

    //! Setup the Camera for rendering the Menu.
    mWorld.camera().render();

    //! Render the Menu
    mWorld.menu().render( mWorld.camera() );
} // GameOverState::render()

//! Update the State.
//! \return void
//! \sa State, Event, Machine
void GameOverState::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameOverState::tick...";

    //! Update the Camera
    mWorld.camera().tick();

    //! Update the Menu
    mWorld.menu().tick();
} // GameOverState::tick()

// GamePlayState ---------------------------------------------------------------

//! The GamePlayState execution begins here.
//! Create a GamePlayState and pass it a reference to the World data.
//! Passes a name for the state as a QString to the base class.
//! \param world is a reference to the World data.
GamePlayState::GamePlayState( World & world ) :
    State( "GamePlayState" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GamePlayState( World & world )...";
} // GamePlayState::GamePlayState( World & world )

//! The GamePlayState execution ends here.
//! Destroy the GamePlayState.
//! Required to free the GamePlayState allocated memory.
GamePlayState::~GamePlayState()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GamePlayState()...";
} // GamePlayState::~GamePlayState()

//! Enter the State.
//! \return void
//! \sa State, Event, Machine
void GamePlayState::enter()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GamePlayState::enter...";

    //! Set the Camera Play flag to trigger the Play Event to Switch to the Play State.
    mWorld.camera().setPlay( true );

    //! Load the Level.
    mWorld.level().load();

    //! Load the Player.
    mWorld.player().load();

    //! Position the Player at the Level start position.
    mWorld.player().position() = mWorld.level().startPosition();
} // GamePlayState::enter()

//! Process received events.
//! \param event is a pointer to a QEvent argument identifing the event.
//! \return bool is set to true when processed.
//! \sa QObject, Machine, State
bool GamePlayState::event( QEvent * event )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GamePlayState::event( QEvent * event)...";

    //! Return value, assume not processed until processed.
    bool rVal = false;

    //! Determine what type of event was received.
    switch( event->type() )
    {
    //! When it is any other event. Do nothing.
    default:
        TraceOut( TRACE_FILE_EXECUTION ) << "Event " << event->type() << " NOT processed in GamePlayState...";
        rVal = QApplication::sendEvent( &mWorld.player(), event );
        break;
    } // switch( event->type() )

    //! Return status of event processing.
    return rVal;
} // GamePlayState::event( QEvent * event )

//! Exit the State.
//! \return void
//! \sa State, Event, Machine
void GamePlayState::exit()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GamePlayState::exit...";

    //! Unload the Level.
    mWorld.level().unload();

    //! Unload the Player.
    mWorld.player().unload();
} // GamePlayState::exit()

//! Render the State.
//! \return void
//! \sa State, Event, Machine
void GamePlayState::render()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GamePlayState::render...";

    //! Setup the Camera for rendering game play.
    mWorld.camera().render();

    //! Render the Level.
    mWorld.level().render( mWorld.camera(), mWorld.light() );

    //! Render the Player.
    mWorld.player().render( mWorld.camera(), mWorld.light() );
} // GamePlayState::render()

//! Update the State.
//! \return void
//! \sa State, Event, Machine
void GamePlayState::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GamePlayState::tick...";

    //! Update the physics.
    mWorld.physics().tick();

    //! Update the Player.
    mWorld.player().tick();

    //! Update the target position of the Light.
    mWorld.light().target() = mWorld.player().position();
    //! Update the target position of the Camera.
    mWorld.camera().targetPosition() = mWorld.player().position();
    //! Update the target direction of the Camera.
    mWorld.camera().targetDirection() = mWorld.player().direction();

    //! Update the Camera
    mWorld.camera().tick();
} // GamePlayState::tick()

// GameWonState ---------------------------------------------------------------

//! The GameWonState execution begins here.
//! Create a GameWonState and pass it a reference to the World data.
//! Passes a name for the state as a QString to the base class.
//! \param world is a reference to the World data.
GameWonState::GameWonState( World & world ) :
    State( "GameWonState" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameWonState( World & world )...";
} // GameWonState::GameWonState( World & world )

//! The GameWonState execution ends here.
//! Destroy the GameWonState.
//! Required to free the GameWonState allocated memory.
GameWonState::~GameWonState()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GameWonState()...";
} // GameWonState::~GameWonState()

//! Enter the State.
//! \return void
//! \sa State, Event, Machine
void GameWonState::enter()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameWonState::enter...";

    //! Set the Camera Menu flag to trigger the Menu Event to Switch to the Menu State.
    mWorld.camera().setMenu( true );

    //! Load the Menu with the won background image.
    mWorld.menu().load(":ImageMenuWon");
} // GameWonState::enter()

//! Process received events.
//! \param event is a pointer to a QEvent argument identifing the event.
//! \return bool is set to true when processed.
//! \sa QObject, Machine, State
bool GameWonState::event( QEvent * event )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameWonState::event( QEvent * event)...";

    //! Return value, assume not processed until processed.
    bool rVal = false;

    //! Determine what type of event was received.
    switch( event->type() )
    {
    //! When it is a KeyRelease event.
    case QEvent::KeyRelease:
        //! Determine what key was released.
        switch( ((QKeyEvent*)event)->key() )
        {
        //! When it is the return key. Set flag in World. Set return value to processed.
        case Qt::Key_Return:
            TraceOut( TRACE_FILE_EXECUTION ) << "Return Key - Set flag in World...";
            mWorld.setReturn( true );
            rVal = true;
            break;

        //! When it is any other key. Do nothing.
        default:
            TraceOut( TRACE_FILE_EXECUTION ) << "Key " << ((QKeyEvent*)event)->key() << " NOT processed in GameWonState...";
            break;
        } // switch( ((QKeyEvent*)event)->key() )
        break;

    //! When it is any other event. Do nothing.
    default:
        TraceOut( TRACE_FILE_EXECUTION ) << "Event " << event->type() << " NOT processed in GameWonState...";
        break;
    } // switch( event->type() )

    //! Return status of event processing.
    return rVal;
} // GameWonState::event( QEvent * event )

//! Exit the State.
//! \return void
//! \sa State, Event, Machine
void GameWonState::exit()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameWonState::exit...";

    //! Unload the menu image.
    mWorld.menu().unload();
} // GameWonState::exit()

//! Render the State.
//! \return void
//! \sa State, Event, Machine
void GameWonState::render()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameWonState::render...";

    //! Setup the Camera for rendering the Menu.
    mWorld.camera().render();

    //! Render the Menu
    mWorld.menu().render( mWorld.camera() );
} // GameWonState::render()

//! Update the State.
//! \return void
//! \sa State, Event, Machine
void GameWonState::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameWonState::tick...";

    //! Update the Camera
    mWorld.camera().tick();

    //! Update the Menu
    mWorld.menu().tick();
} // GameWonState::tick()



//------------------------------------------------------------------------------
// Events


// GameDiedEvent---------------------------------------------------------------

//! The GameDiedEvent execution begins here.
//! Create a GameDiedEvent and pass it a reference to the World data.
//! Passes a name for the event as a QString to the base class.
//! \param world is a reference to the World data.
GameDiedEvent::GameDiedEvent( World & world ) :
    Event( "GameDiedEvent" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameDiedEvent( World & world )...";
} // GameDiedEvent::GameDiedEvent( World & world )

//! The GameDiedEvent execution ends here.
//! Destroy the GameDiedEvent.
//! Required to free the GameDiedEvent allocated memory.
GameDiedEvent::~GameDiedEvent()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GameDiedEvent()...";
} // GameDiedEvent::~GameDiedEvent()

//! Called periodically to check if Event occured.
//! Class derived from Event must implement to test for event.
//! \return State * is a pointer to the state to transition to.
//! \sa Event, State, Machine
State * GameDiedEvent::test()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameDiedEvent::process...";

    //! If Died equals true
    if( mWorld.getDied() == true )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Event occured...";
        //! The Event has been triggered so reset the condition.
        mWorld.setDied( false );
        //! Return the State to transition to.
        return mpState;
    }

    //! Else don't return a State
    return NULL;
} // GameDiedEvent::test()

// GameFinishedEvent---------------------------------------------------------------

//! The GameFinishedEvent execution begins here.
//! Create a GameFinishedEvent and pass it a reference to the World data.
//! Passes a name for the event as a QString to the base class.
//! \param world is a reference to the World data.
GameFinishedEvent::GameFinishedEvent( World & world ) :
    Event( "GameFinishedEvent" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameFinishedEvent( World & world )...";
} // GameFinishedEvent::GameFinishedEvent( World & world )

//! The GameFinishedEvent execution ends here.
//! Destroy the GameFinishedEvent.
//! Required to free the GameFinishedEvent allocated memory.
GameFinishedEvent::~GameFinishedEvent()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GameFinishedEvent()...";
} // GameFinishedEvent::~GameFinishedEvent()

//! Called periodically to check if Event occured.
//! Class derived from Event must implement to test for event.
//! \return State * is a pointer to the state to transition to.
//! \sa Event, State, Machine
State * GameFinishedEvent::test()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameFinishedEvent::process...";

    //! If Finished equals true
    if( mWorld.getFinished() == true )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Event occured...";
        //! The Event has been triggered so reset the condition.
        mWorld.setFinished( false );
        //! Return the State to transition to.
        return mpState;
    }

    //! Else don't return a State
    return NULL;
} // GameFinishedEvent::test()

// GameLastLevelEvent---------------------------------------------------------------

//! The GameLastLevelEvent execution begins here.
//! Create a GameLastLevelEvent and pass it a reference to the World data.
//! Passes a name for the event as a QString to the base class.
//! \param world is a reference to the World data.
GameLastLevelEvent::GameLastLevelEvent( World & world ) :
    Event( "GameLastLevelEvent" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameLastLevelEvent( World & world )...";
} // GameLastLevelEvent::GameLastLevelEvent( World & world )

//! The GameLastLevelEvent execution ends here.
//! Destroy the GameLastLevelEvent.
//! Required to free the GameLastLevelEvent allocated memory.
GameLastLevelEvent::~GameLastLevelEvent()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GameLastLevelEvent()...";
} // GameLastLevelEvent::~GameLastLevelEvent()

//! Called periodically to check if Event occured.
//! Class derived from Event must implement to test for event.
//! \return State * is a pointer to the state to transition to.
//! \sa Event, State, Machine
State * GameLastLevelEvent::test()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameLastLevelEvent::process...";

    //! If LastLevel equals true
    if( mWorld.getLastLevel() == true )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Event occured...";
        //! The Event has been triggered so reset the condition.
        mWorld.setLastLevel( false );
        //! Return the State to transition to.
        return mpState;
    }

    //! Else don't return a State
    return NULL;
} // GameLastLevelEvent::test()

// GameLastLifeEvent---------------------------------------------------------------

//! The GameLastLifeEvent execution begins here.
//! Create a GameLastLifeEvent and pass it a reference to the World data.
//! Passes a name for the event as a QString to the base class.
//! \param world is a reference to the World data.
GameLastLifeEvent::GameLastLifeEvent( World & world ) :
    Event( "GameLastLifeEvent" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameLastLifeEvent( World & world )...";
} // GameLastLifeEvent::GameLastLifeEvent( World & world )

//! The GameLastLifeEvent execution ends here.
//! Destroy the GameLastLifeEvent.
//! Required to free the GameLastLifeEvent allocated memory.
GameLastLifeEvent::~GameLastLifeEvent()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GameLastLifeEvent()...";
} // GameLastLifeEvent::~GameLastLifeEvent()

//! Called periodically to check if Event occured.
//! Class derived from Event must implement to test for event.
//! \return State * is a pointer to the state to transition to.
//! \sa Event, State, Machine
State * GameLastLifeEvent::test()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameLastLifeEvent::process...";

    //! If LastLife equals true
    if( mWorld.getLastLife() == true )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Event occured...";
        //! The Event has been triggered so reset the condition.
        mWorld.setLastLife( false );
        //! Return the State to transition to.
        return mpState;
    }

    //! Else don't return a State
    return NULL;
} // GameLastLifeEvent::test()

// GameReturnEvent---------------------------------------------------------------

//! The GameReturnEvent execution begins here.
//! Create a GameReturnEvent and pass it a reference to the World data.
//! Passes a name for the event as a QString to the base class.
//! \param world is a reference to the World data.
GameReturnEvent::GameReturnEvent( World & world ) :
    Event( "GameReturnEvent" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameReturnEvent( World & world )...";
} // GameReturnEvent::GameReturnEvent( World & world )

//! The GameReturnEvent execution ends here.
//! Destroy the GameReturnEvent.
//! Required to free the GameReturnEvent allocated memory.
GameReturnEvent::~GameReturnEvent()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GameReturnEvent()...";
} // GameReturnEvent::~GameReturnEvent()

//! Called periodically to check if Event occured.
//! Class derived from Event must implement to test for event.
//! \return State * is a pointer to the state to transition to.
//! \sa Event, State, Machine
State * GameReturnEvent::test()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameReturnEvent::process...";

    //! If Return equals true
    if( mWorld.getReturn() == true )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Event occured...";
        //! The Event has been triggered so reset the condition.
        mWorld.setReturn( false );
        //! Return the State to transition to.
        return mpState;
    }

    //! Else don't return a State
    return NULL;
} // GameReturnEvent::test()

// GameSpaceEvent---------------------------------------------------------------

//! The GameSpaceEvent execution begins here.
//! Create a GameSpaceEvent and pass it a reference to the World data.
//! Passes a name for the event as a QString to the base class.
//! \param world is a reference to the World data.
GameSpaceEvent::GameSpaceEvent( World & world ) :
    Event( "GameSpaceEvent" ),
    mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameSpaceEvent( World & world )...";
} // GameSpaceEvent::GameSpaceEvent( World & world )

//! The GameSpaceEvent execution ends here.
//! Destroy the GameSpaceEvent.
//! Required to free the GameSpaceEvent allocated memory.
GameSpaceEvent::~GameSpaceEvent()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "~GameSpaceEvent()...";
} // GameSpaceEvent::~GameSpaceEvent()

//! Called periodically to check if Event occured.
//! Class derived from Event must implement to test for event.
//! \return State * is a pointer to the state to transition to.
//! \sa Event, State, Machine
State * GameSpaceEvent::test()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "GameSpaceEvent::process...";

    //! If Space equals true
    if( mWorld.getSpace() == true )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Event occured...";
        //! The Event has been triggered so reset the condition.
        mWorld.setSpace( false );
        //! Return the State to transition to.
        return mpState;
    }

    //! Else don't return a State
    return NULL;
} // GameSpaceEvent::test()



//------------------------------------------------------------------------------
// Machine


//! The Game execution begins here.
//! Create a Game and pass it a reference to the World data.
//! Passes a name for the Machine as a QString to the base class.
//! \param world is a reference to the World data.
Game::Game( World & world ) :
    Machine( "Game" ), mWorld( world )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Game::Game( World & world )...";

    //! Configure the Game State Machine
    configure();
} // Game::Game( World & world )

//! The Machine execution ends here.
//! Destroy the Game.
//! Required to free the Game allocated memory.
Game::~Game()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Game::~Game()...";
} // Game::~Game()

//! Called to configure the Game Machine.
//! \return void
//! \sa Event, State, Machine
void Game::configure()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Game::configure()...";

    //! Create States.

    //! Create GameDiedState.
    GameDiedState * pGameDiedState = new GameDiedState( mWorld );
    //! Create GameFinishedState.
    GameFinishedState * pGameFinishedState = new GameFinishedState( mWorld );
    //! Create GameMenuState.
    GameMenuState * pGameMenuState = new GameMenuState( mWorld );
    //! Create GameOverState.
    GameOverState * pGameOverState = new GameOverState( mWorld );
    //! Create GamePlayState.
    GamePlayState * pGamePlayState = new GamePlayState( mWorld );
    //! Create GameWonState.
    GameWonState * pGameWonState = new GameWonState( mWorld );

    //! Create Events.

    //! Create GameDiedEvent.
    GameDiedEvent * pGameDiedEvent = new GameDiedEvent( mWorld );
    //! Create GameReturnEvent.
    GameReturnEvent * pGameReturnEvent = new GameReturnEvent( mWorld );
    //! Create GameFinishedEvent.
    GameFinishedEvent * pGameFinishedEvent = new GameFinishedEvent( mWorld );
    //! Create GameLastLevelEvent.
    GameLastLevelEvent * pGameLastLevelEvent = new GameLastLevelEvent( mWorld );
    //! Create GameLastLifeEvent.
    GameLastLifeEvent * pGameLastLifeEvent = new GameLastLifeEvent( mWorld );
    //! Create GameSpaceEvent.
    GameSpaceEvent * pGameSpaceEvent = new GameSpaceEvent( mWorld );

    //! Set Event Transitiions.

    //! Set GameDiedEvent transitiions to GameDiedState.
    pGameDiedEvent->setTransitionState( pGameDiedState );
    //! Set GameReturnEvent transitiions to GameMenuState.
    pGameReturnEvent->setTransitionState( pGameMenuState );
    //! Set GameFinishedEvent transitiions to GameFinishedState.
    pGameFinishedEvent->setTransitionState( pGameFinishedState );
    //! Set GameLastLevelEvent transitiions to GameWonState.
    pGameLastLevelEvent->setTransitionState( pGameWonState );
    //! Set GameLastLifeEvent transitiions to GameOverState.
    pGameLastLifeEvent->setTransitionState( pGameOverState );
    //! Set GameSpaceEvent transitiions to GamePlayState.
    pGameSpaceEvent->setTransitionState( pGamePlayState );

    //! Add Events to the States.

    //! Add GameLastLifeEvent to GameDiedState.
    pGameDiedState->addEvent( pGameLastLifeEvent );
    //! Add GameSpaceEvent to GameDiedState.
    pGameDiedState->addEvent( pGameSpaceEvent );
    //! Add GameLastLevelEvent to GameFinishedState.
    pGameFinishedState->addEvent( pGameLastLevelEvent );
    //! Add GameSpaceEvent to GameFinishedState.
    pGameFinishedState->addEvent( pGameSpaceEvent );
    //! Add GameSpaceEvent to GameMenuState.
    pGameMenuState->addEvent( pGameSpaceEvent );
    //! Add GameDiedEvent to GamePlayState.
    pGamePlayState->addEvent( pGameDiedEvent );
    //! Add GameFinishedEvent to GamePlayState.
    pGamePlayState->addEvent( pGameFinishedEvent );
    //! Add GameReturnEvent to GameOverState.
    pGameOverState->addEvent( pGameReturnEvent );
    //! Add GameReturnEvent to GameWonState.
    pGameWonState->addEvent( pGameReturnEvent );


    //! Add Events to Machine so it can release resources when destructed.

    //! Add GameDiedEvent.
    addEvent( pGameDiedEvent );
    //! Add GameReturnEvent.
    addEvent( pGameReturnEvent );
    //! Add GameFinishedEvent.
    addEvent( pGameFinishedEvent );
    //! Add GameLastLevelEvent.
    addEvent( pGameLastLevelEvent );
    //! Add GameLastLifeEvent.
    addEvent( pGameLastLifeEvent );
    //! Add GameSpaceEvent.
    addEvent( pGameSpaceEvent );

    //! Add States to Machine so it can release resources when destructed.

    //! Add GameDiedState.
    addState( pGameDiedState );
    //! Add GameFinishedState.
    addState( pGameFinishedState );
    //! Add GameMenuState.
    addState( pGameMenuState );
    //! Add GameOverState.
    addState( pGameOverState );
    //! Add GamePlayState.
    addState( pGamePlayState );
    //! Add GameWonState.
    addState( pGameWonState );

    //! Set the start State.
    setStartState( pGameMenuState );
} // Game::configure()
