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

#ifndef FRIENDSMASHER_MATH_DEF
#define FRIENDSMASHER_MATH_DEF

namespace FriendSmasher
{
    namespace Math
    {
        const float kEPSILON = 1.0e-5f;
        const float kBIGEPSILON = 1.0e-2f;
        const float kPI = 3.141592654f;
        const float k2PI = 6.283185308f;
        const float kPIOVER2 = 1.570796327f;
        const float kPIOVER4 = 0.7853981635f;

        const float kDegreesToRadians = 0.017453292519943295769236907684886f; //kPi / 180.0f;
        const float kRadiansToDegrees = 57.295779513082320876798154814105f; //180.0f / kPi;
        const float k1BYPI = 0.318309886f;

        const float kFloatMax = 3.402823466e+38F;
        const float kFloatMin = 1.175494351e-38F;
    }
}

#endif