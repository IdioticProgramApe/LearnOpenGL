#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>

template<typename T>
class Singleton 
{
public:
	static T & getInstance()
	{
		static const std::unique_ptr<T> instance{ new T };
		return *instance;
	}

	Singleton(const Singleton&) = delete;
	Singleton & operator= (const Singleton) = delete;

protected:
	Singleton() {}
};

#endif // SINGLETON_H
