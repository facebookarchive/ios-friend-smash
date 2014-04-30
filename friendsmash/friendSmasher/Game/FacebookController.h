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

#ifndef FRIENDSMASHER_FACEBOOKCONTROLLER
#define FRIENDSMASHER_FACEBOOKCONTROLLER

#import "ViewController.h"
#import "AppDelegate.h"
#include "math_lib.h"

#import <Accounts/Accounts.h>

namespace FriendSmasher
{
    namespace Game
    {
        class FacebookController
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
            
            
            FacebookController();
            virtual ~FacebookController();
            
            static void CreateNewSession();

            static void Login(void (*callback)(bool));
            static void OpenSession(void (*callback)(bool));
            static void Logout(void (*callback)(bool));
            
            static void CheckForPermission(NSString* permission, void (*callback)(bool));
            static bool IsLoggedIn();
            
            // Method for forcing this in the case of a login powered by parse
            static void SetLoggedIn(bool);
            
            static void FetchUserDetails(void (*callback)(bool));
            static void FetchFriendDetails(void (*callback)(NSArray*));
            static void FetchInvitableFriendDetails(void (*callback)(NSArray*));
            
            static void ProcessIncomingURL(NSURL* targetURL, void (*callback)(NSString *, NSString *));
            static void ProcessIncomingRequest(NSURL* targetURL, void (*callback)(NSString *, NSString *));
            static void ProcessIncomingFeed(NSURL* targetURL, void (*callback)(NSString *, NSString *));

            static void SendInvite(NSArray* friendIDs);
            static void SendRequest(NSArray* friendIDs, const int nScore);
            
            static void SendFilteredRequest(const int nScore);
            static void SendBrag(const int nScore);
            static void SendScore(const int nScore);
            static void SendAchievement(eGameAchievements achievement);
            static void GetScores(void (*callback)(NSArray*));

            static void RequestWritePermissions();
            static void ReRequestFriendPermission();
            
            static NSString* GetUserFirstName() { return ms_nsstrFirstName; }
            static u64 GetUserFBID() { return ms_uPlayerFBID; }
            
           
            
        private:
            
            static NSString* ms_nsstrFirstName;
            static u64 ms_uPlayerFBID;
            static bool ms_bIsLoggedIn;
            
            //static FBFrictionlessRecipientCache* ms_friendCache;
            
        };
    }
}



#endif
