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

#ifndef FRIENDSMASHER_TEXTURERESOURCE
#define FRIENDSMASHER_TEXTURERESOURCE

#include "type_defs.h"
#include <string>

namespace FriendSmasher
{
	namespace System
	{
        class TextureResource
        {
        public:
            TextureResource();
            ~TextureResource();
    	
            bool CreateFromFile(const char* filename);

            bool CreateFromFBID(u64 uID, u32 uWidth, u32 uHeight);
            bool CreateFromURL(std::string url, u32 uWidth, u32 uHeight);
            
            u32 GetTextureID() const {return m_uTextureID[0];}
            
            u32 GetWidth() const;
			u32 GetHeight() const;
            bool GetIsReady() const { return m_bReady; }
            
        private:
            
            
            bool    m_bReady;
            u32     m_uWidth;
            u32     m_uHeight;
            u32		m_uTextureID[1];  
        };
        
	}
}

#endif // FRIENDSMASHER_SPRITE