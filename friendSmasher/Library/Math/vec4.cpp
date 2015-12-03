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