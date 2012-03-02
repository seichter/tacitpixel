/*
 * Copyright (C) 1999-2011 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef TP_EVENT_H
#define TP_EVENT_H

#include <tp/referenced.h>
#include <tp/rtti.h>
#include <tp/array.h>
#include <tp/timer.h>

#include <tp/globals.h>
#include <tp/types.h>

/** forward declaration */
class tpEventHandler;


/**
  * Base class for event handling
  *
  */
class TP_API tpEvent {
protected:

	tpReferenced* mSender;
	bool mHandled;
	tpTimerTick mTick;

	tpULong mId;

public:

	TP_TYPE_DECLARE

	/** Default c'tor */
	tpEvent();

	/** Copy c'tor */
	tpEvent(const tpEvent& e);

	/** Assignment operator */
	tpEvent& operator = (const tpEvent& e);

	/** D'tor */
	virtual ~tpEvent();

	void setId(tpULong id) { mId = id; }
	tpULong getId() { return mId; }

	inline void
	setHandled(bool handled = true) { mHandled = handled; }

	inline bool
	getHandled() const { return mHandled; }

};


struct TP_API tpEventProxy {
	virtual void call(tpEventHandler*, tpEvent&) = 0;
	virtual bool corresponds(tpEvent& e) const = 0;

    virtual ~tpEventProxy() {}
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

    void removeAll();

    ~tpEventHandler();
};


#endif
