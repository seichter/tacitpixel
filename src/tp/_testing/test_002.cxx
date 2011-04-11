
#include <tp/factory.h>
#include <tp/node.h>
#include <tp/log.h>

class tpMyNode : public tpNode {
public:
	tpMyNode() : tpNode("MyNode!") {};
};

static tpFactory<tpNode>::Registry<tpMyNode,200> gs_mynodefactory;


int main(int argc, char* argv[])
{
	
	tpLogMessage("Factory(tpNode) %d",tpFactory<tpNode>::get().getSize());

	
	return 0;
}