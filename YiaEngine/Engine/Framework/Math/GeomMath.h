#pragma once
//#include"Include/simple.h"
#include<initializer_list>
#include<cstring>
#include"swizzle.h"
#include"AddByElement.h"


//void simpleTest(const float* in,float* out,int count)
//{
//	ispc::simpleAdd(in, out, count);
//}


namespace YiaEngine
{
	template<typename T, int N> struct vec
	{
	
	};
#pragma region MathOperator
	template<typename U, int M>
	vec< U, M> VecAdd(const vec<U, M>& lhs, const vec<U, M>& rhs)
	{
		vec<U, M> ret;

		ret.x = lhs.x + rhs.x;
		ispc::AddByElement(lhs, rhs, ret, M);
		return ret;
	}

	template<typename T, int N> struct vecbase
	{
		friend vec<T, N> operator + (const vec<T, N>& lhs, const vec<T, N>& rhs)
		{
			return VecAdd(lhs, rhs);
		}
	};
#pragma endregion

	

	template<typename T>
	struct vec<T,2>:vecbase<T,2>
	{
		union {
			T data[2];
			struct { T x, y; };
			struct { T r, g; };
			SWIZZLE2(T, x, y);
			SWIZZLE2(T, r, g);
		};
		vec() { memset(data, 0, sizeof(data)); };
		vec(const T& v) :x(v), y(v) {}
		vec(const vec& v) :x(v.x), y(v.y) {}
		vec(const T& a, const T& b) : x(a), y(b) {}
		vec(std::initializer_list<T> list) {
			int i = 0;
			for (const T* it = list.begin(); it != list.end(); ++it, ++i)
				data[i] = *it;
		}

		operator T* () { return data; }
		operator const T* ()const { return const_cast<const T*>(data); }
	};

	template<typename T>
	struct vec<T, 3> :vecbase<T, 3>
	{
		union {
			T data[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
	
			SWIZZLE3(T, x, y, z);
			SWIZZLE3(T, r, g, b);
		};
		vec() { memset(data, 0, sizeof(data)); };
		vec(const T& v) :x(v), y(v),z(v) {}
		vec(const vec& v) :x(v.x), y(v.y),z(v.z) {}
		vec(const T& a, const T& b, const T& c) : x(a), y(b),z(c) {}
		vec(std::initializer_list<T> list) {
			int i = 0;
			for (const T* it = list.begin(); it != list.end(); ++it, ++i)
				data[i] = *it;
		}

		operator T* () { return data; }
		operator const T* ()const { return const_cast<const T*>(data); }
	};

	template<typename T>
	struct vec<T, 4> :vecbase<T, 4>
	{
		union {
			T data[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
	
			SWIZZLE4(T, x, y, z,w);
			SWIZZLE4(T, r, g, b,a);
		};
		vec() { memset(data, 0, sizeof(data)); };
		vec(const T& v) :x(v), y(v), z(v),w(v) {}
		vec(const vec& v) :x(v.x), y(v.y), z(v.z),w(v.w) {}
		vec(const T& a, const T& b, const T& c, const T& d) : x(a), y(b), z(c),w(d) {}
		vec(std::initializer_list<T> list) {
			int i = 0;
			for (const T* it = list.begin(); it != list.end(); ++it, ++i)
				data[i] = *it;
		}

		operator T* () { return data; }
		operator const T* ()const { return const_cast<const T*>(data); }
	};
	
	template<typename T, int N,int M> struct mat
	{
		vec<T, M> data[N];
	};

	using vec2f = vec<float, 2>;
	using vec3f = vec<float, 3>;
	using vec4f = vec<float, 4>;
	using R8G8B8A8U = vec<uint32_t, 4>;
	using R8G8B8U = vec<uint32_t, 3>;
}
