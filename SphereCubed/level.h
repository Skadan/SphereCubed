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

public: // Member Functions
    //! Called to load the Level OpenGL resources.
    void load();

    //! Called to render the Level OpenGL resources.
    void render( const Camera & camera, const Light & light );

    //! Called to update the Level.
    void tick();

    //! Called to unload the Level OpenGL resources.
    void unload();

    //! Called to get the Cube type at a location in the Level.
    Cube::CubeType cubeType( uint column, uint row );

private: // Constructors / Destructors
    //! Level class constructor.
    Level();

    //! Level class destructor.
    ~Level();

private: // Member Variables
    //! Used to store the number of Cube in Level.
    uint   mCubeCount;

    //! Used to store the number of Cube rows in Level.
    uint   mCubeRows;

    //! Used to store the number of Cube columns in Level.
    uint   mCubeCols;

    //! Used to store the Cubes of the Level.
    Cube * mpCube;

    //! Used to store the current Level index number.
    uint mLevelIndex;

    //! Used to store the start location of the Level.
    QVector3D mStartPosition;
}; // class Level

#endif // LEVEL_H
