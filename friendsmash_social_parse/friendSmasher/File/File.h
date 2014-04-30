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