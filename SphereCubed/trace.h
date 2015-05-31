/*! \file trace.h
\brief TraceOut execution macro file.
This file contains the TraceOut macro declaration for Sphere Cubed.
*/

#ifndef TRACE
#define TRACE

// QtDebug header file required for debug trace output.
#include <QtDebug>

//! Debug Trace output of execution macro.
/*!
Outputs text to Debugger in debug builds.
Individual code traces can enable/disable output via the TraceExecution flag.
*/
#define TraceOut( TraceExecution ) if( TraceExecution ) qDebug()

#endif // TRACE
