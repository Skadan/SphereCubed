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
    //! Called to get/set the player position.
    QVector3D& position()  { return mPosition; }

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
    //! Used to store the Sphere representing the Player.
    Sphere mSphere;

    //! Used to store the position of the player.
    QVector3D mPosition;

    //! Used to store the rotation of the player.
    QQuaternion mRotation;
}; // class Player

#endif // PLAYER_H
