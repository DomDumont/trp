#include "Object.h"



Object::Object()
	: count(1)
{
}

Object::Object(const Object & /*other*/)
	: count(1) // Always start with a reference count of 1.
{
}

void Object::operator= (const Object & other)
{
	this->count = 1;
}

Object::~Object()
{
}

int Object::GetReferenceCount() const
{
	return count;
}

void Object::AddRef()
{
	count.fetch_add(1, std::memory_order_relaxed);
}

void Object::Release()
{

	if (count.fetch_sub(1, std::memory_order_release) == 1)
	{
		std::atomic_thread_fence(std::memory_order_acquire);
		delete this;
	}
}