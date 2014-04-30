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