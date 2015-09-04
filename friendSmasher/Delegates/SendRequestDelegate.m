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

#import "SendRequestDelegate.h"

@implementation SendRequestDelegate


/*!
 @abstract Sent to the delegate when the game request completes without error.
 @param gameRequestDialog The FBSDKGameRequestDialog that completed.
 @param results The results from the dialog.  This may be nil or empty.
 */
- (void)gameRequestDialog:(FBSDKGameRequestDialog *)gameRequestDialog didCompleteWithResults:(NSDictionary *)results {
    NSLog(@"%@", results);
}

/*!
 @abstract Sent to the delegate when the game request encounters an error.
 @param gameRequestDialog The FBSDKGameRequestDialog that completed.
 @param error The error.
 */
- (void)gameRequestDialog:(FBSDKGameRequestDialog *)gameRequestDialog didFailWithError:(NSError *)error {
    NSLog(@"%@", error);
}

/*!
 @abstract Sent to the delegate when the game request dialog is cancelled.
 @param gameRequestDialog The FBSDKGameRequestDialog that completed.
 */
- (void)gameRequestDialogDidCancel:(FBSDKGameRequestDialog *)gameRequestDialog {
    NSLog(@"a game request is canceled");
}

@end
