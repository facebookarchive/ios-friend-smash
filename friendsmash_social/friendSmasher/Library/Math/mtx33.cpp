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

#include "mtx33.h"
#include "mtx33_op.h"

namespace FriendSmasher
{
namespace Math
{
	mtx33 mtx33::identity( 1.0f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f,
						   0.0f, 0.0f, 1.0f );

	mtx33::mtx33( const float aa, const float ab, const float ac, const float ba, const float bb, const float bc, const float ca, const float cb, const float cc )
	{
		SET_MATRIX_ROW(0, aa, ab, ac);
		SET_MATRIX_ROW(1, ba, bb, bc);
		SET_MATRIX_ROW(2, ca, cb, cc);
	}

}
}