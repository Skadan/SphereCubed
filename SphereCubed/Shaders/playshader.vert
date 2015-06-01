#version 130
//! \file playshader.vert
//! \brief Vertex shader file for play.
//! \note Requires OpenGL version 1.3 of greater.
//! \details This file contains the vertex shader for play in the application.

//! Create a uniform mat4 for the model view projection matrix.
uniform mat4 modelViewProjectionMatrix;
//! Create a uniform mat4 for the model view matrix.
uniform mat4 modelViewMatrix;
//! Create a uniform mat3 for the normal matrix.
uniform mat3 normalMatrix;
//! Create a uniform vec3 for the light position.
uniform vec3 lightPosition;
//! Create a in vec4 for the vertex data.
in vec4 vertex;
//! Create a in vec4 for the normal data.
in vec3 normal;
//! Create a in vec2 for the texture coordinate data.
in vec2 textureCoordinate;

//! Create a out varying vec2 for the coordinates normal.
out vec3 varyingNormal;
//! Create a out varying vec2 for the coordinates light direction.
out vec3 varyingLightDirection;
//! Create a out varying vec2 for the coordinates view direction.
out vec3 varyingViewerDirection;
//! Create a out varying vec2 for the Texture coordinates.
out vec2 varyingTextureCoordinate;

//! Entry point for the vertex playshader.
void main(void)
{
    //! Calculate the eye position.
    vec4 eyePosition = modelViewMatrix * vertex;
    //! Output the current coordinates normal.
    varyingNormal = normalMatrix * normal;
    //! Output the current coordinates light direction.
    varyingLightDirection = lightPosition - eyePosition.xyz;
    //! Output the current coordinates view direction.
    varyingViewerDirection = -eyePosition.xyz;
    //! Output the current texture coordinate.
    varyingTextureCoordinate = textureCoordinate;
    //! Output position of the vertex in clip space.
    gl_Position = modelViewProjectionMatrix * vertex;
} // void main(void)
