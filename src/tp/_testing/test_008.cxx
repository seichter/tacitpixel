#include <tp/vec.h>
#include <tp/logutils.h>
#include <tp/font.h>
#include <tp/module.h>



int main(int argc,char* argv[])
{
	tpModuleManager::get()->load("jpg");

	tpRefPtr<tpFont> font = new tpFont();

	font->load(argv[1]);

	font->getImage()->write("font.jpg");


	return 0;
}
