#pragma once
#include"pch.h"  

class YIA_API App
{
public:
	App();
	virtual void Init() = 0;
	virtual void Run() = 0;
	virtual void Destroy() = 0;
};

App* CreateApplication();