/*
 * Copyright (C) 1999-2013 Hartmut Seichter
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

#include <tp/event.h>


tpEvent::tpEvent()
	: mSender(0)
	, mHandled(false)
	, mTick(0)
    , mId(0)
{
	tpTimer::getCurrentTick(mTick);
}

tpEvent::tpEvent(const tpEvent& e) {
	*this = e;
}

tpEvent&
tpEvent::operator = (const tpEvent& e) {
	if (this != &e) {
		mSender = e.mSender;
		mHandled = e.mHandled;
		mTick = e.mTick;
        mId = e.mId;
	}

	return *this;
}

tpEvent::~tpEvent() {

}



bool
tpEventHandler::process(tpEvent& e)
{
	for (tpEventProxyArray::iterator it = mHandlers.begin();
		 it != mHandlers.end() && !e.getHandled();
		 ++it)
	{
		if ((*it)->corresponds(e)) {
			(*it)->call(this,e);
		}
	}

    return e.getHandled();
}

void tpEventHandler::flush()
{
    for (tpEventProxyArray::iterator it = mHandlers.begin();
         it != mHandlers.end();
         ++it)
    {
        delete (*it);
    }
    mHandlers.clear();
}


tpEventHandler::~tpEventHandler()
{
    flush();
}


TP_TYPE_REGISTER_BASE(tpEvent,Event);
