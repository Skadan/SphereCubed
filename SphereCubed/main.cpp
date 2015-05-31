/*! \file main.cpp
\brief Application entry point file.
This file contains SphereCubed application entry point.
*/

// QApplication header required execution.
#include <QApplication>

// Window.h header required for the UI.
#include "window.h"


//! Execution begins here at the application entry point main function taking two argument and returning a status value.
/*!
\param argc argument count in argv.
\param argv argument vector.
\return int exit status
*/
int main( int argc, char * argv[] )
{
    //! Instantiate the QApplication object passing on the entry point arguments.
    QApplication app( argc, argv );

    //! Instantiate the Window object.
    Window win;

    //! Show the Sphere Cubed application window.
    win.show();

    //! Execute the application and return its status when it closes.
    return app.exec();
} // main( int argc, char * argv[] )
