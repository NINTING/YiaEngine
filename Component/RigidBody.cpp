#include "pch.h"
#include "RigidBody.h"
#include"Common/TimeStep.h"
#include"GameObject.h"
namespace YiaEngine
{

	void RigidBody::ComputeLinearForce()
	{
		Math::Vec3f a = force / mass_;
		linearVelocity += a;
	}
	void RigidBody::SetMass(float mass)
	{
		mass_ = mass;
	}
	void RigidBody::SetStartForce(Math::Vec3f f)
	{
		force = f;
		ComputeLinearForce();
		force = Math::Vec3f::Zero();
	}
	void YiaEngine::RigidBody::ComputeData()
	{
	/*	linearVelocity = linearMomentum / mass;
		TensorBodyInv = Rotation * TensorBodyInv * Rotation.transpose();
		angularVelocity = TensorInv * angularMomentum;
		Rotation = QuatRotation.toRotationMatrix();*/
		
	}

	void RigidBody::Update()
	{
		float stepTime = Timestep::GetFrameSeconds();
		gameObject.transform.Position += stepTime * linearVelocity;
		QuatRotation.normalize();
		//Math::Mat3x3f m = (Math::Mat3x3f)QuatRotation.matrix();
		std::cout << QuatRotation.toRotationMatrix() << std::endl;
		gameObject.transform.EularRotation = QuatRotation.toRotationMatrix().eulerAngles(0,1,2);
		//std::cout << gameObject.transform.EularRotation <<std::endl;
	}

}