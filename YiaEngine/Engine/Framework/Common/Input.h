#pragma once

namespace YiaEngine
{
	enum class KeyCodeEnum : int
	{
		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D', 
		E, 
		F, 
		G, 
		H, 
		I, 
		J, 
		K, 
		L, 
		M, 
		N, 
		O, 
		P, 
		Q, 
		R, 
		S, 
		T, 
		U, 
		V, 
		W, 
		X, 
		Y, 
		Z = 'Z'
	};
	class Input
	{
	public:
		static bool IsKeyDown(KeyCodeEnum key);
		static Vec2f MouseDragDelta();
	};
}