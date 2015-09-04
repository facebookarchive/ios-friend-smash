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