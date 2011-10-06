/*
* Twisted Pair Visualization Engine
*
* Copyright (c) 1999-2009 Hartmut Seichter 
* 
* This library is open source and may be redistributed and/or modified under  
* the terms of the Twisted Pair License (TPL) version 1.0 or (at your option) 
* any later version. The full license text is available in the LICENSE file 
* included with this distribution, and on the technotecture.com website.
*
*/

#include <tp/imagehandler.h>
#include <tp/image.h>
#include <tp/log.h>
#include <tp/module.h>
#include <tp/version.h>


class tpImageHandler_TGA : public tpImageHandler {
public:

	TP_TYPE_DECLARE;

	bool getCapability(tpUInt capability,const tpString& name);

	tpImageHandler_TGA();

	virtual tpImage* read(const tpString& name);
};

//////////////////////////////////////////////////////////////////////////

tpImageHandler_TGA::tpImageHandler_TGA() : tpImageHandler()
{
	tpLogNotify("%s TGA image support",tpGetVersionString());
}

bool tpImageHandler_TGA::getCapability( tpUInt capability,const tpString& name )
{
	switch (capability)
	{
		case kCanRead:
			return name.afterLast('.') == "tga";
			break;
		default:
			break;
	}
	return false;
}

tpImage* tpImageHandler_TGA::read( const tpString& name )
{
	return 0;
}


TP_TYPE_REGISTER(tpImageHandler_TGA,tpImageHandler,ImageFactoryBMP);

tpModuleInitializer<tpImageHandler_TGA> g_imagefactory_bmp;

