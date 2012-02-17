#include "tp/text.h"



tpText::tpText()
	: tpPrimitive()
{}

void tpText::set(const tpString& s)
{
	mText = s;
	mFont->text(mText,*this);
}

void tpText::setFont(const tpString &name)
{
	tpFont* font = new tpFont();
	if (font->load(name)) {
		mFont = font;
	}
}
