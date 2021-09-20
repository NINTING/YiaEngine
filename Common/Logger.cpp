#include "Logger.h"

namespace YiaEngine
{
	std::shared_ptr<spdlog::logger> Logger::CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::AppLogger;
	std::shared_ptr<spdlog::logger> Logger::GraphicLogger;
	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		CoreLogger = spdlog::stdout_color_mt("CORE");
		AppLogger = spdlog::stdout_color_mt("APP");
		GraphicLogger = spdlog::stdout_color_mt("GRAPHIC");
		

		CoreLogger->set_level(spdlog::level::trace);
		AppLogger->set_level(spdlog::level::trace);
		GraphicLogger->set_level(spdlog::level::trace);

	}
}