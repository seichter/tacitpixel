
#include <tp/string.h>
#include <tp/arguments.h>
#include <tp/log.h>
#include <tp/module.h>
#include <tp/sort.h>

#include <cstdio>

int main(int argc,char* argv[])
{
	tpModuleManager::get()->load("glsurface,obj");

	tpArray<int> intA;
	intA.add(9); intA. add(2); intA.add(3); intA.add(1); intA.add(3);

	for (tpArray<int>::iterator i = intA.begin();
		 i != intA.end();
		 ++i)
	{
		tpLog::get().printf("%d ",(*i));
	}

	tpLog::get().printf("\n");

	tpSort(intA);

	for (tpArray<int>::iterator i = intA.begin();
		 i != intA.end();
		 ++i)
	{
		tpLog::get().printf("%d ",(*i));
	}

	tpLog::get().printf("\n");



	tpString file,plugins;
	tpArguments args(&argc,argv);

	if (args.get("--plugins",plugins)) {}
	if (args.get("--file",file)) {}

	return 0;
}
