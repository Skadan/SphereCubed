/*! \file box.h
\brief Box class declaration file.
This file contains the Box class declaration for the application.
Used to define bounding box of objects.
*/

#ifndef BOX_H
#define BOX_H

// QVector3D header file required for vertices.
#include <QVector3D>

//! Box class.
/*!
This class declaration encapsulates the Box functionality.
*/
class Box
{
public: // Defined Types

    //! Enumeration of the eight corners of the bounding Box.
    enum Corners
    {
        //! 0
        FAR_TOP_LEFT = 0,
        //! 1
        FAR_TOP_RIGHT,
        //! 2
        NEAR_TOP_RIGHT,
        //! 3
        NEAR_TOP_LEFT,
        //! 4
        FAR_BOTTOM_LEFT,
        //! 5
        FAR_BOTTOM_RIGHT,
        //! 6
        NEAR_BOTTOM_RIGHT,
        //! 7
        NEAR_BOTTOM_LEFT,
        //! 8
        COUNT
    };

public: // Constructors / Destructors
    //! Box class constructor.
    Box( bool unitBox = false );

    //! Boxclass destructor.
    ~Box();

public: // Member Functions
    //! Return the position of the specified corner.
    //! \param corner is an enum of the corner position to return.
    //! \return QVector3D is the position of the corner.
    //! \sa Box
    const QVector3D & corner( Corners corner ) const { return mCorners[ corner ]; }

public: // Operators
    //! Return a translated Box.
    Box operator +( const QVector3D & offset );

private: // Member Variables
    //! A private variable used store the array of conrers of the Box.
    QVector3D mCorners[ Corners::COUNT ];
}; // class Box

#endif // BOX_H
