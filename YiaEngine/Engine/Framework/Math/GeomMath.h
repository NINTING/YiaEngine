#pragma once
#ifndef FRAMEWORK_MATH_GEOMATH_H_
#define FRAMEWORK_MATH_GEOMATH_H_



//#include"Include/simple.h"
#include<string.h>

#include<initializer_list>

#include "operation/AddByElement.h"
#include "swizzle.h"




//void simpleTest(const float* in,float* out,int count)
//{
//	ispc::simpleAdd(in, out, count);
//}


namespace YiaEngine
{
	template<typename T, int N> struct Vec
	{
	
	};
#pragma region MathOperator
	template<typename U, int M>
	Vec< U, M> VecAdd(const Vec<U, M>& lhs, const Vec<U, M>& rhs)
	{
		Vec<U, M> ret;

		ret.x = lhs.x + rhs.x;
		ispc::AddByElement(lhs, rhs, ret, M);
		return ret;
	}

	template<typename T, int N> struct vecbase
	{
		friend Vec<T, N> operator + (const Vec<T, N>& lhs, const Vec<T, N>& rhs)
		{
			return VecAdd(lhs, rhs);
		}

	};
#pragma endregion

	

	template<typename T>
	struct Vec<T,2>:vecbase<T,2>
	{
		union {
			T data[2];
			struct { T x, y; };
			struct { T r, g; };
			SWIZZLE2(T, x, y);
			SWIZZLE2(T, r, g);
		};
		Vec() {  };
		Vec(const T& v) :x(v), y(v) {}
		Vec(const Vec& v) :x(v.x), y(v.y) {}
		Vec(const T& a, const T& b) : x(a), y(b) {}
		Vec(std::initializer_list<T> list) {
			int i = 0;
			for (const T* it = list.begin(); it != list.end(); ++it, ++i)
				data[i] = *it;
		}

		operator T* () { return data; }
		operator const T* ()const { return const_cast<const T*>(data); }
	};

	template<typename T>
	struct Vec<T, 3> :vecbase<T, 3>
	{
		union {
			T data[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
	
			SWIZZLE3(T, x, y, z);
			SWIZZLE3(T, r, g, b);
		};
		Vec() { };
		Vec(const T& v) :x(v), y(v),z(v) {}
		Vec(const Vec& v) :x(v.x), y(v.y),z(v.z) {}
		Vec(const T& a, const T& b, const T& c) : x(a), y(b),z(c) {}
		Vec(std::initializer_list<T> list) {
			int i = 0;
			for (const T* it = list.begin(); it != list.end(); ++it, ++i)
				data[i] = *it;
		}

		operator T* () { return data; }
		operator const T* ()const { return const_cast<const T*>(data); }
	};

	template<typename T>
	struct Vec<T, 4> :vecbase<T, 4>
	{
		union {
			T data[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
	
			SWIZZLE4(T, x, y, z,w);
			SWIZZLE4(T, r, g, b,a);
		};
		Vec() {  };
		Vec(const T& v) :x(v), y(v), z(v),w(v) {}
		Vec(const Vec& v) :x(v.x), y(v.y), z(v.z),w(v.w) {}
		Vec(const T& a, const T& b, const T& c, const T& d) : x(a), y(b), z(c),w(d) {}
		Vec(std::initializer_list<T> list) {
			int i = 0;
			for (const T* it = list.begin(); it != list.end(); ++it, ++i)
				data[i] = *it;
		}
		T& operator [](int idx) { return data[idx]; }
		const T& operator [](int idx)const { return data[idx]; }
		operator T* () { return data; }
		operator const T* ()const { return const_cast<const T*>(data); }


		static const Vec Zero() { return Vec(0); }
		static const Vec One() { return Vec(1); }
	};
	
	template<typename T, int N,int M> struct mat
	{
		Vec<T, M> data[N];
	};

	using Vec2f = Vec<float, 2>;
	using Vec3f = Vec<float, 3>;
	using Vec4f = Vec<float, 4>;
	using Color = Vec<float, 4>;
	using R8G8B8A8U = Vec<uint8_t, 4>;
	using R8G8B8U = Vec<uint8_t, 3>;

	//––æÿ’Û
	//row matrix
	template<typename T, int R, int C>
	struct Matrix
	{
		Vec<T, C>data[R];

		Matrix() { memset(data, 0, sizeof(T) * C * R); }
		Matrix(const Matrix& mat) = default;
		Matrix& operator = (const Matrix& mat) = default;
		
		Matrix(std::initializer_list<const T>list){
			int i = 0;
			for (const T* it = list.begin(); it != list.end(); ++it, ++i)
				data[i] = *it;
		}
		Vec<T, C>& operator[](int col) {
			return data[col];
		}
		const Vec<T, C>& operator[](int col)const {
			return data[col];
		}
		/*void Translation<T, 4, 4>(T x, T y, T z) {
			data[3][0] += x;
			data[3][1] += y;
			data[3][2] += z;
		}*/
		static Matrix Identity()
		{
			
			assert(R == C);
			
			Matrix ret;
			for (unsigned i = 0; i < R; i++)
			{
				ret[i][i] = 1;	
			}
		}
		
		
	};
	using Mat4x4f = Matrix<float, 4, 4>;

	//namespace gemo_math
	//{
	//	Translation
	//}

}//YiaEngine



#endif // !FRAMEWORK_MATH_GEOMATH_H_