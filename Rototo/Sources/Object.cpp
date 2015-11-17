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
	count++;
}

void Object::Release()
{
	count--;
	if (count == 0)
	{
		delete this;
	}
}