#include <tp/nodeops.h>

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



