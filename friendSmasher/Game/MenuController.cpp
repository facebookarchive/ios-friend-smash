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

#include "MenuController.h"
#include "FBLogin.h"
#include "FBShare.h"
#include "FBGraph.h"
#include "FBRequest.h"
#include "FBAppEvents.h"

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

        static void didFetchScores(NSArray* scores) {
            if (kpMenuController) {
                kpMenuController->DidFetchScores(scores);
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
            
            if ([FBLogin isLoggedIn]){
                DidChangeFBLoginState(true);
            }
            else {
                DidChangeFBLoginState(false);
            }
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
                if (m_pUserTexture && m_pUserTexture->GetIsReady() && [FBLogin isLoggedIn] && m_pUserImageSprite == NULL)
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
            
            if (m_pLoginButtonSprite->GetDraw() && m_pLoginButtonSprite->IsPointInside(vPosition, 25.f))
            {
                [FBLogin loginFromViewController:kpMenuController->m_vc callback:^(BOOL success){
                    didLogIn(success);
                }];
            }
            else if (m_pLogoutButtonSprite->GetDraw() && m_pLogoutButtonSprite->IsPointInside(vPosition, 25.f))
            {
                [FBLogin logoutWithCallback:^(BOOL success){
                    didLogout(success);
                }];
            }
            else if (m_pPlayButtonSprite->GetDraw() && m_pPlayButtonSprite->IsPointInside(vPosition, 25.f))
            {
                // We need to check if we have the user's friend permissions
                if ([FBLogin isLoggedIn]) {
                    didFetchUserFriendsPermissionForStart ([FBLogin hasPermission:@"user_friends"]);
                }
                else {
                    StartNewGame(false);
                }
            }
            else if (m_pBombAddButtonSprite->GetDraw() && m_pBombAddButtonSprite->IsPointInside(vPosition, 25.f))
            {
                // Add bombs
                u32 price = 5;
                
                if (ms_uCoins >= price) {
                    ms_uCoins-=price;
                    ms_uBombs++;
                    
                    m_labelCoins.text = [NSString stringWithFormat:@"%d", ms_uCoins];
                    m_labelBombs.text = [NSString stringWithFormat:@"%d", ms_uBombs];
                    
                    [FBAppEvents sendSpentCreditEvent:price];
                }
            }
            else if (m_pChallengeButtonSprite->GetDraw() && m_pChallengeButtonSprite->IsPointInside(vPosition, 25.f))
            {
                if (!m_bViewingGameover) {
                    didFetchUserFriendsPermissionForMFS([FBLogin hasPermission:@"user_friends"]);
                }
                else {
                    
                    NSMutableArray *friends = [NSMutableArray new];
                    NSString *nsstrId = [NSString stringWithFormat:@"%llu", m_uLastFriendID];
                    
                    [friends addObject:nsstrId];
                    
                    [FBRequest sendInviteWithFriendIDs:friends];
                }
            }
            else if (m_pBragButtonSprite->GetDraw() && m_pBragButtonSprite->IsPointInside(vPosition, 25.f))
            {
                // default FB share
                // [FBShare sendBragWithScore:m_uScore];
                
                // iOS Share sheet
                // [FBShare shareSheetOSWithViewController:m_vc];
                
                // FB share with sharesheet
                [FBShare shareSheetFBWithViewController:m_vc];            
                
            }
            else if (m_pScoresButtonSprite->GetDraw() && m_pScoresButtonSprite->IsPointInside(vPosition, 25.f))
            {
                [FBGraph fetchScoresWithCallback:^(NSArray *scores){
                    DidFetchScores(scores);
                }];
            }
            else if (m_pLeaderboardCloseButtonSprite->GetDraw() && m_pLeaderboardCloseButtonSprite->IsPointInside(vPosition, 25.f))
            {
                ViewLeaderboard(false);
            }
            else if (m_pGameOverCloseButtonSprite->GetDraw() && m_pGameOverCloseButtonSprite->IsPointInside(vPosition, 25.f))
            {
                m_bViewingGameover = false;
                m_pPlayButtonSprite->SetDraw(true);
                m_pGameOverCloseButtonSprite->SetDraw(false);
                
                if (![FBLogin isLoggedIn]) {
                    m_pLoginButtonSprite->SetDraw(true);
                }
                else {
                    m_pLogoutButtonSprite->SetDraw(true);
                    m_pScoresButtonSprite->SetDraw(true);
                }
            }
            
        }
        
        
        void MenuController::StartNewGame(bool bSocial)
        {
            [FBAppEvents sendPlayEvent];
            
            u32 uBombsToPlay = ms_uBombs > 3 ? 3 : ms_uBombs;
            m_fsc->StartNewGame(uBombsToPlay, bSocial);
            ms_uBombs -= uBombsToPlay;
        }
        
        void MenuController::DidFetchUserFriendsPermissionForStart(bool bSuccess)
        {
            // If we have the user friends - we can start a social game
            if (bSuccess) {
                StartNewGame([FBLogin isLoggedIn]);
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
                [FBRequest sendInviteWithFriendIDs:nil];
            }
        }
        
        void MenuController::DidDismissFriendPermissionUpsellDialog(bool bResult)
        {
            m_pFriendPermissionUpsellDialog->Dismiss();
            
            if (bResult) {
                
                
                [FBLogin requestFriendPermissionFromViewController:kpMenuController->m_vc callback:^(BOOL success){
                    
                }];
            }
            else {
                StartNewGame([FBLogin isLoggedIn]);
            }
            
        }
        
        void MenuController::DidFetchPublishActionsPermission(bool bResult)
        {
            if (bResult) {
                [FBShare sendScore:m_uScore];
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
                [FBLogin requestWritePermissionFromViewController:kpMenuController->m_vc callback:^(BOOL success){
                    
                }];
            }
            else {
            }
        }
        
        void MenuController::DidDismissMultiFriendSelector(NSArray *friend_ids)
        {
            if (m_pMultiFriendSelector->GetActive()) {
                
                m_pChallengeButtonSprite->SetDraw(true);
                m_pPlayButtonSprite->SetDraw(true);
                m_pScoresButtonSprite->SetDraw(true);
                m_pBragButtonSprite->SetDraw(true);
                m_pLogoutButtonSprite->SetDraw(true);
                
                if (m_pMultiFriendSelector->GetInviteMode()) {
                    [FBRequest sendInviteWithFriendIDs:friend_ids];
                }
                else {
                    [FBRequest sendRequestWithFriendIDs:friend_ids score:m_uScore];
                }
                
                m_pMultiFriendSelector->Dismiss();
            }
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
            for (int i=0; i<m_kuLeaderboardSize; ++i) {
                
                if ([scoreData count] <= i)
                    break;
                
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
            
            if ([FBLogin isLoggedIn]) {
                m_pChallengeButtonSprite->SetDraw(true);
                m_pBragButtonSprite->SetDraw(true);
                
                didFetchPublishActionsPermission([FBLogin hasPermission:@"publish_actions"]);
            }
        }
        
        void MenuController::DidChangeFBLoginState(bool bLoggedIn)
        {
            if (bLoggedIn) {
                m_pLoginButtonSprite->SetDraw(false);
                [FBGraph fetchMeDetailsWithCallback:^(FBUserDetails *userDetails){
                    BOOL success = userDetails != nil;
                    didFetchUserDetails(success);
                }];
                
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
            if (m_bViewingGameover) {
                m_labelName.text = [NSString stringWithFormat:@"Game Over!"];
            }
            else {
                m_labelName.text = [NSString stringWithFormat:@"Welcome, %@", [FBGraph meFirstName]];
            }

            // Create a texture from the user's profile picture
            m_pUserTexture = new System::TextureResource();
            m_pUserTexture->CreateFromFBID([FBGraph meFbid], 256, 256);
        }
        
        void MenuController::ProcessIncomingURL(NSURL* url)
        {
            // TODO: process incoming URL
        }
        
        void MenuController::DidProcessIncomingURL(NSString* challengeName, NSString* challengeFBID)
        {
            m_fsc->StartNewChallengeGame(ms_uBombs, challengeName, challengeFBID);
        }
        
    }
}