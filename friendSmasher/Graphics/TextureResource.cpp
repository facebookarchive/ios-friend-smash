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

#include "TextureResource.h"
#include "debug.h"
#include <OpenGLES/EAGL.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "math_op.h"
#include "FBGraph.h"


#import <UIKit/UIKit.h>
#import "ImageFetcher.h"

namespace FriendSmasher
{
	namespace System
	{
        TextureResource::TextureResource() :
        m_uWidth(0),
        m_uHeight(0),
        m_bReady(false)
        {

        }
        
        TextureResource::~TextureResource()
        {
            
        }
        
        bool TextureResource::CreateFromFile(const char* filename)
        {
            GLuint errorcode = glGetError();
            
            NSString* inFilename = [[NSString alloc] initWithCString:filename encoding:NSASCIIStringEncoding];
                                    
            glGenTextures(1, &m_uTextureID[0]);
            glBindTexture(GL_TEXTURE_2D, m_uTextureID[0]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBlendFunc(GL_ONE, GL_SRC_COLOR);
            
            FBAssert(glGetError() == GL_NO_ERROR);
            errorcode = glGetError();
            
            NSString *extension = [inFilename pathExtension];
            NSString *baseFilenameWithExtension = [inFilename lastPathComponent];
            NSString *baseFilename = [baseFilenameWithExtension substringToIndex:[baseFilenameWithExtension length] - [extension length] - 1];
            
            NSString *path = [[NSBundle mainBundle] pathForResource:baseFilename ofType:extension];
            NSData *texData = [[NSData alloc] initWithContentsOfFile:path];
            
            
            UIImage *image = [[UIImage alloc] initWithData:texData];
            if (image == nil)
                return false;
            
            m_uWidth = CGImageGetWidth(image.CGImage);
            m_uHeight = CGImageGetHeight(image.CGImage);
            CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
            void *imageData = malloc( m_uHeight * m_uWidth * 4 );
            CGContextRef context = CGBitmapContextCreate( imageData, m_uWidth, m_uHeight, 8, 4 * m_uWidth, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
            CGColorSpaceRelease( colorSpace );
            CGContextClearRect( context, CGRectMake( 0, 0, m_uWidth, m_uHeight ) );
            CGContextDrawImage( context, CGRectMake( 0, 0, m_uWidth, m_uHeight ), image.CGImage );
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uWidth, m_uHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            CGContextRelease(context);
            
            free(imageData);
            
            glEnable(GL_BLEND);

            m_bReady = true;
            
            return true;
        }
                
        bool TextureResource::CreateFromFBID(u64 uID, u32 uWidth, u32 uHeight)
        {           
            NSString *url = [FBGraph meProfilePictureURL:uID];
            return CreateFromURL(*new std::string([url UTF8String]), uWidth, uHeight);
        }
        
        bool TextureResource::CreateFromURL(std::string url, u32 uWidth, u32 uHeight)
        {
            NSString *resourceAddress = [NSString stringWithCString:url.c_str() encoding:[NSString defaultCStringEncoding]];
           
            ImageFetcher* fetcher = [[ImageFetcher alloc] init];
            [fetcher fetchImageWithUrl:resourceAddress andCompletionBlock:^(UIImage *image)
             {
             if (image == nil) {
             return false;
             }
             
             glGenTextures(1, &m_uTextureID[0]);
             glBindTexture(GL_TEXTURE_2D, m_uTextureID[0]);
             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
             glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
             glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
             glBlendFunc(GL_ONE, GL_SRC_COLOR);
             
             m_uWidth = uWidth;
             m_uHeight = uHeight;
             
             CGSize newSize;
             newSize.width = m_uWidth;
             newSize.height = m_uHeight;
             
             UIGraphicsBeginImageContextWithOptions(newSize, NO, 0.0);
             [image drawInRect:CGRectMake(0, 0, newSize.width, newSize.height)];
             UIImage *resizednewImage = UIGraphicsGetImageFromCurrentImageContext();
             UIGraphicsEndImageContext();
             
             CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
             void *imageData = malloc( m_uHeight * m_uWidth * 4 );
             CGContextRef context = CGBitmapContextCreate( imageData, m_uWidth, m_uHeight, 8, 4 * m_uWidth, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
             CGColorSpaceRelease( colorSpace );
             CGContextClearRect( context, CGRectMake( 0, 0, m_uWidth, m_uHeight ) );
             CGContextDrawImage( context, CGRectMake( 0, 0, m_uWidth, m_uHeight ), resizednewImage.CGImage );
             
             glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uWidth, m_uHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
             CGContextRelease(context);
             
             free(imageData);
             
             glEnable(GL_BLEND);
             
             m_bReady = true;
             
             return true;
             }];
            
            return true;
        }

        
        
        u32 TextureResource::GetWidth() const {
            return m_uWidth;
        }
        
        u32 TextureResource::GetHeight() const {
            return m_uHeight;
        }
        
	}
}
