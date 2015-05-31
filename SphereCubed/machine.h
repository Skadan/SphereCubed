/*! \file machine.h
\brief Machine class declaration file.
This file contains the Machine class declaration for the application.
This is a set of abstract base class that can be derived from to implment State Machine functionality.
*/

#ifndef MACHINE_H
#define MACHINE_H

// QApplication header file required for sending events to a State.
#include <QApplication>

// QObject header file required for event messsages.
#include <QObject>

// QString header file required for strings.
#include <QString>


// Pre-declared calss
class State;


//------------------------------------------------------------------------------
// Event Class Definition


//! Event class.
/*!
This class declaration encapsulates the Machine Event functionality.
This is an abstract base class that can be derived from to implment specific Event conditions.
*/
class Event
{
    //! Friend of Machine so that only Machine can setup and delete an Event.
    friend class Machine;

    //! Friend of State so that only State can call test on an Event.
    friend class State;

protected: // Constructors / Destructors
    //! Event class constructor.
    Event( QString name );

    //! Event class destructor.
    virtual ~Event();

protected: // Member Fuctions
    //! Access function to get the name of the Event.
    //! \return const QString reference containing the Event name.
    //! \sa Event
    const QString & name() const { return mName; }

    //! A pure virtual protected member returning a State to transistion to.
    //! Derived class must implement to test for Event.
    //! \return State * is apointer to the State to transition to.
    //! \sa Event, State, Machine
    virtual State * test() = 0;

    //! Configuration function to set the State to transistion to when Event condition is triggered.
    void setTransitionState( State * pState );

protected: // Member Variables
    //! Used to hold the State to transition to on Event condition triggered.
    State  *mpState;

private: // Member Variables
    //! Used to hold the name of the Event.
    QString mName;
}; // class Event



//------------------------------------------------------------------------------
// State Class Definition


//! State class.
/*!
This class declaration encapsulates the Machine State functionality.
This is an abstract base class that can be derived from to implment a specific State behavior.
*/
class State : public QObject
{
    // Q_OBJECT macro required to use services provided by Qt's meta-object system.
    Q_OBJECT

    //! Friend of Event so that Event can call name on State.
    friend class Event;

    //! Friend of Machine so that only Machine can call process on a state and delete a State.
    friend class Machine;

protected: // Constructors / Destructors
    //! State class constructor.
    State( QString name, QObject *parent = 0 );

    //! State class destructor.
    virtual ~State();

protected: // Member Functions
    //! Configuration function to add an Event to the State.
    void addEvent( Event * pEvent );

    //! Automatically triggered when a State is entered.
    //! Override in derived class to execute specialized State enter functionality.
    //! \return void
    //! \sa Machine, State, Event
    virtual void enter() {}

    //! Automatically triggered when an event message is received.
    //! Override in derived class to receive and process event messages.
    //! \param event is a pointer to a QEvent argument identifing the event.
    //! \return bool should be set true when event is processed.
    //! \sa QObject, Machine, State
    virtual bool event( QEvent * event ) { Q_UNUSED(event); return false; }

    //! Automatically triggered when a State is exited.
    //! Override in derived class to execute specialized State exit functionality.
    //! \return void
    //! \sa Machine, State, Event
    virtual void exit() {}

    //! Access function to get the name of the State.
    //! \return const QString reference containing the State name.
    //! \sa State
    const QString & name() const { return mName; }

    //! Automatically triggered when the scene needs to be rendred.
    //! Override in derived class to execute specialized State render functionality.
    //! \return void
    //! \sa Machine, State
    virtual void render() {}

    //! Automatically triggered when a State is updated.
    //! Override in derived class to execute specialized State update functionality.
    //! \return void
    //! \sa Machine, State, Event
    virtual void tick() {}

private: // Member Functions
    //! Called periodically to check if any Event occured.
    State * process();

private: // Member Variables
    //! Used to hold the name of the State.
    QString mName;

    //! Used to hold the list of Event known to the State.
    QList<Event*> mEventList;
}; // class State



//------------------------------------------------------------------------------
// Machine Class Definition


//! Machine class.
/*!
This class declaration encapsulates the Machine functionality.
This is an abstract base class that can be derived from to implment a specific Machine behavior.
*/
class Machine : public QObject
{
    // Q_OBJECT macro required to use services provided by Qt's meta-object system.
    Q_OBJECT

public: // Member Functions
    //! Call to render the Machine.
    void render();

    //! Call once to start the Machine before calling tick.
    void start();

    //! Call periodically to update the Machine.
    void tick();

protected: // Constructors / Destructors
    //! Machine class constructor.
    explicit Machine( QString name, QObject *parent = 0 );

    //! Machine class destructor.
    virtual ~Machine();

protected: // Member Functions
    //! Configuration function called to add a Event to the Machine list of Event.
    void addEvent( Event * pEvent );

    //! Configuration function called to add a State to the Machine list of State.
    void addState( State * pState );

    //! Triggered when an event message is received.
    //! \param event is a pointer to a QEvent argument identifing the event.
    //! \return bool should be set true when event is processed.
    //! \sa QObject, Window, Machine, State
    virtual bool event( QEvent * event ) { return QApplication::sendEvent(mpState, event); }

    //! Access function to get the name of the Machine.
    //! \return const QString reference containing the Machine name.
    //! \sa Machine
    const QString & name() const { return mName; }

    //! Configuration function called to set the Machines start State.
    void setStartState( State * pState );

private: // Member Variables
    //! Used to hold the list of Event known to the Machine.
    QList<Event*> mEventList;

    //! Used to hold the name of the Machine.
    QString mName;

    //! Used to hold the current State.
    State * mpState;

    //! Used to hold the list of State known to the Machine.
    QList<State*> mStateList;
}; // class Machine

#endif // MACHINE_H
