
#include <tp/node.h>
#include <tp/ioutils.h>
#include <tp/file.h>
#include <tp/xml.h>
#include <tp/module.h>
#include <tp/version.h>
#include <tp/log.h>
#include <tp/map.h>
#include <tp/stringtokenizer.h>
#include <tp/primitive.h>

struct tpFloatArray {
	tpArray<float> array;
	tpString identifier;
	tpUByte stride;
};

struct tpInstanceGeometry {
	tpString identifier;
	tpRefPtr<tpPrimitive> geometry;
};

inline void
tpStringArrayToFloatArray(const tpStringArray& sa, tpArray<float>& out)
{
	out.clear(); out.reserve(sa.getSize());

	for (tpStringArray::const_iterator it = sa.begin();
		 it != sa.end();
		 ++it)
	{
		out.add(it->toFloat());
	}
}



typedef tpArray< tpFloatArray > tpFloatArrayArray;

class tpColladaParserCallback : public tpXMLNodeCallback {
public:

	tpFloatArrayArray mFloatArrayCache;


	void onFloatArray(const tpXML& xml)
	{

		tpFloatArray fa;

		for (tpXML::Attributes::const_iterator a = xml.getCurrentAttributes().begin();
			 a != xml.getCurrentAttributes().end();
			 ++a)
		{
			if (a->getKey() == "id")
			{
				fa.identifier = a->getValue();
			}
		}

		tpStringArray sa = tpStringTokenizer::split(xml.getCurrentDataNode()," ");

		tpStringArrayToFloatArray(sa,fa.array);

		tpString path = tpString::join(xml.getCurrentPath(),".");

		tpLogNotify("(%s) FloatArray %s %d",path.c_str(),fa.identifier.c_str(),fa.array.getSize());

		mFloatArrayCache.add(fa);

	}

	void operator ()(const tpXML& xml)
	{
#if 0
		for (tpXML::Path::const_iterator p = xml.getCurrentPath().begin();
			 p != xml.getCurrentPath().end();
			 p++)
		{
			printf("/%s",p->c_str());
		}

		printf("\n\tData '%s'\n",xml.getCurrentDataNode().c_str());

		for (tpXML::Attributes::const_iterator a = xml.getCurrentAttributes().begin();
			 a != xml.getCurrentAttributes().end();
			 ++a)
		{
			printf("\t%s=%s\n",a->getKey().c_str(),a->getValue().c_str());
		}
#else

		if (xml.getCurrentPath().back() == "float_array")
		{
			onFloatArray(xml);
		}

		// instance_geometry
		//if (xml.getCurrentPath().back() == "")

//		for (tpXML::Path::const_iterator p = xml.getCurrentPath().begin();
//			 p != xml.getCurrentPath().end();
//			 p++)
//		{
//			printf("/%s",p->c_str());
//		}


#endif
	}
};


class tpColladaNodeHandler : public tpNodeHandler {
public:

	TP_TYPE_DECLARE

	tpColladaNodeHandler()
		: tpNodeHandler()
	{
		tpLogNotify("%s Collada support",tpGetVersionString());
	}

	virtual tpUByte getCapabilities(const tpString& name) {
		if (name.afterLast('.') == "dae") {
			return kRead;
		}
		return kNone;
	}

	virtual tpNode* read(const tpString& name)
	{
		tpFile file;
		if (file.open(name,"rb"))
		{
			tpString dae = tpIOUtility::asString(&file);

			file.close();

			tpRefPtr<tpXML> xml = new tpXML();

			tpColladaParserCallback cb;

			xml->setCallback(&cb);
			xml->parse(dae);

		}

		return 0;
	}

};


TP_TYPE_REGISTER(tpColladaNodeHandler,tpNodeHandler,NodeHandlerCollada);
tpModuleInitializer<tpColladaNodeHandler> g_nodefactory_dae;


