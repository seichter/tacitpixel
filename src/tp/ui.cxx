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

//	tpImage* img = new tpImage();
//	img->allocate(64,64,tpPixelFormat::kGray_8);

//	for (tpUInt r = 0; r < img->getHeight();++r)
//		for (tpUInt c = 0; c < img->getWidth();++c)
//		{
//			static_cast<char*>(img->getData())[r*c+c] = r;
//		}

//	tpTexture* bgtxt = new tpTexture();
//	bgtxt->setImage(img);
//	bgtxt->setWrapMode(tpTexture::kWrapModeRepeat,tpTexture::kWrapModeRepeat);

//	background->setTexture(bgtxt);

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
