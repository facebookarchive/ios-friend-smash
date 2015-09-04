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

#include "MultiFriendSelector.h"
#include "FBGraph.h"

namespace FriendSmasher
{
    namespace Game
    {
        
        static MultiFriendSelector* kpMultiFriendSelector;
        
        static void didFetchInvitableFriendDetails(NSArray* friends) {
            if (kpMultiFriendSelector) {
                kpMultiFriendSelector->Populate_Invites(friends);
            }
        }
        
        static void didFetchFriendDetails(NSArray* friends) {
            if (kpMultiFriendSelector) {
                kpMultiFriendSelector->Populate_Friends(friends);
            }
        }
        
        
        MultiFriendSelector::MultiFriendSelector() :
        m_bActive(false),
        m_vc(NULL),
        m_pSendButtonSprite(NULL),
        m_pMFSToggle_Invite(NULL),
        m_pMFSToggle_Request(NULL),
        m_bPopulating(NULL),
        m_bInviteMode(true)
        {
            kpMultiFriendSelector = this;
        }

        MultiFriendSelector::~MultiFriendSelector()
        {
            for (int i=0; i<8; ++i)
            {
                delete m_Entries[i].pProfileImageSprite;
                m_Entries[i].pProfileImageSprite = NULL;
                
                delete m_Entries[i].pProfileImageBGSprite;
                m_Entries[i].pProfileImageBGSprite = NULL;
                
                delete m_Entries[i].pCheckboxColdSprite;
                m_Entries[i].pCheckboxColdSprite = NULL;
                
                delete m_Entries[i].pCheckboxHotSprite;
                m_Entries[i].pCheckboxHotSprite = NULL;
                
                delete m_Entries[i].pUserTexture;
                m_Entries[i].pUserTexture = NULL;
                
                delete m_Entries[i].strID;
                m_Entries[i].strID = NULL;
            }
            
            delete m_pSendButtonSprite;
            m_pSendButtonSprite = NULL;
            
            delete m_pMFSToggle_Invite;
            m_pMFSToggle_Invite = NULL;
            
            delete m_pMFSToggle_Request;
            m_pMFSToggle_Request = NULL;
        }
        
        void MultiFriendSelector::Initialise(ViewController* vc, void (*tapCallback)(NSArray* friendIDs))
        {
            m_vc = vc;
            
            m_pTapCallback = tapCallback;
            
            m_Entries[0].vPosition = Math::vec2(40.f, 430.f);
            m_Entries[1].vPosition = Math::vec2(360.f, 430.f);
            
            m_Entries[2].vPosition = Math::vec2(40.f, 580.f);
            m_Entries[3].vPosition = Math::vec2(360.f, 580.f);
            
            m_Entries[4].vPosition = Math::vec2(40.f, 730.f);
            m_Entries[5].vPosition = Math::vec2(360.f, 730.f);
            
            m_Entries[6].vPosition = Math::vec2(40.f, 880.f);
            m_Entries[7].vPosition = Math::vec2(360.f, 880.f);
            
            System::TextureResource* pCheckboxColdTextureResource = new System::TextureResource();
            pCheckboxColdTextureResource->CreateFromFile("Art/checkbox_cold.png");
            
            System::TextureResource* pCheckboxHotTextureResource = new System::TextureResource();
            pCheckboxHotTextureResource->CreateFromFile("Art/checkbox_hot.png");
            
            System::TextureResource* pProfilePictureBackgroundTextureResource = new System::TextureResource();
            pProfilePictureBackgroundTextureResource->CreateFromFile("Art/profile_holder_invert.png");

            for (int i=0; i<8; ++i)
            {
                m_Entries[i].bSelected = false;
                m_Entries[i].pUserTexture = NULL;
                m_Entries[i].pProfileImageSprite = NULL;
                m_Entries[i].pProfileImageBGSprite = NULL;
                m_Entries[i].strID = NULL;
                
                m_Entries[i].pCheckboxColdSprite = new System::Sprite(pCheckboxColdTextureResource);
                m_Entries[i].pCheckboxColdSprite->SetPosition(m_Entries[i].vPosition + Math::vec2(100.f, 40.f));
                m_Entries[i].pCheckboxColdSprite->SetLayer(12);
                m_Entries[i].pCheckboxColdSprite->SetDraw(false);
                
                m_Entries[i].pCheckboxHotSprite = new System::Sprite(pCheckboxHotTextureResource);
                m_Entries[i].pCheckboxHotSprite->SetPosition(m_Entries[i].vPosition + Math::vec2(100.f, 40.f));
                m_Entries[i].pCheckboxHotSprite->SetLayer(10);
                m_Entries[i].pCheckboxHotSprite->SetDraw(false);
                
                m_Entries[i].labelName = [[UILabel alloc] initWithFrame:CGRectMake((m_Entries[i].vPosition.x * 0.5 ) + 54.0, (m_Entries[i].vPosition.y * 0.5) - 6.0, 300, 30.0)];
                m_Entries[i].labelName.textAlignment = NSTextAlignmentLeft;
                m_Entries[i].labelName.textColor = [UIColor colorWithRed:241.0/255.0 green:223.0/255.0 blue:204.0/255.0 alpha:1.0];
                m_Entries[i].labelName.backgroundColor = [UIColor clearColor];
                m_Entries[i].labelName.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(20.0)];
                [m_vc.view addSubview: m_Entries[i].labelName];
                m_Entries[i].labelName.text = [NSString stringWithFormat:@""];
                m_Entries[i].labelName.shadowColor = [UIColor blackColor];
                m_Entries[i].labelName.shadowOffset = CGSizeMake(-1,1);
                m_Entries[i].labelName.hidden = YES;
                
                
                m_Entries[i].pProfileImageBGSprite = new System::Sprite(pProfilePictureBackgroundTextureResource);
                m_Entries[i].pProfileImageBGSprite->SetLayer(9);
                m_Entries[i].pProfileImageBGSprite->SetDraw(false);
                m_Entries[i].pProfileImageBGSprite->SetPosition(m_Entries[i].vPosition + Math::vec2(-4.f, -4.f));
                m_Entries[i].pProfileImageBGSprite->SetScale(Math::vec2(0.62f, 0.62f));
            }
            
            System::TextureResource* pSendButtonTextureResource = new System::TextureResource();
            pSendButtonTextureResource->CreateFromFile("Art/send_button.png");
            m_pSendButtonSprite = new System::Sprite(pSendButtonTextureResource);
            m_pSendButtonSprite->SetPosition(Math::vec2(140.f, 1000.f));
            m_pSendButtonSprite->SetLayer(12);
            m_pSendButtonSprite->SetDraw(false);
            
            System::TextureResource* pMFSToggleInviteTextureResource = new System::TextureResource();
            pMFSToggleInviteTextureResource->CreateFromFile("Art/mfs_toggle_invite.png");
            m_pMFSToggle_Invite = new System::Sprite(pMFSToggleInviteTextureResource);
            m_pMFSToggle_Invite->SetPosition(Math::vec2(100.f, 314.f));
            m_pMFSToggle_Invite->SetLayer(12);
            m_pMFSToggle_Invite->SetDraw(false);
   
            System::TextureResource* pMFSToggleRequestTextureResource = new System::TextureResource();
            pMFSToggleRequestTextureResource->CreateFromFile("Art/mfs_toggle_request.png");
            m_pMFSToggle_Request = new System::Sprite(pMFSToggleRequestTextureResource);
            m_pMFSToggle_Request->SetPosition(Math::vec2(100.f, 314.f));
            m_pMFSToggle_Request->SetLayer(12);
            m_pMFSToggle_Request->SetDraw(false);
            
        }
        
        void MultiFriendSelector::Spawn()
        {
            // First we need to fetch the friend details from Facebook - then we can populate
            m_bPopulating = true;
            [FBGraph fetchInvitableFriendsWithCallback:^(NSArray *friends){
                didFetchInvitableFriendDetails(friends);
            }];

            for (int i=0; i<8; ++i) {
                m_Entries[i].pProfileImageBGSprite->SetDraw(true);
            }
            
            m_pSendButtonSprite->SetDraw(true);
            m_pMFSToggle_Invite->SetDraw(true);
            
            m_bInviteMode = true;
            
            m_bActive = true;
        }
        
        void MultiFriendSelector::Populate_Invites(NSArray* friends)
        {
            m_bPopulating = false;
            
            if (friends == nil) {
                return;
            }
            
            int nCount = [friends count];
            nCount = MIN(nCount, 8);
            
            for (int i=0; i<nCount; ++i) {
                NSDictionary *friendData = [friends objectAtIndex:i];
                
                NSString *friendName = [[[friendData objectForKey:@"name"] componentsSeparatedByString:@" "] objectAtIndex:0];
                NSString *friendId = [friendData objectForKey:@"id"];
                NSString *friendImgUrl = [[[friendData objectForKey:@"picture"] objectForKey:@"data"] objectForKey:@"url"];
                
                std::string url = [friendImgUrl UTF8String];

                m_Entries[i].labelName.text = friendName;
                m_Entries[i].labelName.hidden = NO;
                 
                m_Entries[i].pUserTexture = new System::TextureResource();
                m_Entries[i].pUserTexture->CreateFromURL(url, 256, 256);
                
                m_Entries[i].pCheckboxColdSprite->SetDraw(true);
                
                m_Entries[i].strID = new std::string([friendId UTF8String]);
            }

        }
        
        void MultiFriendSelector::Populate_Friends(NSArray* friends)
        {
            m_bPopulating = false;
            
            if (friends == nil) {
                return;
            }
            
            int nCount = [friends count];
            nCount = MIN(nCount, 8);
            
            for (int i=0; i<nCount; ++i) {
                NSDictionary *friendData = [friends objectAtIndex:i];
                
                NSString *friendId = [friendData objectForKey:@"id"];
                NSString *friendName = [[[friendData objectForKey:@"name"] componentsSeparatedByString:@" "] objectAtIndex:0];
                
                u64 uFriendID = [friendId longLongValue];
                
                m_Entries[i].labelName.text = friendName;
                m_Entries[i].labelName.hidden = NO;
                
                m_Entries[i].pUserTexture = new System::TextureResource();
                m_Entries[i].pUserTexture->CreateFromFBID(uFriendID, 256, 256);
                
                m_Entries[i].pCheckboxColdSprite->SetDraw(true);
                
                m_Entries[i].strID = new std::string([friendId UTF8String]);
            }
        }
        
        void MultiFriendSelector::Depopulate()
        {
            for (int i=0; i<8; ++i) {
            
                m_Entries[i].labelName.text = [NSString stringWithFormat:@""];
                m_Entries[i].labelName.hidden = YES;
                
                if (m_Entries[i].pProfileImageSprite) {
                    m_Entries[i].pProfileImageSprite->SetDraw(false);
                    delete m_Entries[i].pProfileImageSprite;
                    m_Entries[i].pProfileImageSprite = NULL;
                }
                
                if (m_Entries[i].pUserTexture) {
                    delete m_Entries[i].pUserTexture;
                    m_Entries[i].pUserTexture = NULL;
                }
                
                if (m_Entries[i].pCheckboxColdSprite) {
                    m_Entries[i].pCheckboxColdSprite->SetDraw(false);
                }
                
                if (m_Entries[i].pCheckboxHotSprite) {
                    m_Entries[i].pCheckboxHotSprite->SetDraw(false);
                }
                
                
                if (m_Entries[i].strID) {
                    delete m_Entries[i].strID;
                    m_Entries[i].strID = NULL;
                }
                
                m_Entries[i].bSelected = false;
            }
            
        }
        
        void MultiFriendSelector::Update()
        {
            for (int i=0; i<8; ++i)
            {
                if (m_Entries[i].pUserTexture && m_Entries[i].pUserTexture->GetIsReady() && m_Entries[i].pProfileImageSprite == NULL)
                {
                    m_Entries[i].pProfileImageSprite = new System::Sprite(m_Entries[i].pUserTexture);
                    m_Entries[i].pProfileImageSprite->SetLayer(10);
                    m_Entries[i].pProfileImageSprite->SetPosition(m_Entries[i].vPosition);
                    m_Entries[i].pProfileImageSprite->SetScale(Math::vec2(0.35f, 0.35f));
                }
            }
        }
        
        void MultiFriendSelector::Dismiss()
        {
            Depopulate();
            
            m_pSendButtonSprite->SetDraw(false);
            m_pMFSToggle_Invite->SetDraw(false);
            m_pMFSToggle_Request->SetDraw(false);
            
            for (int i=0; i<8; ++i) {
                m_Entries[i].pProfileImageBGSprite->SetDraw(false);
            }
            
            m_bActive = false;
        }
        
        void MultiFriendSelector::Uninitialise()
        {
        }
        
        
        void MultiFriendSelector::OnTapDown(int nIndex, Math::vec2 vPosition)
        {
            
        }
        
        void MultiFriendSelector::OnTapUp(int nIndex, Math::vec2 vPosition)
        {
            if (!m_bActive) {
                return;
            }
            
            for (int i=0; i<8; ++i)
            {
                if ( (m_Entries[i].pProfileImageSprite && m_Entries[i].pProfileImageSprite->IsPointInside(vPosition, 25.f)) ||
                     (m_Entries[i].pCheckboxHotSprite && m_Entries[i].pCheckboxHotSprite->IsPointInside(vPosition, 25.f)) ||
                     (m_Entries[i].pCheckboxColdSprite && m_Entries[i].pCheckboxColdSprite->IsPointInside(vPosition, 25.f)))
                {
                    m_Entries[i].bSelected = !m_Entries[i].bSelected;
                    
                    m_Entries[i].pCheckboxHotSprite->SetDraw(m_Entries[i].bSelected);
                    m_Entries[i].pCheckboxColdSprite->SetDraw(!m_Entries[i].bSelected);
                }
            }
            
            if ( ((m_pMFSToggle_Invite && m_pMFSToggle_Invite->IsPointInside(vPosition, 10.f)) || (m_pMFSToggle_Request && m_pMFSToggle_Request->IsPointInside(vPosition, 10.f))) && !m_bPopulating)
            {
                m_pMFSToggle_Invite->SetDraw(!m_pMFSToggle_Invite->GetDraw());
                m_pMFSToggle_Request->SetDraw(!m_pMFSToggle_Request->GetDraw());
                
                m_bInviteMode = !m_bInviteMode;
                
                if (m_pMFSToggle_Invite->GetDraw()) {
                    Depopulate();
                    m_bPopulating = true;
                    [FBGraph fetchInvitableFriendsWithCallback:^(NSArray *friends){
                        didFetchInvitableFriendDetails(friends);
                    }];
                }
                
                if (m_pMFSToggle_Request->GetDraw()) {
                    Depopulate();
                    m_bPopulating = true;
                    
                    [FBGraph fetchFriendDetailsWithCallback:^(NSArray *friends){
                        didFetchFriendDetails(friends);
                    }];
                }
            }
            
            if (m_pSendButtonSprite && m_pSendButtonSprite->IsPointInside(vPosition, 20.f))
            {
                NSMutableArray *friends = [NSMutableArray new];
                
                for (int i=0; i<8; ++i) {
                    
                    if (m_Entries[i].bSelected) {
                        
                        NSString *nsstrId = [NSString stringWithCString:m_Entries[i].strID->c_str() encoding:[NSString defaultCStringEncoding]];
                        
                        [friends addObject:nsstrId];
                    }
                }
                
                m_pTapCallback(friends);
            }
            
        }
        
        
    }
}

