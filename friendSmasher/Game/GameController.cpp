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

#include "GameController.h"

#include "debug.h"
#include "math_lib.h"
#include "random.h"
#include <stdlib.h>
#include <math.h>

namespace FriendSmasher
{
    namespace Game
    {
        
        static GameController* kpGameController;
        
        static void didFetchFriendInfo(NSArray* friends) {
            if (kpGameController) {
                kpGameController->DidFetchFBFriendInfo(friends);
            }
        }
        
        
        GameController::GameController(ViewController* vc, FriendSmashController* fsc) :
            m_vc(vc),
            m_fsc(fsc),
            m_fDeltaTime(0.f),
            m_kGameState(kGAMESTATE_LOADING),
            m_pBackgroundSprite(NULL),
            m_pFriendImageSprite(NULL),
            m_pLoadingSprite(NULL),
            m_pLoadingSpinner(NULL),
            m_pFriendTexture(NULL),
            m_pCoinTexture(NULL),
            m_pDropTheBombSprite(NULL),
            m_nEntitiesSpawned(0),
            m_fSpawnTimer(0.f),
            m_uLivesRemaining(0),
            m_uBombsRemaining(0),
            m_uCurrentScore(0),
            m_kLossType(kUNDEFINED),
            m_pCelebLossEntity(NULL),
            m_confettiEffect(NULL),
            m_nNoSocialFriendCeleb(0),
            m_uFriendFBID(0),
            m_friendName(nil),
            m_uCoins(0),
            m_bSocial(false)
        {
            kpGameController = this;
            
            for (int i=0; i<3; ++i) {
                m_pRiserTextures[i] = NULL;
            }
            
            for (int i=0; i<10; ++i) {
                m_pNonFriendTexture[i] = NULL;
            }
                       
            for (u32 i=0; i<3; ++i) {
                m_pHeartSprite[i] = NULL;
                m_pBombSprite[i] = NULL;

            }
            
            for (u32 i=0; i<GameController::kACHIEVEMENT_MAX; ++i) {
                m_bShouldSendAchievement[i] = false;
            }
            
            
        }

        GameController::~GameController()
        {
        }
		
        void GameController::OnEnter()
        {
            System::TextureResource* pBackgroundTextureResource = new System::TextureResource();
            pBackgroundTextureResource->CreateFromFile("Art/gamescreen_background.png");
            m_pBackgroundSprite = new System::Sprite(pBackgroundTextureResource);
            
            m_labelFriendName = [[UILabel alloc] initWithFrame:CGRectMake(6.0, 0.0, 640.0, 34.0)];
            m_labelFriendName.textAlignment = NSTextAlignmentLeft;
            m_labelFriendName.textColor = [UIColor colorWithRed:0.89 green:0.29 blue:0.14 alpha:1.0];
            m_labelFriendName.backgroundColor = [UIColor clearColor];
            m_labelFriendName.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(28.0)];
            [m_vc.view addSubview: m_labelFriendName];
            m_labelFriendName.text = [NSString stringWithFormat:@""];
            m_labelFriendName.hidden = YES;
            m_labelFriendName.shadowColor = [UIColor blackColor];
            m_labelFriendName.shadowOffset = CGSizeMake(0,1);
            
            m_labelScore = [[UILabel alloc] initWithFrame:CGRectMake(214.0, 6.0, 100.0, 20.0)];
            m_labelScore.textAlignment = NSTextAlignmentRight;
            m_labelScore.textColor = [UIColor whiteColor];
            m_labelScore.backgroundColor = [UIColor clearColor];
            m_labelScore.font = [UIFont fontWithName:@"Avenir Next Condensed" size:(20.0)];
            [m_vc.view addSubview: m_labelScore];
            m_labelScore.text = [NSString stringWithFormat:@"Score: 0"];
            m_labelScore.hidden = YES;
            m_labelScore.shadowColor = [UIColor blackColor];
            m_labelScore.shadowOffset = CGSizeMake(0,1);
            
            m_pCoinTexture = new System::TextureResource();
            m_pCoinTexture->CreateFromFile("Art/coin_icon.png");

            
            
            System::TextureResource* pLoadingTextureResource = new System::TextureResource();
            pLoadingTextureResource->CreateFromFile("Art/isloading.png");
            m_pLoadingSprite = new System::Sprite(pLoadingTextureResource);
            m_pLoadingSprite->SetDraw(false);
            m_pLoadingSprite->SetLayer(10);
            m_pLoadingSprite->SetPosition(Math::vec2(210.f, 460.f));
            
            System::TextureResource* pLoadingSpinnerTextureResource = new System::TextureResource();
            pLoadingSpinnerTextureResource->CreateFromFile("Art/loadingspinner.tga");
            m_pLoadingSpinner = new System::Sprite(pLoadingSpinnerTextureResource);
            m_pLoadingSpinner->SetDraw(false);
            m_pLoadingSpinner->SetLayer(10);
            m_pLoadingSpinner->SetPivot(Math::vec2(m_pLoadingSpinner->GetWidth()*0.5f, m_pLoadingSpinner->GetHeight()*0.5f));
            m_pLoadingSpinner->SetPosition(Math::vec2(274.f, 504.f));
            
            
            System::TextureResource* pBombDropTextureResource = new System::TextureResource();
            pBombDropTextureResource->CreateFromFile("Art/bombbutton.png");
            m_pDropTheBombSprite = new System::Sprite(pBombDropTextureResource);
            m_pDropTheBombSprite->SetDraw(false);
            m_pDropTheBombSprite->SetLayer(20);
            m_pDropTheBombSprite->SetPosition(Math::vec2(-20.f, 930.f));
        
            
            System::TextureResource* pHeartTextureResource = new System::TextureResource();
            pHeartTextureResource->CreateFromFile("Art/heart_red.tga");

            System::TextureResource* pBombTextureResource = new System::TextureResource();
            pBombTextureResource->CreateFromFile("Art/bomb_red.tga");
            
            for (u32 i=0; i<3; ++i)
            {
                m_pHeartSprite[i] = new System::Sprite(pHeartTextureResource);
                m_pHeartSprite[i]->SetDraw(false);
                m_pHeartSprite[i]->SetLayer(20 + i);
                m_pHeartSprite[i]->SetPosition(Math::vec2(10.f + (72.f * static_cast<float>(i)), 60.f));
                
                m_pBombSprite[i] = new System::Sprite(pBombTextureResource);
                m_pBombSprite[i]->SetDraw(false);
                m_pBombSprite[i]->SetLayer(20 + i);
                m_pBombSprite[i]->SetPosition(Math::vec2(8.f + (72.f * static_cast<float>(i)), 140.f));
                
            }
                 
            for (int i=0; i<10; ++i)
            {
                m_pNonFriendTexture[i] = new System::TextureResource();
                char buf[128];
                sprintf(buf, "nonfriend_%d.png", i+1);
                m_pNonFriendTexture[i]->CreateFromFile(buf);
            }
                        
            m_pRiserTextures[0] = new System::TextureResource();
            m_pRiserTextures[0]->CreateFromFile("Art/plus_1.tga");
            
            m_pRiserTextures[1] = new System::TextureResource();
            m_pRiserTextures[1]->CreateFromFile("Art/plus_2.tga");
            
            m_pRiserTextures[2] = new System::TextureResource();
            m_pRiserTextures[2]->CreateFromFile("Art/plus_3.tga");
                        
            m_confettiEffect = ConfettiEffect::Create( 256 );
			m_confettiEffect->SetMaxParticlesPerFrame( 256 );
			m_confettiEffect->SetMinPosition( Math::vec3( 0.0f, 0.0f, -900.0f ) );
			m_confettiEffect->SetMaxPosition( Math::vec3( 0.0f, 0.0f, -900.0f ) );
			m_confettiEffect->SetMinStartVelocity( Math::vec3( -400.0f, -400.0f, -80.0f ) );
			m_confettiEffect->SetMaxStartVelocity( Math::vec3( 400.0f,  400.0f,  -80.0f ) );
			m_confettiEffect->SetFaceCamera( false );
            m_confettiEffect->SetMinStartScale( 4.0f );
            m_confettiEffect->SetMaxStartScale( 12.0f );
            m_confettiEffect->SetMinDamping( 0.95f );
            m_confettiEffect->SetMaxDamping( 0.99f );
            m_confettiEffect->SetMinRotationDelta(  1.0f );
            m_confettiEffect->SetMaxRotationDelta( 10.0f );
            m_confettiEffect->SetGravity( Math::vec3( 0.0f, 0.f, 40.0f ) );
            
            static Math::vec3 colours[] =
            {
                Math::vec3( 1.0f, 0.2f,  0.0f ),
                Math::vec3( 1.0f, 0.5f,  0.0f ),
                Math::vec3( 1.0f, 0.7f,  0.0f ),
                Math::vec3( 1.0f, 0.3f,  0.0f ),
                Math::vec3( 0.0f, 0.0f,  0.0f ),
            };
            
            unsigned int num_colours = sizeof(colours) / sizeof(colours[0]);
            
            m_confettiEffect->SetColours( colours, num_colours );
        }

        void GameController::OnExit()
        {
            m_pBackgroundSprite->SetDraw(false);
            delete m_pBackgroundSprite;
            m_pBackgroundSprite = NULL;
            
            delete m_pFriendImageSprite;
            m_pFriendImageSprite = NULL;
            
            delete m_pLoadingSprite;
            m_pLoadingSprite = NULL;
            
            delete m_pLoadingSpinner;
            m_pLoadingSpinner = NULL;
            
            delete m_pFriendTexture;
            m_pFriendTexture = NULL;
            
            delete m_pCoinTexture;
            m_pCoinTexture = NULL;
            
            delete m_confettiEffect;
            m_confettiEffect = NULL;
            
            delete m_pDropTheBombSprite;
            m_pDropTheBombSprite = NULL;
            
            for (int i=0; i<3; ++i) {
                delete m_pRiserTextures[i];
            }
            
            for (int i=0; i<10; ++i) {
                delete m_pNonFriendTexture[i];
            }
            
            for (u32 i=0; i<3; ++i) {
                delete m_pHeartSprite[i];
                m_pHeartSprite[i] = NULL;
                delete m_pBombSprite[i];
                m_pBombSprite[i] = NULL;
            }
            
            std::vector<EntityInstance*>::iterator itr = m_vecEntities.begin();
            std::vector<EntityInstance*>::iterator end = m_vecEntities.end();
            for (; itr != end; ++itr)
            {
                EntityInstance* pCurrentEntity = *itr;
                delete pCurrentEntity->pSprite;
                delete pCurrentEntity;
            }
            
            m_vecEntities.clear();
            
            m_uLivesRemaining = 3;
            
            m_labelScore.hidden = YES;
            m_labelScore = nil;
            
            m_labelFriendName.hidden = YES;
            m_labelFriendName = nil;
        }
		
        void GameController::OnRender()
        {
            
            static float fov		= 45.0f;
            static float fAspect	= 0.67f;
            static float fNearClip  = 1.0f;
            static float fFarClip	= 1000.0f;
            
            Math::mtx44 viewMat;
            Math::mtx44 projMat;
            
            mtx44MakeIdentity(viewMat);
            
            mtx44MakePerspectiveRH( projMat, fov, fAspect, fNearClip, fFarClip );
            
			m_confettiEffect->Render(viewMat, projMat);
            
        }
        
        void GameController::OnUpdate()
        {   
            if (m_kGameState == kGAMESTATE_LOADING)
            {
                m_pLoadingSpinner->SetRotation(m_pLoadingSpinner->GetRotation() + 0.25f);
                
                if (m_pFriendTexture && m_pFriendTexture->GetIsReady())
                {
                    m_pLoadingSprite->SetDraw(false);
                    m_pLoadingSpinner->SetDraw(false);
                    
                    m_uCurrentScore = 0;
                    m_uLivesRemaining = 3;
                    
                    for (u32 i=0; i<m_uLivesRemaining; ++i) {
                        m_pHeartSprite[i]->SetDraw(true);
                    }
                    
                    for (u32 i=0; i<m_uBombsRemaining; ++i) {
                        m_pBombSprite[i]->SetDraw(true);
                    }
                    
                    if (m_uBombsRemaining > 0) {
                        m_pDropTheBombSprite->SetDraw(true);
                    }
                    
                    m_labelScore.text = [NSString stringWithFormat:@"Score: %u", m_uCurrentScore];
                    m_labelScore.hidden = NO;
                    
                    m_labelFriendName.hidden = NO;

                    m_kLossType = kUNDEFINED;
                    m_pCelebLossEntity = NULL;
                    
                    if (m_bSocial) {
                        m_kGameState = kGAMESTATE_PLAYING;
                    }
                    else {
                        m_kGameState = kGAMESTATE_PLAYING_NOSOCIAL;
                    }

                    return;
                }
   
            }
            else if (m_kGameState == kGAMESTATE_PLAYING || m_kGameState == kGAMESTATE_PLAYING_NOSOCIAL)
            {
                m_fSpawnTimer -= 1.0f / 30.0f;
                
                if (m_fSpawnTimer < 0.f) 
                {
                    SpawnEntity();
                    m_fSpawnTimer = 0.7;
                }
                               

                
                std::vector<EntityInstance*>::iterator itr = m_vecEntities.begin();
                std::vector<EntityInstance*>::iterator end = m_vecEntities.end();
                for (; itr != end; ++itr)
                {
                    EntityInstance* pCurrentEntity = *itr;
                    pCurrentEntity->pSprite->SetPosition(pCurrentEntity->pSprite->GetPosition() + pCurrentEntity->vVelocity);
                    pCurrentEntity->vVelocity.y += 1.f;
                    pCurrentEntity->pSprite->SetRotation(pCurrentEntity->pSprite->GetRotation() + pCurrentEntity->fRotationVelocity);
                    
                    if (pCurrentEntity->pSprite->GetPosition().y > 1000.f && pCurrentEntity->bFriend) {
                        m_vecEntities.erase(itr);
                        pCurrentEntity->pSprite->SetDraw(false);
                        delete pCurrentEntity->pSprite;
                        delete pCurrentEntity;
                        m_uLivesRemaining--;
                        m_pHeartSprite[m_uLivesRemaining]->SetDraw(false);
                        m_kLossType = kLOSSTYPE_RANOUTOFLIVES;

                        itr = m_vecEntities.begin();
                        end = m_vecEntities.end();
                        continue;
                    }
                }
                
                m_confettiEffect->Update();
                
                std::vector<System::Sprite*>::iterator riseritr = m_vecRisers.begin();
                std::vector<System::Sprite*>::iterator riserend = m_vecRisers.end();
                for (; riseritr != riserend; ++riseritr)
                {
                    System::Sprite* pCurrentRiser = *riseritr;
                    pCurrentRiser->SetPosition(Math::vec2(pCurrentRiser->GetPosition().x, pCurrentRiser->GetPosition().y - 2.f));
                    pCurrentRiser->SetAlpha(pCurrentRiser->GetAlpha() - 0.025f);
                    
                    if (pCurrentRiser->GetAlpha() <= 0.f) {
                        pCurrentRiser->SetDraw(false);
                        m_vecRisers.erase(riseritr);
                        delete pCurrentRiser;
                        return;
                    }
                }
                
                // Game over
                if (m_uLivesRemaining == 0) {
                    
                    std::vector<System::Sprite*>::iterator riseritr = m_vecRisers.begin();
                    std::vector<System::Sprite*>::iterator riserend = m_vecRisers.end();
                    for (; riseritr != riserend; ++riseritr)
                    {
                        System::Sprite* pCurrentRiser = *riseritr;
                        pCurrentRiser->SetDraw(false);
                        delete pCurrentRiser;
                    }
                    m_vecRisers.clear();
                    
                    m_confettiEffect->Reset();
                    
                    m_kGameState = m_kGameState == kGAMESTATE_PLAYING ? kGAMESTATE_PLAYING_GAMEOVER : kGAMESTATE_PLAYING_GAMEOVER_NOSOCIAL;
                }
                            
            }
            else if (m_kGameState == kGAMESTATE_PLAYING_GAMEOVER || m_kGameState == kGAMESTATE_PLAYING_GAMEOVER_NOSOCIAL)
            {
                if (m_kLossType == kLOSSTYPE_HITCELEB && m_pCelebLossEntity)
                {
                    float fDistanceToUpright = fmodf(m_pCelebLossEntity->pSprite->GetRotation(),6.28318531f);
                    
                    m_pCelebLossEntity->pSprite->SetRotation(m_pCelebLossEntity->pSprite->GetRotation() + (fDistanceToUpright * 0.05f));

                    m_pCelebLossEntity->pSprite->SetScale(m_pCelebLossEntity->pSprite->GetScale() * Math::vec2(1.1f, 1.1f));
                    
                    if (m_pCelebLossEntity->pSprite->GetScale().x > 30.f)
                    {
                        EndGame();
                        return;
                    }
                }
                else {
                    EndGame();
                    return;
                }
                
            }
        }
        
        
        void GameController::OnTapDown(int nIndex, Math::vec2 vPosition)
        {
            int nFrameScore = 0;
            
            std::vector<EntityInstance*>::iterator itr = m_vecEntities.begin();
            std::vector<EntityInstance*>::iterator end = m_vecEntities.end();
            
            for (; itr != end; ++itr)
            {
                // to prevent race condition
                if (m_vecEntities.size() == 0)
                    break;
                
                EntityInstance* pCurrentEntity = *itr;
                
                if (pCurrentEntity->pSprite->IsPointInside(vPosition, 25.f))
                {
                    if (!pCurrentEntity->bFriend && !pCurrentEntity->bCoin)
                    {
                        m_pCelebLossEntity = pCurrentEntity;
                        m_pCelebLossEntity->pSprite->SetLayer(m_nEntitiesSpawned + 100);
                        m_kLossType = kLOSSTYPE_HITCELEB;
                        m_uLivesRemaining = 0;
                        break;
                    }
                    else if (pCurrentEntity->bCoin)
                    {
                        m_uCoins++;
                     
                        m_vecEntities.erase(std::remove(m_vecEntities.begin(), m_vecEntities.end(), pCurrentEntity), m_vecEntities.end());
                        pCurrentEntity->pSprite->SetDraw(false);
                        delete pCurrentEntity->pSprite;
                        delete pCurrentEntity;
                
                        
                        itr = m_vecEntities.begin();
                        end = m_vecEntities.end();
                        continue;
                    }
                    else
                    {
                        nFrameScore++;
                        
                        m_vecEntities.erase(std::remove(m_vecEntities.begin(), m_vecEntities.end(), pCurrentEntity), m_vecEntities.end());
                        
                        pCurrentEntity->pSprite->SetDraw(false);
                        if (pCurrentEntity != NULL && pCurrentEntity->pSprite != NULL)
                            delete pCurrentEntity->pSprite;
                        
                        if (pCurrentEntity != NULL)
                            delete pCurrentEntity;
                        
                        // If they start to score well, spawn extras
                        if ( !(m_uCurrentScore % 10) )
                        {
                            for (int i=0; i<(m_uCurrentScore/20); ++i) {
                                SpawnEntity();
                            }
                        }
                        
                        itr = m_vecEntities.begin();
                        end = m_vecEntities.end();
                        continue;
                    }
                }
            }
            
            if (nFrameScore > 0)
            {
                System::Sprite* pRiser = new System::Sprite(m_pRiserTextures[nFrameScore < 3 ? (nFrameScore-1) : 2]);
                
                 pRiser->SetPivot(Math::vec2(pRiser->GetWidth() * 0.5f, pRiser->GetHeight() * 0.5f));
                 pRiser->SetPosition(vPosition);
                 pRiser->SetLayer(1000);
                 m_vecRisers.push_back(pRiser);
                 
                 if (nFrameScore > 1) {
                     m_confettiEffect->SetMaxPosition(Math::vec3(vPosition.x - 320.f, (vPosition.y - 480.f) * -1.f, -900.f));
                     m_confettiEffect->SetMinPosition(Math::vec3(vPosition.x - 320.f, (vPosition.y - 480.f) * -1.f, -900.f));
                     m_confettiEffect->SpawnParticles(80);
                 }
                
                 if (nFrameScore >= 3) {
                     m_bShouldSendAchievement[GameController::kACHIEVEMENT_SCOREx3] = true;
                 }
                
                u32 uOldScore = m_uCurrentScore;
                m_uCurrentScore += (nFrameScore * nFrameScore);
                
                if (uOldScore < 50 && m_uCurrentScore >= 50) {
                    m_bShouldSendAchievement[GameController::kACHIEVEMENT_SCORE50] = true;
                }
                else if (uOldScore < 100 && m_uCurrentScore >= 100) {
                    m_bShouldSendAchievement[GameController::kACHIEVEMENT_SCORE100] = true;
                }
                else if (uOldScore < 150 && m_uCurrentScore >= 150) {
                    m_bShouldSendAchievement[GameController::kACHIEVEMENT_SCORE150] = true;
                }
                else if (uOldScore < 200 && m_uCurrentScore >= 200) {
                    m_bShouldSendAchievement[GameController::kACHIEVEMENT_SCORE200] = true;
                }
                
                m_labelScore.text = [NSString stringWithFormat:@"Score: %u", m_uCurrentScore];
            }
        }
        
        void GameController::OnTapUp(int nIndex, Math::vec2 vPosition)
        {
            if (m_pDropTheBombSprite->GetDraw() && m_pDropTheBombSprite->IsPointInside(vPosition, 25.f)) {
                DropTheBomb();
            }
        }
        
        void GameController::DropTheBomb()
        {
            if (m_uBombsRemaining <= 0) {
                return;
            }
            
            m_uBombsRemaining--;
            
            for (u32 i=0; i<3; ++i) {
                m_pBombSprite[i]->SetDraw(false);
            }
            for (u32 i=0; i<m_uBombsRemaining; ++i) {
                m_pBombSprite[i]->SetDraw(true);
            }
            
            if (m_uBombsRemaining == 0) {
                m_pDropTheBombSprite->SetDraw(false);
            }
            
            std::vector<EntityInstance*>::iterator itr = m_vecEntities.begin();
            std::vector<EntityInstance*>::iterator end = m_vecEntities.end();
            for (; itr != end; )
            {
                EntityInstance* pCurrentEntity = *itr;
                
                if (pCurrentEntity->bFriend) {
                    m_uCurrentScore++;
                }
                if (pCurrentEntity->bCoin) {
                    m_uCoins++;
                }
                
                m_vecEntities.erase(itr);
                pCurrentEntity->pSprite->SetDraw(false);
                delete pCurrentEntity->pSprite;
                delete pCurrentEntity;
                
                itr = m_vecEntities.begin();
                end = m_vecEntities.end();
            }
            
            m_vecEntities.clear();
            
            m_labelScore.text = [NSString stringWithFormat:@"Score: %u", m_uCurrentScore];
        }
        
        
        void GameController::StartGame(bool bSocial, u32 uInitialBombs, bool bChallenge, NSString *challengeFriendName,  NSString *challengeFriendId)
        {
            m_bSocial = bSocial;
            m_uCoins = 0;
            m_pLoadingSprite->SetDraw(true);
            m_pLoadingSpinner->SetDraw(true);
            
            for (u32 i=0; i<GameController::kACHIEVEMENT_MAX; ++i) {
                m_bShouldSendAchievement[i] = false;
            }
            
            m_uBombsRemaining = uInitialBombs;
            
            if (bSocial)
            {
                //m_pUserImageSprite->SetDraw(false);
                
                if (bChallenge) {
                  
                    m_labelFriendName.text = [NSString stringWithFormat:@"Smash %@!", [[challengeFriendName componentsSeparatedByString:@" "] objectAtIndex:0]];
                    
                    if (m_pFriendTexture) { 
                        delete m_pFriendTexture;
                    }
                    
                    m_uFriendFBID = [challengeFriendId longLongValue];
                    
                    m_pFriendTexture = new System::TextureResource();
                    m_pFriendTexture->CreateFromFBID(m_uFriendFBID, 128, 128);
                    
                    m_kGameState = kGAMESTATE_LOADING;
                }
                else {
                    [FBGraph fetchFriendDetailsWithCallback:^(NSArray *friends) {
                        didFetchFriendInfo(friends);
                    }];
                    m_kGameState = kGAMESTATE_LOADING;
                }
            }
            else
            {
                SelectNonSocial();
            }
            
        }
        
        
        
        void GameController::DidFetchFBFriendInfo(NSArray* friends)
        {
            if (friends == nil || friends.count == 0)
            {
                SelectNonSocial();
            }
            else
            {
                NSDictionary *friendData = [friends objectAtIndex: arc4random() % friends.count];
                
                NSString *friendId = [friendData objectForKey:@"id"];
                m_friendName = [[[friendData objectForKey:@"name"] componentsSeparatedByString:@" "] objectAtIndex:0];
                
                m_labelFriendName.text = [NSString stringWithFormat:@"Smash %@!", m_friendName];
                
                if (m_pFriendTexture) {
                    delete m_pFriendTexture;
                }
                
                m_uFriendFBID = [friendId longLongValue];
                
                m_pFriendTexture = new System::TextureResource();
                m_pFriendTexture->CreateFromFBID(m_uFriendFBID, 128, 128);
            }
            
        }
        
        void GameController::SelectNonSocial()
        {
            if (m_pFriendTexture) {
                delete m_pFriendTexture;
            }
            
            m_nNoSocialFriendCeleb = Random::GetRandom(1, 10);
            
            char buf[128];
            sprintf(buf, "nonfriend_%d.png", m_nNoSocialFriendCeleb);
            
            m_pFriendTexture = new System::TextureResource();
            m_pFriendTexture->CreateFromFile(buf);
            
            
            NSArray *celebArray =  [NSArray arrayWithObjects:
                                    [NSString stringWithFormat:@"Einstein"],
                                    [NSString stringWithFormat:@"Xzibit"],
                                    [NSString stringWithFormat:@"Fry"],
                                    [NSString stringWithFormat:@"Tina"],
                                    [NSString stringWithFormat:@"Jennifer"],
                                    [NSString stringWithFormat:@"Elizabeth"],
                                    [NSString stringWithFormat:@"Rick"],
                                    [NSString stringWithFormat:@"Keanu"],
                                    [NSString stringWithFormat:@"Arnie"],
                                    [NSString stringWithFormat:@"Jean-Luc"],
                                    nil];
            
            m_labelFriendName.text = [NSString stringWithFormat:@"Smash %@ !", [celebArray objectAtIndex:m_nNoSocialFriendCeleb-1]];
            m_friendName =[celebArray objectAtIndex:m_nNoSocialFriendCeleb-1];
            
            m_kGameState = kGAMESTATE_LOADING;
        }
        
        
        void GameController::EndGame()
        {
            if ([FBLogin isLoggedIn]) {
                if (m_bShouldSendAchievement[GameController::kACHIEVEMENT_SCOREx3]) {
                    [FBShare sendAchievementWithAchievement:kACHIEVEMENT_SCOREx3];
                }
                if (m_bShouldSendAchievement[GameController::kACHIEVEMENT_SCORE150]) {
                    [FBShare sendAchievementWithAchievement:kACHIEVEMENT_SCORE150];
                }
                if (m_bShouldSendAchievement[GameController::kACHIEVEMENT_SCORE100]) {
                    [FBShare sendAchievementWithAchievement:kACHIEVEMENT_SCORE100];
                }
                if (m_bShouldSendAchievement[GameController::kACHIEVEMENT_SCORE50]) {
                    [FBShare sendAchievementWithAchievement:kACHIEVEMENT_SCORE50];
                }
            }
            m_fsc->EndGame(m_friendName, m_uFriendFBID, m_uCurrentScore, m_uCoins, m_uBombsRemaining);
        }
        
        
        void GameController::SpawnEntity()
        {
            EntityInstance* pEntity = new EntityInstance;
            
            float fEntityType = Random::GetRandom(0.f, 100.f);

            
            if (fEntityType < 70.f) {
                pEntity->pSprite = new System::Sprite(m_pFriendTexture);
                pEntity->bFriend = true;
                pEntity->bCoin = false;

            }
            else if (fEntityType < 80) {
                pEntity->pSprite = new System::Sprite(m_pCoinTexture);
                pEntity->pSprite->SetPivot(Math::vec2(52.f, 46.f));
                pEntity->bFriend = false;
                pEntity->bCoin = true;
            }
            else {
                
                int nCelebToSpawn = Random::GetRandom(0, 9);
                
                if (m_kGameState == kGAMESTATE_PLAYING_NOSOCIAL) 
                {
                    while (nCelebToSpawn == (m_nNoSocialFriendCeleb-1))
                    {
                        nCelebToSpawn = Random::GetRandom(0, 9);    
                    }
                }
                
                pEntity->pSprite = new System::Sprite(m_pNonFriendTexture[nCelebToSpawn]);
                pEntity->bFriend = false;
                pEntity->bCoin = false;
            }
            
            
            pEntity->pSprite->SetLayer(10 + m_nEntitiesSpawned);
            pEntity->pSprite->SetPivot(Math::vec2(pEntity->pSprite->GetWidth()*0.5f, pEntity->pSprite->GetHeight()*0.5f));
            pEntity->pSprite->SetPosition(Math::vec2(Random::GetRandom(-200.f, 840.f), 1000.f));
            
            pEntity->fRotationVelocity = Random::GetRandom(-0.5f, 0.5f);

            float fDistanceToMiddle = Random::GetRandom(200.f, 440.f) - pEntity->pSprite->GetPosition().x;
            
            pEntity->vVelocity = Math::vec2(fDistanceToMiddle * Random::GetRandom(0.02f, 0.03f)*System::Graphics::viewPortRatio, Random::GetRandom(-42.f, -34.f));
            
            m_vecEntities.push_back(pEntity);
    m_nEntitiesSpawned++;
        }
    
    }
}