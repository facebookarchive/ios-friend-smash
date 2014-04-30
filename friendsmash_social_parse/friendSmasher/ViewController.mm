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

#import "ViewController.h"
#import "AppDelegate.h"
#import "FriendSmashController.h"

static FriendSmasher::Game::FriendSmashController* pFSController = NULL;

@interface ViewController () {
    }
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation ViewController

@synthesize context = _context;


- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)handledURL: (NSNotification *)deepLinkNotification{
    if (pFSController) {
      NSURL *targetURL = deepLinkNotification.object;
      pFSController->ProcessIncomingURL(targetURL);
    }
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
 
    
    for (int i=0; i<5; ++i) {
        touchArray[i] = NULL;
    } 
    
    pFSController = new FriendSmasher::Game::FriendSmashController(self);
    pFSController->OnEnter();
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handledURL:) name:APP_HANDLED_URL object:nil];
}

- (BOOL)prefersStatusBarHidden
{
    return YES;
}



- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
        
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)update
{
    pFSController->OnUpdate();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{    
    pFSController->OnRender();
}



- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
    NSArray* arrayNewTouches = [touches allObjects];
    
	int nNewTouchCount = [arrayNewTouches count];
	
	for (int i=0; i<nNewTouchCount; ++i)
	{
		for (int j=0; j<5; ++j)
		{
			if ( touchArray[j] == NULL)
			{
				touchArray[j] = [arrayNewTouches objectAtIndex:i];
				
				CGPoint touchPoint = [[arrayNewTouches objectAtIndex:i] locationInView:self.view];
                
                // Normalize everything into retina space - so fixing at double the pixels here.
                pFSController->BeginTouch(j, touchPoint.x * 2.f, touchPoint.y * 2.f);
                
				break;
			}
		}
	}
}

- (void)touchesCancelled:(NSSet *)setEndTouches withEvent:(UIEvent *)event
{
	[self touchesEnded:setEndTouches withEvent:event];
}

- (void)touchesEnded:(NSSet *)setEndTouches withEvent:(UIEvent *)event
{
	NSArray* arrayEndTouches = [setEndTouches allObjects];
	
	int nEndTouchCount = [arrayEndTouches count];
	
	for (int i=0; i<nEndTouchCount; ++i)
	{
		CGPoint touchPoint = [[arrayEndTouches objectAtIndex:i] locationInView:self.view];
        
		for (int j=0; j<5; ++j)
		{
			if ( touchArray[j] == [arrayEndTouches objectAtIndex:i])
			{
				touchArray[j] = NULL;
				
                pFSController->EndTouch(j, touchPoint.x * 2.f, touchPoint.y * 2.f);
			
				break;
			}
		}
	}
}



@end
