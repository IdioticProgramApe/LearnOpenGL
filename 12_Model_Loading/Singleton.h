#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>

template<typename T>
class Singleton
{
public:
	static std::shared_ptr<T> getInstance()
	{
		static std::shared_ptr<T> instance{ new T }; // Guaranteed to be destroyed. Instantiated on first use.
		return instance;
	}

	Singleton() = default;

	Singleton(Singleton const&) = delete;
	void operator=(Singleton const&) = delete;
};

#endif // SINGLETON_H
