/*! \file machine.cpp
\brief Machine class implementation file.
This file contains the abstract Machine class implementation for the application.
*/

// machine.h header file required for Machine class definition.
#include "machine.h"

// QApplication header file required for sending events.
#include <QApplication>

// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION false


//------------------------------------------------------------------------------
// Event Class Implementation

//! The Event execution begins here.
//! Creates an Event with a given name.
//! \param name is a QString name to give the Event.
Event::Event( QString name ) :
    mpState( NULL ), mName( name )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Event::Event( QString name ) - " << mName << "...";
} // Event::Event( QString name )

//! The Event execution ends here.
//! Destroy the Event.
//! Automatically called when the Machine releases its Event.
Event::~Event()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Event::~Event() - " << mName << "...";
} // Event::~Event()

//! Must be called in derived class to setup State transistion.
//! \return void
//! \param pState is a pointer to a State identifing the State to transition to.
//! \sa Event, State, Machine
void Event::setTransitionState( State * pState )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Event::setTransitionState - " << mName << " to " << pState->name() << "...";

    //! Store the State to transition to.
    mpState = pState;
} // Event::setTransissionState( State * pState )



//------------------------------------------------------------------------------
// State Class Implementation

//! The State execution begins here.
//! Create an State with a given name.
//! Passes parent argument onto base class constructor.
//! \param name is a QString name to give the State.
//! \param parent is a pointer the QObject parent object.
State::State( QString name, QObject * parent ) :
    QObject( parent ), mName( name )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "State::State( QString name, QObject * parent ) - " << mName << "...";
} // State::State( QString name, QObject * parent )

//! The Machine execution ends here.
//! Destroy the State.
//! Required to free any allocated memory.
State::~State()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "State::~State() - " << mName << "...";
} // State::~State()

//! Called to add an Event to the State Event list.
//! \return void
//! \param pEvent is a pointer to an Event identifing the Event to add to the State.
//! \sa Event, State, Machine
void State::addEvent( Event * pEvent )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "State::addEvent( Event * pEvent ) - " << mName << " add " << pEvent->name() << "...";

    //! Add the Event to the State list.
    mEventList.push_back( pEvent );
} // State::addEvent( Event * pEvent )

//! Automatically called by the Machine.
//! \return State * is apointer to the State to transition to.
//! \sa Event, State, Machine
State * State::process()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "State::process() - " << mName << "...";

    //! Go through all State Event list
    foreach (Event * pEvent, mEventList)
    {
        //! Test the Event.
        State * pNewState = pEvent->test();

        //! If an Event was triggered.
        if( pNewState != NULL )
        {
            //! return the State to transition to.
            return pNewState;
        } // if( pNewState != NULL )
    } // foreach (Event * pEvent, mEventList)

    //! If no event occured don't return a State.
    return NULL;
} // process()


//------------------------------------------------------------------------------
// Machine Class Implementation

//! The Machine execution begins here.
//! Create a Machine with a given name.
//! Passes parent argument onto base class constructor.
//! \param name is a QString name to give the Machine.
//! \param parent is a pointer the QObject parent object.
Machine::Machine( QString name, QObject * parent ) :
    QObject( parent ), mName( name ), mpState( NULL )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Machine::Machine( QString name, QObject * parent ) - " << mName << "...";
} // Machine::Machine( QString name, QObject * parent )

//! The Machine execution ends here.
//! Destroy the Machine.
//! Required to free the any allocated memory.
Machine::~Machine()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Machine::~Machine() - " << mName << "...";

    //! Go through the Machine Event list
    foreach( Event * pEvent, mEventList )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Event - " << pEvent->name() << "...";
        //! Release the resources allocated for the Event
        delete pEvent;
    } // foreach( Event * pEvent, mEventList )

    //! Go through the Machine State list
    foreach( State * pState, mStateList )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "State - " << pState->name() << "...";
        //! Release the resources allocated for the State
        delete pState;
    } // foreach( State * pState, mStateList )
} // Machine::~Machine()

//! Adding the Events to the Machine enables the Machine to release the allocated memory at destruction.
//! \return void
//! \param pEvent is a pointer to an Event identifing the Event to add to the Machine.
//! \sa Event, State, Machine
void Machine::addEvent( Event * pEvent )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Machine::addEvent - " << mName << " add " << pEvent->name() << "...";

    //! Add the Event to the Machine list of Event.
    mEventList.push_back( pEvent );
} // addEvent( Event * pEvent )

//! Adding the States to the Machine enables the Machine to release the allocated memory at destruction.
//! \return void
//! \param pState is a pointer to an State identifing the State to add to the Machine.
//! \sa Event, State, Machine
void Machine::addState( State * pState )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Machine::addState( State * pState ) - " << mName << " add " << pState->name() << "...";

    //! Add the State to the Machine list of State.
    mStateList.push_back( pState );
} // Machine::addState( State * pState )

//! \return void
//! \param pState is a pointer to an State identifing the State to start execution in.
//! \sa Event, State, Machine
void Machine::setStartState( State * pState )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Machine::setStartState - " << mName << " set " << pState->name() << "...";

    //! Store the start State.
    mpState = pState;
} // setStartState( State * pState )

//! Render the current State.
//! \return void
//! \sa State, Machine
void Machine::render()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Machine::render() - " << mName << " render " << mpState->name() << "...";

    //! Render the current State.
    mpState->render();
} // Machine::render()

//! Enter the Machine start State.
//! \return void
//! \sa State, Machine
void Machine::start()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Machine::start() - " << mName << " add " << mpState->name() << "...";

    //! Enter the State.
    mpState->enter();
} // Machine::start()

//! Process the current State.
//! \return void
//! \sa Event, State, Machine
void Machine::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Machine::tick() - " << mName << "...";

    // A State must be assigned before it can be updated.
    Q_ASSERT( mpState != NULL );

    //! Process the State Event list.
    State* pState = mpState->process();

    //! If an Event occured that caused the active State to change.
    if( pState != NULL )
    {
        //! Exit the current State.
        mpState->exit();

        //! Change to the new active State.
        mpState = pState;

        //! Enter the new State.
        mpState->enter();
    } // if( pState != NULL )

    //! Update the current State.
    mpState->tick();
} // Machine::tick()
