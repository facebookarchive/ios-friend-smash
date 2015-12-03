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

#ifndef FRIENDSMASHER_MTX33_OP
#define FRIENDSMASHER_MTX33_OP

#include "mtx44.h"
#include "math_op.h"
#include "vec2.h"

namespace FriendSmasher
{
namespace Math
{

inline void mtx33MakeIdentity(mtx33& mResult)
{
	 mtx33 m;
	mResult = m;
}

inline void mtx33Multiply(mtx33& mResult, const mtx33& ma, const mtx33& mb)
{
	mResult(0,0) = (ma(0,0) * mb(0,0)) + (ma(0,1) * mb(1,0)) + (ma(0,2) * mb(2,0)) ;
	mResult(0,1) = (ma(0,0) * mb(0,1)) + (ma(0,1) * mb(1,1)) + (ma(0,2) * mb(2,1)) ;
	mResult(0,2) = (ma(0,0) * mb(0,2)) + (ma(0,1) * mb(1,2)) + (ma(0,2) * mb(2,2)) ;

	mResult(1,0) = (ma(1,0) * mb(0,0)) + (ma(1,1) * mb(1,0)) + (ma(1,2) * mb(2,0)) ;
	mResult(1,1) = (ma(1,0) * mb(0,1)) + (ma(1,1) * mb(1,1)) + (ma(1,2) * mb(2,1)) ;
	mResult(1,2) = (ma(1,0) * mb(0,2)) + (ma(1,1) * mb(1,2)) + (ma(1,2) * mb(2,2)) ;

	mResult(2,0) = (ma(2,0) * mb(0,0)) + (ma(2,1) * mb(1,0)) + (ma(2,2) * mb(2,0)) ;
	mResult(2,1) = (ma(2,0) * mb(0,1)) + (ma(2,1) * mb(1,1)) + (ma(2,2) * mb(2,1)) ;
	mResult(2,2) = (ma(2,0) * mb(0,2)) + (ma(2,1) * mb(1,2)) + (ma(2,2) * mb(2,2)) ;
}

inline void mtx33Scale(mtx33& mResult, const vec2& vScale)
{
	mResult(0,0) = vScale.x;
	mResult(1,1) = vScale.y;
}
	
inline void mtx33Rotate(mtx33& mResult, float radAngle)
{
	float ca = cosf(radAngle);
	float sa = sinf(radAngle);
	
	mResult(0,0) = ca;
	mResult(0,1) = -sa;
	mResult(1,0) = sa;
	mResult(1,1) = ca;
}

inline void mtx33Rotate_Deg(mtx33& mResult, float degAngle)
{
	float radangle = DegreesToRadians(degAngle);
	mtx33Rotate(mResult, radangle);
}

inline void mtx33Translate(mtx33& mResult, const vec2& vTranslate)
{
	mResult(2,0) = vTranslate.x;
	mResult(2,1) = vTranslate.y;
}

inline void mtx33Transpose(mtx33& mResult, const mtx33& m)
{
	mtx33 mT = m;

	for(int r=0;r<3;++r)
		for(int c=0;c<3;++c)
			mT(c,r) = m(r,c);

	mResult = mT;
}

inline void mtx33Invert( mtx33& m )
{
	// Calculate the cofactor elements of the matrix
	mtx33 mCofactors;

	for( unsigned int c=0; c<3; ++c )
	{
		for( unsigned int r=0; r<3; ++r )
		{
			static const unsigned int sel0[] = {1,0,0};
			static const unsigned int sel1[] = {2,2,1};

			const unsigned int col0 = sel0[c];
			const unsigned int col1 = sel1[c];
			const unsigned int row0 = sel0[r];
			const unsigned int row1 = sel1[r];

			float minor = m[col0][row0] * m[col1][row1] - 
						  m[col0][row1] * m[col1][row0];

			mCofactors(r,c) = minor;
		}
	}

	// Calculate the determinant of the matrix
	//float det = mCofactors[0][0] + mCofactors[0][1] - mCofactors[0][2];
	float det = m[0][0] * ( m[1][1]*m[2][2] - m[2][1]*m[1][2] ) -
				m[0][1] * ( m[1][0]*m[2][2] - m[2][0]*m[1][2] ) +
				m[0][2] * ( m[1][0]*m[2][1] - m[2][0]*m[1][1] );

	if( det == 0.0f ) { return; } // No solution
	float rcp = 1.0f / det;

	// Now form the inverse matrix
	for( unsigned int r=0; r<3; ++r )
	{
		for( unsigned int c=0; c<3; ++c )
		{
			const unsigned int signmask = ((c+r) & 1) << 31;

			// Now adjust the sign of the cofactor:
			union 
			{
				float f;
				int i;
			} cofactor;

			cofactor.f = mCofactors[r][c];
			cofactor.i ^= signmask;

			m[r][c] = cofactor.f * rcp;
		}
	}	
}

inline mtx33 mtx33CreateTransform(mtx33& mResult, const vec2& vTranslate, const vec2& vScale, float fRotation )
{
	mtx33Rotate(mResult, fRotation);
	mResult(2,0) = vTranslate.x;	mResult(2,1) = vTranslate.y;
	mResult(0,0) *= vScale.x;		mResult(1,1) *= vScale.y;

	return mResult;
}

}
}

#endif // namespace FriendSmasher_MTX33_OP