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

#ifndef FRIENDSMASHER_VEC4
#define FRIENDSMASHER_VEC4

namespace FriendSmasher
{
namespace Math
{
	class vec4
	{
	public:
		vec4(void) : x(0.f), y(0.f), z(0.f), w(0.f) {} ;
		vec4(const float broadcast) : x(broadcast), y(broadcast), z(broadcast) {};
		vec4(const float xin, const float yin, const float zin, const float win) : x(xin), y(yin), z(zin), w(win) {} ;

		~vec4(void) {};

		vec4 operator +  (const vec4 &rhs) const;
		vec4 operator -  (const vec4 &rhs) const;
		vec4 operator *  (const vec4 &rhs) const; 
		vec4 operator /  (const vec4 &rhs) const;
		
		vec4 operator *  (const float &rhs) const; 
		vec4 operator /  (const float &rhs) const;
		
		vec4 operator += (const vec4 &rhs);
		vec4 operator -= (const vec4 &rhs);

		bool operator == (const vec4 &rhs) const;
		bool operator != (const vec4 &rhs) const;

		
		static vec4 allzero;
		static vec4 allone;

		union
		{
			struct  
			{
				float x, y, z, w;
			};
			float f[4];
		};
	};

}
}

#endif // namespace FriendSmasher_VEC4