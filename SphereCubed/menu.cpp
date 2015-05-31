/*! \file menu.cpp
\brief Menu class implementation file.
This file contains the Menu class implementation for the application.
*/

// menu.h header file required for class definition.
#include "menu.h"

// QtGlobal header file required for asserts.
#include <QtGlobal>
// QMatrix4x4 header file required for model, view, projection matrices.
#include <QMatrix4x4>
// QVector header file required for building model.
#include <QVector>
// QVector2D header file required for texture coordinates.
#include <QVector2D>
// QVector3D header file required for vertices.
#include <QVector3D>

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

//! The Menu execution begins here.
//! Creates components to load a Menu.
//! Automatically called by the World during construction.
Menu::Menu() :
    mpTexture( NULL )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Menu::Menu()...";
} // Menu::Menu()

//! The Menu execution ends here.
//! Required to free the Menu allocated memory.
//! Automatically called when World is destroyed.
Menu::~Menu()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Menu::~Menu()...";

    //! If texture memory is allocated.
    if( mpTexture != NULL )
    {
        //! Release the allocated memory.
        delete mpTexture;
        //! Set memory pointer to null.
        mpTexture = NULL;
    } // if( mpTexture != NULL )
} // Menu::~Menu()

//! Load the Opengl vertex data, shader program, and texture image.
void Menu::load( QString imageName )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Menu::load()...";

    #ifdef WIN32
        //! if a windows build.
        //! Assign the ActiveTexture procedure address to the glActiveTexture pointer.
        glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress((LPCSTR) "glActiveTexture");
    #endif

    //! Create a vector for packing the 3D vertices of the background image quad.
    QVector<QVector3D>      vertices;
    //! Create a vector for packing the 2D texture coordinates of the background image quad.
    QVector<QVector2D>      uvCoords;

    //! Attempt to add to the shader program the menu vertex shader.
    if( mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":MenuShaderVertex") == false)
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Menu::load - addShaderFromSourceFile Vertex shader Failed...";
        //! If add failed Assert to alert there is an issue.
        Q_ASSERT_X(false, "mProgram.addShaderFromSourceFile Failed", ":/Shaders/menushader.vert");
        //! Return without loading the Menu.
        return;
    } // if( mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/menushader.vert") == false)

    //! Attempt to add to the shader program the menu fragment shader.
    if( mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":MenuShaderFragment") == false)
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Menu::load - addShaderFromSourceFile Fragment shader Failed...";
        //! If add failed Assert to alert there is an issue.
        Q_ASSERT_X(false, "mProgram.addShaderFromSourceFile Failed", ":/Shaders/menushader.frag");
        //! Remove any attached shaders.
        mProgram.removeAllShaders();
        //! Return without loading the Menu.
        return;
    } // if( mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/menushader.frag") == false)

    //! Attempt to link the shader program.
    if( mProgram.link() == false)
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Menu::load - link menu shader Failed...";
        //! If link failed Assert to alert there is an issue.
        Q_ASSERT_X(false, "mProgram.link", "Failed");
        //! Remove any attached shaders.
        mProgram.removeAllShaders();
        //! Return without loading the Menu.
        return;
    } // if( mProgram.link() == false)

    //! Pack the Quad vertieces that extend from -1 to 1 in the x and y directions.
    vertices << QVector3D( -1,  1,  0 ) << QVector3D( -1, -1,  0 ) << QVector3D(  1, -1,  0 ) << QVector3D(  1,  1,  0 );
    //! Pack the Quad texture coordinates that map the entire image to the entire quad.
    uvCoords << QVector2D( 0, 0 ) << QVector2D( 0, 1 ) << QVector2D( 1, 1 ) << QVector2D( 1, 0 );

    //! Attempt to create an OpenGL buffer for the vertices and texture coordinates.
    if(  mBuffer.create() == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Menu::load - create buffer Failed...";
        //! If create failed Assert to alert there is an issue.
        Q_ASSERT_X(false, "mBuffer.create", "Failed");
        //! Remove any attached shaders.
        mProgram.removeAllShaders();
        //! Return without loading the Menu.
        return;
    } // if(  mBuffer.create() == false )

    //! Attempt to bind to the buffer.
    if( mBuffer.bind() == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Menu::load - bind to buffer Failed...";
        //! If bind failed Assert to alert there is an issue.
        Q_ASSERT_X(false, "mBuffer.bind", "Failed");
        //! Remove any attached shaders.
        mProgram.removeAllShaders();
        //! Return without loading the Menu.
        return;
    } // if( mBuffer.bind() == false )

    //! Allocate memory for the buffer.
    mBuffer.allocate(4 * (3+2) * sizeof(GLfloat));

    //! Track the data offset, the vertex data starts first at zero.
    int offset = 0;
    //! Write the vertex data into the buffer.
    mBuffer.write(offset, vertices.constData(), 4 * 3 * sizeof(GLfloat));

    //! Track the data offset, the texture coordinate data starts after the vertex data.
    offset += 4 * 3 * sizeof(GLfloat);
    //! Write the texture coordinate data into the buffer.
    mBuffer.write(offset, uvCoords.constData(), 4 * 2 * sizeof(GLfloat));

    //! Release the buffer.
    mBuffer.release();

    //! Attempt to load the background image.
    QImage image = QImage(imageName);

    //! If failed to load image.
    if( image.isNull() == true )
    {
        //! Assert to alert there is an issue.
        Q_ASSERT_X(false, "Load Image Failed", imageName.toLatin1().constData());       
        //! Remove any attached shaders.
        mProgram.removeAllShaders();
        //! Return without loading the Menu.
        return;
    } // if( image.isNull() == true )

    //! Convert the image to an OpenGL texture.
    mpTexture = new QOpenGLTexture(image);
} // Menu::load( QString imageName )

//! Render the Menu from the view point of the Camera.
void Menu::render( const Camera & camera )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Menu::render()...";

    //! Verify the texture is loaded.
    if( mpTexture == NULL )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Menu::render - texture not loaded...";
        //! If image not loaded return without rendering.
        return;
    }

    //! Attempt to bind to the shader program.
    if( mProgram.bind() == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Menu::render - program bind failed...";
        //! If bind failed return without rendering.
        return;
    } // if( mProgram.bind() == false )

    //! Combine the model, view, and projection matrices.
    QMatrix4x4 modelViewProjectionMatrix = camera.projectionMatrix() * camera.viewMatrix();

    //! Pass the model view projection matrix to the shader program.
    mProgram.setUniformValue("mvpMatrix", modelViewProjectionMatrix );

    //! Pass the active texture Id to the shader program.
    mProgram.setUniformValue("texture", 0);

    //! Specify which texture unit to make active.
    glActiveTexture(GL_TEXTURE0);

    //! Bind the texture Id.
    glBindTexture(GL_TEXTURE_2D, mpTexture->textureId());

    //! Attempt to bind to the buffer.
    if( mBuffer.bind() == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Menu::render - buffer bind failed...";
        //! If bind failed release the shader program and return without rendering.
        mProgram.release();
        return;
    } // if( mBuffer.bind() == false )

    //! Track the data offset, the vertex data starts first at zero.
    int offset = 0;
    //! Set the attribute buffer to the vertex data.
    mProgram.setAttributeBuffer("vertex", GL_FLOAT, offset, 3, 0);
    //! Enable the attribute array for the vertices.
    mProgram.enableAttributeArray("vertex");

    //! Track the data offset, the texture coordinate data starts after the vertex data.
    offset += 4 * 3 * sizeof(GLfloat);
    //! Set the attribute buffer to the texture coordinate data.
    mProgram.setAttributeBuffer("textureCoordinate", GL_FLOAT, offset, 2, 0);
    //! Enable the attribute array for the texture cooridinates.
    mProgram.enableAttributeArray("textureCoordinate");

    //! Release the OpenGL memory buffer.
    mBuffer.release();

    //! Draw the Menu background Quad.
    glDrawArrays(GL_QUADS, 0, 4);

    //! Disable the attribute array for the vertices.
    mProgram.disableAttributeArray("vertex");
    //! Disable the attribute array for the texture cooridinates.
    mProgram.disableAttributeArray("textureCoordinate");

    //! Release the shader program.
    mProgram.release();
} // Menu::render( const Camera & camera )

//! Update the Menu.
//! \return void
//! \sa Menu, Engine, Game
void Menu::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Menu::tick()...";
} // Menu::tick()

//! Unload the Opengl vertex data, shader program, and texture image.
void Menu::unload()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Menu::unload()...";

    //! If texture memory is allocated.
    if( mpTexture != NULL )
    {
        //! release the allocated memory.
        delete mpTexture;
        //! set the memory pointer to null.
        mpTexture = NULL;
    } // if( mpTexture != NULL )

    //! Remove all shaders from the program.
    mProgram.removeAllShaders();

    //! Destroy the OpenGL buffer.
    mBuffer.destroy();
} // Menu::unload()
