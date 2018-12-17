#pragma once

#include <vector>

class StringUtil
{
public:
	static std::vector<std::string> Split(std::string data, char delim);
	static std::vector<std::string> Split(std::string data, std::string delim);
	static bool StartsWith(std::string data, std::string prefix);
	static bool Contains(std::string data, std::string value);
	static void ConvertToLower(std::string& input);
};
