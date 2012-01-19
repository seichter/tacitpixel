
#include <tp/string.h>
#include <tp/arguments.h>
#include <tp/log.h>
#include <tp/module.h>
#include <tp/sort.h>
#include <tp/stringtokenizer.h>
#include <tp/node.h>
#include <tp/renderer.h>
#include <tp/rendersurface.h>

#include <cstdio>

int main(int argc,char* argv[])
{
	tpModuleManager::get()->load("gl,glsurface,glrenderer,obj");

	tpString file,plugins;
	tpArguments args(&argc,argv);

	if (args.get("--plugins",plugins)) {}
	if (args.get("--file",file)) {}

	tpRefNode root = tpNode::read(file);

	if (!root.isValid()) return -1;

	tpRenderSurfaceTraits traits;
	traits.setSize(640,480).setPosition(10,10).setTitle("Tacit Pixel 3");

	tpRefPtr<tpRenderer> renderer = tpRenderer::create();
	tpRefPtr<tpRenderSurface> rendersurface = tpRenderSurface::create(&traits);

	if (!renderer.isValid() || !rendersurface.isValid()) return -1;

	tpRefPtr<tpCamera> camera = renderer->getActiveCamera();

	camera->setProjectionPerspective(60.0f,1.3f,0.1f,1000.0f);
	camera->setViewLookAt(tpVec3r(2,2,2),tpVec3r(0,0,0),tpVec3r(0,1,0));
	camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);

	camera->setClearColor(tpVec4f(0.5f,0.5f,0.9f,1.0f));
	camera->setViewport(tpVec4i(0,0,640,480));

	if (rendersurface.isValid() && renderer.isValid())
	{
		rendersurface->show(true);
		while (rendersurface->isValid())
		{
//			if (rendersurface->makeCurrent())
			{
				(*renderer)(root.get());
			}
//			rendersurface->swapBuffers();
		}
	}

	return 0;
}
