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

#include "random.h"
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "math_op.h"

namespace FriendSmasher
{
    namespace Random
    {
            
        static float rcp = 1.0f / static_cast<float>(RAND_MAX);

        void Seed()
        {
            Seed(time(NULL));	
        }
            
        void Seed(u32 uSeed)
        {
            srand(uSeed);
        }
            
        int GetRandom()
        {
            return GetRandom(INT_MAX);
        }

        int GetRandom(int iMax)
        {
            iMax = Math::Max(iMax, 2);
            float fRand = (float)rand() * rcp;
            return static_cast<int>(fRand * static_cast<float>(iMax));
        }

        int GetRandom(int iMin, int iMax)
        {
            float t = static_cast<float>(rand()) * rcp;
            return static_cast<int>( 0.5f + static_cast<float>(iMin) + t * static_cast<float>(iMax - iMin) );
        }

        float GetRandom(float fMax)
        {
            float fRand = (float)rand() * rcp;
            return fRand * fMax;
        }

        float GetRandom(float fMin, float fMax)
        {
            float fRand = (float)rand() * rcp;
            float fRange = fMax - fMin;
            return fRand * fRange + fMin;
        }

        bool GetRandomBool()
        {
            return (rand()%2);
        }
        
    }
}