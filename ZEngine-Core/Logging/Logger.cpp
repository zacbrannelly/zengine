#include "Logger.h"

using namespace ZEngine;

Logger::Logger(std::string module) : _module(module)
{
  _loggerSystem = LoggingSystem::GetInstance();
}

void Logger::LogError(const std::string& message) const
{
  Log(message, LOG_ERROR);
}

void Logger::LogWarning(const std::string& message) const
{
  Log(message, LOG_WARNING);
}

void Logger::LogInfo(const std::string& message) const
{
  Log(message, LOG_INFO);
}

void Logger::LogDebug(const std::string& message) const
{
  Log(message, LOG_DEBUG);
}

void Logger::Log(const std::string& message, LogLevel level) const
{
  _loggerSystem->Log(message, level, _module);
}

Logger::~Logger()
{
}
