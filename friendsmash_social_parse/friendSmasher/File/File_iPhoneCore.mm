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
