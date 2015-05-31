/*! \file plane.cpp
\brief Plane class implementation file.
This file contains the Plane class implementation for the application.
*/

// plane.h header file required for Plane class definition.
#include "plane.h"

// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION false


//! The Plane execution begins here.
//! Automatically called by the Frustum during construction.
//! Default constructor creates a undefined Plane.
Plane::Plane()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Plane::Plane()...";
} // Plane::Plane()

//! The Plane execution begins here.
//! Defines a Plane based on the three points given in ccw order..
//! \param p1 is a point on the Plane.
//! \param p2 is a point on the Plane.
//! \param p3 is a point on the Plane.
Plane::Plane( const QVector3D & p1, const QVector3D & p2, const QVector3D & p3 )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Plane::Plane( const QVector3D & p1, const QVector3D & p2, const QVector3D & p3 )...";

    //! Calculate the normal to the Plane
    mNormal = QVector3D::crossProduct( (p1 - p2), (p3 - p2) ).normalized();

    //! Calculate a partial delta used later for calculating the distance to an arbitrary point.
    mDelta = -( mNormal.x() * p2.x() + mNormal.y() * p2.y() + mNormal.z() * p2.z() );
} // Plane::Plane( const QVector3D & p1, const QVector3D & p2, const QVector3D & p3 )

//! The Plane execution ends here.
//! Required to free the Plane allocated memory.
Plane::~Plane()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Plane::~Plane()...";
} // Plane::~Plane()
