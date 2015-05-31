/*! \file engine.cpp
\brief Engine class implementation file.
This file contains the Engine class implementation for the application.
*/

// engine.h header file required for class definition.
#include "engine.h"

// QEvent header file required for processing events.
#include <QEvent>
// <gl.h> header file required for OpenGL commands.
#include <GL/gl.h>

// settings.h header file required for application settings.
#include "settings.h"
// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION true

//! The Engine execution begins here.
//! Automatically called by the Window during construction.
//! Passes parent argument onto base class constructor.
//! \param parent is a pointer the QWidget parent object.
Engine::Engine( QObject * parent ) :
    QObject( parent )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Engine::Engine( QObject * parent )...";
} // Engine::Engine( QObject * parent )

//! The Engine execution ends here.
//! Required to free the Engine allocated memory.
//! Automatically called when the Window is destructed.
Engine::~Engine()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Engine::~Engine()...";
} // Engine::~Engine()

//! Process received events.
//! \param event is a pointer to a QEvent argument identifing the event.
//! \return bool should be set true when processed.
//! \sa QObject, Window
bool Engine::event( QEvent * event )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Engine::event( QEvent * event ): " << event->type() << "...";

    //! Return value, assume processed until otherwise determined.
    bool rVal = true;

    //! Determine what type of event was received.
    switch( event->type() )
    {
    //! When it is a Timer event type.
    case QEvent::Timer:
        //! Tick the Engine to update and render the scene.
        tick();
        break;

    //! When it is any other event.
    default:
        rVal = false;
        break;
    } // switch( event->type() )

    //! Return status of event processing.
    return rVal;
} // Engine::event( QEvent * event )

//! Initialize OpenGL resources.
//! \return void
//! \sa Engine, Window, QGLWidget
void Engine::initialize()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Engine::initialize()...";

    //! Set the color to clear the scene with to black.
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    //! Start the timer to periodically update the Game Machine.
    startTimer( ENGINE_TICK_INTERVAL );
} // Engine::initialize()

//! Render the scene.
//! \return void
//! \sa Engine, Window, QGLWidget
void Engine::render()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Engine::render()...";

    //! Clear the background and depth buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //! Flush all the OpenGL command through the pipeline.
    glFlush();
} // Engine::render()

//! Resized the OpenGL context.
//! \param width in pixels of the OpenGL context.
//! \param height in pixels of the OpenGL context.
//! \return void
//! \sa Camera, Engine, Window, QGLWidget
void Engine::resize( const int width, const int height )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Engine::resize( const int width, const int height ) - (" << width << "," << height << ")...";

} // Engine::resize( const int width, const int height )

//! Update the scene.
//! \return void
//! \sa Engine
void Engine::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Engine::tick()...";

    //! Signal that the engine has updated the scene and is ready to render the scene.
    emit updateUI();
} // Engine::tick()
