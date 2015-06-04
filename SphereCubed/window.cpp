/*! \file window.cpp
\brief Window class implementation file.
This file contains the Window class implementation for the application.
*/

// window.h header file required for class definition.
#include "window.h"

// QApplication header file required for sending event.
#include <QApplication>
// QIcon header file required for window icon.
#include <QIcon>

// settings.h header file required for application settings.
#include "settings.h"
// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION false

//! The Graphical User Interface (GUI) Window execution begins here.
//! Calling this constructor is required to setup the Window.
//! Passes parent argument onto base class constructor.
//! \param parent is a pointer the QWidget parent object.
Window::Window(QWidget *parent)
    : QGLWidget(QGLFormat(/* Additional format options */), parent),
      mEngine( ENGINE_TICK_INTERVAL )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Window::Window()...";

    //! Set the window icon.
    setWindowIcon( QIcon( ":/SphereCubedIcon" ) );

    //! Connect the Engine updateUI signal to the Window updateGL slot.
    connect( &mEngine, SIGNAL(updateUI()), this, SLOT(updateGL()) );
} // Window::Window()

//! The GUI Window execution ends here.
//! Automatically called when QApplication is closed.
//! Required to free any allocated memory.
Window::~Window()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Window::~Window()...";
} //Window::~Window()

//! Process received events.
//! \param event is a pointer to a QEvent argument identifing the event.
//! \return bool
//! \sa Window, QEvent, QGLWidget, QObject
bool Window::event( QEvent * event )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Window::event( QEvent * event): " << event->type() << "...";

    //! Return value, assume NOT processed until otherwise determined.
    bool rVal = false;

    //! Pass the event to the Engine.
    rVal = QApplication::sendEvent( &mEngine, event );

    //! If the Engine did not process the event.
    if( rVal == false )
    {
        //! Determine what type of event was received.
        switch( event->type() )
        {
        //! When it is any other event.
        default:
            //! Pass the unprocessed events to the base class.
            rVal = QGLWidget::event( event );
            break;
        } // switch( event->type() )
    }

    //! If the event was processed.
    if( rVal == true )
    {
        //! Accept the event.
        event->accept();
    } // if( rVal == true )
    //! If the event was NOT processed.
    else
    {
        //! Ignore the event.
        event->ignore();
    } // // if( rVal == true ) else

    //! Return status of event processing.
    return rVal;
} // event( QEvent * event )

//! Initialize OpenGL resources.
//! \return void
//! \sa Window, QGLWidget
void Window::initializeGL()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Window::initializeGL()...";

    //! Initialize the Engine.
    mEngine.initialize();
} // Window::initializeGL()

//! Render OpenGL scene.
//! \return void
//! \sa Window, QGLWidget
void Window::paintGL()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Window::paintGL()...";

    //! Render the Engine.
    mEngine.render();
} // Window::paintGL()

//! Resize OpenGL resources.
//! \return void
//! \sa Window, QGLWidget
void Window::resizeGL( int width, int height )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Window::resizeGL( int width, int height ): " << width << ", " << height << "...";

    //! Resize the Engine.
    mEngine.resize( width, height );
} // Window::resizeGL( int width, int height )

//! Specify the Window default size.
//! \return QSize is the recommended size of the widget.
//! \sa Window, QWidget
QSize Window::sizeHint() const
{
    //! Return the recommended size for the widget.
    return QSize(WINDOW_WIDTH, WINDOW_HEIGHT);
} // Window::sizeHint() const
