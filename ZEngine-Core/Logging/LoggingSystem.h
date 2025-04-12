#pragma once
#include "../Misc/Singleton.h"

#include <string>
#include <vector>

namespace ZEngine
{
  enum LogLevel
  {
    LOG_ERROR = 0,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG
  };

  struct LogMessage
  {
    std::string module;
    std::string message;
    LogLevel level;
  };
  
  class LoggingSystem : public Singleton<LoggingSystem>
  {
  public:
    void Init();
    void Shutdown();

    void LogError(const std::string& message, const std::string& module = "UNKNOWN");
    void LogWarning(const std::string& message, const std::string& module = "UNKNOWN");
    void LogInfo(const std::string& message, const std::string& module = "UNKNOWN");
    void LogDebug(const std::string& message, const std::string& module = "UNKNOWN");
    void Log(const std::string& message, LogLevel level, const std::string& module = "UNKNOWN");

    void SetPrintLogLevel(LogLevel level);
    LogLevel GetPrintLogLevel() const;

    void ClearLogs();
    const std::vector<LogMessage>& GetLogs() const;
    std::vector<LogMessage> GetLogsFromLevel(LogLevel level) const;

    static std::string LogLevelToString(LogLevel level);
  private:
    std::vector<LogMessage> _logs;
    LogLevel _printLogLevel;
  };
}
