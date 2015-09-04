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

#ifndef FRIENDSMASHER_VEC3_OP
#define FRIENDSMASHER_VEC3_OP

#include <math.h>
#include "vec3.h"
#include "math_op.h"

#include "mtx44.h"

////////////////
// Vec3
////////////////

namespace FriendSmasher
{
namespace Math
{

	// Returns the Length of a given vector via Pythagoras
	inline float vec3Length(const vec3& v1)
	{
		float result = v1.x * v1.x;
		result += v1.y * v1.y;
		result += v1.z * v1.z;
		result = sqrt(result);
		return(result);
	}

	// Returns the squared Length of a given vector via Pythagoras
	inline float vec3LengthSquared(const vec3& v1)
	{
		float result = v1.x * v1.x;
		result += v1.y * v1.y;
		result += v1.z * v1.z;
		return(result);
	}

	// Normalizes a given vector.
	inline void vec3Normalize(vec3& vResult, const vec3& v1)
	{
		float norm = vec3Length(v1);
		float reciprocal = ( norm == 0.0f ) ? 1.0f : (1.0f /norm);
		vResult.x = v1.x * reciprocal;
		vResult.y = v1.y * reciprocal;
		vResult.z = v1.z * reciprocal;
	}

	// Component adds two vectors together
	inline void vec3Add(vec3& vResult, const vec3& v1, const vec3& v2)
	{
		vResult.x = v1.x + v2.x;
		vResult.y = v1.y + v2.y;
		vResult.z = v1.z + v2.z;
	}

	// Component subtracts two vectors
	inline void vec3Subtract(vec3& vResult, const vec3& v1, const vec3& v2)
	{
		vResult.x = v1.x - v2.x;
		vResult.y = v1.y - v2.y;
		vResult.z = v1.z - v2.z;
	}

	// Component multiplies two vectors
	inline void vec3Multiply(vec3& vResult, const vec3& v1, const vec3& v2)
	{
		vResult.x = v1.x * v2.x;
		vResult.y = v1.y * v2.y;
		vResult.z = v1.z * v2.z;
	}

	// Component divides two vectors
	inline void vec3Divide(vec3& vResult, const vec3& v1, const vec3& v2)
	{
		vResult.x = v1.x / v2.x;
		vResult.y = v1.y / v2.y;
		vResult.z = v1.z / v2.z;
	}

	// Scales a vector magnitude by a float
	inline void vec3Scale(vec3& vResult, const vec3& v1, const float s)
	{
		vResult.x = v1.x * s;
		vResult.y = v1.y * s;
		vResult.z = v1.z * s;
	}

	// Computes the dot product between two vectors
	inline float vec3DotProduct(const vec3& v1, const vec3& v2)
	{
		 return (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z);
	}

	inline vec3 vec3CrossProduct(const vec3& v1, const vec3& v2)
	{
		return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}


	// Transforms a vector by a matrix
	inline vec3& vec3Transform(vec3& vResult, const vec3& v1, const mtx44& m1)
	{
		vec3 pushvec = v1;

		vResult.x = (m1(0,0) * pushvec.x) + (m1(1,0) * pushvec.y) + (m1(2,0) * pushvec.z) + m1(3,0);
		vResult.y = (m1(0,1) * pushvec.x) + (m1(1,1) * pushvec.y) + (m1(2,1) * pushvec.z) + m1(3,1);
		vResult.z = (m1(0,2) * pushvec.x) + (m1(1,2) * pushvec.y) + (m1(2,2) * pushvec.z) + m1(3,2);

		return vResult;
	}
	

	inline float vec3AngleBetweenRad(const vec3& v1, const vec3& v2)
	{
		float angle = vec3DotProduct(v1, v2);
		angle = acos(angle);
		return angle;
	}

	inline float vec3AngleBetweenDeg(const vec3& v1, const vec3& v2)
	{
		float angle = vec3DotProduct(v1, v2);
		angle = acos(angle);
		angle = RadiansToDegrees(angle);
		return angle;
	}

	// Computes the distance between two vectors
	inline float vec3DistanceBetween(const vec3& v1, const vec3& v2)
	{
		vec3 pushvec;
		vec3Subtract(pushvec, v1, v2);
		return vec3Length(pushvec);
	}

	// Computes a fast distance between two vectors
	inline float vec3DistanceBetweenSquared(const vec3& v1, const vec3& v2)
	{
		vec3 pushvec;
		vec3Subtract(pushvec, v1, v2);
		return vec3LengthSquared(pushvec);
	}

	// Clamp components of a vector into the range 0-1
	inline void vec3Clamp(vec3& vResult, const vec3& v1)
	{
		vResult.x = (v1.x > 1.f) ? 1.f : v1.x;
		vResult.x = (v1.x < 0.f) ? 0.f : v1.x;
		vResult.y = (v1.y > 1.f) ? 1.f : v1.y;
		vResult.y = (v1.y < 0.f) ? 0.f : v1.y;
		vResult.z = (v1.z > 1.f) ? 1.f : v1.z;
		vResult.z = (v1.z < 0.f) ? 0.f : v1.z;
	}

	// Negates the components of a vector 
	inline void vec3Negate(vec3& vResult, const vec3& v1)
	{
		vResult.x = -v1.x;
		vResult.y = -v1.y;
		vResult.z = -v1.z;
	}

	inline void vec3Set(vec3& vResult, float x, float y, float z)
	{
		vResult.x = x;
		vResult.y = y;
		vResult.z = z;
	}


	inline void vec3Copy(vec3& vResult, vec3& v1)
	{
		vResult.x = v1.x;
		vResult.y = v1.y;
		vResult.z = v1.z;
	}

	inline void vec3NegageCopy(vec3& vResult, vec3& v1)
	{
		vResult.x = -v1.x;
		vResult.y = -v1.y;
		vResult.z = -v1.z;
	}


	inline void vec3Absolute(vec3& vResult, vec3& v1)
	{
		vResult.x = fabsf(v1.x);
		vResult.y = fabsf(v1.y);
		vResult.z = fabsf(v1.z);
	}

	inline void vec3Zeroify(vec3& vResult)
	{
		vResult.x = 
		vResult.y = 
		vResult.z = 0.f;
	}

	inline void vec3InterpolateBetween(vec3& vResult, vec3& v1, vec3& v2, const float t)
	{
		float	lrp = 1.f - t;
		vResult.x  = lrp * v1.x + t * v2.x;
		vResult.y  = lrp * v1.y + t * v2.y;
		vResult.z  = lrp * v1.z + t * v2.z;
	}

	inline void vec3MidpointBetween(vec3& vResult, const vec3& v1, const vec3& v2)
	{
		vResult.x = (v1.x + v2.x) * 0.5f;
		vResult.y = (v1.y + v2.y) * 0.5f;
		vResult.z = (v1.z + v2.z) * 0.5f;
	}

	inline void vec3MinComponents(vec3& vResult, const vec3& v1, const vec3& v2)
	{
		vResult.x =  FriendSmasher::Math::Min(v1.x, v2.x);
		vResult.y =  FriendSmasher::Math::Min(v1.y, v2.y);
		vResult.z =  FriendSmasher::Math::Min(v1.z, v2.z);
	}

	inline void vec3MaxComponents(vec3& vResult, const vec3& v1, const vec3& v2)
	{
		vResult.x =  FriendSmasher::Math::Max(v1.x, v2.x);
		vResult.y =  FriendSmasher::Math::Max(v1.y, v2.y);
		vResult.z =  FriendSmasher::Math::Max(v1.z, v2.z);
	}

	inline bool vec3Equal(const vec3& v1, const vec3& v2)
	{
		return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
	}

	inline bool vec3ApproxEqual(const vec3& v1, const vec3& v2)
	{
		return ( FriendSmasher::Math::approxf(v1.x, v2.x,  FriendSmasher::Math::kEPSILON) &&
				 FriendSmasher::Math::approxf(v1.y, v2.y,  FriendSmasher::Math::kEPSILON) && 
				 FriendSmasher::Math::approxf(v1.z, v2.z,  FriendSmasher::Math::kEPSILON));
	}

	inline void vec3ToArray(float* arrayResult, const vec3& v1)
	{
		arrayResult[0] = v1.x;
		arrayResult[1] = v1.y;
		arrayResult[2] = v1.z;
	}

	inline void vec2FromArray(vec3& vResult, const float* array1)
	{
		vResult.x = array1[0];
		vResult.y = array1[1];
		vResult.z = array1[2];
	}

	inline int vec3MinAxis(vec3& v1)
	{
		return ((fabsf(v1.x) < fabsf(v1.y) && fabsf(v1.x) < fabsf(v1.z)) ? 0 : (fabsf(v1.y) < fabsf(v1.z)) ? 1 : 2);
	}

	inline int vec3MaxAxis(vec3& v1)
	{
		return ((fabsf(v1.x) > fabsf(v1.y) && fabsf(v1.x) > fabsf(v1.z)) ? 0 : (fabsf(v1.y) > fabsf(v1.z)) ? 1 : 2);
	}

	inline void vec3ReflectScalar(vec3& vResult, const vec3& v1, const vec3& normal, float scalar)
	{
		vec3  _2ndotvn;
		float dotVal = vec3DotProduct(v1, normal); 

		vec3Scale(_2ndotvn, normal, scalar * 2.f * dotVal);
		vec3Subtract(vResult, v1, _2ndotvn); 
	}

	inline void vec3Reflect(vec3& vResult, const vec3& v1, const vec3& normal)
	{
		vec3ReflectScalar(vResult, v1, normal, 1.f);
	}


} // Math
} //  FriendSmasher

#endif //  FriendSmasher_VEC3_OP