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

#import "FBLogin.h"

@implementation FBLogin

static FBSDKLoginManager *loginManager;

// FBSDKLoginManager initiates the login flow and manage Access Token
+(void)initLoginManager {
    if (!loginManager) {
        loginManager = [[FBSDKLoginManager alloc] init];
    }
}

// check the status whether the person is logged or not.
+(BOOL)isLoggedIn {
    if (!loginManager) {
        loginManager = [[FBSDKLoginManager alloc] init];
    }
    
    // We represent each person logged into your app with the [FBSDKAccessToken currentAccessToken].
    return [FBSDKAccessToken currentAccessToken] != nil;
}

// Check the person has granted given permission from Access Token.
// Access token has information for App ID, User ID, and granted permissions.
+(BOOL)hasPermission: (NSString *)permission {
    if (!loginManager) {
        loginManager = [[FBSDKLoginManager alloc] init];
    }
    
    return [[FBSDKAccessToken currentAccessToken] hasGranted:permission];
}

// Initiate login flow
+(void)loginFromViewController:(UIViewController *)fromViewController callback: (void (^)(BOOL))callback {
    
    // If your app asks for more than public_profile, email and user_friends,
    // Facebook must review it before you release it.
    // Learn more about the review process and what's required to pass review.
    // https://developers.facebook.com/docs/facebook-login/review
    NSArray *permissions = [[NSArray alloc] initWithObjects:
                            @"email", @"public_profile", @"user_friends",
                            nil];
    
    if (!loginManager) {
        loginManager = [[FBSDKLoginManager alloc] init];
    }
    
    // The FBSDKLoginManager sets this token for you
    // and when it sets currentAccessToken it also automatically writes it to a keychain cache.
    [loginManager logInWithReadPermissions:permissions
                        fromViewController:fromViewController
                                   handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
                                       
                                       NSLog(@"Login End");
                                       
                                       if ([FBLogin isLoggedIn]) {
                                           callback(YES);
                                       } else if (result.isCancelled) {
                                           callback(NO);
                                       } else if (error) {
                                           callback(NO);
                                       }
                                   }];

}

// Logs out the person
+(void)logoutWithCallback: (void (^)(BOOL))callback {
    if (!loginManager) {
        loginManager = [[FBSDKLoginManager alloc] init];
    }
    
    // This calls [FBSDKAccessToken setCurrentAccessToken:nil] and [FBSDKProfile setCurrentProfile:nil]
    [loginManager logOut];
    callback(true);
}

// Publish Permissions - You must request these separately from read permissions.
// You should only request publish permissions after someone takes an action that requires it
// https://developers.facebook.com/docs/facebook-login/ios/permissions
//
+(void)requestWritePermissionFromViewController:(UIViewController *)fromViewController callback:(void (^)(BOOL))callback {
    NSArray *permissions = [[NSArray alloc] initWithObjects:
                            @"publish_actions", nil];
    
    if (!loginManager) {
        loginManager = [[FBSDKLoginManager alloc] init];
    }
    
    [loginManager logInWithPublishPermissions:permissions
                           fromViewController:fromViewController
                                      handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
                                          if ([FBLogin hasPermission:@"publish_actions"]){
                                              callback(YES);
                                          } else {
                                              callback(NO);
                                          }
                                      }];
}

// if the person does not grant user_friends permission, the app can rerequest it
+(void)requestFriendPermissionFromViewController:(UIViewController *)fromViewController callback:(void (^)(BOOL))callback {
    NSArray *permissions = [[NSArray alloc] initWithObjects:
                            @"user_friends", nil];
    if (!loginManager) {
        loginManager = [[FBSDKLoginManager alloc] init];
    }
    
    [loginManager logInWithReadPermissions:permissions
                        fromViewController:fromViewController
                                   handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
                                       if ([FBLogin hasPermission:@"user_friends"]) {
                                           callback(YES);
                                       } else {
                                           callback(NO);
                                       }
                                   }];
}

@end
