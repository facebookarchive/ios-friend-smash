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

#include "SpriteManager.h"
#include "Sprite.h"
#include <algorithm>
#include "debug.h"
#include <stdlib.h>

#include "ShaderProgram.h"
#include "GenericShaders.h"
#include "vec2_op.h"
#include "Graphics.h"

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

namespace FriendSmasher
{
    namespace System
    {
        SpriteManager::SpriteManager()
		{
            m_uNumSprites = 0;
			memset( m_apSprites, NULL, sizeof(m_apSprites) );
			
			//
			// Initialise data
			//
			m_vertices		= new Vertex[kMaxNumSprites*4]; // 4 verts per sprite.
			memset( m_vertices, 0, sizeof(Vertex) * kMaxNumSprites * 4 );
			
			m_uIndices		= new u16[kMaxNumSprites*6]; // 6 vert indices per sprite, sounds about right
			
			static u16 pIndices[] = { 0, 1, 2, 1, 2, 3 };
			
			for( unsigned int i=0; i<kMaxNumSprites; ++i )
			{
				m_uIndices[ i * 6 + 0 ] = i * 4 + pIndices[0];
				m_uIndices[ i * 6 + 1 ] = i * 4 + pIndices[1];
				m_uIndices[ i * 6 + 2 ] = i * 4 + pIndices[2];
				m_uIndices[ i * 6 + 3 ] = i * 4 + pIndices[3];
				m_uIndices[ i * 6 + 4 ] = i * 4 + pIndices[4];
				m_uIndices[ i * 6 + 5 ] = i * 4 + pIndices[5];
			}
			
			//
			// Initialise VBOs
			// 
			m_currentBuffer = 0;
			
			glGenBuffers(2, &m_bufferId[0]);
			for( unsigned int i=0; i < 2; ++i )
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_bufferId[i]);
				glBufferData(GL_ARRAY_BUFFER, kMaxNumSprites * 4 * sizeof(Vertex), m_vertices, GL_DYNAMIC_DRAW);
			}
			
			glGenBuffers(1, &m_indexBufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, kMaxNumSprites * 6 * sizeof(u16), m_uIndices, GL_STATIC_DRAW);
			
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			
			m_pSpriteShader = new ShaderProgram(GenericShaders::GetSpriteVertexShader(), GenericShaders::GetSpriteFragmentShader());
			m_pSpriteShader->BindAttributeLocation( kVERTEX_ARRAY, "vPosition" );
			m_pSpriteShader->BindAttributeLocation( kTEXCOORD_ARRAY, "vTexcoord" );
			m_pSpriteShader->BindAttributeLocation( kCOLOR_ARRAY, "vColor" );
            m_mvpUniformLocation = m_pSpriteShader->GetUniformLocation( "g_matWorldViewProj" );
 		}
		
		SpriteManager::~SpriteManager()
		{
			glDeleteBuffers(2, &m_bufferId[0]);
			glDeleteBuffers(1, &m_indexBufferId);
			
			delete m_vertices;
			delete m_uIndices;
			
			delete m_pSpriteShader;
		}
		
		void SpriteManager::Open()
		{
            
		}
    
		
		inline void TransformVert( Math::vec2& vertex, const Math::vec2& position, Math::vec2& offset, const Math::vec2& scale, const Math::vec2& pivot, float ca, float sa, bool usesViewMatrix, const Math::mtx33& mView )
		{
			Math::vec2 temp = offset - pivot;
			
			// Scale around the pivot point
			temp.x *= scale.x;
			temp.y *= scale.y;
			
			// Rotate around the pivot point
			float x = temp.x, y = temp.y;
			temp.x = sa * x - ca * y;
			temp.y = ca * x + sa * y;
			
			vertex = position + temp;
			
			if( usesViewMatrix ) 
			{
				Math::vec2Transform( vertex, mView, vertex );
			}
		}
		
		inline void SetTexture( const Sprite* __restrict sprite )
		{
			glBindTexture(GL_TEXTURE_2D, sprite->GetTextureID() );			
		}
		
		inline void SetBlendMode( const Sprite* __restrict sprite )
		{			
			switch( sprite->GetBlendMode() )
			{
				case Sprite::BLEND_MODULATE:
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glBlendEquation(GL_FUNC_ADD);
					break;
				case Sprite::BLEND_ADDITIVE:
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					glBlendEquation(GL_FUNC_ADD);
					break;
				case Sprite::BLEND_SUBTRACTIVE:
					glEnable(GL_BLEND);
					glBlendFunc(GL_ONE, GL_ONE);
					glBlendEquation(GL_FUNC_SUBTRACT);
				case Sprite::BLEND_PREMULTIPLIEDALPHA:
					glEnable(GL_BLEND);
					glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
					glBlendEquation(GL_FUNC_ADD);
					break;
				case Sprite::BLEND_OFF_MODULATE:
					if( sprite->GetAlpha( ) < 1.0f )
					{
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glBlendEquation(GL_FUNC_ADD);
					}
					else 
					{
						glDisable(GL_BLEND);						
					}
					break;
				case Sprite::BLEND_MASK:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
			}
			
		}
		
		inline void SetRenderMode( const Sprite* __restrict sprite )
		{
			if ( ( Sprite::kFLAG_WRITEMASK & sprite->GetFlags() ) ) {
				Graphics::Instance()->SetSpriteMaskWriteMode();
			}
			else if( Sprite::kFLAG_RECEIVEMASK & sprite->GetFlags() ) {
				Graphics::Instance()->SetSpriteMaskEnabledMode();
			}
			else if( Sprite::kFLAG_SCISSOR & sprite->GetFlags() ) {
				Graphics::Instance()->SetScissorRegion( sprite->GetScissorTopLeft(), 
													   sprite->GetScissorBottomRight() );
			}
			else {
				Graphics::Instance()->SetSpriteDrawingMode();
			}
		}
		
		void SpriteManager::Close(const Math::mtx44& matProjection)
		{
			FBAssert(glGetError() == GL_NO_ERROR);
			FBAssert(m_uNumSprites < kMaxNumSprites);
			
			if( m_uNumSprites == 0 ) { return; }
            
			{				
				SortList();
				
				int idx(0);
				
				for( unsigned int i = 0; i < m_uNumSprites; ++i )
				{
					Sprite* __restrict pSprite = m_apSprites[i];
					
					
                    pSprite->UpdateVertices();
                        
                    for( unsigned int c=0; c < 4; ++c )
                    {
                        m_vertices[idx	].x = pSprite->GetPositions()[c * 2 + 0];
                        m_vertices[idx	].y = pSprite->GetPositions()[c * 2 + 1];
                        
                        m_vertices[idx	].u = pSprite->GetUVs()[c * 2 + 0];
                        m_vertices[idx	].v = pSprite->GetUVs()[c * 2 + 1];
                        
                        m_vertices[idx	].r = pSprite->GetColours()[c * 4 + 0];
                        m_vertices[idx	].g = pSprite->GetColours()[c * 4 + 1];
                        m_vertices[idx	].b = pSprite->GetColours()[c * 4 + 2];
                        m_vertices[idx++].a = pSprite->GetColours()[c * 4 + 3];
                    }
					
				}
                
				glBindBuffer(GL_ARRAY_BUFFER, m_bufferId[m_currentBuffer]);
				glBufferSubData(GL_ARRAY_BUFFER, 0, m_uNumSprites * 4 * sizeof(Vertex), m_vertices);
				
				FBAssert(glGetError() == GL_NO_ERROR);
				
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
				
				m_pSpriteShader->Bind();
				m_pSpriteShader->BindUniform( m_mvpUniformLocation, matProjection );
				
				FBAssert(glGetError() == GL_NO_ERROR);
				
				glEnable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);

				glDepthMask(GL_FALSE);
				
				FBAssert(glGetError() == GL_NO_ERROR);
				
				u32 uOldId		= m_apSprites[0]->GetTextureID();
				u32 uOldBlend	= m_apSprites[0]->GetBlendMode();
				u32 uOldFlags	= m_apSprites[0]->GetFlags();
				
				u32 uNumDrawCalls(0);
				u32 num_to_render(0);
				u32 last_vertex_index(0);
				u32 last_sprite_index(0);
				
				glActiveTexture(GL_TEXTURE0);
				
				FBAssert(glGetError() == GL_NO_ERROR);
				
				glEnableVertexAttribArray(kVERTEX_ARRAY);
				glEnableVertexAttribArray(kTEXCOORD_ARRAY);
				glEnableVertexAttribArray(kCOLOR_ARRAY);
				
				FBAssert(glGetError() == GL_NO_ERROR);
				
				glVertexAttribPointer(kVERTEX_ARRAY,	2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
				glVertexAttribPointer(kTEXCOORD_ARRAY,	2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)8);
				glVertexAttribPointer(kCOLOR_ARRAY,		4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)16);	
				
				FBAssert(glGetError() == GL_NO_ERROR);
				
				SetTexture( m_apSprites[0] );
				FBAssert(glGetError() == GL_NO_ERROR);
				SetBlendMode( m_apSprites[0] );
				FBAssert(glGetError() == GL_NO_ERROR);
				SetRenderMode( m_apSprites[0] );
				
				idx = 0;
				
				for( unsigned int i=0; i<m_uNumSprites; ++i )
				{
					bool dirty = (i == m_uNumSprites-1);
					bool mode_dirty(false);
					bool texture_dirty(false);
					bool blend_dirty(false);
					
					const Sprite* __restrict pSprite = m_apSprites[i];
					
                    if( pSprite->GetFlags() != uOldFlags || pSprite->GetFlags() & Sprite::kFLAG_SCISSOR )
                    {
                        uOldFlags = pSprite->GetFlags();
                        dirty=true;
                        mode_dirty=true;
                    }
                    
                    u32 uTextureID = pSprite->GetTextureID();
                    if( uTextureID != uOldId )
                    {
                        uOldId = uTextureID;
                        dirty=true;
                        texture_dirty=true;
                    }
                    
                    u32 uNewBlend = pSprite->GetBlendMode();
                    if( uNewBlend != uOldBlend || pSprite->GetBlendMode() == Sprite::BLEND_OFF_MODULATE )
                    {
                        uOldBlend = uNewBlend;
                        dirty=true;
                        blend_dirty=true;
                    }
                    
                    //
                    // Something changed - render the old batch
                    //
                    if( dirty )
                    {
                        glDrawElements(GL_TRIANGLES, 6 * num_to_render, GL_UNSIGNED_SHORT, (const GLvoid*)(last_vertex_index * 6 * sizeof(u16)) );
                        
                        ++uNumDrawCalls;
                        num_to_render=0;
                        
                        last_sprite_index = i;
                        last_vertex_index = idx;
                        
                        //
                        // Set render state for next time
                        //
                        if( mode_dirty )	{ SetRenderMode( pSprite ); }
                        if( texture_dirty ) { SetTexture( pSprite );	}
                        if( blend_dirty )	{ SetBlendMode( pSprite );	}
                    }
                    
                    ++num_to_render;
                    ++idx;
					
                    
                    if( i == (m_uNumSprites-1) )
                    {
                        glDrawElements(GL_TRIANGLES, 6 * num_to_render, GL_UNSIGNED_SHORT, (const GLvoid*)(last_vertex_index * 6 * sizeof(u16)) );
                        ++uNumDrawCalls;
                    }
				}
				
				glDisableVertexAttribArray(kVERTEX_ARRAY);
				glDisableVertexAttribArray(kTEXCOORD_ARRAY);
				glDisableVertexAttribArray(kCOLOR_ARRAY);
				
				glVertexAttribPointer(kVERTEX_ARRAY,	2, GL_FLOAT, GL_FALSE, 0, NULL);
				glVertexAttribPointer(kTEXCOORD_ARRAY,	2, GL_FLOAT, GL_FALSE, 0, NULL);
				glVertexAttribPointer(kCOLOR_ARRAY,		4, GL_FLOAT, GL_FALSE, 0, NULL);	
				
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				
				m_currentBuffer ^= 1;
			}
		}
		
		        
        inline int qsortSpriteCompare( const void* v1, const void* v2 )
        {
            const Sprite* s1 = *static_cast< const Sprite* const * >( v1 );
            const Sprite* s2 = *static_cast< const Sprite* const * >( v2 );
            
            if( s1->GetLayer() < s2->GetLayer() )
            {
                return -1;
            }
            else if( s1->GetLayer() > s2->GetLayer() )
            {
                return 1;
            }
            
            if( s1->GetBlendMode() < s2->GetBlendMode() )
            {
                return -1;
            }
            else if( s1->GetBlendMode() > s2->GetBlendMode() )
            {
                return 1;
            }
            
            if( s1->GetFlags() < s2->GetFlags() )
            {
                return -1;
            }
            else if( s1->GetFlags() > s2->GetFlags() )
            {
                return 1;
            }
            
            if( s1->GetTextureID() < s2->GetTextureID() )
            {
                return -1;
            }
            else if( s1->GetTextureID() > s2->GetTextureID() ) 
            {
                return 1;
            }
            
            return 0;
        }			
        
        void SpriteManager::SortList()
        {	
            qsort( (void*)m_apSprites, m_uNumSprites, sizeof(Sprite*), qsortSpriteCompare );
        }
        
        void SpriteManager::AddSprite(Sprite* pSprite)
        {
            FBAssert(m_uNumSprites < kMaxNumSprites);
            m_apSprites[m_uNumSprites] = pSprite;
            ++m_uNumSprites;
        }
        
        
        void SpriteManager::RemoveSprite(Sprite* pSprite)
        {
            for( unsigned int i=0; i<m_uNumSprites; ++i )
            {
                if( m_apSprites[i] == pSprite )
                {
                    m_apSprites[i] = m_apSprites[m_uNumSprites-1];
                    m_apSprites[--m_uNumSprites] = NULL;
                    m_apSprites[m_uNumSprites] = 0;
                    return;
                }
            }
            
            return;
        }
        
    }
}

