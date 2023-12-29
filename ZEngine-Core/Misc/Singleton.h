#pragma once

template <typename T>
class Singleton
{
public:
    // Delete copy constructor and assignment operator
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // Provide a global point of access to the instance
    static T* GetInstance()
    {
        // C++11 guarantees that this will be initialized in a thread-safe way
        static T instance;
        return &instance;
    }

protected:
    // Make constructor protected to prevent direct instantiation
    Singleton() = default;

    // Destructor can be public/protected based on use case
    virtual ~Singleton() = default;
};
