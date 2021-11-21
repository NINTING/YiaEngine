#pragma once


#include"Common/YiaMath.h"
#include"Component.h"


namespace YiaEngine
{
	class RigidBody : Component
	{
	public:
		RigidBody(GameObject&obj):Component(obj){}
	private:
		//����
		double mass_;
		Math::Mat3x3f TensorBody, TensorBodyInv;
		//״̬����
		Math::Vec3f CenterPosW;
		Math::Mat3x3f Rotation;
		Math::Vec3f linearMomentum,angularMomentum;
	
		//����
		Math::Mat3x3f TensorInv;
		Math::Vec3f linearVelocity, angularVelocity;
		Math::Vec3f force, torque;
		void ComputeLinearForce();

	public:
		void SetMass(float mass);
		void SetVelocity(Math::Vec3f v) { linearVelocity =v; };
		void SetStartForce(Math::Vec3f f);
		void ComputeData();
		Math::Quaternion QuatRotation;
	public:
		virtual void Update();

	};
}
