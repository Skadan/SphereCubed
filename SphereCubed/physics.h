/*! \file physics.h
\brief Physics class declaration file.
This file contains the Physics class declaration for the application.
*/

#ifndef PHYSICS_H
#define PHYSICS_H

// level.h header file required for level data access.
#include "level.h"
// player.h header file required for player data access.
#include "player.h"

//! Physics class.
/*!
This class declaration encapsulates the Physics functionality.
*/
class Physics
{
    //! Friend of World so that only World can instantiate the Physics.
    friend class World;

public: // Member Functions
    //! Called to update the Physics.
    void tick();

private: // Constructors / Destructors
    //! Physics class constructor.
    Physics( int timeInterval, Level & level, Player & player );

    //! Physics class destructor.
    ~Physics();

private: // Data Types
    //! Internal collision data structure.
    struct CollisionData
    {
        //! - Collision distance.
        float distance;
        //! - Collision normal.
        QVector3D normal;
        //! - Collision cube position.
        QVector3D cubePosition;
    }; // struct CollisionData

private: // Member Variables
    //! Used to track the next collision.
    CollisionData mCollisionData;

    //! Used to store the size of a Level cube.
    float mHalfCubeSize;

    //! Used to store the radius of the Player sphere.
    float mSphereRadius;

    //! Used to store the time delta to apply the physics over in seconds.
    float mTimeInterval;

    //! Used to hold a reference to the Level data;
    Level  & mLevel;

    //! Used to hold a reference to the Player data;
    Player & mPlayer;

private: // Data Constants
    //! Standard gravity constant.
    const float GRAVITY = 9.81f;

    //! Minimum velocity to eliminate oscillations caused by limited floating point precision.
    const float MINIMUM_VELOCITY = 0.05f;

private: // Helper Functions
    //! Called to get the closest point on a given ray to a given point.
    QVector3D closestPointOnRayToPoint( QVector3D & rayStart, QVector3D & rayDirection, QVector3D & point );

    //! Called to detect next collision.
    void collisionDetection( QVector3D & position, QVector3D & velocity );

    //! Called to detect collisions with corners of Cube.
    void collisionDetectionCornerTest( QVector3D & position, QVector3D & velocity );

    //! Called to detect collisions with edges of Cube.
    void collisionDetectionEdgeTest( QVector3D & position, QVector3D & velocity );

    //! Called to detect collisions with faces of Cube.
    void collisionDetectionFaceTest( QVector3D & position, QVector3D & velocity );

    //! Called to respond to detected collisions.
    void collisionResponse( QVector3D & position, QVector3D & velocity );

    //! Called to get the distance on a given ray to intercept with given plane.
    float distanceOnRayToPlane( QVector3D & rayStart, QVector3D & rayDirection, QVector3D & planePoint, QVector3D & planeNormal );

    //! Called to caclulate all the forces.
    void forces();

    //! Called to apply all the forces.
    void movement();

    //! Called to get a reflection of a given vector around a given normal.
    QVector3D reflectionVector( QVector3D & input, QVector3D & normal );

    //! Called to remove precision error from floating point value.
    float removeFuzz( const float input );

    //! Called to remove precision error from vector of floating point value.
    QVector3D removeFuzz( const QVector3D & input);

    //! Called to roll the player by the given distance.
    void roll( QVector3D & distance );
};

#endif // PHYSICS_H
