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

#ifndef __SHADERPROGRAM_H_INCLUDED__
#define __SHADERPROGRAM_H_INCLUDED__


#include "type_defs.h"
#include "math_lib.h"
#include "OpenGLES/ES2/gl.h"
#include "OpenGLES/ES2/glext.h"

namespace FriendSmasher
{
    namespace System
    {
	
        class ShaderProgram
        {
        public:
            ShaderProgram( const char* vertexProgram, const char* fragmentProgram );
            ~ShaderProgram( );
            
            //
            // Attributes
            //
            void BindAttributeLocation( u32 location, const char* name );
            
            //
            // Uniforms / Constants
            //
            int	 GetUniformLocation( const char* name );
            
            void BindUniform( const char* name, const float			val );
            void BindUniform( const char* name, const int			val );
            void BindUniform( const char* name, const u32			val );
            void BindUniform( const char* name, const Math::vec2&	val );
            void BindUniform( const char* name, const Math::vec3&	val );
            void BindUniform( const char* name, const Math::vec4&	val );
            void BindUniform( const char* name, const Math::mtx33&	val );
            void BindUniform( const char* name, const Math::mtx44&	val );
            
            void BindUniform( int location,     const float         val );
            void BindUniform( int location,     const int           val );
            void BindUniform( int location,     const u32           val );
            void BindUniform( int location,     const Math::vec2&   val );
            void BindUniform( int location,     const Math::vec3&   val );
            void BindUniform( int location,     const Math::vec4&   val );
            void BindUniform( int location,     const Math::mtx33&  val );
            void BindUniform( int location,     const Math::mtx44&  val );
            
            void Bind();
            void Unbind();
            
        private:

            ShaderProgram() {};
            
            GLuint m_vertexShader;
            GLuint m_fragmentShader;
            
            GLuint m_shaderProgram;
        };
    }	
}

#endif