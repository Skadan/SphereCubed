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

//! Check to see if there is a physical Cube at the position.
//! \note Cube of type Hole are not considered a physical Cube.
//! \return bool is true if there is a physical Cube at the position.
bool Level::cubeAtPosition( int x, int y, int z )
{
    //! If Cube array is not loaded.
    if( mpCube == NULL )
    {
        //! -Return false no Cube at the position.
        return false;
    } // if( mpCube == NULL )

    //! If either of the horizontal position values is negative.
    if( x < 0 || z < 0 )
    {
        //! -Return false no Cubes exists in the negative region.
        return false;
    } // if( x < 0 || z < 0 )

    //! If either of the horizontal position values is out of range.
    if( (uint)x >= mCubeCols || (uint)z > mCubeRows )
    {
        //! -Return false no Cubes exists in the out of range region.
        return false;
    } // if( x >= mCubeCols || z > mCubeRows )

    //! If the Cube at that position is of type Hole.
    if( mpCube[ x + z * mCubeCols ].mType == Cube::HOLE )
    {
        //! -Return false effectively no physical Cube exists at that position.
        return false;
    } // if( mpCube[ x + z * mCubeCols ].mType == Cube::HOLE )

    //! If the Y value of the Cube is different then the Y specified.
    if( mpCube[ x + z * mCubeCols ].mPosition.y() != y )
    {
        //! -Return false the cube is at a different height.
        return false;
    } // if( mpCube[ x + z * mCubeCols ].mPosition.y() != y )

    //! Return true there is a cube at the position.
    return true;
} // Level::cubeAtPosition( int x, int y, int z )

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

//! Check to see if the position is inside Level.
//! \param position is value to check if it is inside the Level.
//! \return bool is true if position is with in the limits of the Level.
bool Level::insideLevel( const QVector3D & position )
{
    //! If the position Y is inside the Level.
    if( position.y() >= -0.5f )
    {
        //! -If position X is inside the Level.
        if( position.x() >= -0.5f && position.x() <= (mCubeCols + 0.5) )
        {
            //! --If the position Z is inside the Level.
            if( position.x() >= -0.5f && position.x() <= (mCubeCols + 0.5) )
            {
                //! ---Return true position is inside the Level.
                return true;
            } // if( position.x() >= -0.5f && position.x() <= (mCubeCols + 0.5) )
        } // if( position.x() >= -0.5f && position.x() <= (mCubeCols + 0.5) )
    } // if( position.y() >= -0.5f )

    //! Return false the position is outside the Level.
    return false;
} // Level::insideLevelCube( const QVector3D & position )

//! Load the Opengl resources for the Level.
//! \return void
void Level::load()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Level::load()...";

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

            //! If the cube is a Hole type.
            if( mpCube[x + z * mCubeCols].mType == Cube::CubeType::HOLE )
            {
                //! All faces are invisible.
                top = false;
            } // if( mpCube[x + z * mCubeCols].mType == Cube::CubeType::HOLE )
            //! Determine what faces could be visible.
            else
            {
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
            }

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

//! Check to see if the Player is directly on top of a physical Cube.
//! \param position is the Player position.
//! \return bool is true if player is on a Cube.
bool Level::playerOnACube( const QVector3D & position )
{
    //! Calculate the center of the Cube the position would be in.
    int cubeX = qRound( position.x() );
    int cubeY = qRound( position.y() );
    int cubeZ = qRound( position.z() );

    //! If the Player is out of the Level.
    if( insideLevel( QVector3D( cubeX, cubeY, cubeZ ) ) == false )
    {
        //! -Return false player is outside Level.
        return false;
    } // if( insideLevelCube( QVector3D( cubeX, cubeY, cubeZ ) ) == false )

    //! If the Player is NOT on a plane.
    if( playerOnPlane( position ) == false )
    {
        //! -Return false Player is not on a plane.
        return false;
    } // if( playerOnPlane( position ) == false )

    //! If there is a cube directly below the Player position.
    if( cubeAtPosition( cubeX, (cubeY - 1), cubeZ ) == true )
    {
        //! -Return true the player is on the plane and there is a cube directly below.
        return true;
    } // if( cubeAtPosition( cubeX, (cubeY - 1), cubeZ ) == true )

    //! Calculate the distance from the center of the Cube to the Player position
    float x = cubeX - position.x();
    float z = cubeZ - position.z();

    //! Calculate if the Player is on a edge of the Cube.
    bool left  = qFuzzyCompare( x,  0.5f );
    bool right = qFuzzyCompare( x, -0.5f );
    bool far   = qFuzzyCompare( z,  0.5f );
    bool near  = qFuzzyCompare( z, -0.5f );

    //! Determine if the Player is on a corner of the Cube.

    //! If the Player position is on the far left corner of the Cube.
    if( ( far == true ) && ( left == true ) )
    {
        //! -If there is a cube directly below the Player position.
        if( cubeAtPosition( ( cubeX - 1 ), ( cubeY - 1 ), ( cubeZ - 1 ) ) == true )
        {
            //! --Return true there is a Cube directly below at the far left corner.
            return true;
        } // if( cubeAtPosition( ( cubeX - 1 ), ( cubeY - 1 ), ( cubeZ - 1 ) ) == true )
    } // if( ( far == true ) && ( left == true ) )

    //! If the Player position is on the far right corner of the Cube.
    if( ( far == true ) && ( right == true ) )
    {
        //! -If there is a cube directly below the Player position.
        if( cubeAtPosition( ( cubeX + 1 ), ( cubeY - 1 ), ( cubeZ - 1 ) ) == true )
        {
            //! --Return true there is a Cube directly below at the far right corner.
            return true;
        } // if( cubeAtPosition( ( cubeX + 1 ), ( cubeY - 1 ), ( cubeZ - 1 ) ) == true )
    } // if( ( far == true ) && ( right == true ) )

    //! If the Player position is on the near left corner of the Cube.
    if( ( near == true ) && ( left == true ) )
    {
        //! -If there is a cube directly below the Player position.
        if( cubeAtPosition( ( cubeX - 1 ), ( cubeY - 1 ), ( cubeZ + 1 ) ) == true )
        {
            //! --Return true there is a Cube directly below at the near left corner.
            return true;
        } // if( cubeAtPosition( ( cubeX - 1 ), ( cubeY - 1 ), ( cubeZ + 1 ) ) == true )
    } // if( ( near == true ) && ( left == true ) )

    //! If the Player position is on the near right corner of the Cube.
    if( ( near == true ) && ( right == true ) )
    {
        //! -If there is a cube directly below the Player position.
        if( cubeAtPosition( ( cubeX + 1 ), ( cubeY - 1 ), ( cubeZ + 1 ) ) == true )
        {
            //! --Return true there is a Cube directly below at the near right corner.
            return true;
        } // if( cubeAtPosition( ( cubeX + 1 ), ( cubeY - 1 ), ( cubeZ + 1 ) ) == true )
    } // if( ( bottom == true ) && ( right == true ) )

    //! Determine if the Player is on a edge of the Cube.

    //! If the Player is on the left edge of the Cube.
    if( left == true )
    {
        //! -If there is a cube directly below the Player position.
        if( cubeAtPosition( ( cubeX - 1 ), ( cubeY - 1 ), cubeZ ) == true )
        {
            //! --Return true there is a Cube directly below at the left edge.
            return true;
        } // if( cubeAtPosition( ( cubeX - 1 ), ( cubeY - 1 ), cubeZ ) == true )
    } // if( left == true )

    //! If the Player is on the right edge of the Cube.
    if( right == true )
    {
        //! -If there is a cube directly below the Player position.
        if( cubeAtPosition( ( cubeX + 1 ), ( cubeY - 1 ), cubeZ ) == true )
        {
            //! --Return true there is a Cube directly below at the right edge.
            return true;
        } // if( cubeAtPosition( ( cubeX + 1 ), ( cubeY - 1 ), cubeZ ) == true )
    } // if( right == true )

    //! If the Player is on the far edge of the Cube.
    if( far == true )
    {
        //! -If there is a cube directly below the Player position.
        if( cubeAtPosition( cubeX, ( cubeY - 1 ), ( cubeZ - 1 ) ) == true )
        {
            //! --Return true there is a Cube directly below at the far edge.
            return true;
        } // if( cubeAtPosition( cubeX, ( cubeY - 1 ), ( cubeZ - 1 ) ) == true )
    } // if( far == true )

    //! If the Player is on the near edge of the Cube.
    if( near == true )
    {
        //! -If there is a cube directly below the Player position.
        if( cubeAtPosition( cubeX, ( cubeY - 1 ), ( cubeZ + 1 ) ) == true )
        {
            //! --Return true there is a Cube directly below at the near edge.
            return true;
        } // if( cubeAtPosition( cubeX, ( cubeY - 1 ), ( cubeZ + 1 ) ) == true )
    } // if( near == true )

    //! Return false no Cube below Player position.
    return false;
} // Level::playerOnACube(const QVector3D &position)

//! Check to see if the Player is at an interger height.
//! \param position is the Player position to test.
//! \return bool is true if position is on a plane, an interger height.
bool Level::playerOnPlane( const QVector3D & position )
{
    //! Calculate the distance to the plane.
    float planeDistance = position.y() - qRound(position.y());

    //! If the distance equals zero.
    if( qFuzzyCompare( 1 + planeDistance, 1 + 0.0f ) == true )
    {
        //! -Return true Player is on a plane.
        return true;
    } // if( qFuzzyCompare( 1 + planeDistance, 1 + 0.0f ) == true )

    //! Return false Player is Not on a plane.
    return false;
} // Level::playerOnPlane( const QVector3D& position )

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
