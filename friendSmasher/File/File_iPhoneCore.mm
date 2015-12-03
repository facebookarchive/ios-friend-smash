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

#import "File_iPhoneCore.h"
#import <stdio.h>
#import <UIKit/UIKit.h>
#import  "debug.h"

FILE* LoadFile_iPhone(const char* filename, const char* search, const char* ext)
{
	NSString* NSSearch = [[NSString alloc] initWithCString:search encoding:NSASCIIStringEncoding];
	NSString* NSFilename = [[NSString alloc] initWithCString:filename encoding:NSASCIIStringEncoding];
	NSString* NSExt = [[NSString alloc] initWithCString:ext encoding:NSASCIIStringEncoding];

	NSString * path = [[NSBundle mainBundle] pathForResource:NSFilename ofType:NSExt inDirectory:NSSearch];
	return fopen([path cStringUsingEncoding:1],"rb");
}

void GetFilePath(char* result, const char* filename)
{
	int nDotPosition = 0;
	int nFinalSlashPosition = -1;
	int i=0;
	const char* c = filename;
	while (*c != '\0') {
		if (*c == '.') { nDotPosition = i; }
		if (*c == '\\' || *c == '/') { nFinalSlashPosition = i; }
		i++;
		c++;
	}
	
	FBAssert(nDotPosition > nFinalSlashPosition);
	FBAssert(nDotPosition > 0);
	FBAssert(nFinalSlashPosition > -1);
	
	int nFilenameTotalLength = strlen(filename);
	
	// Create the path name
	char* Filename_Search = (char*)alloca(nFinalSlashPosition);
	strncpy(Filename_Search, filename, nFinalSlashPosition);
	Filename_Search[nFinalSlashPosition] = '\0';
	
	// And the file's name
	char* Filename_Stripped = (char*)alloca(nDotPosition-nFinalSlashPosition);
	memcpy((void*)Filename_Stripped, (void*)(filename + nFinalSlashPosition+1), nDotPosition-nFinalSlashPosition-1);
	Filename_Stripped[nDotPosition-nFinalSlashPosition-1] = '\0';
	
	
	// And the extension
	char* Filename_Extension = (char*)alloca(nFilenameTotalLength-nDotPosition);
	memcpy((void*)Filename_Extension, (void*)(filename + nDotPosition+1), nFilenameTotalLength-nDotPosition);
	
	// Convert to NSString...
	NSString* NSSearch		= [[NSString alloc] initWithCString:Filename_Search encoding:NSASCIIStringEncoding];
	NSString* NSFilename	= [[NSString alloc] initWithCString:Filename_Stripped encoding:NSASCIIStringEncoding];
	NSString* NSExt			= [[NSString alloc] initWithCString:Filename_Extension encoding:NSASCIIStringEncoding];
	NSString* path			= [[NSBundle mainBundle] pathForResource:NSFilename ofType:NSExt inDirectory:NSSearch];
	
	// And back!
	const char* cpath		= [path UTF8String];
	strcpy(result, cpath);
	
}

void GetHomePath(char* result, const char* filename)
{
	// Get the Documents directory of the bundle
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	
	// Create the home path filename
	sprintf(result, "%s/%s", [documentsDirectory UTF8String], filename);
}
