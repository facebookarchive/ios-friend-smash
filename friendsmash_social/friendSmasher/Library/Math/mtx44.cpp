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