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

#import "FBShare.h"
#import "FBGraph.h"
#import "SBJsonWriter.h"
#import "SendRequestDelegate.h"

@implementation FBShare

id<FBSDKSharingDelegate> shareDialogDelegate;

// The Graph API for scores lets game developers build social leaderboards and game-matching by storing players' scores as they play.
// These scores will appear on players' timelines and in the Games Feed on Facebook.
// https://developers.facebook.com/docs/games/scores
+(void)sendScore: (int)score {
    
    NSMutableDictionary* params =   [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                     [NSString stringWithFormat:@"%d", score], @"score",
                                     @"score", @"fields",
                                     nil];
    
    NSLog(@"Fetching current score");
    
    FBSDKGraphRequest *req = [[FBSDKGraphRequest alloc] initWithGraphPath:[NSString stringWithFormat:@"%llu/scores", [FBGraph meFbid]] parameters:params];
    [req startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
        if (!error && result)
        {
            if ([[result objectForKey:@"data"] count] > 0){
                int nCurrentScore = [[[[result objectForKey:@"data"] objectAtIndex:0] objectForKey:@"score"] intValue];
                
                NSLog(@"Current score is %d", nCurrentScore);
                
                if (score > nCurrentScore) {
                    
                    NSLog(@"Posting new score of %d", score);
                    FBSDKGraphRequest *req2 = [[FBSDKGraphRequest alloc]
                                               initWithGraphPath:[NSString stringWithFormat:@"%llu/scores", [FBGraph meFbid]]
                                               parameters:params
                                               HTTPMethod:@"POST"];
                    [req2 startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                        NSLog(@"Score Posted");
                    }];
                }
                else {
                    NSLog(@"Existing score is higher - not posting new score");
                }
            } else {
                NSLog(@"Posting new score of %d", score);
                FBSDKGraphRequest *req2 = [[FBSDKGraphRequest alloc]
                                           initWithGraphPath:[NSString stringWithFormat:@"%llu/scores", [FBGraph meFbid]]
                                           parameters:params
                                           HTTPMethod:@"POST"];
                [req2 startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                    NSLog(@"Score Posted");
                }];
            }
        }
    }];
}

// The Graph API for achievements enables you to publish achievements in your game
// so players can tell more meaningful and relevant stories.
// https://developers.facebook.com/docs/games/achievements
+(void)sendAchievementWithAchievement: (int)achievement {
    NSArray *achievementURLs = [NSArray arrayWithObjects:   @"http://www.friendsmash.com/opengraph/achievement_50.html",
                                @"http://www.friendsmash.com/opengraph/achievement_100.html",
                                @"http://www.friendsmash.com/opengraph/achievement_150.html",
                                @"http://www.friendsmash.com/opengraph/achievement_200.html",
                                @"http://www.friendsmash.com/opengraph/achievement_x3.html",
                                nil];
    
    NSMutableDictionary* params =   [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                     [NSString stringWithFormat:@"%@", [achievementURLs objectAtIndex:achievement]], @"achievement",
                                     nil];
    
    FBSDKGraphRequest *req = [[FBSDKGraphRequest alloc]
                              initWithGraphPath:[NSString stringWithFormat:@"%llu/achievements", [FBGraph meFbid]]
                              parameters:params
                              HTTPMethod:@"POST"];
    [req startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
        NSLog(@"Achievements Posted");
    }];
}

// App Invite is a personal way for people to invite their Facebook friends to a mobile app
// See this doc: https://developers.facebook.com/docs/app-invites
+(void)sendAppInviteFromViewController: (UIViewController *)viewController {
    FBSDKAppInviteContent *content =[[FBSDKAppInviteContent alloc] init];
    
    // this webpage should have app link metadata including Android or iOS URL
    content.appLinkURL = [NSURL URLWithString:@"https://www.friendsmash.com"];

    //optionally set previewImageURL
    content.appInvitePreviewImageURL = [NSURL URLWithString:@"http://www.friendsmash.com/images/logo_large.jpg"];
    
    // present the dialog.
    [FBSDKAppInviteDialog showFromViewController:viewController withContent:content delegate:nil];
}


// Sharing stories using the Share Dialog.
// The Share Dialog does not require a user to be logged in,
// which makes this particularly interesting for games that only provide a lightweight login flow or none at all.
// https://developers.facebook.com/docs/games/sharing#lightweightsharing
+(void)sendBragWithScore : (int)score {
    NSString *linkURL = [NSString stringWithFormat:@"https://www.friendsmash.com/challenge_brag_%llu", [FBGraph meFbid]];
    NSString *pictureURL = @"http://www.friendsmash.com/images/logo_large.jpg";
    FBSDKShareLinkContent *content = [[FBSDKShareLinkContent alloc] init];
    [content setContentURL:[NSURL URLWithString:linkURL]];
    [content setContentTitle:@"Checkout my Friend Smash greatness!"];
    [content setContentDescription:@"Come smash me back!"];
    [content setImageURL:[NSURL URLWithString:pictureURL]];
    
    [FBSDKShareDialog showFromViewController:nil withContent:content delegate:shareDialogDelegate];
}

// Sharing stories using a dialog in the iOS integrated Facebook Share Sheet
+(void)shareSheetFBWithViewController : (UIViewController *)viewController {
    
    NSString *linkURL = @"https://www.friendsmash.com";
    NSString *pictureURL = @"http://www.friendsmash.com/images/logo_large.jpg";
    FBSDKShareLinkContent *content = [[FBSDKShareLinkContent alloc] init];
    [content setContentURL:[NSURL URLWithString:linkURL]];
    [content setContentTitle:@"Checkout my Friend Smash greatness!"];
    [content setContentDescription:@"Come smash me back!"];
    [content setImageURL:[NSURL URLWithString:pictureURL]];

    FBSDKShareDialog *dialog = [[FBSDKShareDialog alloc] init];
    [dialog setMode:FBSDKShareDialogModeShareSheet];
    [dialog setShareContent:content];
    [dialog setFromViewController:viewController];
    [dialog setDelegate:nil];
    
    if ([dialog canShow]){
        [dialog show];
    } else {
        NSLog(@"Cannot show share dialog");
    }
}

// Sharing stories using a dialog in the iOS share sheet (via UIActivityViewController)
+(void)shareSheetOSWithViewController : (UIViewController *)viewController {
    NSString *linkURL = @"https://www.friendsmash.com";
    NSArray *activityItems = [NSArray arrayWithObjects:@"Checkout my Friend Smash greatness", linkURL, nil];
    UIActivityViewController *activityViewController = [[UIActivityViewController alloc] initWithActivityItems:activityItems applicationActivities:nil];
    activityViewController.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
    
    [viewController presentViewController:activityViewController animated:YES completion:nil];
}

@end
