/*! \file menu.h
\brief Menu class declaration file.
This file contains the Menu class declaration for the application.
*/

#ifndef MENU_H
#define MENU_H

// camera.h header file required for Camera class definition.
#include "camera.h"

// QGLBuffer header file required for OpenGL memory buffer.
#include <QGLBuffer>
// QOpenGLShaderProgram header file required for OpenGL shader program.
#include <QOpenGLShaderProgram>
// QOpenGLTexture header file required for OpenGL texture buffer.
#include <QOpenGLTexture>

//! Menu class.
/*!
This class declaration encapsulates the Menu functionality.
*/
class Menu
{
public: // Constructors / Destructors
    //! Menu class constructor.
    Menu();

    //! Menu class destructor.
    ~Menu();

public: // Member Function
    //! Triggered after a tick or when ever scene needs to be rendered.
    void render( const Camera & camera );

    //! Triggered periodically.
    void tick();

    //! Called to load a Menu with a specific image.
    void load( QString imageName );

    //! Called to unload a Menu and image.
    void unload();

private: // Member Variables
    //! Used to store the OpenGL shader program.
    QOpenGLShaderProgram    mProgram;

    //! Used to store the OpenGL buffer.
    QGLBuffer               mBuffer;

    //! Used to store the OpenGL texture memory.
    QOpenGLTexture         *mpTexture;
};

#endif // MENU_H
