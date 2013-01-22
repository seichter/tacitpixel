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

#include "tp/ui.h"

#include "tp/text.h"
#include "tp/logutils.h"
#include "tp/module.h"

tpRefPtr<tpFont> tpUI::mFont;


tpNode* tpUI::Text(const tpString& str,tpPoint p)
{
	tpNode* textbox = new tpNode();

	tpText* text = new tpText();
	text->setFont(tpUI::mFont.get());
	text->set(str);

	tpVec3r aabbMin, aabbMax;
	text->getAABB(aabbMin,aabbMax);

	text->translate(tpVec3f(p.x - (aabbMax(0)/2),
							p.y - (aabbMax(1)/2),0.1));

	tpPrimitive* background = tpPrimitiveFactory::create(tpPrimitiveFactory::kPlane);

	background->scale(tpVec3f(aabbMax(0),aabbMax(1),1));
	background->translate(tpVec3f(p.x-aabbMax(0)/2,aabbMax(1)/2,0));

	background->setUniformColor(tpVec4f(.1,.1,.1,1));
	background->addRenderFlag(tpRenderFlag::kColorMaterial);

	textbox->addChild(background);
	textbox->addChild(text);


	return textbox;
}

tpNode* tpUI::Button(const tpString& label,tpRect area,const tpImage* icon)
{
	tpNode* button = new tpNode();
    tpPrimitive* background = tpPrimitiveFactory::create(tpPrimitiveFactory::kPlane);

	background->scale(tpVec3f(area.size.width,area.size.height,1));
	background->translate(tpVec3f(area.origin.x,area.origin.y,0));
	background->setUniformColor(tpVec4f(.1,.1,.1,1));
	background->addRenderFlag(tpRenderFlag::kColorMaterial);

	tpText* text = new tpText();
	text->setFont(tpUI::mFont.get());
	text->set(label);

	tpVec3r aabbMin, aabbMax;
	text->getAABB(aabbMin,aabbMax);

	tpLog::get() << aabbMax << "\n";

	text->translate(tpVec3f(area.origin.x + (area.size.width/2) - (aabbMax(0)/2),
							area.origin.y + (area.size.height/2) - (aabbMax(1)/2),0.1));



	button->addChild(background);
	button->addChild(text);

	return button;
}

bool
tpUI::setFont(const tpString &fontname)
{
	tpUI::mFont = new tpFont();
	tpUI::mFont->load(fontname);

	return true;
}
