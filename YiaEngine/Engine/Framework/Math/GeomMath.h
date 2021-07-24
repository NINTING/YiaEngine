#ifndef FRAMEWORK_MATH_GEOMATH_H_
#define FRAMEWORK_MATH_GEOMATH_H_



//#include"Include/simple.h"
#include<string.h>
#include<assert.h>
#include<math.h>

#include<initializer_list>

#include "operation/AddByElement.h"
#include "operation/MatrixOperation.h"
#include "operation/SubtractByElement.h"
#include "operation/Dot.h"
#include "operation/Cross.h"
#include"operation/VectorFunc.h"
#include "swizzle.h"




//void simpleTest(const float* in,float* out,int count)
//{
//	ispc::simpleAdd(in, out, count);
//}


namespace YiaEngine
{
	template<typename T, int N> struct Vec;

#pragma region MathOperator
	template<typename U, int M>
	static Vec< U, M> VecAdd(const Vec<U, M>& lhs, const Vec<U, M>& rhs);
	
	template<typename U, int M>
	static Vec< U, M> VecSubtract(const Vec<U, M>& lhs, const Vec<U, M>& rhs);

	template<typename U, int M>
	static Vec< U, M> VecMul(const Vec<U, M>& lhs, const Vec<U, M>& rhs);


	template<typename T, int N> struct vecbase
	{
		friend Vec<T, N> operator + (const Vec<T, N>& lhs, const Vec<T, N>& rhs)
		{
			return VecAdd(lhs, rhs);
		}
		friend Vec<T, N> operator - (const Vec<T, N>& lhs, const Vec<T, N>& rhs)
		{
			return VecSubtract(lhs, rhs);
		}
		friend Vec<T, N> operator * (const Vec<T, N>& lhs, const Vec<T, N>& rhs)
		{
			return VecMul(lhs, rhs);
		}
		Vec<T, N>& operator += (const Vec<T, N>& rhs)
		{
			auto* pTmp = reinterpret_cast<Vec<T, N>*>(this);
			*pTmp = VecAdd(*pTmp, rhs);
			return *pTmp;
		}
		Vec<T, N>& operator *= (const Vec<T, N>& rhs)
		{
			auto* pTmp = reinterpret_cast<Vec<T, N>*>(this);
			*pTmp = VecAdd(*pTmp, rhs);
			return *pTmp;
		}
		bool IsZero()
		{
			float* pn = reinterpret_cast<float*>(this);
			for (int i = 0; i < N; i++)
			{
				if(fabsf(*(pn+i))>1e-10)
					return false;
			}
			return true;
		}
	/*	Vec<T, N>& operator =(const Vec<T, N>& rhs)
		{
			if (this != &rhs)
			{
				memcpy(this, &rhs, sizeof(T) * N);
			}
			return *this;
		}*/

	};
#pragma endregion

	template<typename T, int N> struct Vec :vecbase<T, N>
	{
		T data[N];
		Vec() = default;
		Vec(std::initializer_list<T> list) {
			int i = 0;
			for (const T* it = list.begin(); it != list.end(); ++it, ++i)
				data[i] = *it;
		}
		T& operator [](int idx) { return data[idx]; }
		const T& operator [](int idx)const { return data[idx]; }
		operator T* () { return data; }
		operator const T* ()const { return const_cast<const T*>(data); }
	};

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
		T& operator [](int idx) { return data[idx]; }
		const T& operator [](int idx)const { return data[idx]; }
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
		Vec() :x(0), y(0), z(0) { };
		Vec(const T& v) :x(v), y(v),z(v) {}
		Vec(const Vec& v) :x(v.x), y(v.y),z(v.z) {}
		Vec(const T& a, const T& b, const T& c) : x(a), y(b),z(c) {}
		Vec& operator = (const Vec& lhs) { 
			data[0] = lhs.x;
			data[1] = lhs.y;
			data[2] = lhs.z;
			return *this;
		}
		Vec(std::initializer_list<T> list) {
			int i = 0;
			for (const T* it = list.begin(); it != list.end(); ++it, ++i)
				data[i] = *it;
		}
		T& operator [](int idx) { return data[idx]; }
		const T& operator [](int idx)const { return data[idx]; }
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
		template<int U>
		Vec(const Vec<T, U>& in) :x(0), y(0), z(0), w(0)
		{
			int count = U > 4 ? 4 : U;
			memcpy(this, in, sizeof(T) * count);
		}
		T& operator [](int idx) { return data[idx]; }
		const T& operator [](int idx)const { return data[idx]; }
		operator T* () { return data; }
		operator const T* ()const { return const_cast<const T*>(data); }

		template<int U>
		operator Vec<T, U>()
		{
			Vec<T, U> out{ 0 };
			int count = U > 4 ? 4 : U;
			memcpy(out, this, sizeof(T) * count);
			return out;
		}
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
		union 
		{
			Vec<T, C>vec_data[R];
			T datas[R*C];
		};
		

		Matrix() { memset(datas, 0, sizeof(T) * C * R); }
		Matrix(const Matrix& mat)
		{
			memcpy(datas, mat.datas, sizeof(T) * C * R);
		}
		Matrix& operator = (const Matrix& mat) = default;
		
		Matrix(const std::initializer_list<T>&list){
			int i = 0;
			for (auto it = list.begin(); it != list.end(); ++it, ++i)
				datas[i] = *it;
		}
		Vec<T, C>& operator[](int col) {
			return vec_data[col];
		}
		const Vec<T, C>& operator[](int col)const {
			return vec_data[col];
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
			return ret;
		}
		operator T*()
		{
			return datas;
		}
		operator const T* () const
		{
			return const_cast<const T*>(datas);
		}
	public:
		
		friend	Matrix<float, R, R> operator * (const Matrix<float, R, R>& lhs, const Matrix<float, R, R>& rhs)
		{
			Matrix<float, R, R> ret;
			ispc::Multipy(lhs, rhs, R, ret);
			return ret;
		}
	};
	using Mat4x4f = Matrix<float, 4, 4>;
	using Mat3x3f = Matrix<float, 3, 3>;
	

	template<typename U, int M>
	static Vec< U, M> VecAdd(const Vec<U, M>& lhs, const Vec<U, M>& rhs)
	{
		Vec<U, M> ret;

		//	ret.x = lhs.x + rhs.x;
		ispc::AddByElement(lhs, rhs, ret, M);
		return ret;
	}
	template<typename U, int M>
	static Vec< U, M> VecSubtract(const Vec<U, M>& lhs, const Vec<U, M>& rhs)
	{
		Vec<U, M> ret;

		//	ret.x = lhs.x + rhs.x;
		ispc::SubtractByElement(lhs, rhs, ret, M);
		return ret;
	}
	template<typename U, int M>
	static Vec< U, M> VecMul(const Vec<U, M>& lhs, const Vec<U, M>& rhs)
	{
		Vec<U, M> ret;

		//	ret.x = lhs.x + rhs.x;
		ispc::MultiByElement(lhs, rhs, ret, M);
		return ret;
	}
	template< int M>
	static Vec< float, M> Normalize(const Vec<float, M>& lhs)
	{
		Vec<float, M> ret;
		//	ret.x = lhs.x + rhs.x;
		ispc::Normalize(lhs, ret, M);
		return ret;
	}
	template<int M>
	Vec<float, M>  Transform(const Vec< float, M>& lhs, const Matrix < float , M, M > &rhs)
	{
		
		Vec<float, M> out_vec;
		ispc::Transform(lhs, rhs, M, out_vec);
		return out_vec;
	}
	template<int M>
	Vec< float, M> operator* (const Vec< float, M>& lhs, const Matrix < float, M, M >& rhs)
	{
		Vec<float,M> out_vec;
		ispc::Transform(lhs, rhs, M, out_vec);
		return out_vec;
	}
	template<int M>
	Vec3f Cross(const Vec< float, M>& lhs, const const Vec< float, M>& rhs)
	{
		Vec3f ret;
		ispc::Cross(lhs, rhs, ret, M);
		return ret;
	}
	template<int M>
	float Dot(const Vec< float, M>& lhs, const Vec< float, M>& rhs)
	{
		float ret = 0;
		ispc::Dot(lhs, rhs, &ret, M);
		return ret;
	}
}

//YiaEngine



#endif // !FRAMEWORK_MATH_GEOMATH_H_