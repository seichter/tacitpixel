
#include <tp/factory.h>
#include <tp/node.h>
#include <tp/log.h>
#include <tp/image.h>
#include <tp/imageop.h>
#include <tp/library.h>
#include <tp/module.h>
#include <tp/rendersurface.h>
#include <tp/camera.h>

int main(int argc, char* argv[])
{
	
	tpRefPtr<tpLibrary> gllib = tpLibrary::load("tacit_gl");

	tpRefPtr<tpRenderSurface> rendersurface = tpRenderSurface::create();

	tpRefPtr<tpCamera> camera = new tpCamera;
	camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	camera->setClearColor(tpVec4f(0.1f,0.1f,0.1f,1.0f));

	if (rendersurface.isValid())
	{

		rendersurface->show(true);

		rendersurface->setCamera(camera.get());

		while (rendersurface->isDone() == false) {
			rendersurface->frame();
		}
	}

	tpModuleManager::get(true);
	
	return 0;
}