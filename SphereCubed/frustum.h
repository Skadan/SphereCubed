/*! \file frustum.h
\brief Frustum class declaration file.
This file contains the Frustum class declaration for the application.
The Frustum is used to determine if an object is with in the view and requires rendering.
*/

#ifndef FRUSTUM_H
#define FRUSTUM_H

// QVector3D header file required for positioning.
#include <QVector3D>

// box.h header file required for box.
#include "box.h"
// plane.h header file required for bounding Frustum.
#include "plane.h"

//! Frustum class.
/*!
This class declaration encapsulates the Frustum functionality.
*/
class Frustum
{
    //! Friend of Camera so that only Camera can instantiate and use a Frustum.
    friend class Camera;

private: // Defined Types

    //! Enumeration of the array of six Plane of the bounding Frustum.
    enum Planes
    {
        //! 0
        TOP = 0,
        //! 1
        BOTTOM,
        //! 2
        LEFT,
        //! 3
        RIGHT,
        //! 4
        NEAR,
        //! 5
        FAR,
        //! 6
        COUNT
    }; // enum Planes

    //! Enumeration of the three possible positions of the Box.
    enum HALFSPACE
    {
        //! Outside the Frustum.
        OUT,
        //! On a Plane of the Frustum.
        ON,
        //! Inside the Frustum.
        IN
    }; // enum HALFSPACE

private: // Constructors / Destructors
    //! Frustum class constructor.
    Frustum();

    //! Frustum class destructor.
    ~Frustum();

private: // Member Functions
    //! Determine if Box is with in the view of Camera.
    Frustum::HALFSPACE boxViewable( const Box & box ) const;

    //! Updated the Frustum values based on the new projection.
    void projection( float fieldOfView, float ratio, float near, float far );

    //! Updated the Frustum values based on the new view.
    void view( const QVector3D & eye, const QVector3D & focus, const QVector3D & up );

private: // Member Variables
    //! Used store the array of six Plane of the Frustum.
    Plane mPlane[ Planes::COUNT ];

    //! Used store the distance to the far Plane of the Frustum.
    float mFarDistance;

    //! Used store the height of the far Plane of the Frustum.
    float mFarHeight;

    //! Used store the width of the far Plane of the Frustum.
    float mFarWidth;

    //! Used store the distance to the near Plane of the Frustum.
    float mNearDistance;

    //! Used store the height of the near Plane of the Frustum.
    float mNearHeight;

    //! Used store the width of the near Plane of the Frustum.
    float mNearWidth;
}; // class Frustum

#endif // FRUSTUM_H
