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

#ifndef FRIENDSMASHER_VEC3
#define FRIENDSMASHER_VEC3
#if _MSC_VER >= 1200
#pragma once
#endif

namespace FriendSmasher
{
namespace Math
{
	class vec3
	{
	public:
		vec3(void) {};
		vec3(const float broadcast) : x(broadcast), y(broadcast), z(broadcast) { };
		vec3(const float xin, const float yin, const float zin) : x(xin), y(yin), z(zin) {} ;
		~vec3(void) {};

		// Element wise
		vec3 operator +  (const vec3 &rhs) const;
		vec3 operator -  (const vec3 &rhs) const;
		vec3 operator *  (const vec3 &rhs) const; 
		vec3 operator /  (const vec3 &rhs) const;

		vec3 operator *  (const float &rhs) const; 
		vec3 operator /  (const float &rhs) const;

		vec3 operator += (const vec3 &rhs);
		vec3 operator -= (const vec3 &rhs);

		bool operator == (const vec3 &rhs) const;
		bool operator != (const vec3 &rhs) const;

		vec3 operator -() const;

		static vec3 allzero;
		static vec3 allone;

		union
		{
			struct  
			{
				float x, y, z;
			};
			float f[3];
		};
	};
}
}

#endif // namespace FriendSmasher_VEC3