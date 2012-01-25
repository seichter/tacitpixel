#ifndef TP_EVENT_H
#define TP_EVENT_H

#include <tp/referenced.h>
#include <tp/rtti.h>
#include <tp/array.h>
#include <tp/timer.h>

#include <tp/globals.h>
#include <tp/types.h>

class TP_API tpEvent {
protected:

	tpReferenced* mSender;
	bool mHandled;
	tpTimerTick mTick;

public:

	TP_TYPE_DECLARE

	tpEvent();

	inline
	void setHandled(bool handled = true) {
		mHandled = handled;
	}

	inline
	bool getHandled() const { return mHandled; }

};

class tpEventHandler;

struct TP_API tpEventProxy {
	virtual void call(tpEventHandler*, tpEvent&) = 0;
	virtual bool corresponds(tpEvent& e) const = 0;
};

template <typename EventType>
struct tpEventFunctionProxy : tpEventProxy {

	void (*mHandler)(EventType& e);

	tpEventFunctionProxy(void (*f)(EventType& e)) : mHandler(f) {}

	virtual void call(tpEventHandler*, tpEvent& e) {
		mHandler(static_cast<EventType&>(e));
	}

	virtual bool corresponds(tpEvent &e) const { return (e.getType() == EventType::getTypeInfo()); }

};


template <typename EventType,typename EventClass>
struct tpEventFunctorProxy : tpEventProxy {

	typedef void (EventClass::*EventCallback)(EventType&);

	EventCallback mHandler;
	EventClass* mObject;

	inline
	tpEventFunctorProxy(EventCallback method,EventClass* o)
		: mHandler(method)
		, mObject(o)
	{}

	virtual void call(tpEventHandler* h, tpEvent& e) {
		(mObject->*mHandler)(static_cast<EventType&>(e));
	}

	virtual bool corresponds(tpEvent &e) const { return (e.getType() == EventType::getTypeInfo()); }

};


class TP_API tpEventHandler {
protected:

	typedef tpArray<tpEventProxy*> tpEventProxyArray;
	tpEventProxyArray mHandlers;

public:

	template <typename EventType>
	void attach(void (*f)(EventType& e))
	{
		mHandlers.add(new tpEventFunctionProxy<EventType>(f));
	}

	template <typename EventType,typename EventClass>
	void attach(EventClass* o,void (EventClass::*f)(EventType& e))
	{
		mHandlers.add(new tpEventFunctorProxy<EventType,EventClass>(f,o));
	}

	bool process(tpEvent &e);
};


void TP_API tpEventHandlerTest();

#endif
