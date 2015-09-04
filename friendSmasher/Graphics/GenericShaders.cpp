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

#include "GenericShaders.h"

namespace FriendSmasher
{
    namespace GenericShaders
    {
        // Generic sprite vertex shader
        static const char* g_SpriteVertexShader = 
        "	varying vec4 oColor;													\n"
        "	varying vec2 oTexcoord;													\n"
        "	attribute vec4 vColor;													\n"
        "	attribute vec2 vTexcoord;												\n"
        "	attribute vec4 vPosition;												\n"
        "	uniform mat4 g_matWorldViewProj;										\n"
        "	void main ()															\n"
        "	{																		\n"
        "		gl_Position = (vPosition * g_matWorldViewProj);						\n"
        "		oColor = vColor;													\n"
        "		oTexcoord = vTexcoord;												\n"
        "	}																		\n"
        "																			\n";
                
        // Generic sprite pixel shader
        static const char* g_SpriteFragmentShader = 
        "	precision lowp float;													\n"	
        "	varying vec4 oColor;													\n"
        "	varying vec2 oTexcoord;													\n"
        "	uniform sampler2D texture;												\n"
        "																			\n"
        "	void main ()															\n"
        "	{																		\n"
        "		gl_FragColor = (texture2D (texture, oTexcoord) * oColor);			\n"
        "	}																		\n"
        "																			\n";

        const char* GetSpriteVertexShader()
        {
            return g_SpriteVertexShader;
        }

        const char* GetSpriteFragmentShader()
        {
            return g_SpriteFragmentShader;
        }
        
    }
}


