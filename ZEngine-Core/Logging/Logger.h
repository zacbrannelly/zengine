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

    void LogError(const std::string& message) const;
    void LogWarning(const std::string& message) const;
    void LogInfo(const std::string& message) const;
    void LogDebug(const std::string& message) const;
    void Log(const std::string& message, LogLevel level) const;
  private:
    std::string _module;
    LoggingSystem* _loggerSystem;

    std::string formatMessage(const std::string& message);
  };
}
