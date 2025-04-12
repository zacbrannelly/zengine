#include "LoggingSystem.h"
#include <iostream>

using namespace ZEngine;

void LoggingSystem::Init()
{
  _printLogLevel = LOG_INFO;
}

void LoggingSystem::Shutdown()
{
  ClearLogs();
}

void LoggingSystem::LogError(const std::string& message, const std::string& module)
{
  Log(message, LOG_ERROR, module);
}

void LoggingSystem::LogWarning(const std::string& message, const std::string& module)
{
  Log(message, LOG_WARNING, module);
}

void LoggingSystem::LogInfo(const std::string& message, const std::string& module)
{
  Log(message, LOG_INFO, module);
}

void LoggingSystem::LogDebug(const std::string& message, const std::string& module)
{
  Log(message, LOG_DEBUG, module);
}

void LoggingSystem::Log(const std::string& message, LogLevel level, const std::string& module)
{
  if (level <= _printLogLevel)
  {
    std::cout << "[" << LogLevelToString(level) << "]" << "[" << module << "] " << message << std::endl;
  }

  _logs.push_back({ module, message, level });
}

void LoggingSystem::SetPrintLogLevel(LogLevel level)
{
  _printLogLevel = level;
}

LogLevel LoggingSystem::GetPrintLogLevel() const
{
  return _printLogLevel;
}

void LoggingSystem::ClearLogs()
{
  _logs.clear();
}

const std::vector<LogMessage>& LoggingSystem::GetLogs() const
{
  return _logs;
}

std::vector<LogMessage> LoggingSystem::GetLogsFromLevel(LogLevel level) const
{
  std::vector<LogMessage> filteredLogs;
  for (const auto& log : _logs)
  {
    if (log.level <= level)
    {
      filteredLogs.push_back(log);
    }
  }
  return filteredLogs;
}

std::string LoggingSystem::LogLevelToString(LogLevel level)
{
  switch (level)
  {
    case LOG_ERROR:   return "ERROR";
    case LOG_WARNING: return "WARNING";
    case LOG_INFO:    return "INFO";
    case LOG_DEBUG:   return "DEBUG";
    default:          return "UNKNOWN";
  }
}
