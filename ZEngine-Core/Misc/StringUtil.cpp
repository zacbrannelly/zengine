#include "StringUtil.h"

#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

vector<string> StringUtil::Split(string data, char delim)
{
	vector<string> results;
	stringstream ss(data);

	string chunk;
	while (getline(ss, chunk, delim))
	{
		results.push_back(chunk);
	}

	return results;
}

vector<string> StringUtil::Split(string value, string delim)
{
	vector<string> results;
	string current = value;

	std::size_t pos = -1;
	while ((pos = current.find(delim)) != -1)
	{
		string bit = current.substr(0, pos);
		results.push_back(bit);

		current = current.substr(pos + delim.length(), current.length() - 1);
	}

	if (current.length() > 0)
		results.push_back(current);

	return results;
}

bool StringUtil::StartsWith(string value, string prefix)
{
	return value.substr(0, prefix.length()) == prefix;
}

bool StringUtil::Contains(string data, string value)
{
	return data.find(value) != string::npos;
}

void StringUtil::ConvertToLower(std::string& input)
{
	transform(input.begin(), input.end(), input.begin(), tolower);
}