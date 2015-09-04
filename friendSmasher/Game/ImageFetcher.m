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

#import "ImageFetcher.h"

@implementation ImageFetcher

@synthesize delegate;


- (id) init{

    if (self = [super init]){
    
        queue = [[NSOperationQueue alloc] init];
        queue.maxConcurrentOperationCount = 5;
        
    }
    
    return self;
}

- (void) reportSuccess:(UIImage*)image{
    

    [self.delegate imageFetcherDidFetch:self fetchedImage:image];
}

- (void) reportError:(NSString*)url{
    [self.delegate imageFetcherDidFailToFetch:self urlUsed:url];
}

- (void) fetchImageWithUrl:(NSString *)url andCompletionBlock:(bool (^)(UIImage*))block {
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:url]];
    
    NSHTTPURLResponse *response  =nil;
    NSError *error = nil;
    
    NSData *imageData = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    
    if (error || response.statusCode != 200 || !imageData || imageData.length == 0){
        
        [self performSelectorOnMainThread:@selector(reportError:) withObject:url waitUntilDone:NO];
    }else{
        
        UIImage *image = [[UIImage alloc]initWithData:imageData];
        block(image);
    }
}


- (void) fetchImageSyncWithString:(NSString *)url{
    
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:url]];
    
    NSHTTPURLResponse *response  =nil;
    NSError *error = nil;
    
    NSData *imageData = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    
    if (error || response.statusCode != 200 || !imageData || imageData.length == 0){
        
        [self performSelectorOnMainThread:@selector(reportError:) withObject:url waitUntilDone:NO];
    }
    else{
        UIImage *image = [[UIImage alloc]initWithData:imageData];
        [self performSelectorOnMainThread:@selector(reportSuccess:) withObject:image waitUntilDone:NO];
    }
    
}

- (void) fetchImageWithUrl:(NSString *)url{
    
    NSInvocationOperation *op = [[NSInvocationOperation alloc]initWithTarget:self selector:@selector(fetchImageSyncWithString:) object:url];
    [queue addOperation:op];
}


@end
