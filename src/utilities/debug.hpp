#pragma once

#include <spdlog/spdlog.h>

class Debug {
public:
    Debug();

    // TODO: use function where you need to just pass enum so i don't have to create 19348238 functions for each level
    template <typename ...Args>
    void info(std::string_view message, Args&& ...args) {
        auto x = std::vformat(message, std::make_format_args(args...));
        logger->info(x);
    }

    template <typename ...Args>
    void warn(std::string_view message, Args&& ...args) {
        auto x = std::vformat(message, std::make_format_args(args...));
        logger->warn(x);
    }

    template <typename ...Args>
    void error(std::string_view message, Args&& ...args) {
        auto x = std::vformat(message, std::make_format_args(args...));
        logger->error(x);
    }
private:
    std::shared_ptr<spdlog::logger> logger;
};

extern Debug debug; // only thing that I consider as suitable for global variable
