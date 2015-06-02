/*! \file cube.h
\brief Cube class declaration file.
This file contains the Cube class declaration for the application.
*/

#ifndef CUBE_H
#define CUBE_H

// QGLBuffer header file required for OpenGL buffer.
#include <QGLBuffer>
// QOpenGLShaderProgram header file required for OpenGL shader program.
#include <QOpenGLShaderProgram>
// QOpenGLTexture header file required for OpenGL textures.
#include <QOpenGLTexture>
// QVector3D header file required for vertices.
#include <QVector3D>

// box.h header file required for bounding Box class definition.
#include "box.h"
// camera.h header file required for rendering.
#include "camera.h"
// light.h header file required for rendering.
#include "light.h"

//! Cube class.
/*!
This class declaration encapsulates the Cube functionality.
*/
class Cube
{
    //! Friend of Level so that only Level can instantiate the Cube.
    friend class Level;

public: // Defined Types
    //! Enumeration of the array of 256 possible Cube types.
    enum CubeType
    {
        //! 0x00
        HOLE        = 0x00,
        //! 0x01
        START       = 0x01,
        //! 0x02
        STANDARD    = 0x02,

        //! 0xFF
        FINISH      = 0xFF,
        //! 256
        CUBE_TYPE_COUNT
    }; // enum CubeType

    //! Enumeration of the array of six faces of the Cube.
    enum CubeFace
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
        CUBE_FACE_COUNT
    }; // enum CubeFace

private: // Constructors / Destructors
    //! Cube class constructor.
    Cube();

    //! Cube class destructor.
    ~Cube();

private: // Member Functions
    //! Called to load the Cube OpenGL resources.
    static void load();

    //! Called to render the Cube OpenGL resources.
    void render( const Camera & camera, const Light & light );

    //! Called to update the Cube.
    void tick();

    //! Called to unload the Cube OpenGL resources.
    static void unload();

private: // Helper Functions
    //! Called to load the Cube OpenGL buffers.
    static bool loadBuffers( CubeFace cubeFace );

    //! Called to load the Cube OpenGL shaders.
    static bool loadShaders();

    //! Called to load the Cube OpenGL textures.
    static bool loadTexture( CubeType cubeType, QString texture );

    //! Called to load the Cube face.
    void renderFace( CubeFace cubeFace );

private: // Member Variables
    //! Used to store the bounding Box for the Cube.
    static Box mBox;

    //! Used to store the OpenGL buffers for the Cube.
    static QGLBuffer mBufferFace[ CUBE_FACE_COUNT ];

    //! Used to store the visible faces of the Cube to render.
    bool mFace[ CUBE_FACE_COUNT ];

    //! Used to track if the shared Cube OpenGL resources are loaded.
    static bool mLoaded;

    //! Used to store the position of the Cube.
    QVector3D mPosition;

    //! Used to store the shader program used to render the Cube.
    static QOpenGLShaderProgram mProgram;

    //! Used to store the OpenGL textures for the Cube.
    static QOpenGLTexture *mpTexture[CUBE_TYPE_COUNT];

    //! Used to store the CubeType of the Cube.
    CubeType mType;
}; // class Cube

#endif // CUBE_H
