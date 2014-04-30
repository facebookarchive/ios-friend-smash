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