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

#ifndef FRIENDSMASHER_SPRITEMANAGER
#define FRIENDSMASHER_SPRITEMANAGER

#include "vec2.h"
#include "vec4.h"
#include "mtx44.h"
#include "type_defs.h"

namespace FriendSmasher
{
	namespace System
	{
		class Sprite;
		class ShaderProgram;
		
		class SpriteManager
		{
		public:
			SpriteManager();
			~SpriteManager();
            
            enum eLocationStream
			{
				kVERTEX_ARRAY = 0,
				kTEXCOORD_ARRAY,
				kCOLOR_ARRAY
			};
			
            void					AddSprite( Sprite* pSprite );
			void					RemoveSprite( Sprite* pSprite );
            
			virtual void			Open();
			virtual void			Close(const Math::mtx44& projection);
			
		private:
            
            void SortList();
			
			struct Vertex
			{
				float x, y;
				float u, v;
				float r, g, b, a;
			};
			
			Vertex* m_vertices;
			
			unsigned int m_bufferId[2];
			unsigned int m_indexBufferId;
			unsigned int m_currentBuffer;
			
			u16* m_uIndices;
			
            static const int kMaxNumSprites = 1024;
			
			Sprite*					m_apSprites[kMaxNumSprites];					/// The array of sprites 
			unsigned int			m_uNumSprites;									/// The number of sprites that need to be drawn this frame.
			
			
			ShaderProgram* m_pSpriteShader;		
            int            m_mvpUniformLocation;
		};
		
	}
}


#endif // FRIENDSMASHER_SPRITEMANAGER_IPHONE