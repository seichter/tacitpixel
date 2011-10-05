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


class tpImageHandler_Null : public tpImageHandler
{
public:

	TP_TYPE_DECLARE;

	bool getCapability(tpUInt capability,const tpString& name);

	tpImageHandler_Null();

	virtual tpImage* read(const tpString& name);

	virtual bool write(const tpImage *img, const tpString &name);
};

//////////////////////////////////////////////////////////////////////////

tpImageHandler_Null::tpImageHandler_Null()
	: tpImageHandler()
{
	tpLogNotify("%s null image support",tpGetVersionString());
}

bool
tpImageHandler_Null::getCapability( tpUInt capability,const tpString& name )
{
	return false;
}

tpImage*
tpImageHandler_Null::read( const tpString& name )
{
	return 0L;
}

bool
tpImageHandler_Null::write(const tpImage* img, const tpString& name)
{
	return false;
}

TP_TYPE_REGISTER(tpImageHandler_Null,tpImageHandler,ImageFactoryNull);

tpModuleInitializer<tpImageHandler_Null> g_imagefactory_null;

