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

#ifndef FRIENDSMASHER_DEBUG
#define FRIENDSMASHER_DEBUG

#include "type_defs.h"
#include <map>

//#if defined (1)
#define FBAssert(expression)            if (!(expression)){ FriendSmasher::Debug::Assert((#expression), (__FILE__), __LINE__); }
#define FBAssertMsg(expression, msg)	if (!(expression)){ FriendSmasher::Debug::AssertMsg((msg), (#expression), (__FILE__), __LINE__); }
//#else
//#define FBAssert(expression)
//#define FBAssertMsg(expression, msg)
//#endif

// TODO macro
// usage:
// #pragma TODO ("fix this")
// outputs the filename, line & message in the build output, which can be double clicked to go its location

namespace FriendSmasher
{
	class Debug
	{
	public:
		static void Assert(const char* Expression, const char* file, unsigned int line);
		static void AssertMsg(const char* msg, const char* Expression, const char* file, unsigned int line);
	
	private:

		static std::map<u32, u32> m_sPreviousExpressionHashes;
	
	};
}

#endif // namespace FriendSmasher_DEBUG