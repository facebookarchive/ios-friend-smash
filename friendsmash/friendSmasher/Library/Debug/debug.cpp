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

#include <stdio.h>

#include "debug.h"

namespace FriendSmasher
{
	std::map<u32, u32> Debug::m_sPreviousExpressionHashes;

	void Debug::Assert(const char* Expression, const char* file, unsigned int line)
	{
		//char buf[1024];
		//sprintf(buf, "*Warning* Assert Failed: %s\nOccurred at:\nFile: %s\nLine: %d\n", Expression, file, line);
		//namespace FriendSmasherLogMessage(buf);
		abort();
    }

	void Debug::AssertMsg(const char* msg, const char* Expression, const char* file, unsigned int line)
	{
    	abort();
    }
}