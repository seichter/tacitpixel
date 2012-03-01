
#include <tp/string.h>
#include <tp/arguments.h>
#include <tp/log.h>
#include <tp/logutils.h>
#include <tp/module.h>
#include <tp/sort.h>
#include <tp/stringtokenizer.h>
#include <tp/node.h>
#include <tp/renderer.h>
#include <tp/window.h>
#include <tp/thread.h>
#include <tp/timer.h>
#include <tp/primitive.h>
#include <tp/vec.h>
#include <tp/light.h>
#include <tp/viewer.h>

#include <GL/glut.h>

int main(int argc, char* argv[])
{
	tpModuleManager::get()->load("3ds,obj");


	glutInit(argc,argv);
}