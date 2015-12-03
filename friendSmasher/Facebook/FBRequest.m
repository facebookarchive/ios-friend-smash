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

#import "FBRequest.h"
#import "FBGraph.h"
#import "SBJsonWriter.h"
#import "SendRequestDelegate.h"

@implementation FBRequest

id<FBSDKGameRequestDialogDelegate> gameRequestDelegate;

// Game Requests give players a mechanism for inviting their friends to play a game.
// Requests are sent by a player to one or more specific friends, and always carry a call-to-action that the sender wants the recipient to complete.
// Recipients can choose to accept the request, or they can choose to ignore or decline it.
// https://developers.facebook.com/docs/games/requests/v2.4

+(void)sendRequestWithFriendIDs: (NSArray *)friendIDs score:(int)score {
    SBJsonWriter *jsonWriter = [SBJsonWriter new];
    NSDictionary *challenge =  [NSDictionary dictionaryWithObjectsAndKeys:
                                [NSString stringWithFormat:@"%d", score], @"challenge_score",
                                @"", @"fields",
                                nil];
    NSString *challengeStr = [jsonWriter stringWithObject:challenge];
    
    
    FBSDKGameRequestContent *content = [[FBSDKGameRequestContent alloc] init];
    [content setMessage:[NSString stringWithFormat:@"I just smashed %d friends! Can you beat it?", score]];
    [content setTitle:@"Smashing!"];
    [content setRecipients:friendIDs];
    [content setData:challengeStr];
    
    if(!gameRequestDelegate){
        gameRequestDelegate = [[SendRequestDelegate alloc] init];
    }
    
    [FBSDKGameRequestDialog showWithContent:content delegate:gameRequestDelegate];
}

// Send Game Requests to Friends.
// Using Invitiable Friends API, you can create custom friends selector.
// Then, send Game Requests to selected invitable friends
// Note that the invitable_friends API is only available for games that have a Facebook Desktop Game implementation
// https://developers.facebook.com/docs/games/invitable-friends

+(void)sendInviteWithFriendIDs: (NSArray *)friendIDs {
    
    FBSDKGameRequestContent *content = [[FBSDKGameRequestContent alloc] init];
    [content setMessage:[NSString stringWithFormat:@"Come join me in the friend smash times!"]];
    [content setTitle:@"Smashing Invite!"];
    
    // friendIDS are user IDs, usernames or invite tokens (NSString) of people to send request
    [content setRecipients:friendIDs];
    
    [FBSDKGameRequestDialog showWithContent:content delegate:gameRequestDelegate];
}

@end
