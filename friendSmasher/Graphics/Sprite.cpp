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

#include "Sprite.h"
#include "Graphics.h"
#include "vec2_op.h"
#import "TextureResource.h"

namespace FriendSmasher
{
	namespace System
	{
        Sprite::Sprite(TextureResource* pResource) :
        m_pResource(pResource),
        m_vPosition(Math::vec2(0.0f, 0.0f) ),
        m_vScale(Math::vec2(1.0f, 1.0f) ),
        m_vUVTopLeft(Math::vec2(0.f, 0.f)),
        m_vUVBottomRight(Math::vec2(1.f, 1.f)),
        m_vColour(Math::vec3(1.f,1.f,1.f)),
        m_fAlpha(1.f),
        m_fRotation(0.0f),
        m_uFlags(0),
        m_uLayer(0),
        m_bDraw(true),
        m_vDimensions(0.f, 0.f),
        m_vPivot(0.0f, 0.0f),
        m_kBlendMode(BLEND_MODULATE)
        {
            if (pResource) {
            m_vDimensions = Math::vec2(static_cast<float>(m_pResource->GetWidth()) * m_vUVBottomRight.x, static_cast<float>(m_pResource->GetHeight()) * m_vUVBottomRight.y);
            }
            
            Graphics::Instance()->GetSpriteManager()->AddSprite( this );
        }
        
        Sprite::Sprite() :
        m_pResource(NULL),
        m_vPosition(Math::vec2(0.0f, 0.0f) ),
        m_vScale(Math::vec2(1.0f, 1.0f) ),
        m_vUVTopLeft(Math::vec2(0.f, 0.f)),
        m_vUVBottomRight(Math::vec2(1.f, 1.f)),
        m_vColour(Math::vec3(1.f,1.f,1.f)),
        m_fAlpha(1.f),
        m_fRotation(0.0f),
        m_uFlags(0),
        m_uLayer(0),
        m_bDraw(false),
        m_vDimensions(0.f, 0.f),
        m_vPivot(0.0f, 0.0f),
        m_kBlendMode(BLEND_MODULATE)
		{
			m_vScissorTopLeft = Math::vec2(0.0f, 0.0f);
			m_vScissorBottomRight = Math::vec2(1.0f, 1.0f);
		}
        
        
        Sprite::~Sprite()
        {
            if (m_bDraw) {
				Graphics::Instance()->GetSpriteManager()->RemoveSprite( this );
			}
        }
        
        
        u32 Sprite::GetTextureID() const
		{	
            
            if( m_pResource )
			{
				return m_pResource->GetTextureID();
			}
            
			return 0;
		}
        
        void TransformVert( Math::vec2& vertex, const Math::vec2& position, Math::vec2& offset, const Math::vec2& scale, const Math::vec2& pivot, float ca, float sa, bool usesViewMatrix, Math::mtx33& mView )
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
            
			if( usesViewMatrix ) {
				Math::vec2Transform( vertex, mView, vertex );
			}
		}
        
       
        bool Sprite::IsPointInside( const Math::vec2 &vPoint, float fRadius)
		{
			float fWidth = GetWidth();
			float fHeight = GetHeight();
            
			Math::vec2 vOffsets[] = 
			{
				Math::vec2(0.0f, 0.0f),
				Math::vec2(fWidth, 0.0f),
				Math::vec2(fWidth, fHeight),
				Math::vec2(0.0f, fHeight)
			};
            
			Math::vec2 vPositions[4];
			
			Math::mtx33 mView;
			Graphics::Instance()->Get2DViewMatrix( mView, GetParallaxIndex( ) );
            
			float sa = sinf( GetRotation() );
			float ca = cosf( GetRotation() );
            
			for( unsigned int j=0; j<4; ++j ) {
				TransformVert(vPositions[j], GetPosition(), vOffsets[j], GetScale(), GetPivot(), sa, ca, !!(m_uFlags & kFLAG_SCROLL_MODE), mView );
			}
			
			// Get the four edge vectors of our sprite
			Math::vec2 e0 = vPositions[1] - vPositions[0];
			Math::vec2 e1 = vPositions[2] - vPositions[1];
			Math::vec2 e2 = vPositions[3] - vPositions[2];
			Math::vec2 e3 = vPositions[0] - vPositions[3];
			
			// Now get the four normal vectors of our sprite edges
			Math::vec2 n0, n1, n2, n3;
			
			Math::vec2RotateDegrees( n0, e0, 90.0f );
			Math::vec2RotateDegrees( n1, e1, 90.0f );
			Math::vec2RotateDegrees( n2, e2, 90.0f );
			Math::vec2RotateDegrees( n3, e3, 90.0f );
			
			// Get the four vectors from the point to the corners
			Math::vec2 p0 = vPoint - vPositions[0];
			Math::vec2 p1 = vPoint - vPositions[1];
			Math::vec2 p2 = vPoint - vPositions[2];
			Math::vec2 p3 = vPoint - vPositions[3];
            
            Math::vec2Normalize(n0, n0);
            Math::vec2Normalize(n1, n1);
            Math::vec2Normalize(n2, n2);
            Math::vec2Normalize(n3, n3);
            
            float threshold = -(fRadius);
			
			float d0 = Math::vec2DotProduct( n0, p0 );
			if( d0 < threshold ) return false;
			
			float d1 = Math::vec2DotProduct( n1, p1 );
			if( d1 < threshold ) return false;
			
			float d2 = Math::vec2DotProduct( n2, p2 );
			if( d2 < threshold ) return false;
			
			float d3 = Math::vec2DotProduct( n3, p3 );
			if( d3 < threshold ) return false;
            
			return true;
		}
        
        
        void Sprite::SetDraw( const bool bDraw )
		{
			if (m_bDraw != bDraw)
			{
				m_bDraw = bDraw;
				                
				if (m_bDraw && m_pResource != NULL) {
					Graphics::Instance()->GetSpriteManager()->AddSprite( this );
				}
				else {
					Graphics::Instance()->GetSpriteManager()->RemoveSprite( this );
				}
			}
		}
        
        
		void Sprite::UpdateVertices()
		{
			float fWidth = GetWidth();
			float fHeight = GetHeight();
			
			Math::vec2 vOffsets[] = 
			{
				Math::vec2(0.0f, 0.0f),
				Math::vec2(fWidth, 0.0f),
				Math::vec2(0.0f, fHeight),
				Math::vec2(fWidth, fHeight)
			};
			
			float sa = sinf( GetRotation() );
			float ca = cosf( GetRotation() );
			
			Math::vec2 uvs[4] =
			{
				Math::vec2( m_vUVTopLeft.x,		m_vUVTopLeft.y ),
				Math::vec2( m_vUVBottomRight.x,	m_vUVTopLeft.y ),
				Math::vec2( m_vUVTopLeft.x,		m_vUVBottomRight.y ),
				Math::vec2( m_vUVBottomRight.x, m_vUVBottomRight.y )
			};
			
			Math::mtx33 mView;
			Graphics::Instance()->Get2DViewMatrix( mView, GetParallaxIndex( ) );
			
			for( unsigned int i=0; i<4; ++i )
			{
				Math::vec2 position;
				TransformVert(position, GetPosition(), vOffsets[i], GetScale(), GetPivot(), sa, ca, !!(m_uFlags & kFLAG_SCROLL_MODE), mView );
				
				m_positions[i*2+0] = position.x;
				m_positions[i*2+1] = position.y;
				
				m_uvs[i*2+0] = uvs[i].x;
				m_uvs[i*2+1] = uvs[i].y;
				
				m_colours[i*4+0] = m_vColour.x;
				m_colours[i*4+1] = m_vColour.y;
				m_colours[i*4+2] = m_vColour.z;
				m_colours[i*4+3] = m_fAlpha;
			}
		}
         
	}
}
