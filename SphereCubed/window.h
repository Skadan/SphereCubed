/*! \file window.h
\brief Window class declaration file.
This file contains the Window class declaration for the application.
*/

#ifndef WINDOW_H
#define WINDOW_H

// QEvent header file required for events.
#include <QEvent>

// QGLWidget header file required for the OpenGL window base class.
#include <QGLWidget>

//! Window class.
/*!
This class declaration encapsulates the Window functionality.
The application instantiates one Window to display the Graphic User Interface (GUI).
*/
class Window : public QGLWidget
{
    // Q_OBJECT macro required to use services provided by Qt's meta-object system.
    Q_OBJECT

public: // Constructors / Destructors
    //! Window class constructor.
    Window( QWidget * parent = 0 );

    //! Window class destructor.
    ~Window();

public: // Member Function
    //! Access function for property that holds the recommended size for the widget.
    QSize sizeHint() const;

protected: // Member Function
    //! Triggered when an event is received.
    virtual bool event( QEvent * event );

    //! Triggered once before the first call to paintGL or resizeGL.
    virtual void initializeGL();

    //! Triggered whenever the Window needs to be painted.
    virtual void paintGL();

    //! Triggered whenever the Window has been resized.
    virtual void resizeGL( int width, int height );
}; // Window

#endif // WINDOW_H
