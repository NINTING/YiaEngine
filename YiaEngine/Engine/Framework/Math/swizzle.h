#pragma once
#include<initializer_list>

#include<stdio.h>
namespace YiaEngine
{
	template<typename T, int N> struct Vec;

	template<typename T, int...Indexes>
	struct swizzle {
		//T data[sizeof...(Indexes)];
	public:
		T elem(int i) const
		{
			return reinterpret_cast<const T*>(this)[i];
		}
		T& elem(int i)
		{
			return reinterpret_cast<T*>(this)[i];
		}

		operator Vec<T, sizeof...(Indexes)>()const
		{
			return Vec<T, sizeof...(Indexes)>(std::initializer_list<T>{elem(Indexes)...});
		}
		swizzle() = default;
		swizzle(const Vec<T, sizeof...(Indexes)>& rhs)
		{
			T* tmp = reinterpret_cast<T*>(this);
			int idx[] = { Indexes... };
			for (int i = 0; i < sizeof...(Indexes); i++)
			{

				tmp[idx[i]] = rhs[i];
			}
		}
		swizzle& operator =(const Vec<T, sizeof...(Indexes)>& rhs)
		{
			T* tmp = reinterpret_cast<T*>(this);
			int idx[] = { Indexes... };
			for (int i = 0; i < sizeof...(Indexes); i++)
			{

				tmp[idx[i]] = rhs[i];
			}
			return *this;
		}

		
	};
	template<typename T,int... Is,int...Rs>
	Vec<T, sizeof...(Is)> operator + (const swizzle<T, Is...>&lhs, const swizzle<T, Rs...>&rhs)
	{
		return Vec<T, sizeof...(Is)>(lhs) + Vec<T, sizeof...(Rs)>(rhs);
	}

	template<typename T, int...Indexes>
	void DebugSwizzle(const swizzle<T, Indexes...>& swi)
	{
		int idx[] = { Indexes... };
		for (int i = 0; i < sizeof...(Indexes); i++)
		{
			printf("%f ",swi.elem(idx[i]));
		}
		printf("\n");
	}




#define SWIZZLE2(T,E0,E1) \
	swizzle<T,0,0> E0 ## E0; \
    swizzle<T,0,1> E0 ## E1; \
    swizzle<T,1,0> E1 ## E0; \
    swizzle<T,1,1> E1 ## E1; \

#define SWIZZLE3(T,E0,E1,E2)   \
    swizzle<T,0,0> E0 ## E0; \
    swizzle<T,0,1> E0 ## E1; \
    swizzle<T,1,0> E1 ## E0; \
    swizzle<T,1,1> E1 ## E1; \
    swizzle<T,0,2> E0 ## E2; \
    swizzle<T,2,0> E2 ## E0; \
    swizzle<T,2,2> E2 ## E2; \
    swizzle<T,1,2> E1 ## E2; \
    swizzle<T,2,1> E2 ## E1; \
    swizzle<T,0,0,0> E0 ## E0 ## E0; \
    swizzle<T,0,0,1> E0 ## E0 ## E1; \
    swizzle<T,0,0,2> E0 ## E0 ## E2; \
    swizzle<T,0,1,0> E0 ## E1 ## E0; \
    swizzle<T,0,1,1> E0 ## E1 ## E1; \
    swizzle<T,0,1,2> E0 ## E1 ## E2; \
    swizzle<T,0,2,0> E0 ## E2 ## E0; \
    swizzle<T,0,2,1> E0 ## E2 ## E1; \
    swizzle<T,0,2,2> E0 ## E2 ## E2; \
    swizzle<T,1,0,0> E1 ## E0 ## E0; \
    swizzle<T,1,0,1> E1 ## E0 ## E1; \
    swizzle<T,1,0,2> E1 ## E0 ## E2; \
    swizzle<T,1,1,0> E1 ## E1 ## E0; \
    swizzle<T,1,1,1> E1 ## E1 ## E1; \
    swizzle<T,1,1,2> E1 ## E1 ## E2; \
    swizzle<T,1,2,0> E1 ## E2 ## E0; \
    swizzle<T,1,2,1> E1 ## E2 ## E1; \
    swizzle<T,1,2,2> E1 ## E2 ## E2; \
    swizzle<T,2,0,0> E2 ## E0 ## E0; \
    swizzle<T,2,0,1> E2 ## E0 ## E1; \
    swizzle<T,2,0,2> E2 ## E0 ## E2; \
    swizzle<T,2,1,0> E2 ## E1 ## E0; \
    swizzle<T,2,1,1> E2 ## E1 ## E1; \
    swizzle<T,2,1,2> E2 ## E1 ## E2; \
    swizzle<T,2,2,0> E2 ## E2 ## E0; \
    swizzle<T,2,2,1> E2 ## E2 ## E1; \
    swizzle<T,2,2,2> E2 ## E2 ## E2; 
#define SWIZZLE4(T,E0,E1,E2,E3) \
	swizzle<T,0,0> E0 ## E0; \
	swizzle<T,0,1> E0 ## E1; \
	swizzle<T,1,0> E1 ## E0; \
	swizzle<T,1,1> E1 ## E1; \
	swizzle<T,0,2> E0 ## E2; \
	swizzle<T,2,0> E2 ## E0; \
	swizzle<T,2,2> E2 ## E2; \
	swizzle<T,1,2> E1 ## E2; \
	swizzle<T,2,1> E2 ## E1; \
	swizzle<T,0,0,0> E0 ## E0 ## E0; \
	swizzle<T,0,0,1> E0 ## E0 ## E1; \
	swizzle<T,0,0,2> E0 ## E0 ## E2; \
	swizzle<T,0,1,0> E0 ## E1 ## E0; \
	swizzle<T,0,1,1> E0 ## E1 ## E1; \
	swizzle<T,0,1,2> E0 ## E1 ## E2; \
	swizzle<T,0,2,0> E0 ## E2 ## E0; \
	swizzle<T,0,2,1> E0 ## E2 ## E1; \
	swizzle<T,0,2,2> E0 ## E2 ## E2; \
	swizzle<T,1,0,0> E1 ## E0 ## E0; \
	swizzle<T,1,0,1> E1 ## E0 ## E1; \
	swizzle<T,1,0,2> E1 ## E0 ## E2; \
	swizzle<T,1,1,0> E1 ## E1 ## E0; \
	swizzle<T,1,1,1> E1 ## E1 ## E1; \
	swizzle<T,1,1,2> E1 ## E1 ## E2; \
	swizzle<T,1,2,0> E1 ## E2 ## E0; \
	swizzle<T,1,2,1> E1 ## E2 ## E1; \
	swizzle<T,1,2,2> E1 ## E2 ## E2; \
	swizzle<T,2,0,0> E2 ## E0 ## E0; \
	swizzle<T,2,0,1> E2 ## E0 ## E1; \
	swizzle<T,2,0,2> E2 ## E0 ## E2; \
	swizzle<T,2,1,0> E2 ## E1 ## E0; \
	swizzle<T,2,1,1> E2 ## E1 ## E1; \
	swizzle<T,2,1,2> E2 ## E1 ## E2; \
	swizzle<T,2,2,0> E2 ## E2 ## E0; \
	swizzle<T,2,2,1> E2 ## E2 ## E1; \
	swizzle<T,2,2,2> E2 ## E2 ## E2; \
	swizzle<T,0,3> E0 ## E3; \
	swizzle<T,3,0> E3 ## E0; \
	swizzle<T,3,3> E3 ## E3; \
	swizzle<T,1,3> E1 ## E3; \
	swizzle<T,3,1> E3 ## E1; \
	swizzle<T,0,0,3> E0 ## E0 ## E3; \
	swizzle<T,0,1,3> E0 ## E1 ## E3; \
	swizzle<T,0,3,0> E0 ## E3 ## E0; \
	swizzle<T,0,3,1> E0 ## E3 ## E1; \
	swizzle<T,0,3,3> E0 ## E3 ## E3; \
	swizzle<T,1,0,3> E1 ## E0 ## E3; \
	swizzle<T,1,1,3> E1 ## E1 ## E3; \
	swizzle<T,1,3,0> E1 ## E3 ## E0; \
	swizzle<T,1,3,1> E1 ## E3 ## E1; \
	swizzle<T,1,3,3> E1 ## E3 ## E3; \
	swizzle<T,3,0,0> E3 ## E0 ## E0; \
	swizzle<T,3,0,1> E3 ## E0 ## E1; \
	swizzle<T,3,0,3> E3 ## E0 ## E3; \
	swizzle<T,3,1,0> E3 ## E1 ## E0; \
	swizzle<T,3,1,1> E3 ## E1 ## E1; \
	swizzle<T,3,1,3> E3 ## E1 ## E3; \
	swizzle<T,3,3,0> E3 ## E3 ## E0; \
	swizzle<T,3,3,1> E3 ## E3 ## E1; \
	swizzle<T,3,3,3> E3 ## E3 ## E3; \
	swizzle<T,2,3> E2 ## E3; \
	swizzle<T,3,2> E3 ## E2; \
	swizzle<T,0,2,3> E0 ## E2 ## E3; \
	swizzle<T,0,3,2> E0 ## E3 ## E2; \
	swizzle<T,2,0,3> E2 ## E0 ## E3; \
	swizzle<T,2,2,3> E2 ## E2 ## E3; \
	swizzle<T,2,3,0> E2 ## E3 ## E0; \
	swizzle<T,2,3,2> E2 ## E3 ## E2; \
	swizzle<T,2,3,3> E2 ## E3 ## E3; \
	swizzle<T,3,0,2> E3 ## E0 ## E2; \
	swizzle<T,3,2,0> E3 ## E2 ## E0; \
	swizzle<T,3,2,2> E3 ## E2 ## E2; \
	swizzle<T,3,2,3> E3 ## E2 ## E3; \
	swizzle<T,3,3,2> E3 ## E3 ## E2; \
	swizzle<T,1,2,3> E1 ## E2 ## E3; \
	swizzle<T,1,3,2> E1 ## E3 ## E2; \
	swizzle<T,2,1,3> E2 ## E1 ## E3; \
	swizzle<T,2,3,1> E2 ## E3 ## E1; \
	swizzle<T,3,1,2> E3 ## E1 ## E2; \
	swizzle<T,3,2,1> E3 ## E2 ## E1; \
	swizzle<T,0,0,0,0> E0 ## E0 ## E0 ## E0; \
	swizzle<T,0,0,0,1> E0 ## E0 ## E0 ## E1; \
	swizzle<T,0,0,0,2> E0 ## E0 ## E0 ## E2; \
	swizzle<T,0,0,0,3> E0 ## E0 ## E0 ## E3; \
	swizzle<T,0,0,1,0> E0 ## E0 ## E1 ## E0; \
	swizzle<T,0,0,1,1> E0 ## E0 ## E1 ## E1; \
	swizzle<T,0,0,1,2> E0 ## E0 ## E1 ## E2; \
	swizzle<T,0,0,1,3> E0 ## E0 ## E1 ## E3; \
	swizzle<T,0,0,2,0> E0 ## E0 ## E2 ## E0; \
	swizzle<T,0,0,2,1> E0 ## E0 ## E2 ## E1; \
	swizzle<T,0,0,2,2> E0 ## E0 ## E2 ## E2; \
	swizzle<T,0,0,2,3> E0 ## E0 ## E2 ## E3; \
	swizzle<T,0,0,3,0> E0 ## E0 ## E3 ## E0; \
	swizzle<T,0,0,3,1> E0 ## E0 ## E3 ## E1; \
	swizzle<T,0,0,3,2> E0 ## E0 ## E3 ## E2; \
	swizzle<T,0,0,3,3> E0 ## E0 ## E3 ## E3; \
	swizzle<T,0,1,0,0> E0 ## E1 ## E0 ## E0; \
	swizzle<T,0,1,0,1> E0 ## E1 ## E0 ## E1; \
	swizzle<T,0,1,0,2> E0 ## E1 ## E0 ## E2; \
	swizzle<T,0,1,0,3> E0 ## E1 ## E0 ## E3; \
	swizzle<T,0,1,1,0> E0 ## E1 ## E1 ## E0; \
	swizzle<T,0,1,1,1> E0 ## E1 ## E1 ## E1; \
	swizzle<T,0,1,1,2> E0 ## E1 ## E1 ## E2; \
	swizzle<T,0,1,1,3> E0 ## E1 ## E1 ## E3; \
	swizzle<T,0,1,2,0> E0 ## E1 ## E2 ## E0; \
	swizzle<T,0,1,2,1> E0 ## E1 ## E2 ## E1; \
	swizzle<T,0,1,2,2> E0 ## E1 ## E2 ## E2; \
	swizzle<T,0,1,2,3> E0 ## E1 ## E2 ## E3; \
	swizzle<T,0,1,3,0> E0 ## E1 ## E3 ## E0; \
	swizzle<T,0,1,3,1> E0 ## E1 ## E3 ## E1; \
	swizzle<T,0,1,3,2> E0 ## E1 ## E3 ## E2; \
	swizzle<T,0,1,3,3> E0 ## E1 ## E3 ## E3; \
	swizzle<T,0,2,0,0> E0 ## E2 ## E0 ## E0; \
	swizzle<T,0,2,0,1> E0 ## E2 ## E0 ## E1; \
	swizzle<T,0,2,0,2> E0 ## E2 ## E0 ## E2; \
	swizzle<T,0,2,0,3> E0 ## E2 ## E0 ## E3; \
	swizzle<T,0,2,1,0> E0 ## E2 ## E1 ## E0; \
	swizzle<T,0,2,1,1> E0 ## E2 ## E1 ## E1; \
	swizzle<T,0,2,1,2> E0 ## E2 ## E1 ## E2; \
	swizzle<T,0,2,1,3> E0 ## E2 ## E1 ## E3; \
	swizzle<T,0,2,2,0> E0 ## E2 ## E2 ## E0; \
	swizzle<T,0,2,2,1> E0 ## E2 ## E2 ## E1; \
	swizzle<T,0,2,2,2> E0 ## E2 ## E2 ## E2; \
	swizzle<T,0,2,2,3> E0 ## E2 ## E2 ## E3; \
	swizzle<T,0,2,3,0> E0 ## E2 ## E3 ## E0; \
	swizzle<T,0,2,3,1> E0 ## E2 ## E3 ## E1; \
	swizzle<T,0,2,3,2> E0 ## E2 ## E3 ## E2; \
	swizzle<T,0,2,3,3> E0 ## E2 ## E3 ## E3; \
	swizzle<T,0,3,0,0> E0 ## E3 ## E0 ## E0; \
	swizzle<T,0,3,0,1> E0 ## E3 ## E0 ## E1; \
	swizzle<T,0,3,0,2> E0 ## E3 ## E0 ## E2; \
	swizzle<T,0,3,0,3> E0 ## E3 ## E0 ## E3; \
	swizzle<T,0,3,1,0> E0 ## E3 ## E1 ## E0; \
	swizzle<T,0,3,1,1> E0 ## E3 ## E1 ## E1; \
	swizzle<T,0,3,1,2> E0 ## E3 ## E1 ## E2; \
	swizzle<T,0,3,1,3> E0 ## E3 ## E1 ## E3; \
	swizzle<T,0,3,2,0> E0 ## E3 ## E2 ## E0; \
	swizzle<T,0,3,2,1> E0 ## E3 ## E2 ## E1; \
	swizzle<T,0,3,2,2> E0 ## E3 ## E2 ## E2; \
	swizzle<T,0,3,2,3> E0 ## E3 ## E2 ## E3; \
	swizzle<T,0,3,3,0> E0 ## E3 ## E3 ## E0; \
	swizzle<T,0,3,3,1> E0 ## E3 ## E3 ## E1; \
	swizzle<T,0,3,3,2> E0 ## E3 ## E3 ## E2; \
	swizzle<T,0,3,3,3> E0 ## E3 ## E3 ## E3; \
	swizzle<T,1,0,0,0> E1 ## E0 ## E0 ## E0; \
	swizzle<T,1,0,0,1> E1 ## E0 ## E0 ## E1; \
	swizzle<T,1,0,0,2> E1 ## E0 ## E0 ## E2; \
	swizzle<T,1,0,0,3> E1 ## E0 ## E0 ## E3; \
	swizzle<T,1,0,1,0> E1 ## E0 ## E1 ## E0; \
	swizzle<T,1,0,1,1> E1 ## E0 ## E1 ## E1; \
	swizzle<T,1,0,1,2> E1 ## E0 ## E1 ## E2; \
	swizzle<T,1,0,1,3> E1 ## E0 ## E1 ## E3; \
	swizzle<T,1,0,2,0> E1 ## E0 ## E2 ## E0; \
	swizzle<T,1,0,2,1> E1 ## E0 ## E2 ## E1; \
	swizzle<T,1,0,2,2> E1 ## E0 ## E2 ## E2; \
	swizzle<T,1,0,2,3> E1 ## E0 ## E2 ## E3; \
	swizzle<T,1,0,3,0> E1 ## E0 ## E3 ## E0; \
	swizzle<T,1,0,3,1> E1 ## E0 ## E3 ## E1; \
	swizzle<T,1,0,3,2> E1 ## E0 ## E3 ## E2; \
	swizzle<T,1,0,3,3> E1 ## E0 ## E3 ## E3; \
	swizzle<T,1,1,0,0> E1 ## E1 ## E0 ## E0; \
	swizzle<T,1,1,0,1> E1 ## E1 ## E0 ## E1; \
	swizzle<T,1,1,0,2> E1 ## E1 ## E0 ## E2; \
	swizzle<T,1,1,0,3> E1 ## E1 ## E0 ## E3; \
	swizzle<T,1,1,1,0> E1 ## E1 ## E1 ## E0; \
	swizzle<T,1,1,1,1> E1 ## E1 ## E1 ## E1; \
	swizzle<T,1,1,1,2> E1 ## E1 ## E1 ## E2; \
	swizzle<T,1,1,1,3> E1 ## E1 ## E1 ## E3; \
	swizzle<T,1,1,2,0> E1 ## E1 ## E2 ## E0; \
	swizzle<T,1,1,2,1> E1 ## E1 ## E2 ## E1; \
	swizzle<T,1,1,2,2> E1 ## E1 ## E2 ## E2; \
	swizzle<T,1,1,2,3> E1 ## E1 ## E2 ## E3; \
	swizzle<T,1,1,3,0> E1 ## E1 ## E3 ## E0; \
	swizzle<T,1,1,3,1> E1 ## E1 ## E3 ## E1; \
	swizzle<T,1,1,3,2> E1 ## E1 ## E3 ## E2; \
	swizzle<T,1,1,3,3> E1 ## E1 ## E3 ## E3; \
	swizzle<T,1,2,0,0> E1 ## E2 ## E0 ## E0; \
	swizzle<T,1,2,0,1> E1 ## E2 ## E0 ## E1; \
	swizzle<T,1,2,0,2> E1 ## E2 ## E0 ## E2; \
	swizzle<T,1,2,0,3> E1 ## E2 ## E0 ## E3; \
	swizzle<T,1,2,1,0> E1 ## E2 ## E1 ## E0; \
	swizzle<T,1,2,1,1> E1 ## E2 ## E1 ## E1; \
	swizzle<T,1,2,1,2> E1 ## E2 ## E1 ## E2; \
	swizzle<T,1,2,1,3> E1 ## E2 ## E1 ## E3; \
	swizzle<T,1,2,2,0> E1 ## E2 ## E2 ## E0; \
	swizzle<T,1,2,2,1> E1 ## E2 ## E2 ## E1; \
	swizzle<T,1,2,2,2> E1 ## E2 ## E2 ## E2; \
	swizzle<T,1,2,2,3> E1 ## E2 ## E2 ## E3; \
	swizzle<T,1,2,3,0> E1 ## E2 ## E3 ## E0; \
	swizzle<T,1,2,3,1> E1 ## E2 ## E3 ## E1; \
	swizzle<T,1,2,3,2> E1 ## E2 ## E3 ## E2; \
	swizzle<T,1,2,3,3> E1 ## E2 ## E3 ## E3; \
	swizzle<T,1,3,0,0> E1 ## E3 ## E0 ## E0; \
	swizzle<T,1,3,0,1> E1 ## E3 ## E0 ## E1; \
	swizzle<T,1,3,0,2> E1 ## E3 ## E0 ## E2; \
	swizzle<T,1,3,0,3> E1 ## E3 ## E0 ## E3; \
	swizzle<T,1,3,1,0> E1 ## E3 ## E1 ## E0; \
	swizzle<T,1,3,1,1> E1 ## E3 ## E1 ## E1; \
	swizzle<T,1,3,1,2> E1 ## E3 ## E1 ## E2; \
	swizzle<T,1,3,1,3> E1 ## E3 ## E1 ## E3; \
	swizzle<T,1,3,2,0> E1 ## E3 ## E2 ## E0; \
	swizzle<T,1,3,2,1> E1 ## E3 ## E2 ## E1; \
	swizzle<T,1,3,2,2> E1 ## E3 ## E2 ## E2; \
	swizzle<T,1,3,2,3> E1 ## E3 ## E2 ## E3; \
	swizzle<T,1,3,3,0> E1 ## E3 ## E3 ## E0; \
	swizzle<T,1,3,3,1> E1 ## E3 ## E3 ## E1; \
	swizzle<T,1,3,3,2> E1 ## E3 ## E3 ## E2; \
	swizzle<T,1,3,3,3> E1 ## E3 ## E3 ## E3; \
	swizzle<T,2,0,0,0> E2 ## E0 ## E0 ## E0; \
	swizzle<T,2,0,0,1> E2 ## E0 ## E0 ## E1; \
	swizzle<T,2,0,0,2> E2 ## E0 ## E0 ## E2; \
	swizzle<T,2,0,0,3> E2 ## E0 ## E0 ## E3; \
	swizzle<T,2,0,1,0> E2 ## E0 ## E1 ## E0; \
	swizzle<T,2,0,1,1> E2 ## E0 ## E1 ## E1; \
	swizzle<T,2,0,1,2> E2 ## E0 ## E1 ## E2; \
	swizzle<T,2,0,1,3> E2 ## E0 ## E1 ## E3; \
	swizzle<T,2,0,2,0> E2 ## E0 ## E2 ## E0; \
	swizzle<T,2,0,2,1> E2 ## E0 ## E2 ## E1; \
	swizzle<T,2,0,2,2> E2 ## E0 ## E2 ## E2; \
	swizzle<T,2,0,2,3> E2 ## E0 ## E2 ## E3; \
	swizzle<T,2,0,3,0> E2 ## E0 ## E3 ## E0; \
	swizzle<T,2,0,3,1> E2 ## E0 ## E3 ## E1; \
	swizzle<T,2,0,3,2> E2 ## E0 ## E3 ## E2; \
	swizzle<T,2,0,3,3> E2 ## E0 ## E3 ## E3; \
	swizzle<T,2,1,0,0> E2 ## E1 ## E0 ## E0; \
	swizzle<T,2,1,0,1> E2 ## E1 ## E0 ## E1; \
	swizzle<T,2,1,0,2> E2 ## E1 ## E0 ## E2; \
	swizzle<T,2,1,0,3> E2 ## E1 ## E0 ## E3; \
	swizzle<T,2,1,1,0> E2 ## E1 ## E1 ## E0; \
	swizzle<T,2,1,1,1> E2 ## E1 ## E1 ## E1; \
	swizzle<T,2,1,1,2> E2 ## E1 ## E1 ## E2; \
	swizzle<T,2,1,1,3> E2 ## E1 ## E1 ## E3; \
	swizzle<T,2,1,2,0> E2 ## E1 ## E2 ## E0; \
	swizzle<T,2,1,2,1> E2 ## E1 ## E2 ## E1; \
	swizzle<T,2,1,2,2> E2 ## E1 ## E2 ## E2; \
	swizzle<T,2,1,2,3> E2 ## E1 ## E2 ## E3; \
	swizzle<T,2,1,3,0> E2 ## E1 ## E3 ## E0; \
	swizzle<T,2,1,3,1> E2 ## E1 ## E3 ## E1; \
	swizzle<T,2,1,3,2> E2 ## E1 ## E3 ## E2; \
	swizzle<T,2,1,3,3> E2 ## E1 ## E3 ## E3; \
	swizzle<T,2,2,0,0> E2 ## E2 ## E0 ## E0; \
	swizzle<T,2,2,0,1> E2 ## E2 ## E0 ## E1; \
	swizzle<T,2,2,0,2> E2 ## E2 ## E0 ## E2; \
	swizzle<T,2,2,0,3> E2 ## E2 ## E0 ## E3; \
	swizzle<T,2,2,1,0> E2 ## E2 ## E1 ## E0; \
	swizzle<T,2,2,1,1> E2 ## E2 ## E1 ## E1; \
	swizzle<T,2,2,1,2> E2 ## E2 ## E1 ## E2; \
	swizzle<T,2,2,1,3> E2 ## E2 ## E1 ## E3; \
	swizzle<T,2,2,2,0> E2 ## E2 ## E2 ## E0; \
	swizzle<T,2,2,2,1> E2 ## E2 ## E2 ## E1; \
	swizzle<T,2,2,2,2> E2 ## E2 ## E2 ## E2; \
	swizzle<T,2,2,2,3> E2 ## E2 ## E2 ## E3; \
	swizzle<T,2,2,3,0> E2 ## E2 ## E3 ## E0; \
	swizzle<T,2,2,3,1> E2 ## E2 ## E3 ## E1; \
	swizzle<T,2,2,3,2> E2 ## E2 ## E3 ## E2; \
	swizzle<T,2,2,3,3> E2 ## E2 ## E3 ## E3; \
	swizzle<T,2,3,0,0> E2 ## E3 ## E0 ## E0; \
	swizzle<T,2,3,0,1> E2 ## E3 ## E0 ## E1; \
	swizzle<T,2,3,0,2> E2 ## E3 ## E0 ## E2; \
	swizzle<T,2,3,0,3> E2 ## E3 ## E0 ## E3; \
	swizzle<T,2,3,1,0> E2 ## E3 ## E1 ## E0; \
	swizzle<T,2,3,1,1> E2 ## E3 ## E1 ## E1; \
	swizzle<T,2,3,1,2> E2 ## E3 ## E1 ## E2; \
	swizzle<T,2,3,1,3> E2 ## E3 ## E1 ## E3; \
	swizzle<T,2,3,2,0> E2 ## E3 ## E2 ## E0; \
	swizzle<T,2,3,2,1> E2 ## E3 ## E2 ## E1; \
	swizzle<T,2,3,2,2> E2 ## E3 ## E2 ## E2; \
	swizzle<T,2,3,2,3> E2 ## E3 ## E2 ## E3; \
	swizzle<T,2,3,3,0> E2 ## E3 ## E3 ## E0; \
	swizzle<T,2,3,3,1> E2 ## E3 ## E3 ## E1; \
	swizzle<T,2,3,3,2> E2 ## E3 ## E3 ## E2; \
	swizzle<T,2,3,3,3> E2 ## E3 ## E3 ## E3; \
	swizzle<T,3,0,0,0> E3 ## E0 ## E0 ## E0; \
	swizzle<T,3,0,0,1> E3 ## E0 ## E0 ## E1; \
	swizzle<T,3,0,0,2> E3 ## E0 ## E0 ## E2; \
	swizzle<T,3,0,0,3> E3 ## E0 ## E0 ## E3; \
	swizzle<T,3,0,1,0> E3 ## E0 ## E1 ## E0; \
	swizzle<T,3,0,1,1> E3 ## E0 ## E1 ## E1; \
	swizzle<T,3,0,1,2> E3 ## E0 ## E1 ## E2; \
	swizzle<T,3,0,1,3> E3 ## E0 ## E1 ## E3; \
	swizzle<T,3,0,2,0> E3 ## E0 ## E2 ## E0; \
	swizzle<T,3,0,2,1> E3 ## E0 ## E2 ## E1; \
	swizzle<T,3,0,2,2> E3 ## E0 ## E2 ## E2; \
	swizzle<T,3,0,2,3> E3 ## E0 ## E2 ## E3; \
	swizzle<T,3,0,3,0> E3 ## E0 ## E3 ## E0; \
	swizzle<T,3,0,3,1> E3 ## E0 ## E3 ## E1; \
	swizzle<T,3,0,3,2> E3 ## E0 ## E3 ## E2; \
	swizzle<T,3,0,3,3> E3 ## E0 ## E3 ## E3; \
	swizzle<T,3,1,0,0> E3 ## E1 ## E0 ## E0; \
	swizzle<T,3,1,0,1> E3 ## E1 ## E0 ## E1; \
	swizzle<T,3,1,0,2> E3 ## E1 ## E0 ## E2; \
	swizzle<T,3,1,0,3> E3 ## E1 ## E0 ## E3; \
	swizzle<T,3,1,1,0> E3 ## E1 ## E1 ## E0; \
	swizzle<T,3,1,1,1> E3 ## E1 ## E1 ## E1; \
	swizzle<T,3,1,1,2> E3 ## E1 ## E1 ## E2; \
	swizzle<T,3,1,1,3> E3 ## E1 ## E1 ## E3; \
	swizzle<T,3,1,2,0> E3 ## E1 ## E2 ## E0; \
	swizzle<T,3,1,2,1> E3 ## E1 ## E2 ## E1; \
	swizzle<T,3,1,2,2> E3 ## E1 ## E2 ## E2; \
	swizzle<T,3,1,2,3> E3 ## E1 ## E2 ## E3; \
	swizzle<T,3,1,3,0> E3 ## E1 ## E3 ## E0; \
	swizzle<T,3,1,3,1> E3 ## E1 ## E3 ## E1; \
	swizzle<T,3,1,3,2> E3 ## E1 ## E3 ## E2; \
	swizzle<T,3,1,3,3> E3 ## E1 ## E3 ## E3; \
	swizzle<T,3,2,0,0> E3 ## E2 ## E0 ## E0; \
	swizzle<T,3,2,0,1> E3 ## E2 ## E0 ## E1; \
	swizzle<T,3,2,0,2> E3 ## E2 ## E0 ## E2; \
	swizzle<T,3,2,0,3> E3 ## E2 ## E0 ## E3; \
	swizzle<T,3,2,1,0> E3 ## E2 ## E1 ## E0; \
	swizzle<T,3,2,1,1> E3 ## E2 ## E1 ## E1; \
	swizzle<T,3,2,1,2> E3 ## E2 ## E1 ## E2; \
	swizzle<T,3,2,1,3> E3 ## E2 ## E1 ## E3; \
	swizzle<T,3,2,2,0> E3 ## E2 ## E2 ## E0; \
	swizzle<T,3,2,2,1> E3 ## E2 ## E2 ## E1; \
	swizzle<T,3,2,2,2> E3 ## E2 ## E2 ## E2; \
	swizzle<T,3,2,2,3> E3 ## E2 ## E2 ## E3; \
	swizzle<T,3,2,3,0> E3 ## E2 ## E3 ## E0; \
	swizzle<T,3,2,3,1> E3 ## E2 ## E3 ## E1; \
	swizzle<T,3,2,3,2> E3 ## E2 ## E3 ## E2; \
	swizzle<T,3,2,3,3> E3 ## E2 ## E3 ## E3; \
	swizzle<T,3,3,0,0> E3 ## E3 ## E0 ## E0; \
	swizzle<T,3,3,0,1> E3 ## E3 ## E0 ## E1; \
	swizzle<T,3,3,0,2> E3 ## E3 ## E0 ## E2; \
	swizzle<T,3,3,0,3> E3 ## E3 ## E0 ## E3; \
	swizzle<T,3,3,1,0> E3 ## E3 ## E1 ## E0; \
	swizzle<T,3,3,1,1> E3 ## E3 ## E1 ## E1; \
	swizzle<T,3,3,1,2> E3 ## E3 ## E1 ## E2; \
	swizzle<T,3,3,1,3> E3 ## E3 ## E1 ## E3; \
	swizzle<T,3,3,2,0> E3 ## E3 ## E2 ## E0; \
	swizzle<T,3,3,2,1> E3 ## E3 ## E2 ## E1; \
	swizzle<T,3,3,2,2> E3 ## E3 ## E2 ## E2; \
	swizzle<T,3,3,2,3> E3 ## E3 ## E2 ## E3; \
	swizzle<T,3,3,3,0> E3 ## E3 ## E3 ## E0; \
	swizzle<T,3,3,3,1> E3 ## E3 ## E3 ## E1; \
	swizzle<T,3,3,3,2> E3 ## E3 ## E3 ## E2; \
	swizzle<T,3,3,3,3> E3 ## E3 ## E3 ## E3; \



}