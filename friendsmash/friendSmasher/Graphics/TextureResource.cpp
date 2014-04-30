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

#include "TextureResource.h"
#include "debug.h"
#include <OpenGLES/EAGL.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "math_op.h"


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
            char buf[512];
            sprintf(buf, "https://graph.facebook.com/%llu/picture?width=256&height=256", uID);
            std::string url = buf;
            
            return CreateFromURL(url, uWidth, uHeight);
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
