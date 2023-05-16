#pragma once

template<typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if (T::_instance == nullptr)
			T::_instance = new T();

		return T::_instance;
	}

protected:
	Singleton()
	{
		_instance = static_cast<T*>(this);
	}

	static T* _instance;
};

template<typename T>
T* Singleton<T>::_instance = nullptr;
