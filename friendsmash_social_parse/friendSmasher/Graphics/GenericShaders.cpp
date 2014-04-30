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


