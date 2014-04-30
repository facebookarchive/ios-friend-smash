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