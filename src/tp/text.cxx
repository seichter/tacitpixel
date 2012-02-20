#include "tp/text.h"
#include "tp/log.h"


tpText::tpText()
	: tpPrimitive()
{}

void tpText::set(const tpString& s)
{
	mText = s;
	if (mFont.isValid()) {
		mFont->text(mText,*this);
	} else {
		tpLogError("tpText::set can't be used until a font is set");
	}
}

void tpText::setFont(const tpString &name)
{
	tpFont* font = new tpFont();
	if (font->load(name)) {
		mFont = font;
	}
}
