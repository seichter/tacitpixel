#include "tp/widget.h"


tpWidget::tpWidget()
    : tpReferenced()
    , mId((tpUInt)-1)
{
}

tpWidget::tpWidget(tpWidget *parent, const tpString &name, tpUInt id)
    : tpReferenced()
    , mId(id)
{
    if (parent) parent->add(this);
}

void
tpWidget::add(tpWidget *other)
{
    if (other) mNested.add(other);
}

void
tpWidget::remove(tpWidget *other)
{
    if (other) {
        tpUInt idx = mNested.find(other);
        mNested.erase(idx);
    }
}
