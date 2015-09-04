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

#ifndef FRIENDSMASHER_SPRITE
#define FRIENDSMASHER_SPRITE

#include "type_defs.h"
#include "vec2.h"
#include "vec3.h"


namespace FriendSmasher
{
	namespace System
	{
        
        void TransformVert( Math::vec2&, 
                           const Math::vec2&, 
                           Math::vec2&, 
                           const Math::vec2&,
                           const Math::vec2&, 
                           float, 
                           float,
                           bool);
        
        class TextureResource;
        
        class Sprite
        {
        public:
            
            enum eBlendMode
			{
				BLEND_MODULATE,
				BLEND_ADDITIVE,
				BLEND_SUBTRACTIVE,
				BLEND_PREMULTIPLIEDALPHA,
				BLEND_OFF_MODULATE,				// Blend is off unless alpha < 1.0
				BLEND_MASK
			};
            
            enum eFlags
			{
				kFLAG_WRITEMASK		= (1 << 0),			// Writes a mask
				kFLAG_RECEIVEMASK	= (1 << 1),			// Receives the stencil mask
				kFLAG_SCROLL_MODE	= (1 << 2),			// View matrix affects this one
				kFLAG_SCISSOR		= (1 << 3),			// Does this sprite have a scissor region applied?
			};
			
			// The parallax index sits in the top 4 bits of the flags.
			enum {
				kPARALLAX_INDEX_MASK = 0xf0000000,
				kPARALLAX_INDEX_SHIFT = 28
			};
            
            
            Sprite(TextureResource* pResource);
            Sprite();		// Allow default constructor for creating arrays of these, drawing is defaulted to off and cant be turned on unless a texture resource is defined.
            
            ~Sprite();
            
            TextureResource*	GetTextureResource()    const { return m_pResource; }
            const Math::vec2&	GetPosition()			const { return m_vPosition; }
			const Math::vec2&	GetScale()				const { return m_vScale; }
			const Math::vec2&	GetDimensions()			const { return m_vDimensions; }
			const Math::vec2&	GetPivot()				const { return m_vPivot; }
			const Math::vec2&	GetTopLeftUV()			const { return m_vUVTopLeft; }
			const Math::vec2&	GetBottomRightUV()		const { return m_vUVBottomRight; }
			const Math::vec3&	GetColour()				const { return m_vColour; }
			const Math::vec2&	GetScissorTopLeft()		const { return m_vScissorTopLeft; }
			const Math::vec2&	GetScissorBottomRight()	const { return m_vScissorBottomRight; }
            
            
 			const unsigned int	GetLayer()              const   { return m_uLayer; }
            const eBlendMode	GetBlendMode()          const   { return m_kBlendMode; }
            u32					GetFlags()              const   { return m_uFlags; }
            const float			GetAlpha()              const   { return m_fAlpha; }
            
            const bool			GetDraw()               const   { return m_bDraw; }
			const float			GetWidth()              const   { return m_vDimensions.x; }
			const float			GetHeight()             const   { return m_vDimensions.y; }
			const float			GetRotation()           const   { return m_fRotation; }

            u32                 GetTextureID()          const;
            
            const float*        GetPositions()                  { return &m_positions[0];	}
			const float*        GetUVs()                        { return &m_uvs[0];			}
			const float*        GetColours()                    { return &m_colours[0];		}
            
            
            void                SetTextureResource(TextureResource* pResource) { m_pResource = pResource; }
            void				SetPosition(const Math::vec2& v)	{ m_vPosition = v; }
			void				SetScale(const Math::vec2& v)		{ m_vScale = v; }
			void				SetPivot(const Math::vec2& v)		{ m_vPivot = v; }
			void				SetDimensions(const Math::vec2& v)	{ m_vDimensions = v; }
            void                SetDraw(const bool bDraw);
            void				SetLayer(const unsigned int uLayer) { m_uLayer = uLayer; }
            void				SetRotation( const float fRotation ) { m_fRotation = fRotation; }
            void				SetBlendMode( const eBlendMode blendMode ) { m_kBlendMode = blendMode; }
            void				SetAlpha(const float fAlpha)		{ m_fAlpha = fAlpha; }
            void				SetColour(const Math::vec3& v)		{ m_vColour = v; }

            
            void                UpdateVertices();
            
            bool                IsPointInside( const Math::vec2 &vPoint, float fRadius);

            void				SetUVTopLeft(const Math::vec2& v) { m_vUVTopLeft = v; }
			void				SetUVBottomRight(const Math::vec2& v) { m_vUVBottomRight = v; }
            


			

        private:
            
            TextureResource*    m_pResource;
            
            
            unsigned int        m_uLayer;
            eBlendMode          m_kBlendMode;
            float               m_fAlpha;
            u32                 m_uFlags;
            float               m_fRotation;
            bool                m_bDraw;
                
            Math::vec2          m_vPosition;
			Math::vec2          m_vScale;
			Math::vec2          m_vDimensions;
			Math::vec2          m_vPivot;
			Math::vec2          m_vUVTopLeft;
			Math::vec2          m_vUVBottomRight;
			Math::vec3          m_vColour;

            
            Math::vec2          m_vScissorTopLeft;
			Math::vec2          m_vScissorBottomRight;
            
            float m_positions[8];
			float m_uvs[8];
			float m_colours[16];

            
            inline unsigned int	GetParallaxIndex() const { return (m_uFlags & kPARALLAX_INDEX_MASK) >> kPARALLAX_INDEX_SHIFT; }
			inline void			SetParallaxIndex(int idx) 
			{ 
				m_uFlags &= ~kPARALLAX_INDEX_MASK;				// Clear existing bits
				m_uFlags |= ( (idx << kPARALLAX_INDEX_SHIFT) & kPARALLAX_INDEX_MASK); 
			}
			
            
        };
	}
}

#endif // FRIENDSMASHER_SPRITE