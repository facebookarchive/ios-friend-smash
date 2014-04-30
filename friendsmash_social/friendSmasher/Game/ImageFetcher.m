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
