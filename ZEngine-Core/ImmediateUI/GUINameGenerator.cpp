#include "GUINameGenerator.h"

using namespace std;
using namespace ZEngine;

map<string, int> GUINameGenerator::_counter;

string GUINameGenerator::GetUniqueName(string name)
{
	string result;

	int count = 0;
	auto it = _counter.find(name);

	if (it == _counter.end())
		_counter[name] = 1;
	else
		count = it->second;

	result = name + "###" + name + to_string(count);

	if (it != _counter.end())
		it->second++;

	return result;
}
