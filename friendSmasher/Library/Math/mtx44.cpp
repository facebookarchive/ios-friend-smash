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

#include "mtx44.h"
#include "mtx44_op.h"


namespace FriendSmasher
{
namespace Math
{
	mtx44	mtx44::identity( 1.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 1.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 1.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 1.0f );

#define SET_MATRIX_ROW(row, v1, v2, v3, v4)    \
	m_mat[(row)][0] = (v1); \
	m_mat[(row)][1] = (v2); \
	m_mat[(row)][2] = (v3); \
	m_mat[(row)][3] = (v4)

	mtx44::mtx44( void )
	{
		SET_MATRIX_ROW(0, 1.f, 0.f, 0.f, 0.f);
		SET_MATRIX_ROW(1, 0.f, 1.f, 0.f, 0.f);
		SET_MATRIX_ROW(2, 0.f, 0.f, 1.f, 0.f);
		SET_MATRIX_ROW(3, 0.f, 0.f, 0.f, 1.f);
	}

	mtx44::mtx44( const float aa, const float ab, const float ac, const float ad, const float ba, const float bb, const float bc, const float bd, const float ca, const float cb, const float cc, const float cd, const float da, const float db, const float dc, const float dd )
	{
		SET_MATRIX_ROW(0, aa, ab, ac, ad);
		SET_MATRIX_ROW(1, ba, bb, bc, bd);
		SET_MATRIX_ROW(2, ca, cb, cc, cd);
		SET_MATRIX_ROW(3, da, db, dc, dd);
	}

	bool mtx44::operator == (const mtx44 &rhs) const
	{
		return (this->m_mat[0][0] == rhs.m_mat[0][0] && this->m_mat[0][1] == rhs.m_mat[0][1] && this->m_mat[0][2] == rhs.m_mat[0][2] && this->m_mat[0][3] == rhs.m_mat[0][3] &&
				this->m_mat[1][0] == rhs.m_mat[1][0] && this->m_mat[1][1] == rhs.m_mat[1][1] && this->m_mat[1][2] == rhs.m_mat[1][2] && this->m_mat[1][3] == rhs.m_mat[1][3] &&
				this->m_mat[2][0] == rhs.m_mat[2][0] && this->m_mat[2][1] == rhs.m_mat[2][1] && this->m_mat[2][2] == rhs.m_mat[2][2] && this->m_mat[2][3] == rhs.m_mat[2][3] &&
				this->m_mat[3][0] == rhs.m_mat[3][0] && this->m_mat[3][1] == rhs.m_mat[3][1] && this->m_mat[3][2] == rhs.m_mat[3][2] && this->m_mat[3][3] == rhs.m_mat[3][3]);
	}

	bool mtx44::operator != (const mtx44 &rhs) const
	{
		return !(this->m_mat[0][0] == rhs.m_mat[0][0] && this->m_mat[0][1] == rhs.m_mat[0][1] && this->m_mat[0][2] == rhs.m_mat[0][2] && this->m_mat[0][3] == rhs.m_mat[0][3] &&
				this->m_mat[1][0] == rhs.m_mat[1][0] && this->m_mat[1][1] == rhs.m_mat[1][1] && this->m_mat[1][2] == rhs.m_mat[1][2] && this->m_mat[1][3] == rhs.m_mat[1][3] &&
				this->m_mat[2][0] == rhs.m_mat[2][0] && this->m_mat[2][1] == rhs.m_mat[2][1] && this->m_mat[2][2] == rhs.m_mat[2][2] && this->m_mat[2][3] == rhs.m_mat[2][3] &&
				this->m_mat[3][0] == rhs.m_mat[3][0] && this->m_mat[3][1] == rhs.m_mat[3][1] && this->m_mat[3][2] == rhs.m_mat[3][2] && this->m_mat[3][3] == rhs.m_mat[3][3]);
	}

	mtx44 mtx44::operator* (const mtx44 &rhs) const
	{
		mtx44 result;
		mtx44Multiply(result, *this, rhs);
		return result;
	}
	
}
}