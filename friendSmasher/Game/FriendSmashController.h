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

#ifndef FRIENDSMASHER_FRIENDSMASHCONTROLLER
#define FRIENDSMASHER_FRIENDSMASHCONTROLLER

#import "ViewController.h"
#import "AppDelegate.h"

#include "math_lib.h"

namespace FriendSmasher
{
    namespace Game
    {
        class MenuController;
        class GameController;
        
        class FriendSmashController
        {
            
        public:
            
            FriendSmashController(ViewController* vc);
            virtual ~FriendSmashController();
            
            void OnEnter();
            void OnExit();
            void OnUpdate();
            void OnRender();
            
            void BeginTouch(int nIndex, float fX, float fY);
            void ContinueTouch(int nIndex, float fX, float fY);
            void EndTouch(int nIndex, float fX, float fY);
            
            void StartNewGame(u32 uBombs, bool bSocial);
            void StartNewChallengeGame(u32 uBombs, NSString* challengeName, NSString* challengeFBID);
            
            void EndGame(NSString* friendName, u64 uFriendID, u32 uScore, u32 uCoinsWon, u32 uBombsRemaining);
            
            void ProcessIncomingURL(NSURL* url);
            
        private:
            
            GameController* m_pGameController;
            MenuController* m_pMenuController;
            
            bool m_bIngame;
            
            static const int kMaxNumTouches = 5;
            Math::vec2 m_vBeginTouchPos[kMaxNumTouches];
            Math::vec2 m_vCurrentTouchPos[kMaxNumTouches];
            Math::vec2 m_vEndTouchPos[kMaxNumTouches];
            bool m_bTouched[kMaxNumTouches];
            bool m_bTouchedLastFrame[kMaxNumTouches];
            
        };
    }
}

#endif
