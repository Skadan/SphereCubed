/*! \file cube.cpp
\brief Cube class implementation file.
This file contains the Cube class implementation for the application.
*/

// cube.h header file required for class definition.
#include "cube.h"

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

//! Initialize the static member variables.
Box                     Cube::mBox( true );
QGLBuffer               Cube::mBufferFace[ Cube::CubeFace::CUBE_FACE_COUNT ];
bool                    Cube::mLoaded = false;
QOpenGLShaderProgram    Cube::mProgram;
QOpenGLTexture *        Cube::mpTexture[ Cube::CubeType::CUBE_TYPE_COUNT ];

//! The Cube execution begins here.
//! Creates components to load and render a Cube.
//! Automatically called by the Level during load.
Cube::Cube()
{
    //! Initialize all Cube Textures to NULL.
    for( int i = 0; i < CubeType::CUBE_TYPE_COUNT; i++ ) { mpTexture[i] = NULL; }
} // Cube::Cube()

//! The Cube execution ends here.
//! Required to free the Cube allocated memory.
//! Automatically called when Level unload.
Cube::~Cube()
{
    //! Make sure to unload all Cube resources.
    unload();
} // Cube::~Cube()

//! Load the Opengl vertex data, shader program, and texture image.
//! \return void
void Cube::load()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Cube::load()...";

    //! If Cube resources are already loaded Assert to alert there is an issue.
    Q_ASSERT_X( mLoaded == false, "Cube::load","Resources Already Loaded!");

    //! Attempt to load the Cube OpenGL Shaders, if fail unload and return without loading Cube.
    if( loadShaders() == false ) { unload(); return; }

    //! Attempt to load the Cube OpenGL buffers, if fail unload and return without loading Cube.
    if( loadBuffers( Cube::NEAR )   == false ) { unload(); return; }
    if( loadBuffers( Cube::FAR )    == false ) { unload(); return; }
    if( loadBuffers( Cube::TOP )    == false ) { unload(); return; }
    if( loadBuffers( Cube::BOTTOM ) == false ) { unload(); return; }
    if( loadBuffers( Cube::RIGHT )  == false ) { unload(); return; }
    if( loadBuffers( Cube::LEFT )   == false ) { unload(); return; }

    //! Attempt to load the Cube OpenGL textures, if fail unload and return without loading Cube.
    if( loadTexture( CubeType::STANDARD, ":Standard" ) == false ) { unload(); return; }

    //! Track that the Cube resource are now loaded.
    mLoaded = true;
} // Cube::load()

//! Load the Opengl vertices data.
//! \param cubeFace identifies what vertices to load.
//! \return bool is true on success
bool Cube::loadBuffers( CubeFace cubeFace )
{
    //! Create a vector for packing the 3D vertices of the cube faces.
    QVector<QVector3D>      vertices;
    //! Create a vector for packing the normals of the cube faces.
    QVector<QVector3D>      normals;
    //! Create a vector for packing the 2D texture coordinates of the cube faces.
    QVector<QVector2D>      textureCoordinates;

    //! Determine which face is being packed.
    switch( cubeFace )
    {
    case CubeFace::NEAR:
        //! Pack the front face quad vertieces
        vertices << mBox.corner( Box::Corners::NEAR_TOP_LEFT ) << mBox.corner( Box::Corners::NEAR_BOTTOM_LEFT )
                 << mBox.corner( Box::Corners::NEAR_BOTTOM_RIGHT ) << mBox.corner( Box::Corners::NEAR_TOP_RIGHT );
        normals  << QVector3D( 0,  0,  1 ) << QVector3D( 0,  0,  1 ) << QVector3D( 0,  0,  1 ) << QVector3D( 0,  0,  1 );
        textureCoordinates << QVector2D(0, 0) << QVector2D(0, 1) << QVector2D(1, 1) << QVector2D(1, 0);
        break;

    case CubeFace::FAR:
        //! Pack the back face quad vertieces
        vertices << mBox.corner( Box::Corners::FAR_TOP_RIGHT ) << mBox.corner( Box::Corners::FAR_BOTTOM_RIGHT )
                 << mBox.corner( Box::Corners::FAR_BOTTOM_LEFT ) << mBox.corner( Box::Corners::FAR_TOP_LEFT );
        normals  << QVector3D( 0,  0,  -1 ) << QVector3D( 0,  0,  -1 ) << QVector3D( 0,  0,  -1 ) << QVector3D( 0,  0,  -1 );
        textureCoordinates << QVector2D(0, 0) << QVector2D(0, 1) << QVector2D(1, 1) << QVector2D(1, 0);
        break;

    case CubeFace::TOP:
        //! Pack the top face quad vertieces
        vertices << mBox.corner( Box::Corners::NEAR_TOP_RIGHT ) << mBox.corner( Box::Corners::FAR_TOP_RIGHT )
                 << mBox.corner( Box::Corners::FAR_TOP_LEFT ) << mBox.corner( Box::Corners::NEAR_TOP_LEFT );
        normals  << QVector3D( 0,  1,  0 ) << QVector3D( 0,  1,  0 ) << QVector3D( 0,  1,  0 ) << QVector3D( 0,  1,  0 );
        textureCoordinates << QVector2D(0, 0) << QVector2D(0, 1) << QVector2D(1, 1) << QVector2D(1, 0);
        break;

    case CubeFace::BOTTOM:
        //! Pack the bottom face quad vertieces
        vertices << mBox.corner( Box::Corners::FAR_BOTTOM_RIGHT ) << mBox.corner( Box::Corners::NEAR_BOTTOM_RIGHT )
                 << mBox.corner( Box::Corners::NEAR_BOTTOM_LEFT ) << mBox.corner( Box::Corners::FAR_BOTTOM_LEFT );
        normals  << QVector3D( 0,  -1,  0 ) << QVector3D( 0,  -1,  0 ) << QVector3D( 0,  -1,  0 ) << QVector3D( 0,  -1,  0 );
        textureCoordinates << QVector2D(0, 0) << QVector2D(0, 1) << QVector2D(1, 1) << QVector2D(1, 0);
        break;

    case CubeFace::RIGHT:
        //! Pack the right face quad vertieces
        vertices << mBox.corner( Box::Corners::NEAR_TOP_RIGHT ) << mBox.corner( Box::Corners::NEAR_BOTTOM_RIGHT )
                 << mBox.corner( Box::Corners::FAR_BOTTOM_RIGHT ) << mBox.corner( Box::Corners::FAR_TOP_RIGHT );
        normals  << QVector3D( 1,  0,  0 ) << QVector3D( 1,  0,  0 ) << QVector3D( 1,  0,  0 ) << QVector3D( 1,  0,  0 );
        textureCoordinates << QVector2D(0, 0) << QVector2D(0, 1) << QVector2D(1, 1) << QVector2D(1, 0);
        break;

    case CubeFace::LEFT:
        //! Pack the left face quad vertieces
        vertices << mBox.corner( Box::Corners::FAR_TOP_LEFT ) << mBox.corner( Box::Corners::FAR_BOTTOM_LEFT )
                 << mBox.corner( Box::Corners::NEAR_BOTTOM_LEFT ) << mBox.corner( Box::Corners::NEAR_TOP_LEFT );
        normals  << QVector3D( -1,  0,  0 ) << QVector3D( -1,  0,  0 ) << QVector3D( -1,  0,  0 ) << QVector3D( -1,  0,  0 );
        textureCoordinates << QVector2D(0, 0) << QVector2D(0, 1) << QVector2D(1, 1) << QVector2D(1, 0);
        break;

    default:
        //! Undefined face Assert to alert there is an issue.
        Q_ASSERT_X( false, "Cube::loadBuffers( CubeFace cubeFace )", "Undefined Face!");
        //! Undefined face return false without loading OpenGL buffer.
        return false;
        break;
    } // switch( cubeFace )

    //! Attempt to create the face buffer.
    if( mBufferFace[ cubeFace ].create() == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Cube::loadBuffers() - Failed to create buffer - " << cubeFace << "...";
        //! If failed to create buffer Assert to alert there is an issue.
        Q_ASSERT_X( false, "Cube::loadBuffers( CubeFace cubeFace )", "Faile to create buffer!");
        //! Return false without loading buffer.
        return false;
    }

    //! Attempt to Bind to the buffer.
    if( mBufferFace[ cubeFace ].bind()  == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Cube::loadBuffers() - Failed to bind buffer - " << cubeFace << "...";
        //! If failed to bind buffer Assert to alert there is an issue.
        Q_ASSERT_X( false, "Cube::loadBuffers( CubeFace cubeFace )", "Faile to bind buffer!");
        //! Return false without loading buffer.
        return false;
    }

    //! Allocate space for the 4 vertices with normals and texturecoordinates.
    mBufferFace[ cubeFace ].allocate(4 * (3+3+2) * sizeof(GLfloat));

    //! Transfer the vertex data to GPU memory.
    int offset = 0;
    mBufferFace[ cubeFace ].write(offset, vertices.constData(), 4 * 3 * sizeof(GLfloat));

    //! Then the normal data to GPU memory.
    offset += 4 * 3 * sizeof(GLfloat);
    mBufferFace[ cubeFace ].write(offset, normals.constData(), 4 * 3 * sizeof(GLfloat));

    //! Then the texturecoordinate data to GPU memory.
    offset += 4 * 3 * sizeof(GLfloat);
    mBufferFace[ cubeFace ].write(offset, textureCoordinates.constData(), 4 * 2 * sizeof(GLfloat));

    //! Release the buffer.
    mBufferFace[ cubeFace ].release();

    //! Buffer loaded successfully return true
    return true;
} // Cube::loadBuffers( CubeFace cubeFace )

//! Load the Opengl shader program.
//! \return bool is true on success
bool Cube::loadShaders()
{
    //! Attempt to add to the shader program the cube vertex shader.
    if( mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":PlayShaderVertex") == false)
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Menu::load - addShaderFromSourceFile Vertex shader Failed...";
        //! If add failed Assert to alert there is an issue.
        Q_ASSERT_X(false, "mProgram.addShaderFromSourceFile Failed", ":PlayShaderVertex");
        //! Return false without loading shader.
        return false;
    } // if( mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":PlayShaderVertex") == false)

    //! Attempt to add to the shader program the cube fragment shader
    if( mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":PlayShaderFragment") == false)
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Cube::load - addShaderFromSourceFile Fragment shader Failed...";
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
        TraceOut( TRACE_FILE_EXECUTION ) << "Cube::load - link cube shader Failed...";
        //! If link failed Assert to alert there is an issue.
        Q_ASSERT_X(false, "mProgram.link", "Failed");
        //! Remove all shaders from program.
        mProgram.removeAllShaders();
        //! Return false without loading shader.
        return false;
    } // if( mProgram.link() == false)

    //! Shader loaded successfully return true
    return true;
} // Cube::loadShaders()

//! Load the Opengl texture image.
//! \param cubeType is index into texture array to load texture data to.
//! \param texture is the path to the texture image to load.
//! \return bool is true on success
bool Cube::loadTexture( CubeType cubeType, QString texture )
{
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
        //! Return fasle without loading texture.
        return false;
    } // if( image.isNull() == true )

    //! Attempt to allocate memory for the texture and convert the image to an OpenGL texture.
    if( (mpTexture[ cubeType ] = new QOpenGLTexture( image )) == NULL )
    {
        //! If allocate texture failed Assert to alert there is an issue.
        Q_ASSERT_X( false, "Convert Image to Texture Failed", texture.toLatin1() );
        //! Return fasle without loading texture.
        return false;
    }

    //! Texture loaded successfully return true.
    return true;
} // Cube::loadTexture( CubeType cubeType, QString texture )

//! Render the Opengl resources for the Cube.
//! \param camera is a const reference to Camera viewing the scene.
//! \param light is a const reference to the Light lightinh the scene.
//! \return void
void Cube::render( const Camera & camera, const Light & light )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Cube::render()...";

    //! If Cube resources are NOT loaded Assert to alert there is an issue.
    Q_ASSERT_X( mLoaded == true, "Cube::render","Resources NOT Loaded!");

    //! Get a reference to the Camera view matrix.
    const QMatrix4x4 & viewMatrix       = camera.viewMatrix();
    //! Get a reference to the Camera projection matrix.
    const QMatrix4x4 & projectionMatrix = camera.projectionMatrix();
    //! Create a model matrix.
    QMatrix4x4 modelMatrix;
    //! Translate the model matrix by the Cube position.
    modelMatrix.translate( mPosition );
    //! Calculate the model view matrix.
    QMatrix4x4 modelViewMatrix = viewMatrix * modelMatrix;
    //! Calculate the model view projection matrix.
    QMatrix4x4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
    //! Calculate the normal matrix.
    QMatrix3x3 normalMatrix = modelViewMatrix.normalMatrix();

    //! Attempt to bind to the shader program
    if( mProgram.bind() == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Cube::render - program bind failed...";
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
    //glBindTexture(GL_TEXTURE_2D, mpTexture[mType]->textureId());
    glBindTexture( GL_TEXTURE_2D, mpTexture[CubeType::STANDARD]->textureId() );
    glActiveTexture(0);

    //! Rendr the Cube faces.
    renderFace( CubeFace::TOP );
    renderFace( CubeFace::BOTTOM );
    renderFace( CubeFace::LEFT );
    renderFace( CubeFace::RIGHT );
    renderFace( CubeFace::NEAR );
    renderFace( CubeFace::FAR );

    //! Release the shader program.
    mProgram.release();
} // Cube::render( const Camera & camera, const Light & light )

//! Render the Opengl resources for a face of the Cube.
//! \param cubeFace is the face to render.
//! \return void
void Cube::renderFace( CubeFace cubeFace )
{
    //! Determine if the face is visible.
    if( mFace[ cubeFace ] == true )
    {
        //! Attempt to bind to the face's buffer.
        if( mBufferFace[ cubeFace ].bind() == false )
        {
            TraceOut( TRACE_FILE_EXECUTION ) << "Cube::renderFace - Face buffer bind failed - " <<  cubeFace << "...";
            //! If bind fails return without rendering.
            return;
        } // if( mBufferFace[ cubeFace ].bind() == false )

        //! Set the vertex AttributeBuffer and enable the AttributeArray.
        int offset = 0;
        mProgram.setAttributeBuffer("vertex", GL_FLOAT, offset, 3, 0);
        mProgram.enableAttributeArray("vertex");

        //! Set the normal AttributeBuffer and enable the AttributeArray.
        offset += 4 * 3 * sizeof(GLfloat);
        mProgram.setAttributeBuffer("normal", GL_FLOAT, offset, 3, 0);
        mProgram.enableAttributeArray("normal");

        //! Set the texturecoordinate AttributeBuffer and enable the AttributeArray.
        offset += 4 * 3 * sizeof(GLfloat);
        mProgram.setAttributeBuffer("textureCoordinate", GL_FLOAT, offset, 2, 0);
        mProgram.enableAttributeArray("textureCoordinate");

        //! Release the face's buffer.
        mBufferFace[ cubeFace ].release();

        //! Render the Quad.
        glDrawArrays(GL_QUADS, 0, 4);

        //! Disable the vertex, normal, and texturecoordinate AttributeArray.
        mProgram.disableAttributeArray("vertex");
        mProgram.disableAttributeArray("normal");
        mProgram.disableAttributeArray("textureCoordinate");
    } // if( mFace[ cubeFace ] == true )
} // Cube::renderFace( CubeFace cubeFace )

//! Update the Cube.
//! \return void
void Cube::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Cube::tick()...";
} // Cube::tick()

//! Unload the Opengl resources for the Cube.
//! \return void
void Cube::unload()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Cube::upload()...";

    //! Remove all shaders added to the program.
    mProgram.removeAllShaders();
    //! Release the program.
    mProgram.release();

    //! Release and destroy all face buffers.
    for( int i = 0; i < CubeFace::CUBE_FACE_COUNT; i++ )
    {
        mBufferFace[i].release();
        mBufferFace[i].destroy();
    } // for( int i = 0; i < CubeFace::CUBE_FACE_COUNT; i++ )

    //! Release, destroy, and free all texture memory.
    for( int i = 0; i < CubeType::CUBE_TYPE_COUNT; i++ )
    {
        if( mpTexture[i] != NULL )
        {
            mpTexture[i]->release();
            mpTexture[i]->destroy();

            delete mpTexture[i];
            mpTexture[i] = NULL;
        } // if( mpTexture[i] != NULL )
    } // for( int i = 0; i < CubeType::CUBE_TYPE_COUNT; i++ )

    //! Track that the Cube resource are NOT loaded.
    mLoaded = false;
} // Cube::unload()
