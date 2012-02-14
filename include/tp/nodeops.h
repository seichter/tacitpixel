#ifndef TP_NODEOPS_H
#define TP_NODEOPS_H

#include <tp/node.h>
#include <tp/stack.h>
#include <tp/map.h>
#include <tp/mat.h>


struct TP_API tpMatrixStack {

	tpMat44r model;
	tpMat44r view;
	tpMat44r projection;

	bool cameraOverride;

	tpMatrixStack()
		: model(tpMat44r::Identity())
		, view(tpMat44r::Identity())
		, projection(tpMat44r::Identity())
		, cameraOverride(false)
	{
	}
};

typedef tpStack<tpNode*> tpNodeStack;
typedef tpMap<tpNode*,tpMat44r> tpNodeMatrixMap;
typedef tpMap<tpNode*,tpMatrixStack> tpNodeMatrixStackMap;

/**
  * This implements some essential traversal techniques that
  * are used to dispatch the scenegraph into the rendering
  * hardware.
  */
struct TP_API tpNodeOps {

	/**
	  * @brief traverses a node and returns all of it leafs
	  * @param node node to be traversed
	  * @param array of all leaf nodes
	  */
	static tpNodeArray getLeafNodes(const tpNode* node);

	/**
	  * @brief traverses a node to collect all children of a certain type
	  * @param node node to be traversed
	  * @param rtti
	  * @return array of all nodes in the sub tree (sparse)
	  */
	static tpNodeArray getNodesOfType(const tpNode* node, const tpRTTI* rtti);

	/**
	  * @brief traverses a node to collect all nodepathes
	  * @param node node to be traversed
	  * @return array of all nodes and pathes to root
	  */
	static tpNodeArrayArray getNodePaths(const tpNode *node);

	/**
	  * Collect nodes of a certain type in the subgraph and retrieve their
	  * nodepaths back.
	  * @param node node to start from
	  * @param rtti runtime type information to be used
	  * @param array of all nodes and their pathes to the root
	  */
	static tpNodeArrayArray getNodePathsOfType(const tpNode *node, const tpRTTI *rtti);

	static tpNodeMatrixMap getNodeMatrixMap(const tpNode *node, const tpRTTI *rtti);

	static tpNodeMatrixStackMap getNodeMatrixStackMap(const tpNode *node, const tpRTTI *rtti);
};


#endif
