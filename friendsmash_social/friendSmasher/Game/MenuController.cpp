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
#include "MenuController.h"
#include "FacebookController.h"

namespace FriendSmasher
{
    namespace Game
    {
        static MenuController* kpMenuController;
        
        static void didLogIn(bool bLoggedIn) {
            if (kpMenuController) {
                kpMenuController->DidChangeFBLoginState(bLoggedIn);
            }
        }
        
        static void didLogout(bool bLoggedOut) {
            if (kpMenuController) {
                kpMenuController->DidChangeFBLoginState(!bLoggedOut);
            }
        }
        
        static void didFetchUserDetails(bool bSuccess) {
            if (kpMenuController) {
                kpMenuController->DidFetchFBUserInfo(bSuccess);
            }
        }
        
        static void didProcessFBURL(NSString* challengeName, NSString* challengeFBID) {
            if (kpMenuController) {
                kpMenuController->DidProcessIncomingURL(challengeName, challengeFBID);
            }
        }
        
        static void didFetchUserFriendsPermissionForStart(bool bSuccess) {
            if (kpMenuController) {
                kpMenuController->DidFetchUserFriendsPermissionForStart(bSuccess);
            }
        }
        
        static void didFetchUserFriendsPermissionForMFS(bool bSuccess) {
            if (kpMenuController) {
                kpMenuController->DidFetchUserFriendsPermissionForMFS(bSuccess);
            }
        }
        
        static void didFetchPublishActionsPermission(bool bSuccess) {
            if (kpMenuController) {
                kpMenuController->DidFetchPublishActionsPermission(bSuccess);
            }
        }
        
        static void didDismissFriendPermissionUpsellDialog(bool bResult) {
            if (kpMenuController) {
                kpMenuController->DidDismissFriendPermissionUpsellDialog(bResult);
            }
        }
        
        static void didDismissWritePermissionUpsellDialog(bool bResult) {
            if (kpMenuController) {
                kpMenuController->DidDismissWritePermissionUpsellDialog(bResult);
            }
        }
        
        static void didDismissMultiFriendSelector(NSArray* friends) {
            if (kpMenuController) {
                kpMenuController->DidDismissMultiFriendSelector(friends);
            }
        }

        static void didFetchScores(NSArray* friends) {
            if (kpMenuController) {
                kpMenuController->DidFetchScores(friends);
            }
        }
        
        u32 MenuController::ms_uBombs = 0;
        u32 MenuController::ms_uCoins = 30;
        
        
        
        MenuController::MenuController(ViewController* vc, FriendSmashController* fsc) :
        m_vc(vc),
        m_fsc(fsc),
        m_pBackgroundSprite(NULL),
        m_pLoginButtonSprite(NULL),
        m_pUserTexture(NULL),
        m_pUserImageSprite(NULL),
        m_pPlayButtonSprite(NULL),
        m_pChallengeButtonSprite(NULL),
        m_pScoresButtonSprite(NULL),
        m_pBragButtonSprite(NULL),
        m_pLogoutButtonSprite(NULL),
        m_pMainMenuButtonSprite(NULL),
        m_pCoinButtonSprite(NULL),
        m_pBombAddButtonSprite(NULL),
        m_pFriendPermissionUpsellDialog(NULL),
        m_pWritePermissionUpsellDialog(NULL),
        m_pMultiFriendSelector(NULL),
        m_uLastFriendID(0),
        m_uScore(0),
        m_bViewingLeaderboard(false),
        m_bViewingGameover(false)
        {
            kpMenuController = this;
            
            for (u32 i=0; i<m_kuLeaderboardSize; ++i) {
                m_pLeaderboardEntries[i].pStubSprite = NULL;
                m_pLeaderboardEntries[i].pUserSprite = NULL;
                m_pLeaderboardEntries[i].pUserTexture = NULL;
            }
        }
        
        MenuController::~MenuController()
        {
        }
        
        void MenuController::OnEnter()
        {
            System::TextureResource* pBackgroundTextureResource = new System::TextureResource();
            pBackgroundTextureResource->CreateFromFile("Art/frontscreen_background.png");
            m_pBackgroundSprite = new System::Sprite(pBackgroundTextureResource);
            
            m_labelName = [[UILabel alloc] initWithFrame:CGRectMake(98.0, 34.0, 220.0, 100.0)];
            m_labelName.textAlignment = NSTextAlignmentLeft;
            m_labelName.textColor = [UIColor colorWithRed:241.0/255.0 green:223.0/255.0 blue:204.0/255.0 alpha:1.0];
            m_labelName.backgroundColor = [UIColor clearColor];
            m_labelName.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(28.0)];
            [m_vc.view addSubview: m_labelName];
            m_labelName.text = [NSString stringWithFormat:@"Welcome, Player"];
            m_labelName.shadowColor = [UIColor blackColor];
            m_labelName.shadowOffset = CGSizeMake(-2,2);
            m_labelName.hidden = NO;
            
            m_labelCoins = [[UILabel alloc] initWithFrame:CGRectMake(152.0, 78.0, 220.0, 100.0)];
            m_labelCoins.textAlignment = NSTextAlignmentLeft;
            m_labelCoins.textColor = [UIColor colorWithRed:241.0/255.0 green:223.0/255.0 blue:204.0/255.0 alpha:1.0];
            m_labelCoins.backgroundColor = [UIColor clearColor];
            m_labelCoins.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(28.0)];
            [m_vc.view addSubview: m_labelCoins];
            m_labelCoins.text = [NSString stringWithFormat:@"0"];
            m_labelCoins.shadowColor = [UIColor blackColor];
            m_labelCoins.shadowOffset = CGSizeMake(-2,2);
            m_labelCoins.hidden = YES;
            
            m_labelBombs = [[UILabel alloc] initWithFrame:CGRectMake(252.0, 78.0, 220.0, 100.0)];
            m_labelBombs.textAlignment = NSTextAlignmentLeft;
            m_labelBombs.textColor = [UIColor colorWithRed:241.0/255.0 green:223.0/255.0 blue:204.0/255.0 alpha:1.0];
            m_labelBombs.backgroundColor = [UIColor clearColor];
            m_labelBombs.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(28.0)];
            [m_vc.view addSubview: m_labelBombs];
            m_labelBombs.text = [NSString stringWithFormat:@"0"];
            m_labelBombs.shadowColor = [UIColor blackColor];
            m_labelBombs.shadowOffset = CGSizeMake(-2,2);
            m_labelBombs.hidden = YES;
            
            m_labelScore = [[UILabel alloc] initWithFrame:CGRectMake(10.0, 120.0, 310.0, 100.0)];
            m_labelScore.textAlignment = NSTextAlignmentCenter;
            m_labelScore.textColor = [UIColor colorWithRed:241.0/255.0 green:223.0/255.0 blue:204.0/255.0 alpha:1.0];
            m_labelScore.backgroundColor = [UIColor clearColor];
            m_labelScore.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(20.0)];
            [m_vc.view addSubview: m_labelScore];
            m_labelScore.text = [NSString stringWithFormat:@""];
            m_labelScore.shadowColor = [UIColor blackColor];
            m_labelScore.shadowOffset = CGSizeMake(-2,2);
            m_labelScore.hidden = YES;
            
            m_labelCoinsWon = [[UILabel alloc] initWithFrame:CGRectMake(10.0, 144.0, 310.0, 100.0)];
            m_labelCoinsWon.textAlignment = NSTextAlignmentCenter;
            m_labelCoinsWon.textColor = [UIColor colorWithRed:241.0/255.0 green:223.0/255.0 blue:204.0/255.0 alpha:1.0];
            m_labelCoinsWon.backgroundColor = [UIColor clearColor];
            m_labelCoinsWon.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(20.0)];
            [m_vc.view addSubview: m_labelCoinsWon];
            m_labelCoinsWon.text = [NSString stringWithFormat:@""];
            m_labelCoinsWon.shadowColor = [UIColor blackColor];
            m_labelCoinsWon.shadowOffset = CGSizeMake(-2,2);
            m_labelCoinsWon.hidden = YES;
            
            System::TextureResource* pLoginButtonTextureResource = new System::TextureResource();
            pLoginButtonTextureResource->CreateFromFile("Art/login_button.png");
            m_pLoginButtonSprite = new System::Sprite(pLoginButtonTextureResource);
            m_pLoginButtonSprite->SetPosition(Math::vec2(170.f, 900.f));
            m_pLoginButtonSprite->SetLayer(10);
            m_pLoginButtonSprite->SetDraw(false);
            
            System::TextureResource* pPlayButtonTextureResource = new System::TextureResource();
            pPlayButtonTextureResource->CreateFromFile("Art/playnow_button.png");
            m_pPlayButtonSprite = new System::Sprite(pPlayButtonTextureResource);
            m_pPlayButtonSprite->SetDraw(false);
            m_pPlayButtonSprite->SetLayer(10);
            m_pPlayButtonSprite->SetPosition(Math::vec2(90.f, 440.f));
            
            System::TextureResource* pChallengeButtonTextureResource = new System::TextureResource();
            pChallengeButtonTextureResource->CreateFromFile("Art/challenge_button.png");
            m_pChallengeButtonSprite = new System::Sprite(pChallengeButtonTextureResource);
            m_pChallengeButtonSprite->SetDraw(false);
            m_pChallengeButtonSprite->SetLayer(10);
            m_pChallengeButtonSprite->SetPosition(Math::vec2(90.f, 570.f));
            
            System::TextureResource* pBragButtonTextureResource = new System::TextureResource();
            pBragButtonTextureResource->CreateFromFile("Art/brag_button.png");
            m_pBragButtonSprite = new System::Sprite(pBragButtonTextureResource);
            m_pBragButtonSprite->SetDraw(false);
            m_pBragButtonSprite->SetLayer(10);
            m_pBragButtonSprite->SetPosition(Math::vec2(90.f, 700.f));
            
            System::TextureResource* pLogoutButtonTextureResource = new System::TextureResource();
            pLogoutButtonTextureResource->CreateFromFile("Art/logout_button.png");
            m_pLogoutButtonSprite = new System::Sprite(pLogoutButtonTextureResource);
            m_pLogoutButtonSprite->SetPosition(Math::vec2(90.f, 1000.f));
            m_pLogoutButtonSprite->SetLayer(10);
            m_pLogoutButtonSprite->SetDraw(false);
            
            System::TextureResource* pMainMenuButtonTextureResource = new System::TextureResource();
            pMainMenuButtonTextureResource->CreateFromFile("Art/mainmenu_button.png");
            m_pMainMenuButtonSprite = new System::Sprite(pMainMenuButtonTextureResource);
            m_pMainMenuButtonSprite->SetPosition(Math::vec2(100.f, 862.f));
            m_pMainMenuButtonSprite->SetLayer(10);
            m_pMainMenuButtonSprite->SetDraw(false);
            
            System::TextureResource* pScoresButtonTextureResource = new System::TextureResource();
            pScoresButtonTextureResource->CreateFromFile("Art/scores_button.png");
            m_pScoresButtonSprite = new System::Sprite(pScoresButtonTextureResource);
            m_pScoresButtonSprite->SetDraw(false);
            m_pScoresButtonSprite->SetLayer(10);
            m_pScoresButtonSprite->SetPosition(Math::vec2(90.f, 830.f));
            
            System::TextureResource* pCoinButtonTextureResource = new System::TextureResource();
            pCoinButtonTextureResource->CreateFromFile("Art/coin_icon.png");
            m_pCoinButtonSprite = new System::Sprite(pCoinButtonTextureResource);
            m_pCoinButtonSprite->SetDraw(false);
            m_pCoinButtonSprite->SetLayer(10);
            m_pCoinButtonSprite->SetPosition(Math::vec2(200.f, 210.f));

            System::TextureResource* pBombAddButtonTextureResource = new System::TextureResource();
            pBombAddButtonTextureResource->CreateFromFile("Art/bombplus_icon.png");
            m_pBombAddButtonSprite = new System::Sprite(pBombAddButtonTextureResource);
            m_pBombAddButtonSprite->SetDraw(false);
            m_pBombAddButtonSprite->SetLayer(10);
            m_pBombAddButtonSprite->SetPosition(Math::vec2(400.f, 210.f));
            
            System::TextureResource* pLeaderboardCloseButtonTextureResource = new System::TextureResource();
            pLeaderboardCloseButtonTextureResource->CreateFromFile("Art/no_button.png");
            m_pLeaderboardCloseButtonSprite = new System::Sprite(pLeaderboardCloseButtonTextureResource);
            m_pLeaderboardCloseButtonSprite->SetDraw(false);
            m_pLeaderboardCloseButtonSprite->SetLayer(10);
            m_pLeaderboardCloseButtonSprite->SetPosition(Math::vec2(420.f, 1020.f));
            
            m_pGameOverCloseButtonSprite = new System::Sprite(pLeaderboardCloseButtonTextureResource);
            m_pGameOverCloseButtonSprite->SetDraw(false);
            m_pGameOverCloseButtonSprite->SetLayer(10);
            m_pGameOverCloseButtonSprite->SetPosition(Math::vec2(220.f, 960.f));
            
            
            
            
            
            System::TextureResource* pLeaderboardStubTextureResources[2];
            pLeaderboardStubTextureResources[0] = new System::TextureResource();
            pLeaderboardStubTextureResources[0]->CreateFromFile("Art/scorestub_odd.png");
            
            pLeaderboardStubTextureResources[1] = new System::TextureResource();
            pLeaderboardStubTextureResources[1]->CreateFromFile("Art/scorestub_even.png");
            
            m_vLeaderboardPosition = Math::vec2(0.f, 340.f);
            
            for (u32 i=0; i<m_kuLeaderboardSize; ++i)
            {
                m_pLeaderboardEntries[i].pStubSprite = new System::Sprite(pLeaderboardStubTextureResources[(i%2==0) ? 0 : 1]);
                m_pLeaderboardEntries[i].pStubSprite->SetDraw(false);
                m_pLeaderboardEntries[i].pStubSprite->SetLayer(10);
                m_pLeaderboardEntries[i].pStubSprite->SetPosition(Math::vec2((i%2==0) ? 128.f : 0.f, m_vLeaderboardPosition.y + (110.f * i)));
                
                m_pLeaderboardEntries[i].pFriendName = [[UILabel alloc] initWithFrame:CGRectMake(44.0, (m_vLeaderboardPosition.y * 0.5f) + -34.f + (55.f * i), 232.0, 100.0)];
                m_pLeaderboardEntries[i].pFriendName.textAlignment = (i%2==0) ? NSTextAlignmentRight : NSTextAlignmentLeft;
                m_pLeaderboardEntries[i].pFriendName.textColor = [UIColor colorWithRed:0.84 green:0.84 blue:0.84 alpha:1.0];
                m_pLeaderboardEntries[i].pFriendName.backgroundColor = [UIColor clearColor];
                m_pLeaderboardEntries[i].pFriendName.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(18.0)];
                m_pLeaderboardEntries[i].pFriendName.text = [NSString stringWithFormat:@""];
                m_pLeaderboardEntries[i].pFriendName.hidden = YES;
                
                m_pLeaderboardEntries[i].pFriendScore = [[UILabel alloc] initWithFrame:CGRectMake(44.0, (m_vLeaderboardPosition.y * 0.5f) + -14.f + (55.f * i), 232.0, 100.0)];
                m_pLeaderboardEntries[i].pFriendScore.textAlignment = (i%2==0) ? NSTextAlignmentRight : NSTextAlignmentLeft;
                m_pLeaderboardEntries[i].pFriendScore.textColor = [UIColor colorWithRed:0.84 green:0.84 blue:0.84 alpha:1.0];
                m_pLeaderboardEntries[i].pFriendScore.backgroundColor = [UIColor clearColor];
                m_pLeaderboardEntries[i].pFriendScore.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(15.0)];
                m_pLeaderboardEntries[i].pFriendScore.text = [NSString stringWithFormat:@""];
                m_pLeaderboardEntries[i].pFriendScore.hidden = YES;
                
            }
            
            m_pFriendPermissionUpsellDialog = new AlertDialog();
            m_pFriendPermissionUpsellDialog->Initialise(m_vc, @"Play with Friends?", @"Friend Smash! is more fun with friends! \n Connect to your friends on Facebook?", didDismissFriendPermissionUpsellDialog);
            
            m_pWritePermissionUpsellDialog = new AlertDialog();
            m_pWritePermissionUpsellDialog->Initialise(m_vc, @"Post to Facebook?", @"Share your score with your friends! \n Okay to share?", didDismissWritePermissionUpsellDialog);
            
            m_pMultiFriendSelector = new MultiFriendSelector();
            m_pMultiFriendSelector->Initialise(m_vc, didDismissMultiFriendSelector);
            
#ifndef NO_FACEBOOK_INTEGRATION
            if (!FacebookController::IsLoggedIn()) {
                
                DidChangeFBLoginState(false);
                
                FacebookController::CreateNewSession();
                FacebookController::OpenSession(didLogIn);
            }
            else {
                DidChangeFBLoginState(true);
            }
#else
            m_pLoginButtonSprite->SetDraw(false);
            m_pPlayButtonSprite->SetDraw(true);
            
#endif
        }
        
        void MenuController::OnExit()
        {
            m_pBackgroundSprite->SetDraw(false);
            delete m_pBackgroundSprite;
            m_pBackgroundSprite = NULL;
            
            m_pLoginButtonSprite->SetDraw(false);
            delete m_pLoginButtonSprite;
            m_pLoginButtonSprite = NULL;
    
            delete m_pUserTexture;
            m_pUserTexture = NULL;
            
            if (m_pUserImageSprite) {
                m_pUserImageSprite->SetDraw(false);
                delete m_pUserImageSprite;
                m_pUserImageSprite = NULL;
            }
            
            m_pPlayButtonSprite->SetDraw(false);
            delete m_pPlayButtonSprite;
            m_pPlayButtonSprite = NULL;
            
            m_pChallengeButtonSprite->SetDraw(false);
            delete m_pChallengeButtonSprite;
            m_pChallengeButtonSprite = NULL;
            
            m_pScoresButtonSprite->SetDraw(false);
            delete m_pScoresButtonSprite;
            m_pScoresButtonSprite = NULL;
            
            m_pBragButtonSprite->SetDraw(false);
            delete m_pBragButtonSprite;
            m_pBragButtonSprite = NULL;
            
            m_pLogoutButtonSprite->SetDraw(false);
            delete m_pLogoutButtonSprite;
            m_pLogoutButtonSprite = NULL;
            
            m_pCoinButtonSprite->SetDraw(false);
            delete m_pCoinButtonSprite;
            m_pCoinButtonSprite = NULL;
            
            m_pBombAddButtonSprite->SetDraw(false);
            delete m_pBombAddButtonSprite;
            m_pBombAddButtonSprite = NULL;
            
            m_labelName.hidden = YES;
            m_labelName = nil;
            
            m_labelBombs.hidden = YES;
            m_labelBombs = nil;
            
            m_labelCoins.hidden = YES;
            m_labelCoins = nil;
            
            m_labelScore.hidden = YES;
            m_labelScore = nil;
            
            m_labelCoinsWon.hidden = YES;
            m_labelCoinsWon = nil;
            
            m_pFriendPermissionUpsellDialog->Uninitialise();
            delete m_pFriendPermissionUpsellDialog;
            m_pFriendPermissionUpsellDialog = NULL;
            
            for (u32 i=0; i<m_kuLeaderboardSize; ++i) {
                delete  m_pLeaderboardEntries[i].pStubSprite;
                
                m_pLeaderboardEntries[i].pFriendName = nil;
                m_pLeaderboardEntries[i].pFriendScore = nil;
            }
        }
        
        void MenuController::OnUpdate()
        {
            if (m_pMultiFriendSelector->GetActive()) {
                m_pMultiFriendSelector->Update();
            }
            else
            {
                if (m_pUserTexture && m_pUserTexture->GetIsReady() && FacebookController::IsLoggedIn() && m_pUserImageSprite == NULL)
                {
                    m_pUserImageSprite = new System::Sprite(m_pUserTexture);
                    m_pUserImageSprite->SetLayer(10);
                    m_pUserImageSprite->SetPivot(Math::vec2(m_pUserImageSprite->GetWidth()*0.5f, m_pUserImageSprite->GetHeight()*0.5f));
                    m_pUserImageSprite->SetPosition(Math::vec2(99.f, 216.f));
                    m_pUserImageSprite->SetScale(Math::vec2(0.5975f, 0.5975f));
                }
                
                if (m_bViewingLeaderboard) {
                    UpdateLeaderboard();
                }
            }

        }
        
        void MenuController::OnRender()
        {
            
        }
        
        
        void MenuController::OnTapDown(int nIndex, Math::vec2 vPosition)
        {
            
        }
        
        void MenuController::OnTapUp(int nIndex, Math::vec2 vPosition)
        {
#ifndef NO_FACEBOOK_INTEGRATION
            if (m_pFriendPermissionUpsellDialog->GetActive()) {
                m_pFriendPermissionUpsellDialog->OnTapUp(nIndex, vPosition);
                return;
            }
            
            if (m_pWritePermissionUpsellDialog->GetActive()) {
                m_pWritePermissionUpsellDialog->OnTapUp(nIndex, vPosition);
                return;
            }
            
            if (m_pMultiFriendSelector->GetActive()) {
                m_pMultiFriendSelector->OnTapUp(nIndex, vPosition);
                return;
            }
#endif
            
            if (m_pLoginButtonSprite->GetDraw() && m_pLoginButtonSprite->IsPointInside(vPosition, 25.f))
            {
#ifndef NO_FACEBOOK_INTEGRATION
                FacebookController::Login(didLogIn);
#endif
            }
            else if (m_pLogoutButtonSprite->GetDraw() && m_pLogoutButtonSprite->IsPointInside(vPosition, 25.f))
            {
#ifndef NO_FACEBOOK_INTEGRATION
                FacebookController::Logout(didLogout);
#endif
            }
            else if (m_pPlayButtonSprite->GetDraw() && m_pPlayButtonSprite->IsPointInside(vPosition, 25.f))
            {
               
#ifndef NO_FACEBOOK_INTEGRATION
                
                // We need to check if we have the user's friend permissions
                if (FacebookController::IsLoggedIn()) {
                    FacebookController::CheckForPermission(@"user_friends", didFetchUserFriendsPermissionForStart);
                }
                else {
                    StartNewGame(false);
                }
                
#else
                StartNewGame(false);
#endif
            }
            else if (m_pBombAddButtonSprite->GetDraw() && m_pBombAddButtonSprite->IsPointInside(vPosition, 25.f))
            {
                // Add bombs
                if (ms_uCoins >= 5) {
                    ms_uCoins-=5;
                    ms_uBombs++;
                    
                    m_labelCoins.text = [NSString stringWithFormat:@"%d", ms_uCoins];
                    m_labelBombs.text = [NSString stringWithFormat:@"%d", ms_uBombs];
                }
            }
            else if (m_pChallengeButtonSprite->GetDraw() && m_pChallengeButtonSprite->IsPointInside(vPosition, 25.f))
            {
#ifndef NO_FACEBOOK_INTEGRATION
                if (!m_bViewingGameover) {
                    FacebookController::CheckForPermission(@"user_friends", didFetchUserFriendsPermissionForMFS);
                }
                else {
                    
                    NSMutableArray *friends = [NSMutableArray new];
                    NSString *nsstrId = [NSString stringWithFormat:@"%llu", m_uLastFriendID];
                    
                    [friends addObject:nsstrId];
                    
                    FacebookController::SendInvite(friends);
                }

#endif
            }
            else if (m_pBragButtonSprite->GetDraw() && m_pBragButtonSprite->IsPointInside(vPosition, 25.f))
            {
#ifndef NO_FACEBOOK_INTEGRATION
                FacebookController::SendBrag(m_uScore);
#endif
            }
            else if (m_pScoresButtonSprite->GetDraw() && m_pScoresButtonSprite->IsPointInside(vPosition, 25.f))
            {
#ifndef NO_FACEBOOK_INTEGRATION
                FacebookController::GetScores(didFetchScores);
#endif
            }
            else if (m_pLeaderboardCloseButtonSprite->GetDraw() && m_pLeaderboardCloseButtonSprite->IsPointInside(vPosition, 25.f))
            {
#ifndef NO_FACEBOOK_INTEGRATION
                ViewLeaderboard(false);
#endif
            }
            else if (m_pGameOverCloseButtonSprite->GetDraw() && m_pGameOverCloseButtonSprite->IsPointInside(vPosition, 25.f))
            {
                m_bViewingGameover = false;
                m_pPlayButtonSprite->SetDraw(true);
                m_pGameOverCloseButtonSprite->SetDraw(false);
                
                if (!FacebookController::IsLoggedIn()) {
#ifndef NO_FACEBOOK_INTEGRATION
                    m_pLoginButtonSprite->SetDraw(true);
#endif
                }
                else {
                    m_pLogoutButtonSprite->SetDraw(true);
                    m_pScoresButtonSprite->SetDraw(true);
                }
            }
            
        }
        
        
        void MenuController::StartNewGame(bool bSocial)
        {
            u32 uBombsToPlay = ms_uBombs > 3 ? 3 : ms_uBombs;
            m_fsc->StartNewGame(uBombsToPlay, bSocial);
            ms_uBombs -= uBombsToPlay;
        }
        
        void MenuController::DidFetchUserFriendsPermissionForStart(bool bSuccess)
        {
            // If we have the user friends - we can start a social game
            if (bSuccess) {
                StartNewGame(FacebookController::IsLoggedIn());
            }
            else {
                // If we haven't tried to upsell this person to grant friend permissios before, we should do it now...
                m_pFriendPermissionUpsellDialog->Spawn();
                m_labelScore.hidden = YES;
                m_labelCoinsWon.hidden = YES;
            }
        }
        
        void MenuController::DidFetchUserFriendsPermissionForMFS(bool bSuccess)
        {
#ifndef NO_FACEBOOK_INTEGRATION
            if (bSuccess) {
                m_pMultiFriendSelector->Spawn();
                m_pChallengeButtonSprite->SetDraw(false);
                m_pPlayButtonSprite->SetDraw(false);
                m_pScoresButtonSprite->SetDraw(false);
                m_pBragButtonSprite->SetDraw(false);
                m_pLogoutButtonSprite->SetDraw(false);
                
                [m_labelCoinsWon setHidden:YES];
                [m_labelScore setHidden:YES];
            }
            else {
                FacebookController::SendInvite(NULL);
            }
#endif
        }
        
        void MenuController::DidDismissFriendPermissionUpsellDialog(bool bResult)
        {
            m_pFriendPermissionUpsellDialog->Dismiss();
            
            if (bResult) {
                
#ifndef NO_FACEBOOK_INTEGRATION
                FacebookController::ReRequestFriendPermission();
#endif
            }
            else {
                StartNewGame(FacebookController::IsLoggedIn());
            }
            
        }
        
        void MenuController::DidFetchPublishActionsPermission(bool bResult)
        {
            if (bResult) {
                FacebookController::SendScore(m_uScore);
            }
            else {
                m_pWritePermissionUpsellDialog->Spawn();
                m_labelScore.hidden = YES;
                m_labelCoinsWon.hidden = YES;
            }
        }
        
        void MenuController::DidDismissWritePermissionUpsellDialog(bool bResult)
        {
            m_pWritePermissionUpsellDialog->Dismiss();
            
            if (bResult) {
#ifndef NO_FACEBOOK_INTEGRATION
                FacebookController::RequestWritePermissions();
#endif
            }
            else {
            }
        }
        
        void MenuController::DidDismissMultiFriendSelector(NSArray *friend_ids)
        {
#ifndef NO_FACEBOOK_INTEGRATION
            if (m_pMultiFriendSelector->GetActive()) {
                
                m_pChallengeButtonSprite->SetDraw(true);
                m_pPlayButtonSprite->SetDraw(true);
                m_pScoresButtonSprite->SetDraw(true);
                m_pBragButtonSprite->SetDraw(true);
                m_pLogoutButtonSprite->SetDraw(true);
                
                if (m_pMultiFriendSelector->GetInviteMode()) {
                    FacebookController::SendInvite(friend_ids);
                }
                else {
                    FacebookController::SendRequest(friend_ids, m_uScore);
                }
                
                
                m_pMultiFriendSelector->Dismiss();
            }
#endif
        }
        
        
        void MenuController::ViewLeaderboard(bool bView)
        {
            // Head into scores view?
            if (bView)
            {
                for (u32 i=0; i<m_kuLeaderboardSize; ++i)
                {
                    m_pLeaderboardEntries[i].pStubSprite->SetDraw(true);
                    [m_pLeaderboardEntries[i].pFriendName setHidden:NO];
                    [m_pLeaderboardEntries[i].pFriendScore setHidden:NO];
                    
                    [m_vc.view addSubview:m_pLeaderboardEntries[i].pFriendName];
                    [m_vc.view addSubview:m_pLeaderboardEntries[i].pFriendScore];
                }
                
                [m_labelCoinsWon setHidden:YES];
                [m_labelScore setHidden:YES];
                
                
                m_pPlayButtonSprite->SetDraw(false);
                m_pChallengeButtonSprite->SetDraw(false);
                m_pBragButtonSprite->SetDraw(false);
                m_pLogoutButtonSprite->SetDraw(false);
                m_pScoresButtonSprite->SetDraw(false);
                
                m_bViewingLeaderboard = true;
                m_pLeaderboardCloseButtonSprite->SetDraw(true);
            }
            else
            {
                for (u32 i=0; i<m_kuLeaderboardSize; ++i)
                {
                    m_pLeaderboardEntries[i].pStubSprite->SetDraw(false);
                    
                    [m_pLeaderboardEntries[i].pFriendName removeFromSuperview];
                    [m_pLeaderboardEntries[i].pFriendScore removeFromSuperview];
                    
                    delete m_pLeaderboardEntries[i].pUserSprite;
                    m_pLeaderboardEntries[i].pUserSprite = NULL;
                    
                    delete m_pLeaderboardEntries[i].pUserTexture;
                    m_pLeaderboardEntries[i].pUserTexture = NULL;
                }
                
                [m_labelCoinsWon setHidden:NO];
                [m_labelScore setHidden:NO];
                
                
                m_pPlayButtonSprite->SetDraw(true);
                m_pChallengeButtonSprite->SetDraw(true);
                m_pLogoutButtonSprite->SetDraw(true);
                m_pBragButtonSprite->SetDraw(true);
                m_pScoresButtonSprite->SetDraw(true);
                
                m_pLeaderboardCloseButtonSprite->SetDraw(false);
                
                m_bViewingLeaderboard = false;
            }
        }
        
        void MenuController::UpdateLeaderboard()
        {
            for (u32 i=0; i<m_kuLeaderboardSize; ++i)
            {
                if (m_pLeaderboardEntries[i].pUserTexture && m_pLeaderboardEntries[i].pUserTexture->GetIsReady() && !m_pLeaderboardEntries[i].pUserSprite)
                {
                    m_pLeaderboardEntries[i].pUserSprite = new System::Sprite(m_pLeaderboardEntries[i].pUserTexture);
                    m_pLeaderboardEntries[i].pUserSprite->SetLayer(20);
                    m_pLeaderboardEntries[i].pUserSprite->SetPosition(Math::vec2((i%2==0) ? 567.f : 8.f, m_vLeaderboardPosition.y + 20.f + (110.f * i)));
                }
            }
        }
        
        void MenuController::DidFetchScores(NSArray *scoreData)
        {
            if (scoreData == nil) {
                return;
            }
#ifndef NO_FACEBOOK_INTEGRATION
            for (int i=0; i<m_kuLeaderboardSize; ++i) {
                NSDictionary *friendData = [scoreData objectAtIndex:i];
                
                NSString *name = [[[[friendData objectForKey:@"user"] objectForKey:@"name"] componentsSeparatedByString:@" "] objectAtIndex:0];
                NSString *strScore = [friendData objectForKey:@"score"];
                
                
                m_pLeaderboardEntries[i].pFriendName.text = [NSString stringWithFormat:@"%d. %@", i+1, name];
                m_pLeaderboardEntries[i].pFriendScore.text = [NSString stringWithFormat:@"Score: %@", strScore];
                
                delete m_pLeaderboardEntries[i].pUserSprite;
                m_pLeaderboardEntries[i].pUserSprite = NULL;
                
                u64 uFriendID = [[[friendData objectForKey:@"user"] objectForKey:@"id"] longLongValue];
                m_pLeaderboardEntries[i].pUserTexture = new System::TextureResource();
                m_pLeaderboardEntries[i].pUserTexture->CreateFromFBID(uFriendID, 64, 64);
            }
            
            
            ViewLeaderboard(true);
#endif
        }
        
        void MenuController::ShowEndGame(NSString* friendName, u64 uFriendID, u32 uScore, u32 uCoinsWon, u32 uBombsRemaining)
        {
            ms_uCoins += uCoinsWon;
            ms_uBombs += uBombsRemaining;
            m_uScore = uScore;
            m_uLastFriendID = uFriendID;
            
            m_labelScore.text = uScore == 1 ? [NSString stringWithFormat:@"You smashed %@ %u time!", friendName, uScore] : [NSString stringWithFormat:@"You smashed %@ %u times!", friendName, uScore];
            m_labelScore.hidden = NO;
            
            m_labelCoinsWon.text = uCoinsWon == 1 ? [NSString stringWithFormat:@"Collected %u coin!", uCoinsWon] : [NSString stringWithFormat:@"Collected %u coins!", uCoinsWon];
            m_labelCoinsWon.hidden = NO;
           
            m_labelCoins.text = [NSString stringWithFormat:@"%d", ms_uCoins];
            m_labelBombs.text = [NSString stringWithFormat:@"%d", ms_uBombs];
            
            m_pPlayButtonSprite->SetDraw(false);
            m_pScoresButtonSprite->SetDraw(false);
            m_pLogoutButtonSprite->SetDraw(false);
            m_pGameOverCloseButtonSprite->SetDraw(true);
            m_pLoginButtonSprite->SetDraw(false);
            
            m_bViewingGameover = true;
            
#ifndef NO_FACEBOOK_INTEGRATION
            
            if (FacebookController::IsLoggedIn()) {
                m_pChallengeButtonSprite->SetDraw(true);
                m_pBragButtonSprite->SetDraw(true);
                
                FacebookController::CheckForPermission(@"publish_actions", didFetchPublishActionsPermission);
            }
#endif
        }
        
        void MenuController::DidChangeFBLoginState(bool bLoggedIn)
        {
            if (bLoggedIn) {
                m_pLoginButtonSprite->SetDraw(false);
#ifndef NO_FACEBOOK_INTEGRATION
                FacebookController::SetLoggedIn(true);
                FacebookController::FetchUserDetails(didFetchUserDetails);
#endif
                
                m_pCoinButtonSprite->SetDraw(true);
                m_pBombAddButtonSprite->SetDraw(true);
                
                m_pChallengeButtonSprite->SetDraw(true);
                
                m_pBragButtonSprite->SetDraw(true);
                
                if (!m_bViewingGameover) {
                    m_pLogoutButtonSprite->SetDraw(true);
                    m_pScoresButtonSprite->SetDraw(true);
                    m_pPlayButtonSprite->SetDraw(true);
                }
                
                m_labelCoins.text = [NSString stringWithFormat:@"%d", ms_uCoins];
                m_labelBombs.text = [NSString stringWithFormat:@"%d", ms_uBombs];
                
                m_labelCoins.hidden = NO;
                m_labelBombs.hidden = NO;
                
            }
            else {
                m_pLogoutButtonSprite->SetDraw(false);
                m_pChallengeButtonSprite->SetDraw(false);
                m_pBragButtonSprite->SetDraw(false);
                m_pScoresButtonSprite->SetDraw(false);
                m_pLoginButtonSprite->SetDraw(true);
                m_labelName.text = [NSString stringWithFormat:@"Welcome, Player"];

                m_pCoinButtonSprite->SetDraw(false);
                m_pBombAddButtonSprite->SetDraw(false);
                
                m_pPlayButtonSprite->SetDraw(true);
                
                m_labelCoins.hidden = YES;
                m_labelBombs.hidden = YES;
                
                m_labelCoinsWon.hidden = YES;
                
                delete m_pUserImageSprite;
                m_pUserImageSprite = NULL;
            }
        }
        
        void MenuController::DidFetchFBUserInfo(bool bSuccess)
        {
#ifndef NO_FACEBOOK_INTEGRATION
            if (m_bViewingGameover) {
                m_labelName.text = [NSString stringWithFormat:@"Game Over!"];
            }
            else {
                m_labelName.text = [NSString stringWithFormat:@"Welcome, %@", FacebookController::GetUserFirstName()];
            }

            
            // Create a texture from the user's profile picture
            m_pUserTexture = new System::TextureResource();
            m_pUserTexture->CreateFromFBID(FacebookController::GetUserFBID(), 256, 256);
#endif
        }
        
        void MenuController::ProcessIncomingURL(NSURL* url)
        {
#ifndef NO_FACEBOOK_INTEGRATION
            FacebookController::ProcessIncomingURL(url, didProcessFBURL);
#endif
        }
        
        void MenuController::DidProcessIncomingURL(NSString* challengeName, NSString* challengeFBID)
        {
            m_fsc->StartNewChallengeGame(ms_uBombs, challengeName, challengeFBID);
        }
        
    }
}