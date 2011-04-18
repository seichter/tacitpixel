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
#include <tp/module.h>

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


void 
tpNode::traverse(tpTraverser& traverser)
{
	traverser.push(this);
	for (tpArray<tpRefPtr<tpNode> >::iterator iter = m_children.begin();
		 iter != m_children.end();
		 ++iter)
	{
		(*iter).get()->traverse(traverser);
	}	
	traverser.pop(this);
}


//! add a child node
tpNode* 
tpNode::addChild(tpNode* node)
{
	if (node)
	{
		m_children.add(node);
		node->m_parents.add(this);
	}
	return node;
}

//! remove child node
tpNode* 
tpNode::removeChild(tpNode* node)
{
	if (node)
	{
		tpSizeT idx_c = m_children.find(node);
		tpSizeT idx_p = node->m_parents.find(this);
		
		m_children.erase(m_children.begin() + idx_c);
		node->m_parents.erase(node->m_parents.begin() + idx_p);
	}
	return node;
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


tpNode* tpNode::read(const tpString& file)
{
	tpNode* result(0);

	if (file.isEmpty()) return 0;

	const tpModuleList& ml = tpModuleManager::get()->getModules();
	
	//// add the path to the list
	//tpFilePathList& fpl = tpSystem::get()->getFileSearchPaths();
	//tpString relpath = file.beforeLast(*tpPathSep[0]);
	//fpl.add(relpath);

	

	for (tpSizeT i = 0; i < ml.getSize(); i++)
	{
		tpRefPtr<tpReferenced> item = ml[i];

		if (item->getType()->isOfType(tpNodeHandler::getTypeInfo()))
		{
			//tpLogNotify("%s found %s",__FUNCTION__,item->getType()->getName().c_str());

			tpNodeHandler* nf = reinterpret_cast<tpNodeHandler*>(item.get());

			if (nf && (tpNodeHandler::kRead & nf->getCapabilities(file))) 
			{
				result = nf->read(file);

			}
		}
	}
	
	return result;

}


//////////////////////////////////////////////////////////////////////////

tpNodeHandler::tpNodeHandler() : tpReferenced()
{

}

tpNodeHandler::~tpNodeHandler()
{
}

TP_TYPE_REGISTER(tpNodeHandler,tpReferenced,NodeHandler);





