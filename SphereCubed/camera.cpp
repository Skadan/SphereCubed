/*! \file camera.cpp
\brief Camera class implementation file.
This file contains the Camera class implementation for the application.
*/

// camera.h header file required for class definition.
#include "camera.h"

// gl.h header file required for gl commands.
#include <GL/gl.h>
// QtMath header is required for trig functions.
#include <QtMath>

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

    //! Turn off depth testing anf back face culling.
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_CULL_FACE );

    //! Setup the camera to view the Menu.

    //! Set the projection matrix mode to orthoginal and recalculate the matrix.
    mCamera.setProjectionMode( Camera::ProjectMode::ORTHOGONAL );
    mCamera.projection();

    //! Position the Camera 1 units out in the Z direction.
    mCamera.eye()       = QVector3D(0.0f,0.0f,1.0f);
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
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );

    //! Set the color to clear the scene with to dark grey.
    glClearColor(0.2f,0.2f,0.2f,1.0f);

    //! Setup the camera to view game play.

    //! Set the projection matrix mode to perspective and recalculate the matrix.
    mCamera.setProjectionMode( Camera::ProjectMode::PERSPECTIVE );
    mCamera.projection();

    //! Position the Camera.
    mCamera.eye()       = QVector3D(0.0f,10.0f,0.0f);
    //! Focus the Camera.
    mCamera.focus()     = QVector3D(0,0,0);
    //! Orient the Camera so that positive Y is up.
    mCamera.direction() = QVector3D(0,1,0);
    //! Calculate the distance between the camera and its point of focus.
    mCamera.targetDistance() = mCamera.eye() - mCamera.focus();
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

    //! Calculate the angle to rotate to, default to the current angle.
    float angle = mCamera.angleYaw();

    //! If the camera target is moving.
    if( mCamera.targetDirection().length() > 0.0f )
    {
        //! *Calculate the heading angle of the moving target.
        angle = (180 + qRadiansToDegrees( qAtan2( mCamera.targetDirection().z(), mCamera.targetDirection().x() ) ) );
    } // if( mCamera.targetDirection().length() > 0.0f )

    //! If there is a difference between the current Camera angle and the target heading angle.
    if( false == qFuzzyCompare( mCamera.angleYaw(), angle ) )
    {
        //! *Calculate the difference between the Camera angle and the target heading angle.
        float deltaAngle = ((angle + 360) - (mCamera.angleYaw() + 360));
        //! Clamp angle from 0 to 360.
        deltaAngle = deltaAngle < 0 ? deltaAngle + 360 : deltaAngle - 360;
        deltaAngle = deltaAngle > 180 ? (deltaAngle - 360) : deltaAngle;
        deltaAngle = deltaAngle < -180 ? (deltaAngle + 360) : deltaAngle;

        //! *Calculate an interpolation of 10% of the total angle difference or 1.0 degree which ever is greater.
        float slerp = qAbs(deltaAngle) < 1.0f ? deltaAngle : (deltaAngle *.1);

        //! *Step the Camera angle towrds the target heading angle by the interpolation angle.
        mCamera.angleYaw() += slerp;
        mCamera.angleYaw() = (mCamera.angleYaw() >= 360) ? (mCamera.angleYaw() - 360) : mCamera.angleYaw();
        mCamera.angleYaw() = (mCamera.angleYaw() < 0) ? (mCamera.angleYaw() + 360) : mCamera.angleYaw();
    } //     if( false == qFuzzyCompare( mCamera.angleYaw(), angle ) )

    //! Move the Camera to follow the target.
    mCamera.eye() += mCamera.targetPosition() - mCamera.focus();
    mCamera.focus() = mCamera.targetPosition();

    //! Rotate the Camera to the new angle.
    mCamera.rotateCamera();
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

//! Calculate the OpenGL projection matrix.
//! \return void
//! \sa Camera
void Camera::projection()
{
    //! Set the viewport to be the entire window.
    glViewport(0, 0, mWindowSize.width(), mWindowSize.height());

    //! If the current projection mode is orthoginal.
    if( mProjectMode == ORTHOGONAL )
    {
        //! -Calculate the orthogonal projection matrix.
        mProjectionMatrix.setToIdentity();
        mProjectionMatrix.ortho( 0.0f, 0, 0.0f, 1, -1.0f, 1.0f );
    } // if( mProjectMode == ORTHOGONAL )
    //! Else the current projection mode is perspective.
    else
    {
        //! -Calculate the ratio and prevent a divide by zero.
        float ratio = (float)mWindowSize.width() / ( mWindowSize.height()==0 ? 1.0f : (float)mWindowSize.height());

        //! -Calculate the perspective projection matrix.
        mProjectionMatrix.setToIdentity();
        mProjectionMatrix.perspective( mFeildOfView, ratio, mNearDistance, mFarDistance );

        //! -Update the bounding frustum with the projection settings.
        mFrustum.projection( mFeildOfView, ratio, mNearDistance, mFarDistance );
    } // if( mProjectMode == ORTHOGONAL ) else
} // Camera::projection()

//! Resized the OpenGL projection matrix.
//! \param width in pixels of the OpenGL context.
//! \param height in pixels of the OpenGL context.
//! \return void
//! \sa Camera, Engine, Window, QGLWidget
void Camera::resize( const int width, const int height )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Camera::resize...";

    //! Store the window's new width and height.
    mWindowSize = QSize( width, height );

    //! Re-calculate the projection matrix.
    projection();
} // Camera::resize( const int width, const int height )

//! Rotate the poisition of the Camera around the point of focus by the Yaw and Pitch angles.
//! \return void
//! \sa Camera
void Camera::rotateCamera()
{
    //! Rotate the Camera.
    mCameraEye.setX( mCameraFocus.x() + mCameraToTarget.length() * cos( qDegreesToRadians( mCameraAngleYaw ) ) * sin( qDegreesToRadians( mCameraAnglePitch ) ) );
    mCameraEye.setY( mCameraFocus.y() + mCameraToTarget.length() * cos( qDegreesToRadians( mCameraAnglePitch ) ) );
    mCameraEye.setZ( mCameraFocus.z() + mCameraToTarget.length() * sin( qDegreesToRadians( mCameraAngleYaw ) ) * sin( qDegreesToRadians( mCameraAnglePitch ) ) );

    //! Calculate the distance between the Camera and its point of focus.
    mCameraToTarget = mCameraEye - mCameraFocus;
} // Camera::rotateCamera()
