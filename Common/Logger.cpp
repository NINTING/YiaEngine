#include"pch.h"
#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
namespace YiaEngine
{
	std::shared_ptr<spdlog::logger> Logger::CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::AppLogger;
	std::shared_ptr<spdlog::logger> Logger::GraphicLogger;
	void Logger::Init()
	{
	

		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Yia.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		CoreLogger = std::make_shared<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
		spdlog::register_logger(CoreLogger);
		CoreLogger->set_level(spdlog::level::trace);
		CoreLogger->flush_on(spdlog::level::trace);

		GraphicLogger = std::make_shared<spdlog::logger>("GRAPHIC", begin(logSinks), end(logSinks));
		spdlog::register_logger(GraphicLogger);
		GraphicLogger->set_level(spdlog::level::trace);
		GraphicLogger->flush_on(spdlog::level::trace);

		AppLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(AppLogger);
		AppLogger->set_level(spdlog::level::trace);
		AppLogger->flush_on(spdlog::level::trace);

	}
}