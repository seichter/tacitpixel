#include <tp/event.h>
#include <tp/referenced.h>
#include <tp/rtti.h>
#include <tp/array.h>
#include <tp/map.h>
#include <tp/sort.h>
#include <tp/timer.h>

class tpEvent {
protected:
    tpReferenced* mSender;
    bool mHandled;
    tpTimerTick mTick;

public:

    TP_TYPE_DECLARE

    tpEvent();

    inline
    void setHandled(bool handled=true) {
        mHandled = handled;
    }

    inline
    bool getHandled() const { return mHandled; }



};

tpEvent::tpEvent()
    : mSender(0)
    , mHandled(false)
    , mTick(0)
{
    tpTimer::getCurrentTick(mTick);
}


TP_TYPE_REGISTER_BASE(tpEvent,Event);

typedef void (*tpEventHandlerType)(tpEvent&);

class tpEventHandler {

    typedef tpArray<tpEvent> tpEventsArray;
    typedef tpMap<tpULong,tpEventHandlerType> tpEventHandlersArray;

    tpEventsArray mEvents;
    tpEventHandlersArray mHandlers;

public:


    void t()
    {
        for (tpEventsArray::iterator eit = mEvents.begin();
             eit != mEvents.end();
             eit++)
        {
            for (tpEventHandlersArray::iterator hit = mHandlers.begin();
                 hit != mHandlers.end();
                 hit++)
            {
                if (hit->getKey() == eit->getTypeInfo()->getID())
                {
                    (*eit).setHandled(true);
                    (*hit).getValue()((*eit));
                }
            }
        }
    }

    void purge()
    {
    }




//    template <typename EventType>
//    void attach(EventType& e)
//    {
//    }

//    template <typename EventType>
//    void detach(EventType& e)
//    {
//    }

protected:

};

