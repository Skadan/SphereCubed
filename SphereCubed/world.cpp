/*! \file world.cpp
\brief World class implementation file.
This file contains the World class implementation for the application.
*/

// world.h header file required for World class definition.
#include "world.h"

// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION false

//! The World execution begins here.
//! Create a World.
//! Automatically called when the Engine is constructed.
World::World()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "World::World()...";

    //! Initialize the member variables.
    mDied = false;
    mFinished = false;
    mLastLevel = false;
    mLastLife = false;
    mReturnKeyPressed = false;
    mSpaceKeyPressed = false;
} // World::World()

//! The World execution ends here.
//! Destroy the World.
//! Automatically called when the Engine is destructed.
World::~World()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "World::~World()...";
}  // World::~World()
