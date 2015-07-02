/*
 This software is provided 'as-is', without any express or implied
 warranty.  In no event will the authors be held liable for any damages
 arisinfg from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.

 \file Mat4f.cpp

 Created on: Feb 19, 2015
 \author     fkzey
 */

#include "Mat4f.hpp"

#include "Vec2f.hpp"
#include "Vec3f.hpp"
#include "Quaternion.hpp"
#include "../GeekMathConstants.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>

static const unsigned int MATRIX_SIZE = sizeof( float ) * 16;

Mat4f::Mat4f( void )
{
  memset( m_Values, 0.0f, MATRIX_SIZE );
}

Mat4f::Mat4f( const Mat4f& matrix )
{
  memcpy( m_Values, matrix.m_Values, MATRIX_SIZE );
}

Mat4f::~Mat4f( void ) { return; }

Mat4f
Mat4f::Scale( const Vec3f& scale )
{
  return Scale( scale[ 0 ], scale[ 1 ], scale[ 2 ] );
}

Mat4f
Mat4f::Scale( const float& x, const float& y, const float& z )
{
  m_Values[0][0] = x;    m_Values[0][1] = 0.0f; m_Values[0][2] = 0.0f; m_Values[0][3] = 0.0f;
  m_Values[1][0] = 0.0f; m_Values[1][1] = y;    m_Values[1][2] = 0.0f; m_Values[1][3] = 0.0f;
  m_Values[2][0] = 0.0f; m_Values[2][1] = 0.0f; m_Values[2][2] = z;    m_Values[2][3] = 0.0f;
  m_Values[3][0] = 0.0f; m_Values[3][1] = 0.0f; m_Values[3][2] = 0.0f; m_Values[3][3] = 1.0f;

  return *this;
}

Mat4f
Mat4f::Identity( void )
{
  m_Values[ 0 ][ 0 ] = 1.0f;
  m_Values[ 1 ][ 1 ] = 1.0f;
  m_Values[ 2 ][ 2 ] = 1.0f;
  m_Values[ 3 ][ 3 ] = 1.0f;

  return *this;
}

Mat4f
Mat4f::Rotation( const Quaternion& quaternion )
{
  Vec3f n = Vec3f(        2.0f * (quaternion.X() * quaternion.Z() - quaternion.W() * quaternion.Y()),
                          2.0f * (quaternion.Y() * quaternion.Z() + quaternion.W() * quaternion.X()),
                   1.0f - 2.0f * (quaternion.X() * quaternion.X() + quaternion.Y() * quaternion.Y()));

  Vec3f v = Vec3f(        2.0f * (quaternion.X() * quaternion.Y() + quaternion.W() * quaternion.Z()),
                   1.0f - 2.0f * (quaternion.X() * quaternion.X() + quaternion.Z() * quaternion.Z()),
                          2.0f * (quaternion.Y() * quaternion.Z() - quaternion.W() * quaternion.X()));

  Vec3f u = Vec3f( 1.0f - 2.0f * (quaternion.Y() * quaternion.Y() + quaternion.Z() * quaternion.Z()),
                          2.0f * (quaternion.X() * quaternion.Y() - quaternion.W() * quaternion.Z()),
                          2.0f * (quaternion.X() * quaternion.Z() + quaternion.W() * quaternion.Y()));

  return Rotation( n, v, u );
}

Mat4f
Mat4f::Rotation( const Vec3f& forward, const Vec3f& up )
{
  Vec3f n = forward.Normalize();
  Vec3f u = Vec3f( up.Normalize() ).Cross( n );
  Vec3f v = n.Cross(u);

  m_Values[ 0 ][ 0 ] = u.X(); m_Values[ 0 ][ 1 ] = u.Y(); m_Values[ 0 ][ 2 ] = u.Z(); m_Values[ 0 ][ 3 ] = 0.0f;
  m_Values[ 1 ][ 0 ] = v.X(); m_Values[ 1 ][ 1 ] = v.Y(); m_Values[ 1 ][ 2 ] = v.Z(); m_Values[ 1 ][ 3 ] = 0.0f;
  m_Values[ 2 ][ 0 ] = n.X(); m_Values[ 2 ][ 1 ] = n.Y(); m_Values[ 2 ][ 2 ] = n.Z(); m_Values[ 2 ][ 3 ] = 0.0f;
  m_Values[ 3 ][ 0 ] = 0.0f;  m_Values[ 3 ][ 1 ] = 0.0f;  m_Values[ 3 ][ 2 ] = 0.0f;  m_Values[ 3 ][ 3 ] = 1.0f;

  return *this;
}

Mat4f
Mat4f::Rotation( const float x, const float y, const float z )
{
  Mat4f rx, ry, rz;

  rx.m_Values[0][0] = 1.0f;     rx.m_Values[1][0] = 0.0f     ; rx.m_Values[2][0] =  0.0f     ; rx.m_Values[3][0] = 0.0f;
  rx.m_Values[0][1] = 0.0f;     rx.m_Values[1][1] = cosf( x ); rx.m_Values[2][1] = -sinf( x ); rx.m_Values[3][1] = 0.0f;
  rx.m_Values[0][2] = 0.0f;     rx.m_Values[1][2] = sinf( x ); rx.m_Values[2][2] =  cosf( x ); rx.m_Values[3][2] = 0.0f;
  rx.m_Values[0][3] = 0.0f;     rx.m_Values[1][3] = 0.0f     ; rx.m_Values[2][3] =  0.0f     ; rx.m_Values[3][3] = 1.0f;

  ry.m_Values[0][0] = cosf( y ); ry.m_Values[1][0] = 0.0f;     ry.m_Values[2][0] = -sinf( y ); ry.m_Values[3][0] = 0.0f;
  ry.m_Values[0][1] = 0.0f     ; ry.m_Values[1][1] = 1.0f;     ry.m_Values[2][1] =  0.0f     ; ry.m_Values[3][1] = 0.0f;
  ry.m_Values[0][2] = sinf( y ); ry.m_Values[1][2] = 0.0f;     ry.m_Values[2][2] =  cosf( y ); ry.m_Values[3][2] = 0.0f;
  ry.m_Values[0][3] = 0.0f     ; ry.m_Values[1][3] = 0.0f;     ry.m_Values[2][3] =  0.0f     ; ry.m_Values[3][3] = 1.0f;

  rz.m_Values[0][0] = cosf( z ); rz.m_Values[1][0] = -sinf( z ); rz.m_Values[2][0] = 0.0f;     rz.m_Values[3][0] = 0.0f;
  rz.m_Values[0][1] = sinf( z ); rz.m_Values[1][1] =  cosf( z ); rz.m_Values[2][1] = 0.0f;     rz.m_Values[3][1] = 0.0f;
  rz.m_Values[0][2] = 0.0f     ; rz.m_Values[1][2] =  0.0f     ; rz.m_Values[2][2] = 1.0f;     rz.m_Values[3][2] = 0.0f;
  rz.m_Values[0][3] = 0.0f     ; rz.m_Values[1][3] =  0.0f     ; rz.m_Values[2][3] = 0.0f;     rz.m_Values[3][3] = 1.0f;

  *this = rz * ry * rx;

  return *this;
}

Mat4f
Mat4f::Rotation( const Vec3f& n, const Vec3f& v, const Vec3f& u )
{
  m_Values[ 0 ][ 0 ] = u.X(); m_Values[ 0 ][ 1 ] = u.Y(); m_Values[ 0 ][ 2 ] = u.Z(); m_Values[ 0 ][ 3 ] = 0.0f;
  m_Values[ 1 ][ 0 ] = v.X(); m_Values[ 1 ][ 1 ] = v.Y(); m_Values[ 1 ][ 2 ] = v.Z(); m_Values[ 1 ][ 3 ] = 0.0f;
  m_Values[ 2 ][ 0 ] = n.X(); m_Values[ 2 ][ 1 ] = n.Y(); m_Values[ 2 ][ 2 ] = n.Z(); m_Values[ 2 ][ 3 ] = 0.0f;
  m_Values[ 3 ][ 0 ] = 0.0f;  m_Values[ 3 ][ 1 ] = 0.0f;  m_Values[ 3 ][ 2 ] = 0.0f;  m_Values[ 3 ][ 3 ] = 1.0f;

  return *this;
}

Vec3f
Mat4f::Transformation( const Vec3f& vector ) const
{
  return ( Transformation( Quaternion( vector ) ) ).XYZ();
}

Quaternion
Mat4f::Transformation( const Quaternion& vector ) const
{
  Quaternion result( 0, 0, 0, 0 );

  for(unsigned int i = 0; i < 4; i++)
    for(unsigned int j = 0; j < 4; j++)
      result[i] += m_Values[j][i] * vector[j];

  return result;
}

Mat4f
Mat4f::Transpose( void ) const
{
  Mat4f t;

  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      t.m_Values[i][j] = m_Values[j][i];

  return t;
}


Mat4f
Mat4f::Translation( const float& x, const float& y, const float& z )
{
  m_Values[ 0 ][ 0 ] = 1.0f; m_Values[ 0 ][ 1 ] = 0.0f; m_Values[ 0 ][ 2 ] = 0.0f; m_Values[ 0 ][ 3 ] = x;
  m_Values[ 1 ][ 0 ] = 0.0f; m_Values[ 1 ][ 1 ] = 1.0f; m_Values[ 1 ][ 2 ] = 0.0f; m_Values[ 1 ][ 3 ] = y;
  m_Values[ 2 ][ 0 ] = 0.0f; m_Values[ 2 ][ 1 ] = 0.0f; m_Values[ 2 ][ 2 ] = 1.0f; m_Values[ 2 ][ 3 ] = z;
  m_Values[ 3 ][ 0 ] = 0.0f; m_Values[ 3 ][ 1 ] = 0.0f; m_Values[ 3 ][ 2 ] = 0.0f; m_Values[ 3 ][ 3 ] = 1.0f;

  return *this;
}

Mat4f
Mat4f::Translation( const Vec3f& t )
{
  return Translation( t[ 0 ], t[ 1 ], t[ 2 ] );
}

float cotf( float value )
{
  return tanf( M_PI_2 - value );
}

Mat4f
Mat4f::Perspective( const float FoV, const float Aspect, const float zNear, const float zFar )
{
  const float f   = cotf( DEG2RAD( FoV ) / 2.0f );
  const float tz  = -( zFar - zNear ) / ( zNear - zFar );
  const float z   = ( 2.0f * zFar * zNear ) / ( zNear - zFar );

  m_Values[0][0] = f * Aspect;  m_Values[0][1] = 0.0f; m_Values[0][2] =  0.0f; m_Values[0][3] = 0.0f;
  m_Values[1][0] = 0.0f;        m_Values[1][1] = f;    m_Values[1][2] =  0.0f; m_Values[1][3] = 0.0f;
  m_Values[2][0] = 0.0f;        m_Values[2][1] = 0.0f; m_Values[2][2] =  tz;   m_Values[2][3] = z;
  m_Values[3][0] = 0.0f;        m_Values[3][1] = 0.0f; m_Values[3][2] = 1.0f; m_Values[3][3] = 0.0f;

  return *this;
}

Mat4f
Mat4f::Orthographic( const float Left, const float Right,
                     const float Bottom, const float Top,
                     const float zNear, const float zFar )
{
  if ( zNear >= zFar ) throw "ERROR: zNear must be less than zFar.";

  const float width   = Right - Left;
  const float height  = Top - Bottom;
  const float depth   = zFar - zNear;

  m_Values[0][0] = 2.0f / width; m_Values[0][1] = 0.0f;          m_Values[0][2] =  0.0f;         m_Values[0][3] = -( ( Right + Left ) / width );
  m_Values[1][0] = 0.0f;         m_Values[1][1] = 2.0f / height; m_Values[1][2] =  0.0f;         m_Values[1][3] = -( ( Top + Bottom ) / height );
  m_Values[2][0] = 0.0f;         m_Values[2][1] = 0.0f;          m_Values[2][2] = -2.0f / depth; m_Values[2][3] = -( ( zFar + zNear ) / depth );
  m_Values[3][0] = 0.0f;         m_Values[3][1] = 0.0f;          m_Values[3][2] =  0.0f;         m_Values[3][3] =  1.0f;

  return *this;
}

Mat4f
Mat4f::operator *( const Mat4f& factor ) const
{
  Mat4f result;

  for ( unsigned int row = 0 ; row < 4 ; row++ )
    for ( unsigned int col = 0 ; col < 4; col++ )
      result.m_Values[ row ][ col ] = m_Values[ row ][ 0 ] * factor.m_Values[ 0 ][ col ] +
                                      m_Values[ row ][ 1 ] * factor.m_Values[ 1 ][ col ] +
                                      m_Values[ row ][ 2 ] * factor.m_Values[ 2 ][ col ] +
                                      m_Values[ row ][ 3 ] * factor.m_Values[ 3 ][ col ];

  return result;
}

Quaternion
Mat4f::operator *( const Quaternion& q ) const
{
  return Quaternion(
    m_Values[0][0] * q.X() + m_Values[0][1] * q.Y() + m_Values[0][2] * q.Z() + m_Values[0][3] * q.W(),
    m_Values[1][0] * q.X() + m_Values[1][1] * q.Y() + m_Values[1][2] * q.Z() + m_Values[1][3] * q.W(),
    m_Values[2][0] * q.X() + m_Values[2][1] * q.Y() + m_Values[2][2] * q.Z() + m_Values[2][3] * q.W(),
    m_Values[3][0] * q.X() + m_Values[3][1] * q.Y() + m_Values[3][2] * q.Z() + m_Values[3][3] * q.W()
  );
}

float*
Mat4f::operator []( unsigned int row )
{
  return m_Values[ row ];
}

const float*
Mat4f::operator []( unsigned int row ) const
{
  return m_Values[ row ];
}

const float*
Mat4f::Values( void ) const
{
  return *m_Values;
}

void
Mat4f::Set( const unsigned int row, const unsigned int col, const float value )
{
  m_Values[ row ][ col ] = value;
}

void
Mat4f::Print( void ) const
{
  printf( "( %f | %f | %f | %f )\n", m_Values[ 0 ][ 0 ], m_Values[ 0 ][ 1 ], m_Values[ 0 ][ 2 ], m_Values[ 0 ][ 3 ] );
  printf( "( %f | %f | %f | %f )\n", m_Values[ 1 ][ 0 ], m_Values[ 1 ][ 1 ], m_Values[ 1 ][ 2 ], m_Values[ 1 ][ 3 ] );
  printf( "( %f | %f | %f | %f )\n", m_Values[ 2 ][ 0 ], m_Values[ 2 ][ 1 ], m_Values[ 2 ][ 2 ], m_Values[ 2 ][ 3 ] );
  printf( "( %f | %f | %f | %f )\n", m_Values[ 3 ][ 0 ], m_Values[ 3 ][ 1 ], m_Values[ 3 ][ 2 ], m_Values[ 3 ][ 3 ] );
}
