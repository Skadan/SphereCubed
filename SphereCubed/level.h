/*! \file level.h
\brief Level class declaration file.
This file contains the Level class declaratio for the application.
*/

#ifndef LEVEL_H
#define LEVEL_H

// camera.h header file required for rendering.
#include "camera.h"
// cube.h header file required for class definition.
#include "cube.h"
// light.h header file required for rendering.
#include "light.h"

//! Level class.
/*!
This class declaration encapsulates the Level functionality.
*/
class Level
{
    //! Friend of World so that only World can instantiate the Level.
    friend class World;

public: // Access Functions
    //! Called to get the Cube size.
    float cubeSize() { return mCubeSize; }

    //! Called to get the Cube type at a location in the Level.
    Cube::CubeType cubeType( uint column, uint row );

    //! Called to get the start position for the Level.
    QVector3D & startPosition() { return mStartPosition; }

public: // Query Functions
    //! Called to find out if there is a physical Cube at the position specified.
    bool cubeAtPosition( int x, int y, int z );

    //! Called to find out if the position is with in a Cube of the Level.
    bool insideLevel( const QVector3D & position );

    //! Called to find out if the Player Position is directly on top of a physical Cube.
    bool playerOnACube( const QVector3D & position );

    //! Called to find out if the Player is on a plane.
    bool playerOnPlane( const QVector3D& position );

public: // Member Functions
    //! Called to load the Level OpenGL resources.
    void load();

    //! Called to render the Level OpenGL resources.
    void render( const Camera & camera, const Light & light );

    //! Called to update the Level.
    void tick();

    //! Called to unload the Level OpenGL resources.
    void unload();

private: // Constructors / Destructors
    //! Level class constructor.
    Level();

    //! Level class destructor.
    ~Level();

private: // Member Variables
    //! Used to store the number of Cube in Level.
    uint   mCubeCount = 0;

    //! Used to store the number of Cube rows in Level.
    uint   mCubeRows = 0;

    //! Used to store the number of Cube columns in Level.
    uint   mCubeCols = 0;

    //! Used to store the Cubes of the Level.
    Cube * mpCube = NULL;

    //! Used to store the Cube size.
    const float mCubeSize = 1.0f;

    //! Used to store the current Level index number.
    uint mLevelIndex = 0;

    //! Used to store the start location of the Level.
    QVector3D mStartPosition;
}; // class Level

#endif // LEVEL_H
