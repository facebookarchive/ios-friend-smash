#if _MSC_VER >= 1200
#pragma once
#endif

#ifndef __PARTICLESYSTEM_3D_H__
#define __PARTICLESYSTEM_3D_H__

#include "math_lib.h"
#include "TextureResource.h"
#include "Sprite.h"

namespace FriendSmasher
{
	
	namespace System
	{
		class ShaderProgram;
	}

    //------------------------------------------------------------------------
    
    class ConfettiEffect
    {
    public:
        
        //------------------------------------------------------------------------
        
        static ConfettiEffect*		Create(unsigned int max_particles);
        
        //------------------------------------------------------------------------
        
        struct UpdateParticle
        {
            UpdateParticle() :
            m_vPosition(Math::vec3(0.f, 0.f, 0.f)),
            m_vVelocity(Math::vec3(0.f, 0.f, 0.f)),
            m_fVelocityDamping(1.0f),
            m_vScale(Math::vec2(1.f, 1.f)),
            m_vScaleDelta(Math::vec2(0.f, 0.f)),
            m_vColour(Math::vec3(1.f,1.f,1.f)),
            m_vColourDelta(Math::vec3(0.f,0.f,0.f)),
            m_fAlpha(1.f),
            m_fAlphaDelta(0.f),
            m_fRotation(0.f),
            m_fRotationDelta(0.f),
            m_nRemainingLifeMS(0),
            m_nMidlifeMS(0)
            {
            }
            
            Math::vec3 m_vPosition;
            
            Math::vec3 m_vVelocity;
            float m_fVelocityDamping;
            
            Math::vec2 m_vScale;
            Math::vec2 m_vScaleDelta;
            
            Math::vec3 m_vColour;
            Math::vec3 m_vColourDelta;
            
            float m_fAlpha;
            float m_fAlphaDelta;		
            float m_fRotation;
            Math::vec3 m_vRotationAxis;
            float m_fRotationDelta;
            
            int	m_nRemainingLifeMS;
            int m_nMidlifeMS;
        };
        
        //------------------------------------------------------------------------
        
        struct RenderParticle
        {
            Math::vec3		m_vPosition;
            Math::vec2		m_vScale;
            Math::vec3		m_vColour;
            float			m_fAlpha;
            Math::vec3		m_vRotationAxis;
            float			m_fRotation;
        };
        
        //------------------------------------------------------------------------
        
        ConfettiEffect( unsigned int max_particles );
        virtual ~ConfettiEffect( );
        
        //------------------------------------------------------------------------
        
        void SetMaxParticlesPerFrame( const unsigned int m)			{ m_maxParticlesPerFrame= m; }
        void SetMinPosition			( const Math::vec3& p )			{ m_vMinPosition		= p; }
        void SetMaxPosition			( const Math::vec3& p )			{ m_vMaxPosition		= p; }
        void SetMinStartVelocity	( const Math::vec3& v )			{ m_vMinStartVelocity	= v; }
        void SetMaxStartVelocity	( const Math::vec3& v )			{ m_vMaxStartVelocity	= v; }
        void SetStartScale			( const float s )				{ m_fMinStartScale = m_fMaxStartScale = s; }
        void SetMinStartScale		( const float s )				{ m_fMinStartScale		= s; }
        void SetMaxStartScale		( const float s )				{ m_fMaxStartScale		= s; }
        void SetGravity				( const Math::vec3& g )			{ m_vGravity			= g; }
        void SetMinDamping			( const float f	)				{ m_fMinDamping			= f; }
        void SetMaxDamping			( const float f )				{ m_fMaxDamping			= f; }
        void SetMinAlphaDelta		( const float f )				{ m_fMinAlphaDelta		= f; }
        void SetMaxAlphaDelta		( const float f )				{ m_fMaxAlphaDelta		= f; }
        void SetMinRotationDelta	( const float f )				{ m_fMinRotationDelta	= f; }
        void SetMaxRotationDelta	( const float f )				{ m_fMaxRotationDelta	= f; }
    
        void SetTextureResource		( System::TextureResource* t )	{ m_textureResource		= t; }
        
        void SetFaceCamera			( const bool b )				{ m_faceCamera			= b; }
        void SetColours				( const Math::vec3* p, u32 n )	{ m_colours				= p;
                                                                      m_numColours			= n; }
        
        void SetAlphaCoefficient	( const float a );
        float GetAlphaCoefficient	() const							{ return m_fAlphaCoefficient; }
        
        const Math::vec3&			GetGravity			( )			{ return m_vGravity;		 }
        
        //------------------------------------------------------------------------
        
        void			Update( );
        virtual void	Render( const Math::mtx44& view_matrix, const Math::mtx44& proj_matrix ) = 0;
        
        void			Reset();
        
        void SpawnParticles( float amount );
        
        //------------------------------------------------------------------------
        
    protected:
        
        //------------------------------------------------------------------------
        
        System::TextureResource*	m_textureResource;
        
        unsigned int				m_maxParticles;
        unsigned int				m_maxParticlesPerFrame;
        unsigned int				m_numParticles;
        UpdateParticle*				m_updateParticles[2];
        RenderParticle*				m_renderParticles;
        float						m_particleAccumulator;
        unsigned int				m_particleLife;
        unsigned int				m_particleMaxLifeOffset;
        float						m_particleMiddleAge;
        
        Math::vec3					m_vMinPosition;
        Math::vec3					m_vMaxPosition;
        Math::vec3					m_vMinStartVelocity;
        Math::vec3					m_vMaxStartVelocity;
        Math::vec3					m_vStartVelocityOffset;
        float						m_fMinStartScale;
        float						m_fMaxStartScale;
        Math::vec3					m_vGravity;
        float						m_fMinDamping;
        float						m_fMaxDamping;
        float						m_fMinAlphaDelta;
        float						m_fMaxAlphaDelta;
        float						m_fMinRotationDelta;
        float						m_fMaxRotationDelta;
        float						m_fFloorPlane;
        float						m_fAlphaCoefficient;
        
        bool						m_faceCamera;
        
        unsigned int				m_buffer;
                
        System::Sprite::eBlendMode	m_blendMode;
        
        const Math::vec3*			m_colours;
        unsigned int				m_numColours;
        
        
        //------------------------------------------------------------------------
        
    };
    
    //------------------------------------------------------------------------
    

	
}

#endif
