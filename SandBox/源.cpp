#include<YiaEngine.h>
using namespace YiaEngine;
class SampleApp :public Application
{
public:
	virtual void Update()
	{
	//	YIA_INFO("Update");
	}
};


Application* CreateApplication()
{
	return new SampleApp();
}