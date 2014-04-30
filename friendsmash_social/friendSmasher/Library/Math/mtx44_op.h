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

#ifndef FRIENDSMASHER_MTX44_OP
#define FRIENDSMASHER_MTX44_OP

#include "mtx44.h"
#include "math_op.h"
#include "vec3.h"

namespace FriendSmasher
{
namespace Math
{

inline void mtx44MakeIdentity(mtx44& mResult)
{
	mResult = mtx44::identity;
}

inline void mtx44Multiply(mtx44& mResult, const mtx44& ma, const mtx44& mb)
{
	mResult(0,0) = (ma(0,0) * mb(0,0)) + (ma(0,1) * mb(1,0)) + (ma(0,2) * mb(2,0)) + (ma(0,3) * mb(3,0));
	mResult(0,1) = (ma(0,0) * mb(0,1)) + (ma(0,1) * mb(1,1)) + (ma(0,2) * mb(2,1)) + (ma(0,3) * mb(3,1));
	mResult(0,2) = (ma(0,0) * mb(0,2)) + (ma(0,1) * mb(1,2)) + (ma(0,2) * mb(2,2)) + (ma(0,3) * mb(3,2));
	mResult(0,3) = (ma(0,0) * mb(0,3)) + (ma(0,1) * mb(1,3)) + (ma(0,2) * mb(2,3)) + (ma(0,3) * mb(3,3));

	mResult(1,0) = (ma(1,0) * mb(0,0)) + (ma(1,1) * mb(1,0)) + (ma(1,2) * mb(2,0)) + (ma(1,3) * mb(3,0));
	mResult(1,1) = (ma(1,0) * mb(0,1)) + (ma(1,1) * mb(1,1)) + (ma(1,2) * mb(2,1)) + (ma(1,3) * mb(3,1));
	mResult(1,2) = (ma(1,0) * mb(0,2)) + (ma(1,1) * mb(1,2)) + (ma(1,2) * mb(2,2)) + (ma(1,3) * mb(3,2));
	mResult(1,3) = (ma(1,0) * mb(0,3)) + (ma(1,1) * mb(1,3)) + (ma(1,2) * mb(2,3)) + (ma(1,3) * mb(3,3));

	mResult(2,0) = (ma(2,0) * mb(0,0)) + (ma(2,1) * mb(1,0)) + (ma(2,2) * mb(2,0)) + (ma(2,3) * mb(3,0));
	mResult(2,1) = (ma(2,0) * mb(0,1)) + (ma(2,1) * mb(1,1)) + (ma(2,2) * mb(2,1)) + (ma(2,3) * mb(3,1));
	mResult(2,2) = (ma(2,0) * mb(0,2)) + (ma(2,1) * mb(1,2)) + (ma(2,2) * mb(2,2)) + (ma(2,3) * mb(3,2));
	mResult(2,3) = (ma(2,0) * mb(0,3)) + (ma(2,1) * mb(1,3)) + (ma(2,2) * mb(2,3)) + (ma(2,3) * mb(3,3));

	mResult(3,0) = (ma(3,0) * mb(0,0)) + (ma(3,1) * mb(1,0)) + (ma(3,2) * mb(2,0)) + (ma(3,3) * mb(3,0));
	mResult(3,1) = (ma(3,0) * mb(0,1)) + (ma(3,1) * mb(1,1)) + (ma(3,2) * mb(2,1)) + (ma(3,3) * mb(3,1));
	mResult(3,2) = (ma(3,0) * mb(0,2)) + (ma(3,1) * mb(1,2)) + (ma(3,2) * mb(2,2)) + (ma(3,3) * mb(3,2));
	mResult(3,3) = (ma(3,0) * mb(0,3)) + (ma(3,1) * mb(1,3)) + (ma(3,2) * mb(2,3)) + (ma(3,3) * mb(3,3));
}

inline void mtx44Scale(mtx44& mResult, const vec3& v1)
{
	mResult(0,0) = v1.x;
	mResult(1,1) = v1.y;
	mResult(2,2) = v1.z;
}

inline void mtx44RotateX_Rad(mtx44& mResult, const float radangle)
{
	float ca = cosf(radangle);
	float sa = sinf(radangle);
	
	mResult(1,1) = ca;
	mResult(1,2) = sa;
	mResult(2,1) = -sa;
	mResult(2,2) = ca;
}

inline void mtx44RotateY_Rad(mtx44& mResult, const float radangle)
{
	float ca = cosf(radangle);
	float sa = sinf(radangle);
	
	mResult(0,0) = ca;
	mResult(0,2) = -sa;
	mResult(2,0) = sa;
	mResult(2,2) = ca;
}

inline void mtx44RotateZ_Rad(mtx44& mResult, const float radangle)
{
	float ca = cosf(radangle);
	float sa = sinf(radangle);

	mResult(0,0) = ca;
	mResult(0,1) = sa;
	mResult(1,0) = -sa;
	mResult(1,1) = ca;
}

inline void mtx44RotateX_Deg(mtx44& mResult, float degangle)
{
	float radangle = DegreesToRadians(degangle);
	mtx44RotateX_Rad(mResult, radangle);
}

inline void mtx44RotateY_Deg(mtx44& mResult, float degangle)
{
	float radangle = DegreesToRadians(degangle);
	mtx44RotateY_Rad(mResult, radangle);
}

inline void mtx44RotateZ_Deg(mtx44& mResult, float degangle)
{
	float radangle = DegreesToRadians(degangle);
	mtx44RotateZ_Rad(mResult, radangle);
	mtx44RotateZ_Rad(mResult, radangle);
}

inline void mtx44RotateArbitary_Rad(mtx44& mResult, const float radangle, const vec3& axis)
{
	float c = cosf(radangle);
	float s = sinf(radangle);
	float t = -1.0f*c;

	mResult(0,0) = t*(axis.x * axis.x) + c;
	mResult(0,1) = t*axis.x*axis.y - s*axis.z;
	mResult(0,2) = t*axis.x*axis.z + s*axis.y;
	mResult(0,3) = 0.f;

	mResult(1,0) = t*axis.x*axis.y + s*axis.z;
	mResult(1,1) = t*(axis.y*axis.y) + c;
	mResult(1,2) = t*axis.y*axis.z - s*axis.x;
	mResult(1,3) = 0.f;

	mResult(2,0) = t*axis.x*axis.z - s*axis.y;
	mResult(2,1) = t*axis.y*axis.z + s*axis.x;
	mResult(2,2) = t*(axis.z*axis.z) + c;
	mResult(2,3) = 0.f;

	mResult(3,0) = 0.f;
	mResult(3,1) = 0.f;
	mResult(3,2) = 0.f;
	mResult(3,3) = 1.f;
}

inline void mtx44RotateArbitary_Deg(mtx44& mResult, float degangle, const vec3& axis)
{
	float radangle = DegreesToRadians(degangle);
	mtx44RotateArbitary_Rad(mResult, radangle, axis);
}

inline void mtx44Translate(mtx44& mResult, const vec3& v1)
{
	mResult(3,0) = v1.x;
	mResult(3,1) = v1.y;
	mResult(3,2) = v1.z;
}

inline void mtx44Transpose(mtx44& mResult, const mtx44& m1)
{
	mtx44 m1T = m1;

	for(int r=0; r<4; r++)
		for(int c=0;c<4; c++)
			m1T(c,r) = m1(r, c);

	mResult = m1T;
}


inline void mtx44MakeOrthographicRH(mtx44& mResult, float w, float h, float zn, float zf )
{
	mtx44MakeIdentity(mResult);

	mResult(0,0) = 2.f/w;
	mResult(1,1) = 2.f/h;
	mResult(2,2) = 1.f/(zn-zf);
	mResult(2,3) = zn/(zn-zf);
}

inline void mtx44MakeOrthographicLH(mtx44& mResult, float w, float h, float zn, float zf )
{
	mtx44MakeIdentity(mResult);

	mResult(0,0) = 2.f/w;
	mResult(1,1) = 2.f/h;
	mResult(2,2) = 1.f/(zf-zn);
	mResult(2,3) = zn/(zn-zf);
}

inline void mtx44MakeOrthographicOffCenterRH(mtx44& mResult, float l, float r, float t, float b, float zn, float zf )
{
	mtx44MakeIdentity(mResult);

	mResult(0,0) = 2.f / (r-l);
	mResult(1,1) = 2.f / (t-b);
	mResult(2,2) = 1.f / (zn-zf);
	mResult(0,3) = (l+r)/(l-r);
	mResult(1,3) = (t+b)/(b-t);
	mResult(2,3) = zn/(zn-zf);
}

inline void mtx44MakeOrthographicOffCenterLH(mtx44& mResult, float l, float r, float t, float b, float zn, float zf )
{
	mtx44MakeIdentity(mResult);

	mResult(0,0) = 2.f / (r-l);
	mResult(1,1) = 2.f / (t-b);
	mResult(2,2) = 1.f / (zf-zn);
	mResult(0,3) = (l+r)/(l-r);
	mResult(1,3) = (t+b)/(b-t);
	mResult(2,3) = zn/(zn-zf);
}


inline void mtx44MakePerspectiveRH(mtx44& mResult, float fFOV, float fAspect, float fNear, float fFar )
{
	float yScale = 1.0f / tanf(fFOV / 2.0f);
	float xScale = yScale / fAspect;

	mtx44MakeIdentity(mResult);

// 	xScale     0          0              0
// 	0        yScale       0              0
// 	0          0      zf/(zn-zf)        -1
// 	0          0      zn*zf/(zn-zf)      0

	mResult(0,0) = xScale;
	mResult(1,1) = yScale;
	mResult(2,2) = fFar/(fNear-fFar);
	mResult(2,3) = -1.0f;
	mResult(3,2) = (fNear*fFar)/(fNear-fFar);
	mResult(3,3) = 0.0f;
}

inline void mtx44MakePerspectiveLH(mtx44& mResult, float fFOV, float fAspect, float fNear, float fFar )
{
	float yScale = 1.0f / tanf(fFOV / 2.0f);
	float xScale = yScale / fAspect;

	mtx44MakeIdentity(mResult);

// 	xScale     0          0               0
// 	0        yScale       0               0
// 	0          0       zf/(zf-zn)         1
// 	0          0       -zn*zf/(zf-zn)     0

	mResult(0,0) = xScale;
	mResult(1,1) = yScale;
	mResult(2,2) = fFar/(fFar-fNear);
	mResult(2,3) = 1.0f;
	mResult(3,2) = -fNear*fFar/(fFar-fNear);
	mResult(3,3) = 0.0f;
}

inline void mtx44Invert(mtx44& m)
{
	// Calculate the cofactor elements of the matrix
	mtx44 mCofactors;

	for( unsigned int c=0; c<4; ++c )
	{
		for( unsigned int r=0; r<4; ++r )
		{
			static const unsigned int sel0[] = { 1,0,0,0 };
			static const unsigned int sel1[] = { 2,2,1,1 };
			static const unsigned int sel2[] = { 3,3,3,2 };

			const unsigned	int col0 = sel0[c];
			const unsigned	int col1 = sel1[c];
			const unsigned	int col2 = sel2[c];
			const unsigned	int row0 = sel0[r];
			const unsigned	int row1 = sel1[r];
			const unsigned	int row2 = sel2[r];

			float minor = ( m[col0][row0] * m[col1][row1] * m[col2][row2] +
							 m[col0][row1] * m[col1][row2] * m[col2][row0] +
							 m[col0][row2] * m[col1][row0] * m[col2][row1] ) -
						   ( m[col0][row0] * m[col1][row2] * m[col2][row1] +
							 m[col0][row1] * m[col1][row0] * m[col2][row2] +
							 m[col0][row2] * m[col1][row1] * m[col2][row0] );

			mCofactors(r,c) = minor;
		}
	}

	// Calculate the determinants of the matrix
	// TODO: Fix this, the signs on these are probably a bit wrong.
	float deta = mCofactors[0][0];
	float detb = mCofactors[0][1];
	float detc = mCofactors[0][2];
	float detd = mCofactors[0][3];

	float det = deta - detb + detc - detd;

	if( det == 0.0f ) { return; } // No solution
	float rcp = 1.0f / det;

	// Now form the inverse matrix
	for( unsigned int r=0; r<4; ++r )
	{
		for( unsigned int c=0; c<4; ++c )
		{
			const unsigned	int signmask = ((c + r) & 1) << 31;

			// Now let's adjust the sign of the cofactor:
			union
			{
				float f;
				int	  i;
			} cofactor;

			cofactor.f  = mCofactors[r][c];
			cofactor.i ^= signmask;

			m[r][c] = cofactor.f * rcp;
		}
	}
}

} // Math
} // namespace FriendSmasher

#endif // namespace FriendSmasher_MTX44_OP