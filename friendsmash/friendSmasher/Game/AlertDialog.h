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

#ifndef FRIENDSMASHER_ALERTDIALOG
#define FRIENDSMASHER_ALERTDIALOG

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
        class AlertDialog
        {
            
        public:
        
            AlertDialog();
            virtual ~AlertDialog();
            
            void Initialise(ViewController* vc, NSString* title, NSString* body, void (*tapCallback)(bool));
            void Spawn();
            void Dismiss();
            void Uninitialise();
            
            void OnTapDown(int nIndex, Math::vec2 vPosition);
            void OnTapUp(int nIndex, Math::vec2 vPosition);
            
            inline bool GetActive() { return m_bActive; }
            
        private:
            
            ViewController* m_vc;
            
            UILabel *m_labelTitle;
            UILabel *m_labelBody;
            
            bool m_bActive;
            System::Sprite* m_pBackgroundSprite;
            System::Sprite* m_pYesButtonSprite;
            System::Sprite* m_pNoButtonSprite;
            
            void (*m_pTapCallback)(bool);
        };
    }
}



#endif
