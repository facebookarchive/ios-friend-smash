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

#ifndef FRIENDSMASHER_MTX44
#define FRIENDSMASHER_MTX44

#include "debug.h"

namespace FriendSmasher
{
namespace Math
{
	class mtx44
	{
	public:

		mtx44(void);

		mtx44(const float aa, const float ab, const float ac, const float ad,
			  const float ba, const float bb, const float bc, const float bd,
			  const float ca, const float cb, const float cc, const float cd,
			  const float da, const float db, const float dc, const float dd);

		~mtx44(void) {};

		bool operator == (const mtx44 &rhs) const;
		bool operator != (const mtx44 &rhs) const;
		mtx44 operator * (const mtx44 &rhs) const;

		inline float& operator()(int row, int col)
		{
			FBAssert(row >= 0);
			FBAssert(row <= 3);
			FBAssert(col >= 0);
			FBAssert(col <= 3);
			return m_mat[row][col];
		}
		
		inline float operator()(int row, int col) const
		{
			FBAssert(row >= 0);
			FBAssert(row <= 3);
			FBAssert(col >= 0);
			FBAssert(col <= 3);
			return m_mat[row][col];
		}

		inline float* operator[](int row)
		{
			FBAssert(row >= 0);
			FBAssert(row <= 3);
			return m_mat[row];
		}
		
		inline const float* operator[](int row) const
		{
			FBAssert(row >= 0);
			FBAssert(row <= 3);
			return m_mat[row];
		}

		static mtx44 identity;

	private:
		float m_mat[4][4];
	};
}
}

#endif // namespace FriendSmasher_MTX44