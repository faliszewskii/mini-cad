//
// Created by faliszewskii on 19.02.24.
//

#ifndef OPENGL_SANDBOX_LOGGER_H
#define OPENGL_SANDBOX_LOGGER_H

#include <string>
#include <vector>

class Logger {

    #define LOG_TYPES \
    LOG_TYPE(Info, "INF", 1, 1, 1)\
    LOG_TYPE(Debug, "DBG", 0.5, 0.5, 1)\
    LOG_TYPE(Error, "ERR", 1, 0.2, 0.2)

    enum LogType {
        #define LOG_TYPE(ENUM, name, r, g, b) \
            ENUM,
        LOG_TYPES
        #undef LOG_TYPE
    };

    struct LogEntry {
        LogType logType;
        std::string message;
        float time;
    };

    std::vector<LogEntry> logs;

public:

    static std::string getName(LogType logType) {
        switch(logType) {
            #define LOG_TYPE(ENUM, name, r, g, b) \
                case ENUM:               \
                    return name;
            LOG_TYPES
            #undef LOG_TYPE
        }
        return "";
    }

    static glm::vec3 getColor(LogType logType) {
        switch(logType) {
            #define LOG_TYPE(ENUM, name, r, g, b) \
                case ENUM:               \
                    return glm::vec3(r, g, b);
            LOG_TYPES
            #undef LOG_TYPE
        }
        return {};
    }

    #define LOG_TYPE(ENUM, name, r, g, b) \
        void log##ENUM(const std::string &message) { log(ENUM, message); }
    LOG_TYPES
    #undef LOG_TYPE

    void log(LogType logType, const std::string &message) {
        logs.push_back(LogEntry(logType, message, (float)(glfwGetTime())));
    }

    [[nodiscard]] const std::vector<LogEntry> &getLogs() const {
        return logs;
    }
};

#endif //OPENGL_SANDBOX_LOGGER_H
