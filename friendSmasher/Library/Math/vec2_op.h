/**
 * Copyright (c) 2014-present, Facebook, Inc. All rights reserved.
 *
 * You are hereby granted a non-exclusive, worldwide, royalty-free license to use,
 * copy, modify, and distribute this software in source code or binary form for use
 * in connection with the web services and APIs provided by Facebook.
 *
 * As with any software that integrates with the Facebook platform, your use of
 * this software is subject to the Facebook Developer Principles and Policies
 * [http://developers.facebook.com/policy/]. This copyright notice shall be
 * included in all copies or substantial portions of the software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef FRIENDSMASHER_VEC2_OP
#define FRIENDSMASHER_VEC2_OP

#include <math.h>
#include "vec2.h"
#include "math_op.h"
#include "mtx44.h"
#include "mtx33.h"


namespace FriendSmasher
{
namespace Math
{

////////////////
// Vec2
////////////////

// Returns the Length of a given vector via Pythagoras
inline float vec2Length(const vec2& v1)
{
	float result = v1.x * v1.x;
	result += v1.y * v1.y;
	result = sqrt(result);
	return(result);
}


// Returns the squared Length of a given vector via Pythagoras
inline float vec2LengthSquared(const vec2& v1)
{
	float result = v1.x * v1.x;
	result += v1.y * v1.y;
	return(result);
}

// Normalizes a given vector.
inline void vec2Normalize(vec2& vResult, const vec2& v1)
{
	float norm = vec2Length(v1);
	float reciprocal = ( norm == 0.0f ) ? 1.0f : (1.0f /norm);
	vResult.x = v1.x * reciprocal;
	vResult.y = v1.y * reciprocal;
}

// Component adds two vectors together
inline void vec2Add(vec2& vResult, const vec2& v1, const vec2& v2)
{
	vResult.x = v1.x + v2.x;
	vResult.y = v1.y + v2.y;
}

// Component subtracts two vectors
inline void vec2Subtract(vec2& vResult, const vec2& v1, const vec2& v2)
{
	vResult.x = v1.x - v2.x;
	vResult.y = v1.y - v2.y;
}

// Component multiplies two vectors
inline void vec2Multiply(vec2& vResult, const vec2& v1, const vec2& v2)
{
	vResult.x = v1.x * v2.x;
	vResult.y = v1.y * v2.y;
}

// Component divides two vectors
inline void vec2Divide(vec2& vResult, const vec2& v1, const vec2& v2)
{
	vResult.x = v1.x / v2.x;
	vResult.y = v1.y / v2.y;
}

// Scales a vector magnitude by a float
inline void vec2Scale(vec2& vResult, const vec2& v1, const float s)
{
	vResult.x = v1.x * s;
	vResult.y = v1.y * s;
}

// Computes the dot product between two vectors
inline float vec2DotProduct(const vec2& v1, const vec2& v2)
{
    return (v1.x*v2.x) + (v1.y*v2.y);
}

inline vec2 vec2Transform( vec2& vResult, const mtx33& m, const vec2& v)
{
	vec2 a = v;
	vResult.x = (m(0,0) * a.x) + (m(1,0) * a.y) + m(2,0);
	vResult.y = (m(0,1) * a.x) + (m(1,1) * a.y) + m(2,1);

	return vResult;
}

inline vec2 vec2TransformNormal( vec2& vResult, const mtx33& m, const vec2& v )
{
	vec2 a = v;
	vResult.x = (m(0,0) * a.x) + (m(1,0) * a.y);
	vResult.y = (m(0,1) * a.x) + (m(1,1) * a.y);

	return vResult;
}

inline float vec2AngleBetweenRad(const vec2& v1, const vec2& v2)
{
	float angle = vec2DotProduct(v1, v2);
	angle = acos(angle);
	return angle;
}

inline float vec2AngleBetweenDeg(const vec2& v1, const vec2& v2)
{
	float angle = vec2DotProduct(v1, v2);
	angle = acos(angle);
	angle = RadiansToDegrees(angle);
	return angle;
}

// Computes the distance between two vectors
inline float vec2DistanceBetween(const vec2& v1, const vec2& v2)
{
	vec2 pushvec;
	vec2Subtract(pushvec, v1, v2);
	return vec2Length(pushvec);
}

// Computes a fast distance between two vectors
inline float vec2DistanceBetweenSquared(const vec2& v1, const vec2& v2)
{
	vec2 pushvec;
	vec2Subtract(pushvec, v1, v2);

	float result = pushvec.x * pushvec.x;
	result += pushvec.y * pushvec.y;
	return(result);
}
	
// Clamp vector to length
inline void vec2ClampLength(vec2& result, const vec2& v, float l)
{
	vec2 p = v;
	float length = vec2Length(p);
	if(length > 0.0f)
	{
		p.Normalise(length);
		length = length > l ? l : length;
		p = p * length;
		result = p;
	}
}

// Clamp components of a vector into the range 0-1
inline void vec2Clamp(vec2& vResult, const vec2& v1)
{
	vResult.x = (v1.x > 1.f) ? 1.f : v1.x;
	vResult.x = (v1.x < 0.f) ? 0.f : v1.x;
	vResult.y = (v1.y > 1.f) ? 1.f : v1.y;
	vResult.y = (v1.y < 0.f) ? 0.f : v1.y;
}

// Negates the components of a vector 
inline void vec2Negate(vec2& vResult, const vec2& v1)
{
	vResult.x = -v1.x;
	vResult.y = -v1.y;
}

inline void vec2Set(vec2& vResult, float x, float y)
{
	vResult.x = x;
	vResult.y = y;
}


inline void vec2Copy(vec2& vResult, vec2& v1)
{
	vResult.x = v1.x;
	vResult.y = v1.y;
}

inline void vec2NegateCopy(vec2& vResult, vec2& v1)
{
	vResult.x = -v1.x;
	vResult.y = -v1.y;
}


inline void vec2Absolute(vec2& vResult, vec2& v1)
{
	vResult.x = fabsf(v1.x);
	vResult.y = fabsf(v1.y);
}

inline void vec2Zero(vec2& vResult)
{
	vResult.x = 
	vResult.y = 0.f;
}

inline void vec2MidpointBetween(vec2& vResult, const vec2& v1, const vec2& v2)
{
	vResult.x = (v1.x + v2.x) * 0.5f;
	vResult.y = (v1.y + v2.y) * 0.5f;
}

inline void vec2MinComponents(vec2& vResult, vec2& v1, vec2& v2)
{
	vResult.x = FriendSmasher::Math::Min(v1.x, v2.x);
	vResult.y = FriendSmasher::Math::Min(v1.y, v2.y);
}

inline void vec2MaxComponents(vec2& vResult, vec2& v1, vec2& v2)
{
	vResult.x =  FriendSmasher::Math::Max(v1.x, v2.x);
	vResult.y =  FriendSmasher::Math::Max(v1.y, v2.y);
}

inline bool vec2Equal(const vec2& v1, const vec2& v2)
{
	return (v1.x == v2.x && v1.y == v2.y);
}

inline bool vec2ApproxEqual(const vec2& v1, const vec2& v2)
{
	return ( FriendSmasher::Math::approxf(v1.x, v2.x,  FriendSmasher::Math::kEPSILON) &&
			 FriendSmasher::Math::approxf(v1.y, v2.y,  FriendSmasher::Math::kEPSILON));
}

inline void vec2ToArray(float* arrayResult, const vec2& v1)
{
	arrayResult[0] = v1.x;
	arrayResult[1] = v1.y;
}

inline void vec2FromArray(vec2& vResult, const float* array1)
{
	vResult.x = array1[0];
	vResult.y = array1[1];
}

inline int vec2MinAxis(vec2& v1)
{
	return ((fabsf(v1.x) < fabsf(v1.y)) ? 0 : 1);
}

inline int vec2MaxAxis(vec2& v1)
{
	return ((fabsf(v1.x) > fabsf(v1.y)) ? 0 : 1);
}

inline void vec2ReflectScalar(vec2& vResult, const vec2& v1, const vec2& normal, float scalar)
{
	vec2  _2ndotvn;
	float dotVal = vec2DotProduct(v1, normal); 

	vec2Scale(_2ndotvn, normal, scalar * 2.f * dotVal);
	vec2Subtract(vResult, v1, _2ndotvn); 
}

inline void vec2Reflect(vec2& vResult, const vec2& v1, const vec2& normal)
{
	vec2ReflectScalar(vResult, v1, normal, 1.f);
}

inline void vec2ReflectPoint(vec2& vResult, const vec2& v, const vec2& p, const vec2& normal, float scalar = 1.0f)
{
	vec2 t = v - p;
	vec2 r;
	vec2ReflectScalar(r, t, normal, scalar);
	vResult = r + p;
}
	
inline void vec2RotateRadians(vec2& vResult, const vec2& v1, float ca, float sa)
{
	vResult = Math::vec2(ca*v1.x-sa*v1.y, sa*v1.x+ca*v1.y);
}

inline void vec2RotateRadians(vec2& vResult, const vec2& v1, float fTheta)
{
	float c, s;
	c = cosf(fTheta);
	s = sinf(fTheta);
	vec2RotateRadians(vResult, v1, c, s);
	//vResult = Math::vec2(c*v1.x-s*v1.y, s*v1.x+c*v1.y);
}

inline void vec2RotateDegrees(vec2& vResult, const vec2& v1, float fTheta)
{
	vec2RotateRadians(vResult, v1, fTheta * kDegreesToRadians);
}

inline void vec2RotatePoint(vec2& vResult, const vec2& v1, const vec2& pivot, float fTheta)
{
	vec2 v = v1 - pivot;
	vec2RotateRadians(vResult, v, fTheta);
	vResult += pivot;
}

inline void vec2ProjectPoint(vec2& vResult, const vec2& vIn, const vec2& vStart, const vec2& vEnd)
{
	vec2 l = vEnd - vStart;
	vec2 v = vIn - vStart;
	float dot = vec2DotProduct(l,v);
	vResult = vStart + l*dot;
}

inline void vec2Lerp(vec2& vResult, const vec2& v1, const vec2& v2, float t)
{
	vec2 v = v1 + (v2 - v1) * t;
	vResult = v;
}


}
}

#endif //  FriendSmasher_VEC2_OP