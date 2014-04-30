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

#ifndef FRIENDSMASHER_MTX33
#define FRIENDSMASHER_MTX33

#include <string>

namespace FriendSmasher
{
namespace Math
{

#define SET_MATRIX_ROW(row, v1, v2, v3) \
	m_mat[(row)][0] = (v1);	\
	m_mat[(row)][1] = (v2);	\
	m_mat[(row)][2] = (v3);


	class mtx33
	{
	public:

		mtx33( void )
		{
			SET_MATRIX_ROW(0, 1.0f, 0.0f, 0.0f );
			SET_MATRIX_ROW(1, 0.0f, 1.0f, 0.0f );
			SET_MATRIX_ROW(2, 0.0f, 0.0f, 1.0f );
		}

		mtx33(const float aa, const float ab, const float ac,
			  const float ba, const float bb, const float bc,
			  const float ca, const float cb, const float cc );

		~mtx33(void) {};

		float& operator()(int row, int col)
		{
			return m_mat[row][col];
		}

		float operator()(int row, int col) const
		{
			return m_mat[row][col];
		}

		float* operator[](int row)
		{
			return m_mat[row];
		}

		const float* operator[](int row) const
		{
			return m_mat[row];
		}

		bool operator == ( const mtx33& rhs ) const
		{
			return memcmp(this, &rhs, sizeof(mtx33)) == 0;		// TODO: Maybe change to lots of == ?  Needs profiling
		}

		bool operator != ( const mtx33& rhs ) const
		{
			return memcmp(this, &rhs, sizeof(mtx33)) != 0;
		}

		mtx33 operator * ( const mtx33& rhs ) const
		{
			mtx33 mResult;
			
 			mResult(0,0) = (m_mat[0][0] * rhs(0,0)) + (m_mat[0][1] * rhs(1,0)) + (m_mat[0][2] * rhs(2,0)) ;
 			mResult(0,1) = (m_mat[0][0] * rhs(0,1)) + (m_mat[0][1] * rhs(1,1)) + (m_mat[0][2] * rhs(2,1)) ;
 			mResult(0,2) = (m_mat[0][0] * rhs(0,2)) + (m_mat[0][1] * rhs(1,2)) + (m_mat[0][2] * rhs(2,2)) ;
 
 			mResult(1,0) = (m_mat[1][0] * rhs(0,0)) + (m_mat[1][1] * rhs(1,0)) + (m_mat[1][2] * rhs(2,0)) ;
 			mResult(1,1) = (m_mat[1][0] * rhs(0,1)) + (m_mat[1][1] * rhs(1,1)) + (m_mat[1][2] * rhs(2,1)) ;
 			mResult(1,2) = (m_mat[1][0] * rhs(0,2)) + (m_mat[1][1] * rhs(1,2)) + (m_mat[1][2] * rhs(2,2)) ;
 
 			mResult(2,0) = (m_mat[2][0] * rhs(0,0)) + (m_mat[2][1] * rhs(1,0)) + (m_mat[2][2] * rhs(2,0)) ;
 			mResult(2,1) = (m_mat[2][0] * rhs(0,1)) + (m_mat[2][1] * rhs(1,1)) + (m_mat[2][2] * rhs(2,1)) ;
 			mResult(2,2) = (m_mat[2][0] * rhs(0,2)) + (m_mat[1][1] * rhs(1,2)) + (m_mat[2][2] * rhs(2,2)) ;

			return mResult;
		}

		static mtx33 identity;


	private:
		float m_mat[3][3];
	};


}
}

#endif // namespace FriendSmasher_MTX33