/*! \file physics.cpp
\brief Physics class implementation file.
This file contains the Physics class implementation for the application.
*/

// physics.h header file required for class definition.
#include "physics.h"

// float.h header file required for FLT_EPSILON and FLT_MAX.
#include <float.h>
// QQuaternion header required for rolling.
#include <QQuaternion>
// QtMath Required for round, floor, ceil...
#include <QtMath>

// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION false

//! The Physics execution begins here.
//! Create the Physics.
//! Automatically called when the World is constructed.
//! \param timeInterval is the time interval in milliseconds between Physics updates.
//! \param level is a refernece to the Level data.
//! \param player is a reference to the Player data.
Physics::Physics(int timeInterval, Level &level, Player &player) :
    mTimeInterval( timeInterval * 0.001f ), mLevel( level ), mPlayer( player )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Physics::Physics(int timeInterval, Level &level, Player &player)...";
} // Physics::Physics(int timeInterval, Level &level, Player &player)

//! The Physics execution ends here.
//! Required to free the Physics allocated memory.
//! Automatically called when the World is destructed.
Physics::~Physics()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Physics::~Physics()...";
} // Physics::~Physics()

//! Calculate the closest point on the given ray to the given point.
//! \param rayStart is the point where the ray starts.
//! \param rayDirection is the direction of the ray.
//! \param point is the given point.
//! \return QVector3D is the closet point on the ray to the given point.
//! \sa Physics
QVector3D Physics::closestPointOnRayToPoint( QVector3D & rayStart, QVector3D & rayDirection, QVector3D & point )
{
    //! Calculate the ray between the ray start point and the given point.
    QVector3D rayToPoint = point - rayStart;

    //! Calculate the dot product between that ray and the given ray direction.
    float dotProduct = QVector3D::dotProduct( rayDirection, rayToPoint );

    //! Calculate the closet point on th given ray.
    QVector3D closest    = rayStart + ( dotProduct * rayDirection );

    //! Return the closest point.
    return closest;
} // Physics::closestPointOnRayToPoint( QVector3D & rayStart, QVector3D & rayDirection, QVector3D & point )

//! Test for collisions between the Level and the Player.
//! \param position is the current internal position of the player for time interval.
//! \param velocity is the current internal velocity of the player for time interval.
//! \return void
void Physics::collisionDetection( QVector3D & position, QVector3D & velocity )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Physics::collisionDetection( QVector3D position, QVector3D velocity )...";

    //! Reset the internal collision data tracking information for the start of a new time interval.
    mCollisionData = { FLT_MAX, QVector3D(0.0f,0.0f,0.0f), QVector3D(0,0,0) };

    //! If the player is in the Level.
    if( true == mLevel.insideLevel( position ) )
    {
        //! *Test for collisions along the path to travel.
        collisionDetectionFaceTest(   position, velocity );
        collisionDetectionEdgeTest(   position, velocity );
        collisionDetectionCornerTest( position, velocity );
    } // if( true == mLevel.insideLevel( playerPosition ) )
} // Physics::collisionDetection( QVector3D position, QVector3D velocity )

//! Test for collisions between the Player and corners of Cubes.
//! \param position is the current internal position of the player for time interval.
//! \param velocity is the current internal velocity of the player for time interval.
//! \return void
//! \todo Add to collisionDetectionCornerTest test for top corners - currently not required.
void Physics::collisionDetectionCornerTest( QVector3D &position, QVector3D &velocity )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Physics::collisionDetectionCornerTest( QVector3D &position, QVector3D &velocity )...";

    //! Create variable to store distance from destination to corner.
    float distance = 0.0f;
    //! Create variable to store the defined corner to test.
    QVector3D corner;
    //! Determine what Cube the Player currently occupies.
    QVector3D currentCube = QVector3D( qRound( position.x() ), qRound( position.y() ), qRound( position.z() ) );
    //! Determine where destination would be without any collisions occurring.
    QVector3D destination = position + velocity;

    //! "Test bottom far right of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the corner.
    corner = QVector3D( (currentCube.x() + mHalfCubeSize), (currentCube.y() - mHalfCubeSize), (currentCube.z() - mHalfCubeSize) );
    //! Calculate the distance to the corner from the destination.
    distance = destination.distanceToPoint( corner ) - mSphereRadius;
    //! If the Player would would have collided with the corner.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the corner from the current position.
        float currentDistance = position.distanceToPoint( corner ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the corner.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage > 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) >= 0 )
            {
                //! ***Determine which Cube could have been hit.
                QVector3D cube         = QVector3D( qFloor( corner.x() ), qFloor( corner.y() ), qCeil( corner.z() ) );
                QVector3D cubeFarRight = QVector3D( (cube.x() + 1),       cube.y(),             (cube.z() - 1) );
                QVector3D cubeFar      = QVector3D( cube.x(),             cube.y(),             (cube.z() - 1) );
                QVector3D cubeRight    = QVector3D( (cube.x() + 1),       cube.y(),              cube.z() );

                //! ***If there is a Cube at that height then there is collision.
                //! ***Keep track of the new closest collision.
                if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + (velocity * percentage) - corner).normalized();
                    mCollisionData.cubePosition = cube;
                } // if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                else if( true == mLevel.cubeAtPosition( cubeFarRight.x(), cubeFarRight.y(), cubeFarRight.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + (velocity * percentage) - corner).normalized();
                    mCollisionData.cubePosition = cubeFarRight;
                } // else if( true == mLevel.cubeAtPosition( cubeFarRight.x(), cubeFarRight.y(), cubeFarRight.z() ) )
                else if( true == mLevel.cubeAtPosition( cubeFar.x(), cubeFar.y(), cubeFar.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + (velocity * percentage) - corner).normalized();
                    mCollisionData.cubePosition = cubeFar;
                } // else if( true == mLevel.cubeAtPosition( cubeFar.x(), cubeFar.y(), cubeFar.z() ) )
                else if( true == mLevel.cubeAtPosition( cubeRight.x(), cubeRight.y(), cubeRight.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + (velocity * percentage) - corner).normalized();
                    mCollisionData.cubePosition = cubeRight;
                } // else if( true == mLevel.cubeAtPosition( cubeRight.x(), cubeRight.y(), cubeRight.z() ) )
            } // if( (mCollisionData.distance - distance) >= 0 )
        } // if( percentage <= 1.0f && percentage > 0.0f )
    } // if( distance < FLT_EPSILON)

    //! "Test bottom near right of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the corner.
    corner = QVector3D( (currentCube.x() + mHalfCubeSize), (currentCube.y() - mHalfCubeSize), (currentCube.z() + mHalfCubeSize) );
    //! Calculate the distance to the corner from the destination.
    distance = destination.distanceToPoint( corner ) - mSphereRadius;
    //! If the Player would would have collided with the corner.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the corner from the current position.
        float currentDistance = position.distanceToPoint( corner ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the corner.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage > 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) >= 0 )
            {
                //! ***Determine which Cube could have been hit.
                QVector3D cube          = QVector3D( qFloor( corner.x() ), qFloor( corner.y() ), qFloor( corner.z() ) );
                QVector3D cubeNearRight = QVector3D( (cube.x() + 1),       cube.y(),             (cube.z() + 1) );
                QVector3D cubeNear      = QVector3D( cube.x(),             cube.y(),             (cube.z() + 1) );
                QVector3D cubeRight     = QVector3D( (cube.x() + 1),       cube.y(),             cube.z() );

                //! ***If there is a Cube at that height then there is collision.
                //! ***Keep track of the new closest collision.
                if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cube;
                } // if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                else if( true == mLevel.cubeAtPosition( cubeNearRight.x(), cubeNearRight.y(), cubeNearRight.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cubeNearRight;
                } // else if( true == mLevel.cubeAtPosition( cubeNearRight.x(), cubeNearRight.y(), cubeNearRight.z() ) )
                else if( true == mLevel.cubeAtPosition( cubeNear.x(), cubeNear.y(), cubeNear.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cubeNear;
                } // else if( true == mLevel.cubeAtPosition( cubeNear.x(), cubeNear.y(), cubeNear.z() ) )
                else if( true == mLevel.cubeAtPosition( cubeRight.x(), cubeRight.y(), cubeRight.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cubeRight;
                } // else if( true == mLevel.cubeAtPosition( cubeRight.x(), cubeRight.y(), cubeRight.z() ) )
            } // if( (mCollisionData.distance - distance) >= 0 )
        } // if( percentage <= 1.0f && percentage > 0.0f )
    } // if( distance < FLT_EPSILON)

    //! "Test bottom near left of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the corner.
    corner = QVector3D( (currentCube.x() - mHalfCubeSize), (currentCube.y() - mHalfCubeSize), (currentCube.z() + mHalfCubeSize) );
    //! Calculate the distance to the corner from the destination.
    distance = destination.distanceToPoint( corner ) - mSphereRadius;
    //! If the Player would would have collided with the corner.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the corner from the current position.
        float currentDistance = position.distanceToPoint( corner ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the corner.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage > 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) > 0 )
            {
                //! ***Determine which Cube could have been hit.
                QVector3D cube         = QVector3D( qCeil( corner.x() ), qFloor( corner.y() ), qFloor( corner.z() ) );
                QVector3D cubeNearLeft = QVector3D( (cube.x() - 1),      cube.y(),             (cube.z() + 1) );
                QVector3D cubeNear     = QVector3D( cube.x(),            cube.y(),             (cube.z() + 1) );
                QVector3D cubeLeft     = QVector3D( (cube.x() - 1),      cube.y(),             cube.z() );

                //! ***If there is a Cube at that height then there is collision.
                //! ***Keep track of the new closest collision.
                if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cube;
                } // if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                else if( true == mLevel.cubeAtPosition( cubeNearLeft.x(), cubeNearLeft.y(), cubeNearLeft.z()   ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cubeNearLeft;
                } // else if( true == mLevel.cubeAtPosition( cubeNearLeft.x(), cubeNearLeft.y(), cubeNearLeft.z()   ) )
                else if( true == mLevel.cubeAtPosition( cubeNear.x(), cubeNear.y(), cubeNear.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cubeNear;
                } // else if( true == mLevel.cubeAtPosition( cubeNear.x(), cubeNear.y(), cubeNear.z() ) )
                else if( true == mLevel.cubeAtPosition( cubeLeft.x(), cubeLeft.y(), cubeLeft.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cubeLeft;
                } // else if( true == mLevel.cubeAtPosition( cubeLeft.x(), cubeLeft.y(), cubeLeft.z() ) )
            } // if( (mCollisionData.distance - distance) >= 0 )
        } // if( percentage <= 1.0f && percentage > 0.0f )
    } // if( distance < FLT_EPSILON)

    //! "Test bottom far left of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the corner.
    corner = QVector3D( (currentCube.x() - mHalfCubeSize), (currentCube.y() - mHalfCubeSize), (currentCube.z() - mHalfCubeSize) );
    //! Calculate the distance to the corner from the destination.
    distance = destination.distanceToPoint( corner ) - mSphereRadius;
    //! If the Player would would have collided with the corner.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the corner from the current position.
        float currentDistance = position.distanceToPoint( corner ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the corner.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage > 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) >= 0 )
            {
                //! ***Determine which Cube could have been hit.
                QVector3D cube        = QVector3D( qCeil( corner.x() ), qFloor( corner.y() ), qCeil( corner.z() ) );
                QVector3D cubeFarLeft = QVector3D( (cube.x() - 1),      cube.y(),             (cube.z() - 1) );
                QVector3D cubeFar     = QVector3D( cube.x(),            cube.y(),             (cube.z() - 1) );
                QVector3D cubeLeft    = QVector3D( (cube.x() - 1),      cube.y(),             cube.z() );

                //! ***If there is a Cube at that height then there is collision.
                //! ***Keep track of the new closest collision.
                if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cube;
                } // if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                else if( true == mLevel.cubeAtPosition( cubeFarLeft.x(), cubeFarLeft.y(), cubeFarLeft.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cubeFarLeft;
                } // else if( true == mLevel.cubeAtPosition( cubeFarLeft.x(), cubeFarLeft.y(), cubeFarLeft.z() ) )
                else if( true == mLevel.cubeAtPosition( cubeFar.x(), cubeFar.y(), cubeFar.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cubeFar;
                } // else if( true == mLevel.cubeAtPosition( cubeFar.x(), cubeFar.y(), cubeFar.z() ) )
                else if( true == mLevel.cubeAtPosition( cubeLeft.x(), cubeLeft.y(), cubeLeft.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (position + velocity * percentage - corner).normalized();
                    mCollisionData.cubePosition = cubeLeft;
                } // else if( true == mLevel.cubeAtPosition( cubeLeft.x(), cubeLeft.y(), cubeLeft.z() ) )
            } // if( (mCollisionData.distance - distance) >= 0 )
        } // if( percentage <= 1.0f && percentage > 0.0f )
    } // if( distance < FLT_EPSILON)
} // Physics::collisionDetectionCornerTest( QVector3D &position, QVector3D &velocity )

//! Test for collisions between the Player and edges of Cubes.
//! \param position is the current internal position of the player for time interval.
//! \param velocity is the current internal velocity of the player for time interval.
//! \return void
//! \todo Add to collisionDetectionEdgeTest test for top edges - currently not required.
void Physics::collisionDetectionEdgeTest( QVector3D &position, QVector3D &velocity )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Physics::collisionDetectionEdgeTest( QVector3D &position, QVector3D &velocity )...";

    //! Create variable to store distance from destination to edge.
    float distance = 0.0f;
    //! Create variable to store the a point on the edge to test.
    QVector3D edgeRayStartPoint;
    //! Create variable to store the direction of the edge to test.
    QVector3D edgeRayDirection;
    //! Determine what Cube the Player currently occupies.
    QVector3D currentCube = QVector3D( qRound( position.x() ), qRound( position.y() ), qRound( position.z() ) );
    //! Determine where destination would be without any collisions occurring.
    QVector3D destination = position + velocity;

    //! "Test the far right edge of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the edge ray start point and direction.
    edgeRayStartPoint = QVector3D( (currentCube.x() + mHalfCubeSize), 0.0f, (currentCube.z() - mHalfCubeSize) );
    edgeRayDirection  = QVector3D( 0.0f, 1.0f, 0.0f );
    //! Calculate the distance to the edge from the destination.
    distance = destination.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
    //! If the Player would would have collided with the edge.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the edge from the current position.
        float currentDistance = position.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the edge.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage >= 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) > 0 )
            {
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = closestPointOnRayToPoint( edgeRayStartPoint, edgeRayDirection, destination );

                //! ***Determine which Cube could have been hit.
                QVector3D cube = QVector3D( qCeil( collisionPosition.x() ), qCeil( collisionPosition.y() ), qFloor( collisionPosition.z() ) );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cube;
                } // if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
            } // if( (mCollisionData.distance - distance) > 0 )
        } // if( percentage <= 1.0f && percentage >= 0.0f )
    } // if( distance < FLT_EPSILON)

    //! "Test the near right edge of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the edge ray start point and direction.
    edgeRayStartPoint = QVector3D( (currentCube.x() + mHalfCubeSize), 0.0f, (currentCube.z() + mHalfCubeSize) );
    edgeRayDirection  = QVector3D( 0.0f, 1.0f, 0.0f );
    //! Calculate the distance to the edge from the destination.
    distance = destination.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
    //! If the Player would would have collided with the edge.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the edge from the current position.
        float currentDistance = position.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the edge.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage >= 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) > 0 )
            {
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = closestPointOnRayToPoint( edgeRayStartPoint, edgeRayDirection, destination );

                //! ***Determine which Cube could have been hit.
                QVector3D cube = QVector3D( qCeil( collisionPosition.x() ), qCeil( collisionPosition.y() ), qCeil( collisionPosition.z() ) );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cube;
                } // if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
            } // if( (mCollisionData.distance - distance) > 0 )
        } // if( percentage <= 1.0f && percentage >= 0.0f )
    } // if( distance < FLT_EPSILON)

    //! "Test the near left edge of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the edge ray start point and direction.
    edgeRayStartPoint = QVector3D( (currentCube.x() - mHalfCubeSize), 0.0f, (currentCube.z() + mHalfCubeSize) );
    edgeRayDirection  = QVector3D( 0.0f, 1.0f, 0.0f );
    //! Calculate the distance to the edge from the destination.
    distance = destination.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
    //! If the Player would would have collided with the edge.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the edge from the current position.
        float currentDistance = position.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the edge.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage >= 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) > 0 )
            {
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = closestPointOnRayToPoint( edgeRayStartPoint, edgeRayDirection, destination );

                //! ***Determine which Cube could have been hit.
                QVector3D cube = QVector3D( qFloor( collisionPosition.x() ), qCeil( collisionPosition.y() ), qCeil( collisionPosition.z() ) );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cube;
                } // if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
            } // if( (mCollisionData.distance - distance) > 0 )
        } // if( percentage <= 1.0f && percentage >= 0.0f )
    } // if( distance < FLT_EPSILON)

    //! "Test the far left edge of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the edge ray start point and direction.
    edgeRayStartPoint = QVector3D( (currentCube.x() - mHalfCubeSize), 0.0f, (currentCube.z() - mHalfCubeSize) );
    edgeRayDirection  = QVector3D( 0.0f, 1.0f, 0.0f );
    //! Calculate the distance to the edge from the destination.
    distance = destination.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
    //! If the Player would would have collided with the edge.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the edge from the current position.
        float currentDistance = position.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the edge.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage >= 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) > 0 )
            {
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = closestPointOnRayToPoint( edgeRayStartPoint, edgeRayDirection, destination );

                //! ***Determine which Cube could have been hit.
                QVector3D cube = QVector3D( qFloor( collisionPosition.x() ), qCeil( collisionPosition.y() ), qFloor( collisionPosition.z() ) );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cube;
                } // if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
            } // if( (mCollisionData.distance - distance) > 0 )
        } // if( percentage <= 1.0f && percentage >= 0.0f )
    } // if( distance < FLT_EPSILON)

    //! "Test the bottom far edge of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the edge ray start point and direction.
    edgeRayStartPoint = QVector3D( 0.0f, (currentCube.y() - mHalfCubeSize), (currentCube.z() - mHalfCubeSize) );
    edgeRayDirection  = QVector3D( 1.0f, 0.0f, 0.0f );
    //! Calculate the distance to the edge from the destination.
    distance = destination.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
    //! If the Player would would have collided with the edge.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the edge from the current position.
        float currentDistance = position.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the edge.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage >= 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) > 0 )
            {
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = closestPointOnRayToPoint( edgeRayStartPoint, edgeRayDirection, destination );

                //! ***Determine which Cube could have been hit.
                QVector3D cubeNear = QVector3D( qRound( collisionPosition.x() ), qFloor( collisionPosition.y() ), qCeil( collisionPosition.z() ) );
                QVector3D cubeFar  = QVector3D( cubeNear.x(),                    cubeNear.y(),                    (cubeNear.z() - 1) );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( mLevel.cubeAtPosition( cubeNear.x(), cubeNear.y(), cubeNear.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cubeNear;
                } // if( mLevel.cubeAtPosition( cubeNear.x(), cubeNear.y(), cubeNear.z() ) == true )
                else if( mLevel.cubeAtPosition( cubeFar.x(), cubeFar.y(), cubeFar.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cubeFar;
                } // if( mLevel.cubeAtPosition( cubeFar.x(), cubeFar.y(), cubeFar.z() ) == true )
            } // if( (mCollisionData.distance - distance) > 0 )
        } // if( percentage <= 1.0f && percentage >= 0.0f )
    } // if( distance < FLT_EPSILON)

    //! "Test the bottom near edge of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the edge ray start point and direction.
    edgeRayStartPoint = QVector3D( 0.0f, (currentCube.y() - mHalfCubeSize), (currentCube.z() + mHalfCubeSize) );
    edgeRayDirection  = QVector3D( 1.0f, 0.0f, 0.0f );
    //! Calculate the distance to the edge from the destination.
    distance = destination.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
    //! If the Player would would have collided with the edge.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the edge from the current position.
        float currentDistance = position.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the edge.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage >= 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) > 0 )
            {
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = closestPointOnRayToPoint( edgeRayStartPoint, edgeRayDirection, destination );

                //! ***Determine which Cube could have been hit.
                QVector3D cubeFar  = QVector3D( qRound( collisionPosition.x() ), qFloor( collisionPosition.y() ), qFloor( collisionPosition.z() ) );
                QVector3D cubeNear = QVector3D( cubeFar.x(),                     cubeFar.y(),                     (cubeFar.z() + 1) );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( mLevel.cubeAtPosition( cubeFar.x(), cubeFar.y(), cubeFar.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cubeFar;
                } // if( mLevel.cubeAtPosition( cubeFar.x(), cubeFar.y(), cubeFar.z() ) == true )
                else if( mLevel.cubeAtPosition( cubeNear.x(), cubeNear.y(), cubeNear.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cubeNear;
                } // if( mLevel.cubeAtPosition( cubeNear.x(), cubeNear.y(), cubeNear.z() ) == true )
            } // if( (mCollisionData.distance - distance) > 0 )
        } // if( percentage <= 1.0f && percentage >= 0.0f )
    } // if( distance < FLT_EPSILON)

    //! "Test the bottom right edge of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the edge ray start point and direction.
    edgeRayStartPoint = QVector3D( (currentCube.x() + mHalfCubeSize), (currentCube.y() - mHalfCubeSize), 0.0f );
    edgeRayDirection  = QVector3D( 0.0f, 0.0f, 1.0f );
    //! Calculate the distance to the edge from the destination.
    distance = destination.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
    //! If the Player would would have collided with the edge.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the edge from the current position.
        float currentDistance = position.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the edge.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage >= 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) > 0 )
            {
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = closestPointOnRayToPoint( edgeRayStartPoint, edgeRayDirection, destination );

                //! ***Determine which Cube could have been hit.
                QVector3D cubeLeft  = QVector3D( qFloor( collisionPosition.x() ), qFloor( collisionPosition.y() ), qRound( collisionPosition.z() ) );
                QVector3D cubeRight = QVector3D( (cubeLeft.x() + 1),              cubeLeft.y(),                     cubeLeft.z() );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( mLevel.cubeAtPosition( cubeLeft.x(), cubeLeft.y(), cubeLeft.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cubeLeft;
                } // if( mLevel.cubeAtPosition( cubeLeft.x(), cubeLeft.y(), cubeLeft.z() ) == true )
                else if( mLevel.cubeAtPosition( cubeRight.x(), cubeRight.y(), cubeRight.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cubeRight;
                } // if( mLevel.cubeAtPosition( cubeRight.x(), cubeRight.y(), cubeRight.z() ) == true )
            } // if( (mCollisionData.distance - distance) > 0 )
        } // if( percentage <= 1.0f && percentage >= 0.0f )
    } // if( distance < FLT_EPSILON)

    //! "Test the bottom left edge of current Cube."
    //! _________________________________________________________________________________________________
    //! Define the edge ray start point and direction.
    edgeRayStartPoint = QVector3D( (currentCube.x() - mHalfCubeSize), (currentCube.y() - mHalfCubeSize), 0.0f );
    edgeRayDirection  = QVector3D( 0.0f, 0.0f, 1.0f );
    //! Calculate the distance to the edge from the destination.
    distance = destination.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
    //! If the Player would would have collided with the edge.
    if( distance < FLT_EPSILON)
    {
        //! *Calculate the distance to the edge from the current position.
        float currentDistance = position.distanceToLine( edgeRayStartPoint, edgeRayDirection ) - mSphereRadius;
        //! *Calculate the total distance traveling directly towards the edge.
        float delta = currentDistance - distance;
        //! *Calculate the percentage of the distance to collision.
        float percentage = 1.0f - (-distance / delta);
        //! *If the percentage is between 0 and 100%.
        if( percentage <= 1.0f && percentage >= 0.0f )
        {
            //! **Calculate the distance to travel along the velocity vector.
            distance = velocity.length() * percentage;
            //! **If the distance is less then the currently tracked collision distance.
            if( (mCollisionData.distance - distance) > 0 )
            {
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = closestPointOnRayToPoint( edgeRayStartPoint, edgeRayDirection, destination );

                //! ***Determine which Cube could have been hit.
                QVector3D cubeRight = QVector3D( qCeil( collisionPosition.x() ), qFloor( collisionPosition.y() ), qRound( collisionPosition.z() ) );
                QVector3D cubeLeft  = QVector3D( (cubeRight.x() - 1),            cubeRight.y(),                   cubeRight.z() );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( mLevel.cubeAtPosition( cubeRight.x(), cubeRight.y(), cubeRight.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cubeRight;
                } // if( mLevel.cubeAtPosition( cubeRight.x(), cubeRight.y(), cubeRight.z() ) == true )
                else if( mLevel.cubeAtPosition( cubeLeft.x(), cubeLeft.y(), cubeLeft.z() ) == true )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = (destination - collisionPosition).normalized();
                    mCollisionData.cubePosition = cubeLeft;
                } // if( mLevel.cubeAtPosition( cubeLeft.x(), cubeLeft.y(), cubeLeft.z() ) == true )
            } // if( (mCollisionData.distance - distance) > 0 )
        } // if( percentage <= 1.0f && percentage >= 0.0f )
    } // if( distance < FLT_EPSILON)
} // Physics::collisionDetectionEdgeTest( QVector3D & position, QVector3D & velocity )

//! Test for collisions between the Player and faces of Cubes.
//! \param position is the current internal position of the player for time interval.
//! \param velocity is the current internal velocity of the player for time interval.
//! \return void
//! \todo Add to collisionDetectionFaceTest test for top face - currently not required.
void Physics::collisionDetectionFaceTest( QVector3D & position, QVector3D & velocity )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Physics::collisionDetectionFaceTest( QVector3D & position, QVector3D & velocity )...";

    //! Determine what Cube the Player currently occupies.
    QVector3D currentCube = QVector3D( qRound( position.x() ), qRound( position.y() ), qRound( position.z() ) );

    //! "Test the far face of current Cube."
    //! _________________________________________________________________________________________________
    //! If the Player velocity is towards the far face.
    if( velocity.z() < 0 )
    {
        //! *Define a point on the face and the face normal.
        QVector3D facePoint  = currentCube;
        QVector3D faceNormal = QVector3D( 0.0f, 0.0f, 1.0f );

        //! *Calculate the distance to the plane of the face.
        float distance = distanceOnRayToPlane( position, velocity, facePoint, faceNormal );

        //! *if the distance is within the distance to travel there could be a collision
        if( distance <= velocity.length() )
        {
            //! **If the distance is less then the currently tracked collision distance.
            if( distance < mCollisionData.distance )
            {
                //! ***Calculate the distance delta to travel to collision point.
                QVector3D delta = velocity * (distance / velocity.length());
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = position + delta + QVector3D( 0.0f, 0.0f, -mSphereRadius );
                //! ***Determine which Cube could have been hit.
                QVector3D cube = QVector3D( qRound( collisionPosition.x() ), qRound( collisionPosition.y() ), qFloor( collisionPosition.z() ) );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = faceNormal;
                    mCollisionData.cubePosition = cube;
                } // if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
            } // if( distance < mCollisionData.distance )
        } // if( distance <= velocity.length() )
    } // if( velocity.z() < 0 )

    //! "Test the near face of current Cube."
    //! _________________________________________________________________________________________________
    //! If the Player velocity is towards the near face.
    if( velocity.z() > 0 )
    {
        //! *Define a point on the face and the face normal.
        QVector3D facePoint  = currentCube;
        QVector3D faceNormal = QVector3D( 0.0f, 0.0f, -1.0f );

        //! *Calculate the distance to the plane of the face.
        float distance = distanceOnRayToPlane( position, velocity, facePoint, faceNormal );

        //! *if the distance is within the distance to travel there could be a collision
        if( distance <= velocity.length() )
        {
            //! **If the distance is less then the currently tracked collision distance.
            if( distance < mCollisionData.distance )
            {
                //! ***Calculate the distance delta to travel to collision point.
                QVector3D delta = velocity * (distance / velocity.length());
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = position + delta + QVector3D( 0.0f, 0.0f, mSphereRadius );
                //! ***Determine which Cube could have been hit.
                QVector3D cube = QVector3D( qRound( collisionPosition.x() ), qRound( collisionPosition.y() ), qCeil( collisionPosition.z() ) );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = faceNormal;
                    mCollisionData.cubePosition = cube;
                } // if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
            } // if( distance < mCollisionData.distance )
        } // if( distance <= velocity.length() )
    } // if( velocity.z() > 0 )

    //! "Test the left face of current Cube."
    //! _________________________________________________________________________________________________
    //! If the Player velocity is towards the left face.
    if( velocity.x() < 0 )
    {
        //! *Define a point on the face and the face normal.
        QVector3D facePoint  = currentCube;
        QVector3D faceNormal = QVector3D( 1.0f, 0.0f, 0.0f );

        //! *Calculate the distance to the plane of the face.
        float distance = distanceOnRayToPlane( position, velocity, facePoint, faceNormal );

        //! *if the distance is within the distance to travel there could be a collision
        if( distance <= velocity.length() )
        {
            //! **If the distance is less then the currently tracked collision distance.
            if( distance < mCollisionData.distance )
            {
                //! ***Calculate the distance delta to travel to collision point.
                QVector3D delta = velocity * (distance / velocity.length());
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = position + delta + QVector3D( -mSphereRadius, 0.0f, 0.0f );
                //! ***Determine which Cube could have been hit.
                QVector3D cube = QVector3D( qFloor( collisionPosition.x() ), qRound( collisionPosition.y() ), qRound( collisionPosition.z() ) );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = faceNormal;
                    mCollisionData.cubePosition = cube;
                } // if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
            } // if( distance < mCollisionData.distance )
        } // if( distance <= velocity.length() )
    } // if( velocity.x() < 0 )

    //! "Test the right face of current Cube."
    //! _________________________________________________________________________________________________
    //! If the Player velocity is towards the right face.
    if( velocity.x() > 0 )
    {
        //! *Define a point on the face and the face normal.
        QVector3D facePoint  = currentCube;
        QVector3D faceNormal = QVector3D( -1.0f, 0.0f, 0.0f );

        //! *Calculate the distance to the plane of the face.
        float distance = distanceOnRayToPlane( position, velocity, facePoint, faceNormal );

        //! *if the distance is within the distance to travel there could be a collision
        if( distance <= velocity.length() )
        {
            //! **If the distance is less then the currently tracked collision distance.
            if( distance < mCollisionData.distance )
            {
                //! ***Calculate the distance delta to travel to collision point.
                QVector3D delta = velocity * (distance / velocity.length());
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = position + delta + QVector3D( mSphereRadius, 0.0f, 0.0f );
                //! ***Determine which Cube could have been hit.
                QVector3D cube = QVector3D( qCeil( collisionPosition.x() ), qRound( collisionPosition.y() ), qRound( collisionPosition.z() ) );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = faceNormal;
                    mCollisionData.cubePosition = cube;
                } // if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
            } // if( distance < mCollisionData.distance )
        } // if( distance <= velocity.length() )
    } // if( velocity.x() > 0 )

    //! "Test the bottom face of current Cube."
    //! _________________________________________________________________________________________________
    //! If the Player velocity is towards the bottom face.
    if( velocity.y() < 0 )
    {
        //! *Define a point on the face and the face normal.
        QVector3D facePoint  = currentCube;
        QVector3D faceNormal = QVector3D( 0.0f, 1.0f, 0.0f );

        //! *Calculate the distance to the plane of the face.
        float distance = distanceOnRayToPlane( position, velocity, facePoint, faceNormal );

        //! *if the distance is within the distance to travel there could be a collision
        if( distance <= velocity.length() )
        {
            //! **If the distance is less then the currently tracked collision distance.
            if( distance < mCollisionData.distance )
            {
                //! ***Calculate the distance delta to travel to collision point.
                QVector3D delta = velocity * (distance / velocity.length());
                //! ***Calculate the position of the collision.
                QVector3D collisionPosition = position + delta + QVector3D( 0.0f, -mSphereRadius, 0.0f );
                //! ***Determine which Cube could have been hit.
                QVector3D cube = QVector3D( qRound( collisionPosition.x() ), qFloor( collisionPosition.y() ), qRound( collisionPosition.z() ) );

                //! ***If there is a Cube at that height then there is a collision.
                //! ***Keep track of the new closest collision.
                if( true == mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) )
                {
                    mCollisionData.distance = distance;
                    mCollisionData.normal = faceNormal;
                    mCollisionData.cubePosition = cube;
                } // if( mLevel.cubeAtPosition( cube.x(), cube.y(), cube.z() ) == true )
            } // if( distance < mCollisionData.distance )
        } // if( distance <= velocity.length() )
    } // if( velocity.y() < 0 )
}

//! Respond to collisions between the Player Level.
//! \param position is the current internal position of the player for time interval.
//! \param velocity is the current internal velocity of the player for time interval.
//! \return void
//! \todo play sound effect based on collision angle and velocity.
void Physics::collisionResponse(QVector3D & position, QVector3D &velocity )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Physics::collisionResponse(QVector3D & position, QVector3D &velocity )...";

    //! If the current collision data distance equals the maximum possible value.
    if( mCollisionData.distance == FLT_MAX )
    {
        //! *Then no collision occurred and the Player position and rotation can move forward the entire distance,
        position += velocity;
        roll( velocity );
        velocity = QVector3D(0.0f,0.0f,0.0f);
    } // if( mCollisionData.distance == FLT_MAX )
    //! Else a collision occurred.
    else
    {
        //! *Clean the normal to get a good reflection.
        mCollisionData.normal = removeFuzz( mCollisionData.normal );

        //! *Calculate the angle of impact.
        // float dotProduct = QVector3D::dotProduct( velocity.normalized(), mCollisionData.normal );
        //! *If the angle of impact is acute it was a bounce
        //! *If the angle of impact is obtuse it was a skid

        //! *Calculate the reflection
        QVector3D reflection = reflectionVector(velocity, mCollisionData.normal);

        //! *Roll and position the Player to the point of collision.
        double percentage = (mCollisionData.distance / velocity.length());
        QVector3D move = velocity * percentage;
        velocity -= move;
        position += move;
        position = removeFuzz( position );
        roll( move );

        //! *Reflect the Player velocity.
        velocity = reflection * velocity.length();
        mPlayer.velocity() = reflection * mPlayer.velocity().length();

        //! *Apply damping to the Player velocity caused by the collision.
        velocity = velocity.normalized() * velocity.length() * 0.7f;
        mPlayer.velocity() = mPlayer.velocity().normalized() * mPlayer.velocity().length() * 0.7f;
    } // if( mCollisionData.distance == FLT_MAX ) else

    //! Calculate the Player current horizontal heading.
    QVector3D horizontalHeading = QVector3D( mPlayer.velocity().x(), 0.0f, mPlayer.velocity().z() );
    //! If the horizontal heading magnitude is greater then the MINIMUM_VELOCITY.
    if( horizontalHeading.length() > MINIMUM_VELOCITY )
    {
        //! Track that as the current direction to apply user force.
        mPlayer.direction() = horizontalHeading.normalized();
    } // if( horizontalHeading > MINIMUM_VELOCITY )
} // Physics::collisionResponse(QVector3D & position, QVector3D &velocity )

//! Calculate the distance along a ray to its intersection with a plane.
//! \param rayStart is the point where the ray starts.
//! \param rayDirection is the direction of the ray.
//! \param planePoint is a point on the plane.
//! \param planeNormal is the normal to the plane.
//! \return float is the distance to the ray plane intersection.
//! \sa Physics
float Physics::distanceOnRayToPlane( QVector3D & rayStart, QVector3D & rayDirection, QVector3D & planePoint, QVector3D & planeNormal )
{
    //! Calculate the dot product between the ray direction and plane normal.
    float dotProduct = QVector3D::dotProduct(rayDirection.normalized(), planeNormal);

    //! If the ray is parallel to the plane.
    if( ( dotProduct <  FLT_EPSILON ) && ( dotProduct > -FLT_EPSILON ) )
    {
        //! *Return maximum float value, no intersection will occur.
        return FLT_MAX;
    } // if( ( dotProduct <  FLT_EPSILON ) && ( dotProduct > -FLT_EPSILON ) )

    //! Calculate the distance to the intersection.
    float distance = QVector3D::dotProduct( ( planePoint - rayStart ), planeNormal ) / dotProduct;

    //! If collision occurred behind ray start.
    if( distance < -FLT_EPSILON )
    {
        //! *Return maximum float value, no intersection will occur.
        return FLT_MAX;
    } // if( distance < -FLT_EPSILON )

    //! Return the distance.
    return distance;
} // Physics::distanceOnRayToPlane( QVector3D & rayStart, QVector3D & rayDirection, QVector3D & planePoint, QVector3D & planeNormal )

//! Calculate the forces.
//! \return void
//! \sa Physics, Player, Level
void Physics::forces()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Physics::force()...";

    //! Calculate the sum of accelerations
    QVector3D acceleration;

    //! Calculate how much force the user has applied to the mass.
    acceleration += mPlayer.force().normalized() * ( mPlayer.force().length() / mPlayer.mass() );
    //! Reset applied user force to zero until next tick.
    mPlayer.force() = QVector3D(0,0,0);

    //! If the Player is on a Cube.
    if( mLevel.playerOnACube( mPlayer.position() ) == true )
    {
        //! *Apply rolling resistance to the Players movement.
        acceleration += mPlayer.velocity().normalized() * mPlayer.rollingResistanceCoefficient() * mPlayer.mass() * -GRAVITY;
    } // if( mLevel.playerOnACube( mPlayer.position() ) == true )
    //! Else the Player is NOT on a Cube.
    else
    {
        //! *Apply gravity.
        acceleration += QVector3D(0.0f,-GRAVITY,0.0f);
    } // if( mLevel.playerOnACube( mPlayer.position() ) == true ) else

    //! Apply the sum of accelerations to the velocity for the time step.
    mPlayer.velocity() += acceleration * mTimeInterval;

    //! If the Player velocity is now greater then the defined terminal velocity.
    if( mPlayer.velocity().length() > mPlayer.terminalVelocity() )
    {
        //! *Cap the velocity to the terminal velocity.
        mPlayer.velocity() = mPlayer.velocity().normalized() * mPlayer.terminalVelocity();
    } // if( mPlayer.velocity().length() > mPlayer.terminalVelocity() )

    //! If the Player velocity in the X direction is less then the minimum defined threshold.
    if( qAbs( mPlayer.velocity().x() ) < MINIMUM_VELOCITY )
    {
        //! *Set the Player liner velocity in the X direction to zero to prevent oscillations caused by limited precision.
        mPlayer.velocity().setX( 0.0f );
    } // if( qAbs( mPlayer.velocity().x() ) < MINIMUM_VELOCITY )

    //! If the Player velocity in the Y direction is less then the minimum defined threshold.
    if( qAbs( mPlayer.velocity().y() ) < MINIMUM_VELOCITY )
    {
        //! *Set the Player liner velocity in the Y direction to zero to prevent oscillations caused by limited precision.
        mPlayer.velocity().setY( 0.0f );
        //! *Snap to Cube surface to prevent gravity from being applied and causing oscillations caused by limited precision.
        mPlayer.position().setY( qRound( mPlayer.position().y() ) );
    } // if( qAbs( mPlayer.velocity().y() ) < MINIMUM_VELOCITY )

    //! If the Player velocity in the Z direction is less then the minimum defined threshold.
    if( qAbs( mPlayer.velocity().z() ) < MINIMUM_VELOCITY )
    {
        //! *Set the Player liner velocity in the Z direction to zero to prevent oscillations caused by limited precision.
        mPlayer.velocity().setZ( 0.0f );
    } // if( qAbs( mPlayer.velocity().z() ) < MINIMUM_VELOCITY )
} // Physics::forces()

//! Move the player to the next position in time.
//! \return void
//! \sa Physics, Player, Level
void Physics::movement()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Physics::movement()...";

    //! Create a internal position to track were to move the player over the time interval.
    QVector3D intervalPosition = mPlayer.position();
    //! Create a internal distance to track how far to move the player over the time interval.
    QVector3D intervalDistance = mPlayer.velocity() * mTimeInterval;

    //! Move the player over the distance.
    while( intervalDistance.length() > 0 )
    {
        //! *Check for collisions with the Level over the distance to travel for the time interval.
        collisionDetection(intervalPosition, intervalDistance);
        //! *Move the Player to first point of collision or to end of distance to travel for time interval.
        collisionResponse(intervalPosition, intervalDistance);
    } //! Repeat until entire distance has been traveled.

    //! Update the Players position based on the end result of the internal tracking position.
    mPlayer.position() = intervalPosition;
} // Physics::movement()

//! Calculate the reflection of a given vector around a given normal.
//! \param input is the vector to reflect.
//! \param normal is to reflect around.
//! \return QVector3D is the reflected vector.
//! \sa Physics
QVector3D Physics::reflectionVector( QVector3D & input, QVector3D & normal )
{
    //! Create an vector to return.
    QVector3D output;

    //! Calculate the reflection.
    output = (2 * QVector3D::dotProduct( (-1*input.normalized()), normal ) * normal + input.normalized() );

    //! Return the reflected vector.
    return output;
} // Physics::reflectionVector(QVector3D input, QVector3D normal)

//! Clean floating point value to remove accumulated precision error noise.
//! \param input is float to clean.
//! \return float is the input cleaned up..
//! \sa Physics
float Physics::removeFuzz( const float input )
{
    //! Create a float to return;
    float output;

    //! Clean all digits pass four decimal places.
    output = (float)(qRound( input * 10000.0f ) / 10000.0f);

    //! Return clean floating point value.
    return output;
} // Physics::removeFuzz(const float input)

//! Clean vector of floating point value to remove accumulated precision error noise.
//! \param input is vector of floats to clean.
//! \return QVector3D is the input cleaned up..
//! \sa Physics
QVector3D Physics::removeFuzz( const QVector3D& input )
{
    //! Create a vector to return;
    QVector3D output;

    //! Remove the fuzz on x,y,z floats.
    output.setX( removeFuzz( input.x() ) );
    output.setY( removeFuzz( input.y() ) );
    output.setZ( removeFuzz( input.z() ) );

    //! Return clean vector value.
    return output;
} // Physics::removeFuzz( const QVector3D& input )

//! Roll the player.
//! \param distance is the how far the player will roll.
//! \return void
//! \sa Physics, Player, Sphere
void Physics::roll( QVector3D & distance )
{
    //! Remove vertical component from distance only horizontal component impacts rolling.
    QVector3D rollDistance = QVector3D( distance.x(), 0, distance.z() );

    //! Calculate the axis of rotation.
    QVector3D axis = QVector3D::crossProduct( QVector3D( 0.0f, 1.0f, 0.0f ), rollDistance );

    //! Calculate the angle of rotation in degrees.
    float angle = rollDistance.length() / M_PI * 360;

    //! Roll the player.
    mPlayer.rotation() = QQuaternion::fromAxisAndAngle( axis, angle ) * mPlayer.rotation();
} // Physics::roll( QVector3D & distance )

//! Update the Physics.
//! \return void
//! \sa Physics, Player, Level
void Physics::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Physics::tick()...";

    mHalfCubeSize = mLevel.cubeSize() / 2.0f;
    mSphereRadius = mPlayer.sphereRadius();

    //! Calculate forces.
    forces();

    //! Apply forces..
    movement();
} // Physics::tick()
