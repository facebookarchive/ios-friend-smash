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
