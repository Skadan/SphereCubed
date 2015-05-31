/*! \file engine.h
\brief Engine class declaration file.
This file contains the Engine class declaration for the application.
*/

#ifndef ENGINE_H
#define ENGINE_H

// QObject header file required for signals, slots, and events.
#include <QObject>

// game.h header file required for game Machine.
#include "game.h"
// world.h header file required for world functionality.
#include "world.h"

//! Engine class.
/*!
This class declaration encapsulates the Engine functionality.
The Window instantiates a Engine to run and render the game.
*/
class Engine : public QObject
{
    // Q_OBJECT macro required to use services provided by Qt's meta-object system.
    Q_OBJECT

    //! Friend of Window so that only Window can instantiate the Engine.
    friend class Window;

private: // Constructors / Destructors
    //! Engine class constructor.
    explicit Engine(QObject *parent = 0);

    //! Engine class destructor.
    ~Engine();

private: // Member Function
    //! Triggered when an event is received.
    virtual bool event(QEvent *event);

    //! Triggered once before the first call to render or resize.
    void initialize();

    //! Triggered whenever the scene needs to be rendered.
    void render();

    //! Triggered whenever the OpenGL context needs to be resized.
    void resize(const int width, const int height);

    //! Triggered whenever the Timer expires.
    void tick();

private: // Member Variables
    //! Used for game play state machine logic to control World.
    Game mGame;

    //! Used to encapsulate game data and functionality.
    World mWorld;

signals:
    //! Emitted to begin rendering scene.
    //! \return void
    //! \sa Engine
    void updateUI();
}; // class Engine

#endif // ENGINE_H
