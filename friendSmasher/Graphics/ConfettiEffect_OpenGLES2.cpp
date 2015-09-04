#include <stdlib.h>
#include "ConfettiEffect_OpenGLES2.h"
#include "ShaderProgram.h"
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>


//------------------------------------------------------------------------

using namespace FriendSmasher;
using namespace FriendSmasher::Math;

//------------------------------------------------------------
// Shaders

//------------------------------------------------------------
// Billboard vertex shader
static const char* s_ConfettiVertexShader =
"	uniform mat4 g_worldViewProjectionMatrix;														\n"
"	attribute vec4 vPosition;																		\n"
"	attribute vec2 vTexcoords;																		\n"
"	attribute vec4 vColor;																			\n"
"	attribute vec3 vNormal;																			\n"
"	varying vec2 oTexcoords;																		\n"
"	varying vec4 oColor;																			\n"
"																									\n"
"	void main()																						\n"
"	{																								\n"
"		gl_Position		= vPosition * g_worldViewProjectionMatrix;									\n"
"		oTexcoords		= vTexcoords;																\n"
"		oColor			= vColor;																	\n"
"	}																								\n"
"																									\n";

//------------------------------------------------------------
// Billboard fragment shader
static const char* s_ConfettiFragmentShader =
"	precision lowp float;																			\n"
"	uniform sampler2D texture;																		\n"
"	varying vec2 oTexcoords;																		\n"
"	varying vec4 oColor;																			\n"
"																									\n"
"	void main()																						\n"
"	{																								\n"
"		vec4 texture_color = texture2D(texture, oTexcoords);										\n"
"		gl_FragColor = texture_color * oColor;														\n"
"	}																								\n"
"																									\n";

//------------------------------------------------------------
// Billboard fragment shader (no texture)
static const char* s_ConfettiFragmentShaderNoTexture = 
"	precision lowp float;																			\n"
"	uniform sampler2D texture;																		\n"
"	varying vec2 oTexcoords;																		\n"
"	varying vec4 oColor;																			\n"
"																									\n"
"	void main()																						\n"
"	{																								\n"
"		gl_FragColor = oColor;																		\n"
"	}																								\n"
"																									\n";

//------------------------------------------------------------------------

static void SetBlendMode( System::Sprite::eBlendMode mode )
{
	switch( mode )
	{
		case System::Sprite::BLEND_MODULATE:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
			break;
		case System::Sprite::BLEND_ADDITIVE:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glBlendEquation(GL_FUNC_ADD);
			break;
		case System::Sprite::BLEND_SUBTRACTIVE:
			glBlendFunc(GL_ONE, GL_ONE);
			glBlendEquation(GL_FUNC_SUBTRACT);
			break;
		case System::Sprite::BLEND_MASK:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
        case System::Sprite::BLEND_PREMULTIPLIEDALPHA:
            break;
        case System::Sprite::BLEND_OFF_MODULATE:
            break;
	}
}

//------------------------------------------------------------------------

ConfettiEffect_OpenGLES2::ConfettiEffect_OpenGLES2(unsigned int max_particles) :
	ConfettiEffect(max_particles)
{
	m_confettiProgram		= new System::ShaderProgram( s_ConfettiVertexShader, s_ConfettiFragmentShader );
	m_confettiProgram->BindAttributeLocation(kATTRIB_POSITION,  "vPosition" );
	m_confettiProgram->BindAttributeLocation(kATTRIB_COLOR,		"vColor"    );
	m_confettiProgram->BindAttributeLocation(kATTRIB_TEXCOORDS, "vTexcoords");
	
	m_confettiProgramNoTexture	= new System::ShaderProgram( s_ConfettiVertexShader, s_ConfettiFragmentShaderNoTexture );
	m_confettiProgramNoTexture->BindAttributeLocation(kATTRIB_POSITION, "vPosition"	);
	m_confettiProgramNoTexture->BindAttributeLocation(kATTRIB_COLOR,	"vColor"	);
	m_confettiProgramNoTexture->BindAttributeLocation(kATTRIB_TEXCOORDS,"vTexcoords");		
}

//------------------------------------------------------------------------

ConfettiEffect_OpenGLES2::~ConfettiEffect_OpenGLES2()
{
	delete m_confettiProgram;
	m_confettiProgram = NULL;
	
	delete m_confettiProgramNoTexture;
	m_confettiProgramNoTexture = NULL;	
}

//------------------------------------------------------------------------

void ConfettiEffect_OpenGLES2::Render( const Math::mtx44& view_matrix, const Math::mtx44& proj_matrix )
{
	if( m_numParticles == 0 ) {
		return;
	}
	
	struct ParticleVertex
	{
		vec3 position;
		vec4 colour;
		vec2 texcoords;
	};
	
	ParticleVertex* vertices = (ParticleVertex*)alloca( m_numParticles * 4 * sizeof(ParticleVertex) );
	unsigned short* indices	 = (unsigned short*)alloca( m_numParticles * 6 * sizeof(unsigned short) );
	
	SetBlendMode( m_blendMode );
	
	glEnableVertexAttribArray(kATTRIB_POSITION);
	glEnableVertexAttribArray(kATTRIB_COLOR);
	glEnableVertexAttribArray(kATTRIB_TEXCOORDS);
	
	glVertexAttribPointer(kATTRIB_POSITION,	3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), &vertices[0].position );
	glVertexAttribPointer(kATTRIB_COLOR,	4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), &vertices[0].colour	 );
	glVertexAttribPointer(kATTRIB_TEXCOORDS,2, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), &vertices[0].texcoords);
	
	const mtx44& m = view_matrix;
	
	vec3 camera_right	( m[0][0], m[1][0], m[2][0] );
	vec3 camera_up		( m[0][1], m[1][1], m[2][1] );
	vec3 camera_forwards( m[0][2], m[1][2], m[2][2] );
	
	mtx44 wvp = proj_matrix * view_matrix;
	
	if( m_textureResource )
	{
		m_confettiProgram->Bind();
		m_confettiProgram->BindUniform( "g_worldViewProjectionMatrix", wvp );
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureResource->GetTextureID());
	}
	else {
		m_confettiProgramNoTexture->Bind();
		m_confettiProgramNoTexture->BindUniform( "g_worldViewProjectionMatrix", wvp );
	}
	
	for( unsigned int i=0; i < m_numParticles; ++i )
	{
		u32 base_vertex					   = i * 4;
		u32 base_index					   = i * 6;
		
		RenderParticle* __restrict particle	= &m_renderParticles[ i ];
		
		vec4 colour						   = vec4( particle->m_vColour.x, particle->m_vColour.y, particle->m_vColour.z, particle->m_fAlpha );
		
		// Positions
		vec3 right, up;
		vec3Scale( right,	camera_right,	particle->m_vScale.x );
		vec3Scale( up,		camera_up,		particle->m_vScale.y );
		
		vec3 offsets[ 4 ] =
		{
			-right - up,
			right - up,
			right + up,
			-right + up
		};
		
		mtx44 rot_mat;
		if( m_faceCamera ) {
			mtx44RotateArbitary_Rad( rot_mat, particle->m_fRotation, camera_forwards );
		}
		else {
			mtx44RotateArbitary_Rad( rot_mat, particle->m_fRotation, particle->m_vRotationAxis );
		}
		
		for( unsigned int p=0; p<4; ++p )
		{
			vec3Transform( offsets[ p ], offsets[ p ], rot_mat );
		}
		
		// Position
		vertices[ base_vertex + 0 ].position	= particle->m_vPosition + offsets[ 0 ];
		vertices[ base_vertex + 1 ].position	= particle->m_vPosition + offsets[ 1 ];
		vertices[ base_vertex + 2 ].position	= particle->m_vPosition + offsets[ 2 ];
		vertices[ base_vertex + 3 ].position	= particle->m_vPosition + offsets[ 3 ];
		
		// Colour
		vertices[ base_vertex + 0 ].colour		= colour;
		vertices[ base_vertex + 1 ].colour		= colour;
		vertices[ base_vertex + 2 ].colour		= colour;
		vertices[ base_vertex + 3 ].colour		= colour;
		
		vertices[ base_vertex + 0 ].texcoords	= vec2( 0.0f, 0.0f );
		vertices[ base_vertex + 1 ].texcoords	= vec2( 1.0f, 0.0f );
		vertices[ base_vertex + 2 ].texcoords	= vec2( 1.0f, 1.0f );
		vertices[ base_vertex + 3 ].texcoords	= vec2( 0.0f, 1.0f );
		
		// indices
		indices[ base_index + 0 ]				= base_vertex + 0;
		indices[ base_index + 1 ]				= base_vertex + 1;
		indices[ base_index + 2 ]				= base_vertex + 2;
		indices[ base_index + 3 ]				= base_vertex + 0;
		indices[ base_index + 4 ]				= base_vertex + 2;
		indices[ base_index + 5 ]				= base_vertex + 3;
	}
	
	glDrawElements(GL_TRIANGLES, m_numParticles * 6, GL_UNSIGNED_SHORT, indices);
	
	if( m_textureResource )
	{
		m_confettiProgram->Unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else 
	{
		m_confettiProgramNoTexture->Unbind();
	}
	
	glVertexAttribPointer(kATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(kATTRIB_COLOR,	4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(kATTRIB_TEXCOORDS,2, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glDisableVertexAttribArray(kATTRIB_POSITION);
	glDisableVertexAttribArray(kATTRIB_COLOR);
	glDisableVertexAttribArray(kATTRIB_TEXCOORDS);
}

//------------------------------------------------------------------------
