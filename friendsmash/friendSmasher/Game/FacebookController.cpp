    //
//  FacebookController.cpp
//  friendSmasher
//
//  Created by Ali Parr on 23/10/2013.
//
//

#include "FacebookController.h"

#include <Social/Social.h>
#include <Social/SLComposeViewController.h>
#include "SBJson.h"

namespace FriendSmasher
{
    namespace Game
    {
        
        static const u64 kuFBAppID = 480369938658210;          // Replace with your own APP ID
        
        NSString* FacebookController::ms_nsstrFirstName = NULL;
        u64 FacebookController::ms_uPlayerFBID(0);
        bool FacebookController::ms_bIsLoggedIn = false;
        
        
        //Frictionless Recipients Cache
        //FBFrictionlessRecipientCache* FacebookController::ms_friendCache = NULL;   // Uncomment
        
        void FacebookController::CreateNewSession()
        {
            // Code to create a new Facebook Session
        }
        
        bool FacebookController::IsLoggedIn()
        {
            //Check if the user is logged into Facebook
            return ms_bIsLoggedIn;
        }
        
        void FacebookController::SetLoggedIn(bool bLoggedIn)
        {
            ms_bIsLoggedIn = bLoggedIn;
        }
        
        void FacebookController::OpenSession(void (*callback)(bool))
        {
            //Open the Facebook session
        }
        
        void FacebookController::Login(void (*callback)(bool))
        {
           //Log into Facebook
        }
        
        void FacebookController::Logout(void (*callback)(bool))
        {
            //Log out of Facebook
        }
        
        void FacebookController::CheckForPermission(NSString* permission, void (*callback)(bool))
        {
            //Check the user's current permission
        }
        
        
        void FacebookController::FetchUserDetails(void (*callback)(bool))
        {
            //Get the current Facebook user's details
        }
        
        void FacebookController::FetchFriendDetails(void (*callback)(NSArray*))
        {
            //Get the Facebook friend details
        }
        
        
        void FacebookController::FetchInvitableFriendDetails(void (*callback)(NSArray*))
        {
            //Get the user's friends who haven't played the game
        }
        
        
        void FacebookController::ProcessIncomingURL(NSURL* targetURL, void (*callback)(NSString *, NSString *))
        {
            // Process the incoming url and see if it's of value...
            
            NSRange range = [targetURL.query rangeOfString:@"notif" options:NSCaseInsensitiveSearch];
            
            // If the url's query contains 'notif', we know it's coming from a notification - let's process it
            if(targetURL.query && range.location != NSNotFound)
            {
                // Yes the incoming URL was a notification
                ProcessIncomingRequest(targetURL, callback);
            }
            
            range = [targetURL.path rangeOfString:@"challenge_brag" options:NSCaseInsensitiveSearch];
            
            // If the url's path contains 'challenge_brag', we know it comes from a feed post
            if(targetURL.path && range.location != NSNotFound)
            {
                // Yes the incoming URL was a notification
                ProcessIncomingFeed(targetURL, callback);
            }
        }
        
        void FacebookController::ProcessIncomingRequest(NSURL* targetURL, void (*callback)(NSString *, NSString *))
        {
            // Extract the notification id
           
        }
        
        void FacebookController::ProcessIncomingFeed(NSURL* targetURL, void (*callback)(NSString *, NSString *))
        {
            // Here we process an incoming link that has launched the app via a feed post
            
        }
        
        void FacebookController::SendInvite(NSArray* friendIDs)
        {
            
            //Send an invite to a list of friends
        }
        
        
        void FacebookController::SendRequest(NSArray* friendIDs, const int nScore)
        {
           //Send game requests to a list of friends, sending the user's score
        }
        
        void FacebookController::SendFilteredRequest(const int nScore)
        {
            // Okay, we're going to filter our friends by their device, we're looking for friends with an iPhone or iPad
           
        }
        
        void FacebookController::SendBrag(const int nScore)
        {
            // This function will invoke the Feed Dialog to post to a user's Timeline and News Feed
            // It will attemnt to use the Facebook Native Share dialog
            // If that's not supported we'll fall back to the web based dialog.
           
        }
        
        void FacebookController::SendScore(const int nScore)
        {
            //Post to the Score API
            
        }
        
        void FacebookController::SendAchievement(eGameAchievements achievement)
        {
            //Post to the Achievements API
        }
        
        void FacebookController::GetScores(void (*callback)(NSArray*))
        {
            //Get the current user scores via the Scores API
        }
        
        
        void FacebookController::RequestWritePermissions()
        {
            //Upsell write permissions
        }
        
        void FacebookController::ReRequestFriendPermission()
        {
            //Upsell friend permissions
        }
    }
}

