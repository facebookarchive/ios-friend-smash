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

#ifndef FRIENDSMASHER_MENUCONTROLLER
#define FRIENDSMASHER_MENUCONTROLLER

#import "ViewController.h"
#import "AppDelegate.h"

#include "math_lib.h"

#include "Graphics.h"
#include "Sprite.h"
#include "TextureResource.h"

#include "FriendSmashController.h"

#include "AlertDialog.h"
#include "MultiFriendSelector.h"

namespace FriendSmasher
{
    namespace Game
    {
        class MenuController
        {
            
        public:
            
            MenuController(ViewController* vc, FriendSmashController* fsc);
            virtual ~MenuController();
            
            void OnEnter();
            void OnExit();
            void OnUpdate();
            void OnRender();

            void OnTapDown(int nIndex, Math::vec2 vPosition);
            void OnTapUp(int nIndex, Math::vec2 vPosition);
            
            void DidChangeFBLoginState(bool bLoggedIn);
            void DidFetchFBUserInfo(bool bSuccess);
            void DidFetchUserFriendsPermissionForStart(bool bSuccess);
            void DidFetchUserFriendsPermissionForMFS(bool bSuccess);
            void DidFetchPropertyFromParse(NSString* property, u32 uValue);
            void DidSavePropertyToParse(NSString* property, bool bSucceeded);
            
            void ViewLeaderboard(bool bView);
            void UpdateLeaderboard();
            
            void ShowEndGame(NSString* friendName, u64 uFriendID, u32 uScore, u32 uCoinsWon, u32 uBombsRemaining);
            
            void ProcessIncomingURL(NSURL* url);
            void DidProcessIncomingURL(NSString* challengeName, NSString* challengeFBID);
            void DidDismissFriendPermissionUpsellDialog(bool bResult);
            void DidDismissWritePermissionUpsellDialog(bool bResult);
            void DidFetchPublishActionsPermission(bool bResult);
            void DidDismissMultiFriendSelector(NSArray* friend_ids);
            void DidFetchScores(NSArray* scoreData);
            
        private:
            
            void StartNewGame(bool bSocial);
            
            ViewController* m_vc;
            FriendSmashController* m_fsc;
            
            UILabel *m_labelName;
            UILabel *m_labelCoins;
            UILabel *m_labelBombs;
            UILabel *m_labelScore;
            UILabel *m_labelCoinsWon;
            
            AlertDialog* m_pFriendPermissionUpsellDialog;
            AlertDialog* m_pWritePermissionUpsellDialog;
            
            MultiFriendSelector* m_pMultiFriendSelector;
            
            bool m_bReturningFromGame;
            static u32 ms_uBombs;
            static u32 ms_uCoins;
            u32 m_uScore;
            u64 m_uLastFriendID;
            
            NSString* m_nsstrUserName;
            
            System::Sprite* m_pBackgroundSprite;
            System::Sprite* m_pLoginButtonSprite;
            System::Sprite* m_pUserImageSprite;
            System::Sprite* m_pPlayButtonSprite;
            System::Sprite* m_pLogoutButtonSprite;
            System::Sprite* m_pMainMenuButtonSprite;
            System::Sprite* m_pChallengeButtonSprite;
            System::Sprite* m_pScoresButtonSprite;
            System::Sprite* m_pBragButtonSprite;

            System::Sprite* m_pCoinButtonSprite;
            System::Sprite* m_pBombAddButtonSprite;
            
            
            System::TextureResource* m_pUserTexture;
            
            static const u32 m_kuLeaderboardSize = 6;
            struct LeaderboardboardInstance
            {
                System::Sprite* pStubSprite;
                UILabel *pFriendName;
                UILabel *pFriendScore;
                System::TextureResource* pUserTexture;
                System::Sprite* pUserSprite;
            };
            
            System::Sprite* m_pLeaderboardCloseButtonSprite;
            System::Sprite* m_pGameOverCloseButtonSprite;
            
            LeaderboardboardInstance m_pLeaderboardEntries[m_kuLeaderboardSize];
            Math::vec2 m_vLeaderboardPosition;

            bool m_bViewingLeaderboard;
            bool m_bViewingGameover;
        };
    }
}

#endif
