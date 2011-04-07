#include <tp/log.h>
#include <tp/library.h>
#include <tp/system.h>

int main(int argc, char* argv[])
{
	
	tpLogMessage("Plugin Path: %s",tpSystem::get()->getPluginPath().c_str());
	tpLogMessage("Executable Path: %s",tpSystem::get()->getExecutablePath().c_str());
	tpLogMessage("Resource Path: %s",tpSystem::get()->getResourcePath().c_str());
	tpLogMessage("CWD: %s",tpSystem::get()->getCWD().c_str());
	
	
	return 0;
}