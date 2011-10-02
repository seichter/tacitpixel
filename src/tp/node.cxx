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

#include <tp/node.h>
#include <tp/log.h>
#include <tp/module.h>


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

tpObject*
tpNode::clone()
{
	return new tpNode(*this);
}

void
tpNode::traverse(tpTraverser& traverser)
{
	traverser.enter(this);

	if (traverser.getDirection() == tpTraverser::kDownward)
	{

		for (tpArray<tpRefPtr<tpNode> >::iterator iter = m_children.begin();
			 iter != m_children.end();
			 ++iter)
		{
			(*iter).get()->traverse(traverser);
		}

	} else {
		for (tpArray<tpNode*>::iterator iter = m_parents.begin();
			 iter != m_parents.end();
			 ++iter)
		{
			(*iter)->traverse(traverser);
		}
	}

	traverser.leave(this);
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


TP_TYPE_REGISTER(tpNode,tpObject,Node);


tpNode* tpNode::read(const tpString& file)
{
	tpNode* result(0);

	if (file.isEmpty()) return 0;

	const tpModuleList& ml = tpModuleManager::get()->getModules();

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





