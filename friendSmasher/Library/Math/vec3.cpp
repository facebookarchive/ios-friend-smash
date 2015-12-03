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