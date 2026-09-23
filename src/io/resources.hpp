#pragma once

// future-proof if i wanted to ever add more info about resource
struct Resource {
    std::string data;
};

class resources {
public:
    static std::string get_resource_path(std::string_view file_name);
    static Resource get_resource(std::string_view file_name);
private:
    constexpr static std::string_view RESOURCES_PATH = "res/";
};
