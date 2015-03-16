//
//  AlertDialog.cpp
//  friendSmasher
//
//  Created by Ali Parr on 23/10/2013.
//
//

#include "AlertDialog.h"

namespace FriendSmasher
{
    namespace Game
    {
        AlertDialog::AlertDialog() :
        m_pBackgroundSprite(NULL),
        m_pYesButtonSprite(NULL),
        m_pNoButtonSprite(NULL),
        m_bActive(false),
        m_vc(NULL)
        {
            
        }

        AlertDialog::~AlertDialog()
        {
            
        }
        
        void AlertDialog::Initialise(ViewController* vc, NSString* title, NSString* body, void (*tapCallback)(bool))
        {
            m_vc = vc;
            
            m_pTapCallback = tapCallback;
            
            System::TextureResource* pBackgroundTextureResource = new System::TextureResource();
            pBackgroundTextureResource->CreateFromFile("Art/alert_bg.png");
            m_pBackgroundSprite = new System::Sprite(pBackgroundTextureResource);
            m_pBackgroundSprite->SetDraw(false);
            m_pBackgroundSprite->SetLayer(100);
            m_pBackgroundSprite->SetPosition(Math::vec2(0.f, 240.f));
            
            System::TextureResource* pYesButtonTextureResource = new System::TextureResource();
            pYesButtonTextureResource->CreateFromFile("Art/yes_button.png");
            m_pYesButtonSprite = new System::Sprite(pYesButtonTextureResource);
            m_pYesButtonSprite->SetDraw(false);
            m_pYesButtonSprite->SetLayer(100);
            m_pYesButtonSprite->SetPosition(Math::vec2(90.f, 600.f));
            
            System::TextureResource* pNoButtonTextureResource = new System::TextureResource();
            pNoButtonTextureResource->CreateFromFile("Art/no_button.png");
            m_pNoButtonSprite = new System::Sprite(pNoButtonTextureResource);
            m_pNoButtonSprite->SetDraw(false);
            m_pNoButtonSprite->SetLayer(100);
            m_pNoButtonSprite->SetPosition(Math::vec2(360.f, 626.f));
            
            m_labelTitle = [[UILabel alloc] initWithFrame:CGRectMake(108.0, 152.0, 220.0, 100.0)];
            m_labelTitle.textAlignment = UITextAlignmentCenter;
            m_labelTitle.textColor = [UIColor colorWithRed:241.0/255.0 green:223.0/255.0 blue:204.0/255.0 alpha:1.0];
            m_labelTitle.backgroundColor = [UIColor clearColor];
            m_labelTitle.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(28.0)];
            [m_vc.view addSubview: m_labelTitle];
            m_labelTitle.text = title;
            m_labelTitle.shadowColor = [UIColor blackColor];
            m_labelTitle.shadowOffset = CGSizeMake(-1,1);
            m_labelTitle.hidden = YES;
            
            m_labelBody = [[UILabel alloc] initWithFrame:CGRectMake(0.0, 220.0, 320.0, 100.0)];
            m_labelBody.textAlignment = UITextAlignmentCenter;
            m_labelBody.numberOfLines = 0;
            m_labelBody.textColor = [UIColor colorWithRed:241.0/255.0 green:223.0/255.0 blue:204.0/255.0 alpha:1.0];
            m_labelBody.backgroundColor = [UIColor clearColor];
            m_labelBody.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(20.0)];
            [m_vc.view addSubview: m_labelBody];
            m_labelBody.text = body;
            m_labelBody.shadowColor = [UIColor blackColor];
            m_labelBody.shadowOffset = CGSizeMake(-1,1);
            m_labelBody.hidden = YES;
        }
        
        void AlertDialog::Spawn()
        {
            m_pBackgroundSprite->SetDraw(true);
            m_bActive = true;
            m_pYesButtonSprite->SetDraw(true);
            m_pNoButtonSprite->SetDraw(true);
            m_labelTitle.hidden = NO;
            m_labelBody.hidden = NO;
        }
        
        void AlertDialog::Dismiss()
        {
            m_pBackgroundSprite->SetDraw(false);
            m_bActive = false;
            m_pYesButtonSprite->SetDraw(false);
            m_pNoButtonSprite->SetDraw(false);
            m_labelTitle.hidden = YES;
            m_labelBody.hidden = YES;
        }
        
        void AlertDialog::Uninitialise()
        {
            m_labelTitle.hidden = YES;
            m_labelTitle = nil;
            
            m_labelBody.hidden = YES;
            m_labelBody = nil;
            
            delete m_pBackgroundSprite;
            m_pBackgroundSprite = NULL;
        }
        
        
        void AlertDialog::OnTapDown(int nIndex, Math::vec2 vPosition)
        {
            
        }
        
        void AlertDialog::OnTapUp(int nIndex, Math::vec2 vPosition)
        {
            if (m_pYesButtonSprite->GetDraw() && m_pYesButtonSprite->IsPointInside(vPosition, 25.f))
            {
                m_pTapCallback(true);
            }
            
            if (m_pNoButtonSprite->GetDraw() && m_pNoButtonSprite->IsPointInside(vPosition, 25.f))
            {
                m_pTapCallback(false);
            }
        }
        
        
    }
}

