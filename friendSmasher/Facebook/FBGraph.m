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

#import "FBGraph.h"

@implementation FBGraph

typedef NS_ENUM(NSInteger, eGameAchievements) {
    kACHIEVEMENT_SCORE50 = 0,
    kACHIEVEMENT_SCORE100,
    kACHIEVEMENT_SCORE150,
    kACHIEVEMENT_SCORE200,
    kACHIEVEMENT_SCOREx3,
    kACHIEVEMENT_MAX
};

static FBUserDetails *meDetails;

// Fetch /me Graph API and cache to FBUserDetails object
// /me is equivalent to /{user-id} and see this doc: https://developers.facebook.com/docs/graph-api/reference/user
+(void)fetchMeDetailsWithCallback: (void (^)(FBUserDetails *))callback
{
    FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"me" parameters:@{@"fields": @"first_name,id"}];
    [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
        if (!error && result) {
            FBUserDetails *userDetails = [[FBUserDetails alloc] init];
            userDetails.fbid = [[result objectForKey:@"id"] longLongValue];
            userDetails.first_name = [result objectForKey:@"first_name"];
            
            // After fetch me data, it will be cached
            if (!meDetails) {
                meDetails = [[FBUserDetails alloc] init];
            }
            meDetails.fbid = userDetails.fbid;
            meDetails.first_name = userDetails.first_name;

            callback(userDetails);
        }
        else {
            callback(nil);
        }
    }];
}


// Fetch friends list from Graph API. The list includes only friends who logged into this app before.
// user_friends permission is reuiqred. See this doc: https://developers.facebook.com/docs/graph-api/reference/v2.5/user/friends
+(void)fetchFriendDetailsWithCallback: (void (^)(NSArray *))callback
{    
    FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"me/friends" parameters:@{@"fields":@""}];
    [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
        if (!error && result)
        {
            NSArray* fetchedFriendData = [[NSArray alloc] initWithArray:[result objectForKey:@"data"]];
            callback(fetchedFriendData);
        }
        else
        {
            callback(nil);
        }
    }];
}

// The invitable_friends API is only available for games that have a Facebook Canvas app implementation.
// This API is only available on version 2.0 (or later) of the Graph API.
// Please read this doc: https://developers.facebook.com/docs/games/invitable-friends/v2.4
+(void)fetchInvitableFriendsWithCallback: (void (^)(NSArray*))callback
{
    
    FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"me/invitable_friends" parameters:@{@"fields":@""}];
    [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
        if (!error && result)
        {
            NSArray* fetchedFriendData = [[NSArray alloc] initWithArray:[result objectForKey:@"data"]];
            callback(fetchedFriendData);
        }
        else
        {
            callback(nil);
        }
    }];
}

// The Graph API for scores lets game developers build social leaderboards and game-matching by storing players' scores as they play.
// These scores will appearin the Games Feed on Facebook.
// https://developers.facebook.com/docs/games/scores
+(void)fetchScoresWithCallback: (void (^)(NSArray *))callback {
    FBSDKGraphRequest *req = [[FBSDKGraphRequest alloc] initWithGraphPath:[NSString stringWithFormat:@"%@/scores", [FBSDKSettings appID]] parameters:@{@"fields":@"score,user"}];
    [req startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
        if (!error && result)
        {
            NSArray* fetchedScoreData = [[NSArray alloc] initWithArray:[result objectForKey:@"data"]];
            if ([fetchedScoreData count] > 0){
                callback(fetchedScoreData);
                return;
            }
        }
        callback(nil);
    }];
}


// get my first name from cached info
+(NSString *)meFirstName {
    if (meDetails){
        return meDetails.first_name;
    }
    return nil;
}

// get my facebook id from cached info
+(long long)meFbid {
    if (meDetails){
        return meDetails.fbid;
    }
    return 0;
}

// get profile picture url using my facebook id
+(NSString *)meProfilePictureURL: (long long)fbid
{
    return [NSString stringWithFormat:@"https://graph.facebook.com/%llu/picture?width=256&height=256", fbid];
}

@end
