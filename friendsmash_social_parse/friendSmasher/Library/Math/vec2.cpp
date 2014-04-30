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

	vec2 vec2::allzero	= vec2(0.0f,0.0f);
	vec2 vec2::allone	= vec2(1.0f,1.0f);
	vec2 vec2::min	= vec2(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	vec2 vec2::max	= vec2( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());

	float vec2::Length() const
	{
		return sqrtf(this->x * this->x + this->y * this->y);
	}

	// Returns the original length
	float vec2::Normalise()
	{
		float fLength = Length();
		float fRcpLength = 1.0f / fLength;
		this->x *= fRcpLength;
		this->y *= fRcpLength;

		return fLength;
	}

	void vec2::Normalise(const float fLength)
	{
		float fRcpLength = 1.0f / fLength;
		this->x *= fRcpLength;
		this->y *= fRcpLength;
	}

}
}