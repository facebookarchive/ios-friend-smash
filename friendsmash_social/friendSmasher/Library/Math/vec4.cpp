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
	vec4 vec4::allzero	= vec4(0.0f,0.0f,0.f,0.f);
	vec4 vec4::allone	= vec4(1.0f,1.0f,1.f,1.f);
	
	vec4 vec4::operator + (const vec4 &rhs) const
	{
		return vec4(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w);
	}

	vec4 vec4::operator - (const vec4 &rhs) const
	{
		return vec4(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->w - rhs.w);
	}
	
	vec4 vec4::operator * (const vec4 &rhs) const
	{
		return vec4(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z, this->w * rhs.w);
	}
		
	
	vec4 vec4::operator / (const vec4 &rhs) const
	{
		return vec4(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z, this->w / rhs.w);
	}
	
	vec4 vec4::operator * (const float &rhs) const
	{
		vec4 vResult;
		vec4Scale(vResult, *this, rhs);
		return vResult;
	}
	
	vec4 vec4::operator / (const float &rhs) const
	{
		vec4 vResult;
		vec4Scale(vResult, *this, 1.f/rhs);
		return vResult;
	}
	
	

	vec4 vec4::operator += (const vec4 &rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		this->w += rhs.w;
		return *this;
	}

	vec4 vec4::operator -= (const vec4 &rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->w -= rhs.w;
		this->z -= rhs.z;
		return *this;
	}

	bool vec4::operator == (const vec4 &rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w);
	}


	bool vec4::operator != (const vec4 &rhs) const
	{
		return !(this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w);
	}
}

}