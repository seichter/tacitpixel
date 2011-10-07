
#include <tp/traverser.h>
#include <tp/node.h>
#include <tp/transform.h>
#include <tp/log.h>
#include <tp/stringformater.h>
#include <tp/stack.h>
#include <tp/nodeops.h>
#include <tp/primitive.h>
#include <tp/camera.h>


void report(const tpNodeArray& nodes)
{
	for (tpNodeArray::const_iterator i = nodes.begin();
		 i != nodes.end();
		 ++i)
	{
		printf("%s, ",(*i)->getName().c_str());
	}

	printf("\n");
}


void report(const tpNodeArrayArray& nodepaths)
{
	for (tpNodeArrayArray::const_iterator i = nodepaths.begin();
		 i != nodepaths.end();
		 ++i)
	{
		report(*i);

		printf("\n");
	}
}

void report(const tpNodeMatrixMap& nodemap)
{
	for (tpNodeMatrixMap::const_iterator i = nodemap.begin();
		 i != nodemap.end();
		 ++i)
	{
		tpString mStr; mStr << (*i).getValue();

		printf("%s %s",(*i).getKey()->getName().c_str(),mStr.c_str());

		printf("\n");
	}
}


void report(const tpMat44r& mat)
{
	tpString mStr; mStr << mat;
	printf("%s",mStr.c_str());
}

int main(int argc, char* argv[])
{
	tpRefPtr<tpNode> leaf = new tpNode(); leaf->setName("Leaf");
	tpRefPtr<tpNode> root = new tpNode(); root->setName("Root");

	tpRefPtr<tpTransform> tfn_1 = new tpTransform(); tfn_1->setName("Transform 1");
	tpRefPtr<tpTransform> tfn_2 = new tpTransform(); tfn_2->setName("Transform 2");

	tpRefPtr<tpPrimitive> prim = new tpPrimitive(); prim->setName("Primitive");

	tpMat44r m;
	m.setTranslation(1,2,3);
	tfn_1->setMatrix(m);
	m.setTranslation(4,5,6);
	tfn_2->setMatrix(m);

	root->addChild(tfn_1.get());
	tfn_1->addChild(tfn_2.get());

	tfn_1->addChild(leaf.get());
	tfn_2->addChild(leaf.get());

	// add a primitive
	tfn_2->addChild(prim.get());

	tpRefPtr<tpCamera> camera = new tpCamera();
	camera->setProjectionPerspective(60.0f,1.3f,0.1f,1000.0f);
	camera->setViewLookAt(tpVec3r(2,2,2),tpVec3r(0,0,0),tpVec3r(0,1,0));

	tpMat44r mcam = camera->getViewInverse();
	report(mcam);
	mcam = camera->getView();
	report(mcam);
	mcam = camera->getProjection();
	report(mcam);


	tpNodeArray nodes = tpNodeOps::getNodesOfType(root.get(),tpTransform::getTypeInfo());
	printf("Transforms: %d\n",nodes.getSize());

	nodes = tpNodeOps::getNodesOfType(root.get(),tpNode::getTypeInfo());
	printf("Nodes: %d\n",nodes.getSize());

	nodes = tpNodeOps::getNodesOfType(root.get(),tpPrimitive::getTypeInfo());
	printf("Primitives: %d\n",nodes.getSize());

	//
	tpNodeArrayArray nodepaths = tpNodeOps::getNodePaths(leaf.get());
	//report(nodepaths);

	nodepaths = tpNodeOps::getNodePathsOfType(root.get(),tpPrimitive::getTypeInfo());
	report(nodepaths);

	tpNodeMatrixMap nodemap = tpNodeOps::getNodeMatrixMap(root.get(),tpPrimitive::getTypeInfo());
	report(nodemap);


	return 0;
}
