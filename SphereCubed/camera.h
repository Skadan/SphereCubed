/*! \file camera.h
\brief Camera class declaration file.
This file contains the Camera class declaration for the application.
*/

#ifndef CAMERA_H
#define CAMERA_H

// QMatrix4x4 header file required for model, view, projection matrices.
#include <QMatrix4x4>
// QVector3D header file required for positioning.
#include <QVector3D>

// frustum.h header file required for bounding Frustum class definition.
#include "frustum.h"
// machine.h header file required for Machine class definition.
#include "machine.h"
// settings.h header file required for application settings.
#include "settings.h"

// Pre-declared calss
class Camera;

//------------------------------------------------------------------------------
// States


//! CameraMenuState class.
/*!
This class declaration encapsulates the CameraMenuState functionality.
*/
class CameraMenuState : public State
{
    //! Friend of Camera so that only Camera can instantiate the CameraMenuState.
    friend class Camera;

private: // Constructors / Destructors
    //! CameraMenuState class constructor.
    CameraMenuState( Camera & camera );

    //! CameraMenuState class destructor.
    ~CameraMenuState();

private: // Member Function
    //! Triggered when entering.
    virtual void enter();

    //! Triggered when exiting.
    virtual void exit();

    //! Triggered after a tick or when ever scene needs to be rendered.
    virtual void render();

    //! Triggered periodically.
    virtual void tick();

private: // Member Variables
    //! Used to reference the Camera functionality.
    Camera & mCamera;
};  // class CameraMenuState

//! CameraPlayState class.
/*!
This class declaration encapsulates the CameraPlayState functionality.
*/
class CameraPlayState : public State
{
    //! Friend of Camera so that only Camera can instantiate the CameraPlayState.
    friend class Camera;

private: // Constructors / Destructors
    //! CameraPlayState class constructor.
    CameraPlayState( Camera & camera );

    //! CameraPlayState class destructor.
    ~CameraPlayState();

private: // Member Function
    //! Triggered when entering.
    virtual void enter();

    //! Triggered when exiting.
    virtual void exit();

    //! Triggered after a tick or when ever scene needs to be rendered.
    virtual void render();

    //! Triggered periodically.
    virtual void tick();

private: // Member Variables
    //! Used to reference the Camera functionality.
    Camera & mCamera;
}; // class CameraPlayState



//------------------------------------------------------------------------------
// Events


//! CameraMenuEvent class.
/*!
This class declaration encapsulates the CameraMenuEvent functionality.
*/
class CameraMenuEvent : public Event
{
    //! Friend of Camera so that only Camera can instantiate the CameraMenuEvent.
    friend class Camera;

private: // Constructors / Destructors
    //! CameraMenuEvent class constructor.
    CameraMenuEvent( Camera & camera );

    //! CameraMenuEvent class destructor.
    ~CameraMenuEvent();

private: // Member Function
    //! Test Event condition triggered.
    virtual State * test();

private: // Member Variables
    //! Used to reference the Camera functionality.
    Camera & mCamera;
};

//! CameraPlayEvent class.
/*!
This class declaration encapsulates the CameraPlayEvent functionality.
*/
class CameraPlayEvent : public Event
{
    //! Friend of Camera so that only Camera can instantiate the CameraPlayEvent.
    friend class Camera;

private: // Constructors / Destructors
    //! CameraPlayEvent class constructor.
    CameraPlayEvent( Camera & camera );

    //! CameraPlayEvent class destructor.
    ~CameraPlayEvent();

private: // Member Function
    //! Test Event condition triggered.
    virtual State * test();

private: // Member Variables
    //! Used to reference the Camera functionality.
    Camera & mCamera;
};




//------------------------------------------------------------------------------
// Machine


//! Camera class.
/*!
This class declaration encapsulates the Camera functionality.
The World instantiates a Camera to manage the Camera State Machine.
*/
class Camera : public Machine
{
    //! Friend of World so that only World can instantiate the Camera.
    friend class World;

public:
    //! Access function to get/set Camera pitch angle.
    //! \return float reference to the Camera pitch angle in degrees.
    //! \sa Camera
    float & anglePitch() { return mCameraAnglePitch; }

    //! Access function to get/set Camera yaw angle.
    //! \return float reference to the Camera yaw angle in degrees.
    //! \sa Camera
    float & angleYaw() { return mCameraAngleYaw; }

    //! Determine if the bounding Box is with in the view of the Camera.
    //! \param box const reference to the box to test.
    //! \return bool is true if the Box is with in the view of the Camera.
    //! \sa Camera
    bool boxViewable( const Box & box ) const
    { return ( ( mFrustum.boxViewable( box ) == Frustum::HALFSPACE::OUT ) ? false : true); }

    //! Access function to get Camera Up direction.
    //! \return QVector reference to the Camera Up direction vector.
    //! \sa Camera
    QVector3D& direction() { return mCameraDirection; }

    //! Access function to get Camera position.
    //! \return QVector reference to the Camera position.
    //! \sa Camera
    QVector3D& eye() { return mCameraEye; }

    //! Access function to get Camera position of focus.
    //! \return QVector reference to the Camera position of focus.
    //! \sa Camera
    QVector3D& focus() { return mCameraFocus; }

    //! Access function to get Camera Menu condition trigger.
    //! \return bool is the state.
    //! \sa Camera
    bool getMenu() { return mMenu; }

    //! Access function to get Camera Play condition trigger.
    //! \return bool is the state.
    //! \sa Camera
    bool getPlay() { return mPlay; }

    //! Access function to get Camera projection matrix..
    //! \return QMatrix4x4 reference to the Camera projection matrix.
    //! \sa Camera
    QMatrix4x4& projectionMatrix() { return mProjectionMatrix; }

    //! Const access function to get Camera projection matrix..
    //! \return const QMatrix4x4 reference to the Camera projection matrix.
    //! \sa Camera
    const QMatrix4x4& projectionMatrix() const { return mProjectionMatrix; }

    //! Triggered whenever the OpenGL context needs to be resized.
    void resize(const int width, const int height);

    //! Called to rotate the Camera to the current angle settings.
    void rotateCamera();

    //! Access function to set the Camera Menu condition trigger.
    //! \param value is the state to set trigger to.
    //! \sa Camera
    void setMenu( bool value ) { mMenu = value; }

    //! Access function to set the Camera Play condition trigger.
    //! \param value is the state to set trigger to.
    //! \sa Camera
    void setPlay( bool value ) { mPlay = value; }

    //! Access function to get/set the Camera target direction..
    //! \return QVector3D reference to the Camera target direction.
    //! \sa Camera
    QVector3D & targetDirection() { return mCameraTargetDirection; }

    //! Access function to get/set the Camera target distance..
    //! \return QVector3D reference to the Camera target distence.
    //! \sa Camera
    QVector3D & targetDistance()  { return mCameraToTarget;   }

    //! Access function to get/set the Camera target position..
    //! \return QVector3D reference to the Camera target position.
    //! \sa Camera
    QVector3D & targetPosition()  { return mCameraTargetPosition; }

    //! Access function to setup the bounding Frustum planes for the new settings.
    //! \param eye is the position of the Camera.
    //! \param focus is the position the Camera is pointer towards.
    //! \param up is the up direction of the camera.
    //! \return void
    //! \sa Camera, Frustum, Box, Cube
    void view( const QVector3D & eye, const QVector3D & focus, const QVector3D & up )
    { mFrustum.view( eye, focus, up ); }

    //! Access function to get Camera view matrix..
    //! \return QMatrix4x4 reference to the Camera view matrix.
    //! \sa Camera
    QMatrix4x4& viewMatrix() { return mViewMatrix; }

    //! Const access function to get Camera view matrix..
    //! \return const QMatrix4x4 reference to the Camera view matrix.
    //! \sa Camera
    const QMatrix4x4& viewMatrix() const { return mViewMatrix; }

private: // Constructors / Destructors
    //! Camera class constructor.
    Camera();

    //! Camera class destructor.
    ~Camera();

private: // Member Function
    //! Configure the Machine.
    void configure();

private: // Member Variables
    //! Used to store the Camera yaw angle, initialize to 270.0 degrees.
    float mCameraAngleYaw = 270.0f;

    //! Used to store the Camera pitch angle, initialize to 45.0 degrees.
    float mCameraAnglePitch = 45.0f;

    //! Used to store the up direction of Camera.
    QVector3D mCameraDirection;

    //! Used to store the position of the Camera.
    QVector3D mCameraEye;

    //! Used to store the focal position of the Camera.
    QVector3D mCameraFocus;

    //! Used to store the distance between the focal position and the Camera.
    QVector3D mCameraToTarget;

    //! Used to store the far clipping plane of the Camera, initialize to the default distance.
    float mFarDistance = CAMERA_FAR_CLIPPING_PLANE_DISTANCE;

    //! Used to store the feild of view of the Camera, initialize to the default angle.
    float mFeildOfView = CAMERA_FEILD_OF_VIEW_ANGLE_DEGREES;

    //! Used to calculate the Camera view bounding Frustum.
    Frustum mFrustum;

    //! Used to store the Camera Menu Event condition trigger, initialize to the false.
    bool mMenu = false;

    //! Used to store the near clipping plane of the Camera, initialize to the default distance.
    float mNearDistance = CAMERA_NEAR_CLIPPING_PLANE_DISTANCE;

    //! Used to store the Camera Play Event condition trigger, initialize to the false.
    bool mPlay = false;

    //! Used to store the Camera projection matrix.
    QMatrix4x4 mProjectionMatrix;

    //! Used to store the direction the target is moving.
    QVector3D mCameraTargetDirection;

    //! Used to store the position of the target.
    QVector3D mCameraTargetPosition;

    //! Used to store the Camera view matrix.
    QMatrix4x4 mViewMatrix;
}; // class Camera

#endif // CAMERA_H
