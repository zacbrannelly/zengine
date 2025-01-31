#pragma once

#include <string>
#include <map>

namespace ZEngine
{
	class GUINameGenerator
	{
	public:
		static std::string GetUniqueName(std::string name);

	private:
		static std::map<std::string, int> _counter;
	};
}
