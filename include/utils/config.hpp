#pragma once
#include <string>
#include <unordered_map>

namespace utils {
    class Config {
    public:
        static Config& instance();
        void load(const std::string& path);
        int         getInt   (const std::string& key, int         def = 0)    const;
        float       getFloat (const std::string& key, float       def = 0.0f) const;
        std::string getString(const std::string& key, const std::string& def = "") const;

    private:
        Config() = default;
        std::unordered_map<std::string, std::string> values;
    };

    inline Config& Cfg() { return Config::instance(); }
}
