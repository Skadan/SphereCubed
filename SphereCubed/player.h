/*! \file player.h
\brief Player class declaration file.
This file contains the Player class declaration for the application.
*/

#ifndef PLAYER_H
#define PLAYER_H

// QObject header file required for events.
#include <QObject>

// camera.h header file required for rendering.
#include "camera.h"
// light.h header file required for rendering.
#include "light.h"
// sphere.h header file required for class definition.
#include "sphere.h"

//! Player class.
/*!
This class declaration encapsulates the Player functionality.
*/
class Player : public QObject
{
    // Q_OBJECT macro required to use services provided by Qt's meta-object system.
    Q_OBJECT

    //! Friend of World so that only World can instantiate the Level.
    friend class World;

public: // Access Functions
    //! Called to get/set the applied force to the player.
    QVector3D & force() { return mForce; }

    //! Called to get/set the Players mass.
    float & mass() { return mMass; }

    //! Called to get/set the player's position.
    QVector3D & position()  { return mPosition; }

    //! Called to get the player's sphere radius.
    float sphereRadius() { return mSphereRadius; }

    //! Called to get/set the player's rolling resistance.
    float & rollingResistanceCoefficient() { return mRollingResistanceCoefficient; }

    //! Called to get/set the player's rotation.
    QQuaternion & rotation() { return mRotation; }

    //! Called to get/set the player's terminal velocity.
    float & terminalLinearVelocity() { return mTerminalVelocity; }

    //! Called to get/set the player's velocity.
    QVector3D & velocity() { return mVelocity; }

public: // Member Functions
    //! Called to load the Player OpenGL resources.
    void load();

    //! Called to render the Player OpenGL resources.
    void render( const Camera & camera, const Light & light );

    //! Called to update the Player.
    void tick();

    //! Called to unload the Player OpenGL resources.
    void unload();

private: // Constructors / Destructors
    //! Player class constructor.
    explicit Player(QObject *parent = 0);

    //! Player class destructor.
    ~Player();

private: // Member Functions
    //! Triggered when an event is received.
    virtual bool event(QEvent *event);

private:
    //! Used to store the direction to apply the user force to the player.
    QVector3D mDirection = QVector3D(0,0,1);

    //! Used to store the force the user is applying to the player.
    QVector3D mForce;

    //! Used to store the player's mass.
    float mMass = 0.010f;

    //! Used to store the position of the player.
    QVector3D mPosition;

    //! Used to store the player's rolling resistance coefficent.
    float mRollingResistanceCoefficient = 0.005f;

    //! Used to store the rotation of the player.
    QQuaternion mRotation;

    //! Used to store the Sphere representing the Player.
    Sphere mSphere;

    //! Used to store the Sphere radius.
    const float mSphereRadius = 0.5f;

    //! Used to store amount of force the user can apply.
    float mUserStrength = 0.1f;

    //! Used to store the player's terminal velocity.
    float mTerminalVelocity = 3.0f;

    //! Used to store the player's velocity.
    QVector3D mVelocity;
}; // class Player

#endif // PLAYER_H
