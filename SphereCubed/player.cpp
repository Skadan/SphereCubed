/*! \file player.cpp
\brief Player class implementation file.
This file contains the Player class implementation for the application.
*/

// player.h header file required for class definition.
#include "player.h"

// QKeyEvent header file required for KeyPress Event.
#include <QKeyEvent>

// trace.h header file required for tracing execution.
#include "trace.h"

//! Trace file execution flag.
#define TRACE_FILE_EXECUTION false

//! The Player execution begins here.
//! Automatically called by the World during construction.
//! Passes parent argument onto base class constructor.
//! \param parent is a pointer the QWidget parent object.
Player::Player( QObject * parent ) :
    QObject( parent )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Player::Player( QObject * parent )...";    
} // Player::Player( QObject *parent )

//! The Player execution ends here.
//! Required to free the Player allocated memory.
//! Automatically called when the World is destructed.
Player::~Player()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Player::~Player()...";
} // Player::~Player()

//! Process received events.
//! \param event is a pointer to a QEvent argument identifing the event.
//! \return bool should be set true when processed.
//! \sa QObject, Window, Engine, Game
bool Player::event( QEvent * event )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Player::event( QEvent * event ): " << event->type() << "...";

    //! Return value, assume processed until otherwise determined.
    bool rVal = true;

    //! Determine what type of event was received.
    switch( event->type() )
    {
    //! When it is a KeyPress event type.
    case QEvent::KeyPress:
        //! Determine which key was pressed.
        switch( ((QKeyEvent*)event)->key() )
        {
        //! When it is the Down key.
        case Qt::Key_Down:
            //! Apply the force in the negative direction.
            mForce += -mDirection * mUserStrength;
            break;

        //! When it is the Left key.
        case Qt::Key_Left:
            //! Apply the force to the left of the direction.
            mForce += QVector3D( mDirection.z(), 0, (-1 * mDirection.x()) ) * mUserStrength;
            break;

        //! When it is the Right key.
        case Qt::Key_Right:
            //! Apply the force to the Right of the direction.
            mForce += QVector3D( (-1 * mDirection.z()), 0, mDirection.x() ) * mUserStrength;
            break;

        //! When it is the Up key.
        case Qt::Key_Up:
            //! Apply the force in the direction.
            mForce += mDirection * mUserStrength;
            break;

        //! When it is any other key.
        default:
            //! Dont process the key press event and set the return value to false.
            rVal = false;
            break;
        }
        break;

    //! When it is any other event.
    default:
        //! Dont process the event and set the return value to false.
        rVal = false;
        break;
    }

    //! Return status of event processing.
    return rVal;
} // Player::event( QEvent * event )

//! Load the Opengl resources for the Player.
//! \return void
void Player::load()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Player::load()...";

    //! Reset the player heading direction.
    mDirection = QVector3D(0,0,1);

    //! Reset the user force.
    mForce = QVector3D( 0.0f, 0.0f, 0.0f );

    //! reset the user velocity.
    mVelocity = QVector3D( 0.0f, 0.0f, 0.0f );

    //! Load the Sphere openGL resources.
    mSphere.load();
} // Player::load()

//! Render the Opengl resources for the Player.
//! \param camera is a const reference to Camera viewing the scene.
//! \param light is a const reference to the Light lightinh the scene.
//! \return void
void Player::render( const Camera & camera, const Light & light )
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Player::render( const Camera & camera, const Light & light )...";

    //! Render the Sphere.
    mSphere.render( camera, light );
} // Player::render( const Camera & camera, const Light & light )

//! Update the Player.
//! \return void
void Player::tick()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Player::tick()...";

    //! Update the Sphere position.
    mSphere.mPosition = mPosition;

    //! Update the Sphere rotation.
    mSphere.mRotation = mRotation;

    //! Update the Sphere.
    mSphere.tick();
} // Player::tick()

//! Unload the Player.
//! \return void
void Player::unload()
{
    TraceOut( TRACE_FILE_EXECUTION ) << "Player::unload()...";

    //! Unload the Sphere OpenGL resources.
    mSphere.unload();
} // Player::unload()
