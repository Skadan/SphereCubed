/*! \file light.h
\brief Light class declaration file.
This file contains the Light class declaration for the application.
*/

#ifndef LIGHT_H
#define LIGHT_H

// QColor header file required ambient, diffused, specular.
#include <QColor>
// QVector3D header file required position.
#include <QVector3D>

//! Light class.
/*!
This class declaration encapsulates the Light functionality.
*/
class Light
{
    //! Friend of World so that only World can instantiate the Light.
    friend class World;

public: // Member Functions
    //! Access function ambient Light color.
    //! \return QColor reference to ambient Light color.
    //! \sa Light
    QColor & ambient() { return mAmbientColor; }

    //! Const access function ambient Light color.
    //! \return QColor const reference to ambient Light color.
    //! \sa Light
    const QColor & ambient() const { return mAmbientColor; }

    //! Access function diffuse Light color.
    //! \return QColor reference to diffuse Light color.
    //! \sa Light
    QColor & diffuse() { return mDiffuseColor; }

    //! Const access function diffuse Light color.
    //! \return QColor const reference to diffuse Light color.
    //! \sa Light
    const QColor & diffuse() const { return mDiffuseColor; }

    //! Access function Light offset from its illumination target.
    //! \return QVector reference to Light offset from its illumination target.
    //! \sa Light
    QVector3D & offset() { return mOffset; }

    //! Const access function Light position.
    //! \return QVector Light position.
    //! \sa Light
    QVector3D position() const { return (mTarget + mOffset); }

    //! Access function specular Light color.
    //! \return QColor reference to specular Light color.
    //! \sa Light
    QColor & specular() { return mSpecularColor; }

    //! Const access function specular Light color.
    //! \return QColor const reference to specular Light color.
    //! \sa Light
    const QColor & specular() const { return mSpecularColor; }

    //! Access function Light illumination target position.
    //! \return QVector reference to Light illumination target position.
    //! \sa Light
    QVector3D & target() { return mTarget; }

private: // Constructors / Destructors
    //! Light class constructor.
    Light();

    //! Light class destructor.
    ~Light();

private: // Member Variables
    //! Used to store the ambient Light color, initialize to dark grey.
    QColor mAmbientColor = QColor(32,32,32);

    //! Used to store the diffuse Light color, initialize to medium grey.
    QColor mDiffuseColor = QColor(128,128,128);

    //! Used to store the Light offset from its illumination target, initialize to 10 units above the target.
    QVector3D mOffset = QVector3D(0,10,0);

    //! Used to store the specular Light color, initialize to white.
    QColor mSpecularColor = QColor(255,255,255);

    //! Used to store the Light illumination target position, initialize to the origin.
    QVector3D mTarget = QVector3D(0,0,0);
}; // class Light

#endif // LIGHT_H
