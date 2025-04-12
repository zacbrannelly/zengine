#pragma once
#include "LoggingSystem.h"
#include <string>

namespace ZEngine
{
  class Logger
  {
  public:
    Logger(std::string module);
    ~Logger();

    void LogError(const std::string& message);
    void LogWarning(const std::string& message);
    void LogInfo(const std::string& message);
    void LogDebug(const std::string& message);
    void Log(const std::string& message, LogLevel level);
  private:
    std::string _module;
    LoggingSystem* _loggerSystem;

    std::string formatMessage(const std::string& message);
  };
}
