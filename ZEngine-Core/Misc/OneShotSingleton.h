#pragma once

template<typename T>
class OneShotSingleton
{
public:
	static T* GetInstance()
	{
		if (T::_instance == nullptr)
			T::_instance = new T();

		return T::_instance;
	}

protected:
	static T* _instance;
};

template<typename T>
typename T* OneShotSingleton<T>::_instance = nullptr;

