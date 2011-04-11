#include <tp/log.h>
#include <tp/library.h>
#include <tp/system.h>

#include <tp/refptr.h>
#include <tp/scopeptr.h>
#include <tp/image.h>
#include <tp/timer.h>
#include <tp/node.h>


class tpLogTraverser : public tpTraverser {
public:
	
	tpLogTraverser() : tpTraverser() {}
	
	void push(tpNode* node) { tpLogMessage("push node: %s",node->getName().c_str()); }
		
	void pop(tpNode* node) { tpLogMessage("pop node: %s",node->getName().c_str()); }
	
	
};


int main(int argc, char* argv[])
{
	
	tpLogMessage("Plugin Path: %s",tpSystem::get()->getPluginPath().c_str());
	tpLogMessage("Executable Path: %s",tpSystem::get()->getExecutablePath().c_str());
	tpLogMessage("Resource Path: %s",tpSystem::get()->getResourcePath().c_str());
	tpLogMessage("CWD: %s",tpSystem::get()->getCWD().c_str());
	
	
	tpRefPtr<tpImage> image = new tpImage();
	
	image->allocate(320,240,TP_RGB888);
	
	tpScopePtr<tpImageOperator> op = tpImageOperator::create(tpImageOperator::SwapRedBlue);
	
	
	tpTimer t;
	for (int i = 0; i < 100; i++)
	{
		(*op)(*image.get());
	}
	
	// 
	tpLogMessage("Access: %3.0lfms",t.getElapsed(TP_TIME_MILLISEC));
	
	// field system
	
	tpRefPtr<tpNode> obj = new tpNode("My Root Node");
	
	tpFields fields = obj->getFields();
	for (tpFields::iterator iter = fields.begin(); iter != fields.end(); ++iter)
	{
		tpLogMessage("Field: %s",(*iter)->getName().c_str());
	}
	
	// traverser
	obj->addChild(new tpNode("Child 1"));
	obj->addChild(new tpNode("Child 2"))->addChild(new tpNode("Child of Child 2"));
	
	
	tpRefPtr<tpLogTraverser> logtraverser = new tpLogTraverser();
	
	obj->traverse(*logtraverser);
	
	
	getchar();
	
	
	return 0;
}