#version 130
//! \file menushader.frag
//! \brief Fragment shader file for menus.
//! \note Requires OpenGL version 1.3 of greater.
//! \details This file contains the fragment shader for menus in the application.

//! Create a uniform sampler2D for the texture.
uniform sampler2D texture;

//! Create a in varying vec2 for the Texture coordinates.
in vec2 varyingTextureCoordinate;

//! Create a out vec4 for the fragement color.
out vec4 fragColor;

//! Entry point for the fragment menushader.
void main(void)
{
    //! Lookup the fragment color in the texture for the coordinate.
    fragColor = texture2D(texture, varyingTextureCoordinate);
} // void main(void)
