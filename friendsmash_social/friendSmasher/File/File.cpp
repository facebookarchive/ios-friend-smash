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

#include "File.h"
#include "debug.h"

#include <alloca.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <aio.h>
#include <errno.h>

namespace FriendSmasher 
{
    namespace System
    {
        File* File::ms_Instance=NULL;

        File* File::Instance()
        {
            if( !ms_Instance ) {
                ms_Instance = new File;
            }

            return ms_Instance;
        }

        File::File()
        {
        }

        File::~File()
        {
        }

        bool File::Initialise()
        {
            return true;
        }

        void File::Update()
        {
        }

        bool File::Uninitialise()
        {
            return true;
        }
        
        u32 File::GetFileSizeBytes(const char* filename, bool bCompressed)
        {
            u32 sz=0;
            char bundle_path[512];
            GetFilePath(bundle_path, filename);
            
            FILE* fptr = fopen(bundle_path, "rb");
            
            if( fptr != NULL )
            {
                sz = GetFileSizeBytes(fptr, bCompressed);
                
                fclose(fptr);
            }
            
            return sz;
        }
        
        u32 File::GetFileSizeBytes(FILE* fptr, bool bCompressed)
        {
                fseek(fptr, 0, SEEK_END);
                u32 sz = ftell(fptr);
                fseek(fptr, 0, SEEK_SET);
                return sz;
        }
        
        // Simple synchronous (blocking) file load.
        unsigned char* File::LoadFile(const char* filename)
        {
            unsigned char* pMem = NULL;
            
            u32 sz = GetFileSizeBytes(filename);
            if( sz > 0 )
            {
                char bundle_path[512];
                GetFilePath(bundle_path, filename);
                
                FILE* fptr = fopen(bundle_path, "rb");
                if( fptr ) 
                {
                    pMem = new unsigned char[sz];
                    fread(pMem, 1, sz, fptr);
                    fclose( fptr );
                }
            }
            
            return pMem;
        }
        
	}
}