
#include "ConfettiEffect.h"
#include "time.h"
#include "random.h"
#include "ConfettiEffect_OpenGLES2.h"

using namespace FriendSmasher;
using namespace FriendSmasher::Math;
using namespace FriendSmasher::System;

//------------------------------------------------------------------------

static vec3 s_defaultColours[] = 
{
	vec3( 1.0f, 1.0f,  1.0f ),
	vec3( 0.95f, 0.90f,  1.0f ),
	vec3( 0.8f, 0.8f,  1.0f )
};

//------------------------------------------------------------------------

ConfettiEffect* ConfettiEffect::Create( unsigned int max_particles )
{
    return new ConfettiEffect_OpenGLES2(max_particles);
}

//------------------------------------------------------------------------

ConfettiEffect::ConfettiEffect( unsigned int max_particles ) :
m_textureResource(NULL),
m_maxParticles(max_particles),
m_maxParticlesPerFrame(0xffffffff),
m_numParticles(0),
m_particleAccumulator(0.0f),
m_particleLife(10),
m_particleMaxLifeOffset(0),	
m_faceCamera(true),
m_buffer(0),
m_fAlphaCoefficient(1.f)
{
	for( unsigned int i=0; i < 2; ++i )
	{
		m_updateParticles[i] = new UpdateParticle[ m_maxParticles ];
	}
	
	m_renderParticles = new RenderParticle[ m_maxParticles ];
	
	m_vMinPosition			= vec3( 0.0f, 0.0f, 0.0f );
	m_vMaxPosition			= vec3( 0.0f, 0.0f, 0.0f );
	m_vMinStartVelocity		= vec3( 0.0f, 0.0f, 0.0f );
	m_vMaxStartVelocity		= vec3( 0.0f, 0.0f, 0.0f );
	m_vStartVelocityOffset	= vec3( 0.0f, 0.0f, 0.0f );
	m_fMinStartScale		= 0.5f;
	m_fMaxStartScale		= 0.5f;
	m_vGravity				= vec3( 0.0f, 0.0f, 0.0f );
	m_fMinDamping			= 1.0f;
	m_fMaxDamping			= 1.0f;
	m_fMinAlphaDelta		= 0.0f;
	m_fMaxAlphaDelta		= 0.0f;
	m_fMinRotationDelta		= 0.0f;
	m_fMaxRotationDelta		= 0.0f;
	
	m_blendMode				= System::Sprite::BLEND_MODULATE;
	
	m_colours				= s_defaultColours;
	m_numColours			= sizeof(s_defaultColours) / sizeof(s_defaultColours[0]);
}

//------------------------------------------------------------------------

ConfettiEffect::~ConfettiEffect( )
{
	for( unsigned int i=0; i < 2; ++i )
	{
		delete[] m_updateParticles[i];
		m_updateParticles[i] = NULL;
	}
	
	delete[] m_renderParticles;
	m_renderParticles = NULL;
}

//------------------------------------------------------------------------

void ConfettiEffect::Update( )
{
	const UpdateParticle* __restrict	pParticleRead	= m_updateParticles[ m_buffer ^ 1 ];
	UpdateParticle* __restrict			pParticleWrite	= m_updateParticles[ m_buffer	  ];
		
	float fDeltaTime = 1.0f / 30.0f; // static_cast<float>(uDeltaTime);
	
	//////////////////////////////////////////////////////////////////////////
	
	u32 uNumParticles = m_numParticles;
	for( u32 i=0; i<uNumParticles; ++i ) 
	{
		//if( pParticleRead->m_fAlpha <= 0.0f )
		if( pParticleRead->m_vPosition.x < -400.0f )
		{
			// Yep - time to move on
			++pParticleRead;
			--m_numParticles;
		}
		else
		{
			// Position
			vec3Add(pParticleWrite->m_vPosition, pParticleRead->m_vPosition, (pParticleRead->m_vVelocity * fDeltaTime));
			
			// Velocity
			pParticleWrite->m_vVelocity			= pParticleRead->m_vVelocity * pParticleRead->m_fVelocityDamping;
			
			
			vec3Add( pParticleWrite->m_vVelocity, pParticleRead->m_vVelocity, m_vGravity );
						
			// Update scale
			vec2Add(pParticleWrite->m_vScale, pParticleRead->m_vScale, (pParticleRead->m_vScaleDelta * fDeltaTime));
			if (pParticleWrite->m_vScale.x < 0.f) { pParticleWrite->m_vScale.x = 0.f; }
			if (pParticleWrite->m_vScale.y < 0.f) { pParticleWrite->m_vScale.y = 0.f; }
			
			// Colour
			vec3Add(pParticleWrite->m_vColour, pParticleRead->m_vColour, (pParticleRead->m_vColourDelta * fDeltaTime));
			vec3Clamp(pParticleWrite->m_vColour, pParticleWrite->m_vColour);
			
			// Alpha
			pParticleWrite->m_fAlpha			= (pParticleRead->m_fAlpha + pParticleRead->m_fAlphaDelta * fDeltaTime) * m_fAlphaCoefficient;
			Math::Saturate(&pParticleWrite->m_fAlpha);
			
			// Rotation
			pParticleWrite->m_fRotation			= pParticleRead->m_fRotation + (pParticleRead->m_fRotationDelta * fDeltaTime);
			pParticleWrite->m_vRotationAxis		= pParticleRead->m_vRotationAxis;
			
			// Copy over the deltas...
			pParticleWrite->m_fVelocityDamping	= pParticleRead->m_fVelocityDamping;
			pParticleWrite->m_vColourDelta		= pParticleRead->m_vColourDelta;
			pParticleWrite->m_vScaleDelta		= pParticleRead->m_vScaleDelta;
			pParticleWrite->m_fAlphaDelta		= pParticleRead->m_fAlphaDelta;
			pParticleWrite->m_fRotationDelta	= pParticleRead->m_fRotationDelta;
			pParticleWrite->m_nRemainingLifeMS	= pParticleRead->m_nRemainingLifeMS - static_cast<int>(fDeltaTime);
			pParticleWrite->m_nMidlifeMS		= pParticleRead->m_nMidlifeMS;
			
			++pParticleWrite;
			++pParticleRead;
		}
	}
	
	unsigned int new_particles = floorf( m_particleAccumulator );
	new_particles = Math::Min(new_particles, m_maxParticles - m_numParticles);
	new_particles = Math::Min(new_particles, m_maxParticlesPerFrame);
	for( unsigned int i=0; i < new_particles; ++i )
	{
		vec3 pos							= vec3( Random::GetRandom(m_vMinPosition.x, m_vMaxPosition.x),
												    Random::GetRandom(m_vMinPosition.y, m_vMaxPosition.y),
												    Random::GetRandom(m_vMinPosition.z, m_vMaxPosition.z) );
		
		pParticleWrite->m_vPosition			= pos;
		pParticleWrite->m_vVelocity			= vec3( Random::GetRandom(m_vMinStartVelocity.x, m_vMaxStartVelocity.x),
												    Random::GetRandom(m_vMinStartVelocity.y, m_vMaxStartVelocity.y),
												    Random::GetRandom(m_vMinStartVelocity.z, m_vMaxStartVelocity.z) );
		
		pParticleWrite->m_vScale			= vec2( Random::GetRandom( m_fMinStartScale, m_fMaxStartScale ) );
		
		vec3 offset							= vec3( Random::GetRandom( -m_vStartVelocityOffset.x, m_vStartVelocityOffset.x ),
												    Random::GetRandom( -m_vStartVelocityOffset.y, m_vStartVelocityOffset.y ),
												    Random::GetRandom( -m_vStartVelocityOffset.z, m_vStartVelocityOffset.z ) );
			
		pParticleWrite->m_vColour			= m_colours[ Random::GetRandom( 0, m_numColours-1 ) ];
		
		pParticleWrite->m_vVelocity			+= offset;
		
		pParticleWrite->m_fVelocityDamping	= Random::GetRandom( m_fMinDamping, m_fMaxDamping );
		
		pParticleWrite->m_fAlpha			= 1.0f;
		pParticleWrite->m_fAlphaDelta		= Random::GetRandom( m_fMinAlphaDelta, m_fMaxAlphaDelta );
		
		pParticleWrite->m_fRotation			= 0.0f;
		pParticleWrite->m_fRotationDelta	= Random::GetRandom( m_fMinRotationDelta, m_fMaxRotationDelta );
		
		vec3 rot_axis;
		
		if( m_faceCamera ) {
			
			rot_axis = vec3( 0.f, 0.f, 1.0f );
			
		}
		else {
			
			rot_axis						= vec3( Random::GetRandom( -1.0f, 1.0f ),
													Random::GetRandom( -1.0f, 1.0f ),
													Random::GetRandom( -1.0f, 1.0f ) );			
			
			float l = vec3Length(rot_axis);
			if( l == 0.0f )
			{
				rot_axis = vec3( 0.0f, 0.0f, 1.0f );
			}
			else {
				rot_axis = rot_axis / l;
			}
		}
											
		pParticleWrite->m_vRotationAxis		= rot_axis;
		
		++pParticleWrite;
	}
	
	m_numParticles								+= new_particles;
	m_particleAccumulator						-= float(new_particles);
	
	pParticleRead								= m_updateParticles[m_buffer];
	RenderParticle* __restrict pRenderWrite		= m_renderParticles;
	
	// Set the particles to visible
	unsigned int i=0;
	for( ; i<m_numParticles; ++i ) 
	{
		pRenderWrite->m_vPosition			= pParticleRead->m_vPosition;
		pRenderWrite->m_vScale				= pParticleRead->m_vScale;
		pRenderWrite->m_vColour				= pParticleRead->m_vColour;
		pRenderWrite->m_fAlpha				= pParticleRead->m_fAlpha;
		pRenderWrite->m_fRotation			= pParticleRead->m_fRotation;
		pRenderWrite->m_vRotationAxis		= pParticleRead->m_vRotationAxis;
		
		++pRenderWrite;
		++pParticleRead;
	}
	
	m_buffer ^= 1;

}

//------------------------------------------------------------------------

void ConfettiEffect::SetAlphaCoefficient(const float a)
{
    m_fAlphaCoefficient	= a;
    
//    for( int i=0; i < m_numParticles; ++i )
//    {
//        m_updateParticles[m_buffer][i].m_fAlpha *= a;
//    }
}

//------------------------------------------------------------------------

void ConfettiEffect::Reset()
{
	m_numParticles = 0;
	m_particleAccumulator = 0;
}

//------------------------------------------------------------------------

void ConfettiEffect::SpawnParticles( float num_particles )
{
	m_particleAccumulator += num_particles;
}

//------------------------------------------------------------------------
