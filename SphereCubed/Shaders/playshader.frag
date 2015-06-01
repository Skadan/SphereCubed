#version 130
//! \file playshader.frag
//! \brief Fragment shader file for play.
//! \note Requires OpenGL version 1.3 of greater.
//! \details This file contains the fragment shader for play in the application.

//! Create a uniform vec4 for ambient light color.
uniform vec4 ambientColor;
//! Create a uniform vec4 for diffuse light color.
uniform vec4 diffuseColor;
//! Create a uniform vec4 for specular light color.
uniform vec4 specularColor;
//! Create a uniform float for ambient light reflection coefficient.
uniform float ambientReflection;
//! Create a uniform float for diffuse light reflection coefficient.
uniform float diffuseReflection;
//! Create a uniform float for specular light reflection coefficient.
uniform float specularReflection;
//! Create a uniform float for surface shininess coefficient.
uniform float shininess;
//! Create a uniform sampler2D for the texture.
uniform sampler2D texture;

//! Create a varying vec3 for the normal.
in vec3 varyingNormal;
//! Create a varying vec3 for the light direction coordinates.
in vec3 varyingLightDirection;
//! Create a varying vec3 for the view direction coordinates.
in vec3 varyingViewerDirection;
//! Create a varying vec2 for the texture coordinates.
in vec2 varyingTextureCoordinate;

//! Create a out vec4 for the fragement color.
out vec4 fragmentColor;

//! Entry point for the fragment playshader.
void main(void)
{
    //! Get the normal.
    vec3 normal = normalize(varyingNormal);
    //! Get the light direction.
    vec3 lightDirection = normalize( varyingLightDirection );
    //! Get the view direction.
    vec3 viewerDirection = normalize( varyingViewerDirection );
    //! Calculate the ambient illumination.
    vec4 ambientIllumination = ambientReflection * ambientColor;
    //! Calculate the diffuse illumination.
    vec4 diffuseIllumination = diffuseReflection * max( 0.0, dot( lightDirection, normal ) ) * diffuseColor;
    //! Calculate the specular illumination.
    vec4 specularIllumination = specularReflection * pow( max( 0.0, dot( -reflect( lightDirection, normal ), viewerDirection ) ), shininess ) * specularColor;
    //! Lookup the fragment color in the texture for the coordinate and apply the illumination.
    fragmentColor = texture2D( texture, varyingTextureCoordinate ) * ( ambientIllumination + diffuseIllumination ) + specularIllumination;
} // void main(void)
