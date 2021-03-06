/*! \file light.cpp
\brief Light class implementation file.
This file contains the Light class implementation for the application.
*/

// light.h header file required for class definition.
#include "light.h"

// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION false

//! The Light execution begins here.
//! Create a Light.
Light::Light()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Light::Light()...";
} // Light::Light()

//! The Light execution ends here.
//! Destroy the Light.
//! Required to free the Light allocated memory.
Light::~Light()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Light::~Light()...";
} // Light::~Light()
