#version 130
//! Requires OpenGL version 1.3 of greater.

/*! \file menushader.vert
\brief menushader vertex shader file.
This file contains the vertex shader for menus in the game application.
*/

//! Create a uniform mat4 for the model view projection matrix.
uniform mat4 mvpMatrix;

//! Create a in vec4 for the vertex data.
in vec4 vertex;

//! Create a in vec2 for the texture coordinate data.
in vec2 textureCoordinate;

//! Create a out varying vec2 for the Texture coordinates.
out vec2 varyingTextureCoordinate;

//! Entry point for the vertex menushader.
void main(void)
{
    //! Output the current texture coordinate.
    varyingTextureCoordinate = textureCoordinate;

    //! Output position of the vertex in clip space.
    gl_Position = mvpMatrix * vertex;
} // void main(void)
