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

#include "FriendSmashController.h"
#include "random.h"

#include "GameController.h"
#include "MenuController.h"

namespace FriendSmasher
{
    namespace Game
    {

        FriendSmashController::FriendSmashController(ViewController* vc) :
        m_bIngame(false)
        {
            m_pGameController = new GameController(vc, this);
            m_pMenuController = new MenuController(vc, this);
            
            for (int i=0; i<kMaxNumTouches; ++i) {
                m_bTouchedLastFrame[i] = false;
                m_bTouched[i] = false;
            }
        }

        FriendSmashController::~FriendSmashController()
        {
            delete m_pGameController;
            delete m_pMenuController;
        }

        void FriendSmashController::OnEnter()
        {
            Random::Seed();
            System::Graphics::Instance()->Initialise();
            
            m_pMenuController->OnEnter();
            //m_pGameController->OnEnter();
        }

        void FriendSmashController::OnExit()
        {
            System::Graphics::Instance()->Uninitialise();
        }

        void FriendSmashController::OnUpdate()
        {
            m_bIngame ? m_pGameController->OnUpdate() : m_pMenuController->OnUpdate();
            
            for (int i=0; i<kMaxNumTouches; ++i) {
                if (m_bTouchedLastFrame[i] != m_bTouched[i] && !m_bTouched[i]) {
                    m_bIngame ? m_pGameController->OnTapUp(i, Math::vec2(m_vEndTouchPos[i].x, m_vEndTouchPos[i].y)) : m_pMenuController->OnTapUp(i, Math::vec2(m_vEndTouchPos[i].x, m_vEndTouchPos[i].y));
                }
				m_bTouchedLastFrame[i] = m_bTouched[i];
            }
        }

        void FriendSmashController::OnRender()
        {
            System::Graphics::Instance()->Update();
            m_bIngame ? m_pGameController->OnRender() : m_pMenuController->OnRender();
        }

        
        void FriendSmashController::BeginTouch(int nIndex, float fX, float fY)
        {
            m_bTouched[nIndex] = true;
            m_bTouchedLastFrame[nIndex] = true;
            m_vBeginTouchPos[nIndex] = Math::vec2(fX, fY);
            m_vCurrentTouchPos[nIndex] = Math::vec2(fX, fY);

            m_bIngame ? m_pGameController->OnTapDown(nIndex, Math::vec2(fX, fY)) : m_pMenuController->OnTapDown(nIndex, Math::vec2(fX, fY));
        }
        
        void FriendSmashController::ContinueTouch(int nIndex, float fX, float fY)
        {
            m_vCurrentTouchPos[nIndex] = Math::vec2(fX, fY);
            //m_bIngame ? m_pGameController->ContinueTouch(nIndex, fX, fY) : m_pMenuController->ContinueTouch(nIndex, fX, fY);
        
        }
        
        void FriendSmashController::EndTouch(int nIndex, float fX, float fY)
        {
            m_bTouched[nIndex] = false;
            m_vEndTouchPos[nIndex] = Math::vec2(fX, fY);
            m_vCurrentTouchPos[nIndex] = Math::vec2::allzero;
            
        }

        void FriendSmashController::StartNewGame(u32 uBombs, bool bSocial)
        {
            m_pMenuController->OnExit();
            m_pGameController->OnEnter();
            m_pGameController->StartGame(bSocial, uBombs, false, NULL, NULL);
            m_bIngame = true;
        }
        
        void FriendSmashController::StartNewChallengeGame(u32 uBombs, NSString* challengeName, NSString* challengeFBID)
        {
            m_pMenuController->OnExit();
            m_pGameController->OnEnter();
            m_pGameController->StartGame(true, uBombs, true, challengeName, challengeFBID);
            m_bIngame = true;
        }
        
        void FriendSmashController::EndGame(NSString* friendName, u64 uFriendID, u32 uScore, u32 uCoinsWon, u32 uBombsRemaining)
        {
            m_pGameController->OnExit();
            m_pMenuController->OnEnter();
            m_pMenuController->ShowEndGame(friendName, uFriendID, uScore, uCoinsWon, uBombsRemaining);
            m_bIngame = false;
        }
        
        void FriendSmashController::ProcessIncomingURL(NSURL* url)
        {
            if (m_pMenuController && !m_bIngame) {
                m_pMenuController->ProcessIncomingURL(url);
            }
        }
        
    }
}