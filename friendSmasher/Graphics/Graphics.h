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

			bool Initialise(int w, int h);
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
            
            static float               viewPortRatio ;
			
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