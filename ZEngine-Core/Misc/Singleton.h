#pragma once

#include <stdexcept>

enum SingletonMode
{
    CREATE_ON_FETCH,
    SET_ON_CREATE
};

template<typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if (T::_instance == nullptr)
        {
            switch (T::_mode)
            {
                case CREATE_ON_FETCH:
                    T::_instance = new T();
                    break;
                default:
                    throw std::runtime_error("No instance created yet, use CREATE_ON_FETCH mode instead if applicable");
            }
        }

		return T::_instance;
	}

protected:
	Singleton(SingletonMode mode = CREATE_ON_FETCH)
	{
		_instance = static_cast<T*>(this);
        _mode = mode;
	}

	static T* _instance;
    static SingletonMode _mode;
};

template<typename T>
T* Singleton<T>::_instance = nullptr;

template<typename T>
SingletonMode Singleton<T>::_mode = CREATE_ON_FETCH;
