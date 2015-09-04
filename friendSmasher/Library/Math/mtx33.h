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