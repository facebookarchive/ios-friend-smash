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

#ifndef FRIENDSMASHER_PARSECONTROLLER
#define FRIENDSMASHER_PARSECONTROLLER

#import "ViewController.h"
#import "AppDelegate.h"
#include "math_lib.h"


namespace FriendSmasher
{
    namespace Game
    {
        class ParseController
        {
            
        public:
            
            ParseController();
            virtual ~ParseController();
            
            static void Login(void (*callback)(bool));
            static void GetProperty(NSString* property, void (*callback)(NSString*, u32));
            static void SetProperty(NSString* property, u32 uValue, void (*callback)(NSString*, bool));
            
        private:
            
            
        };
    }
}



#endif
