#include <tp/primitive.h>

#include <tp/mutex.h>
#include <tp/scopelock.h>

#include <tp/node.h>
#include <tp/module.h>



int main(int argc, char* argv[]) {

	tpModuleManager::get()->load("dae");

	tpNode::read(argv[1]);

	return 0;
}
