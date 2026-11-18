#include "Logger.h"
#include "Logger.h"

#include <fstream>
#include <iostream>
#include <mutex>

#include "SFML/System/Err.hpp"

namespace {
    struct State {
        std::mutex mutex;
        std::ofstream file;
        std::string path = "errors.log";
        bool initialized = false;
    };

    State& state() {
        static State* instance = new State;
        return *instance;
    }

    std::string timestamp() {
        const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
        return buf;
    }

    const char* levelName(Logger::LogLevel level) {
        switch (level) {
            case Logger::LogLevel::ERROR: return "ERROR";
                break;
            case Logger::LogLevel::DEBUG: return "DEBUG";
                break;
            case Logger::LogLevel::INFO: return "INFO";
                break;
            case Logger::LogLevel::WARNING: return "WARNING";
                break;

        }
        return "UNKNOWN";
    }

    class SfmlErrBuffer : public std::streambuf {
        std::string line;
    protected:
        int_type overflow(int_type ch) override {
            if (ch == traits_type::eof()) {
                return traits_type::not_eof(ch);
            }
            if (ch == '\n') {
                if (!line.empty()) {
                    Logger::warning("[SFML] " + line);
                    line.clear();
                }
            } else {
                line.push_back(static_cast<char>(ch));
            }
            return ch;
        }
    };
}

void Logger::init(const std::string &path) {
    State& s = state();
    std::lock_guard<std::mutex> lock(s.mutex);
    if (s.initialized) {
        return;
    }
    s.initialized = true;
    s.path = path;
    s.file.open(path, std::ios::app);
    if (!s.file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
    } else {
        s.file << "=== Start programu " << timestamp() << " ===" << std::endl;
    }

    static SfmlErrBuffer* buffer=new SfmlErrBuffer();
    sf::err().rdbuf(buffer);

    std::set_terminate([] {
        if (const std::exception_ptr ex = std::current_exception()) {
            try {
                std::rethrow_exception(ex);
            } catch (const std::exception& e) {
                error(std::string("Nieobsluzony wyjatek: ") + e.what());
            } catch (...) {
                error("Nieobsluzony wyjatek nieznanego typu");
            }
        } else {
            error("std::terminate wywolane bez wyjatku");
        }
        std::abort();
    });
}

void Logger::log(LogLevel level, const std::string &message) {
    bool needsInit;
    {
        State& s = state();
        std::lock_guard<std::mutex> lock(s.mutex);
        needsInit = s.initialized;
    }
    if (needsInit) {
        init(state().path);
    }
    State& s = state();
    std::lock_guard<std::mutex> lock(s.mutex);
    if (level == INFO) {
        std::cout << message << std::endl;
        return;
    }
    const std::string line = timestamp() + " [" + levelName(level) + "] " + message;
    std::cerr << line << std::endl;
    if (s.file) {
        s.file << line << std::endl;
    }
}

const std::string& Logger::path() {
    return state().path;
}