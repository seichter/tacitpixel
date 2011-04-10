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

#include <tp/node.h>
#include <tp/log.h>

template <class T> class tpRenderInterface
{
public:

	tpRenderInterface(T* object){};

protected:

	T* m_object;

};


tpNode::tpNode(const tpString& name)
	: tpObject(name)
{
}

tpNode::tpNode(const tpNode& node)
	: tpObject(node)
{
}

tpNode::~tpNode() 
{
}

tpObject* tpNode::clone()
{
	return new tpNode(*this);
}


tpVoid tpNode::traverse( tpTraverser* traverse, tpContext& context )
{
//	context.push(this);
//	traverse->preAction(this,context);
//	doActions(m_actions,&context);
//	traverse->postAction(this,context);
//	context.pop();
}

//! add a child node
void 
tpNode::addChild(tpNode* node)
{
	m_children.add(node);
	node->m_parents.add(this);
}

//! remove child node
void 
tpNode::removeChild(tpNode* node)
{
	tpSizeT idx_c = m_children.find(node);
	tpSizeT idx_p = node->m_parents.find(this);
	
	m_children.erase(m_children.begin() + idx_c);
	node->m_parents.erase(node->m_parents.begin() + idx_p);
}

//! set child node
bool setChild(tpSizeT idx,tpNode* node)
{
	return false;
}


/*

void tpNode::setOpaquePtr(tpSizeT opaquesize, tpRawPtr opaqueptr)
{
	m_opaque = opaqueptr;
	m_opaquesize = opaquesize;
}

tpRawPtr tpNode::getOpaquePtr()
{
	return m_opaque;
}


tpSizeT tpNode::getOpaquePtrSize() const
{
	return m_opaquesize;
}


tpVoid tpNode::setTraverseFlags(tpUInt flags) {
	this->m_traverserflags = flags;
}
*/


TP_TYPE_REGISTER(tpNode,tpObject,Node);


#if 0

tpNode* tpNode::load(const tpString& file)
{
	tpNode* result(0);

	if (file.isEmpty()) return 0;

	const tpModuleList& ml = tpModuleManager::get()->getModules();
	
	// add the path to the list
	tpFilePathList& fpl = tpSystem::get()->getFileSearchPaths();
	tpString relpath = file.beforeLast(*tpPathSep[0]);
	fpl.add(relpath);

	

	for (int i = 0; i < ml.getSize(); i++)
	{
		const tpRefPtr<tpReference>& item = ml[i];

		if (item->getType()->isOfType(tpNodeFactory::getTypeInfo()))
		{
			//tpLogNotify("%s found %s",__FUNCTION__,item->getType()->getName().c_str());

			tpNodeFactory* nf = reinterpret_cast<tpNodeFactory*>(item.get());

			if (nf && nf->getCapabilities(TP_NODE_CAN_READ,file)) 
			{
				result = nf->read(file);

			} else {
				//tpLogNotify("%s '%s' can't read %s",__FUNCTION__,
				//	item->getType()->getName().c_str(),
				//	file.c_str());
			}
		}
	}
	
	// remove the relative path from the list
	fpl.pop();
	
	return result;

}


//////////////////////////////////////////////////////////////////////////

tpNodeFactory::tpNodeFactory() : tpReference()
{

}

tpNodeFactory::~tpNodeFactory()
{
}

TP_TYPE_REGISTER(tpNodeFactory,tpReference,NodeFactory);

#endif





