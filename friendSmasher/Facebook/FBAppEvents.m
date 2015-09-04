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

#import "FBAppEvents.h"

@implementation FBAppEvents

// Send launch event to Facebook
// This will help you track your app install and retention
// See this doc: https://developers.facebook.com/docs/app-events
+(void)sendLaunchEvent {
    [FBSDKAppEvents activateApp];
}

// Send 'play' custom App event in every new game play
// Read more available App Events: https://developers.facebook.com/docs/app-events/best-practices
+(void)sendPlayEvent {
    [FBSDKAppEvents logEvent:@"Play"];
}

// Send Spent Credit App event in Coin spending to buy bombs
// ContentID and NumItems parameters are used to identify detailed spending
// Read more about parameters: https://developers.facebook.com/docs/app-events/best-practices#EventsParameters
+(void)sendSpentCreditEvent: (double)number {
    [FBSDKAppEvents logEvent:FBSDKAppEventNameSpentCredits
                  valueToSum:   number
                  parameters:@{ FBSDKAppEventParameterNameContentID   : @"Coin",
                                FBSDKAppEventParameterNameNumItems    : [NSNumber numberWithDouble:number]} ];

}

@end
