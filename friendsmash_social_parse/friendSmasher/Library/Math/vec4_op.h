/*
 * Copyright 2012 Facebook
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FRIENDSMASHER_VEC4_OP
#define FRIENDSMASHER_VEC4_OP

#include <math.h>
#include "vec4.h"

namespace FriendSmasher
{
namespace Math
{

	////////////////
	// Vec4
	////////////////

	// Returns the Length of a given vector via Pythagoras
	inline float vec4Length(const vec4& v1)
	{
		float result = v1.x * v1.x;
		result += v1.y * v1.y;
		result += v1.z * v1.z;
		result += v1.w * v1.w;
		result = sqrt(result);
		return(result);
	}


	// Returns the squared Length of a given vector via Pythagoras
	inline float vec4LengthSquared(const vec4& v1)
	{
		float result = v1.x * v1.x;
		result += v1.y * v1.y;
		result += v1.z * v1.z;
		result += v1.w * v1.w;
		return(result);
	}

	// Normalizes a given vector.
	inline void vec4Normalize(vec4& vResult, const vec4& v1)
	{
		float norm = vec4Length(v1);
		float reciprocal = ( norm == 0.0f ) ? 1.0f : (1.0f /norm);
		vResult.x = v1.x * reciprocal;
		vResult.y = v1.y * reciprocal;
		vResult.z = v1.z * reciprocal;
		vResult.w = v1.w * reciprocal;
	}

	// Component adds two vectors together
	inline void vec4Add(vec4& vResult, const vec4& v1, const vec4& v2)
	{
		vResult.x = v1.x + v2.x;
		vResult.y = v1.y + v2.y;
		vResult.z = v1.z + v2.z;
		vResult.w = v1.w + v2.w;
	}

	// Component subtracts two vectors
	inline void vec4Subtract(vec4& vResult, const vec4& v1, const vec4& v2)
	{
		vResult.x = v1.x - v2.x;
		vResult.y = v1.y - v2.y;
		vResult.z = v1.z - v2.z;
		vResult.w = v1.w - v2.w;
	}

	// Component multiplies two vectors
	inline void vec4Multiply(vec4& vResult, const vec4& v1, const vec4& v2)
	{
		vResult.x = v1.x * v2.x;
		vResult.y = v1.y * v2.y;
		vResult.z = v1.z * v2.z;
		vResult.w = v1.w * v2.w;
	}

	// Component divides two vectors
	inline void vec4Divide(vec4& vResult, const vec4& v1, const vec4& v2)
	{
		vResult.x = v1.x / v2.x;
		vResult.y = v1.y / v2.y;
		vResult.z = v1.z / v2.z;
		vResult.w = v1.w / v2.w;
	}

	// Scales a vector magnitude by a float
	inline void vec4Scale(vec4& vResult, const vec4& v1, const float s)
	{
		vResult.x = v1.x * s;
		vResult.y = v1.y * s;
		vResult.z = v1.z * s;
		vResult.w = v1.w * s;
	}

	// Computes the dot product between two vectors
	inline float vec4DotProduct(const vec4& v1, const vec4& v2)
	{
		return (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z) + (v1.w*v2.w);
	}

	inline float vec4AngleBetweenRad(const vec4& v1, const vec4& v2)
	{
		float angle = vec4DotProduct(v1, v2);
		angle = acos(angle);
		return angle;
	}

	inline float vec4AngleBetweenDeg(const vec4& v1, const vec4& v2)
	{
		float angle = vec4DotProduct(v1, v2);
		angle = acos(angle);
		angle = RadiansToDegrees(angle);
		return angle;
	}

	// Computes the distance between two vectors
	inline float vec4DistanceBetween(const vec4& v1, const vec4& v2)
	{
		vec4 pushvec;
		vec4Subtract(pushvec, v1, v2);
		return vec4Length(pushvec);
	}

	// Computes a fast distance between two vectors
	inline float vec4DistanceBetweenSquared(const vec4& v1, const vec4& v2)
	{
		vec4 pushvec;
		vec4Subtract(pushvec, v1, v2);
		return vec4LengthSquared(pushvec);
	}

	// Clamp components of a vector into the range 0-1
	inline void vec4Clamp(vec4& vResult, const vec4& v1)
	{
		vResult.x = (v1.x > 1.f) ? 1.f : v1.x;
		vResult.x = (v1.x < 0.f) ? 0.f : v1.x;
		vResult.y = (v1.y > 1.f) ? 1.f : v1.y;
		vResult.y = (v1.y < 0.f) ? 0.f : v1.y;
		vResult.z = (v1.z > 1.f) ? 1.f : v1.z;
		vResult.z = (v1.z < 0.f) ? 0.f : v1.z;
		vResult.w = (v1.w > 1.f) ? 1.f : v1.w;
		vResult.w = (v1.w < 0.f) ? 0.f : v1.w;
	}

	inline vec4& vec4Transform(vec4& vResult, const vec4& v1, const mtx44& m1)
	{
		vec4 pushvec = v1;
		
		vResult.x = (m1(0,0) * pushvec.x) + (m1(1,0) * pushvec.y) + (m1(2,0) * pushvec.z) + (m1(3,0) * pushvec.w);
		vResult.y = (m1(0,1) * pushvec.x) + (m1(1,1) * pushvec.y) + (m1(2,1) * pushvec.z) + (m1(3,1) * pushvec.w);
		vResult.z = (m1(0,2) * pushvec.x) + (m1(1,2) * pushvec.y) + (m1(2,2) * pushvec.z) + (m1(3,2) * pushvec.w);
		vResult.w = (m1(0,3) * pushvec.x) + (m1(1,3) * pushvec.y) + (m1(2,3) * pushvec.z) + (m1(3,3) * pushvec.w);
		
		return( vResult );
	}

	// Negates the components of a vector 
	inline void vec4Negate(vec4& vResult, const vec4& v1)
	{
		vResult.x = -v1.x;
		vResult.y = -v1.y;
		vResult.z = -v1.z;
		vResult.w = -v1.w;
	}

	inline void vec4Set(vec4& vResult, float x, float y, float z, float w)
	{
		vResult.x = x;
		vResult.y = y;
		vResult.z = z;
		vResult.w = w;
	}


	inline void vec4Copy(vec4& vResult, vec4& v1)
	{
		vResult.x = v1.x;
		vResult.y = v1.y;
		vResult.z = v1.z;
		vResult.w = v1.w;
	}

	inline void vec4NegateCopy(vec4& vResult, vec4& v1)
	{
		vResult.x = -v1.x;
		vResult.y = -v1.y;
		vResult.z = -v1.z;
		vResult.w = -v1.w;
	}


	inline void vec4Absolute(vec4& vResult, vec4& v1)
	{
		vResult.x = fabsf(v1.x);
		vResult.y = fabsf(v1.y);
		vResult.z = fabsf(v1.z);
		vResult.w = fabsf(v1.w);
	}

	inline void vec4Zeroify(vec4& vResult)
	{
		vResult.x = 
			vResult.y = 
			vResult.z = 
			vResult.w = 0.f;
	}

	inline void vec4InterpolateBetween(vec4& vResult, vec4& v1, vec4& v2, const float t)
	{
		float	lrp = 1.f - t;
		vResult.x  = lrp * v1.x + t * v2.x;
		vResult.y  = lrp * v1.y + t * v2.y;
		vResult.z  = lrp * v1.z + t * v2.z;
		vResult.w  = lrp * v1.w + t * v2.w;
	}

	inline void vec4MidpointBetween(vec4& vResult, const vec4& v1, const vec4& v2)
	{
		vResult.x = (v1.x + v2.x) * 0.5f;
		vResult.y = (v1.y + v2.y) * 0.5f;
		vResult.z = (v1.z + v2.z) * 0.5f;
		vResult.w = (v1.w + v2.w) * 0.5f;
	}

	inline void vec4MinComponents(vec4& vResult, const vec4& v1, const vec4& v2)
	{
		vResult.x =  FriendSmasher::Math::Min(v1.x, v2.x);
		vResult.y =  FriendSmasher::Math::Min(v1.y, v2.y);
		vResult.z =  FriendSmasher::Math::Min(v1.z, v2.z);
		vResult.w =  FriendSmasher::Math::Min(v1.w, v2.w);
	}

	inline void vec4MaxComponents(vec4& vResult, const vec4& v1, const vec4& v2)
	{
		vResult.x =  FriendSmasher::Math::Max(v1.x, v2.x);
		vResult.y =  FriendSmasher::Math::Max(v1.y, v2.y);
		vResult.z =  FriendSmasher::Math::Max(v1.z, v2.z);
		vResult.w =  FriendSmasher::Math::Max(v1.w, v2.w);
	}

	inline bool vec4Equal(const vec4& v1, const vec4& v2)
	{
		return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w);
	}

	inline bool vec4ApproxEqual(const vec4& v1, const vec4& v2)
	{
		return (approxf(v1.x, v2.x,  FriendSmasher::Math::kEPSILON) &&
				approxf(v1.y, v2.y,  FriendSmasher::Math::kEPSILON) && 
				approxf(v1.z, v2.z,  FriendSmasher::Math::kEPSILON) &&
				approxf(v1.w, v2.w,  FriendSmasher::Math::kEPSILON));
	}

	inline void vec4ToArray(float* arrayResult, const vec4& v1)
	{
		arrayResult[0] = v1.x;
		arrayResult[1] = v1.y;
		arrayResult[2] = v1.z;
		arrayResult[3] = v1.w;
	}

	inline void vec2FromArray(vec4& vResult, const float* array1)
	{
		vResult.x = array1[0];
		vResult.y = array1[1];
		vResult.z = array1[2];
		vResult.w = array1[3];
	}

	inline int Vec4MinAxis(vec4& v1)
	{
		return ((fabsf(v1.x) < fabsf(v1.y) && fabsf(v1.x) < fabsf(v1.z)) ? 0 : (fabsf(v1.y) < fabsf(v1.z)) ? 1 : (fabsf(v1.z) < fabsf(v1.w) ? 2 : 3));
	}

	inline int vec4MaxAxis(vec4& v1)
	{
		return ((fabsf(v1.x) > fabsf(v1.y) && fabsf(v1.x) > fabsf(v1.z)) ? 0 : (fabsf(v1.y) > fabsf(v1.z)) ? 1 : (fabsf(v1.z) > fabsf(v1.w) ? 2 : 3));
	}

	inline void vec4ReflectScalar(vec4& vResult, const vec4& v1, const vec4& normal, float scalar)
	{
		vec4  _2ndotvn;
		float dotVal = vec4DotProduct(v1, normal); 
		vec4Scale(_2ndotvn, normal, scalar * 2.f * dotVal);
		vec4Subtract(vResult, v1, _2ndotvn); 
	}

	inline void vec4Reflect(vec4& vResult, const vec4& v1, const vec4& normal)
	{
		vec4ReflectScalar(vResult, v1, normal, 1.f);
	}
} // Math
} //  FriendSmasher

#endif //  FriendSmasher_VEC4_OP