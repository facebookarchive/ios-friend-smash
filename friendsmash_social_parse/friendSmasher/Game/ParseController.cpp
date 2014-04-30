//
//  FacebookController.cpp
//  friendSmasher
//
//  Created by Ali Parr on 23/10/2013.
//
//

#include "ParseController.h"
#include "FacebookController.h"

namespace FriendSmasher
{
    namespace Game
    {
        void ParseController::Login(void (*callback)(bool))
        {
            [PFFacebookUtils initializeFacebook];
            
            NSArray *permissions = [[NSArray alloc] initWithObjects:
                                    @"email", @"public_profile", @"user_friends",
                                    nil];
            
            [PFFacebookUtils logInWithPermissions:permissions block:^(PFUser *user, NSError *error) {
             
                if (!user) {
                    NSLog(@"Uh oh. The user cancelled the Facebook login.");
                    [[FBSession activeSession] closeAndClearTokenInformation];
                    [FBSession setActiveSession:nil];
                    FacebookController::CreateNewSession();
                    FacebookController::ms_bIsLoggedIn = false;
                } else {
                    NSLog(@"User logged in.");
                    FacebookController::ms_bIsLoggedIn  = true;
                    callback(true);
                }
             }];
        }
        
        void ParseController::GetProperty(NSString* property, void (*callback)(NSString*, u32))
        {
            PFUser *currentUser = [PFUser currentUser];
            u32 uValue = [[currentUser objectForKey:property] intValue];
            callback(property, uValue);
        }
        
        void ParseController::SetProperty(NSString* property, u32 uValue, void (*callback)(NSString*, bool))
        {
            NSNumber *val = [NSNumber numberWithInt:uValue];
            
            [[PFUser currentUser] setObject:val forKey:property];
            [[PFUser currentUser] saveInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
             callback(property, succeeded);
             }];
        }
    }
}

