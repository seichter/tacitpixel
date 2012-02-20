

#include <tp/xml.h>
#include <tp/refptr.h>
#include <tp/file.h>

void tpIO2String(tpIO* io,tpString& s)
{
	tpArray<tpChar> buffer;
	buffer.resize(1024);

	while(io->isGood()) {
		io->read(buffer.getData(),buffer.getSize());

		tpString tmp; tmp.set(buffer.getData(),io->getCount());

		s += tmp;
	}
}

class MyCallback : public tpXMLNodeCallback {
	void operator ()(const tpXML& xml)
	{
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
	}
};

int main(int argc, char* argv[])
{

	tpRefPtr<tpXML> xml = new tpXML();

	xml->setCallback(new MyCallback());

	tpString doc;

	if (argc > 1) {
		tpFile file;
		if (file.open(argv[1])) tpIO2String(&file,doc);
	} else {

		doc = "<we are=\"cool\"><some stuff=\"yes\">And also some text<tag a=\"b\"/></some><br/></we>";
	}

	xml->parse(doc);

	return 0;
}
