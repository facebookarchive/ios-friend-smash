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