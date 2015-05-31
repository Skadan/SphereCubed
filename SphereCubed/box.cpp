/*! \file box.cpp
\brief Box class implementation file.
This file contains the Box class implementation for the game application.
*/

// box.h header file required for Box class definition.
#include "box.h"

// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION false

//! The Box execution begins here.
//! Constructor creates a box either undefined or a 1x1x1 unit Box
//! centered at the origin depending on the parameter.
//! \param unitBox when true creates a 1x1x1 unit box centered at the origin.
Box::Box( bool unitBox )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Box::Box()...";

    //! If the unitBox is set to true.
    if( unitBox == true )
    {
        //! Create corners for a 1x1x1 unit box centered at the origin.
        mCorners[ FAR_TOP_LEFT ]      = QVector3D( -0.5f,  0.5f, -0.5f );
        mCorners[ FAR_TOP_RIGHT ]     = QVector3D(  0.5f,  0.5f, -0.5f );
        mCorners[ NEAR_TOP_RIGHT ]    = QVector3D(  0.5f,  0.5f,  0.5f );
        mCorners[ NEAR_TOP_LEFT ]     = QVector3D( -0.5f,  0.5f,  0.5f );
        mCorners[ FAR_BOTTOM_LEFT ]   = QVector3D( -0.5f, -0.5f, -0.5f );
        mCorners[ FAR_BOTTOM_RIGHT ]  = QVector3D(  0.5f, -0.5f, -0.5f );
        mCorners[ NEAR_BOTTOM_RIGHT ] = QVector3D(  0.5f, -0.5f,  0.5f );
        mCorners[ NEAR_BOTTOM_LEFT ]  = QVector3D( -0.5f, -0.5f,  0.5f );
    } // if( unitBox == true )
} // Box::Box()

//! The Box execution ends here.
//! Required to free the Box allocated memory.
Box::~Box()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Box::~Box()...";
} // Box::~Box()

//! Add an offset to the Box position and return a translated box.
//! \param offset is a vector to translate the box to.
//! \return Box is a Box translated by the given offset.
//! \sa Box
Box Box::operator +( const QVector3D & offset )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Box::operator +(const QVector3D & offset )...";

    //! Create new Box to return.
    Box box;

    //! Set the corners of the new box equal to the Box corners translated.
    box.mCorners[ FAR_TOP_LEFT ]      = this->mCorners[ FAR_TOP_LEFT ]      + offset;
    box.mCorners[ FAR_TOP_RIGHT ]     = this->mCorners[ FAR_TOP_RIGHT ]     + offset;
    box.mCorners[ NEAR_TOP_RIGHT ]    = this->mCorners[ NEAR_TOP_RIGHT ]    + offset;
    box.mCorners[ NEAR_TOP_LEFT ]     = this->mCorners[ NEAR_TOP_LEFT ]     + offset;
    box.mCorners[ FAR_BOTTOM_LEFT ]   = this->mCorners[ FAR_BOTTOM_LEFT ]   + offset;
    box.mCorners[ FAR_BOTTOM_RIGHT ]  = this->mCorners[ FAR_BOTTOM_RIGHT ]  + offset;
    box.mCorners[ NEAR_BOTTOM_RIGHT ] = this->mCorners[ NEAR_BOTTOM_RIGHT ] + offset;
    box.mCorners[ NEAR_BOTTOM_LEFT ]  = this->mCorners[ NEAR_BOTTOM_LEFT ]  + offset;

    //! Return the new translated Box.
    return box;
} // Box::operator +( const QVector3D & offset )
