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

#ifndef FRIENDSMASHER_GRAPHICS
#define FRIENDSMASHER_GRAPHICS

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "debug.h"
#include "mtx33.h"
#include "mtx33_op.h"

#include "SpriteManager.h"


namespace FriendSmasher
{
	namespace System
	{

		class SpriteManager;

		class Graphics
		{
		public:
			
            static Graphics* Instance();
            ~Graphics();
			
			static void CreateInstance();

			bool Initialise();
			void Update();
			bool Uninitialise();

			void BeginRender();
			void EndRender();

			void Begin2DRender();
			void Render2D();
			void End2DRender();

			void Render3D();
			void PostRender3D();

			SpriteManager*	GetSpriteManager() const;
			
			void Set2DViewMatrix( const Math::mtx33& mView, int index = 0);
			void Get2DViewMatrix( Math::mtx33& mOutput, int index = 0 ) const;
			void Reset2DViewMatrix(int index = 0);
			void ResetAll2DViewMatrices();
			
			void				SetSpriteDrawingMode();
			void				SetSpriteMaskWriteMode();
			void				SetSpriteMaskEnabledMode();
			
			void				SetScissorRegion( const Math::vec2& vTopLeft, const Math::vec2& vBottomRight );
			
			unsigned int		GetFrameBuffer() const;
            int                 GetBackbufferWidth() const;
            int                 GetBackbufferHeight() const;
			
		protected:

			Graphics();
			static Graphics* ms_Instance;

			static const unsigned int   kMaxNumViewMatrices = 16;
			Math::mtx33                 m_2DViewMatrix[kMaxNumViewMatrices];
			
			SpriteManager*              m_pSpriteManager;
            
            Math::mtx44					m_projMatrix;
		};
	}
}

#endif // FREIENDSMASHER_GRAPHICS