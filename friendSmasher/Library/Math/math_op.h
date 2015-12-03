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

#ifndef FRIENDSMASHER_MATH_OP
#define FRIENDSMASHER_MATH_OP

#include <math.h>
#include "math_def.h"

namespace FriendSmasher
{
namespace Math
{

	inline bool approxf(const float& f1, const float& f2, const float& epsilon) 
	{
		return (f1 > (f2-epsilon) && f1 < (f2+epsilon));
	}

	inline float DegreesToRadians(float degrees){return degrees * kDegreesToRadians;}
	inline float RadiansToDegrees(float radians){return radians * kRadiansToDegrees;}

	inline float SinFromCos(float cos)
	{
		float sin=1.0f-cos*cos;
		if (sin <= 0.0f)
			return 0.0f;
		else
			return sqrtf(sin);
	}

	inline float CosFromSin(float sin)
	{
		return SinFromCos(sin);
	}


	inline unsigned int NextPow2( unsigned int x ) {
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		return ++x;
	}
    
    inline unsigned int PrevPow2(unsigned int n)
    {
        if (!n) return n; 
        
        int x = 1;
        while(x < n)
        {
            x <<= 1;
        }
        return x >>= 1;
    }
	
	inline unsigned int nLog2(unsigned int x)
	{
		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);
		x >>= 1;
		x -= (x >> 1) & 0x55555555;
		x = ((x >> 2) & 0x33333333) + (x & 0x33333333);
		x = ((x >> 4) + x) & 0x0f0f0f0f;
		x += (x >> 8);
		x += (x >> 16);
		return x & 63;
	}
	
	inline float frac( float x )
	{
		return x - floorf( x );
	}

	template<typename T> 
	inline T Min(const T& x, const T& y) { return (x < y) ? x : y; }

	template<typename T> 
	inline T Max(const T& x, const T& y) { return (x > y) ? x : y; }

	// Optimised version of Sign for float.
	// Tests show that it's about twice as fast as the non-specialised Sign function.
	// Sign returns -1 if the value is negative, +1 otherwise (including if the value is zero)
	template<typename T>
	inline T Sign(T k) { return k < (T)0 ? (T)-1 : (T)1; }
	
	inline float Sign(float k)
	{
		reinterpret_cast<unsigned&>(k) =
			(reinterpret_cast<unsigned&>(k) & 0x80000000) | 0x3f800000;
		return k;
	}
	
	template<typename T>
	inline T Sqr(T k)
	{
		return k * k;
	}

	inline float Saturate(float v)
	{
		v = (v > 1.f) ? 1.f : v;
		v = (v < 0.f) ? 0.f : v;	
		return v;
	}
	
	inline void Saturate(float* v)
	{
		*v = (*v > 1.f) ? 1.f : *v;
		*v = (*v < 0.f) ? 0.f : *v;	
	}

	template<class T>
	inline T Clamp(T value, T min, T max)
	{
		if( min > max ) 
		{
			T temp = min;
			min = max;
			max = temp;
		}
		
		if (value < min)
			return min;
		else if (max < value)
			return max;
		else
			return value;
	}
	
	template<class T>
	inline T Normalize(T value, T min, T max)
	{		
		if (value < min)
			return min;
		else if (max < value)
			return max;
		else {
			float fRange = max-min;
			float fDelta = value - min;
			return fDelta/fRange;
			
		}
		
	}
    
    inline float Round(float t)
    {
        return (float)((int)(t + 0.5f));
    }

	inline int RoundToNearest(int nInput, int nNearest)
	{
		if(nNearest>nInput || nNearest <= 0)
			return nInput;
		
		float x = float(nInput%nNearest);
		
		return ( x < (nNearest/2) ) ? (int)(nInput - x) : (int)(nInput + (nNearest-x));
	}
	
	// MIT HAKMEM Count
	inline int BitCount(unsigned int u) 
	{
		/* works for 32-bit numbers only    */
		
		unsigned int uCount;
		
		uCount = u - 
				 ((u >> 1) & 033333333333) - 
				 ((u >> 2) & 011111111111);
		
		return ((uCount + (uCount >> 3)) & 030707070707) % 63;
	}


	inline int HighestBitSet(unsigned int u)
	{
		// Count the number of leading zeros
#if defined(_MSC_VER)
		unsigned int x = u;

		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);

		return(31 - BitCount(x));
#elif defined (__GNUC__)
		unsigned int v = 31 - __builtin_clz(u);

		// Return the bit number
		return v;
#endif
	}


} // Math
} // namespace FriendSmasher

#endif