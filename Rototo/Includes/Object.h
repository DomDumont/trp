#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <atomic>

class Object
{
public:

	
	Object();
	Object(const Object &other);
	void operator=(const Object & other);

	virtual ~Object() = 0;

	int GetReferenceCount() const;

	virtual void AddRef();

	virtual void Release();

private:

	std::atomic<int> count;

};


#endif