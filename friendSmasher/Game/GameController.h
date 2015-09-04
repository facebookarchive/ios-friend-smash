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

#ifndef FRIENDSMASHER_GAMECONTROLLER
#define FRIENDSMASHER_GAMECONTROLLER

#include <vector>

#include "math_lib.h"

#include "Graphics.h"
#include "Sprite.h"
#include "TextureResource.h"
#include "ConfettiEffect.h"

#import "ViewController.h"
#import "AppDelegate.h"

#import <Accounts/Accounts.h>

#include "FriendSmashController.h"
#include "FBGraph.h"
#include "FBLogin.h"
#include "FBShare.h"

namespace FriendSmasher
{
    namespace Game
    { 
        class GameController
        {
            
        public:
            
            enum eGameAchievements
            {
                kACHIEVEMENT_SCORE50 = 0,
                kACHIEVEMENT_SCORE100,
                kACHIEVEMENT_SCORE150,
                kACHIEVEMENT_SCORE200,
                kACHIEVEMENT_SCOREx3,
                kACHIEVEMENT_MAX
            };

            GameController(ViewController* vc, FriendSmashController* fsc);
            virtual ~GameController();

            void OnEnter();
            void OnExit();
            void OnUpdate();
            void OnRender();
            
            
            void OnTapDown(int nIndex, Math::vec2 vPosition);
            void OnTapUp(int nIndex, Math::vec2 vPosition);
            
            void StartGame(bool bSocial, u32 uInitialBombs, bool bChallenge, NSString *challengeFriendName,  NSString *challengeFriendId);

            void DidFetchFBFriendInfo(NSArray* friends);
            
        protected:    
            
            void SelectNonSocial();
            void SpawnEntity();
            void EndGame();
            
            void ViewScoreboard(bool bView);
            void UpdateScoreboard();
            void DropTheBomb();

            System::Sprite* m_pBackgroundSprite;
            System::Sprite* m_pFriendImageSprite;
            System::Sprite* m_pLoadingSprite;
            System::Sprite* m_pLoadingSpinner;
            System::Sprite* m_pHeartSprite[3];
            System::Sprite* m_pBombSprite[3];
            System::Sprite* m_pDropTheBombSprite;

            
            
            System::TextureResource* m_pFriendTexture;
            System::TextureResource* m_pCoinTexture;
            
            System::TextureResource* m_pNonFriendTexture[10];
            System::TextureResource* m_pRiserTextures[3];
    
            ///////
            
            ConfettiEffect* m_confettiEffect;
            
            ///////
            
            UILabel *m_labelFriendName;
            UILabel *m_labelScore;
            
            NSString *m_friendName;
            
            struct EntityInstance
            {
                System::Sprite* pSprite;
                Math::vec2 vVelocity;
                float fRotationVelocity;
                bool bFriend;
                bool bCoin;
            };
            
            std::vector<EntityInstance*> m_vecEntities;
            
            int m_nEntitiesSpawned;
            
            int m_nNoSocialFriendCeleb;
            
            float m_fSpawnTimer;

            std::vector<System::Sprite*> m_vecRisers;
            
            ///////
                
            
            enum eGameState
            {
                kGAMESTATE_LOADING,
                kGAMESTATE_PLAYING,
                kGAMESTATE_PLAYING_NOSOCIAL,
                kGAMESTATE_PLAYING_GAMEOVER,
                kGAMESTATE_PLAYING_GAMEOVER_NOSOCIAL,
            };

            
            eGameState m_kGameState;
            float m_fDeltaTime;
            

            u32 m_uLivesRemaining;
            u32 m_uBombsRemaining;
            u32 m_uCurrentScore;
            u32 m_uCoins;
            bool m_bSocial;
            
            enum eLossType
            {
                kUNDEFINED,
                kLOSSTYPE_RANOUTOFLIVES,
                kLOSSTYPE_HITCELEB,
            };
            

            u64 m_uFriendFBID;
            
            eLossType m_kLossType;
            
            EntityInstance* m_pCelebLossEntity;
            
            ViewController* m_vc;
            FriendSmashController* m_fsc;
            
            ACAccount* m_facebookAccount;
            
            bool m_bShouldSendAchievement[kACHIEVEMENT_MAX];
            

        };
    }
}

#endif