#include "utils/config.hpp"
#include <fstream>
#include <cctype>

namespace utils {
    Config& Config::instance() {
        static Config inst;
        return inst;
    }

    void Config::load(const std::string& path) {
        std::ifstream f(path);
        if (!f.is_open()) return;

        auto trim = [](std::string s) {
            while (!s.empty() && std::isspace((unsigned char)s.front())) s.erase(s.begin());
            while (!s.empty() && std::isspace((unsigned char)s.back()))  s.pop_back();
            return s;
        };

        std::string line, section;
        while (std::getline(f, line)) {
            line = trim(line);
            if (line.empty() || line[0] == '#' || line[0] == ';') continue;
            if (line[0] == '[') {
                section = line.substr(1, line.find(']') - 1);
                continue;
            }
            auto eq = line.find('=');
            if (eq == std::string::npos) continue;
            std::string key   = trim(section + "." + line.substr(0, eq));
            std::string value = trim(line.substr(eq + 1));
            values[key] = value;
        }
    }

    int Config::getInt(const std::string& key, int def) const {
        auto it = values.find(key);
        if (it == values.end()) return def;
        try { return std::stoi(it->second); } catch (...) { return def; }
    }

    float Config::getFloat(const std::string& key, float def) const {
        auto it = values.find(key);
        if (it == values.end()) return def;
        try { return std::stof(it->second); } catch (...) { return def; }
    }

    std::string Config::getString(const std::string& key, const std::string& def) const {
        auto it = values.find(key);
        return it == values.end() ? def : it->second;
    }
}
