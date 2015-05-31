/*! \file frustum.cpp
\brief Frustum class implementation file.
This file contains the Frustum class implementation for the game application.
*/

// frustum.h header file required for Frustum class definition.
#include "frustum.h"

// trace.h header file required for tracing execution.
#include "trace.h"

// QtMath header file required for Trig and Degrees to Radians.
#include <QtMath>

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION false

//! The Frustum execution begins here.
//! Constructor creates a undefined Frustum.
//! Automatically called by the Camera during construction.
Frustum::Frustum()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Frustum::Frustum()...";
} // Frustum::Frustum()

//! The Frustum execution ends here.
//! Required to free the Frustum allocated memory.
Frustum::~Frustum()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Frustum::~Frustum()...";
} // Frustum::~Frustum()

//! Calculate the HALFSPACE the Box is in.
//! Called to determine if objects need to be rendered.
//! \param box is a bounding Box to test if it is inside of the Frustum.
//! \return HALFSPACE is the side of the frustum the box is on.
//! \sa Plane, Frustum, Box
Frustum::HALFSPACE Frustum::boxViewable( const Box & box ) const
{
    //! Create a counter to track the number of corners inside the Frustum.
    int inside;

    //! Create a counter to track the number of corners outside the Frustum.
    int outside;

    //! For each Plane of the Frustum.
    for( int i = 0; i < Planes::COUNT; i++ )
    {
        //! -Reset the inside counter to zero.
        inside = 0;
        //! -Reset the outside counter to zero.
        outside = 0;

        //! -For each corner of the box.
        for ( int j = 0; j < Box::Corners::COUNT; j++ )
        {
            //! --Determine if the corner is outside the Frustum.
            if( mPlane[i].distance( box.corner((Box::Corners)j) ) < 0)
            {
                //! --If outside increment the outside counter.
                outside++;
            } // if( mPlane[i].distance( box.corner((Box::Corners)j) ) < 0)
            //! --Else the corner is inside the Frustum.
            else
            {
                //! --Increment the inside counter.
                inside++;
            } // if( mPlane[i].distance( box.corner((Box::Corners)j) ) < 0) else

            //! --If there are corners inside and outside.
            if( ( inside != 0 ) && ( outside != 0 ) )
            {
                //! --Exit loop, the box is on the plane of the Frustum.
                break;
            } // if( ( inside != 0 ) && ( outside != 0 ) )
        } // for (int j = 0; j < Box::Corners::COUNT; j++)

        //! -if all corners are all outside the Frustum.
        if ( inside == 0 )
        {
            //! -Return OUT.
            return OUT;
        }  // if ( inside == 0 )
        //! -Else if some corners are outside and some are inside the Frustum.
        else if ( outside > 0 )
        {
            //! -Return ON.
            return ON;
        } // else if ( outside > 0 )
    } // for( int i = 0; i < Planes::COUNT; i++ )

    //! All corners where inside, so return IN.
    return IN;
} // Frustum::boxViewable( const Box & box ) const

//! Called when the OpenGL context is resized.
//! Used to setup the near and far planes distance, height, and width.
//! \param fieldOfView is the angle of the feild of view in degree.
//! \param ratio is the ratio of the OpenGL context height and width.
//! \param near is the unitless distance to the near Plane of the Frustum.
//! \param far is the unitless distance to the far Plane of the Frustum.
//! \return void.
//! \sa Plane, Frustum, Box
void Frustum::projection( float fieldOfView, float ratio, float near, float far )
{
    //! Calculate the tangent to half the field of view.
    float tangent = qTan( qDegreesToRadians( fieldOfView * 0.5 ) );

    //! Store the distance to the Frustum near and far Plane.
    mNearDistance = near;
    mFarDistance  = far;

    //! Calculate and store the height and width of the Frustum near and far Plane.
    mNearHeight = near * tangent;
    mNearWidth  = mNearHeight * ratio;
    mFarHeight  = far * tangent;
    mFarWidth   = mFarHeight * ratio;
} // Frustum::projection( float fieldOfView, float ratio, float near, float far )

//! Called to account for repositioning of the Camera.
//! Used to calculate the array of six Plane of the Frustum given the information collected first in the call to projection.
//! \param eye is the Camera position.
//! \param focus is the point the Camera is aimed towards.
//! \param up is the up direction of the Camera.
//! \return void.
//! \sa Plane, Frustum, Box
void Frustum::view( const QVector3D & eye, const QVector3D & focus, const QVector3D & up )
{
    //! Calculate the Z axis of camera opposite of the looking direction.
    QVector3D z = eye - focus;
    z.normalize();

    //! Calculate the X axis of camera with given "up" vector and Z axis.
    QVector3D x = QVector3D::crossProduct( up, z );
    x.normalize();

    //! Calculate the Y axis of the camera.
    QVector3D y = QVector3D::crossProduct( z, x );

    //! Calculate the centers of the near and far planes.
    QVector3D nearPlaneCenter = eye - z * mNearDistance;
    QVector3D farPlaneCenter  = eye - z * mFarDistance;

    //! Calculate 4 corners of the frustum on the near plane.
    QVector3D nearTopLeft     = nearPlaneCenter + y * mNearHeight - x * mNearWidth;
    QVector3D nearTopRight    = nearPlaneCenter + y * mNearHeight + x * mNearWidth;
    QVector3D nearBottomLeft  = nearPlaneCenter - y * mNearHeight - x * mNearWidth;
    QVector3D nearBottomRight = nearPlaneCenter - y * mNearHeight + x * mNearWidth;

    //! Calculate 4 corners of the frustum on the far plane.
    QVector3D farTopLeft     = farPlaneCenter + y * mFarHeight - x * mFarWidth;
    QVector3D farTopRight    = farPlaneCenter + y * mFarHeight + x * mFarWidth;
    QVector3D farBottomLeft  = farPlaneCenter - y * mFarHeight - x * mFarWidth;
    QVector3D farBottomRight = farPlaneCenter - y * mFarHeight + x * mFarWidth;

    //! Calculate the six planes.
    mPlane[TOP]     = Plane( farTopLeft,        nearTopLeft,       nearTopRight );
    mPlane[BOTTOM]  = Plane( farBottomRight,    nearBottomRight,   nearBottomLeft );
    mPlane[LEFT]    = Plane( farBottomLeft,     nearBottomLeft,    nearTopLeft );
    mPlane[RIGHT]   = Plane( farBottomRight,    nearTopRight,      nearBottomRight );
    mPlane[NEAR]    = Plane( nearBottomRight,   nearTopRight,      nearTopLeft );
    mPlane[FAR]     = Plane( farBottomLeft,     farTopLeft,        farTopRight );
} // Frustum::view( const QVector3D & eye, const QVector3D & focus, const QVector3D & up )
