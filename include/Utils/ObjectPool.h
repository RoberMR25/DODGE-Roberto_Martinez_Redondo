#pragma once

#include <array>
#include <cassert>
#include <list>

template <typename T, size_t N>
class ObjectPool
{
public:
	ObjectPool();
	~ObjectPool();

	size_t getCapacity() const { return m_elements.size(); }	// max number of elements in the pool
	size_t getSize() const { return m_elements.size() - m_availableElements.size(); }	// number of elements currently in use

	T& get();
	void release(T& element);

private:
	std::array<T, N> m_elements;	// T m_elements[N];
	std::list<T*> m_availableElements;

};

template <typename T, size_t N>
ObjectPool<T, N>::ObjectPool()
{
	for (T& element : m_elements)
		m_availableElements.emplace_back(&element);
}

template <typename T, size_t N>
ObjectPool<T, N>::~ObjectPool()
{
	assert(m_elements.size() == m_availableElements.size() && "Missing elements in the pool prior to its destruction");
}

template <typename T, size_t N>
T& ObjectPool<T, N>::get()
{
	assert(m_availableElements.size() > 0 && "There are no elements available in the pool, please increase its capacity");
	T* availableElement = m_availableElements.front();
	m_availableElements.pop_front();

	return *availableElement;
}

template <typename T, size_t N>
void ObjectPool<T, N>::release(T& element)
{
	assert(m_availableElements.size() < m_elements.size() && "Pool is already full");
	m_availableElements.push_back(&element);
}