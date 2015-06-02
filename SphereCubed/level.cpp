/*! \file level.cpp
\brief Level class implementation file.
This file contains the Level class implementation for the application.
*/

// level.h header file required for class definition.
#include "level.h"

// QFile header file required for opening Level file.
#include <QFile>

// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION false

//! The Level execution begins here.
//! Creates components to load and render a Level.
//! Automatically called by the World during construction.
Level::Level()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Level::Level()...";

    //! Initialize the member variables
    mCubeCount  = 0;
    mCubeRows   = 0;
    mCubeCols   = 0;
    mpCube      = NULL;
    mLevelIndex = 0;
} // Level::Level()

//! The Level execution ends here.
//! Required to free the Level allocated memory.
//! Automatically called when Level unload.
Level::~Level()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Level::~Level()...";

    //! Make sure to unload all Level resources.
    unload();
} // Level::~Level()

//! Lookup what type of Cube is at the specified location.
//! \param column is the X position in the Level.
//! \param row is the Z position in the Level.
//! \return CubeType is the type of cube at the specified location.
Cube::CubeType Level::cubeType( uint column, uint row )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Level::cubeType( uint column, uint row )...";

    //! Assert to alert there is an issue if the specified location is NOT within the bounds of the Level.
    Q_ASSERT_X( column < mCubeCols, "Level::cubeType( uint column, uint row )","Column is out of bounds.");
    Q_ASSERT_X( row < mCubeRows,    "Level::cubeType( uint column, uint row )","Row is out of bounds.");

    //! Return the CubeType at the location.
    return mpCube[ row * mCubeCols + column ].mType;
} // Level::cubeType( uint column, uint row )

//! Load the Opengl resources for the Level.
//! \return void
void Level::load()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Level::load()...";

    //! Increment level index to next Level.
    mLevelIndex++;

    //! Create Level file name with Level index.
    QString name(":Level" + QString::number(mLevelIndex) );

    //! Create a file handle.
    QFile file( name );

    //! Verify the level file exists.
    if( file.exists() == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Level::Load( QString level ) - File does NOT exists - " << name <<"...";
        //! If it does not exists return without loading Level.
        return;
    } // if( file.exists() == false )

    //! Open the file for reading.
    if( file.open( QIODevice::ReadOnly ) == false )
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Level::Load( QString level ) - Unable to open Level file...";
        //! If it fails to open return without loading Level.
        return;
    } // if( file.open( QIODevice::ReadOnly ) == false )

    //! Create a text stream for the file parsing.
    QTextStream textStream(&file);

    //! Create a string list to load the rows of level text.
    QStringList levelRowStringList;

    //! Loop through the file.
    while (true)
    {
        //! Read a line from the file.
        QString line = textStream.readLine();
        //! Check the line read to see if it is the end of file.
        if (line.isNull())
        {
            //! If it is the end of file stop looping through file.
            break;
        } // if (line.isNull())
        else
        {
            //! If it is not the end of the file append the row to the list of row string.
            levelRowStringList.append(line);
        } // if (line.isNull()) else
    } // while (true)

    //! The file is loaded into memory.
    //! Close the file.
    file.close();

    //! Unpack the rows into cubes.
    QVector<QStringList> cubeData;
    foreach (QString string, levelRowStringList)
    {
        QStringList rowData = string.split(",");
        cubeData << rowData;
    } // foreach (QString string, levelRowStringList)

    //! Determine how many cubes are in the level.
    mCubeRows  = cubeData[0].length();
    mCubeCols  = cubeData.length();
    mCubeCount = mCubeRows * mCubeCols;

    //! Attempt to allocat the cubes for the level
    if( (mpCube = new Cube[mCubeCount]) == NULL)
    {
        TraceOut( TRACE_FILE_EXECUTION ) << "Level::Load( QString level ) - Unable to allocate memory for the Cube array...";
        //! If it fails to allocate memory return without loading Level.
        return;
    } // if( (mpCube = new Cube[mCubeCount]) == NULL)

    //! Load the Cube openGL resources.
    Cube::load();

    //! Configure the cubes

    //! Go through the rows of Cube.
    uint z = 0;
    foreach (QStringList sl, cubeData)
    {
        //! Go through each Cube in the row.
        uint x=0;
        foreach (QString s, sl)
        {
            //! Parse out the Type and height of the Cube.
            QStringList vals = s.split(":");

            //! Verify that the parse found 2 parts the Type and height.
            if( vals.length() != 2 )
            {
                TraceOut( TRACE_FILE_EXECUTION ) << "Level::Load( QString level ) - Parse Type and Height Failed.- Level file corrupt...";
                //! If parse failed Assert to alert of issue.
                Q_ASSERT_X( false, "Parse Type and Height Failed.", "Level File Corrupted." );
                //! Return without loading Level.
                return;
            } // if( vals.length() == 2 )

            //! Validate the Type and Height conversion from base 16 string to base 10 unsigned int.
            bool ok;
            uint type = vals[0].toInt(&ok,16);
            if( ok == false )
            {
                TraceOut( TRACE_FILE_EXECUTION ) << "Level::Load( QString level ) - Convert Type String Failed...";
                //! If convert failed Assert to alert of issue.
                Q_ASSERT_X( false, "Convert Type Failed.", "Level File Corrupted." );
                //! Return without loading Level.
                return;
            } // if( ok == false )

            uint height  = vals[1].toInt(&ok,16);
            if( ok == false )
            {
                TraceOut( TRACE_FILE_EXECUTION ) << "Level::Load( QString level ) - Convert Height String Failed...";
                //! If convert failed Assert to alert of issue.
                Q_ASSERT_X( false, "Convert Height Failed.", "Level File Corrupted." );
                //! Return without loading Level.
                return;
            } // if( ok == false )

            //! Set the Cube Position.
            mpCube[z*sl.length()+x].mPosition = QVector3D(x,height,z);
            //! Set the Cube Type.
            mpCube[z*sl.length()+x].mType = (Cube::CubeType)type;

            //! Check to see if the Cube is the Start position.
            if( type == Cube::START)
            {
                //! If it is the Start position, store that location.
                mStartPosition = QVector3D(x,height+1,z);
            } // if( type == Cube::START)
            x++; // Incrment the column index.
        } // oreach (QString s, sl)
        z++;  // Incrment the row index.
    } // foreach (QStringList sl, cubeData)

    //! Turn off hidden faces. Face that can never be seen such as between adjacent Cubes.

    //! Loop through the Level by row.
    for(z=0; z<mCubeRows; z++)
    {
        //! Loop through the Cubes in the row.
        for(uint x=0; x<mCubeCols; x++)
        {
            //! Track Which faces can be seen.
            //! Assume that the Top face is always visible and all other are not until proven otherwise.
            bool top    = true;
            bool bottom = false;
            bool left   = false;
            bool right  = false;
            bool front  = false;
            bool back   = false;

            //! If the cube is on the back row.
            if( z == 0 )
            {
                //! -Then the back face is visible.
                back = true;

                //! -If it is higher then the Cube in fornt of it.
                if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[x + (z + 1) * mCubeCols].mPosition.y() )
                {
                    //! --Then the front face is visible.
                    front = true;
                } // if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[x + (z + 1) * mCubeCols].mPosition.y() )
            } // if( z == 0 )
            //! If the cube is on the front row.
            else if( z == (mCubeRows - 1) )
            {
                //! -Then the front face is visible.
                front = true;

                //! -If it is higher then the Cube in behind it.
                if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[x + (z - 1) * mCubeCols].mPosition.y() )
                {
                    //! --Then the back face is visible.
                    back = true;
                } // if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[x + (z - 1) * mCubeCols].mPosition.y() )
            } // else if( z == (mCubeRows - 1) )
            //! Else the Cube is not on the front or back edge of the Level.
            else
            {
                //! -If it is higher then the Cube in behind it.
                if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[x + (z - 1) * mCubeCols].mPosition.y() )
                {
                    //! --Then the back face is visible.
                    back = true;
                } // if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[x + (z - 1) * mCubeCols].mPosition.y() )

                //! -If it is higher then the Cube in front of it.
                if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[x + (z + 1) * mCubeCols].mPosition.y() )
                {
                    //! --Then the front face is visible.
                    front = true;
                } // if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[x + (z + 1) * mCubeCols].mPosition.y() )

                //! -If the Cube behind is a HOLE cube..
                if( mpCube[x + (z - 1) * mCubeCols].mType == Cube::HOLE)
                {
                    //! --Then the back face is visible.
                    back = true;
                } // if( mpCube[x + (z - 1) * mCubeCols].type() == Cube::HOLE)

                //! -If the Cube in front is a HOLE cube..
                if( mpCube[x + (z + 1) * mCubeCols].mType == Cube::HOLE)
                {
                    //! --Then the front face is visible.
                    front = true;
                } // if( mpCube[x + (z + 1) * mCubeCols].type() == Cube::HOLE)
            } // if( z == 0 ) else if( z == (mCubeRows - 1) ) else

            //! If the cube is on the left row.
            if( x == 0 )
            {
                //! -Then the left face is visible.
                left = true;

                //! -If it is higher then the Cube to the right of it.
                if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[(x + 1) + z * mCubeCols].mPosition.y() )
                {
                    //! --Then the right face is visible.
                    right = true;
                } // if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[(x + 1) + z * mCubeCols].mPosition.y() )
            } // if( x == 0 )
            //! If the cube is on the right row.
            else if( x == (mCubeCols - 1) )
            {
                //! -Then the right face is visible.
                right = true;

                //! -If it is higher then the Cube to the left of it.
                if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[(x - 1) + z * mCubeCols].mPosition.y() )
                {
                    //! --Then the LEFT face is visible.
                    left = true;
                } // if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[(x - 1) + z * mCubeCols].mPosition.y() )
            } // else if( x == (mCubeCols - 1) )
            //! Else the Cube is not on the left or right edge of the Level.
            else
            {
                //! -If it is higher then the Cube to the left it.
                if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[(x - 1) + z * mCubeCols].mPosition.y() )
                {
                    //! --Then the left face is visible.
                    left = true;
                } // if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[(x - 1) + z * mCubeCols].mPosition.y() )

                //! -If it is higher then the Cube to the right it.
                if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[(x + 1) + z * mCubeCols].mPosition.y() )
                {
                    //! --Then the right face is visible.
                    right = true;
                } // if( mpCube[x + z * mCubeCols].mPosition.y() > mpCube[(x + 1) + z * mCubeCols].mPosition.y() )

                //! -If the Cube to the left is a HOLE Cube.
                if( mpCube[(x - 1) + z * mCubeCols].mType == Cube::HOLE)
                {
                    //! --Then the left face is visible.
                    left = true;
                } // if( mpCube[(x - 1) + z * mCubeCols].type() == Cube::HOLE)

                //! -If the Cube to the right is a HOLE Cube.
                if( mpCube[(x + 1) + z * mCubeCols].mType == Cube::HOLE)
                {
                    //! --Then the right face is visible.
                    right = true;
                } // if( mpCube[(x + 1) + z * mCubeCols].type() == Cube::HOLE)
            } // if( x == 0 ) else if( x == (mCubeCols - 1) ) else

            //! If the Cube is higher then 1.
            if( mpCube[x + z * mCubeCols].mPosition.y() > 1 )
            {
                //! Assume the bottom is visible.
                bottom = true;
            } // if( mpCube[x + z * mCubeCols].mPosition.y() > 1 )

            //! Set the Cube Face visibility as has been determined.
            mpCube[x + z * mCubeCols].mFace[ Cube::CubeFace::TOP    ] = top;
            mpCube[x + z * mCubeCols].mFace[ Cube::CubeFace::BOTTOM ] = bottom;
            mpCube[x + z * mCubeCols].mFace[ Cube::CubeFace::LEFT   ] = left;
            mpCube[x + z * mCubeCols].mFace[ Cube::CubeFace::RIGHT  ] = right;
            mpCube[x + z * mCubeCols].mFace[ Cube::CubeFace::NEAR   ] = front;
            mpCube[x + z * mCubeCols].mFace[ Cube::CubeFace::FAR    ] = back;
        } // for(uint x=0; x<mCubeCols; x++)
    } // for(z=0; z<mCubeRows; z++)
} // Level::load()

//! Render the Opengl resources for the Level.
//! \param camera is a const reference to Camera viewing the scene.
//! \param light is a const reference to the Light lightinh the scene.
//! \return void
void Level::render( const Camera & camera, const Light & light )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Level::render( const Camera & camera, const Light & light )...";

    //! Verify the Cube array pointer is valid.
    if( mpCube != NULL )
    {
        //! Loop through all the Cube in the Level.
        for(uint i=0; i<mCubeCount; i++)
        {
            //! Render the Cube.
            mpCube[i].render( camera, light );
        } // for(uint i=0; i<mCubeCount; i++)
    } // if( mpCube != NULL )
} // Level::render( const Camera & camera, const Light & light )

//! Update the Level.
//! \return void
void Level::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Level::tick()...";

    //! Verify the Cube array pointer is valid.
    if( mpCube != NULL )
    {
        //! Loop through all the Cube in the Level.
        for(uint i=0; i<mCubeCount; i++)
        {
            //! Update the Cube.
            mpCube[i].tick();
        } // for(uint i=0; i<mCubeCount; i++)
    } // if( mpCube != NULL )
} // Level::tick()

//! Unload the Level.
//! \return void
void Level::unload()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Level::unload()...";

    //! Unload the Cube OpenGL resources.
    Cube::unload();

    //! If the Cube array pointer is valid.
    if( mpCube != NULL )
    {
        //! Release the allocated memory.
        delete [] mpCube;
        //! Set the Cube array pointer to NULL.
        mpCube = NULL;
    } // if( mpCube != NULL )
} // Level::unload()
