#ifndef __EVENT_H__
#define __EVENT_H__

#include <memory> // for unique_ptr
class Event_p;

class Event
{
public:
	Event();
public: //TODO change this
	std::unique_ptr<Event_p> event_p; // opaque type here
};

#endif