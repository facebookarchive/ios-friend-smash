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

#ifndef FRIENDSMASHER_VEC2
#define FRIENDSMASHER_VEC2

namespace FriendSmasher
{
namespace Math
{
	class vec2
	{
	public:
		vec2(void) : x(0.0f), y(0.0f) {} ;
		vec2(const float broadcast) : x(broadcast), y(broadcast) {};
		vec2(const float xin, const float yin) : x(xin), y(yin) {} ;

		~vec2(void) {};

		// Vector operators
		inline vec2 operator +  (const vec2 &rhs) const;
		inline vec2 operator -  (const vec2 &rhs) const;

		inline vec2 operator -  () const;

		inline vec2 operator *  (const vec2 &rhs) const;
		inline vec2 operator *  (const float &rhs) const;
		
		inline vec2 operator /  (const float &rhs) const;

		inline vec2 operator += (const vec2 &rhs);
		inline vec2 operator -= (const vec2 &rhs);

		inline bool operator == (const vec2 &rhs) const;
		inline bool operator != (const vec2 &rhs) const;

		inline vec2 operator *= (const float& rhs);
		inline vec2 operator *= (const vec2 &rhs);

		inline float operator[] (const int i) const;
		
		inline bool  operator < (const vec2 &rhs) const;
		inline bool	 operator <=(const vec2 &rhs) const;
		
		inline bool  operator > (const vec2 &rhs) const;
		inline bool  operator >=(const vec2 &rhs) const;

		static vec2 allzero;
		static vec2 allone;
		static vec2 min;
		static vec2 max;
		
		float					Length() const;
		float					Normalise();		// Returns the original length before the normalise operation
		void					Normalise(const float fLength); // If you've already worked out the length, you can pass it in here to avoid calculating it again.

		union
		{
			struct  
			{
				float x, y;
			};
			float f[2];
		};
	};
	
	vec2 vec2::operator + (const vec2 &rhs) const
	{
		return vec2(this->x + rhs.x, this->y + rhs.y);
	}
	
	vec2 vec2::operator - (const vec2 &rhs) const
	{
		return vec2(this->x - rhs.x, this->y - rhs.y);
	}
	
	vec2 vec2::operator - () const 
	{
		return vec2(-this->x, -this->y);
	}
	
	vec2 vec2::operator * (const vec2 &rhs) const
	{
		return vec2(this->x * rhs.x, this->y * rhs.y);
	}
	
	vec2 vec2::operator += (const vec2 &rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
	
	vec2 vec2::operator -= (const vec2 &rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}
	
	bool vec2::operator == (const vec2 &rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y );
	}
	
	
	bool vec2::operator != (const vec2 &rhs) const
	{
		return !(this->x == rhs.x && this->y == rhs.y );
	}
	
	vec2 vec2::operator *  (const float& rhs) const
	{
		return vec2( this->x * rhs, this->y * rhs );
	}
	
	vec2 vec2::operator /  (const float& rhs) const
	{
		float rcp = 1.0f / rhs;
		return vec2( this->x * rcp, this->y * rcp );
	}
	
	vec2 vec2::operator *= (const float &rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}

	vec2 vec2::operator *= (const vec2 &rhs)
	{
		this->x *= rhs.x;
		this->y *= rhs.y;
		return *this;
	}
	
	float vec2::operator [](const int i) const
	{
		return this->f[i];
	}
	
	bool vec2::operator< (const vec2 &rhs) const
	{
		return (x < rhs.x &&
				y < rhs.y );
	}
	
	bool vec2::operator<=(const vec2 &rhs) const
	{
		return (x <= rhs.x &&
				y <= rhs.y );
	}
	
	bool vec2::operator> (const vec2& rhs) const
	{
		return (x > rhs.x &&
				y > rhs.y );
	}
	
	bool vec2::operator>=(const vec2& rhs) const
	{
		return (x >= rhs.x &&
				y >= rhs.y );
	}
	
}
}

#endif // namespace FriendSmasher_VEC2