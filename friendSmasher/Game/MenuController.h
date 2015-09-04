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
            
            ViewController* m_vc;
            
        private:
            
            void StartNewGame(bool bSocial);
            
            FriendSmashController* m_fsc;
            
            UILabel *m_labelName;
            UILabel *m_labelCoins;
            UILabel *m_labelBombs;
            UILabel *m_labelScore;
            UILabel *m_labelCoinsWon;
            
            AlertDialog* m_pFriendPermissionUpsellDialog;
            AlertDialog* m_pWritePermissionUpsellDialog;
            
            MultiFriendSelector* m_pMultiFriendSelector;
            
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
