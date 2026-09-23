#include <fstream>
#include <format>

#include "resources.hpp"

std::string resources::get_resource_path(std::string_view file_name) {
    return std::format("{}{}", RESOURCES_PATH, file_name);
}

Resource resources::get_resource(std::string_view file_name) {
    auto file_path = get_resource_path(file_name);
    std::ifstream file(file_path);

    if (file.fail())
        throw std::runtime_error(std::format("Failed to open resource: {}", file_path));

    auto resource = Resource();
    resource.data = std::string(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());
    return resource;
}
