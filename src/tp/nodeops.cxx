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

#include <tp/nodeops.h>
#include <tp/transform.h>
#include <tp/projection.h>
#include <tp/camera.h>

#include <tp/log.h>
#include <tp/logutils.h>

struct tpLeafNodeCollector : tpTraverser {

	tpNodeArray& leafnodes;

	tpLeafNodeCollector(tpNodeArray& a) : leafnodes(a) {}

	unsigned char getDirection() const { return kDownward; }

	void enter(tpNode *node)
	{
		if (node->isLeaf() && !leafnodes.contains(node))
		{
			leafnodes.add(node);
		}
	}
};

struct tpNodePathCollector : tpTraverser {

	tpNodeArrayArray& nodepaths;
	tpNodeStack nodestack;

	tpNodePathCollector(tpNodeArrayArray& np) : nodepaths(np) {}

	unsigned char getDirection() const { return kUpward; }

	void enter(tpNode* node)
	{
		nodestack.push(node);
	}

	void leave(tpNode* node)
	{
		if (node->isRoot())
		{
			tpNodeArray nodepath;
			for (tpNodeStack::iterator i = nodestack.begin();
				 i != nodestack.end();
				 ++i)
			{
				nodepath.add(*i);
			}
			nodepaths.add(nodepath);
		}
		nodestack.pop();
	}
};

struct tpNodeTypeCollector : tpTraverser {

	tpNodeArray& typednodes;
	const tpRTTI* rtti;

	tpNodeTypeCollector(tpNodeArray& a,const tpRTTI* r) : typednodes(a), rtti(r) {}

	unsigned char getDirection() const { return kDownward; }

	void enter(tpNode *node)
	{
		if (node->getType()->isOfType(rtti) && !typednodes.contains(node))
		{
			typednodes.add(node);
		}
	}
};



/* static */
tpNodeArray
tpNodeOps::getLeafNodes(const tpNode *root)
{
	tpNodeArray result;

	tpLeafNodeCollector lnc(result);

	const_cast<tpNode*>(root)->traverse(lnc);

	return result;
}


/* static */
tpNodeArray
tpNodeOps::getNodesOfType(const tpNode *node, const tpRTTI* rtti)
{
	tpNodeArray result;

	tpNodeTypeCollector ntc(result,rtti);

	const_cast<tpNode*>(node)->traverse(ntc);

	return result;
}


/* static */
tpNodeArrayArray
tpNodeOps::getNodePaths(const tpNode *node)
{
	tpNodeArrayArray result;

	tpNodePathCollector npc(result);

	const_cast<tpNode*>(node)->traverse(npc);

	return result;
}

/* static */
tpNodeArrayArray
tpNodeOps::getNodePathsOfType(const tpNode *node,const tpRTTI* rtti)
{

	tpNodeArray typednodes;
	tpNodeTypeCollector ntc(typednodes,rtti);
	const_cast<tpNode*>(node)->traverse(ntc);


	tpNodeArrayArray result;

	for (tpNodeArray::iterator i = typednodes.begin();
		 i != typednodes.end();
		 ++i)

	{
		tpNodeArrayArray localresult;
		tpNodePathCollector npc(localresult);
		const_cast<tpNode*>(*i)->traverse(npc);

		result.append(localresult);
	}


	return result;
}

/* static */
tpNodeMatrixMap
tpNodeOps::getNodeMatrixMap(const tpNode *node,const tpRTTI* rtti)
{

	tpNodeMatrixMap result;
	tpNodeArray typednodes;

	tpNodeTypeCollector ntc(typednodes,rtti);
	const_cast<tpNode*>(node)->traverse(ntc);


	tpNodeArrayArray nodearray;

	for (tpNodeArray::iterator i = typednodes.begin();
		 i != typednodes.end();
		 ++i)

	{
		tpNodeArrayArray localresult;
		tpNodePathCollector npc(localresult);
		const_cast<tpNode*>(*i)->traverse(npc);

		localresult.reverse();

		nodearray.append(localresult);
	}

	for (tpNodeArrayArray::iterator i = nodearray.begin();
		 i != nodearray.end();
		 ++i)
	{
        tpMat44r m(tpMat44r::Identity());

		for (tpNodeArray::iterator j = (*i).begin();
			 j != (*i).end();
			 ++j)
		{
			tpTransform* t = (*j)->getType()->self_cast<tpTransform>(*j);
			if (t)
			{
				t->getMatrix(true,m);
			}
		}

		result.add((*i).front(),m);
	}

	return result;
}

tpNodeMatrixStackMap
tpNodeOps::getNodeMatrixStackMap(const tpNode *node, const tpRTTI *rtti)
{

	tpNodeMatrixStackMap result;
	tpNodeArray typednodes;

	tpNodeTypeCollector ntc(typednodes,rtti);
	const_cast<tpNode*>(node)->traverse(ntc);

	tpNodeArrayArray nodearray;

	for (tpNodeArray::iterator i = typednodes.begin();
		 i != typednodes.end();
		 ++i)

	{
		tpNodeArrayArray localresult;
		tpNodePathCollector npc(localresult);
		const_cast<tpNode*>(*i)->traverse(npc);

		localresult.reverse();

		nodearray.append(localresult);
	}

	for (tpNodeArrayArray::iterator i = nodearray.begin();
		 i != nodearray.end();
		 ++i)
	{
		tpMatrixStack matrixstack;

		for (tpNodeArray::iterator j = (*i).begin();
			 j != (*i).end();
			 ++j)
		{
			tpTransform* t = (*j)->getType()->self_cast<tpTransform>(*j);
			if (t)
			{
				t->getMatrix(true,matrixstack.model);
			}
			tpCamera* camera = (*j)->getType()->self_cast<tpCamera>(*j);
			if (camera)
			{
				matrixstack.projection = camera->getProjection();
				matrixstack.view = camera->getViewInverse();
			}
		}

		result.add((*i).front(),matrixstack);
	}

	return result;
}



