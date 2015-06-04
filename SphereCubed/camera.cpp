/*! \file camera.cpp
\brief Camera class implementation file.
This file contains the Camera class implementation for the application.
*/

// camera.h header file required for class definition.
#include "camera.h"

// gl.h header file required for gl commands.
#include <GL/gl.h>

// settings.h header file required for application settings.
#include "settings.h"
// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION true


//------------------------------------------------------------------------------
// States

// CameraMenuState -------------------------------------------------------------

//! The CameraMenuState execution begins here.
//! Create a CameraMenuState and pass it a reference to the Camera functionality.
//! Passes a name for the state as a QString to the base class.
//! \param camera is a reference to the Camera functionality.
CameraMenuState::CameraMenuState( Camera & camera ) :
    State( "CameraMenuState" ), mCamera( camera )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraMenuState::CameraMenuState( Camera & camera )...";
} // CameraMenuState::CameraMenuState( Camera & camera )

//! The CameraMenuState execution ends here.
//! Destroy the CameraMenuState.
//! Required to free the CameraMenuState allocated memory.
CameraMenuState::~CameraMenuState()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraMenuState::~CameraMenuState()...";
} // CameraMenuState::~CameraMenuState()

//! Enter the State.
//! \return void
//! \sa State, Event, Machine
void CameraMenuState::enter()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraMenuState::enter()...";

    //! Set the color to clear the scene with to black
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    //! Setup the camera to view the Menu.

    //! Position the Camera 2 units out in the Z direction.
    mCamera.eye()       = QVector3D(0.0f,0.0f,2.0f);
    //! Focus the Camera on the origin.
    mCamera.focus()     = QVector3D(0,0,0);
    //! Orient the Camera so that positive Y is up.
    mCamera.direction() = QVector3D(0,1,0);
} // CameraMenuState::enter()

//! Exit the State.
//! \return void
//! \sa State, Event, Machine
void CameraMenuState::exit()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraMenuState::exit()...";
} // CameraMenuState::exit()

//! Render the State.
//! \return void
//! \sa State, Event, Machine
void CameraMenuState::render()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraMenuState::render()...";

    //! Clear the background and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //! Position, focus, and orient the camera to look at the menu.
    mCamera.viewMatrix().setToIdentity();
    mCamera.viewMatrix().lookAt( mCamera.eye(), mCamera.focus(), mCamera.direction() );
} // CameraMenuState::render()

//! Update the State.
//! \return void
//! \sa State, Event, Machine
void CameraMenuState::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraMenuState::tick()...";
} // CameraMenuState::tick()

// CameraPlayState ---------------------------------------------------------------

//! The CameraPlayState execution begins here.
//! Create a CameraPlayState and pass it a reference to the Camera functionality.
//! Passes a name for the state as a QString to the base class
//! \param camera is a reference to the Camera functionality.
CameraPlayState::CameraPlayState( Camera & camera ) :
    State( "CameraPlayState" ), mCamera( camera )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraPlayState::CameraPlayState( Camera & camera )...";
} // CameraPlayState::CameraPlayState( Camera & camera )

//! The CameraPlayState execution ends here.
//! Destroy the CameraPlayState.
//! Required to free the CameraPlayState allocated memory.
CameraPlayState::~CameraPlayState()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraPlayState::~CameraPlayState()...";
} // CameraPlayState::~CameraPlayState()

//! Enter the State.
//! \return void
//! \sa State, Event, Machine
void CameraPlayState::enter()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraPlayState::enter()...";

    //! Turn on depth testing anf back face culling.
    glEnable(GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );

    //! Set the color to clear the scene with to black
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    //! Setup the camera to view game play.

    //! Position the Camera.
    mCamera.eye()       = QVector3D(5.0f,25.0f,5.0f);
    //! Focus the Camera.
    mCamera.focus()     = QVector3D(6,0,6);
    //! Orient the Camera so that positive Y is up.
    mCamera.direction() = QVector3D(0,1,0);
} // CameraPlayState::enter()

//! Exit the State.
//! \return void
//! \sa State, Event, Machine
void CameraPlayState::exit()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraPlayState::exit()...";
} // CameraPlayState::exit()

//! Render the State.
//! \return void
//! \sa State, Event, Machine
void CameraPlayState::render()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraPlayState::render()...";

    //! Clear the background and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //! Position, focus, and orient the camera
    mCamera.viewMatrix().setToIdentity();
    mCamera.viewMatrix().lookAt( mCamera.eye(), mCamera.focus(), mCamera.direction() );

    //! Setup the bounding Frustum planes for the new settings.
    mCamera.view(  mCamera.eye(), mCamera.focus(), mCamera.direction() );
} // CameraPlayState::render()

//! Update the State.
//! \return void
//! \sa State, Event, Machine
void CameraPlayState::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraPlayState::tick()...";
} // CameraPlayState::tick()



//------------------------------------------------------------------------------
// Events

// CameraMenuEvent -------------------------------------------------------------

//! The CameraMenuEvent execution begins here.
//! Create a CameraMenuEvent and pass it a reference to the Camera functionality.
//! Passes a name for the event as a QString to the base class.
//! \param camera is a reference to the Camera functionality.
CameraMenuEvent::CameraMenuEvent( Camera & camera ) :
    Event( "CameraMenuEvent" ), mCamera( camera )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraMenuEvent::CameraMenuEvent( Camera & camera )...";
} // CameraMenuEvent::CameraMenuEvent( Camera & camera )

//! The CameraMenuEvent execution ends here.
//! Destroy the CameraMenuEvent.
//! Required to free the CameraMenuEvent allocated memory.
CameraMenuEvent::~CameraMenuEvent()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraMenuEvent::~CameraMenuEvent()...";
} // CameraMenuEvent::~CameraMenuEvent()

//! Called periodically to check if Event occured.
//! Class derived from Event must implement to test for event.
//! \return State * is a pointer to the state to transition to.
//! \sa Event, State, Machine
State * CameraMenuEvent::test()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraMenuEvent::process...";

    //! If menu equals true
    if( mCamera.getMenu() == true )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Event occured...";
        //! The Event has been triggered so reset the condition.
        mCamera.setMenu( false );
        //! Return the State to transition to.
        return mpState;
    }

    //! Else don't return a State
    return NULL;
}  // CameraMenuEvent::process()

// CameraPlayEvent -------------------------------------------------------------

//! The CameraPlayEvent execution begins here.
//! Create a CameraPlayEvent and pass it a reference to the Camera functionality.
//! Passes a name for the event as a QString to the base class.
//! \param camera is a reference to the Camera functionality.
CameraPlayEvent::CameraPlayEvent( Camera & camera ) :
    Event( "CameraPlayEvent" ), mCamera( camera )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraPlayEvent::CameraPlayEvent( Camera & camera )...";
} // CameraPlayEvent::CameraPlayEvent( Camera & camera )

//! The CameraPlayEvent execution ends here.
//! Destroy the CameraPlayEvent.
//! Required to free the CameraPlayEvent allocated memory.
CameraPlayEvent::~CameraPlayEvent()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraPlayEvent::~CameraPlayEvent()...";
} // CameraPlayEvent::~CameraPlayEvent()

//! Called periodically to check if Event occured.
//! Class derived from Event must implement to test for event.
//! \return State * is a pointer to the state to transition to.
//! \sa Event, State, Machine
State * CameraPlayEvent::test()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "CameraMenuEvent::process...";

    //! If play equals true
    if( mCamera.getPlay() == true )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Event occured...";
        //! The Event has been triggered so reset the condition.
        mCamera.setPlay( false );
        //! Return the State to transition to.
        return mpState;
    }

    //! Else don't return a State
    return NULL;
} // CameraPlayEvent::process()



//------------------------------------------------------------------------------
// Machine


//! The Camera execution begins here.
//! Create a Camera.
//! Passes a name for the Machine as a QString to the base class.
Camera::Camera() :
    Machine( "Camera" )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Camera::Camera()...";

    //! Initialize the member variables
    mFarDistance = CAMERA_FAR_CLIPPING_PLANE_DISTANCE;
    mFeildOfView = CAMERA_FEILD_OF_VIEW_ANGLE_DEGREES;
    mMenu = false;
    mNearDistance = CAMERA_NEAR_CLIPPING_PLANE_DISTANCE ;
    mPlay = false;

    //! Configure the Camera State Machine
    configure();
} // Camera::Camera()

//! The Machine execution ends here.
//! Destroy the Camera.
//! Required to free the Camera allocated memory.
Camera::~Camera()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Camera::~Camera()...";
} // Camera::~Camera()

//! Called to configure the Camera Machine.
//! \return void
//! \sa Event, State, Machine
void Camera::configure()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Camera::configure()...";

    //! Create States.

    //! Create CameraMenuState.
    CameraMenuState * pCameraMenuState = new CameraMenuState( *this );
    //! Create CameraPlayState.
    CameraPlayState * pCameraPlayState = new CameraPlayState( *this );


    //! Create Events.

    //! Create CameraMenuEvent.
    CameraMenuEvent * pCameraMenuEvent = new CameraMenuEvent( *this );
    //! Create CameraPlayEvent.
    CameraPlayEvent * pCameraPlayEvent = new CameraPlayEvent( *this );


    //! Set Event Transitiions.

    //! Set CameraMenuEvent transitiions to CameraMenuState.
    pCameraMenuEvent->setTransitionState( pCameraMenuState );
    //! Set CameraPlayEvent transitiions to CameraPlayState.
    pCameraPlayEvent->setTransitionState( pCameraPlayState );


    //! Add Events to the States.

    //! Add CameraMenuEvent to CameraMenuState.
    pCameraMenuState->addEvent( pCameraMenuEvent );
    //! Add CameraPlayEvent to CameraMenuState.
    pCameraMenuState->addEvent( pCameraPlayEvent );
    //! Add CameraMenuEvent to CameraPlayState.
    pCameraPlayState->addEvent( pCameraMenuEvent );


    //! Add Events to Machine so it can release resources when destructed.

    //! Add CameraMenuEvent.
    addEvent( pCameraMenuEvent );
    //! Add CameraPlayEvent.
    addEvent( pCameraPlayEvent );

    //! Add States to Machine so it can release resources when destructed.

    //! Add CameraMenuState.
    addState( pCameraMenuState );
    //! Add CameraPlayState.
    addState( pCameraPlayState );


    //! Set the start State.
    setStartState( pCameraMenuState );
} // Camera::configure()

//! Resized the OpenGL projection matrix.
//! \param width in pixels of the OpenGL context.
//! \param height in pixels of the OpenGL context.
//! \return void
//! \sa Camera, Engine, Window, QGLWidget
void Camera::resize( const int width, const int height )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Camera::resize...";

    //! Calculate the ratio and prevent a divide by zero.
    float ratio = (float)width / ( height==0 ? 1.0f : (float)height);

    //! Set the viewport to be the entire window.
    glViewport(0, 0, width, height);

    //! Set the perspective.
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective( mFeildOfView, ratio, mNearDistance, mFarDistance );

    //! Update the bounding frustum with the projectio settings.
    mFrustum.projection( mFeildOfView, ratio, mNearDistance, mFarDistance );
} // Camera::resize( const int width, const int height )
