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
#include "FacebookController.h"

namespace FriendSmasher
{
    namespace Game
    { 
        class GameController
        {
            
        public:

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
            
            bool m_bShouldSendAchievement[FacebookController::kACHIEVEMENT_MAX];
            

        };
    }
}

#endif