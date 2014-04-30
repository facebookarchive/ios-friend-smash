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

#ifndef FRIENDSMASHER_MULTIFRIENDSELECTOR
#define FRIENDSMASHER_MULTIFRIENDSELECTOR

#import "ViewController.h"
#import "AppDelegate.h"

#include "Graphics.h"
#include "Sprite.h"
#include "TextureResource.h"

#include "math_lib.h"


namespace FriendSmasher
{
    namespace Game
    {
        class MultiFriendSelector
        {
            
        public:
        
            MultiFriendSelector();
            virtual ~MultiFriendSelector();
            
            void Initialise(ViewController* vc, void (*tapCallback)(NSArray* friendIDs));
            void Spawn();
            void Update();
            void Dismiss();
            void Uninitialise();
            
            void OnTapDown(int nIndex, Math::vec2 vPosition);
            void OnTapUp(int nIndex, Math::vec2 vPosition);
            
            inline bool GetActive() { return m_bActive; }
            inline bool GetInviteMode() { return m_bInviteMode; }
            
            void Populate_Invites(NSArray* friends);
            void Populate_Friends(NSArray* friends);
            void Depopulate();
            
        private:
            
            ViewController* m_vc;
            
            bool m_bActive;
            bool m_bInviteMode;
            bool m_bPopulating;
            
            void (*m_pTapCallback)(NSArray* friendIDs);
            
            struct Entry
            {
                Math::vec2 vPosition;
                
                System::Sprite* pProfileImageSprite;
                System::Sprite* pProfileImageBGSprite;
                
                System::Sprite* pCheckboxColdSprite;
                System::Sprite* pCheckboxHotSprite;
                
                System::TextureResource* pUserTexture;
                
                std::string* strID;
                
                UILabel *labelName;
                bool bSelected;
            };
            
            Entry m_Entries[8];
            
            System::Sprite* m_pSendButtonSprite;
            
            System::Sprite* m_pMFSToggle_Invite;
            System::Sprite* m_pMFSToggle_Request;
        };
    }
}



#endif
