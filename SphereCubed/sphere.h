/*! \file sphere.h
\brief Sphere class declaration file.
This file contains the Sphere class declaration for the application.
*/

#ifndef SPHERE_H
#define SPHERE_H

// QGLBuffer header file required for OpenGL buffer.
#include <QGLBuffer>
// QOpenGLShaderProgram header file required for OpenGL shader program.
#include <QOpenGLShaderProgram>
// QOpenGLTexture header file required for OpenGL textures.
#include <QOpenGLTexture>
// QQuaternion header file required for rotation.
#include <QQuaternion>
// QVector3D header file required for vertices and position.
#include <QVector3D>

// camera.h header file required for rendering.
#include "camera.h"
// light.h header file required for rendering.
#include "light.h"

//! Sphere class.
/*!
This class declaration encapsulates the Sphere functionality.
*/
class Sphere
{
    //! Friend of Player so that only Player can instantiate the Sphere.
    friend class Player;

private: // Constructors / Destructors
    //! Sphere class constructor.
    Sphere();

    //! Sphere class destructor.
    ~Sphere();

private: // Member Functions
    //! Called to load the Sphere OpenGL resources.
    void load();

    //! Called to render the Sphere OpenGL resources.
    void render( const Camera & camera, const Light & light );

    //! Called to update the Sphere.
    void tick();

    //! Called to unload the Sphere OpenGL resources.
    void unload();

private: // Helper Functions
    //! Called to load the Sphere OpenGL buffers.
    bool loadBuffer();

    //! Called to load the Sphere OpenGL shaders.
    bool loadShaders();

    //! Called to load the Sphere OpenGL textures.
    bool loadTexture( QString texture );

private:
    //! Used to store the OpenGL buffer for the Sphere.
    QGLBuffer mBuffer;

    //! Used to store the position of the Sphere.
    QVector3D mPosition;

    //! Used to store the shader program used to render the Sphere.
    QOpenGLShaderProgram mProgram;

    //! Used to store the rotation of the Sphere.
    QQuaternion mRotation;

    //! Used to store the OpenGL texture for the Sphere.
    QOpenGLTexture * mpTexture;

    //! Used to store the number of vertices used to render the Sphere.
    uint mVerticesCount;
}; // class Sphere

#endif // SPHERE_H
