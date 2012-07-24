#include <tp/xml.h>

#define RAPIDXML_NO_EXCEPTIONS 1

#include "rapidxml/rapidxml.hpp"

#include <tp/log.h>
#include <tp/refptr.h>

void rapidxml::parse_error_handler(const char *what, void *where)
{
	tpLogError("tpXML error %s",what);
}


class tpXML;


/*
 * This thing is utterly and completely not threadsafe!
 */
void rapidxml_dispatch(tpXML& xml,rapidxml::xml_node<>* node)
{
	//tpLogNotify("Node %s '%s'",node->name(),node->value());

	xml.getCurrentDataNode() = tpString(node->value());
	xml.getCurrentPath().add(tpString(node->name()));

	xml.getCurrentAttributes().clear();

	// push attributes
	for (rapidxml::xml_attribute<>* attr = node->first_attribute();
		 attr;
		 attr = attr->next_attribute())
	{
		xml.getCurrentAttributes().add(tpString(attr->name()),tpString(attr->value()));
		//tpLogNotify("\tAttribute %s = %s",attr->name(),attr->value());
	}

	xml.callback();

	// iterate over child nodes
	for (rapidxml::xml_node<>* child = node->first_node();
		 child;
		 child = child->next_sibling())
	{
		rapidxml_dispatch(xml,child);
	}

	xml.getCurrentPath().removeEnd();
}

tpXML::tpXML()
	: tpReferenced()
	, mCallback(0)
{
}

void
tpXML::parse(const tpString& doc)
{
	// we need a copy
	mDoc = doc;

	using namespace rapidxml;

	// now parse
	xml_document<> xmldoc;
	xmldoc.parse<parse_no_data_nodes>(mDoc.c_str());

	rapidxml_dispatch(*this,xmldoc.first_node());

	// sanity check
	this->mAttributes.clear();
	this->mData.empty();
	this->mPath.clear();

}

void tpXML::callback() const
{
	if (mCallback) (*mCallback)(*this);
}
