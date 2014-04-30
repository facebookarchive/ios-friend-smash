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

#include "math_lib.h"

namespace FriendSmasher
{
namespace Math
{

	vec3 vec3::allzero	= vec3(0.0f,0.0f,0.f);
	vec3 vec3::allone	= vec3(1.0f,1.0f,1.f);

	vec3 vec3::operator + (const vec3 &rhs) const
	{
		return vec3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
	}

	vec3 vec3::operator - (const vec3 &rhs) const
	{
		return vec3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
	}

	vec3 vec3::operator * (const vec3 &rhs) const
	{
		return vec3(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
	}

	vec3 vec3::operator / (const vec3 &rhs) const
	{
		return vec3(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
	}

	vec3 vec3::operator * (const float &rhs) const
	{
		vec3 vResult;
		vec3Scale(vResult, *this, rhs);
		return vResult;
	}

	vec3 vec3::operator / (const float &rhs) const
	{
		vec3 vResult;
		vec3Scale(vResult, *this, 1.f/rhs);
		return vResult;
	}

	vec3 vec3::operator += (const vec3 &rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}

	vec3 vec3::operator -= (const vec3 &rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}

	bool vec3::operator == (const vec3 &rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
	}

	bool vec3::operator != (const vec3 &rhs) const
	{
		return !(this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
	}

	vec3 vec3::operator - () const
	{
		return vec3(-x, -y, -z);
	}

}
}