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
		for (tpRefNodeArray::iterator iter = mChildren.begin();
			 iter != mChildren.end();
			 ++iter)
		{
			(*iter).get()->traverse(traverser);
		}
	} else {

		for (tpNodeArray::iterator iter = mParents.begin();
			 iter != mParents.end();
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
		mChildren.add(node);
		node->mParents.add(this);
	}
	return node;
}

//! remove child node
tpNode*
tpNode::removeChild(tpNode* node)
{
	if (node)
	{
		tpSizeT idx_c = mChildren.find(node);
		tpSizeT idx_p = node->mParents.find(this);

		mChildren.erase(mChildren.begin() + idx_c);
		node->mParents.erase(node->mParents.begin() + idx_p);
	}
	return node;
}


void
tpNode::setUpdateFlags(tpUInt flag,bool add /*= true*/)
{
	// transform needs to be promoted downwards
	if (flag & kUpdateTransform)
	{
		for (tpRefNodeArray::iterator i = mChildren.begin();
			 i != mChildren.end();
			 ++i)
		{
			tpUInt flag = (*i)->getUpdateFlags() | kUpdateTransform;
			(*i)->setUpdateFlags(flag);
		}
	}

	if (flag & kUpdateBound)
	{
		for (tpNodeArray::iterator i = mParents.begin();
			 i != mParents.end();
			 ++i)
		{
			tpUInt flag = (*i)->getUpdateFlags() | kUpdateTransform;
			(*i)->setUpdateFlags(flag);
		}
	}
}

TP_TYPE_REGISTER(tpNode,tpObject,Node);


tpNode* tpNode::read(const tpString& file)
{
	tpNode* result(0);

	tpLogNotify("%s %d '%s'",__FUNCTION__,__LINE__,file.c_str());

	if (file.isEmpty()) return 0;

	const tpModuleList& ml = tpModuleManager::get()->getModules();

	for (tpSizeT i = 0; i < ml.getSize(); i++)
	{
		tpRefPtr<tpReferenced> item = ml[i];

		if (item->getType()->isOfType(tpNodeHandler::getTypeInfo()))
		{
			tpLogNotify("%s found %s",__FUNCTION__,item->getType()->getName());

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





