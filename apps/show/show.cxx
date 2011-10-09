
#include <tp/string.h>
#include <tp/arguments.h>
#include <tp/log.h>
#include <tp/module.h>

#include <cstdio>

int main(int argc,char* argv[])
{

	tpModuleManager::get()->load("glsurface,obj");

	tpString file,plugins;
	tpArguments args(&argc,argv);

	for (tpStringArray::const_iterator i = args.get().begin();
		 i != args.get().end();
		 ++i)
	{
		tpLog::get().printf("a %s\n",i->c_str());
	}

	if (args.get("--plugins",plugins)) {}
	if (args.get("--file",file)) {}


	tpLogNotify("files %s",file.c_str());
	tpLogNotify("plugins %s",plugins.c_str());

	return 0;
}
