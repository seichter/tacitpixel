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

void tpEventHandler::removeAll()
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
    removeAll();
}


TP_TYPE_REGISTER_BASE(tpEvent,Event);


#include <tp/log.h>


class tpSpecialEvent : public tpEvent {

public:
	TP_TYPE_DECLARE

};
TP_TYPE_REGISTER(tpSpecialEvent,tpEvent,SpecialEvent);

void SpecialEventHandler(tpSpecialEvent& e)
{
	tpLogNotify("Special Event!");
	e.setHandled();
}

void SomeEventHandler(tpEvent& e)
{
	tpLogNotify("Here we go!");
}

struct tpMyEventHandler {

	void onSpecialEvent(tpSpecialEvent& e) {
		tpLogNotify("Special Event in method");
	}
};

void tpEventHandlerTest()
{
	tpEventHandler gah;
	tpEvent blah;
	tpSpecialEvent spec;
	tpMyEventHandler meh;

	gah.attach<tpSpecialEvent>(SpecialEventHandler);
	gah.attach<tpSpecialEvent,tpMyEventHandler>(&meh,&tpMyEventHandler::onSpecialEvent);


	gah.process(blah);

	gah.process(spec);


}

