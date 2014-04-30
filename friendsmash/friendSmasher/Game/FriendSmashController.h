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
