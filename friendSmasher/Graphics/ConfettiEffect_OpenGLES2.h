#ifndef __CONFETTI_EFFECT_OPENGLES2_H_INCLUDED__
#define __CONFETTI_EFFECT_OPENGLES2_H_INCLUDED__

#include "ConfettiEffect.h"

namespace FriendSmasher
{
    class ConfettiEffect_OpenGLES2 : public ConfettiEffect
    {
    public:
        
                                    ConfettiEffect_OpenGLES2(unsigned int max_particles);
        virtual						~ConfettiEffect_OpenGLES2();
        
        virtual void				Render( const Math::mtx44& view_matrix, const Math::mtx44& proj_matrix );
        
    protected:
               
        enum eAttributeLocation
        {
            kATTRIB_POSITION,
            kATTRIB_COLOR,
            kATTRIB_TEXCOORDS
        };
        
        System::ShaderProgram*		m_confettiProgram;
        System::ShaderProgram*		m_confettiProgramNoTexture;
    };
		
}

#endif