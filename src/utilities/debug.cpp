#include "debug.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

Debug debug = Debug();

Debug::Debug() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("%^[%T] [%l] %v%$");

    logger = std::make_shared<spdlog::logger>("logger", console_sink);
}
