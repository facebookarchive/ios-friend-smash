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