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

#ifndef FRIENDSMASHER_FILE
#define FRIENDSMASHER_FILE

#include <aio.h>
#include <stdio.h>
#include "type_defs.h"
#include "File_iPhoneCore.h"

namespace FriendSmasher
{
	namespace System
	{
            typedef u32 FileHandle;

            class File
            {
            public:

                    static File* Instance();
                    virtual ~File();

                    bool Initialise();
                    void Update();
                    bool Uninitialise();
                    
                    virtual const char* GetName() { return "File"; }

                    u32 GetFileSizeBytes(const char* filename, bool bCompressed=false);
                    u32 GetFileSizeBytes(FILE* fptr, bool bCompressed=false);
                    u32 GetFileSizeBytes(FileHandle handle);

                    // Synchronous file handling
                    unsigned char* LoadFile(const char* filename);

            private:
                
                    File();
                    static File* ms_Instance;
                
                    FILE* OpenFile(char* filename);
                
            };
	}
}

#endif // FRIENDSMASHER_FILE