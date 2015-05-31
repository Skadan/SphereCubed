/*! \file plane.h
\brief Plane class declaration file.
This file contains the Plane class declaration for the application.
*/

#ifndef PLANE_H
#define PLANE_H

// QVector3D header file required for positioning.
#include <QVector3D>

//! Plane class.
/*!
This class declaration encapsulates the Plane functionality.
*/
class Plane
{
    //! Friend of Frustum so that only Frustum can instantiate and use a Plane.
    friend class Frustum;

private: // Constructors / Destructors
    //! Plane class constructor.
    Plane();

    //! Plane class constructor.
    Plane( const QVector3D & p1, const QVector3D & p2, const QVector3D & p3 );

    //! Plane class destructor.
    ~Plane();

private: // Member Function
    //! Calculate the distance of the point from the Plane.
    //! \param point is the position to calculate the distance to.
    //! \return float is the point's distance from the Plane.
    //! \sa Plane, Frustum, Box
    float distance( const QVector3D & point ) const
    { return mDelta + ( mNormal.x() * point.x() + mNormal.y() * point.y() + mNormal.z() * point.z() ); }

private: // Member Variables
    //! Used store the normal to the Plane.
    QVector3D mNormal;

    //! Used to store the partial delta distance to an arbitrary point.
    float     mDelta;
}; // class Plane

#endif // PLANE_H
