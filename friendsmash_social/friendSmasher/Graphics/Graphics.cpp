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

#include "Graphics.h"
#include "mtx44_op.h"

namespace FriendSmasher
{
	namespace System
	{
		Graphics* Graphics::ms_Instance = NULL;

        Graphics* Graphics::Instance()
		{
			if( !ms_Instance ) {
				ms_Instance = new Graphics;
			}
			return ms_Instance;
		}
        
		Graphics::Graphics() :
			m_pSpriteManager(NULL)
		{
		}
		
		Graphics::~Graphics()
		{
		}
        
		bool Graphics::Initialise()
		{
			FBAssert(ms_Instance);
            m_pSpriteManager = new SpriteManager();
			ResetAll2DViewMatrices();
            
			return true;
		}
		
		unsigned int Graphics::GetFrameBuffer() const
		{
            return 0;
		}
                
        int Graphics::GetBackbufferWidth() const
        {
            // Forcing iPhone5
            return 640;
        }
        
        int Graphics::GetBackbufferHeight() const
        {
            // Forcing iPhone5
            return 1136;
        }
		
		void Graphics::Update()
		{	
            BeginRender();
			Render3D();
            Begin2DRender();
            Render2D();
			End2DRender();
            PostRender3D();
            EndRender();
        }
		
		bool Graphics::Uninitialise()
		{
			delete m_pSpriteManager;
			
			return true;
		}
		
		void Graphics::BeginRender()
		{
            glClearColor(0.f, 0.f, 0.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, GetBackbufferWidth(), GetBackbufferHeight());
		}
		
		void Graphics::EndRender()
		{
			
		}
		
		void Graphics::Begin2DRender()
		{
            
			float w = GetBackbufferWidth();
			float h = GetBackbufferHeight();
			 
            
            Math::mtx44MakeOrthographicOffCenterLH(m_projMatrix, 
											 0.0f, w, 
											 0.0f, h, 
											 0.0f, 1.0f);
		}
		
		void Graphics::Render2D()
		{
			m_pSpriteManager->Open();
			m_pSpriteManager->Close(m_projMatrix);
		}
		
		void Graphics::End2DRender()
		{
			return;
		}
		
		void Graphics::Render3D()
		{

		}
		
		void Graphics::PostRender3D()
		{

		}
		
		void Graphics::SetSpriteDrawingMode()
		{
            glDisable(GL_SCISSOR_TEST);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);			
		}
		
		void Graphics::SetSpriteMaskWriteMode()
		{
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
		}
		
		void Graphics::SetSpriteMaskEnabledMode()
		{
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
		}
		
		void Graphics::SetScissorRegion(const Math::vec2& tl, const Math::vec2& br)
		{
			glEnable(GL_SCISSOR_TEST);
			
			float w = 0.f;
			float h = 0.f;
                
			w = static_cast<float>(GetBackbufferWidth());
			h = static_cast<float>(GetBackbufferHeight());
			            
			// Note the transpose of x/y on iPhone hardware	
			float tlx = (1.0f - br.y) * h;
			float tly = (1.0f - br.x) * w;
			
			float ox = (br.y - tl.y) * h;
			float oy = (br.x - tl.x) * w;
            			
			if (ox < 0.f) { ox = 0.f; }
			if (oy < 0.f) { oy = 0.f; }
			
			glScissor( static_cast<GLint>(tlx),
                      static_cast<GLint>(tly),
                      static_cast<GLsizei>(ox),
                      static_cast<GLsizei>(oy) );	
		}
        
		SpriteManager* Graphics::GetSpriteManager() const
		{
			return m_pSpriteManager;
		}

		void Graphics::Set2DViewMatrix( const Math::mtx33& mView, int index /* = 0 */ )
		{
			m_2DViewMatrix[index] = mView;
		}
        
		void Graphics::Get2DViewMatrix( Math::mtx33& mOutput, int index /* = 0 */ ) const
		{
			mOutput = m_2DViewMatrix[index];
		}

		void Graphics::Reset2DViewMatrix(int index /* = 0 */)
		{
			mtx33MakeIdentity(m_2DViewMatrix[index]);
		}
		
		void Graphics::ResetAll2DViewMatrices()
		{
			for( unsigned int i=0; i<kMaxNumViewMatrices; ++i ) {
				Reset2DViewMatrix(i);
			}
		}
        
	}
}


