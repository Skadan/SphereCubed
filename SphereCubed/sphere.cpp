/*! \file sphere.cpp
\brief Sphere class implementation file.
This file contains the Sphere class implementation for the application.
*/

// sphere.h header file required for class definition.
#include "sphere.h"

// QtMath header file required for Trig functions.
#include <QtMath>

// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION true

#ifdef WIN32
    //! If a Windows build
    // glext.h header required for windows OpenGL textures.
    #include <GL/glext.h>
    //! Pointer to active texture on windows platform
    PFNGLACTIVETEXTUREPROC pGlActiveTexture = NULL;
    //! Define pointer to active texture on windows platform to gl command.
    #define glActiveTexture pGlActiveTexture
#endif //WIN32

//! The Sphere execution begins here.
//! Creates components to load and render a Sphere.
//! Automatically called by the Player during load.
Sphere::Sphere()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::Sphere()...";

    //! Initialize the Texture to NULL.
    mpTexture = NULL;
} // Sphere::Sphere()

//! The Sphere execution ends here.
//! Required to free the Sphere allocated memory.
//! Automatically called when Player unload.
Sphere::~Sphere()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::~Sphere()...";

    //! If the Texture is allocated.
    if( mpTexture != NULL )
    {
        //! Release the Texture.
        delete mpTexture;
        //! Set the Texture pointer to NULL.
        mpTexture = NULL;
    } // if( mpTexture != NULL )
} // Sphere::~Sphere()

//! Load the Opengl vertex data, shader program, and texture image.
//! \return void
void Sphere::load()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::load()...";

    //! Attempt to load the Spheree OpenGL Shaders, if fail unload and return without loading Sphere.
    if( loadShaders() == false ) { unload(); return; }

    //! Attempt to load the Sphere OpenGL buffers, if fail unload and return without loading Sphere.
    if( loadBuffer( ) == false ) { unload(); return; }

    //! Attempt to load the Sphere OpenGL textures, if fail unload and return without loading Sphere.
    if( loadTexture( ":Concrete" ) == false ) { unload(); return; }
} // Sphere::load()

//! Load the Opengl vertices data.
//! \return bool is true on success
bool Sphere::loadBuffer()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::loadBuffer()...";

    //! \todo Clean up logic and add detailed comments for Sphere vertex and UV generation.

    //! Set the radius of the Sphere to 0.5 units
    const float radius = 0.5f;
    //! Set the number of slices in the Sphere to 36
    const uint slices = 36;

    //! Create a Qvector3D array big enough to hold all the vertices.
    QVector3D vertex[slices * slices];

    //! Loop through the verticle slices.
    for(uint j=0; j<slices; j++)
    {
        //! Calculate the radius for the slice.
        float r = radius * qSin(qDegreesToRadians((180.0f/float(slices-1)) * float(j)));
        //! Calculate the y for the slice.
        float y = radius * qCos(qDegreesToRadians((180.0f/float(slices-1)) * float(j)));

        //! Loop through the horizontal slices.
        for(uint i=0; i<slices; i++)
        {
            //! Calculate the vertex at the intersection of the slices..
            vertex[j*slices+i] = QVector3D( (r * qSin(qDegreesToRadians( (360.0f/float(slices)) * float(i) - 90))),
                                               y,
                                               r * qCos(qDegreesToRadians((360.0f/float(slices)) * float(i) - 90)) );
        } // for(uint i=0; i<slices; i++)
    } // for(uint j=0; j<slices; j++)

    //! Create a vector of QVector3D to pack the vertex data for the quads.
    QVector<QVector3D> vertices;
    //! Create a vector of QVector3D to pack the normal data for the quads.
    QVector<QVector3D> normals;
    //! Create a vector of QVector2D to pack the texturecoordinate data for the quads.
    QVector<QVector2D> textureCoordinates;

    //! Loop through the vertical slices.
    for(uint k=1; k<slices; k++)
    {
        //! Loop through the horizontal slices.
        for(uint l=1; l<=slices; l++)
        {
            //! Calculate the vertices and texturecoordinates of the quad.
            QVector3D vert;
            QVector2D uv;

            //! Calculate the lower left vertex and uv coordinate.
            vert = vertex[(k-1)*slices + l-1];
            vertices << vert;
            vert.normalize();
            normals << vert;
            float _x1 = 0.5 + (float)( qRound( qAtan2(vert.z(),vert.x()) / (2*M_PI) * 1000) ) / 1000.0f;
            _x1 = _x1 == 0 ? 1 : _x1;
            float _y1 = 0.5 - qAsin(vert.y()) / M_PI;
            uv = QVector2D(_x1, _y1);
            //uv = QVector2D(0.0 + qAtan2(vert.z(),vert.x()) / (2*M_PI), 0.0f - qAsin(vert.y()) / M_PI);
            textureCoordinates << uv;

            //! Calculate the lower right vertex and uv coordinate.
            vert = vertex[(k-0)*slices + l-1];
            vertices << vert;
            vert.normalize();
            normals << vert;
            float _x2 = 0.5 + (float)( qRound( qAtan2(vert.z(),vert.x()) / (2*M_PI) * 1000) ) / 1000.0f;
            _x2 = _x2 == 0 ? 1 : _x2;
            float _y2 = 0.5 - qAsin(vert.y()) / M_PI;
            uv = QVector2D(_x2, _y2);
            //uv = QVector2D(0.0 + qAtan2(vert.z(),vert.x()) / (2*M_PI), 0.0f - qAsin(vert.y()) / M_PI);
            textureCoordinates << uv;

            //! Calculate the upper right vertex and uv coordinate.
            vert = vertex[(k-0)*slices + l];
            vertices << vert;
            vert.normalize();
            normals << vert;
            float _x3 = 0.5 + (float)( qRound( qAtan2(vert.z(),vert.x()) / (2*M_PI) * 1000) ) / 1000.0f;
            float _y3 = 0.5 - qAsin(vert.y()) / M_PI;
            uv = QVector2D(_x3, _y3);
            //uv = QVector2D(0.0 + qAtan2(vert.z(),vert.x()) / (2*M_PI), 0.0f - qAsin(vert.y()) / M_PI);
            textureCoordinates << uv;

            //! Calculate the upper left vertex and uv coordinate.
            vert = vertex[(k-1)*slices + l];
            vertices << vert;
            vert.normalize();
            normals << vert;
            float _x4 = 0.5 + (float)( qRound( qAtan2(vert.z(),vert.x()) / (2*M_PI) * 1000) ) / 1000.0f;
            float _y4 = 0.5 - qAsin(vert.y()) / M_PI;
            uv = QVector2D(_x4, _y4);
            //uv = QVector2D(0.0 + qAtan2(vert.z(),vert.x()) / (2*M_PI), 0.0f - qAsin(vert.y()) / M_PI);
            textureCoordinates << uv;
        } // for(uint l=1; l<=slices; l++)
    } // for(uint k=1; k<=slices; k++)

    //! Store the number of vertices in the Sphere.
    mVerticesCount = vertices.length();

    //! Attempt to create the buffer.
    if( mBuffer.create() == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::loadBuffer() - Failed to create buffer...";
        //! If failed to create buffer Assert to alert there is an issue.
        Q_ASSERT_X( false, "Sphere::loadBuffer()", "Faile to create buffer!");
        //! Return false without loading buffer.
        return false;
    } // if( mBuffer.create() == false )

    //! Attempt to Bind to the buffer.
    if( mBuffer.bind()  == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::loadBuffer() - Failed to bind buffer...";
        //! If failed to bind buffer Assert to alert there is an issue.
        Q_ASSERT_X( false, "Sphere::loadBuffers()", "Faile to bind buffer!");
        //! Return false without loading buffer.
        return false;
    } // if( mBuffer.bind()  == false )

    //! Allocate space for the vertices with normals and texturecoordinates.
    mBuffer.allocate( mVerticesCount * (3+3+2) * sizeof(GLfloat) );

    //! Transfer the vertex data to GPU memory.
    int offset = 0;
    mBuffer.write( offset, vertices.constData(), mVerticesCount * 3 * sizeof(GLfloat) );

    //! Then the normal data to GPU memory.
    offset += mVerticesCount * 3 * sizeof(GLfloat);
    mBuffer.write( offset, normals.constData(), mVerticesCount * 3 * sizeof(GLfloat) );

    //! Then the texturecoordinate data to GPU memory.
    offset += mVerticesCount * 3 * sizeof(GLfloat);
    mBuffer.write( offset, textureCoordinates.constData(), mVerticesCount * 2 * sizeof(GLfloat) );

    //! Release the buffer.
    mBuffer.release();

    //! Buffer loaded successfully return true
    return true;
} // Sphere::loadBuffer()

//! Load the Opengl shader program.
//! \return bool is true on success
bool Sphere::loadShaders()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::loadShaders()...";

    //! Attempt to add to the shader program the Sphere vertex shader.
    if( mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":PlayShaderVertex") == false)
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::load - addShaderFromSourceFile Vertex shader Failed...";
        //! If add failed Assert to alert there is an issue.
        Q_ASSERT_X(false, "mProgram.addShaderFromSourceFile Failed", ":PlayShaderVertex");
        //! Return false without loading shader.
        return false;
    } // if( mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":PlayShaderVertex") == false)

    //! Attempt to add to the shader program the Sphere fragment shader
    if( mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":PlayShaderFragment") == false)
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::load - addShaderFromSourceFile Fragment shader Failed...";
        //! If add failed Assert to alert there is an issue.
        Q_ASSERT_X(false, "mProgram.addShaderFromSourceFile Failed", ":PlayShaderFragment");
        //! Remove all shaders from program.
        mProgram.removeAllShaders();
        //! Return false without loading shader.
        return false;
    } // if( mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":PlayShaderFragment") == false)

    //! Attempt to link the shader program
    if( mProgram.link() == false)
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::load - link Sphere shader Failed...";
        //! If link failed Assert to alert there is an issue.
        Q_ASSERT_X(false, "mProgram.link", "Failed");
        //! Remove all shaders from program.
        mProgram.removeAllShaders();
        //! Return false without loading shader.
        return false;
    } // if( mProgram.link() == false)

    //! Shader loaded successfully return true
    return true;
} // Sphere::loadShaders()

//! Load the Opengl texture image.
//! \param texture is the path to the texture image to load.
//! \return bool is true on success
bool Sphere::loadTexture( QString texture )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::loadTexture()...";

    #ifdef WIN32
    //! if a windows build
    //! Assign the ActiveTexture procedure address to the glActiveTexture pointer
    glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress((LPCSTR) "glActiveTexture");
    #endif

    //! Attempt to load the image.
    QImage image = QImage( texture );

    //! If failed to load image
    if( image.isNull() == true )
    {
        //! Assert to alert there is an issue.
        Q_ASSERT_X( false, "Load Image Failed", texture.toLatin1() );
        //! Return false without loading texture.
        return false;
    } // if( image.isNull() == true )

    //! Attempt to allocate memory for the texture and convert the image to an OpenGL texture.
    if( (mpTexture = new QOpenGLTexture( image )) == NULL )
    {
        //! If allocate texture failed Assert to alert there is an issue.
        Q_ASSERT_X( false, "Convert Image to Texture Failed", texture.toLatin1() );
        //! Return false without loading texture.
        return false;
    } // if( (mpTexture = new QOpenGLTexture( image )) == NULL )

    //! Texture loaded successfully return true.
    return true;
} // Sphere::loadTexture()

//! Render the Opengl resources for the Sphere.
//! \param camera is a const reference to Camera viewing the scene.
//! \param light is a const reference to the Light lightinh the scene.
//! \return void
void Sphere::render( const Camera &camera, const Light &light )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::render( const Camera &camera, const Light &light )...";

    //! Get a reference to the Camera view matrix.
    const QMatrix4x4 & viewMatrix = camera.viewMatrix();
    //! Get a reference to the Camera projection matrix.
    const QMatrix4x4 & projectionMatrix = camera.projectionMatrix();
    //! Create a model matrix.
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    //! Translate the model matrix by the Sphere position.
    modelMatrix.translate( mPosition );
    //! Rotate the model matrix by the Sphere rotation.
    modelMatrix.rotate( mRotation );
    //! Calculate the model view matrix.
    QMatrix4x4 modelViewMatrix = viewMatrix * modelMatrix;
    //! Calculate the model view projection matrix.
    QMatrix4x4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
    //! Calculate the normal matrix.
    QMatrix3x3 normalMatrix = modelViewMatrix.normalMatrix();

    //! Attempt to bind to the shader program
    if( mProgram.bind() == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::render - program bind failed...";
        //! If bind failed return without rendering.
        return;
    } // if( mProgram.bind() == false )

    //! Set the shader programs uniform values.
    mProgram.setUniformValue("modelViewProjectionMatrix", modelViewProjectionMatrix);
    mProgram.setUniformValue("modelViewMatrix", modelViewMatrix);
    mProgram.setUniformValue("normalMatrix", normalMatrix);
    mProgram.setUniformValue("lightPosition", viewMatrix * light.position());
    mProgram.setUniformValue("ambientColor", light.ambient());
    mProgram.setUniformValue("diffuseColor", light.diffuse());
    mProgram.setUniformValue("specularColor", light.specular());
    mProgram.setUniformValue("ambientReflection", (GLfloat) 1.0);
    mProgram.setUniformValue("diffuseReflection", (GLfloat) 1.0);
    mProgram.setUniformValue("specularReflection", (GLfloat) 1.0);
    mProgram.setUniformValue("shininess", (GLfloat) 100.0);
    mProgram.setUniformValue("texture", 0);

    //! Set the active multitexture index to 0 and bind to it.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mpTexture->textureId());
    glActiveTexture(0);

    //! Attempt to bind to the buffer.
    if( mBuffer.bind() == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::renderFace - Buffer bind failed...";
        //! If bind fails return without rendering.
        return;
    } // if( mBuffer.bind() == false )

    //! Set the vertex AttributeBuffer and enable the AttributeArray.
    int offset = 0;
    mProgram.setAttributeBuffer("vertex", GL_FLOAT, offset, 3, 0);
    mProgram.enableAttributeArray("vertex");

    //! Set the normal AttributeBuffer and enable the AttributeArray.
    offset += mVerticesCount * 3 * sizeof(GLfloat);
    mProgram.setAttributeBuffer("normal", GL_FLOAT, offset, 3, 0);
    mProgram.enableAttributeArray("normal");

    //! Set the texturecoordinate AttributeBuffer and enable the AttributeArray.
    offset += mVerticesCount * 3 * sizeof(GLfloat);
    mProgram.setAttributeBuffer("textureCoordinate", GL_FLOAT, offset, 2, 0);
    mProgram.enableAttributeArray("textureCoordinate");

    //! Release the buffer.
    mBuffer.release();

    //! Render the Sphere.
    glDrawArrays(GL_QUADS, 0, mVerticesCount);

    //! Disable the vertex, normal, and texturecoordinate AttributeArray.
    mProgram.disableAttributeArray("vertex");
    mProgram.disableAttributeArray("normal");
    mProgram.disableAttributeArray("textureCoordinate");

    //! Release the shader program.
    mProgram.release();
} // Sphere::render( const Camera &camera, const Light &light )

//! Update the Sphere.
//! \return void
void Sphere::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::tick()...";
} // Sphere::tick()

//! Unload the Opengl resources for the Sphere.
//! \return void
void Sphere::unload()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Sphere::unload()...";

    //! Remove all shaders added to the program.
    mProgram.removeAllShaders();
    //! Release the program.
    mProgram.release();

    //! Release and destroy all buffer.
    mBuffer.release();
    mBuffer.destroy();

    //! Release, destroy, and free texture memory.
    if( mpTexture != NULL )
    {
        mpTexture->release();
        mpTexture->destroy();

        delete mpTexture;
        mpTexture = NULL;
    } // if( mpTexture != NULL )
} // Sphere::unload()
