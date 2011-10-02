#include <tp/log.h>
#include <tp/library.h>
#include <tp/system.h>

#include <tp/refptr.h>
#include <tp/scopeptr.h>
#include <tp/image.h>
#include <tp/imageop.h>
#include <tp/timer.h>
#include <tp/node.h>
#include <tp/transform.h>
#include <tp/primitive.h>


class tpLogTraverser : public tpTraverser {
public:

	tpLogTraverser() : tpTraverser() {}

	void enter(tpNode* node) { tpLogMessage("enter node: %s",node->getName().c_str()); }

	void leave(tpNode* node) { tpLogMessage("leave node: %s",node->getName().c_str()); }


};

class tpMatTraverser : public tpTraverser {
public:

	tpMatTraverser() : tpTraverser() {}

	void enter(tpNode* node)
	{
		tpLogMessage("enter node: %s",node->getName().c_str());
		//
		if (node->getType()->isOfType(tpTransform::getTypeInfo()))
		{
			tpLogMessage("Transform of name '%s'",node->getName().c_str());
		}
	}

	void leave(tpNode* node)
	{
		tpLogMessage("leave node: %s",node->getName().c_str());
	}


};



int main(int argc, char* argv[])
{

	tpLogMessage("Plugin Path: %s",tpSystem::get()->getPluginPath().c_str());
	tpLogMessage("Executable Path: %s",tpSystem::get()->getExecutablePath().c_str());
	tpLogMessage("Resource Path: %s",tpSystem::get()->getResourcePath().c_str());
	tpLogMessage("CWD: %s",tpSystem::get()->getCWD().c_str());

	// field system
	tpRefPtr<tpNode> obj = new tpNode("My Root Node");

	tpFields fields = obj->getFields();
	for (tpFields::iterator iter = fields.begin(); iter != fields.end(); ++iter)
	{
		tpLogMessage("Field: %s",(*iter)->getName().c_str());
	}


	// primitives
	tpRefPtr<tpAttributeBuffer> attribBuffer = new tpAttributeBuffer();
	//attribBuffer->add(tpVec3r(3,3,3));



	// traverser
	obj->addChild(new tpNode("Child 1"));
	obj->addChild(new tpNode("Child 2"))->addChild(new tpNode("Child of Child 2"))->addChild(new tpTransform("A transform"));


	tpLogTraverser logtraverser;
	tpMatTraverser mattraverser;

	obj->traverse(mattraverser);


	return 0;
}
