#pragma once

#include <memory>

#include"pch.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace YiaEngine
{
	class YIA_API Logger
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger> CoreLog() { return CoreLogger; };
		static std::shared_ptr<spdlog::logger> AppLog() { return AppLogger; };
		static std::shared_ptr<spdlog::logger> GraphicLog() { return GraphicLogger; };
	private:
		static std::shared_ptr<spdlog::logger>CoreLogger;
		static std::shared_ptr<spdlog::logger>AppLogger;
		static std::shared_ptr<spdlog::logger>GraphicLogger;
	};
}

#define LOG_INIT() YiaEngine::Logger::Init()

#define YIA_CORE_INFO(...)  YiaEngine::Logger::CoreLog()->info(__VA_ARGS__)


#define YIA_INFO(...)  YiaEngine::Logger::AppLog()->info(__VA_ARGS__)


#define YIA_GRAPHIC_ERR(...)  ::YiaEngine::Logger::GraphicLog()->error(__VA_ARGS__)
#define YIA_GRAPHIC_WARN(...)  YiaEngine::Logger::GraphicLog()->warn(__VA_ARGS__)

#define STRINGIFY(x) #x
#define STRINGIFY_BUILTIN(x) STRINGIFY(x)


#define ASSERT_SUCCEEDED(hr,...)\
		if(FAILED(hr))					\
		{\
			YIA_GRAPHIC_ERR("\nHRESULT failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
			YIA_GRAPHIC_ERR("desc:"#__VA_ARGS__); \
			YIA_GRAPHIC_ERR("\n"); \
			__debugbreak(); \
		}